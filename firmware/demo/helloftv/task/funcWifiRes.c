#include"uiWin.h"

enum
{
	WIFI_MODE_ID=0,
	WIFI_HIDE_RECT_ID,
	WIFI_REC_TIME_ID,
	WIFI_RESOLUTION_ID,
	WIFI_MD_ID,// motion detect
	WIFI_MONITOR_ID, // parking monitoring
	WIFI_IRLED_ID,
	WIFI_LOCK_ID,
	WIFI_SD_ID,
	WIFI_MIC_ID,
	WIFI_BATERRY_ID,
	WIFI_POWERON_TIME_ID,
	WIFI_CAPTURE_PHOTO_ID,
	WIFI_SYSTIME_ID,
	WIFI_SSID_ID,
	WIFI_PW_ID,
	WIFI_CH_ID,
	WIFI_EXE_MAIN_ID,
	WIFI_MAX_ID
};
static widgetCreateInfor wifiWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	createRect(WIFI_HIDE_RECT_ID,       Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	//createImageIcon(WIFI_MODE_ID,      Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(WIFI_REC_TIME_ID,  Rx(24), Ry(0), Rw(108), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(WIFI_SSID_ID,Rx(0), Ry(32), Rw(320), Rh(25),strToResId("SSID:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(WIFI_PW_ID,Rx(0), Ry(58), Rw(180), Rh(25),strToResId("WPA2:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(WIFI_CH_ID,Rx(0), Ry(84), Rw(320), Rh(25),strToResId("INFO:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	//createImageIcon(WIFI_MD_ID,        Rx(172), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	//createImageIcon(WIFI_MONITOR_ID,   Rx(192), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),

	//createImageIcon(WIFI_LOCK_ID,      Rx(236),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	//createImageIcon(WIFI_SD_ID,        Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(WIFI_MIC_ID,       Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	//createImageIcon(WIFI_BATERRY_ID,    Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(WIFI_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	//createImageIcon(WIFI_CAPTURE_PHOTO_ID,  Rx(147), Ry(26), Rw(20), Rh(25), R_ID_ICON_MTPHOTO,ALIGNMENT_CENTER),
	createStringIcon(WIFI_SYSTIME_ID,        Rx(5), Ry(215), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#if TP_FUCTION
	createImageIcon(WIFI_EXE_MAIN_ID,     Rx(270), Ry(200), Rw(48), Rh(48), R_ID_ICON_MENUBACK,ALIGNMENT_CENTER),
#endif
	widgetEnd(),
};
static void wifiResolutionShow(winHandle handle)
{
#if 0
	if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_HD)
		winSetResid(winItem(handle,WIFI_RESOLUTION_ID),strToResId("HD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_480FHD)
		winSetResid(winItem(handle,WIFI_RESOLUTION_ID),strToResId("480FHD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,WIFI_RESOLUTION_ID),strToResId("VGA"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_FHD)
		winSetResid(winItem(handle,WIFI_RESOLUTION_ID),strToResId("FHD"));
	else 
		winSetResid(winItem(handle,WIFI_RESOLUTION_ID),strToResId("???"));
#endif
}
static void wifiMDShow(winHandle handle)
{
#if 0
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,WIFI_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,WIFI_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,WIFI_MD_ID),false);
#endif
}
static void wifiMonitorShow(winHandle handle)
{
#if 0
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,WIFI_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,WIFI_MONITOR_ID),R_ID_ICON_MTPARKOFF);
#endif
}
static void wifiIrLedShow(winHandle handle)
{
#if 0
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,WIFI_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,WIFI_IRLED_ID),R_ID_ICON_MTIRON);
#endif
#endif
}
static void wifiLockShow(winHandle handle)
{
#if 0
	if(SysCtrl.g_lock)
	{
		winSetResid(winItem(handle,WIFI_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,WIFI_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,WIFI_LOCK_ID),false);
#endif
}
static void wifiSDShow(winHandle handle)
{
#if 0
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,WIFI_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,WIFI_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,WIFI_SD_ID),R_ID_ICON_MTSDCNORMAL);
#endif
}
static void wifiMicShow(winHandle handle)
{
#if 0
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,WIFI_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,WIFI_MIC_ID),R_ID_ICON_MTMICON);
#endif
}

static void wifiBaterryShow(winHandle handle)
{
#if 0
	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,WIFI_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,WIFI_BATERRY_ID),true);
#endif
}

static void wifiRemainTimeShow(winHandle handle,uint32 sec)
{

}

static void wifiRecTimeShow(winHandle handle,uint32 sec)
{
	static char recTimeStr[]="00:00";
	sec=sec%3600;
	recTimeStr[0] = sec/600+'0';  // m
	recTimeStr[1] = ((sec/60)%10)+'0';   // m
	recTimeStr[2] = ':';
	sec=sec%60;
	recTimeStr[3] = sec/10+'0';  // s
	recTimeStr[4] = sec%10+'0';   // s
	recTimeStr[5] = 0;
	winSetStrInfor(winItem(handle,WIFI_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_RED);
	winSetResid(winItem(handle,WIFI_REC_TIME_ID),strToResId(recTimeStr));
	winSetVisible(winItem(handle,WIFI_REC_TIME_ID),true);
}



