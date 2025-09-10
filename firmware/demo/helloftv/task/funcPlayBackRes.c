#include"uiWin.h"
#include "mediaFile/demuxer.h"

enum
{
	PLAYBACK_MODE_ID=0,
	//PLAYBACK_BK_ID,
	PLAYBACK_HIDE_RECT_ID,
	PLAYBACK_PLAY_TIME_ID,
	PLAYBACK_RESOLUTION_ID,
	//PLAYBACK_MD_ID,// motion detect
	//PLAYBACK_MONITOR_ID, // parking monitoring
	PLAYBACK_IRLED_ID,
	PLAYBACK_LOCK_ID,
	PLAYBACK_SD_ID,
	//PLAYBACK_MIC_ID,
	PLAYBACK_BATERRY_ID,
	PLAYBACK_POWERON_TIME_ID,
	PLAYBACK_FILE_NAME_ID,
	PLAYBACK_FILE_INDEX_ID,
	PLAYBACK_ERROR_ID,
	PLAYBACK_SPEED_ID,

	PLAYBACK_STA_ID,
   	PLAYBACK_EXE_PRE_ID,
	PLAYBACK_EXE_NEXT_ID,
	PLAYBACK_EXE_SET_ID,
	PLAYBACK_EXE_MAIN_ID,	
	PLAYBACK_MAX_ID
};
static widgetCreateInfor playbackWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PLAYBACK_BK_ID,              Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK),
	createRect(PLAYBACK_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TRANSFER),
	createImageIcon(PLAYBACK_MODE_ID,           Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTPLAY2,ALIGNMENT_LEFT),
	createStringIcon(PLAYBACK_PLAY_TIME_ID,      Rx(24), Ry(0), Rw(108), Rh(25),strToResId("00:00:00"),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createStringIcon(PLAYBACK_RESOLUTION_ID,Rx(132), Ry(0), Rw(48), Rh(25),strToResId("1M"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PLAYBACK_MD_ID,                Rx(172), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MONITOR_ID,     Rx(192), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_IRLED_ID,             Rx(214),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_LOCK_ID,              Rx(236),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),

	createImageIcon(PLAYBACK_SD_ID,                  Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(PLAYBACK_MIC_ID,                Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_BATERRY_ID,       Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(PLAYBACK_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(5), Ry(25), Rw(160), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(170), Ry(25), Rw(120), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_ERROR_ID,      Rx(110), Ry(100), Rw(80), Rh(40),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createStringIcon(PLAYBACK_SPEED_ID,      Rx(110), Ry(200), Rw(40), Rh(40),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_RED,0),

    createImageIcon(PLAYBACK_STA_ID,     Rx(140), Ry(100), Rw(40), Rh(40), R_ID_ICON_MTPLAY1,ALIGNMENT_CENTER),
#if TP_FUCTION
	createImageIcon(PLAYBACK_EXE_PRE_ID,    Rx(150), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTBACK1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_NEXT_ID,   Rx(190), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTFORWARD1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_SET_ID,    Rx(230), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTMENU1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_MAIN_ID,   Rx(270), Ry(200), Rw(48), Rh(48), R_ID_ICON_MENUBACK,ALIGNMENT_CENTER),
#endif
	widgetEnd(),
};

static void playbackBKShow(winHandle handle,u8 en)
{
	/*if(en)
		winSetVisible(winItem(handle,PLAYBACK_BK_ID),true);
	else
		winSetVisible(winItem(handle,PLAYBACK_BK_ID),false);*/
}
static void playbackPlayTimeShow(winHandle handle,INT32U playSeconde)
{
	static char TimeStr[9];
	//static INT32U LastSeconde=0;
	INT32U Minutes,secodes;
	secodes = playSeconde;
	Minutes=secodes/60;
	secodes=secodes%60;
	TimeStr[0]='0';
	TimeStr[1]='0';
	TimeStr[2]=':';
	TimeStr[3]=(Minutes/10)+'0';
	TimeStr[4]=(Minutes%10)+'0';
	TimeStr[5]=':';
	TimeStr[6]=(secodes/10)+'0';
	TimeStr[7]=(secodes%10)+'0';
	TimeStr[8]=0;
	winSetResid(winItem(handle,PLAYBACK_PLAY_TIME_ID),strToResId(TimeStr));
	winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),true);
}
static u32 playSpeedShowCnt;
static void playbackSpeedShow(winHandle handle,int speed)
{
	static char speedStr[9];
	playSpeedShowCnt=5;
	if(speed<0)
	{
		speedStr[0]='-';
		speed=0-speed;
	}
	else
	{
		speedStr[0]='+';
	}
	uintToString(&speedStr[1],8,speed);
	winSetResid(winItem(handle,PLAYBACK_SPEED_ID),strToResId(speedStr));
	winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),true);
}
static void playbackResolutionShow(winHandle handle)
{
	INT16U width=0,height=0;
	INT16U rWidth=0,rHeight=0;
	int type;
	getCurPlayFileRes(&width,&height);
	type = getCurPlayFileType();
	if(DEMUXER_AVI == type)
	{
		if(width==640)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
		else if(width==848)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("WVGA"));
		else if(height==720)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("HD"));
		else if(width==1440)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1080P"));
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("FHD"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
		winSetVisible(winItem(handle,PLAYBACK_RESOLUTION_ID),true);
	}
	else if((DEMUXER_JPG == type))
	{
		if(height<=480)
		{
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("0.3M"));
		}
		else if(height==576)
		{
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("0.5M"));
		}
		else if(height==720||height==960)
		{
			jpgGetLargeJpgSize(&rWidth, &rHeight);
			if (rHeight > 5000)
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("48M"));
			else if (rHeight > 3400)
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("20M"));
			else		
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1M"));
		}
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("2M"));
		else if(height==1440||height==1536)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("3M"));
		else if(height==1800||height==1944)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("5M"));
		else if(height==2160||height==2448)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("8M"));
		else if(height==2520||height==2736)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("10M"));
		else if(height==2880||height==3024)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("12M"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
		winSetVisible(winItem(handle,PLAYBACK_RESOLUTION_ID),true);
	}	
	else
		winSetVisible(winItem(handle,PLAYBACK_RESOLUTION_ID),false);
}
static void playbackMDShow(winHandle handle)
{

	/*if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PLAYBACK_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),false);*/
}
static void playbackMonitorShow(winHandle handle)
{

	/*if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKOFF);*/
}
static void playbackIrLedShow(winHandle handle)
{
	winSetVisible(winItem(handle,PLAYBACK_IRLED_ID),false);
}
static void playbackLockShow(winHandle handle)
{
	if(curFileIsLock())
	{
		winSetResid(winItem(handle,PLAYBACK_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),false);
}
static void playbackSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void playbackMicShow(winHandle handle)
{

	/*if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICON);*/
}

static void playbackBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}

static void playbackFileNameShow(winHandle handle)
{
	int idx;
	static char indexStr[10];
	static char fileNameStr[15];
	getCurFileName(fileNameStr,15);
	winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(fileNameStr));
}

//==sta: 0 backward 4X, 1: backward 2x, 2:normal , 3:forward 2X, 4: forward 4X ==
static void playbackStateShow(winHandle handle,s8 sta)
{


	/*if(0==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(1==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else */if(2==sta)
	{
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),false);
	}
	/*else if(3==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(4==sta)
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}*/
	else
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTPAUSE1);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}

}


