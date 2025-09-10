#include"uiWin.h"

enum
{
	PHOTO_MODE_ID=0,
	PHOTO_HIDE_RECT_ID,
	PHOTO_RESOLUTION_ID,
	PHOTO_IRLED_ID,
	PHOTO_SD_ID,
	PHOTO_MIC_ID,
	PHOTO_BATERRY_ID,
	PHOTO_POWERON_TIME_ID,
	PHOTO_CAPTURE_PHOTO_ID,
	PHOTO_SYSTIME_ID,

	PHOTO_EXE_MAIN_ID,
	PHOTO_WIFI_ID,
	PHOTO_LED_ID,
	PHOTO_ZOOM_ID,
	//PHOTO_EXE_ID,
	//PHOTO_EXE_PHOTO_CAP_ID,
	//PHOTO_EXE_LOCK_ID,
	//PHOTO_EXE_MIC_ID,

	PHOTO_MAX_ID
};
static widgetCreateInfor takePhotoWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PHOTO_HIDE_RECT_ID,       Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PHOTO_MODE_ID,      Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTPHOTO,ALIGNMENT_LEFT),
	createImageIcon(PHOTO_LED_ID,        Rx(0), Ry(35), Rw(35), Rh(35), R_ID_ICON_LEDOFF,ALIGNMENT_CENTER),

	createStringIcon(PHOTO_RESOLUTION_ID,Rx(132), Ry(0), Rw(40), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PHOTO_IRLED_ID,     Rx(214),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_WIFI_ID,        Rx(200), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTWIFIDISCON,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_ZOOM_ID,     Rx(300), Ry(190), Rw(30), Rh(25),strToResId("1X"),ALIGNMENT_LEFT, R_COLOR_YELLOW,0),

	createImageIcon(PHOTO_SD_ID,        Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_BATERRY_ID,    Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(PHOTO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createImageIcon(PHOTO_CAPTURE_PHOTO_ID,  Rx((320-24)/2), Ry((240-24)/2), Rw(24), Rh(24), R_ID_ICON_MENULOOPRECORD,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_SYSTIME_ID,        Rx(5), Ry(215), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),

	//createImageIcon(PHOTO_EXE_PHOTO_CAP_ID,     Rx(215), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTRECORD1,ALIGNMENT_CENTER),
#if TP_FUCTION
	createImageIcon(PHOTO_EXE_MAIN_ID,     Rx(270), Ry(200), Rw(48), Rh(48), R_ID_ICON_MENUBACK,ALIGNMENT_CENTER),
#endif
	widgetEnd(),
};
  

static void photoLedShow(winHandle handle,bool state)
{
	if(state)
		winSetResid(winItem(handle,PHOTO_LED_ID),R_ID_ICON_LEDON);
	else
		winSetResid(winItem(handle,PHOTO_LED_ID),R_ID_ICON_LEDOFF);
	return;
}

static void videoTakePhotoSuccessShow(winHandle handle, INT8U state)
{
	if(state)
	{
		winSetVisible(winItem(handle,PHOTO_CAPTURE_PHOTO_ID),true);	
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_CAPTURE_PHOTO_ID),false);
	}

}

static void photoResolutionShow(winHandle handle)
{

	if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_1M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("1M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_2M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("2M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_3M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("3M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_5M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("5M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_8M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("8M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_10M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("10M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_12M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("12M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_20M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("20M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_48M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("48M"));	
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("0.3M"));
	else 
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("???"));
}


static void photoIrLedShow(winHandle handle)
{

#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIRON);
#endif
}
static void photoSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}

static void photoBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
}

#if SMALL_PANEL_SUPPORT==0
static void photoPoweOnTimeShow(winHandle handle,uint32 sec)
{
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,PHOTO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
}
#endif

static void photoWifiShow(winHandle handle, INT8U show, int force)
{
	static int lastStat = -1;
	int stat;
	if (show)
	{
		stat = get_wifi_statue(NULL, NULL);
		if ((stat != lastStat) || (force != 0))
		{
			lastStat = stat;
			if(stat)
				winSetResid(winItem(handle,PHOTO_WIFI_ID),R_ID_ICON_MTWIFICON);
			else
				winSetResid(winItem(handle,PHOTO_WIFI_ID),R_ID_ICON_MTWIFIDISCON);
		}
		winSetVisible(winItem(handle,PHOTO_WIFI_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_WIFI_ID),false);
	}
}
  

static void photoZoomShow(winHandle handle, u8 item)
{
    switch(item)
    {
        case 0:
            winSetResid(winItem(handle,PHOTO_ZOOM_ID),strToResId("1X"));
            break;
        case 1:
            winSetResid(winItem(handle,PHOTO_ZOOM_ID),strToResId("2X"));
            break;
        case 2:
            winSetResid(winItem(handle,PHOTO_ZOOM_ID),strToResId("3X"));
            break;
        case 3:
            winSetResid(winItem(handle,PHOTO_ZOOM_ID),strToResId("4X"));
            break;
        default:
            winSetResid(winItem(handle,PHOTO_ZOOM_ID),strToResId("1X"));
            break;
    }
}
