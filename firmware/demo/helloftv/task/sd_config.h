#ifndef __SD_CONFIG_H__
#define __SD_CONFIG_H__
/*
{
  "wifi_set": {
    "mode": "STA",
    "ssid": "ipc_test",
    "password": "123456789",
    "re_cnn_timeout":120,
    "tmp_set": 1
  },
  "ip_set": {
    "enable": 1,
    "server_ipaddr": "192.168.66.1",
    "dhcpd_begin_ipaddr": "192.168.66.2",
    "dhcpd_end_ipaddr": "192.168.66.128",
    "dhcpc_enable": 1,
    "client_ipaddr": "192.168.88.199",
    "ipmask": "255.255.255.0",
    "gateway": "192.168.88.1",
    "tmp_set": 1
  },
  "iperf_set": {
    "enable": 1,
    "server_mode": 0,
    "server_ipaddr": "192.168.88.25",
    "server_port": 5001,
    "udp_mode": 0,
    "send_bps": 20,
    "time_duration": 60,
    "info_printf_duration": 1
  }
}

*/


typedef enum {
	SD_CFG_WIFI_MODE_STA		    =0,
	SD_CFG_WIFI_MODE_AP				=1,
	SD_CFG_WIFI_MODE_BUTT,

}ENU_SD_WIFI_MODE_SET;


typedef struct _sd_wifi_set{
	unsigned char 			enable;
	ENU_SD_WIFI_MODE_SET 	mode;
	char 					ssid[64];
	char 					wifi_pw[64];
	unsigned int 			re_cnn_timeout;//|¨¬?????¡§¡é??¨º?3?¡§o?¨¤¡§o?¨¤??,|¨¬¡ê¡è??s
	unsigned char 			tmp_set;
}sd_wifi_set;


typedef struct _sd_ip_set
{
    unsigned char 	enable;
    char 			server_ipaddr[32];
    char 			dhcpd_begin_ipaddr[32];
    char 			dhcpd_end_ipaddr[32];
    unsigned char 	dhcpc_enable;
    char 			client_ipaddr[32];
    char 			client_ipmask[32];
    char 			client_gateway[32];
    unsigned char 	tmp_set;
}sd_ip_set;

typedef struct _sd_iperf_set
{
	unsigned char 	enable;
	unsigned char 	server_mode;
	char 			server_ipaddr[32];
	unsigned int 	server_port;
	unsigned char 	udp_mode;
	unsigned int 	send_bps;
	unsigned char 	time_duration;
	unsigned short 	info_printf_duration;
}sd_iperf_set;

//success 1
int check_sd_config_file();
sd_iperf_set* get_sd_iperf_config();
sd_ip_set* get_sd_ip_config();
sd_wifi_set* get_sd_wifi_config();



#endif
