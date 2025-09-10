#include"uiWin.h"

#if SUPPORT_AUD_REC

enum
{
	AUDIO_MODE_ID=0,
	AUDIO_HIDE_RECT_ID,
	AUDIO_REC_TIME_ID,
	AUDIO_LOCK_ID,
	AUDIO_SD_ID,
	AUDIO_BATERRY_ID,
	AUDIO_POWERON_TIME_ID,
	AUDIO_SYSTIME_ID,
	AUDIO_MAX_ID
};
static widgetCreateInfor audioRecordeWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
//	createRect(AUDIO_HIDE_RECT_ID,       Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TRANSFER),
	createImageIcon(AUDIO_MODE_ID,      Rx(140), Ry(80), Rw(40), Rh(40), R_ID_ICON_MENUAUDIO,ALIGNMENT_CENTER),
	createStringIcon(AUDIO_REC_TIME_ID,  Rx(150), Ry(110), Rw(108), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createImageIcon(AUDIO_LOCK_ID,      Rx(236),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_SD_ID,        Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(AUDIO_BATERRY_ID,    Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(AUDIO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(AUDIO_SYSTIME_ID,        Rx(5), Ry(215), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	widgetEnd(),
};
static void audioLockShow(winHandle handle)
{
	if(curFileIsLock()&&videoIsRecording())
	{
		winSetResid(winItem(handle,AUDIO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,AUDIO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,AUDIO_LOCK_ID),false);
}
static void audioSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,AUDIO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,AUDIO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,AUDIO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}

static void audioBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUDIO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,AUDIO_BATERRY_ID),true);
}

#if SMALL_PANEL_SUPPORT==0
static void audioPoweOnTimeShow(winHandle handle,uint32 sec)
{
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,AUDIO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
}
#endif

static void audioRemainTimeShow(winHandle handle)
{
	winSetVisible(winItem(handle,AUDIO_REC_TIME_ID),false);
	return;
	static char recTimeStr[]="00:00:00";
	uint32 sec=vidRecordRemainTime();
	recTimeStr[0] = sec/36000+'0';  // h
	recTimeStr[1] = ((sec/3600)%10)+'0';   // h
	recTimeStr[2] = ':';
	sec=sec%3600;
	recTimeStr[3] = sec/600+'0';  // m
	recTimeStr[4] = ((sec/60)%10)+'0';   // m
	recTimeStr[5] = ':';
	sec=sec%60;
	recTimeStr[6] = sec/10+'0';  // s
	recTimeStr[7] = sec%10+'0';   // s
	recTimeStr[8] = 0;
	winSetStrInfor(winItem(handle,AUDIO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	winSetResid(winItem(handle,AUDIO_REC_TIME_ID),strToResId(recTimeStr));
}

static void audioRecTimeShow(winHandle handle,uint32 sec)
{
	
	winSetVisible(winItem(handle,AUDIO_REC_TIME_ID),true);
	static char recTimeStr[]="00:00";
	sec=sec%3600;
	recTimeStr[0] = sec/600+'0';  // m
	recTimeStr[1] = ((sec/60)%10)+'0';   // m
	recTimeStr[2] = ':';
	sec=sec%60;
	recTimeStr[3] = sec/10+'0';  // s
	recTimeStr[4] = sec%10+'0';   // s
	recTimeStr[5] = 0;
	winSetStrInfor(winItem(handle,AUDIO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_RED);
	winSetResid(winItem(handle,AUDIO_REC_TIME_ID),strToResId(recTimeStr));
	if (SysCtrl.photoMode == PHOTO_AI_CHAT)
	{
		if (sec>=10)
		{
			sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_STOP,0);
		}
	}
}

#endif


