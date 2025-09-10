#include "debug.h"
#include "asm/osal_socket.h"
#include "netif/etharp.h"
#include "board_config.h"

#define ISSOCKHANDLE(x) (x != INVALID_SOCKET)

SOCK_HDL osal_socket_open(int type)
{
  int protocal = 0;

  if (type == SOCK_STREAM)
    protocal = IPPROTO_TCP;
  else if (type == SOCK_DGRAM)
    protocal = IPPROTO_UDP;

  return socket(AF_INET, type, protocal);
}

void osal_socket_close(SOCK_HDL fd)
{
  if (ISSOCKHANDLE(fd))
  {
    closesocket(fd);
  }
  else
    printf("socket close fial\n");
}

int osal_set_tcp_nodelay(SOCK_HDL fd, int o)
{
  BOOL opt = o;
  return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof opt);
}

int osal_set_reuseaddr(SOCK_HDL fd, int o)
{
  BOOL opt = o;
  return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof opt);
}
int osal_set_tcp_nolinger(SOCK_HDL fd)
{
	struct linger d;
	d.l_onoff=1;
	d.l_linger=0;
	return setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&d, sizeof(struct linger));
}
int osal_socket_block(SOCK_HDL fd, int b)
{
  if (b)
  {
    p_warn("LwIP can only support setting nonblock\r\n");
    return -1;
  }
  else
  {
    return fcntl(fd, F_SETFL, O_NONBLOCK);
  }
}

void osal_socket_getmac(SOCK_HDL fd, char *mac)
{
  struct sockaddr_in dstadd_in;
  int len = sizeof(struct sockaddr_in);
  unsigned long llen;
  struct netif *netif;
  ip_addr_t naddr;
  struct eth_addr *eth_ret;

  if (getpeername(fd, (struct sockaddr *)&dstadd_in, &len) < 0)
  {
    p_warn("Can't not getpeername from fd %d\r\n", (int)fd);
    return;
  }

  /* 1. 如果ip4_route不可用，则要使用全局变量
   * 2. dstadd_in.sin_addr.s_addr严格来说不是ip_addr_t，可能需要使用get_socket/netconn_peer来获取
   */
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV || WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
  netif = ip4_route(&dstadd_in.sin_addr.s_addr);
#endif

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152) ||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158) // 支持sv6030
 // netif = ip_route(&dstadd_in.sin_addr.s_addr);
#endif

  if (etharp_find_addr(netif, &dstadd_in.sin_addr.s_addr, &eth_ret, &naddr) < 0)
  {
    p_warn("etharp_find_addr error\r\n");
    return;
  }

  memcpy(mac, eth_ret->addr, 6);
  mac[6] = mac[7] = 0;
}
