#include "app.h"
#include "funcVideoRecordRes.c"
#include "mediaFile/demuxer.h"
#include "mediaFile/userInterface.h"

extern int videoIsRecording(void);
extern void sensor_change(void);

u32 mdRecordCnt=0;
void mdRecordCntReset(void)
{
	mdRecordCnt=30;
}
void mdRecordStopDeal(void)
{
	if(videoIsRecording())
	{
		if(mdRecordCnt)
		{
			mdRecordCnt--;
			if(mdRecordCnt==0)
				sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
		}
	}
}

static int videoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//sendMsgFrontAndWait(OS_TASK_PRINTER, TASK_MSG_PRINTER_JPG,0);
		//return;
		if(videoIsRecording())
		{
			sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
		}
		else
		{
			//extern int8 videoRecordSetRedLevel(int8 value);
			//videoRecordSetRedLevel(-4/*-2*//*0*//*2*/);
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
		}
	}
	return 0;
}

static int videoKeyMsgLongOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(!videoIsRecording())
		{
			freePicStickerA();
		//	animationInit(LEFTBOTTOM2RIGHTUPPER);
		//	osal_task_sleep(50);
			sensor_change();
		}

	}
	return 0;
}
static int videoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//printf("lcd kick\n");
		//ax32xx_lcdKick();
		//return 0;
	#if WIFI_EN
		functionStart(FUNCTION_WIFI,0);
	#endif
		//textCrcCheck();
	}
	return 0;
}
static int videoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		lcdShowModeChange();
		//picStickerLoop();
		return 0;
		localTaketPhotoStart();
		while(dacIsPlaying())
		{
			osal_task_sleep(5);
		}
		dacPlaySound(RES_TAKE_PHOTO_MUSIC,getSoundVol());
		//lcdShowModeChange();
#if 0
		if(SysCtrl.crop_level>0)
			SysCtrl.crop_level--;
		lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
#else
		//aviThumbnailInfor("1:/DCIM/AVIPRE.AVI");
		//showTaskFreeStak();
#endif
	}
	return 0;
}
extern menu MENU(movie);
static int videoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 i,j,lastTime,alignsize,size;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//showTaskFreeStak();
		//return 0;
		if(videoIsRecording())
			sendMsg(OS_TASK_VID_REC, TASK_MSG_VID_LOCK,1);
		else
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
	}
	return 0;
}
static int videoKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionChange();
		functionStart(FUNCTION_MAINMENU2,0);

	}
	return 0;
}

static u8 lastlevel=0;
static int videoKeyMsgZoomIn(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        SysCtrl.crop_level++;
        if(SysCtrl.crop_level>CROPLEVEMAX-1)
            SysCtrl.crop_level=0;        
        if (lastlevel!= SysCtrl.crop_level)
        {
            lastlevel = SysCtrl.crop_level;
            lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
        }
        videoZoomShow(handle, SysCtrl.crop_level);
	}
	return 0;
}
static int videoKeyMsgZoomOut(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
        SysCtrl.crop_level--;
        if(SysCtrl.crop_level>CROPLEVEMAX-1)
            SysCtrl.crop_level=CROPLEVEMAX-1;              
        if (lastlevel!= SysCtrl.crop_level)
        {
            lastlevel = SysCtrl.crop_level;
            lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
        }
        videoZoomShow(handle, SysCtrl.crop_level);
	}
	return 0;
}

static int videoKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	u8 buffer;
	uint32 keyState=KEY_STATE_INVALID;  
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(ax32xx_VDDGSENRead()){
			ax32xx_VDDGSENEnable(0);
			videoLedShow(handle,0);
		}else{
			ax32xx_VDDGSENEnable(1);
			videoLedShow(handle,1);
		}
	}
	return 0;
}


static int videoKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoIsRecording())
		{
			printf("%s...animationRunEndFlagGet()=1\n", __func__);
			return 0;
		}	
		//printf("===sticker loop\n");
		//picStickerLoop();
		functionStart(FUNCTION_MAINMENU2,0);
	}
	return 0;
}
static int videoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoSDShow(handle);
	videoRemainTimeShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,2);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,2);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,2);
	return 0;
}

static int videoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoBaterryShow(handle);
	return 0;
}

static int videoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoBaterryShow(handle);
	return 0;
}
static int videoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	mdRecordCntReset();
	if(!videoIsRecording())
	{
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
	}
	return 0;
}

static int videoSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int videoRecordTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/

	if(parameNum!=3)
	{
		printf("videoRecordTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==VIDEO_EXE_MAIN_ID)
		{
			printf("==back to mainmenu\n");
			if(videoIsRecording())
			{
				return 0;
			}
			functionStart(FUNCTION_MAINMENU,0);
		}
		else if(parame[0]==VIDEO_EXE_RECORD_ID)
		{		
			printf("the event record press\n");
			
			if(videoIsRecording())
			{
				sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
			}
			else
			{
				sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
			}
		}
		else if (parame[0] == VIDEO_BATERRY_ID)
		{
			if(SysCtrl.crop_level>0)
				SysCtrl.crop_level--;
			else
				SysCtrl.crop_level=6;
			
			lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
		}
		/*else if(parame[0]==VIDEO_CAPTURE_PHOTO_ID)
		{
			if(videoRecordGetStatus() == MEDIA_STAT_START)
			{
				winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),true);
				videoCapturePhoto();
			}
		}
		else if(parame[0]==VIDEO_EXE_LOCK_ID)
		{
			XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(KEY_EVENT_LOCK,KEY_PRESSED));
		}
		else if(parame[0]==VIDEO_EXE_MIC_ID)
		{
			XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(KEY_EVENT_MIC,KEY_PRESSED));
		}
		else if(parame[0]==VIDEO_EXE_LIG_ID)
		{
		}*/
	}
	return 0;
}

static int videoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("video Open Win!!!\n");
	mdSetEn(getMdEn());
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
	videoRemainTimeShow(handle);
	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	if(SysCtrl.timeStamp==1)
		winSetResid(winItem(handle,VIDEO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	if(SysCtrl.astern == ASTERN_ON)
	{
		uiOpenWindow(&asternWindow,0);
	}
	return 0;
}
static int videoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("video Close Win!!!\n");
	return 0;
}
static int videoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("video WinChild Close!!!\n");
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	return 0;
}
static int videoWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("video WinChild Open!!!\n");
	return 0;
}

static int videoRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	videoRecTimeShow(handle, 0);
	return 0;
}
static int videoRecordRestart(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	return 0;
}
static int videoRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	videoRemainTimeShow(handle);
	return 0;
}
static int videoRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	videoRemainTimeShow(handle);
	return 0;
}
static int videoRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoLockShow(handle);
	return 0;
}
extern u32 usbJpgInterval;
extern u32 usb2JpgInterval;
extern u32 csiJpgInterval;
u32 getDebug1SData(u32 index,u32 *sum)
{
	u32 temp;
	*sum=3;
	if(index==0)
	{
		return usbJpgInterval;
	}
	if(index==1)
	{
		return usb2JpgInterval;
	}
	if(index==2)
	{
		return csiJpgInterval;
	}
	return 0;
}

static int videoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	mdRecordStopDeal();
	if(SysCtrl.timeStamp==1)
		winSetResid(winItem(handle,VIDEO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int videoSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	//static char debugStr1[100];
	//winSetResid(winItem(handle,VIDEO_SYSTIME_ID),debugDataString(debugStr1,getDebug1SData));
}
static int videoSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoRecTimeShow(handle, parame[0]);
	//printf("record time:%d\n",parame[0]);
}


msgDealInfor videoRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,videoOpenWin},
	{SYS_CLOSE_WINDOW,videoCloseWin},
	{SYS_CHILE_COLSE,videoWinChildClose},
	{SYS_CHILE_OPEN,videoWinChildOpen},
	{SYS_TOUCH_WINDOW,videoRecordTouchWin},

	{KEY_EVENT_PHOTO,videoKeyMsgOk},
	//{KEY_EVENT_LONG_OK,videoKeyMsgLongOk},
	{KEY_EVENT_LONG_OK,videoKeyMsgUp},
	// {KEY_EVENT_DOWN,videoKeyMsgDown},
	{KEY_EVENT_SETTING,videoKeyMsgMenu},
	{KEY_EVENT_MODE,videoKeyMsgMode},
	{KEY_EVENT_RIGHT,videoKeyMsgRight},
	{KEY_EVENT_ZOOMIN,videoKeyMsgZoomIn},
	{KEY_EVENT_ZOOMOUT,videoKeyMsgZoomOut},
	{SYS_EVENT_SDC,videoSysMsgSD},
	{SYS_EVENT_USB,videoSysMsgUSB},
	{SYS_EVENT_BAT,videoSysMsgBattery},
	{SYS_EVENT_MD,videoSysMsgMD},
	{VID_EVENT_REC_STOP,videoRecordStopped},
	{VID_EVENT_REC_START,videoRecordStarted},
	{VID_EVENT_REC_RESTART,videoRecordRestart},
	{VID_EVENT_REC_ERROR,videoRecordErr},
	{VID_EVENT_LOCK_FILE,videoRecordLockFile},
	{SYS_EVENT_500MS,videoSysMsg500MS},
	{SYS_TIME_UPDATE,videoSysMsgUpdateTime},
	{SYS_EVENT_1S,videoSysMsg1S},
	{KEY_EVENT_POWER,videoKeyMsgPower},
	//{KEY_EVENT_POWER,videoKeyMsgUp},
	{EVENT_MAX,NULL},
};

WINDOW(videoRecordWindow,videoRecordeMsgDeal,videoRecordeWin)


