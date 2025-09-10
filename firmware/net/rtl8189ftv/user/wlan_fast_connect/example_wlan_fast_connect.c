/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/

/** @file

   This example demonstrate how to implement wifi fast reconnection
**/
#include <autoconf.h>
#include <wlan_fast_connect/example_wlan_fast_connect.h>

#include "task.h"
#include <platform/platform_stdlib.h>
#include <wifi/wifi_conf.h>
#include "flash/stm32_flash.h"
#include "debug.h"

write_reconnect_ptr p_write_reconnect_ptr;

extern void fATW0(void *arg);
extern void fATW1(void *arg);
extern void fATW2(void *arg);
extern void fATWC(void *arg);
extern u8 *rtw_zmalloc(u32 sz);
extern void rtw_mfree(u8 *pbuf, u32 sz);

/*
 * Usage:
 *       wifi connection indication trigger this function to save current
 *       wifi profile in flash
 *
 * Condition:
 *       CONFIG_EXAMPLE_WLAN_FAST_CONNECT flag is set
 */

int wlan_wrtie_reconnect_data_to_flash(u8 *data, uint32_t len)
{
	struct wlan_fast_reconnect read_data = {0};
	if (!data)
		return -1;

	flash_Read(FAST_RECONNECT_DATA, (char *)&read_data, sizeof(struct wlan_fast_reconnect));
	// wirte it to flash if different content: SSID, Passphrase, Channel
	if (memcmp(data, (u8 *)&read_data, sizeof(struct wlan_fast_reconnect)) != 0)
	{
		printf("\r\n %s():not the same ssid/passphrase/channel, write new profile to flash", __func__);
		printf("\r\n %s():not the same ssid/passphrase/channel, need to flash it", __func__);
#if defined STM32F2XX || STM32F4XX
		flash_EraseSector(FAST_RECONNECT_SECTOR);
#elif defined(STM32f1xx)
		flash_EraseSector(FAST_RECONNECT_DATA);
#endif
		flash_Wrtie(FAST_RECONNECT_DATA, (char *)data, len);
	}

	return 0;
}

/*
 * Usage:
 *       After wifi init done, waln driver call this function to check whether
 *       auto-connect is required.
 *
 *       This function read previous saved wlan profile in flash and execute connection.
 *
 * Condition:
 *       CONFIG_EXAMPLE_WLAN_FAST_CONNECT flag is set
 */
int wlan_init_done_callback()
{
	struct wlan_fast_reconnect *data;
	uint32_t channel;
	char key_id;

	data = (struct wlan_fast_reconnect *)rtw_zmalloc(sizeof(struct wlan_fast_reconnect));
	flash_Read(FAST_RECONNECT_DATA, (char *)data, sizeof(struct wlan_fast_reconnect));
	if (*((uint32_t *)data) != ~0x0)
	{
		memcpy(psk_essid, data->psk_essid, sizeof(data->psk_essid));
		memcpy(psk_passphrase, data->psk_passphrase, sizeof(data->psk_passphrase));
		memcpy(wpa_global_PSK, data->wpa_global_PSK, sizeof(data->wpa_global_PSK));
		memcpy(&channel, &(data->channel), 4);
		sprintf(&key_id, "%d", (channel >> 28));
		channel &= 0xff;

		// set partial scan for entering to listen beacon quickly
		wifi_set_pscan_chan((uint8_t *)&channel, 1);

#ifdef CONFIG_AUTO_RECONNECT
		// wifi_set_autoreconnect(1);
#endif
		// set wifi connect
		// open mode
		if (!strlen((char *)psk_passphrase))
		{
			fATW0((char *)psk_essid);
		}
		// wep mode
		else if (strlen((char *)psk_passphrase) == 5 || strlen((char *)psk_passphrase) == 13)
		{
			fATW0((char *)psk_essid);
			fATW1((char *)psk_passphrase);
			fATW2(&key_id);
		}
		// WPA/WPA2
		else
		{
			sprintf(&key_id, "%d", -1);
			fATW2(&key_id);
			fATW0((char *)psk_essid);
			fATW1((char *)psk_passphrase);
		}
		fATWC(NULL);
	}
	if (data)
		rtw_mfree((u8 *)data, sizeof(struct wlan_fast_reconnect));
	return 0;
}

void example_wlan_fast_connect()
{
	// Call back from wlan driver after wlan init done
	p_wlan_init_done_callback = wlan_init_done_callback;

	// Call back from application layer after wifi_connection success
	p_write_reconnect_ptr = wlan_wrtie_reconnect_data_to_flash;
}
