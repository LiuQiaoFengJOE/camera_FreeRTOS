#include "uiWin.h"

static int sysMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionChange();
	}
	return 0;
}
static int sysMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int sysMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("sys recommend deal:change function\n");
		//functionChange();
	}
	return 0;
}
static int sysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int sysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int SysMsgAstern(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.astern == ASTERN_ON)
	{
		INT8U funId = functionCurrent();
		if ((funId != FUNCTION_VIDEO_RECORD) && (funId != FUNCTION_TAKE_PHOTO))
			functionStart(FUNCTION_VIDEO_RECORD,0);
		else
			uiOpenWindow(&asternWindow,0);
		printf("player : astern on\n");
	}
	return 0;
}
static int SysMsgUsensor(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
#if SUPPORT_RF_RECV
static int keyMsgRfRecv(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	//printf("RfRecvCmd:0x%x\n", keyState);
	if(keyState == 0x26)
	{
		if(functionCurrent() != FUNCTION_TAKE_PHOTO)
		functionStart(FUNCTION_TAKE_PHOTO,0);
	}
	else if(keyState == 0x46)
	{
		if(functionCurrent() != FUNCTION_VIDEO_RECORD)
		functionStart(FUNCTION_VIDEO_RECORD,0);
	}
	sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, 0);
	return 0;
}
#endif
msgDealInfor sysRecommendMsgDeal[]=
{
	{KEY_EVENT_POWER,sysMsgPower},
	{KEY_EVENT_MENU,sysMsgMenu},
	{KEY_EVENT_MODE,sysMsgMode},
	{SYS_EVENT_USB,sysMsgUSB},
	{SYS_EVENT_BAT,sysMsgBattery},
	{SYS_EVENT_ASTERN,SysMsgAstern},
	{SYS_EVENT_USENSOR,SysMsgUsensor},
	#if SUPPORT_RF_RECV
	{KEY_EVENT_RF_RECV,keyMsgRfRecv},
	#endif
	{EVENT_MAX,NULL},
};






