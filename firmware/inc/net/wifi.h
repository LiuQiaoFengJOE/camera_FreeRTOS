#ifndef _WIFI_H_
#define _WIFI_H_

#include "board_config.h"

typedef enum
{
	KEY_NONE = 0,
	KEY_WEP,
	KEY_WPA,
	KEY_WPA2,
	KEY_MAX_VALUE
} SECURITY_TYPE;

void wifi_create_ap(char *essid, char *password, int key_type, int channel, int mac_on);

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152) ||(WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158)// 支持sv6030

#define is_broadcast_ether_addr(addr) (((addr)[0] & (addr)[1] & (addr)[2] & (addr)[3] & (addr)[4] & (addr)[5]) == 0xff)

// #define  is_zero_ether_addr(addr)  (!((addr)[0] | (addr)[1] | (addr)[2] | (addr)[3] | (addr)[4] | (addr)[5]))

#define is_multicast_ether_addr(addr) (0x01 & (addr)[0])

#define is_local_ether_addr(addr) (0x02 & (addr)[0])

#define is_unicast_ether_addr(addr) !(is_multicast_ether_addr)

#define is_valid_ether_addr(addr) (!is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr))
#endif

#endif
