#include "app.h"
#include "funcUsbDevRes.c"
#include "mediaFile/demuxer.h"

extern int usbDevIsRecording(void);

static int usbDevKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int usbDevKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd,i;
	static  bool   stopRec=false;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}

static int usbDevKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fd;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
//extern MENU(movie);

static int usbDevKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int usbDevKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("usb device change function\n");
		functionChange();
	}
	return 0;
}
static int usbDevSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int usbDevSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(!getUsbConnect())
		functionChange();
	return 0;
}

static int usbDevSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int usbDevSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int usbDevSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}


static int usbDevOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("usbDev Open Win!!!\n");
	return 0;
}
static int usbDevCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("usbDev Close Win!!!\n");
	return 0;
}
static int usbDevWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("usbDev WinChild Close!!!\n");
	return 0;
}
static int usbDevWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("usbDev WinChild Open!!!\n");
	return 0;
}
static int usbDevRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDevRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDevRecordResume1(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int usbDevRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDevRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDevSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	winSetResid(winItem(handle,USBDEV_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	return 0;
}
static int usbDevSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
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
static int usbDevSysMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
}

msgDealInfor usbDevMsgDeal[]=
{
	{SYS_OPEN_WINDOW,usbDevOpenWin},
	{SYS_CLOSE_WINDOW,usbDevCloseWin},
	{SYS_CHILE_COLSE,usbDevWinChildClose},
	{SYS_CHILE_OPEN,usbDevWinChildOpen},
	{KEY_EVENT_OK,usbDevKeyMsgOk},
	{KEY_EVENT_UP,usbDevKeyMsgUp},
	{KEY_EVENT_DOWN,usbDevKeyMsgDown},
	{KEY_EVENT_MENU,usbDevKeyMsgMenu},
	{KEY_EVENT_MODE,usbDevKeyMsgMode},
	//{SYS_EVENT_SDC,usbDevSysMsgSD},
	{SYS_EVENT_USB,usbDevSysMsgUSB},
	//{SYS_EVENT_BAT,usbDevSysMsgBattery},
	{SYS_EVENT_MD,usbDevSysMsgMD},
	{SYS_TIME_UPDATE,usbDevSysMsgUpdateTime},
	{SYS_EVENT_1S,usbDevSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(usbDevWindow,usbDevMsgDeal,usbDevWin)


