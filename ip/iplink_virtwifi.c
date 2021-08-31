/*
 * iplink_virtwifi.c	virt_wifi device support
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Authors:     Andrew Beltrano <anbeltra@microsoft.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rt_names.h"
#include "utils.h"
#include "ip_common.h"

static void print_explain(FILE *f)
{
	fprintf(f,
		"Usage: ... virt_wifi\n"
		"		[ lower { DEVICE } ]\n"
		"\n"
	);
}

static void explain(void)
{
	print_explain(stderr);
}

static int virt_wifi_parse_opt(struct link_util *lu, int argc, char **argv,
			  struct nlmsghdr *n)
{
	char *lower = NULL;

	while (argc > 0) {
		if (matches(*argv, "lower") == 0) {
			NEXT_ARG();
			lower = *argv;
		} else if (matches(*argv, "help") == 0) {
			explain();
			return -1;
		} else {
			fprintf(stderr, "virt_wifi: unknown command \"%s\"?\n", *argv);
			explain();
			return -1;
		}
		argc--, argv++;
	}

	if (lower) {
		int ifindex;

		ifindex = ll_name_to_index(lower);
		if (!ifindex)
			return nodev(lower);
		addattr32(n, 1024, IFLA_VIRTWIFI_LOWER, ifindex);
	}

	return 0;
}

static void virt_wifi_print_opt(struct link_util *lu, FILE *f, struct rtattr *tb[])
{
	if (!tb)
		return;

	if (tb[IFLA_VIRTWIFI_LOWER]) {
		int iflink = rta_getattr_u32(tb[IFLA_VIRTWIFI_LOWER]);
		fprintf(f, "link %u: %s\n", iflink, ll_index_to_name(iflink));
	}
}

static void virt_wifi_print_help(struct link_util *lu, int argc, char **argv,
			    FILE *f)
{
	print_explain(f);
}

struct link_util virt_wifi_link_util = {
	.id			= "virt_wifi",
	.maxattr	= IFLA_VIRTWIFI_MAX,
	.parse_opt	= virt_wifi_parse_opt,
	.print_opt	= virt_wifi_print_opt,
	.print_help	= virt_wifi_print_help,
};
