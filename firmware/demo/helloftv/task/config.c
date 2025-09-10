/****************************************************************************
**
 **                              CONFIGURE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  CONFIGURE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : config.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
#include "spi/spi.h"

#define DEFAULT_SSID_STR 	"JRXCam"
#define DEFAULT_PWD_STR 	"12345678"

typedef struct
{
	INT32U flag[127];	// must  CFG_ID_MAX < 127
	network_config network_set;
	INT32U CheckSum;
}SYSTEM_FLAY;		// size must 256 aligned 
static SYSTEM_FLAY SysFlay;
static int configDirt=0;

/*******************************************************************************
* Function Name  : userConfigReset
* Description    : reset or default user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigReset(void)
{
    printf("config : reset\n");
	configSet(CONFIG_ID_YEAR,             2017);
	configSet(CONFIG_ID_MONTH,            1);
	configSet(CONFIG_ID_MDAY,             1);
	configSet(CONFIG_ID_WDAY,             4);
	configSet(CONFIG_ID_HOUR,             0);
	configSet(CONFIG_ID_MIN,              0);
	configSet(CONFIG_ID_SEC,              0);
	configSet(CONFIG_ID_LANGUAGE,         R_ID_STR_LAN_SCHINESE);
	configSet(CONFIG_ID_AUTOOFF,          R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_SCREENSAVE,       R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FREQUNCY,         R_ID_STR_COM_50HZ);
	configSet(CONFIG_ID_ROTATE,           R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FILLIGHT,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_RESOLUTION,       R_ID_STR_RES_HD);
	configSet(CONFIG_ID_TIMESTAMP,        R_ID_STR_COM_ON);
	configSet(CONFIG_ID_MOTIONDECTION,    R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PARKMODE,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_GSENSOR,          R_ID_STR_COM_MIDDLE);
	configSet(CONFIG_ID_KEYSOUND,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_IR_LED,           R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_LOOPTIME,         R_ID_STR_TIM_3MIN);
	configSet(CONFIG_ID_AUDIOREC,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_EV,               R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_WBLANCE,          R_ID_STR_ISP_AUTO);
	configSet(CONFIG_ID_PRESLUTION,      R_ID_STR_RES_1M);
	configSet(CONFIG_ID_PFASTVIEW,       R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PTIMESTRAMP,     R_ID_STR_COM_ON);
	configSet(CONFIG_ID_PEV,             R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_VOLUME,          R_ID_STR_COM_LOW);
	configSet(CONFIG_ID_THUMBNAIL,       R_ID_STR_COM_ON);
	configSet(CONFIG_ID_GSENSORMODE,     R_ID_STR_COM_ON);

	configSet(CONFIG_ID_WIFIPREVIEWQUALITY,R_ID_STR_COM_MIDDLE);
    configSet(CONFIG_ID_WIFIPREVIERES,R_ID_STR_RES_VGA);
    configSet(CONFIG_ID_VIDEOQUALITY,R_ID_STR_COM_MIDDLE);
    configSet(CONFIG_ID_PHOTOBURST,R_ID_STR_COM_OFF);
    configSet(CONFIG_ID_PHOTOTIMELAPSE,R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PHOTOTIMESTAMP,        R_ID_STR_COM_ON);
	configSet(CONFIG_ID_SHARPNESS,        R_ID_STR_COM_STANDARD);
	configSet(CONFIG_ID_ISO,        R_ID_STR_ISP_AUTO);
	configSet(CONFIG_ID_DEBOUNCE,        R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_DATEFORMAT,         R_ID_STR_COM_YMD);
	configSet(CONFIG_ID_PHOTOQUALITY,        R_ID_STR_COM_STANDARD);
	configSet(CONFIG_ID_WDR,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PICIDENTIFY,         R_ID_STR_COM_ON);
	set_network_cfg(NULL);
	userConfigSave();

	return 0;
}

static void config_read(INT32U buff,INT32U len)
{
	spi_read(FLASH_START,buff,len);
}
static void config_write(INT32U buff,INT32U len)
{
	spi_write(FLASH_START,buff,len);
}

u32 configCheckSumCal(void)
{
	u32 CheckSum=0,i;
	for(i=0;i<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]);i++)
	{
		CheckSum+=SysFlay.flag[i];
	}
	u32*_ptr = (u32*)(&(SysFlay.network_set));
	printf("configCheckSumCal extend lenght:%u\n",sizeof(network_config));
	for(i=0;i < sizeof(network_config)/sizeof(u32);i++)
	{
		CheckSum+=_ptr[i];
	}
	printf("configCheckSumCal extend lenght:%u,CheckSum:0x%x\n",i,CheckSum);
	if(CheckSum==0)
		CheckSum=0xaa55aa55;
	
	return CheckSum;
}

/*******************************************************************************
* Function Name  : userConfigSave
* Description    : save user configure value to spi flash
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigSave(void)
{
	network_config*f_network_set = &(SysFlay.network_set);
	//printf("userConfigSave:%s\n",f_network_set->cfg_ssid);

	if(configDirt)
	{
		configDirt=0;
	  	SysFlay.CheckSum=configCheckSumCal();
		printf("write config check sum:0x%x\n",SysFlay.CheckSum);
		config_write((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
		return 0;
	}
    printf("no need to write config\n");
}
void configSaveClear(void)
{
	configDirt=0;
}
int get_network_default(network_config*network_set)
{
	if(NULL == network_set){
		return -1;
	}
	memset(network_set,0,sizeof(network_config));

	network_set->mode = 1;						//1:ap mode,0:sta mode
	network_set->dhcpc_enable = 1;				//sta mode dhcpc or static IP address
	network_set->re_cnn_timeout = 120;			//sta mode disconnect reconnect timeout

	//"192.168.100.1",·´Ðò
	network_set->ip_addr0 = 0x0164a8c0;			//static client IP && server IP
	//"192.168.100.2",·´Ðò
	network_set->ip_addr1 = 0x0264a8c0;			//static client mask && server dhcpd begin
	//"192.168.100.128",·´Ðò
	network_set->ip_addr2 = 0x8064a8c0;			//static client gatway && server dhcpd end
	strcpy(network_set->cfg_ssid,DEFAULT_SSID_STR);//static client join ap ssid && server ap ssid
	strcpy(network_set->pwd,DEFAULT_PWD_STR);//static client join ap password && server ap password

	return 0;
}

int set_network_cfg(network_config*network_set)
{
	network_config*f_network_set = &(SysFlay.network_set);
	
	if(network_set == NULL){
		get_network_default(f_network_set);
	}else if(network_set != f_network_set){
		memcpy(f_network_set,network_set,sizeof(network_config));
	}else{
		printf("save the same addr,write flash directly\n");
	}
	printf("set_network_cfg:%s\n",f_network_set->cfg_ssid);
	configDirt=1;
	userConfigSave();
}

network_config *get_network_cfg()
{
	return &(SysFlay.network_set);
}

/*******************************************************************************
* Function Name  : userConfigInitial
* Description    : initial user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                      0 : always
*******************************************************************************/
s32_t userConfigInitial(void)
{
    INT32U CheckSum=0;
	config_read((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
	CheckSum=configCheckSumCal();
	printf("read config check sum:0x%x,0x%x\n",CheckSum,SysFlay.CheckSum);
	if((CheckSum!=SysFlay.CheckSum)||(0 == CheckSum))
	{
		printf("config error,reset flag.0x%x\n",SysFlay.CheckSum);
		memset(&SysFlay,0,sizeof(SYSTEM_FLAY));
		userConfigReset();
	}
    return 0;
}

/*******************************************************************************
* Function Name  : configSet
* Description    : set configure value
* Input          : u8_t configId : configure id
*                  u32_t value   : configure value
* Output         : None
* Return         : none
*******************************************************************************/
void configSet(u8_t configId,u32_t value)
{
	if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
	{
		if(SysFlay.flag[configId]!=value)
		{
			configDirt=1;
    		SysFlay.flag[configId]=value;
		}
	}
}

/*******************************************************************************
* Function Name  : userCofigGetValue
* Description    : get configure value in configure table
* Input          : u8_t configId : configure id
* Output         : None
* Return         : u32_t : configure value
*******************************************************************************/
u32_t configGet(u8_t configId)
{
    if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
		return SysFlay.flag[configId];
    return 0;
}

const uint8_t *getWifiSSID(void)
{
	return (uint8_t*)SysFlay.network_set.cfg_ssid;
}
void setWifiSSID(uint8_t *ssid)
{
	if (ssid == NULL)
		return;
	printf("change ssid:%s\n",ssid);
	if(SysFlay.network_set.cfg_ssid != ssid){
		memset(SysFlay.network_set.cfg_ssid, 0, WIFI_SSID_LEN_MAX);
		strcpy(SysFlay.network_set.cfg_ssid,ssid);
	}
	configDirt=1;

	userConfigSave();
}
uint8_t *getWifiPWD(void)
{
	return (uint8_t*)SysFlay.network_set.pwd;
}
void setWifiPWD(uint8_t *pwd)
{
	if (pwd == NULL)
		return;
	
	memset(SysFlay.network_set.pwd, 0, WIFI_PWD_LEN_MAX);
	strcpy(SysFlay.network_set.pwd,pwd);
	configDirt=1;

	userConfigSave();
}
const uint8_t *getWifiMAC(void)
{
	return (uint8_t*)SysFlay.network_set.mac;
}
void setWifiMAC(uint8_t *mac)
{
	if (mac == NULL)
		return;
	if(mac != SysFlay.network_set.mac){
		memset(SysFlay.network_set.mac, 0, WIFI_MAC_LEN_MAX);
		strcpy(SysFlay.network_set.mac,mac);
	}

	configDirt=1;

	userConfigSave();

}



