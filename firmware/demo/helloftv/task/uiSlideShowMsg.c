#include "uiSlideShowRes.c"

extern void slideShowEn(bool en,u32 intervalTime);
extern void slideShowPause(bool en);
extern bool slideShowIsPause(void);

static int slideShowKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int slideShowKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int slideShowKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int slideShowKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int slideShowKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int slideShowSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int slideShowSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int slideShowSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int slideShowOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("slideShow Open Win!!!\n");
	return 0;
}
static int slideShowCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("slideShow Close Win!!!\n");
	return 0;
}
static int slideShowWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("slideShow WinChild Close!!!\n");
	return 0;
}
static int slideShowSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor slideShowMsgDeal[]=
{
	{SYS_OPEN_WINDOW,slideShowOpenWin},
	{SYS_CLOSE_WINDOW,slideShowCloseWin},
	{SYS_CHILE_COLSE,slideShowWinChildClose},
	{KEY_EVENT_OK,slideShowKeyMsgOk},
	{KEY_EVENT_UP,slideShowKeyMsgUp},
	{KEY_EVENT_DOWN,slideShowKeyMsgDown},
	{KEY_EVENT_MENU,slideShowKeyMsgMenu},
	{KEY_EVENT_MODE,slideShowKeyMsgMode},
	{SYS_EVENT_SDC,slideShowSysMsgSD},
	{SYS_EVENT_USB,slideShowSysMsgUSB},
	{SYS_EVENT_BAT,slideShowSysMsgBattery},
	{SYS_EVENT_1S,slideShowSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(slideShowWindow,slideShowMsgDeal,slideShowWin)



