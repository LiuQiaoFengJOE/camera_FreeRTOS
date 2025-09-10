

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define MEM_LIBC_MALLOC 1
#define mem_clib_free vPortFree
#define mem_clib_malloc pvPortMalloc
#define mem_clib_calloc(c, s) pvPortMalloc((c) * (s))

#define MEMP_MEM_MALLOC 1
#define MEM_ALIGNMENT 4
#define MEMP_NUM_NETCONN 64
#define MEMP_NUM_TCP_PCB 32

#define LWIP_COMPAT_MUTEX 1
#define LWIP_COMPAT_MUTEX_ALLOWED 1

#define LWIP_DNS 1
#define DNS_SERVER_ADDRESS(ipaddr) (ip4_addr_set_u32(ipaddr, ipaddr_addr("208.67.222.222")))
#define LWIP_IPV4 1
#define LWIP_DHCP 1
#define LWIP_IGMP 1

#define TCPIP_THREAD_PRIO 30
#define TCPIP_THREAD_STACKSIZE 1024
#define TCP_LISTEN_BACKLOG 1
#define TCPIP_MBOX_SIZE 160
#define DEFAULT_UDP_RECVMBOX_SIZE 100
#define DEFAULT_TCP_RECVMBOX_SIZE 60
#define DEFAULT_ACCEPTMBOX_SIZE 10
#define LWIP_TCPIP_TIMEOUT 1
#define LWIP_TCP_KEEPALIVE 1
#define SO_REUSE 1
#define LWIP_TCPIP_CORE_LOCKING 0
#define LWIP_NETIF_LOOPBACK 0
#define DEFAULT_RAW_RECVMBOX_SIZE    5


//#define LWIP_SO_LINGER 1

// #define ETHARP_DEBUG                    LWIP_DBG_ON
// #define IP_DEBUG                        LWIP_DBG_ON
// #define UDP_DEBUG                       LWIP_DBG_ON
// #define TCPIP_DEBUG                     LWIP_DBG_ON
// #define SYS_DEBUG                       LWIP_DBG_ON
// #define TIMERS_DEBUG                    LWIP_DBG_ON

// #define TCP_DEBUG                       LWIP_DBG_ON
// #define TCP_INPUT_DEBUG                 LWIP_DBG_ON
// #define TCP_OUTPUT_DEBUG                LWIP_DBG_ON

// #define LWIP_HOOK_UNKNOWN_ETH_PROTOCOL(p,n) ({printf("\xA1\xBE%s\xA1\xBFLWIP_HOOK_UNKNOWN_ETH_PROTOCOL\r\n", pcTaskGetName(NULL));1;})

// #define LWIP_HOOK_IP4_ROUTE(dest) lwip_hook_ip4_route(dest)

#endif /* __LWIPOPTS_H__ */
