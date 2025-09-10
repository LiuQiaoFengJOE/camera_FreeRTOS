#include "app.h"
#include "funcWifiRes.c"
#include "mediaFile/demuxer.h"
#include "mediaFile/userInterface.h"
#include "wifiProc.h"


extern int wifiIsRecording(void);
extern int g_ap_sta_rc_retry_set;

static int wifiKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//packerInputChange();
		//showTaskFreeStak();
	}
	return 0;
}
static int wifiKeyMsgLongOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if ((takePictrueIsWorking()==0) && (!videoIsRecording()))
		{
			freePicStickerA();
			animationInit(LEFTBOTTOM2RIGHTUPPER);
			osal_task_sleep(40);
			sensor_change();
		}

	}
	return 0;
}
static int wifiKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		packerInputChange();
		//taskQueueEnable(0);
		//textCrcCheck();
		//ssv6xxx_set_tx_retry_cnt(0);
		//dropConnAndWaite();
	}
	return 0;
}
extern int usb2Debug;
static int wifiKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//usb2Debug=1;
	}
	return 0;
}
//extern MENU(movie);

static int wifiKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	u32 i,j,lastTime,alignsize,size;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int wifiKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionChange();
		functionStart(functionPre(),0);
	}
	return 0;
}

static int wifiKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU,0);
	}
	return 0;
}
static int wifiSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiSDShow(handle);
	return 0;
}

static int wifiSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiBaterryShow(handle);
	return 0;
}

static int wifiSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int wifiSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	mdRecordCntReset();
	if(!videoIsRecording())
	{
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
	}
	return 0;
}

static int wifiSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int wifiTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/

	if(parameNum!=3)
	{
		printf("wifiTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==WIFI_EXE_MAIN_ID)
		{
			printf("==back to mainmenu\n");
			functionStart(FUNCTION_MAINMENU,0);
		}
	}
	return 0;
}


static int wifiOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("wifi Open Win!!!\n");
	wifiResolutionShow(handle);
	wifiMDShow(handle);
	wifiMonitorShow(handle);
	wifiIrLedShow(handle);
	wifiLockShow(handle);
	wifiSDShow(handle);
	wifiMicShow(handle);
	wifiBaterryShow(handle);
	winSetVisible(winItem(handle,WIFI_CAPTURE_PHOTO_ID),false);

	if(0 == start_wifi(0)){
		winSetResid(winItem(handle,WIFI_SSID_ID),strToResId(getWifiSSIDInfoString()));
		winSetResid(winItem(handle,WIFI_PW_ID),strToResId(getWifiPwString()));
		winSetResid(winItem(handle,WIFI_CH_ID),strToResId(getWifiConnectInfoString()));
	}

	
	//winSetResid(winItem(handle,WIFI_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int wifiCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("wifi Close Win!!!\n");
	return 0;
}
static int wifiWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("wifi WinChild Close!!!\n");
	wifiResolutionShow(handle);
	wifiMDShow(handle);
	wifiMonitorShow(handle);
	wifiIrLedShow(handle);
	wifiLockShow(handle);
	wifiSDShow(handle);
	wifiMicShow(handle);
	wifiBaterryShow(handle);
	winSetVisible(winItem(handle,WIFI_CAPTURE_PHOTO_ID),false);
	return 0;
}
static int wifiWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("wifi WinChild Open!!!\n");
	return 0;
}
static int wifiRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiLockShow(handle);
	winSetVisible(winItem(handle,WIFI_REC_TIME_ID),false);
	return 0;
}
static int wifiRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiLockShow(handle);
	wifiRecTimeShow(handle, 0);
	return 0;
}
static int wifiRecordResume1(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiLockShow(handle);
	return 0;
}

static int wifiRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiLockShow(handle);
	winSetVisible(winItem(handle,WIFI_REC_TIME_ID),false);
	return 0;
}
static int wifiRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiLockShow(handle);
	return 0;
}
extern u32 rx_len;
extern int staInactiveCnt;
extern u32 getDebug1SData(u32 index,u32 *sum);
static int wifiSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	
	mdRecordStopDeal();
	if(wifiGetTxCnt())
	{
		printf("tx cnt:%d\n",wifiGetTxCnt());
	}
	if(staInactiveCnt)
	{
		printf("staInactiveCnt1024:%d\n",staInactiveCnt);
		staInactiveCnt=0;
	}
	//printf("rx_len:%d\n",rx_len);
	
	//winSetResid(winItem(handle,WIFI_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int wifiSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	//static char debugStr1[100];
	//winSetResid(winItem(handle,WIFI_SYSTIME_ID),debugDataString(debugStr1,getDebug1SData));
}
static int wifiSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	wifiRecTimeShow(handle, parame[0]);
	//printf("record time:%d\n",parame[0]);
}
extern int wifiChannel;
static int wifiSysMsgWifiChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	winSetResid(winItem(handle,WIFI_SSID_ID),strToResId(getWifiSSIDInfoString()));
	winSetResid(winItem(handle,WIFI_PW_ID),strToResId(getWifiPwString()));
	winSetResid(winItem(handle,WIFI_CH_ID),strToResId(getWifiConnectInfoString()));
	return 0;
}
#if SUPPORT_RF_RECV
static int wifiKeyMsgRfRecv(winHandle handle,uint32 parameNum,uint32* parame)
{
	int cnt = 0;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	//printf("RfRecvCmd:0x%x\n", keyState);

	if(get_current_workmode() != WM_LIVE)
	{
		printf("err: Not in live mode,the current mode is %d!!!\n\n\n\n", get_current_workmode());
		return 0;
	}
	if(get_sd_status() == 0)
	{
		printf("unshieldedMsgRfRecv no sd\n");
		return 0;
	}
	if(keyState == 0x26)
	{
		localTaketPhotoStart();
		int delay=getTakePhotoTime();
		delay=delay*1000+2000;
		while(takePictrueIsWorking())
		{
			osal_task_sleep(10);
			cnt+=10;
			if(cnt>delay)
			{
				break;
			}
		}
	}
	else if(keyState == 0x46)
	{
		if (videoIsRecording())
			sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
		else
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
	}
}
#endif
msgDealInfor wifiMsgDeal[]=
{
	{SYS_OPEN_WINDOW,wifiOpenWin},
	{SYS_CLOSE_WINDOW,wifiCloseWin},
	{SYS_CHILE_COLSE,wifiWinChildClose},
	{SYS_CHILE_OPEN,wifiWinChildOpen},
	{SYS_TOUCH_WINDOW,wifiTouchWin},
	{KEY_EVENT_OK,wifiKeyMsgOk},
	{KEY_EVENT_LONG_OK,wifiKeyMsgLongOk},
	{KEY_EVENT_UP,wifiKeyMsgUp},
	{KEY_EVENT_DOWN,wifiKeyMsgDown},
	{KEY_EVENT_MENU,wifiKeyMsgMenu},
	{KEY_EVENT_MODE,wifiKeyMsgMode},
	//{KEY_EVENT_POWER,wifiKeyMsgPower},
	//{SYS_EVENT_SDC,wifiSysMsgSD},
	//{SYS_EVENT_USB,wifiSysMsgUSB},
	//{SYS_EVENT_BAT,wifiSysMsgBattery},
	{SYS_EVENT_MD,wifiSysMsgMD},
	{SYS_WIFI_CHANGE,wifiSysMsgWifiChange},
	{VID_EVENT_REC_STOP,wifiRecordStopped},
	//{VID_EVENT_REC_START,wifiRecordStarted},
	//{VID_EVENT_REC_RESTART,wifiRecordResume1},
	//{VID_EVENT_REC_ERROR,wifiRecordErr},
	//{VID_EVENT_LOCK_FILE,wifiRecordLockFile},
	{SYS_EVENT_500MS,wifiSysMsg500MS},
	{SYS_TIME_UPDATE,wifiSysMsgUpdateTime},
	{SYS_EVENT_1S,wifiSysMsg1S},
	#if SUPPORT_RF_RECV
	{KEY_EVENT_RF_RECV,wifiKeyMsgRfRecv},
	#endif
	{EVENT_MAX,NULL},
};

WINDOW(wifiWindow,wifiMsgDeal,wifiWin)


