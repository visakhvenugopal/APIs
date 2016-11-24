#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

struct net_interface {
	int up;
	char interface[64];
	char ipv4_address[64];
	char ipv6_address[64];
};

int main(int argc, char *argv[])
{
	struct net_interface nt = {0, "NILL", "NILL", "NILL"};

	get_inet_address(&nt);
	if(nt.up == 1)
		printf("%s\n\t%s\n\t%s\n", nt.interface, nt.ipv4_address, nt.ipv6_address);
	else
		printf("\tNo ethernet interface connacted.!\n");

}

int get_inet_address(struct net_interface *nt)
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s, n;
	unsigned int net_flags;
	char host[NI_MAXHOST];
	char interface_name[64];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	/* Walk through linked list, maintaining head pointer so we
	   can free list later */

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;
		net_flags = 0;
		net_flags |= ifa->ifa_flags;

		/* Display interface name and family (including symbolic
		   form of the latter for the common families) */
		if((net_flags & IFF_UP) && (family == AF_INET || family == AF_INET6) && strstr(ifa->ifa_name, "eth") != NULL) {

			strcpy(interface_name, ifa->ifa_name);
			strcpy(nt->interface, ifa->ifa_name);
			for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
				if (ifa->ifa_addr == NULL)
					continue;
				family = ifa->ifa_addr->sa_family;

				if((family == AF_INET || family == AF_INET6) && strcmp(ifa->ifa_name, interface_name) == 0) {

					/* For an AF_INET* interface address, display the address */
					s = getnameinfo(ifa->ifa_addr,
							(family == AF_INET) ? sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6),
							host, NI_MAXHOST,
							NULL, 0, NI_NUMERICHOST);
					if (s != 0) {
						printf("getnameinfo() failed: %s\n", gai_strerror(s));
						return -1;
					}

					if(family == AF_INET) {
						strcpy(nt->ipv4_address, host);
					} else if(family == AF_INET6) {
						strcpy(nt->ipv6_address, host);
					}
					nt->up = 1;		
				}
			}
			freeifaddrs(ifaddr);
			return 0;
		}
	}
	return 0;	
}
