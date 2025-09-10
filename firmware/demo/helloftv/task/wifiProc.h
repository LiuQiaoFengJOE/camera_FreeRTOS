#ifndef _WIFI_PROC_H
#define _WIFI_PROC_H
typedef unsigned int IPADDR_N_TYPE ;
 typedef struct _dev_server_ip_set
 {
 	 unsigned char	         enable;
	 IPADDR_N_TYPE			 server_ipaddr;
	 IPADDR_N_TYPE			 dhcpd_begin_ipaddr;
	 IPADDR_N_TYPE			 dhcpd_end_ipaddr;
 }dev_server_ip_set;

 typedef struct _dev_client_ip_set
 {
	 unsigned char	 		 dhcpc_enable;
	 IPADDR_N_TYPE			 client_ipaddr;
	 IPADDR_N_TYPE			 client_ipmask;
	 IPADDR_N_TYPE			 client_gateway;
 }dev_client_ip_set;


 typedef enum {
	CFG_WIFI_MODE_STA 		=0,
	CFG_WIFI_MODE_AP 		=1,
	CFG_WIFI_MODE_APSTA		=2,//promiscuous mode,
	CFG_WIFI_MODE_BUTT,
 
 }ENU_WIFI_MODE_SET;
	
typedef union _wifi_ip_set{
	 dev_client_ip_set client_ip_set;
	 dev_server_ip_set server_ip_set;
 }wifi_ip_set;

 typedef struct _dev_wifi_set{
	 unsigned char			enable;
	 ENU_WIFI_MODE_SET	 	mode;
	 char					ssid[64];
	 char					wifi_pw[64];
	 unsigned int 			re_cnn_timeout;
	 wifi_ip_set ip_set;

 }dev_wifi_set;
 
 typedef enum{
	 WIFI_CLOSE=0,
	 WIFI_OPEN_ERR,
	 WIFI_CLOSING,
	 WIFI_OPENING,
	 WIFI_OPEN_SUCCESS,
	 WIFI_RE_OPENING,
	
 }E_WIFI_STATUS;

 typedef enum{
	 DHCP_UNINIT=0,
	 DHCP_WAIT,
	 DHCPC_SUCCESS,
	 DHCPD_SUCCESS,
	 DHCP_ERR,
 }E_DHCP_STATUS;

//网络就绪返回1,进行中返回0,不支持WIFI返回-1
int get_wifi_statue(E_WIFI_STATUS*wifi_status,E_DHCP_STATUS*dhcp_status);

int wifi_init(dev_wifi_set*wifi_set);
int start_wifi(unsigned char force);
int wifi_stop();
int wifi_deinit();


char *get_wifi_ssid_string(void);
char *getWifiSSIDInfoString(void);
char *getWifiPwString(void);
char *getWifiConnectInfoString(void);


/*net work API*/
int set_ifconfig(struct netif *pnetif, dev_client_ip_set*client_ip_set);

//sdio init
int wifi_dev_init();

#endif



