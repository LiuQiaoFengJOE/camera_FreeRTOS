
#include "typedef.h"
#include "wifi.h"
#include "wifi/wifi_conf.h"
#include <wifi/wifi_util.h>
#include <platform/platform_stdlib.h>
#include "main.h"

u32 FTV_channl;

// #define COB_MODULE

/* startup ap */
int wifiGetChannel(void)
{
	return FTV_channl;
}
void wifi_create_ap(char *essid, char *password, int key_type, int channel, int mac_on)
{

	int ret, i, len;
	u8 ssid[40], mac[6];

	u8 wifiMAC_tmp[13];
	u32 mac_set;
	// wifiMAC_tmp = wifiMAC;

	wifi_on(RTW_MODE_AP);
	if (channel == 15)
	{
		FTV_channl = wifi_autochl_select(1, 11);
	}
	else
	{

		FTV_channl = channel;
	}

	// FTV_channl = 11;

	printf("---select channle---- : %d\n", FTV_channl);
	printf("essid:%s\n", essid);

	if (mac_on)
	{

		u8 mac_ftv[20], mac_ftv_tmp[20];

#ifdef COB_MODULE
		u8 *wifiMAC = get_wifiMAC();
		if (wifiMAC[0] == 0)
		{
			// struct setJson *sensorSetJson = get_sensor_setJson();
			printf("rtl ftv not have mac\n");
			mac_set = rand();
			printf("mac_set:%x\n", mac_set);
			for (i = 0; i < 8; i++)
			{
				if (((mac_set & 0xf) >= 0) && ((mac_set & 0xf) <= 9))
				{
					wifiMAC_tmp[i] = (mac_set & 0xf) + '0';
				}

				if (((mac_set & 0xf) >= 10) && ((mac_set & 0xf) <= 15))
				{
					wifiMAC_tmp[i] = (mac_set & 0xf) + 'a' - 10;
				}

				mac_set = mac_set >> 4;
			}
			osal_task_sleep(10);
			mac_set = rand();
			// mac_set = mac_set<<16;
			printf("mac_set:%x\n", mac_set);
			for (i = 8; i < 12; i++)
			{
				if (((mac_set & 0xf) >= 0) && ((mac_set & 0xf) <= 9))
				{
					wifiMAC_tmp[i] = (mac_set & 0xf) + '0';
				}

				if (((mac_set & 0xf) >= 10) && ((mac_set & 0xf) <= 15))
				{
					wifiMAC_tmp[i] = (mac_set & 0xf) + 'a' - 10;
				}

				mac_set = mac_set >> 4;
			}
			wifiMAC_tmp[12] = '\0';

			for (i = 0; i < 6; i++)
			{
				mac_ftv_tmp[0 + 2 * i] = wifiMAC_tmp[0 + i * 2];
				mac_ftv_tmp[1 + 2 * i] = wifiMAC_tmp[1 + i * 2];
			}

			// setJson_valueString(sensorSetJson,WIFIMAC,wifiMAC);	//保存wifiMAC
			// save_sensor_setJson();
		}
		else
		{
			printf("rtl ftv have mac");
			for (i = 0; i < 6; i++)
			{
				mac_ftv_tmp[0 + 2 * i] = wifiMAC[0 + i * 2];
				mac_ftv_tmp[1 + 2 * i] = wifiMAC[1 + i * 2];
			}
		}
#else
		wifi_get_mac_address(mac_ftv);

		for (i = 0; i < 6; i++)
		{
			mac_ftv_tmp[0 + 2 * i] = mac_ftv[0 + i * 3];
			mac_ftv_tmp[1 + 2 * i] = mac_ftv[1 + i * 3];
		}
#endif
		memset(mac_ftv, 0, 20);
		for (i = 0; i < 6; i++)
		{
			if ((mac_ftv_tmp[2 * i + 1] <= '9') && (mac_ftv_tmp[2 * i + 1] >= '0'))
			{
				mac_ftv[i] = mac_ftv_tmp[2 * i + 1] - '0';
			}
			else if ((mac_ftv_tmp[2 * i + 1] <= 'f') && (mac_ftv_tmp[2 * i + 1] >= 'a'))
			{
				mac_ftv[i] = mac_ftv_tmp[2 * i + 1] - 'a' + 10;
			}

			if ((mac_ftv_tmp[2 * i] <= '9') && (mac_ftv_tmp[2 * i] >= '0'))
			{
				mac_ftv[i] |= ((mac_ftv_tmp[2 * i] - '0') << 4);
			}
			else if ((mac_ftv_tmp[2 * i] <= 'f') && (mac_ftv_tmp[2 * i] >= 'a'))
			{
				mac_ftv[i] |= ((mac_ftv_tmp[2 * i] - 'a' + 10) << 4);
			}
		}
		len = strlen(essid);
		memcpy(ssid, essid, len);
		ssid[len] = '_';
		sprintf(&ssid[++len], "%02x%02x%02x%02x%02x%02x", mac_ftv[0], mac_ftv[1], mac_ftv[2], mac_ftv[3], mac_ftv[4], mac_ftv[5]);
		printf("  %x%x%x%x%x%x\n", mac_ftv[0], mac_ftv[1], mac_ftv[2], mac_ftv[3], mac_ftv[4], mac_ftv[5]);
		ssid[len + 12] = '\0';
		len = strlen(ssid);
	}
	else
	{
		len = strlen(essid);
		memcpy(ssid, essid, len);
		ssid[len] = '\0';
	}
	printf("ssid:%s\n", ssid);

	if (key_type != KEY_NONE)
	{
		ret = wifi_start_ap(ssid,
							RTW_SECURITY_WPA2_AES_PSK,
							password,
							strlen(ssid),
							strlen(password),
							FTV_channl); // AP_DEFAULT_CH
	}
	else
	{
		ret = wifi_start_ap(ssid,
							RTW_SECURITY_OPEN,
							NULL,
							strlen(ssid),
							0,
							FTV_channl); // AP_DEFAULT_CH
	}

	if (ret == 0)
	{
		printf("Creat AP ok \n ");
		memcpy(essid, ssid, strlen(ssid) + 1);
		//				err_led_even |= SDIO_OK|WIFI_OK;
	}
	else
	{
		//				err_led_even |=WIFI_ERR_LED;
	}
}

int wifi_create_station(char *essid, char *password)
{
	int ret;
	char passwd[50];
	int ssid_len = strlen(essid);
	int buf_len = 1024;
	int pw_len = 0;

	wifi_on(RTW_MODE_STA);

	
	wifi_scan_networks_with_ssid(NULL, &buf_len, essid, ssid_len);
	printf("wifi scan over\n");
	
	rtw_security_t key_type = RTW_SECURITY_OPEN;
	if(password != NULL){
		pw_len = strlen(password);
		if(0 != pw_len){
			key_type = RTW_SECURITY_WPA2_AES_PSK;
		}	
	}
	
	printf("wifi 8189 connect ssid:%s,security:0x%x,pwd:%s,ssid_len:%d,pw_len:%d\n",\
							essid, key_type, password, ssid_len, pw_len);
	
	rtw_network_info_t wifi = {0};
	wifi.security_type = key_type; 
	strcpy((char *)wifi.ssid.val, essid);
	wifi.password = password;
	wifi.password_len = pw_len;
	wifi.ssid.len = ssid_len;
	ret = wifi_connect((char *)wifi.ssid.val,
					   wifi.security_type,
					   (char *)wifi.password,
					   wifi.ssid.len,
					   wifi.password_len,
					   wifi.key_id,
					   NULL);

	printf("wifi_connect:%d\n", ret);
	return ret;
	// LwIP_DHCP(0,0);
}
