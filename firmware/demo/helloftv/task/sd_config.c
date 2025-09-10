#include "application.h"
#include "fatfs/ff.h"
#include "cjson/cJSON.h"
#include "sd_config.h"



//#include "user_config.h"
extern void *pvPortMalloc(size_t xWantedSize);
extern void vPortFree(void *pv);


//#include "comm_util.h"
#define SD_CONFIG_FILE_PATH "1:config_param.json"


#define SD_CFG_DEBUG(msg...) \
	do{ \
		printf("[ %s, %d]",__func__, __LINE__); \
		printf(msg); \
		printf("\r\n"); \
	}while(0)

static sd_wifi_set 	g_sd_wifi_config={0};
static sd_ip_set 	g_sd_ip_set={0};
static sd_iperf_set g_sd_iperf_config={0};


/*
"ip_set": {
  "enable": 1,
  "server_mode": 0,
  "server_ipaddr": "192.168.66.1",
  "dhcpd_begin_ipaddr": "192.168.66.2",
  "dhcpd_end_ipaddr": "192.168.66.128",
  "dhcpc_enable": 1,
  "client_ipaddr": "192.168.88.199",
  "client_ipmask": "255.255.255.0",
  "client_gateway": "192.168.88.1",
  "tmp_set": 1
}
*/
sd_iperf_set* get_ip_node(cJSON *json_root)
{

	cJSON* json_ip_item = cJSON_GetObjectItem_or1k(json_root,"ip_set");
	g_sd_ip_set.enable = 0;

	if(NULL != json_ip_item){
		
		cJSON*	ip_enable_item = cJSON_GetObjectItem_or1k(json_ip_item,"enable");
		if(NULL != ip_enable_item){
			g_sd_ip_set.enable = ip_enable_item->valueint;
			SD_CFG_DEBUG("Set ip set enable:%u\n",g_sd_ip_set.enable);
		}else{
			SD_CFG_DEBUG("Don't ip set enable.\n");
			return NULL;
		}	
		char*ipaddr = NULL;
		cJSON*  server_ipaddr_item = cJSON_GetObjectItem_or1k(json_ip_item,"server_ipaddr");
		if(NULL != server_ipaddr_item){
			ipaddr =  server_ipaddr_item->valuestring;
			strcpy(g_sd_ip_set.server_ipaddr,ipaddr);
			SD_CFG_DEBUG("Set ip server ipaddr:%s\n",ipaddr);
		}

		cJSON*  dhcpd_begin_ipaddr_item = cJSON_GetObjectItem_or1k(json_ip_item,"dhcpd_begin_ipaddr");
		if(NULL != dhcpd_begin_ipaddr_item){
			ipaddr =  dhcpd_begin_ipaddr_item->valuestring;
			strcpy(g_sd_ip_set.dhcpd_begin_ipaddr,ipaddr);
			SD_CFG_DEBUG("Set dhcpd begin ipaddr:%s\n",ipaddr);
		}

		cJSON*  dhcpd_end_ipaddr_item = cJSON_GetObjectItem_or1k(json_ip_item,"dhcpd_end_ipaddr");
		if(NULL != dhcpd_end_ipaddr_item){
			ipaddr =  dhcpd_end_ipaddr_item->valuestring;
			strcpy(g_sd_ip_set.dhcpd_end_ipaddr,ipaddr);
			SD_CFG_DEBUG("Set dhcpd end ipaddr:%s\n",ipaddr);
		}

		/*dhcpc mode*/
		cJSON* dhcpc_enable_item = cJSON_GetObjectItem_or1k(json_ip_item,"dhcpc_enable");
		if(NULL == dhcpc_enable_item){
			SD_CFG_DEBUG("Set iperf server port null \n");
			return NULL;
		}
		g_sd_ip_set.dhcpc_enable = dhcpc_enable_item->valueint;
		SD_CFG_DEBUG("Set dhcpc enable :%u\n",g_sd_ip_set.dhcpc_enable);

		cJSON*  client_ipaddr_item = cJSON_GetObjectItem_or1k(json_ip_item,"client_ipaddr");
		if(NULL != client_ipaddr_item){
			ipaddr =  client_ipaddr_item->valuestring;
			strcpy(g_sd_ip_set.client_ipaddr,ipaddr);
			SD_CFG_DEBUG("Set client static ipaddr:%s\n",ipaddr);
		}

		cJSON*  client_ipmask_item = cJSON_GetObjectItem_or1k(json_ip_item,"client_ipmask");
		if(NULL != client_ipmask_item){
			ipaddr =  client_ipmask_item->valuestring;
			strcpy(g_sd_ip_set.client_ipmask,ipaddr);
			SD_CFG_DEBUG("Set client begin ipmask:%s\n",ipaddr);
		}
		cJSON*  client_gateway_item = cJSON_GetObjectItem_or1k(json_ip_item,"client_gateway");
		if(NULL != client_gateway_item){
			ipaddr =  client_gateway_item->valuestring;
			strcpy(g_sd_ip_set.client_gateway,ipaddr);
			SD_CFG_DEBUG("Set clien gateway ipaddr:%s\n",ipaddr);
		}
		cJSON*  json_ip_tmp_item = cJSON_GetObjectItem_or1k(json_ip_item,"tmp_set");
		if(NULL != json_ip_tmp_item){
			g_sd_ip_set.tmp_set =  json_ip_tmp_item->valueint;
			SD_CFG_DEBUG("Set ip config from SDCard is_tmp:%d\n",g_sd_ip_set.tmp_set);
		}

	}else{
		return NULL;
	}
	return &g_sd_ip_set;

}


/*
"iperf_set":{
	"enable":1,
	"server_mode":0,
	"server_ipaddr":"192.168.88.25",
	"server_port":5001,
	"udp_mode":0,
	"send_bps":20,
	"time_duration":60,
	"info_printf_duration":1,
}
*/
sd_iperf_set* get_iperf_node(cJSON *json_root)
{

	cJSON* json_iperf_item = cJSON_GetObjectItem_or1k(json_root,"iperf_set");
	g_sd_iperf_config.enable = 0;

	if(NULL != json_iperf_item){
		
		cJSON*	json_iperf_enable_item = cJSON_GetObjectItem_or1k(json_iperf_item,"enable");
		if(NULL != json_iperf_enable_item){
			g_sd_iperf_config.enable =  json_iperf_enable_item->valueint;
			SD_CFG_DEBUG("Set iperf enable:%u\n",g_sd_iperf_config.enable);
		}else{
			SD_CFG_DEBUG("Don't set iperf enable.\n");
			return NULL;
		}
	
		cJSON* json_iperf_server_mode_item = cJSON_GetObjectItem_or1k(json_iperf_item,"server_mode");
		if(NULL == json_iperf_server_mode_item){
			SD_CFG_DEBUG("Set iperf server mode null \n");
			return NULL;
		}
		g_sd_iperf_config.server_mode = json_iperf_server_mode_item->valueint;
		
		char*ipaddr = NULL;
		cJSON*  json_iperf_ipaddr_item = cJSON_GetObjectItem_or1k(json_iperf_item,"server_ipaddr");
		if(NULL != json_iperf_ipaddr_item){
			ipaddr =  json_iperf_ipaddr_item->valuestring;
			strcpy(g_sd_iperf_config.server_ipaddr,ipaddr);
			SD_CFG_DEBUG("Set iperf server ipaddr:%s\n",ipaddr);
		}
		
		cJSON* json_iperf_server_port_item = cJSON_GetObjectItem_or1k(json_iperf_item,"server_port");
		if(NULL == json_iperf_server_port_item){
			SD_CFG_DEBUG("Set iperf server port null \n");
			return NULL;
		}
		g_sd_iperf_config.server_port = json_iperf_server_port_item->valueint;

		cJSON* json_iperf_udp_mode_item = cJSON_GetObjectItem_or1k(json_iperf_item,"udp_mode");
		if(NULL == json_iperf_udp_mode_item){
			SD_CFG_DEBUG("Set iperf udp mode null \n");
			return NULL;
		}
		g_sd_iperf_config.udp_mode = json_iperf_udp_mode_item->valueint;
		
		cJSON* json_iperf_send_bps_item = cJSON_GetObjectItem_or1k(json_iperf_item,"send_bps");
		if(NULL == json_iperf_send_bps_item){
			SD_CFG_DEBUG("Set iperf send bps null \n");
			return NULL;
		}
		g_sd_iperf_config.send_bps = json_iperf_send_bps_item->valueint;

		cJSON* json_iperf_time_duration_item = cJSON_GetObjectItem_or1k(json_iperf_item,"time_duration");
		if(NULL == json_iperf_time_duration_item){
			SD_CFG_DEBUG("Set iperf time duration null \n");
			return NULL;
		}
		g_sd_iperf_config.time_duration = json_iperf_time_duration_item->valueint;
		
		cJSON* json_iperf_info_printf_duration_item = cJSON_GetObjectItem_or1k(json_iperf_item,"info_printf_duration");
		if(NULL == json_iperf_info_printf_duration_item){
			SD_CFG_DEBUG("Set iperf info printf duration null \n");
			return NULL;
		}
		g_sd_iperf_config.info_printf_duration = json_iperf_info_printf_duration_item->valueint;


	}else{
		return NULL;
	}
	return &g_sd_iperf_config;

}
/*
"wifi_set":{
	"mode":"STA",
	"ssid":"ipc_test",	
	"password":"123456789",
	"re_cnn_timeout":120,
	"tmp_set":1,
},
*/
sd_wifi_set* get_wifi_node(cJSON *json_root)
{

	cJSON* json_wifiset_item = cJSON_GetObjectItem_or1k(json_root,"wifi_set");

	if(NULL != json_wifiset_item){
		
		char*mode = NULL;
		cJSON*	json_wifiset_mode_item = cJSON_GetObjectItem_or1k(json_wifiset_item,"mode");
		if(NULL != json_wifiset_mode_item){
			mode =  json_wifiset_mode_item->valuestring;
			int mode_str_len = strlen(mode);
			if(mode_str_len == 2){
				if( (mode[0]=='A'|| mode[0]=='a') &&
					(mode[1]=='P'|| mode[1]=='p')){
					g_sd_wifi_config.mode = SD_CFG_WIFI_MODE_AP;
				}
			}else if(mode_str_len == 3){
				if( (mode[0]=='S'|| mode[0]=='s')&&
					(mode[1]=='T'|| mode[1]=='t')&&
					(mode[2]=='A'|| mode[2]=='a')){
					g_sd_wifi_config.mode = SD_CFG_WIFI_MODE_STA;
				}
			}else{
				return NULL;
			}
			
			SD_CFG_DEBUG("Set wifi config from SDCard mode:%s\n",mode);
		}else{
			SD_CFG_DEBUG("Don't set wifi mode:%s\n",mode);
			return NULL;

		}
	
		cJSON* json_wifiset_ssid_item = cJSON_GetObjectItem_or1k(json_wifiset_item,"ssid");
		if(NULL == json_wifiset_ssid_item){
			SD_CFG_DEBUG("Set wifi config from SDCard  ssid item null \n");
			return NULL;
		}
		char*ssid = json_wifiset_ssid_item->valuestring;
		strcpy(g_sd_wifi_config.ssid,ssid);
		SD_CFG_DEBUG("Set wifi config from SDCard ssid:%s \n",ssid);
		
		char*wifi_pw = NULL;
		cJSON*  json_wifiset_pw_item = cJSON_GetObjectItem_or1k(json_wifiset_item,"password");
		if(NULL != json_wifiset_pw_item){
			wifi_pw =  json_wifiset_pw_item->valuestring;
			strcpy(g_sd_wifi_config.wifi_pw,wifi_pw);
			SD_CFG_DEBUG("Set wifi config from SDCard password:%s\n",wifi_pw);
		}
		//|¨¬?????¡§¡é??¨º?3?¡§o?¨¤¡§o?¨¤??,|¨¬¡ê¡è??s
		cJSON*	json_wifiset_re_cnnct_item = cJSON_GetObjectItem_or1k(json_wifiset_item,"re_cnn_timeout");
		if(NULL != json_wifiset_re_cnnct_item){
			g_sd_wifi_config.re_cnn_timeout =  json_wifiset_re_cnnct_item->valueint;
			SD_CFG_DEBUG("Set wifi config from SDCard re_cnn_timeout:%d\n",g_sd_wifi_config.re_cnn_timeout);
		}

		cJSON*  json_wifiset_tmp_item = cJSON_GetObjectItem_or1k(json_wifiset_item,"tmp_set");
		if(NULL != json_wifiset_tmp_item){
			g_sd_wifi_config.tmp_set =  json_wifiset_tmp_item->valueint;
			SD_CFG_DEBUG("Set wifi config from SDCard is_tmp:%d\n",g_sd_wifi_config.tmp_set);
		}

		g_sd_wifi_config.enable = 1;

	}else{
		return NULL;
	}
	return &g_sd_wifi_config;

}

sd_wifi_set* get_sd_wifi_config()
{
	if(g_sd_wifi_config.enable == 1){
		return &g_sd_wifi_config;
	}else{
		return NULL;
	}
}
sd_ip_set* get_sd_ip_config()
{
	if(g_sd_ip_set.enable == 1){
		return &g_sd_ip_set;
	}else{
		return NULL;
	}
}
sd_iperf_set* get_sd_iperf_config()
{
	if(g_sd_iperf_config.enable == 1){
		return &g_sd_iperf_config;
	}else{
		return NULL;
	}
}

int check_sd_config_file()
{
	SD_CFG_DEBUG("begin\n");
	unsigned int ret = 0;
	unsigned int file_lenght = 0;
	char* file_buff = NULL;
	unsigned int read_lenght = 0;

	FILE fd;
	memset(&g_sd_wifi_config,0,sizeof(sd_wifi_set));
	
	if(!appGetSdStatus()){//fs init not ok
		SD_CFG_DEBUG("file system not ready\n");
		return -1;
	}

	FILINFO fno;
	ret = f_stat(SD_CONFIG_FILE_PATH, &fno);
	if(FR_OK!= ret){
		SD_CFG_DEBUG("get file info fail,filepath:%s\n",SD_CONFIG_FILE_PATH);
		return -1;
	}
	
	ret = f_open(&fd,SD_CONFIG_FILE_PATH,FA_READ);
	if(ret != FR_OK){//
		SD_CFG_DEBUG("config file not exist,ret:%d,path:%s\n",ret,SD_CONFIG_FILE_PATH);
		return -1;
	}
	
	file_lenght = fno.fsize;
	if(file_lenght > 0){
		file_buff = pvPortMalloc(file_lenght + 64);
		if(file_buff == NULL){
			SD_CFG_DEBUG("malloc buffer fail,lenght:%d\n",file_lenght+64);
			goto ERR;
		}
		memset(file_buff,0,file_lenght + 64);
		f_lseek(&fd,0);
		ret = f_read(&fd,file_buff,file_lenght,&read_lenght);
		if(ret != FR_OK){//read failed
			SD_CFG_DEBUG("read failed,ret:%d\n",ret);
			goto ERR;
		}
		cJSON *json_root = cJSON_Parse_or1k(file_buff);
		if(NULL == json_root)
		{
			SD_CFG_DEBUG("set sta from sdcard json_root parse null\n");
			goto ERR;
		}

		get_wifi_node(json_root);
		get_ip_node(json_root);
		get_iperf_node(json_root);

		cJSON_Delete_or1k(json_root);


	}else{
		SD_CFG_DEBUG("read file fail,lenght 0\n");
		goto ERR;
	}
	if(file_buff != NULL){
		vPortFree(file_buff);
	}
	f_close(&fd);
	return 1;

ERR:
	if(file_buff != NULL){
		vPortFree(file_buff);
	}
	f_close(&fd);
	return -1;
}



