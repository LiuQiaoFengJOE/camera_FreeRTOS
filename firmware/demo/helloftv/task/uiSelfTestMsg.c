#include "uiSelfTestRes.c"

static int selfTestKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int selfTestKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int selfTestKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int selfTestKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	
	}
	return 0;
}
static int selfTestKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		
	}
	return 0;
}
static int selfTestSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NULL)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int selfTestSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int selfTestSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int selfTestOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("selfTest Open Win!!!\n");
	winSetResid(winItem(handle,TEST_LCD_ID),strToResId(LcdGetName()));
	winSetResid(winItem(handle,TEST_SENSOR_ID),strToResId(SensorGetName()));
	return 0;
}
static int selfTestCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("selfTest Close Win!!!\n");

	return 0;
}
static int selfTestWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("selfTest WinChild Close!!!\n");
	return 0;
}
static int selfTestSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static char adcKeyStr[25]="keyADC value:";
	uintToString(&adcKeyStr[13],11,getKeyADCvalue());
	winSetResid(winItem(handle,TEST_ADCKEY_VAL_ID),strToResId(adcKeyStr));
	static char batteryADCStr[15];
	if(SysCtrl.usb == USB_STAT_NULL)
	{
		batteryADCStr[0]='0'+getCurBatteryLevel();
		batteryADCStr[1]='-';
		uintToString(&batteryADCStr[2],13,getCurBatteryADC());
		winSetResid(winItem(handle,TEST_BATTERY_ID),strToResId(batteryADCStr));
	}
	else
		winSetResid(winItem(handle,TEST_BATTERY_ID),strToResId("charging"));
	return 0;
}
static int selfTestSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	
	return 0;
}


msgDealInfor selfTestMsgDeal[]=
{
	{SYS_OPEN_WINDOW,selfTestOpenWin},
	{SYS_CLOSE_WINDOW,selfTestCloseWin},
	{SYS_CHILE_COLSE,selfTestWinChildClose},
	{KEY_EVENT_OK,selfTestKeyMsgOk},
	{KEY_EVENT_UP,selfTestKeyMsgUp},
	{KEY_EVENT_DOWN,selfTestKeyMsgDown},
	{KEY_EVENT_MENU,selfTestKeyMsgMenu},
	{KEY_EVENT_MODE,selfTestKeyMsgMode},
	{SYS_EVENT_SDC,selfTestSysMsgSD},
	{SYS_EVENT_USB,selfTestSysMsgUSB},
	{SYS_EVENT_BAT,selfTestSysMsgBattery},
	{SYS_EVENT_1S,selfTestSysMsg1S},
	{SYS_EVENT_500MS,selfTestSysMsg500MS},
	{EVENT_MAX,NULL},
};

WINDOW(selfTestWindow,selfTestMsgDeal,selfTestWin)



