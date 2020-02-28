/*
 * -----------------------------------------------------------------------+
 * file		: main.c
 * date		: 02/31/2020
 * compiler	: gcc
 * author	: Visakh Venugopal [visakh.vinayak@gmail.com]
 *
 * Entry point
 * -----------------------------------------------------------------------+
 */

#include <stdio.h>
#include "lgc_net_iface_info.h"

int main(int argc, char *argv[])
{
	struct lgc_net_interface nt = {0, "NILL", "NILL", "NILL"};

	lgc_get_net_iface_info(&nt, "enp2s0");
	if(nt.up == 1)
		printf("%s\n\t%s\n\t%s\n", nt.interface, nt.ipv4_address, nt.ipv6_address);
	else
		printf("\tNo ethernet interface connacted.!\n");

}

