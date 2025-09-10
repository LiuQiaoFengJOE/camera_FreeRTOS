#include "app.h"
#include "funcPlayBackRes.c"
#include "fatfs/fileOps.h"
#include "touchPanel.h"

extern int playbackIsRecording(void);
extern menu MENU(movie);

static int playbackKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int type;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		type = getCurPlayFileType();
		#if SUPPORT_AUD_ENC
		if(DEMUXER_AVI == type || DEMUXER_WAV == type)
		#else
		if(DEMUXER_AVI == type)
		#endif
		{
			if(getPlayStat()==PLAY_BACK_STOP)
			{
				while(dacIsPlaying())
				{
					osal_task_sleep(5);
				}
				#if SUPPORT_AUD_ENC
				if(DEMUXER_WAV == type)
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_START,0);
				else
				#endif
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_START,0);
				
				playbackStateShow(handle,2);
			}
			else if(getPlayStat()==PLAY_BACK_PLAYING)
			{
				#if SUPPORT_AUD_ENC
				if(DEMUXER_WAV == type)
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_PAUSE,0);
				else
				#endif
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PAUSE,0);
				playbackStateShow(handle,8);
			}
			else if(getPlayStat()==PLAY_BACK_PAUSE)
			{
				while(dacIsPlaying())
				{
					osal_task_sleep(5);
				}
				#if SUPPORT_AUD_ENC
				if(DEMUXER_WAV == type)
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_RESUME,0);
				else
				#endif
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_RESUME,0);
				playbackStateShow(handle,2);
			}
		}
	}
	return 0;
}
static int playbackKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()==PLAY_BACK_PLAYING)
		{
		//	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_FASTBACKWARD,0);
		//	playbackSpeedShow(handle,getCurPlaySpeed());
		}
		else
		{
			type = getCurPlayFileType();
			#if SUPPORT_AUD_ENC
			if(DEMUXER_WAV == type)
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_PRE,0);
			else
			#endif
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);
			winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
			type = getCurPlayFileType();
			if(DEMUXER_JPG==type)
			{
				playbackStateShow(handle,2);
			}
			else
			{
				if(getPlayStat()==PLAY_BACK_PAUSE)
				{
					playbackStateShow(handle,8);
				}
			}
		}
		
	}
	return 0;
}

static int playbackKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()==PLAY_BACK_PLAYING)
		{
		//	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_FASTFORWARD,0);
		//	playbackSpeedShow(handle,getCurPlaySpeed());
		}
		else
		{
			type = getCurPlayFileType();
			#if SUPPORT_AUD_ENC
			if(DEMUXER_WAV == type)
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_NEXT,0);
			else
			#endif
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_NEXT,0);
			winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
			type = getCurPlayFileType();
			if(DEMUXER_JPG==type)
			{
				playbackStateShow(handle,2);
			}
			else
			{
				if(getPlayStat()==PLAY_BACK_PAUSE)
				{
					playbackStateShow(handle,8);
				}
			}
		}
	}
	return 0;
}
extern menu MENU(playBack);
static int playbackKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 i,j,lastTime,alignsize,size;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()!=PLAY_BACK_PLAYING)
			uiOpenWindow(&menuItemWindow,1,&MENU(playBack));
	}
	return 0;
}
static int playbackKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()==PLAY_BACK_PLAYING)
			return 0; 
		functionStart(FUNCTION_MAINMENU3,0);
#if 1	
		//printf("===fileName:%s, type:%d\n", FIND_CUR_FILE, getCurPlayFileType());
		//getCurPlayFileType
		//functionChange();
#else		
		if(getPlayStat()!=PLAY_BACK_PLAYING)
		{
			#if SUPPORT_AUD_ENC
			type = getCurPlayFileType();
			if(DEMUXER_AVI == type||DEMUXER_JPG == type)
			{
				changeDir(WAV_FILE);
				fileInforInit(WAV_FILE);
				winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
				if(curFloderHasFile())
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_PRE,0);
				else
					uiOpenWindow(&menuItemWindow,1,&MENU(movie));
			}
			else if(DEMUXER_WAV == type)
			#endif
				uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
#endif		
	}
	return 0;
}
static int playbackKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()==PLAY_BACK_PLAYING)
			return 0; 
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}
static int playbackSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	playbackSDShow(handle);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}

static int playbackSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	playbackBaterryShow(handle);
	return 0;
}

static int playbackSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int playBackTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/
	
	if(parameNum!=3)
	{
		printf("playBackTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==PLAYBACK_EXE_MAIN_ID)
		{
		if(getPlayStat()!=PLAY_BACK_PLAYING)
		{
			#if SUPPORT_AUD_ENC
			int type;
			type = getCurPlayFileType();
			if(DEMUXER_AVI == type||DEMUXER_JPG == type)
			{
				changeDir(WAV_FILE);
				fileInforInit(WAV_FILE);
				winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
				if(curFloderHasFile())
					sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_AUD_PRE,0);
				else
					uiOpenWindow(&menuItemWindow,1,&MENU(movie));
			}
			else if(DEMUXER_WAV == type)
			#endif
				functionStart(FUNCTION_MAINMENU3,0);
		}
		}
		else if(parame[0]==PLAYBACK_EXE_PRE_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_UP, KEY_PRESSED);
		}
		else if(parame[0]==PLAYBACK_EXE_NEXT_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_DOWN, KEY_PRESSED);
		}
		else if(parame[0]==PLAYBACK_EXE_SET_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_MENU, KEY_PRESSED);
		}
        
		else if(parame[0]==PLAYBACK_STA_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, KEY_PRESSED);
		}
	}
	return 0;
}
static int playBackSlidRelease(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=1)
	{
		printf("playBackSlidRelease, parame num error %d\n",parameNum);
		return 0;
	}
	if(parame[0]==SLID_DIRECT_UP)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_DOWN, KEY_PRESSED);
	else if(parame[0]==SLID_DIRECT_DOWN)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_UP, KEY_PRESSED);
	else if(parame[0]==SLID_DIRECT_RIGHT)
	{
		//deamon_keysound_play();
		//functionStart(FUNCTION_VIDEO_RECORD,0);
	}		
	return 0;
}
static int playbackOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	resInfor infor;
	printf("playback Open Win!!!\n");
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_LEFT;
	infor.fontColor=R_COLOR_WHITE;
	infor.rimColor=R_COLOR_BLACK;
	winSetUnselectInfor(winItem(handle,PLAYBACK_FILE_NAME_ID),&infor);
	winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
	resInforInit(&infor);
	playbackResolutionShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackLockShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);
	playbackFileNameShow(handle);
	playbackStateShow(handle,2);
	playbackBKShow(handle, 0);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}
static int playbackCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback Close Win!!!\n");
	return 0;
}
static int playbackWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback WinChild Close!!!\n");
	winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
	playbackResolutionShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackLockShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);
	playbackFileNameShow(handle);
	playbackStateShow(handle,2);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}
static int playbackWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback WinChild Open!!!\n");
	return 0;
}

static int playbackStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	type = getCurPlayFileType();
	#if SUPPORT_AUD_ENC
	if(DEMUXER_AVI == type || DEMUXER_WAV == type)
	#else
	if(DEMUXER_AVI == type)
	#endif
	{		
		playbackPlayTimeShow(handle,curPlayFileTotalTime()/1000);
		playbackLockShow(handle);
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
	}
	playbackFileNameShow(handle);
	playbackResolutionShow(handle);
	return 0;
}
static int playbackPause(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playbackResume(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playbackStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	int type;
	type = getCurPlayFileType();
	#if SUPPORT_AUD_ENC
	if(DEMUXER_AVI == type || DEMUXER_WAV == type)
	#else
	if(DEMUXER_AVI == type)
	#endif
	{
		playbackPlayTimeShow(handle,curPlayFileTotalTime()/1000);
	}
	return 0;
}
static int playbackErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playbackSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(playSpeedShowCnt&&getCurPlaySpeed()==1)
	{
		playSpeedShowCnt--;
		if(playSpeedShowCnt==0)
			winSetVisible(winItem(handle,PLAYBACK_SPEED_ID),false);
	}

	if(DEMUXER_AVI== getCurPlayFileType())
	{
		if(getPlayStat()==PLAY_BACK_PAUSE)
		{
			playbackStateShow(handle,8);
		}
	}
	return 0;
}
static int playbackSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{

}
static int playbackSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	playbackPlayTimeShow(handle, parame[0]);
	//printf("record time:%d\n",parame[0]);
}


msgDealInfor playbackMsgDeal[]=
{
	{SYS_OPEN_WINDOW,playbackOpenWin},
	{SYS_CLOSE_WINDOW,playbackCloseWin},
	{SYS_CHILE_COLSE,playbackWinChildClose},
	{SYS_CHILE_OPEN,playbackWinChildOpen},
	{SYS_TOUCH_WINDOW,playBackTouchWin},
	{SYS_SLID_RELEASE,playBackSlidRelease},
	{KEY_EVENT_OK,playbackKeyMsgOk},
	{KEY_EVENT_UP,playbackKeyMsgUp},
	{KEY_EVENT_DOWN,playbackKeyMsgDown},
	{KEY_EVENT_SETTING,playbackKeyMsgMenu},
	{KEY_EVENT_MODE,playbackKeyMsgMode},
	{KEY_EVENT_POWER,playbackKeyMsgPower},
	{SYS_EVENT_SDC,playbackSysMsgSD},
	{SYS_EVENT_USB,playbackSysMsgUSB},
	{SYS_EVENT_BAT,playbackSysMsgBattery},
	{PLAY_EVENT_PLAYSTART,playbackStarted},
	{PLAY_EVENT_PLAYPAUSE,playbackPause},
	{PLAY_EVENT_PLAYRESUME,playbackResume},
	{PLAY_EVENT_PLAYSTOP,playbackStopped},
	//{SYS_EVENT_500MS,playbackSysMsg500MS},
	{SYS_TIME_UPDATE,playbackSysMsgUpdateTime},
	{SYS_EVENT_1S,playbackSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(playbackWindow,playbackMsgDeal,playbackWin)


