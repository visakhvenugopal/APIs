/*
 * -----------------------------------------------------------------------+
 * file		: extra/lgc_net_iface_info.h
 * date		: 02/31/2020
 * compiler	: gcc
 * author	: Visakh Venugopal [visakh.v@gadgeon.com]
 *
 * Implements system wide general APIs
 * -----------------------------------------------------------------------+
 * Copyright (c) 2020 By GadgEon Smart Systems Pvt. Ltd.
 */

#ifndef LGC_NET_IFACE_INFO_H
#define LGC_NET_IFACE_INFO_H

struct lgc_net_interface {
	int up;
	char interface[64];
	char ipv4_address[64];
	char ipv6_address[64];
};

int lgc_get_net_iface_info(struct lgc_net_interface *, const char *);

#endif
