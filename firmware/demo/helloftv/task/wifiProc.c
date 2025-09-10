#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"

#include "fatfs/ff.h"

// #include "hal_auadc.h"
#include "http/http.h"
#include "sys/sys.h"
#include "stdlib.h"
#include "sd_config.h"

#include "wifi.h"

#include "wifiProc.h"
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
#include "host_apis.h"
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
#include "atbm_wifi_driver_api.h"
#include "atbm_hal.h"
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
#include "zt_typedef.h"
#include "wlan_api.h"
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV

#endif
	

#include "mmc/core/sdio.h"



#include "osal/osal_os.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "config.h"
#include "app.h"


#include "eventTimer/eventTimer.h"
#include "sysCommon/commonInclude.h"
#include "mac_fd/mac_fd.h"

#define IPERF_TEST_SET 0
#if IPERF_TEST_SET
#include "iperf2_adapt.h"
#endif

#include "udhcp-0.9.8/dhcpd.h"
#include "udhcp-0.9.8/dhcpc.h"
#include "cjson/cJSON.h"
//#include "ledProc.h"
#include "sys/btcom/btcom_user.h"

#include "uiWin.h"

#define WIFI_PROC_LOG(msg,...) \
	do{ \
		printf("[ %s, %d]",__func__,__LINE__); \
		printf(msg,##__VA_ARGS__); \
		printf("\r\n"); \
	}while(0)


#define LOCAL_DHCP_STOP             (uint8_t)0   //added by yue

#define LOCAL_DHCP_START            (uint8_t)1
#define LOCAL_DHCP_WAIT_ADDRESS     (uint8_t)2
#define LOCAL_DHCP_ADDRESS_ASSIGNED (uint8_t)3
#define LOCAL_DHCP_TIMEOUT          (uint8_t)4
#define LOCAL_DHCP_LINK_DOWN        (uint8_t)5
#define LOCAL_DHCP_OFF              (uint8_t)6

#define MAX_DHCP_TRIES        (uint8_t)200


 typedef enum
 {
	WIFI_OPT_EVENT_UNINIT,
	WIFI_OPT_EVENT_STA_CONNECTING,
	WIFI_OPT_EVENT_STA_CONNECT_FAIL,
	WIFI_OPT_EVENT_STA_CONNECT_SUCCESS,
	WIFI_OPT_EVENT_AP_STARTING,
	WIFI_OPT_EVENT_AP_START_FAIL,
	WIFI_OPT_EVENT_AP_START_SUCCESS,
	WIFI_OPT_EVENT_STA_DHCPC_STARTING,
	WIFI_OPT_EVENT_STA_DHCPC_START_FAIL,
	WIFI_OPT_EVENT_STA_DHCPC_START_SUCCESS,
	WIFI_OPT_EVENT_AP_DHCPD_START_SUCCESS,
	WIFI_OPT_EVENT_AP_DHCPD_START_FAIL,
	WIFI_OPT_EVENT_STOPING,
	WIFI_OPT_EVENT_STOP_SUCCESS,
	WIFI_OPT_EVENT_RUNNING,
	
 }E_DEV_WIFI_OPT_EVENT;


typedef struct _dev_wifi_context{
	
	volatile uint8_t wifi_run;
	volatile uint8_t wifi_status;
	volatile uint8_t dhcp_status;
	volatile uint8_t dhcp_run;

	volatile uint8_t wifi_task_run;
	volatile uint8_t wifi_task_runing;
	volatile uint8_t wifi_switch;
	volatile uint8_t b_scaned;
	volatile uint8_t sdio_run;
	volatile uint8_t low_power;
	dev_wifi_set*wifi_set;
	volatile uint32_t disconnect_time;
	volatile uint32_t re_connect_idx;

	char s_wifi_ssid_info_str[64];
	char s_wifi_pw_str[30];
	char s_wifi_connect_info_str[128];
	char s_wifi_ip_str[30];
}dev_wifi_context;

extern void osal_task_sleep(UINT32 ms);

extern uint8_t *getWifiSSID(void);
extern uint8_t *getWifiPW(void);
extern uint8_t *getWifiMAC(void);

extern void setWifiSSID(uint8_t*);
extern void setWifiPWD(uint8_t*);
extern u8 appGetSdStatus(void);


int wifi_dev_init_sta=0;

u32 g_ticks = 0;


dev_wifi_set g_dev_wifi_set={0};

static dev_wifi_context s_wifi_context = {
	.sdio_run = 0,
	.low_power = 0,
	.wifi_run			 = 0,
	.dhcp_status		 = DHCP_UNINIT,
	.dhcp_run			 = 0,

	.wifi_task_run		 = 0,
	.wifi_task_runing	 = 0,
	.wifi_switch		 = 0,
	.b_scaned			 = 0,
	
	.wifi_status = WIFI_CLOSE,
	.wifi_set=&g_dev_wifi_set,
};
static u32 lastRemainSize;

extern err_t ethernetif_init(struct netif *netif);
extern struct netif xnetif[];
struct netif *p_netif = NULL;
// struct netif xnetif[NET_IF_NUM]; /* network interface structure */
struct netif xnetif[2]; /* network interface structure */
extern err_t tcpip_input(struct pbuf *p, struct netif *inp);

void tcpip_init_end(void *pdata)
{
	WIFI_PROC_LOG("%s SP = %X\n", __FUNCTION__, ({uint32_t sp;asm("l.ori %0,r1,0":"=r"(sp));sp; }));
}
u32 getIPaddr(void)
{
	return inet_addr("192.168.66.1");
}
char*get_ssid_by_mac(char*prefix,char*mac)
{
	static char ssid[64];
	memset(ssid,0,64);
	strcpy(ssid,prefix);
	int len = strlen(prefix);
	ssid[len] = '_';
	sprintf(&ssid[++len], "%s", mac);
	return ssid;
}

int lwip_netif_init()
{
	ip_addr_t ipaddr, netmask, gw;

	gw.addr = getIPaddr();
	ipaddr.addr = getIPaddr();
	netmask.addr = inet_addr("255.255.255.0");

	if (p_netif)
		netif_remove(p_netif);

	p_netif = &xnetif[0];
	if (netif_add(p_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input) == 0)
	{
		p_err("netif_add faild \r\n");
		return -1;
	}
	WIFI_PROC_LOG("enter netif_add , inputfun : %x\n", tcpip_input);

	netif_set_default(p_netif);
	netif_set_up(p_netif);
	return 0;
}

/*当STA连接失败发生时,可能回退到AP模式，并初始化flash等配置*/
void set_ap_ip_default()
{
	set_network_cfg(NULL);
	network_config*network_set = get_network_cfg();

	g_dev_wifi_set.enable = 1;
	g_dev_wifi_set.mode =  network_set->mode;
	g_dev_wifi_set.ip_set.server_ip_set.server_ipaddr = network_set->ip_addr0;
	g_dev_wifi_set.ip_set.server_ip_set.dhcpd_begin_ipaddr = network_set->ip_addr1;
	g_dev_wifi_set.ip_set.server_ip_set.dhcpd_end_ipaddr = network_set->ip_addr2;

}

//从flash 中获取默认的配置参数
static void get_wifi_default_set()
{
	uint8_t *wifiSSID = getWifiSSID();
	uint8_t *wifiPW = getWifiPWD();
	WIFI_PROC_LOG("wifiSSID:%s,wifiPW:%s",(char*)wifiSSID,(char*)wifiPW);

	network_config*network_set = get_network_cfg();

	g_dev_wifi_set.enable 	= 1;
	g_dev_wifi_set.mode 		= network_set->mode;
	strcpy(g_dev_wifi_set.ssid,(char*)wifiSSID);
	strcpy(g_dev_wifi_set.wifi_pw,(char*)wifiPW);
	g_dev_wifi_set.re_cnn_timeout = network_set->re_cnn_timeout;	
	
	if(g_dev_wifi_set.mode == CFG_WIFI_MODE_STA){
		g_dev_wifi_set.ip_set.client_ip_set.dhcpc_enable   = network_set->dhcpc_enable;

		if(0 == g_dev_wifi_set.ip_set.client_ip_set.dhcpc_enable){

			u32_t client_ipaddr 	 = network_set->ip_addr0;
			u32_t client_ipmask 	 = network_set->ip_addr1;
			u32_t client_gateway	 = network_set->ip_addr2;
			g_dev_wifi_set.ip_set.client_ip_set.client_ipaddr  = client_ipaddr;
			g_dev_wifi_set.ip_set.client_ip_set.client_ipmask  = client_ipmask;
			g_dev_wifi_set.ip_set.client_ip_set.client_gateway = client_gateway;
			
		}
	}else{
		g_dev_wifi_set.ip_set.server_ip_set.enable   = 1;
		g_dev_wifi_set.ip_set.server_ip_set.server_ipaddr      = network_set->ip_addr0;
		g_dev_wifi_set.ip_set.server_ip_set.dhcpd_begin_ipaddr = network_set->ip_addr1;
		g_dev_wifi_set.ip_set.server_ip_set.dhcpd_end_ipaddr   = network_set->ip_addr2;

		//å¦‚æžœflashä¸­æ²¡æœ‰é…ç½®
		if(g_dev_wifi_set.ip_set.server_ip_set.server_ipaddr == 0){
			set_ap_ip_default();//å¦‚æžœåœ°å€æ²¡æœ‰é…ç½®,åˆ™ä½¿ç”¨å®å®šä¹‰çš„åœ°å€é…ç½®
		}
	}

}

static void save_sd_wifi_config(sd_wifi_set*wifi_set)
{
	if(wifi_set->enable && 1 != wifi_set->tmp_set){

		network_config*network_set = get_network_cfg();
		network_set->mode = wifi_set->mode;
		setWifiSSID(wifi_set->ssid);
		setWifiPWD(wifi_set->wifi_pw);
		network_set->re_cnn_timeout = wifi_set->re_cnn_timeout;
		set_network_cfg(network_set);
	}
}
//将SD卡的配置保存到flash
static void save_sd_ip_config(sd_ip_set*ip_set)
{
	if(ip_set->enable == 1&& 1 != ip_set->tmp_set){
		network_config*network_set = get_network_cfg();
		if(g_dev_wifi_set.mode == CFG_WIFI_MODE_AP){
			network_set->ip_addr0 = g_dev_wifi_set.ip_set.server_ip_set.server_ipaddr;
			network_set->ip_addr1 = g_dev_wifi_set.ip_set.server_ip_set.dhcpd_begin_ipaddr;
			network_set->ip_addr2 = g_dev_wifi_set.ip_set.server_ip_set.dhcpd_end_ipaddr;

		}else if(g_dev_wifi_set.mode == CFG_WIFI_MODE_STA){
			network_set->dhcpc_enable	= g_dev_wifi_set.ip_set.client_ip_set.dhcpc_enable;
			network_set->ip_addr0 		= g_dev_wifi_set.ip_set.client_ip_set.client_ipaddr;
			network_set->ip_addr1 		= g_dev_wifi_set.ip_set.client_ip_set.client_ipmask;
			network_set->ip_addr2		= g_dev_wifi_set.ip_set.client_ip_set.client_gateway;
			
		}else{
			WIFI_PROC_LOG("not suports such wifi mode on this version.\n");
		}

		set_network_cfg(network_set);
	}
}

static void wifiset_update()
{
	get_wifi_default_set(&g_dev_wifi_set);
	
	if(1==appGetSdStatus()){
		WIFI_PROC_LOG("SD inline check sd config\n");
		check_sd_config_file();
		sd_wifi_set*wifi_set = get_sd_wifi_config();
		if(wifi_set != NULL){
			if(wifi_set->enable){
				if(wifi_set->mode == SD_CFG_WIFI_MODE_AP){
					g_dev_wifi_set.mode		= CFG_WIFI_MODE_AP;
				}else{
					g_dev_wifi_set.mode		= CFG_WIFI_MODE_STA;
				}
				strcpy(g_dev_wifi_set.ssid,wifi_set->ssid);
				strcpy(g_dev_wifi_set.wifi_pw,wifi_set->wifi_pw);
			}
			g_dev_wifi_set.re_cnn_timeout = wifi_set->re_cnn_timeout;
			if(0 == wifi_set->tmp_set){//需要保存到SD卡
				save_sd_wifi_config(wifi_set);
			}
		}
		sd_ip_set*ip_set = get_sd_ip_config();
		if(ip_set != NULL && ip_set->enable == 1){
			if(g_dev_wifi_set.mode == CFG_WIFI_MODE_AP){
				dev_server_ip_set*server_ip_set= &g_dev_wifi_set.ip_set.server_ip_set;
				server_ip_set->server_ipaddr	  = inet_addr(ip_set->server_ipaddr);
				server_ip_set->dhcpd_begin_ipaddr = inet_addr(ip_set->dhcpd_begin_ipaddr);
				server_ip_set->dhcpd_end_ipaddr   = inet_addr(ip_set->dhcpd_end_ipaddr);

			}else if(g_dev_wifi_set.mode == CFG_WIFI_MODE_STA){
				dev_client_ip_set*client_ip_set = &g_dev_wifi_set.ip_set.client_ip_set;
				client_ip_set->dhcpc_enable   = ip_set->dhcpc_enable;
				client_ip_set->client_ipaddr  = inet_addr(ip_set->client_ipaddr);
				client_ip_set->client_ipmask  = inet_addr(ip_set->client_ipmask);
				client_ip_set->client_gateway = inet_addr(ip_set->client_gateway);
			}else{
				WIFI_PROC_LOG("not suports such wifi mode on this version.\n");
			}
			if(0 == ip_set->tmp_set){//需要保存到SD卡
				save_sd_ip_config(ip_set);
			}

		}


#if IPERF_TEST_SET
		iperf2_set iperf2_config;
		memset(&iperf2_config,0,sizeof(iperf2_set));
		sd_iperf_set*iperf_set = get_sd_iperf_config();
		if(iperf_set != NULL){
			if(iperf_set->enable){
				iperf2_config.enable				= iperf_set->enable;
				iperf2_config.server_mode			= iperf_set->server_mode;
			
				strcpy(iperf2_config.server_ipaddr,iperf_set->server_ipaddr);
				
				iperf2_config.server_port			= iperf_set->server_port;
				iperf2_config.udp_mode				= iperf_set->udp_mode;
				iperf2_config.send_bps				= iperf_set->send_bps;
				iperf2_config.time_duration 		= iperf_set->time_duration;
				iperf2_config.info_printf_duration	= iperf_set->info_printf_duration;

				iperf2_init(&iperf2_config);
				
			}
		}
#endif		

	}else{
		WIFI_PROC_LOG("SD offline check sd config\n");
	}
}

u32 wifiGetTxCnt(void)
{
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	return ssv6xxx_wifi_get_tx_cnt();
#else
	return 0;
#endif
}
int wifiStaActive(void)
{
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	return ssvwifiStaActive();
#else
	return 1;
#endif
}
u32 wifiTxCntMax(void)
{
	return 30;
}

char *get_wifi_ssid_string(void)
{
	return s_wifi_context.wifi_set->ssid;
}

char *getWifiSSIDInfoString(void)
{
	if(strlen(s_wifi_context.s_wifi_ssid_info_str) == 0){
		char* ssid = get_ssid_by_mac(g_dev_wifi_set.ssid,getWifiMAC());
		sprintf(s_wifi_context.s_wifi_ssid_info_str, "AP-SSID:%s", ssid);
	}
	return s_wifi_context.s_wifi_ssid_info_str;
}
char *getWifiPwString(void)
{
	if(strlen(s_wifi_context.s_wifi_pw_str) == 0){
		sprintf(s_wifi_context.s_wifi_pw_str, "AP-WPA2 %s", (char*)getWifiPWD());
	}
	return s_wifi_context.s_wifi_pw_str;
}
char *getWifiConnectInfoString(void)
{
	return s_wifi_context.s_wifi_connect_info_str;
}

char *getWifiIPString(void)
{
    struct netif *netif = netif_default;  // 获取默认网络接口
    if (netif && !ip_addr_isany(&netif->ip_addr)) {
        uint32_t ip = netif->ip_addr.addr;
        sprintf(s_wifi_context.s_wifi_ip_str,"%d.%d.%d.%d\n",
            (ip >> 0) & 0xFF,
            (ip >> 8) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 24) & 0xFF); 
    }
	return s_wifi_context.s_wifi_ip_str;
}

int getWifiChannel(void){

#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
	return wifiGetChannel();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030
	extern int wifi_channel;
	channel = wifi_channel;
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152
	extern int wifi_channel;
	return wifi_channel;
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	return wifiGetChannel();

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM	
	return wifiGetChannel();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
	return  zt_get_wifi_channel();
#endif

	return 0;
}


void dhcpc_event_handler(int event, void *parameter)
{
	WIFI_PROC_LOG("dhcp_event_handler begin,event:%d\n",event);
	if(LOCAL_DHCP_ADDRESS_ASSIGNED == event){
		s_wifi_context.dhcp_status = DHCPC_SUCCESS;
		GetAuthId(); // 调用GetAuthId()
	}else if(LOCAL_DHCP_WAIT_ADDRESS == event){
		s_wifi_context.dhcp_status = DHCP_WAIT;
	}else if(LOCAL_DHCP_TIMEOUT == event){
		s_wifi_context.dhcp_status = DHCP_ERR;
	}else if(LOCAL_DHCP_START == event){
		s_wifi_context.dhcp_status = DHCP_WAIT;
	}else{
		WIFI_PROC_LOG("dhcp_event_handler other,event:%d\n",event);
	}
	
}

void update_sta_status()
{
	if(g_dev_wifi_set.mode == CFG_WIFI_MODE_AP){
		return;
	}
	
	int rssi_val = 0;
	char *SSID = g_dev_wifi_set.ssid;
	char *wifiPW = g_dev_wifi_set.wifi_pw;
	
	if(s_wifi_context.wifi_status == WIFI_OPEN_SUCCESS){
		get_current_rssi(&rssi_val);
		WIFI_PROC_LOG("s_wifi_context.wifi_status:%d,RSSI:%d\r\n", s_wifi_context.wifi_status,rssi_val);	
		
		sprintf(s_wifi_context.s_wifi_ssid_info_str, "STA-SSID %s", SSID);
		sprintf(s_wifi_context.s_wifi_pw_str, "STA-WPA2 %s", wifiPW);
		if(s_wifi_context.dhcp_status == DHCPC_SUCCESS||
		   s_wifi_context.dhcp_status == DHCPD_SUCCESS){
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO rssi:%d,ip:%s",rssi_val,getWifiIPString());
		}else if(s_wifi_context.dhcp_status == DHCP_ERR){
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO rssi:%d,ip:dhcpc error",rssi_val);
		}else if(s_wifi_context.dhcp_status == DHCP_WAIT){
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO rssi:%d,ip:dhcpc wait",rssi_val);
		}else{
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO rssi:%d,ip:%s",rssi_val,getWifiIPString());
		}
	}else{
		sprintf(s_wifi_context.s_wifi_ssid_info_str, "STA-SSID %s", SSID);
		sprintf(s_wifi_context.s_wifi_pw_str, "STA-WPA2 %s", wifiPW);
		if(s_wifi_context.wifi_status == WIFI_OPENING||
		   s_wifi_context.wifi_status == WIFI_RE_OPENING){
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO:connecting");
		}else{
			sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO:disconnect");
		}
			
	}
	
	WIFI_PROC_LOG("s_wifi_ssid_info_str:%s\r\n", s_wifi_context.s_wifi_ssid_info_str);
	WIFI_PROC_LOG("s_wifi_pw_str:%s\r\n", s_wifi_context.s_wifi_pw_str);
	WIFI_PROC_LOG("s_wifi_connect_info_str:%s\r\n", s_wifi_context.s_wifi_connect_info_str);

	sendMsgNonblock(OS_TASK_UI,SYS_WIFI_CHANGE,0);
	
	//led_set(LED_NO_FLICKER_TYPE,80);
}

static void dhcpc_start()
{
	int ret = 0;
	if(0 == g_dev_wifi_set.ip_set.client_ip_set.dhcpc_enable){
		set_ifconfig(p_netif,&(g_dev_wifi_set.ip_set.client_ip_set));
		s_wifi_context.dhcp_status = DHCPC_SUCCESS;
	}else{
		s_wifi_context.dhcp_run = 1;
		ret = dhcp_client_task(p_netif);
		if(ret < 0){
			update_sta_status();
			return;
		}	
	}
	update_sta_status();
	return;
}

static void wifi_close()
{
	WIFI_PROC_LOG("==== wifi close start ===\r\n");
	if(s_wifi_context.wifi_status == WIFI_CLOSE || 0 == s_wifi_context.wifi_run){
		WIFI_PROC_LOG("==== wifi has been close other ===\r\n");
		return;
	}
	s_wifi_context.wifi_run = 0;
	int try = 0;
	while (try < 600){
		if(s_wifi_context.wifi_status== WIFI_OPENING){
			if(try%10== 0){
				WIFI_PROC_LOG("wait wifi open&close,status:0x%x\r\n",s_wifi_context.wifi_status);
			}
			osal_task_sleep(100);
			try++;
		}else if(s_wifi_context.wifi_status == WIFI_CLOSE||
			     s_wifi_context.wifi_status == WIFI_CLOSING){
			WIFI_PROC_LOG("==== wifi has been closed by others ===\r\n");
			return;
		}else{
			break;
		}
	}

	if(s_wifi_context.wifi_status == WIFI_OPEN_ERR){
		osal_task_sleep(120);
	}
	//存在异常,后不报scan事件情况
	if(s_wifi_context.wifi_status == WIFI_OPENING){
		WIFI_PROC_LOG("wait WIFI_OPENING fail,set close immediately...\r\n");
		s_wifi_context.wifi_status = WIFI_OPEN_ERR;
	}

	if( s_wifi_context.wifi_status == WIFI_OPEN_SUCCESS || 
		s_wifi_context.wifi_status == WIFI_RE_OPENING||
		s_wifi_context.wifi_status == WIFI_OPEN_ERR){
		
		s_wifi_context.wifi_status = WIFI_CLOSING;

#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
		extern int wifi_off(void);
		wifi_off();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030
		wifi_off_6030(1);
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152
		wifi_off_6152(1);
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
		ssv_wifi_set_mode(WIFI_MODE_STA);
		//ssv_wifi_stop_softap();
		ssv_wifi_enter_ps();
		//release_udhcpd();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
		if(g_dev_wifi_set.mode == CFG_WIFI_MODE_STA){
			atbm_wifi_sta_disjoin_ap();
			atbmwifi_stop();
		}else{
			atbm_wifi_off(1);
		}
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
		if(s_wifi_context.low_power == 1){
			WIFI_PROC_LOG("wlan_exit_low_power for close\n");
			wlan_exit_low_power("wlan0");
			s_wifi_context.low_power = 0;
		}
		wlan_app_disc("wlan0");
#endif
			
	}
	WIFI_PROC_LOG("==== wifi close end ===\r\n");
	s_wifi_context.wifi_status = WIFI_CLOSE; // wifi关闭完成
	s_wifi_context.disconnect_time = 0;
	update_sta_status();

}

int dhcp_client_task(void *argument)
{
	int ret = 0;
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	s_wifi_context.dhcp_status = DHCPC_SUCCESS;

	WIFI_PROC_LOG("BOARD_WIFI_SV6158 next step auto runing.\n");
	return 0;
#else	
    struct netif *netif = (struct netif *)argument;
    struct dhcp  *dhcp;
	int DHCP_state = LOCAL_DHCP_START;

	dhcpc_event_handler(LOCAL_DHCP_START,NULL);
	int dbg_indx = 0;
    while (s_wifi_context.wifi_run != 0&&s_wifi_context.dhcp_run != 0) {

        switch (DHCP_state) {
        case LOCAL_DHCP_START: {
            ip_addr_set_zero_ip4(&netif->ip_addr);
            ip_addr_set_zero_ip4(&netif->netmask);
            ip_addr_set_zero_ip4(&netif->gw);
            DHCP_state = LOCAL_DHCP_WAIT_ADDRESS;
            dhcp_start(netif);
			dhcpc_event_handler(LOCAL_DHCP_START,NULL);
            WIFI_PROC_LOG("after dhcpc_start\n");
        } break;
        case LOCAL_DHCP_WAIT_ADDRESS: {
            if (dhcp_supplied_address(netif)) {

                WIFI_PROC_LOG("IP address assigned,current ip: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
				DHCP_state = LOCAL_DHCP_ADDRESS_ASSIGNED;
				dhcpc_event_handler(LOCAL_DHCP_ADDRESS_ASSIGNED,NULL);
				ret = 0;
				goto END;
            } else {
                dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
				if(dbg_indx % 10 == 0)
					WIFI_PROC_LOG("Wait IP address,tries:%u,DHCP server:%s\n",dhcp->tries,ip4addr_ntoa(&(dhcp->server_ip_addr)));
				dhcp->tries++;

				/* DHCP timeout */
                if (dhcp->tries > MAX_DHCP_TRIES) {
					 WIFI_PROC_LOG("Wait IP address assigned timeout,tries:%u,MAX_DHCP_TRIES:%u\n",dhcp->tries,MAX_DHCP_TRIES);
                    DHCP_state = LOCAL_DHCP_TIMEOUT;
					dhcpc_event_handler(LOCAL_DHCP_TIMEOUT,NULL);
					ret = -1;
				    goto END;
                }
            }
        } break;
        default:
            break;
        }
		dbg_indx++;
        vTaskDelay(100);
    }
#endif
END:
	if(s_wifi_context.dhcp_status != DHCPC_SUCCESS){
		s_wifi_context.dhcp_status = DHCP_ERR;//被终止
		WIFI_PROC_LOG("Set dhcp_status to DHCP_ERR\n");
		dhcp_stop(netif);
	}else{
		WIFI_PROC_LOG("Set dhcp_status to DHCPC_SUCCESS\n");
	}
	return ret;
}

#if WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
//函数内不阻塞WIFI动作,用并行方式同步
void atbm_user_event_cb(unsigned int mode,int event, void *parameter)
{

	switch(event){
		case ATBM_WIFI_DEAUTH_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_DEAUTH_EVENT.\n");
		}break;
		case ATBM_WIFI_JOIN_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_JOIN_EVENT.\n");
		}break;
		case ATBM_WIFI_AUTH_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_AUTH_EVENT.\n");
		}break;
		case ATBM_WIFI_ASSOC_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_ASSOC_EVENT.\n");
		}break;
		case ATBM_WIFI_ASSOCRSP_TXOK_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_ASSOCRSP_TXOK_EVENT.\n");
		}break;
		case ATBM_WIFI_DEASSOC_EVENT://关联失败
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_DEASSOC_EVENT.\n");
			
			if(WIFI_OPEN_SUCCESS == s_wifi_context.wifi_status){//连接到掉线
				if(s_wifi_context.wifi_set->re_cnn_timeout != 0){//掉线重连
					s_wifi_context.disconnect_time = xTaskGetTickCount();
					s_wifi_context.wifi_status = WIFI_RE_OPENING;
					wait_dhcp_exit(0);
					update_sta_status();
				}else{
					WIFI_PROC_LOG("disconnect_time:%u.\n",s_wifi_context.disconnect_time);
					s_wifi_context.wifi_switch = 0;
				}
			}else{//查无此AP,或者加密类型错误
				s_wifi_context.wifi_status = WIFI_OPEN_ERR;
					s_wifi_context.wifi_switch = 0;
			}
			
		}
		case ATBM_WIFI_SCANSTART_EVENT://开始扫描
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_SCANSTART_EVENT.\n");
		}break;
		case ATBM_WIFI_SCANDONE_EVENT://扫描结束		
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_SCANDONE_EVENT.\n");

			s_wifi_context.b_scaned = 1;
			
			if(s_wifi_context.wifi_run == 0){//set exit by user
				s_wifi_context.wifi_status = WIFI_OPEN_ERR;
				WIFI_PROC_LOG("=== atmb wifi set close.event:%u\n",event);
				return;
			}

			if(s_wifi_context.wifi_status == WIFI_RE_OPENING){
				uint32_t tm = xTaskGetTickCount();
				s_wifi_context.re_connect_idx ++;
				unsigned int re_cnn_timeout = s_wifi_context.wifi_set->re_cnn_timeout*configTICK_RATE_HZ;
				u32 diff =  tm - s_wifi_context.disconnect_time;
				WIFI_PROC_LOG("diff:%u,re_cnn_timeout:%u.\n",diff,re_cnn_timeout);
				if(diff >= re_cnn_timeout){
					s_wifi_context.wifi_status = WIFI_OPEN_ERR;
					s_wifi_context.wifi_switch = 0;
				}
			}

		
		}break;
		case ATBM_WIFI_ENABLE_NET_EVENT:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_ENABLE_NET_EVENT.\n");
			WIFI_PROC_LOG("atbm wifi connect success,dhcp_status:%d\n",s_wifi_context.dhcp_status);
			s_wifi_context.wifi_status = WIFI_OPEN_SUCCESS;
			s_wifi_context.re_connect_idx = 0;
			s_wifi_context.disconnect_time = 0;
			p_netif=atbm_get_netif(ATBM_WIFI_STA_MODE);

			if(s_wifi_context.dhcp_status == DHCPC_SUCCESS){
				//led_set(LED_NO_FLICKER_TYPE,80);
			}else if(s_wifi_context.dhcp_status != DHCP_WAIT){
				//dhcpc again
				p_netif=atbm_get_netif(ATBM_WIFI_STA_MODE);
				add_event_timerList(LOW, dhcpc_start, NULL, NULL, 0);
			}
			update_sta_status();
		}break;
		case ATBM_WIFI_DISENABLE_NET_EVENT://掐断连接
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_DISENABLE_NET_EVENT.\n");
			WIFI_PROC_LOG("atbm wifi disconnect happened.\n");
		break;
		case ATBM_WIFI_SMARTCONFIG_SUCCESS:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_SMARTCONFIG_SUCCESS.\n");
		}break;
		case ATBM_WIFI_WPS_SUCCESS:
		{
			WIFI_PROC_LOG("=== atmb wifi ATBM_WIFI_WPS_SUCCESS.\n");
		}break;
		default:{
			WIFI_PROC_LOG("=== atmb wifi Unknow event:%d.\n",event);
		}break;
	}
}
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101

extern struct mmc_host zt_bw_mmc_host;

struct sdio_func ztop_func = {
    .num = 1,
    .enable_timeout = 100,
};

struct mmc_card ztop_card = {
    .host = &zt_bw_mmc_host,
    .func = &ztop_func,
};

struct mmc_host zt_bw_mmc_host = {
    .card = &ztop_card,
	.controller = NULL,
};

struct mmc_card *sdio_get_card(void)
{
    return &ztop_card;
}

struct sdio_func *sdio_get_func(void)
{
    return &ztop_func;
}

struct mmc_host *sdio_get_mmc_host(void)
{
    return &zt_bw_mmc_host;
}

int zt_get_wifi_channel(void)
{
    struct wlan_app_info_t wdn_app_info = {0};
    wlan_app_get_wdn_info("wlan0", &wdn_app_info);

    return wdn_app_info.channel;
}
u8  mac_addr[6];
u8* zt_wifi_get_mac_address()
{
    wlan_get_mac_address("wlan0", mac_addr);
	WIFI_PROC_LOG("mac:%02x%02x%02x%02x%02x%02x\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	return mac_addr;
}

//in AP mode, when a sta auth sucess
void on_sta_port_authorized(unsigned char *pMacAddr)
{
	WIFI_PROC_LOG("on_sta_port_authorized begin,pMacAddr:%s\r\n", pMacAddr);

}
//in AP mode, when a sta assoc sucess
void on_sta_associated(unsigned char *pMacAddr)
{
	s_wifi_context.b_scaned = 1;
	WIFI_PROC_LOG("on_sta_associated begin,pMacAddr:%s\r\n", pMacAddr);

}
//in AP mode, when a sta disassoc
void on_sta_disassociated(unsigned char *pMacAddr)
{
	WIFI_PROC_LOG("on_sta_disassociated begin,pMacAddr:%s\r\n", pMacAddr);

}
//in AP mode, when a sta disconneted
void on_sta_disconnect(unsigned char *pMacAddr)
{
	WIFI_PROC_LOG("on_sta_disconnect begin,pMacAddr:%s\r\n", pMacAddr);

}
//start ap successfully
void on_sta_ap_ready(void)
{
	WIFI_PROC_LOG("on_sta_ap_ready begin\r\n");

}

static void zt_reconnect()
{
	int ret = 0;
	char *SSID = g_dev_wifi_set.ssid;
	char *wifiPW = g_dev_wifi_set.wifi_pw;
	ret = wlan_app_join("wlan0",SSID,wifiPW);
	WIFI_PROC_LOG("reconnect ssid:%s,pw:%s,wifi_status:%u\r\n", SSID,wifiPW,s_wifi_context.wifi_status);
	if(ret != 0){
		 //if(WIFI_RE_OPENING == s_wifi_context.wifi_status){}//重连失败
		unsigned int re_cnn_timeout = s_wifi_context.wifi_set->re_cnn_timeout*configTICK_RATE_HZ;
		uint32_t tm = xTaskGetTickCount();
		if((tm - s_wifi_context.disconnect_time) > re_cnn_timeout){//重连超时
			s_wifi_context.wifi_switch = 0;
		}else{//继续重连
			s_wifi_context.re_connect_idx ++;
			WIFI_PROC_LOG("sec reconnect involk,re_connect_idx:%d\r\n",s_wifi_context.re_connect_idx);
			int timer = s_wifi_context.re_connect_idx*1000;
			if(timer > 60*1000){
				timer = 60*1000;
			}
			add_event_timerList(LOW, zt_reconnect, NULL, NULL, timer);
		}
	}
	
}

//in STA mode, connected:1; disconnected:0
void on_link2ap_connectd(char *pIntfName, int status)
{
	WIFI_PROC_LOG("on_link2ap_connectd begin,pIntfName:%s,status:%d\r\n", pIntfName,status);
	s_wifi_context.b_scaned = 1;
	if(0 == status){
		if(WIFI_OPEN_SUCCESS == s_wifi_context.wifi_status){//掉线

			if(s_wifi_context.wifi_set->re_cnn_timeout != 0){//需要重连
				s_wifi_context.disconnect_time = xTaskGetTickCount();
				s_wifi_context.wifi_status = WIFI_RE_OPENING;
				WIFI_PROC_LOG("first reconnect involk\r\n");
				wait_dhcp_exit(0);
				add_event_timerList(LOW, zt_reconnect, NULL, NULL, 1000);
			}else{
				s_wifi_context.wifi_switch = 0;
			}
		}
	}else if(1 == status){//连接成功
		s_wifi_context.wifi_status = WIFI_OPEN_SUCCESS;
		s_wifi_context.disconnect_time = 0;
		s_wifi_context.re_connect_idx = 0;
		p_netif = wlan_app_netif_get("wlan0");
		if(s_wifi_context.dhcp_status == DHCPC_SUCCESS){//DHCP已经启动
			//led_set(LED_NO_FLICKER_TYPE,80);
		}else if(s_wifi_context.dhcp_status != DHCP_WAIT){//
			add_event_timerList(LOW, dhcpc_start, NULL, NULL, 0);
		}
	}
	update_sta_status();
}

static struct user_notify_hook_t zt_notify_hook={
	.hook_sta_port_authorized 	= on_sta_port_authorized,
	.hook_sta_associated 		= on_sta_associated,
	.hook_sta_disassociated		= on_sta_disassociated,
	.hook_sta_disconnect		= on_sta_disconnect,
	.hook_sta_ap_ready			= on_sta_ap_ready,
	.hook_link2ap_connectd		= on_link2ap_connectd,
};

int wifi_module_probe()
{
    int ret = 0;
    struct mmc_card* card = &ztop_card;
    struct sdio_func* func = &ztop_func;
    struct mmc_host* host = &zt_bw_mmc_host;

    func->card = card;
    host->card = card;

    wlan_app_set_segment(66, 1);
    wlan_app_register_hook(&zt_notify_hook);

    ret = wlan_app_init();
    WIFI_PROC_LOG("wlan_app_init over!");
    if (ret){
        WIFI_PROC_LOG("%d: wlan_app_init error!", ret);
        goto out;
    }
    WIFI_PROC_LOG("%s: probe success!", __func__);
out:
    return ret;
}

//卸载驱动
void zt_disconnect_wifi_driver()
{
    WIFI_PROC_LOG("\n[DisconnectWiFiDriver] Start....\n");
    struct netif *netif;
    netif = wlan_app_netif_get("wlan0");
    if (netif == NULL) {
        WIFI_PROC_LOG("wlan device not exist!\n");
    }
    wlan_app_disc("wlan0");
    wlan_app_exit();//退出SDIO
	s_wifi_context.sdio_run = 0;
    netif = wlan_app_netif_get("wlan0");
    if (netif == NULL) {
        WIFI_PROC_LOG("DisconnectWiFiDriver ok\n");
    }
    else{
        WIFI_PROC_LOG("DisconnectWiFiDriver fail\n");
    }
    WIFI_PROC_LOG("\n[DisconnectWiFiDriver] END.\n");
	
}

#endif


volatile u32 alignTest;
u32 *getAlignTestAddr(void)
{
	return ((u32*)(&alignTest))+1;
}


int get_current_rssi(int *pRSSI)
{
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
			extern int wifi_get_rssi(int*);
			wifi_get_rssi(pRSSI);
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030
	
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152
	
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
	*pRSSI = atbmwifi_get_rssi();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
	struct wlan_app_info_t wdn_app_info;

	wlan_app_get_wdn_info("wlan0", &wdn_app_info);
	*pRSSI = wdn_app_info.rssi;

#endif

	return 0;
	
}
/*
dev_client_ip_set client_ip_set;
client_ip_set.dhcpc_enable	  = 1;
client_ip_set.client_ipaddr	  = inet_addr("192.168.88.199");
client_ip_set.client_ipmask	  = inet_addr("255.255.255.0");
client_ip_set.client_gateway  = inet_addr("192.168.88.1");
set_ifconfig(pnetif, &client_ip_set);
*/



int set_ifconfig(struct netif *pnetif, dev_client_ip_set*client_ip_set)
{
	/* set ip address */
    if (0 != client_ip_set->client_ipaddr){
		WIFI_PROC_LOG("netif_set_ipaddr:0x%x\r\n",client_ip_set->client_ipaddr);	
        netif_set_ipaddr(pnetif, (ip_addr_t*)(&client_ip_set->client_ipaddr));
    }
    /* set gateway address */
    if (0 != client_ip_set->client_ipmask){
		WIFI_PROC_LOG("netif_set_netmask:0x%x\r\n",client_ip_set->client_ipmask);
        netif_set_netmask(pnetif, (ip_addr_t*)(&client_ip_set->client_ipmask));
    }
    /* set netmask address */
    if (0 != client_ip_set->client_gateway){
		WIFI_PROC_LOG("netif_set_gw:0x%x\r\n",client_ip_set->client_gateway);
        netif_set_gw(pnetif, (ip_addr_t*)(&client_ip_set->client_gateway));
    }

    return 1;
}

static void wifi_open_ap_thread()
{
	char *wifiSSID = g_dev_wifi_set.ssid;
	char *wifiPW = g_dev_wifi_set.wifi_pw;
	int channel = 0;

	static char SSID[64];//如果只输入头,createAP时会copy回来带mac的实际SSID
	memset(SSID,0,64);
	char mac[16];
	if(wifi_dev_init_sta==0){
		s_wifi_context.wifi_status = WIFI_CLOSE;
		return;
	}
	if (1 == g_dev_wifi_set.enable){

		s_wifi_context.wifi_status = WIFI_OPENING;
		strcpy(SSID,wifiSSID);//ssid头
		WIFI_PROC_LOG("wifiSSID:%s\n",wifiSSID);
		// WIFI_PROC_LOG("wifiMAC:%s\n",wifiMAC);
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
		extern int wifiGetChannel(void);
		wifi_create_ap(SSID, wifiPW, KEY_WPA2, 15, 1);
		channel = wifiGetChannel();

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030
		extern int wifi_channel;
		wifi_create_ap_6030(SSID, wifiPW, 0, 16); // 6030
		channel = wifi_channel;
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152
		// wifi_create_ap_6152(SSID,wifiPW,0,2); 	//6030
		extern int wifi_channel;
		channel = wifi_channel;
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
		struct netif *p_netif0 = NULL;
		int i = 0;

		while((p_netif0=netdev_get_netif("wlan0"))==NULL)
		{
			osal_task_sleep(20);
			if(i++>100)
				break;
		}
		WIFI_PROC_LOG("wlan0[0x%x] mac:%02x:%02x:%02x:%02x:%02x:%02x, len:%d\n",p_netif0,p_netif0->hwaddr[0], p_netif0->hwaddr[1],
				p_netif0->hwaddr[2],p_netif0->hwaddr[3],p_netif0->hwaddr[4],p_netif0->hwaddr[5],p_netif0->hwaddr_len);

		sprintf(SSID, "%s_%02x%02x%02x%02x%02x%02x", wifiSSID, p_netif0->hwaddr[0], p_netif0->hwaddr[1],
				p_netif0->hwaddr[2],p_netif0->hwaddr[3],p_netif0->hwaddr[4],p_netif0->hwaddr[5]);
		ssv_wifi_exit_ps();
		//ssv_wifi_set_mode(1);
		ssv_wifi_start_softap_ext(SSID,wifiPW,KEY_WPA2,EN_CHANNEL_AUTO_SELECT,0,1);//EN_CHANNEL_AUTO_SELECT
		i=0;
		while((p_netif=netdev_get_netif("wlan1"))==NULL)
		{
			osal_task_sleep(20);
			if(i++>100)
				break;
		}
		WIFI_PROC_LOG("wlan1[0x%x] mac:%02x:%02x:%02x:%02x:%02x:%02x, len:%d\n",p_netif, p_netif->hwaddr[0], p_netif->hwaddr[1],
					p_netif->hwaddr[2],p_netif->hwaddr[3],p_netif->hwaddr[4],p_netif->hwaddr[5],p_netif->hwaddr_len);
		if(p_netif==NULL)
		{
			strcpy(SSID,"no wifi");
			ssv_wifi_set_mode(WIFI_MODE_STA);
			ssv_wifi_enter_ps();
			s_wifi_context.wifi_status = WIFI_OPEN_ERR;
			return ;
		}
		WIFI_PROC_LOG("***heap remain size:%d %d %d [%dKB]\n\n",sizeof(ssv_wifi_apcfg_t),lastRemainSize-xPortGetFreeHeapSize(),xPortGetFreeHeapSize(),xPortGetFreeHeapSize()>>10);
		lastRemainSize=xPortGetFreeHeapSize();
		channel = wifiGetChannel();

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
		int len = 0;
		int i = 0;
		uint8_t mac[6];
		atbm_wifi_get_mac_address(mac);
		len = strlen(wifiSSID);
		SSID[len] = '_';
		sprintf(&SSID[++len], "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		for (i = 0; i < 12; i++){
			if (SSID[len + i] >= 97 && SSID[len + i] <= 122){
				SSID[len + i] -= 32;
			}
		}
		SSID[len + 12] = '\0';

		atbm_wifi_on(ATBM_WIFI_AP_MODE);
		p_netif = atbm_get_netif(ATBM_WIFI_AP_MODE);
		
		WIFI_PROC_LOG("init p_netif:%x\n\n", p_netif);
		atbm_wifi_ap_create(SSID, WLAN_WPA2_AUTH_PSK, 0, wifiPW, 0, 0);

		channel = wifiGetChannel();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
		int len = 0;
		memset(mac,0,16);
		u8*zt_mac = zt_wifi_get_mac_address();
		
		sprintf(mac, "%02x%02x%02x%02x%02x%02x", zt_mac[0], zt_mac[1], zt_mac[2], zt_mac[3], zt_mac[4], zt_mac[5]);
		WIFI_PROC_LOG("zt_wifi_get_mac_address,mac:%s.\n",mac);
		char* ssid = get_ssid_by_mac(g_dev_wifi_set.ssid,mac);

		WIFI_PROC_LOG("ssid:%s\n",ssid);
	 	int ret = wlan_app_ap_start("wlan0", 2, ssid, wifiPW);
		if(0 == ret && strlen(mac)>0){
			char*old_mac = (char*)getWifiMAC();
			if(strcmp(mac,old_mac)!=0 || 0 == strlen(old_mac)){
				WIFI_PROC_LOG("set mac to flash,mac:%s,old_mac:%s\n",mac,old_mac);
				setWifiMAC(mac);
			}
		}
		WIFI_PROC_LOG("ZT9101 wlan_app_ap_start,ret:%d.\n",ret);
		p_netif = wlan_app_netif_get("wlan0");
		channel = 2;//zt_get_wifi_channel();
		strcpy(SSID,ssid);

#endif


#if WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158
	WIFI_PROC_LOG("BOARD_WIFI_SV6158 next step auto runing.\n");
	return;
#else

	vTaskDelay(100);
	s_wifi_context.wifi_status = WIFI_OPEN_SUCCESS;
	s_wifi_context.dhcp_status = DHCP_WAIT;

	init_udhcpd();
	//修改server 端网关和主机IP
	if(s_wifi_context.wifi_set->ip_set.server_ip_set.enable ==1){
		
		struct ip4_addr addr;
		addr.addr = s_wifi_context.wifi_set->ip_set.server_ip_set.server_ipaddr;
		WIFI_PROC_LOG("set server ip_addr:%s\n",ipaddr_ntoa(&addr));
		if (addr.addr != 0){
			netif_set_ipaddr(p_netif, &addr);
			netif_set_gw(p_netif, &addr);
		}
		server_config.start = s_wifi_context.wifi_set->ip_set.server_ip_set.dhcpd_begin_ipaddr;
		server_config.end 	= s_wifi_context.wifi_set->ip_set.server_ip_set.dhcpd_end_ipaddr;
	}
	
	enable_dhcp_server();

	s_wifi_context.dhcp_status = DHCPD_SUCCESS;
	WIFI_PROC_LOG("open ap service success,wifi_status:%d,dhcp_status:%u\r\n",\
							s_wifi_context.wifi_status,s_wifi_context.dhcp_status);

	//char mem_str[50];
	sprintf(s_wifi_context.s_wifi_ssid_info_str, "AP-SSID %s", SSID);
	sprintf(s_wifi_context.s_wifi_pw_str, "AP-WPA2 %s", wifiPW);
	sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO chn:%u,ip:%s",channel,getWifiIPString());
	WIFI_PROC_LOG("s_wifi_ssid_info_str:%s\r\n", s_wifi_context.s_wifi_ssid_info_str);
	WIFI_PROC_LOG("s_wifi_pw_str:%s\r\n", s_wifi_context.s_wifi_pw_str);
	WIFI_PROC_LOG("s_wifi_connect_info_str:%s\r\n", s_wifi_context.s_wifi_connect_info_str);

	sendMsgNonblock(OS_TASK_UI,SYS_WIFI_CHANGE,0);

	u32 suces_ticks = xTaskGetTickCount();
	u32 deff = suces_ticks - g_ticks;
	
	WIFI_PROC_LOG("===========spend %u.%u s to start AP======\r\n",deff/1000,deff%1000 );

	return;
	
#endif
	}
ERR:
	s_wifi_context.wifi_status = WIFI_CLOSE;
	s_wifi_context.dhcp_run = 0;
	s_wifi_context.dhcp_status = DHCP_UNINIT;
	
	strcpy(s_wifi_context.s_wifi_ssid_info_str, "SSID:none");
	strcpy(s_wifi_context.s_wifi_pw_str, "WPA2:");
	sprintf(s_wifi_context.s_wifi_connect_info_str, "INFO chn:%u,ip:%s",0,"none");

	sendMsgNonblock(OS_TASK_UI,SYS_WIFI_CHANGE,0);
	WIFI_PROC_LOG("WIFISSID isn't correct!\r\n");

}

static void wifi_open_sta_thread()
{
	int ret = 0;
	char *wifiSSID = g_dev_wifi_set.ssid;
	char *wifiPW = g_dev_wifi_set.wifi_pw;
	
	char* SSID = wifiSSID;
	
	if(wifi_dev_init_sta==0 || 0 == g_dev_wifi_set.enable){
		s_wifi_context.wifi_status = WIFI_CLOSE;
		return;
	}
	
	//线程优先级较低,存在外部关闭可能
	if (s_wifi_context.wifi_run != 0){
		s_wifi_context.wifi_status = WIFI_OPENING;	// 打开完成标志开启
		update_sta_status();
	
		WIFI_PROC_LOG("wifiSSID:%s,wifiPW:%s\n",wifiSSID,wifiPW);
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
		//8189 将自动添加mac地址
		WIFI_PROC_LOG("=== 8189ftv wifi connect start ====\n");
		extern int wifi_create_station(char*, char*);
		ret = wifi_create_station(SSID, wifiPW);
		if(0 == ret){
			s_wifi_context.wifi_status = WIFI_OPEN_SUCCESS;
			s_wifi_context.disconnect_time = 0;
			s_wifi_context.re_connect_idx = 0;		
			add_event_timerList(LOW, dhcpc_start, NULL, NULL, 0);
		}
		
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6158

#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM

	WIFI_PROC_LOG("=== atbm wifi connect start ====\n");
	int pw_len = 0;

	WLAN_AUTH_MODE auth_mode = WLAN_WPA_AUTH_NONE;
	WLAN_ENCRYPTION encryption = WLAN_ENCRYPT_NONE;
		if(wifiPW != NULL){
			pw_len = strlen(wifiPW);
			if(0 != pw_len){
				auth_mode = WLAN_WPA2_AUTH_PSK;
				encryption = WLAN_ENCRYPT_AES;
			}	
		}

		atbm_wifi_sta_disjoin_ap();
		atbm_set_user_event_callback(atbm_user_event_cb);
		atbm_wifi_on(ATBM_WIFI_STA_MODE);
		
		ret = atbm_wifi_sta_join_ap(SSID, NULL, auth_mode, encryption, wifiPW);
		
		WIFI_PROC_LOG("end of atbm sta logic,ret:%u\r\n",ret);
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
		ret = wlan_app_join("wlan0",SSID,wifiPW);
		//没连上
		if(0 != ret){
			if(s_wifi_context.wifi_set->re_cnn_timeout != 0){//需要重连
				s_wifi_context.disconnect_time = xTaskGetTickCount();
				s_wifi_context.wifi_status = WIFI_RE_OPENING;
				WIFI_PROC_LOG("first reconnect involk\r\n");
				add_event_timerList(LOW, zt_reconnect, NULL, NULL, 1000);
			}
			ret = 0;//connect next time;
		}

#endif
		if(ret == 0){
			WIFI_PROC_LOG("wait connect scan....\r\n");
			return;
		}else{
			s_wifi_context.wifi_status = WIFI_OPEN_ERR;
			wifi_close();
			goto ERR;
		}
	}
	return;
ERR:
	strcpy(s_wifi_context.s_wifi_ssid_info_str, "SSID:none");
	strcpy(s_wifi_context.s_wifi_pw_str, "WPA2:");
	s_wifi_context.wifi_status = WIFI_CLOSE;
	sendMsgNonblock(OS_TASK_UI,SYS_WIFI_CHANGE,0);
	WIFI_PROC_LOG("WIFISSID isn't correct!\r\n");
}


//默认等50ms轮询一次，默认等100次
int wait_dhcp_exit(int timeout)
{
	if(s_wifi_context.dhcp_status == DHCP_UNINIT){
		return 0;
	}
	int ret = -1;
	const int loop_wait =  50;//vTaskDelay time
	s_wifi_context.dhcp_run = 0;
	int try = timeout==0?100:timeout/loop_wait;
	try = try>0?try:1;
	while(1){
		if(s_wifi_context.dhcp_status != DHCP_WAIT ){
			ret = 0;
			break;
		}
		if(try != 0 ){
			try --;
			vTaskDelay(loop_wait);
		}else{
			break;
		}		
	}
	if(DHCPD_SUCCESS == s_wifi_context.dhcp_status){
		//DHCP 服务端去初始化
		disable_dhcp_server();
		release_udhcpd();

	}else if(DHCPC_SUCCESS == s_wifi_context.dhcp_status){
		dhcp_stop(p_netif);
	}
	s_wifi_context.dhcp_status = DHCP_UNINIT;
	return ret;
}

void wifi_ap_task_proc(void* p)
{
	uint8_t wifi_switch = 0;
	s_wifi_context.wifi_task_runing = 1;
	while(s_wifi_context.wifi_task_run == 1){
		if(s_wifi_context.wifi_switch != wifi_switch){
			WIFI_PROC_LOG("switch change local:0x%x,outside:0x%x\n",wifi_switch,s_wifi_context.wifi_switch);

			if(s_wifi_context.wifi_switch == 1){//开操作
				if(s_wifi_context.wifi_status == WIFI_CLOSE){
					wifi_open_ap_thread(NULL);
				}else{
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101 
					//仅仅是低电模式
					if(s_wifi_context.low_power == 1){
						WIFI_PROC_LOG("=======wlan_exit_low_power.=========\n");
						wlan_exit_low_power("wlan0");
						s_wifi_context.low_power = 0;
					}			
#endif
				}
				wifi_switch = 1;
			}else{//关操作
				if(s_wifi_context.wifi_status!=WIFI_CLOSE ){
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101 
					if(s_wifi_context.low_power == 0){
						//不退出驱动,仅进入低电模式
						WIFI_PROC_LOG("===== set_wifi_low_power======\n");
						wlan_entry_low_power("wlan0");
						s_wifi_context.low_power = 1;
						vTaskDelay(1000);
					}
#else
					//退出驱动
					wait_dhcp_exit(0);
					wifi_close();
#endif
				}			
				wifi_switch = 0;
			}
		}
		osal_task_sleep(10);
	}
	
	wait_dhcp_exit(0);
	wifi_close();
	s_wifi_context.wifi_switch = 0;//wifi dev close
	s_wifi_context.wifi_task_runing = 0;
	vTaskDelete(NULL);
}

static int start_ap()
{
	if(s_wifi_context.wifi_task_runing == 0){
		s_wifi_context.wifi_task_run = 1;
		if (!osal_thread_create(wifi_ap_task_proc, NULL, 5, 0, 2048, "wifi_sta_task_proc")){
			printf("wifi_sta_task_proc error!\r\n");
		}
	}
	WIFI_PROC_LOG("start AP\n");
	s_wifi_context.wifi_switch = 1;

	//iperf2_start();

	return 0;
}
static int stop_ap()
{
	WIFI_PROC_LOG("*** %s\n", __func__);
	s_wifi_context.wifi_switch = 0;
	return 0;
}

void wifi_sta_task_proc(void* p)
{
	uint8_t wifi_switch = 0;
	s_wifi_context.wifi_task_runing = 1;
	while(s_wifi_context.wifi_task_run == 1){
		if(s_wifi_context.wifi_switch != wifi_switch){
			WIFI_PROC_LOG("switch change local:0x%x,outside:0x%x\n",wifi_switch,s_wifi_context.wifi_switch);

			if(s_wifi_context.wifi_switch == 1){//开操作
				if(s_wifi_context.wifi_status == WIFI_CLOSE){
					s_wifi_context.b_scaned = 0;
					
					s_wifi_context.disconnect_time = 0;
					s_wifi_context.re_connect_idx = 0;
					s_wifi_context.wifi_run = 1;
					wifi_open_sta_thread(NULL);
					//状态等待处理
					while(s_wifi_context.wifi_task_run == 1){
						if(s_wifi_context.wifi_status == WIFI_OPEN_SUCCESS){
							 wifi_switch = 1;//wifi dev working
							 break;
						}else if( s_wifi_context.wifi_status == WIFI_OPEN_ERR){
							wifi_close();
							wifi_switch = 0;//wifi dev close
							s_wifi_context.wifi_switch = 0;//打开失败
							break;
						}
						else if(s_wifi_context.wifi_status == WIFI_OPENING && s_wifi_context.b_scaned){
							osal_task_sleep(120);
							wifi_switch = 1;//wifi dev working
							break;
						}else if(s_wifi_context.wifi_status == WIFI_RE_OPENING){
							wifi_switch = 1;//wifi dev working
							break;
						}else{
							osal_task_sleep(10);
							continue;
						}
					}
				}else{
					WIFI_PROC_LOG("*** know statue:0x%x\n",s_wifi_context.wifi_status);
					osal_task_sleep(120);
					continue;
				}
			}else{//关操作
				wait_dhcp_exit(0);
				wifi_close();
				wifi_switch = 0;//wifi dev close
				osal_task_sleep(120);
				continue;
			}
		}
		osal_task_sleep(10);
	}
	
	wait_dhcp_exit(0);
	wifi_close();
	s_wifi_context.wifi_switch = 0;//wifi dev close
	s_wifi_context.wifi_task_runing = 0;
	vTaskDelete(NULL);
}

static int start_sta(void)
{
	if(s_wifi_context.wifi_task_runing == 0){
		s_wifi_context.wifi_task_run = 1;
		if (!osal_thread_create(wifi_sta_task_proc, NULL, 5, 0, 2048, "wifi_sta_task_proc")){
			printf("wifi_sta_task_proc error!\r\n");
		}
	}
	WIFI_PROC_LOG("start STA\n");
	s_wifi_context.wifi_switch = 1;
	
	return 0;
}

static int stop_sta(void)
{
	WIFI_PROC_LOG("close STA\n");
	s_wifi_context.wifi_switch = 0;
	return 0;
}

int wifi_dev_init()
{
	s_wifi_context.sdio_run = 1;
	 sdc_testio_init();
	 extern int bw_sdio_rw_extend(struct mmc_host * host, u32 arg, u8 * buf, unsigned blocks, unsigned blksz);
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_ZT9101
	extern struct mmc_card ztop_card;
	extern struct mmc_host zt_bw_mmc_host;
	zt_bw_mmc_host.controller = sdc1_io_init();
	zt_bw_mmc_host.mmc_io_rw_extend = bw_sdio_rw_extend;
#else
	extern struct mmc_card rtw_card;
	extern struct mmc_host bw_mmc_host;
	bw_mmc_host.controller = sdc1_io_init();
	bw_mmc_host.mmc_io_rw_extend = bw_sdio_rw_extend;
#endif
	 WIFI_PROC_LOG("sdc_io_init\r\n");
	 wifi_dev_init_sta=0;
#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV
	 sdio_init(&bw_mmc_host, &rtw_card);
	 //////////////////////// for test
	 sdio_bus_probe();
	 srand(sys_now());

	 lwip_netif_init();
	 
#if DEBUG_WIFI_MODULE
	 wifi_on();
	 start_interactive_mode();
	 while(1)
		 sleep(3);
#endif
#elif (WIFI_SUPPORT_VERSION==BOARD_WIFI_SV6158)  // (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030) || (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6152)
	 wifi_config_sv6xxx();
#elif WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
	 tcpip_init(tcpip_init_end, NULL);
 
	 sdio_init(&bw_mmc_host, &rtw_card);
	 atbm_wifi_hw_init();
	 vTaskDelay(100);	 
	 WIFI_PROC_LOG("init atbm wifi end\r\n");
#elif (WIFI_SUPPORT_VERSION==BOARD_WIFI_ZT9101)
	//tcpip_init(tcpip_init_end, NULL);
    wifi_module_probe();
	wlan_app_init();

#endif

}


//卸载驱动
void DisconnectWiFiDriver()
{
    WIFI_PROC_LOG("\n[DisconnectWiFiDriver] Start....\n");
    struct netif *netif;

    netif = wlan_app_netif_get("wlan0");
    if (netif == NULL) {
        WIFI_PROC_LOG("wlan device not exist!\n");
    }

    wlan_app_disc("wlan0");
    wlan_app_exit();

    netif = wlan_app_netif_get("wlan0");
    if (netif == NULL) {
        WIFI_PROC_LOG("DisconnectWiFiDriver ok\n");
    }
    else{
        WIFI_PROC_LOG("DisconnectWiFiDriver fail\n");
    }
    WIFI_PROC_LOG("\n[DisconnectWiFiDriver] END.\n");
	s_wifi_context.sdio_run = 0;
}


//手机二维码解析字符串:WIFI:T:WPA;S:ipc_test;P:123456789;
int wifi_string_analyze(const char*wifi_string,dev_wifi_set*wifi_set)
{
	if(wifi_string ==NULL || wifi_set == NULL){
		WIFI_PROC_LOG("param verify err.\n");
		return -1;
	}
	WIFI_PROC_LOG("wifi_string:%s\n",wifi_string);
	memset(wifi_set,0,sizeof(dev_wifi_set));
	
	char tmpstr[64];
	if(strncmp(wifi_string, "WIFI:", 5) != 0){
		WIFI_PROC_LOG("head error.\n");
		return -2;
	}
	//T:WPA;S:ipc_test;P:123456789;
	const char* str_base = wifi_string+5;//strlen("WIFI:")
	const char* str_ptr = strstr(str_base,"S:");
	if(str_ptr == NULL){
		WIFI_PROC_LOG("analyze ssid error.\n");
		return -3;
	}
	str_base = str_ptr+2;//strlen("S:")
	int idx = 0;
	memset(tmpstr,0,64);
	while(str_base[0] != ';'&&str_base[0] != '\0'){
		tmpstr[idx]=str_base[0];
		str_base++;
		idx ++;
	}
	WIFI_PROC_LOG("tmpstr:%s\n",tmpstr);
	
	strcpy(wifi_set->ssid, tmpstr);
	
	WIFI_PROC_LOG("ssid:%s\n",wifi_set->ssid);

	str_ptr = strstr(str_base,"P:");
	if(str_ptr == NULL){
		WIFI_PROC_LOG("analyze password  error.\n");
		return -4;
	}
	str_base = str_ptr+2;//strlen("P:")
	idx = 0;
	memset(tmpstr,0,64);
	while(str_base[0] != ';'&&str_base[0] != '\0'){
		tmpstr[idx]=str_base[0];
		str_base++;
		idx ++;
	}
	WIFI_PROC_LOG("tmpstr:%s\n",tmpstr);
	strcpy(wifi_set->wifi_pw, tmpstr);
	WIFI_PROC_LOG("wifi_pw:%s\n",wifi_set->wifi_pw);

	wifi_set->enable = 1;
	wifi_set->mode= CFG_WIFI_MODE_STA;
	wifi_set->re_cnn_timeout = 300;
	wifi_set->ip_set.client_ip_set.dhcpc_enable = 1;

	return 0;
}

int wifi_init(dev_wifi_set*wifi_set)
{
#if WIFI_EN
	if(wifi_set == NULL){
		//外面不配置则使用sd 卡配置
		wifiset_update();
	}else{
		g_dev_wifi_set.enable = 1;
		strcpy(g_dev_wifi_set.ssid, wifi_set->ssid);
		strcpy(g_dev_wifi_set.wifi_pw, wifi_set->wifi_pw);
		g_dev_wifi_set.mode= wifi_set->mode;
		g_dev_wifi_set.re_cnn_timeout = wifi_set->re_cnn_timeout;
		g_dev_wifi_set.ip_set.client_ip_set.dhcpc_enable = wifi_set->ip_set.client_ip_set.dhcpc_enable;
	}
	
	return 0;

#endif
	return -1;
}

int wifi_deinit()
{
#if WIFI_EN
	WIFI_PROC_LOG("s_wifi_context.wifi_status WIFI_CLOSE\n");

	s_wifi_context.wifi_task_run = 0;
	while(s_wifi_context.wifi_task_runing == 1){
		vTaskDelay(10);
	}

	return 0;

#endif
	return -1;
}
int set_wifi_low_power()
{
	s_wifi_context.wifi_switch = 0;
	return 0;
}

int start_wifi(unsigned char force)
{
	WIFI_PROC_LOG("begin,force,%d \n",force);

#if WIFI_EN
	if(force == 1){
		wifi_stop();
	}
	if(s_wifi_context.sdio_run == 0){
		wifi_dev_init();
	}

	//已经启动
	ENU_WIFI_MODE_SET mode = CFG_WIFI_MODE_BUTT;
	if(s_wifi_context.wifi_task_runing == 1){
		mode = g_dev_wifi_set.mode;
	}
	
	if(g_dev_wifi_set.enable == 0){
		//没有初始化，则到SD卡查找
		WIFI_PROC_LOG("enable is 0,so get from sd card.\n");
		wifiset_update();
	}

	//正在执行中,且与即将展开的模式不一致，关闭之前线程
	if(mode != CFG_WIFI_MODE_BUTT && mode != g_dev_wifi_set.mode){
		s_wifi_context.wifi_task_run = 0;
		while(s_wifi_context.wifi_task_runing == 1){
			vTaskDelay(10);
		}
	}
	g_ticks = xTaskGetTickCount();

	if(g_dev_wifi_set.mode == CFG_WIFI_MODE_AP){
		//start_ap();
	}else{
		start_sta();
	}
	
	return 1;
	

#endif
	return -1;
}
int wifi_stop()
{
	WIFI_PROC_LOG("begin\n");

#if WIFI_EN
#if IPERF_TEST_SET
	iperf2_stop();
#endif
	

	if(g_dev_wifi_set.mode == CFG_WIFI_MODE_AP){
		stop_ap();
	}else{
		stop_sta();
	}
	
	s_wifi_context.low_power = 0;
	return 0;
#endif
	return -1;

}
//网络就绪返回1,进行中返回0,不支持WIFI返回-1
int get_wifi_statue(E_WIFI_STATUS*wifi_status,E_DHCP_STATUS*dhcp_status)
{

#if WIFI_EN
	if (wifi_status != NULL)
		*wifi_status = s_wifi_context.wifi_status;
	if (dhcp_status != NULL)
		*dhcp_status = s_wifi_context.dhcp_status;

	if(s_wifi_context.wifi_status == WIFI_OPEN_SUCCESS && 
	  (s_wifi_context.dhcp_status == DHCPC_SUCCESS ||s_wifi_context.dhcp_status == DHCPD_SUCCESS)){
		//WIFI_PROC_LOG("wifi & dhcp work normal.\n");
		return 1;
	}else{
		//WIFI_PROC_LOG("wifi work error,wifi_status:%d,dhcp_status:%d\n",s_wifi_context.wifi_status,s_wifi_context.dhcp_status);
		return 0;
	}
	
#endif

	return -1;
}

#if 0
void bt_configWifi(const u8 *wifiParm, u32 len)
{
	dev_wifi_set wifi_set;
	
		
	if ((wifiParm)&&(len >3))
	{
		const u8 *sepPos = strchr(wifiParm, '@');
		u8 *pw = sepPos+1;
		if (sepPos && pw) {
				wifi_set.mode = SD_CFG_WIFI_MODE_STA;
				strncpy(wifi_set.ssid,wifiParm, sepPos-wifiParm);
				wifi_set.ssid[sepPos-wifiParm] = '\0';
				strcpy(wifi_set.wifi_pw,pw);
				printf("ssid:%s", wifi_set.ssid);
				printf("wifi_pw:%s", wifi_set.wifi_pw);

				wifi_init(&wifi_set);
		}
	}
}
#endif


E_BT_WIFI_CONFIG_STATUS bt_configWifi(const u8 *wifiParm, u32 len)
{
	dev_wifi_set wifi_set;
    // 1. ½âÎö JSON
    cJSON *json = cJSON_Parse_or1k((const char *)wifiParm);
    if (!json) {
        printf("JSON parse error: %s\n", cJSON_GetErrorPtr_or1k());
        return BT_WIFI_CONFIG_ERR_JSON_PARSE;
    }

    // 2. »ñÈ¡ SSID ºÍÃÜÂë
    cJSON *ssid_item = cJSON_GetObjectItem_or1k(json, "ssid");
    cJSON *password_item = cJSON_GetObjectItem_or1k(json, "password");

    // 3. ¼ì²é SSID ºÍÃÜÂëÊÇ·ñÓÐÐ§
    if (!ssid_item || ssid_item->type != cJSON_String || !ssid_item->valuestring ||
        !password_item || password_item->type != cJSON_String || !password_item->valuestring) {
        printf("Invalid or missing ssid/password in JSON\n");
        cJSON_Delete_or1k(json);
        return BT_WIFI_CONFIG_ERR_INVALID_PARAMS;
    }

    // 新增：WiFi 密码有效性检查
    size_t pw_len = strlen(password_item->valuestring);
    if (pw_len != 0 && pw_len < 8) {
        printf("Invalid WiFi password length: %zu (must be empty or >=8 characters)\n", pw_len);
        cJSON_Delete_or1k(json);
        return BT_WIFI_CONFIG_ERR_INVALID_PASSWORD;
    }

    // 4. ¼ì²é SSID ºÍÃÜÂë³¤¶È
    if (strlen(ssid_item->valuestring) >= sizeof(wifi_set.ssid) ||
        pw_len >= sizeof(wifi_set.wifi_pw)) {
        printf("SSID or password too long\n");
        cJSON_Delete_or1k(json);
        return BT_WIFI_CONFIG_ERR_INVALID_PARAMS;
    }
    // 5. ÅäÖÃ WiFi ÉèÖÃ
    wifi_set.mode = SD_CFG_WIFI_MODE_STA;
	wifi_set.re_cnn_timeout = 120;//set reconnect timeout val
    strcpy(wifi_set.ssid, ssid_item->valuestring);
    strcpy(wifi_set.wifi_pw, password_item->valuestring);

    // 6. ´òÓ¡ÅäÖÃ
    printf("ssid: %s\n", wifi_set.ssid);
    printf("wifi_pw: %s\n", wifi_set.wifi_pw);

#if 1 // wifi connect
    printf("wifi_init\n");
    int ret = wifi_init(&wifi_set);
    if (ret != 0) {
        printf("Failed to start WiFi station mode\n");
        cJSON_Delete_or1k(json);
        return BT_WIFI_CONFIG_ERR_WIFI_INIT_FAIL;
    }

    // 强制关闭当前WiFi
    wifi_stop();
    // 等待线程退出（增加超时循环）
    int timeout = 100;  // 5秒超时（50ms * 100）
    while (s_wifi_context.wifi_task_runing == 1 && timeout-- > 0) {
        osal_task_sleep(50);
    }
    // if (timeout <= 0) {
    //     // 日志错误，返回失败
    //     WIFI_PROC_LOG("WiFi thread shutdown timeout\n");
    //     return BT_WIFI_CONFIG_ERR_WIFI_SHUTDOWN_FAIL;
    // }

    // ATBM特定：显式关闭驱动
    atbm_wifi_sta_disjoin_ap();
    atbm_wifi_off(1);  // 完全关闭WiFi模块
    osal_task_sleep(500);  // 延迟500ms确保驱动释放

    // 重置状态机
    s_wifi_context.wifi_status = WIFI_CLOSE;
    s_wifi_context.dhcp_status = DHCP_UNINIT;
    s_wifi_context.disconnect_time = 0;
    s_wifi_context.re_connect_idx = 0;

    // 然后启动新配置
    start_wifi(0);  // 无需force=1，已手动关闭
    printf("start_wifi\n");

    int try = 0;
    const int max_tries = 60; 
    while (get_wifi_statue(NULL, NULL) == 0 && try < max_tries) {
        osal_task_sleep(500);
        try++;
    }

    if (get_wifi_statue(NULL, NULL) != 1) {
        printf("WiFi connection failed: wifiStatus=%d, DHCP_state=%d\n", s_wifi_context.wifi_status, s_wifi_context.dhcp_status);
        wifi_close();
        cJSON_Delete_or1k(json);
        return BT_WIFI_CONFIG_ERR_CONNECTION_TIMEOUT;  // 或根据具体状态细化
    }
#endif
    f_unlink("1:/config_param.json");
    cJSON *root = cJSON_CreateObject_or1k();
    cJSON *JSwifi_set = cJSON_CreateObject_or1k();
    cJSON_AddItemToObject_or1k(JSwifi_set, "mode", cJSON_CreateString_or1k("STA"));
    cJSON_AddItemToObject_or1k(JSwifi_set, "ssid", cJSON_CreateString_or1k(wifi_set.ssid));
    cJSON_AddItemToObject_or1k(JSwifi_set, "password", cJSON_CreateString_or1k(wifi_set.wifi_pw));
	cJSON_AddItemToObject_or1k(JSwifi_set, "re_cnn_timeout", cJSON_CreateNumber_or1k(300));
    cJSON_AddItemToObject_or1k(JSwifi_set, "tmp_set", cJSON_CreateNumber_or1k(1));
    cJSON_AddItemToObject_or1k(root, "wifi_set", JSwifi_set);

    char *json_str = cJSON_Print_or1k(root);
    if (!json_str) {
        printf("Failed to create JSON string\n");
        cJSON_Delete_or1k(root);
        cJSON_Delete_or1k(json);
        wifi_close();
        return BT_WIFI_CONFIG_ERR_JSON_CREATE_FAIL;
    }

    FIL file;
    FRESULT res;
    res = f_open(&file, "1:/config_param.json", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("Failed to open file: %d\n", res);
        cJSON_Delete_or1k(root);
        cJSON_Delete_or1k(json);
        wifi_close();
        return BT_WIFI_CONFIG_ERR_FILE_OPEN_FAIL;
    }

    UINT bytes_written;
    res = f_write(&file, json_str, strlen(json_str), &bytes_written);
    if (res != FR_OK || bytes_written != strlen(json_str)) {
        printf("Failed to write file: %d, written: %u\n", res, bytes_written);
        f_close(&file);
        cJSON_Delete_or1k(root);
        cJSON_Delete_or1k(json);
        wifi_close();
        return BT_WIFI_CONFIG_ERR_FILE_WRITE_FAIL;
    }

    f_close(&file);
    printf("Created /config_param.json successfully\n");
    cJSON_Delete_or1k(root);
    cJSON_Delete_or1k(json);

    return BT_WIFI_CONFIG_SUCCESS;
    return 0;
}


