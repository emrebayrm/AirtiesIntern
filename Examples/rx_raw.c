#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <errno.h>
#include <libgen.h>
#include <time.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

#include "myether.h"
/*****************************************************************************
 * C03:	  PRIVATE CONSTANT DEFINITIONS
 *****************************************************************************/
#define MAX_IFNAMSIZ 32
#define ETHER_TYPE   ((u_int16_t) 0x1234)
#define RX_BUF_SIZE	1600
/*****************************************************************************
 * C04:   PRIVATE DATA TYPES
 *****************************************************************************/
struct rx_pack_hdr {
	struct ether_header eth;
	char data[0];
}__attribute__((packed));
/*****************************************************************************
 * C05:   PRIVATE DATA DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * C06:   PRIVATE (LOCAL) FUNCTION PROTOTYPES
 *****************************************************************************/
static void usage();
/*****************************************************************************
 * C07:   GLOBAL DATA DECLARATIONS
 *****************************************************************************/

/*****************************************************************************
 * C08:   GLOBAL FUNCTIONS
 *****************************************************************************/

int main(int argc, char *argv[])
{
	int sfd;
	char ifname[MAX_IFNAMSIZ] = {0};
	int ret;
	//struct timespec sleep_time;
	struct rx_pack_hdr *hdr;
	char *buffer;
	char *arg_ifname;

	if (argc != 2) {
		usage();
		goto bail;
	}

	arg_ifname = argv[1];

	snprintf(ifname, MAX_IFNAMSIZ, "%s", arg_ifname);

	if (!net_device_up(ifname)) {
		fprintf(stderr, "%s is not up\n", ifname);
		goto bail;
	}

	sfd = net_create_raw_socket(ifname, ETHER_TYPE, 0);
	if (sfd == -1) {
		fprintf(stderr, "failed to init socket\n");
		goto bail;
	}

	buffer = malloc(RX_BUF_SIZE);
	if (!buffer) {
		fprintf(stderr, "memory allocation error!\n");
		goto bail;
	}
	hdr = (struct rx_pack_hdr*)buffer;

	while (1) {
		ret = recv(sfd, hdr, RX_BUF_SIZE, 0);
		if (ret <= 0) {
			fprintf(stderr, "ERROR: recv failed ret: %d, errno: %d\n", ret, errno);
			goto bail;
		}
		fprintf(stderr, "%d bytes received\n", ret);
		fprintf(stderr, "data: %s\n", hdr->data);
	}

	return 0;

bail:
	return -1;
}
/*****************************************************************************
 * C09:   PRIVATE FUNCTIONS
 *****************************************************************************/
static void usage()
{
	fprintf(stderr, "\nUsage:\n./rx_raw <ifname>\n");
	fprintf(stderr, "Example:\n./rx_raw eth0\n");
}
/*****************************************************************************
 * C10:   END OF CODE
 *****************************************************************************/
