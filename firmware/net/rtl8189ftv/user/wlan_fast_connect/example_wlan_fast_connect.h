#ifndef __EXAMPLE_FAST_RECONNECTION_H__
#define __EXAMPLE_FAST_RECONNECTION_H__

/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#include "FreeRTOS.h"
#include <autoconf.h>

#if defined(STM32F2XX)
#include "stm32f2xx.h"
#elif defined(STM32F4XX)
#include "stm32f4xx.h"
#elif defined(STM32f1xx)
#include "stm32f10x.h"

#endif
#define FAST_RECONNECT_DATA (0x080E0000 - 0x1000)
#if defined STM32F2XX || STM32F4XX
#define FAST_RECONNECT_SECTOR FLASH_Sector_10
#endif

#define NDIS_802_11_LENGTH_SSID 32
#define A_SHA_DIGEST_LEN 20
#define IW_PASSPHRASE_MAX_SIZE 64

typedef int (*wlan_init_done_ptr)(void);
typedef int (*write_reconnect_ptr)(uint8_t *data, uint32_t len);

struct wlan_fast_reconnect
{
	unsigned char psk_essid[NDIS_802_11_LENGTH_SSID + 4];
	unsigned char psk_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
	unsigned char wpa_global_PSK[A_SHA_DIGEST_LEN * 2];
	uint32_t channel;
};

// Variable
extern unsigned char psk_essid[NET_IF_NUM][NDIS_802_11_LENGTH_SSID + 4];
extern unsigned char psk_passphrase[NET_IF_NUM][32];
extern unsigned char wpa_global_PSK[NET_IF_NUM][A_SHA_DIGEST_LEN * 2];
// Function
extern wlan_init_done_ptr p_wlan_init_done_callback;
extern write_reconnect_ptr p_write_reconnect_ptr;

void example_wlan_fast_connect(void);

#endif // #ifndef __EXAMPLE_FAST_RECONNECTION_H__
