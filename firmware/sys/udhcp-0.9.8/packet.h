#ifndef _PACKET_H
#define _PACKET_H

#include "typedef.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "board_config.h"

#if WIFI_SUPPORT_VERSION != BOARD_WIFI_SV6152
#define time_t uint32_t
#endif
#define udphdr udp_hdr

typedef u16 __sum16;
typedef u32 __wsum;

struct dhcpMessage
{
	u_int8_t op;
	u_int8_t htype;
	u_int8_t hlen;
	u_int8_t hops;
	u_int32_t xid;
	u_int16_t secs;
	u_int16_t flags;
	u_int32_t ciaddr;
	u_int32_t yiaddr;
	u_int32_t siaddr;
	u_int32_t giaddr;
	u_int8_t chaddr[16];
	u_int8_t sname[64];
	u_int8_t file[128];
	u_int32_t cookie;
	u_int8_t options[308]; /* 312 - cookie */
};
#if 1
struct iphdr
{
	__u8 ihl : 4,
		version : 4;
	__u8 tos;
	__be16 tot_len;
	__be16 id;
	__be16 frag_off;
	__u8 ttl;
	__u8 protocol;
	__sum16 check;
	__be32 saddr;
	__be32 daddr;
	/*The options start here. */
};
#endif

struct udp_dhcp_packet
{
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
};

void init_header(struct dhcpMessage *packet, char type);
int get_packet(struct dhcpMessage *packet, int fd);
// u_int16_t checksum(void *addr, int count);
u_int16_t checksum(uint16_t *addr, int nleft);
int raw_packet(struct dhcpMessage *payload, u_int32_t source_ip, int source_port,
			   u_int32_t dest_ip, int dest_port, unsigned char *dest_arp, int ifindex);
int kernel_packet(struct dhcpMessage *payload, u_int32_t source_ip, int source_port,
				  u_int32_t dest_ip, int dest_port);

u32 time(u32 *t);

#endif
