#include "app.h"
#include"fatfs/fileManage.h"
#include"mediaFile/picIdentify.h"
#include "funcAudioRecordRes.c"
#include "mediaFile/demuxer.h"

#if SUPPORT_AUD_REC

extern int videoIsRecording(void);
static int audioKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoIsRecording())
		{
			sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_STOP,0);
		}
		else
		{
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_START,0);
		}
	}
	return 0;
}

static int audioKeyMsgLongOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_START,0);
	}
	else if (keyState==KEY_RELEASE)
	{
		sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_STOP,0);
	}

	return 0;
}
static int audioKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		SysCtrl.ailangid = SysCtrl.ailangid==PICID_LANGID_SCH?PICID_LANGID_ENG:PICID_LANGID_SCH;
		printf("==@==ailangid:%d\n", SysCtrl.ailangid);
		picIdtfySetLangid(SysCtrl.ailangid);		
	}
	return 0;
}
static int audioKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//printf("key down\n");
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
static int audioKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 i,j,lastTime,alignsize,size;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		/*if(videoIsRecording())
			sendMsg(OS_TASK_VID_REC, TASK_MSG_VID_LOCK,1);
		else
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));*/
	}
	return 0;
}
static int audioKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionChange();
	}
	return 0;
}

static int audioKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		{
			//ax32xx_csiLCDDmaEnable(1);
			//lcdPreviewStart();
			//return 0;
		}
		
		if(videoIsRecording())
		{
			sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_AUD_REC_STOP,0);
		}	
		
		if((configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)&& (SysCtrl.photoMode!=PHOTO_NORMAL))
		{
			picAiUnInit();
		}	
		
		functionStart(FUNCTION_TAKE_PHOTO/*FUNCTION_MAINMENU*/,0);

	}
	return 0;
}
static int audioSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioSDShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,2);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,2);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,2);
	return 0;
}

static int audioSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioBaterryShow(handle);
	return 0;
}

static int audioSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioBaterryShow(handle);
	return 0;
}
static int audioSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(!videoIsRecording())
	{
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
	}
	return 0;
}

static int audioSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int audioSysMsgLivePreview(winHandle handle)
{
	ax32xx_csiLCDDmaEnable(0);
	lcdPreviewStop();

	char *name;
	fileInforInit(AVI_FILE);
	changeDir(AVI_FILE);
	name=FIND_PRE_VID;
	LivePreviewShow(name);
}

static int audioOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audio Open Win!!!\n");
	audioSysMsgLivePreview(handle);
	audioLockShow(handle);
	audioSDShow(handle);
	audioBaterryShow(handle);
	audioRemainTimeShow(handle);
	winSetResid(winItem(handle,AUDIO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int audioCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audio Close Win!!!\n");
	/*if((configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)&& (SysCtrl.photoMode!=PHOTO_NORMAL))
	{
		printf("===@@@@@@@@==wifi stop\n");
		wifi_stop();
	}*/	
	return 0;
}
static int audioWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audio WinChild Close!!!\n");
	audioLockShow(handle);
	audioSDShow(handle);
	audioBaterryShow(handle);
	return 0;
}
static int audioWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audio WinChild Open!!!\n");
	return 0;
}

static int audioRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioLockShow(handle);
	audioRecTimeShow(handle, 0);
	return 0;
}
static int audioRecordRestart(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioLockShow(handle);
	return 0;
}
static int audioRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioLockShow(handle);
	audioRemainTimeShow(handle);
	printf("====audioRecordStopped\n");
	if (SysCtrl.photoMode == PHOTO_AI_CHAT)
	{
		int rr;
		char *name;
		PicAiRet_T *aiContent= NULL;
		
		fileInforInit(WAV_FILE);
		changeDir(WAV_FILE);
		name=FIND_PRE_WAV;		
		printf("===aud idtfy, name:%s\n", name);
		rr = picIdtfyUploadAud(name);
		aiContent = picIdtfyContentGet();
#if 0
		if (aiContent)
		{//test code
			int i;
			printf("==#=audio idtfy, textNum:%d, urlNum:%d, recode:%d\n", aiContent->textNum, aiContent->urlNum,aiContent->reCode);
			for(i=0; i<aiContent->textNum; i++)
			{
				printf("===aiRet->text[%d]:%s\n", i, aiContent->text[i]);
			}
			for(i=0; i<aiContent->textNum; i++)
			{
				printf("aiRet->url[%d]:%s\n", i, aiContent->url[i]);
			}
		}
		//return 0;
#endif
		requestAudioDownload(aiContent);
		if (SysCtrl.ailangid == PICID_LANGID_SCH)
		{
			printf("==%s,%d, aiRet:%p\n", __FUNCTION__, __LINE__, aiContent);
			uiOpenWindow(&idContentUniWindow,1, (uint32)aiContent);
		}
		else
		{
			printf("==%s,%d, aiRet:%p\n", __FUNCTION__, __LINE__, aiContent);
			uiOpenWindow(&idContentWindow,1, (uint32)aiContent);
		}		
	}
	return 0;
}
static int audioRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioLockShow(handle);
	audioRemainTimeShow(handle);
	return 0;
}
static int audioRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioLockShow(handle);
	return 0;
}
static int audioSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	winSetResid(winItem(handle,AUDIO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int audioSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	static uint32 lastTime = 0;
	if(lastTime==0)
	{
		lastTime=XOSTimeGet();
		printf("start time:%d    !!!!!!!!!!!!!!!!!!!!!!\n\n\n",lastTime);
		return 0;
	}
	if(XOSTimeGet()-lastTime>60*3*1000)
	{
		lastTime=XOSTimeGet();
		printf("end time:%d    !!!!!!!!!!!!!!!!!!!!!!\n\n\n",lastTime);
	}
#endif
}
static int audioSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioRecTimeShow(handle, parame[0]);
	//printf("record time:%d\n",parame[0]);
}


msgDealInfor audioRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioOpenWin},
	{SYS_CLOSE_WINDOW,audioCloseWin},
	{SYS_CHILE_COLSE,audioWinChildClose},
	{SYS_CHILE_OPEN,audioWinChildOpen},
	{KEY_EVENT_OK,audioKeyMsgOk},
	{KEY_EVENT_LONG_OK,audioKeyMsgLongOk},
	{KEY_EVENT_UP,audioKeyMsgUp},
	{KEY_EVENT_DOWN,audioKeyMsgDown},
	{KEY_EVENT_MENU,audioKeyMsgMenu},
	{KEY_EVENT_MODE,audioKeyMsgMode},
	{KEY_EVENT_POWER,audioKeyMsgPower},
	{SYS_EVENT_SDC,audioSysMsgSD},
	{SYS_EVENT_USB,audioSysMsgUSB},
	{SYS_EVENT_BAT,audioSysMsgBattery},
	{VID_EVENT_REC_STOP,audioRecordStopped},
	{VID_EVENT_REC_START,audioRecordStarted},
	{VID_EVENT_REC_RESTART,audioRecordRestart},
	{VID_EVENT_REC_ERROR,audioRecordErr},
	{VID_EVENT_LOCK_FILE,audioRecordLockFile},
	//{SYS_EVENT_500MS,audioSysMsg500MS},
	{SYS_TIME_UPDATE,audioSysMsgUpdateTime},
	{SYS_EVENT_1S,audioSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(audioRecordWindow,audioRecordeMsgDeal,audioRecordeWin)

#endif

