#include "uiWin.h"
#include "fatfs/ff.h"

static uint32 usbOutPowerOffCnt=0;
static uint32 batlowPowerOffCnt=0;

static int unshieldedKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];   
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
	}
	return 0;
}
static int unshieldedKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
	}
	return 0;
}
static int unshieldedKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
	}
	return 0;
}
static int unshieldedKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
	}
	return 0;
}
static int unshieldedKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
	}
	return 0;
}
extern void setKeyEn(bool en);
static int unshieldedKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sysAutoPoweroff(1);
		sysScreenSave(1);
		#if 0
		if(lcd_bk_on())
			lcd_bk_ctrl(0);
		else
		{
			sysScreenSave(1);
			setKeyEn(true);
		}
		printf("sys deal:powerkey\n");
		#endif
	}
	return 0;
}
static int unshieldedKeyMsgPowerOff(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uint32 lcdOn=0;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_POWER_OFF,0);
		printf("sys deal:power off\n");
	}
	return 0;
}

static int unshieldedMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	sysScreenSave(1);
	setKeyEn(true);
	if((SysCtrl.usb != USB_STAT_NULL))  // DC IN
	{
		usbOutPowerOffCnt=0;
		batlowPowerOffCnt=0;
	}
	else
		usbOutPowerOffCnt=5;
#if LCD_EN
	if((SysCtrl.usb == USB_STAT_PC)) 
		uiOpenWindow(&menuUsbWindow,0);
#else
	if((SysCtrl.usb == USB_STAT_PC)) 
		functionStart(FUNCTION_USB_DEVICE,R_ID_STR_SET_USBCAM);
#endif
	return 0;
}
static int unshieldedMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{      
	int ret;
	sysScreenSave(1);
	setKeyEn(true);
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		void *fp;
		fp = osal_fopen(_DATDSK_"/SELFTEST.bin", "r");
		if(fp)
		{
			osal_fclose(fp);
			uiOpenWindow(&selfTestWindow,0);
		}
	}
	return 0;
}
static int unshieldedMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.battery <= BATTERY_STAT_1)
	{
		if(batlowPowerOffCnt==0)
			batlowPowerOffCnt=5;
		uiOpenWindow(&tips1Window,2,TIPS_POWER_LOW,TIPS_KEEP_SHOWING);
	}
	return 0;
}
extern uint32 uvcIntCnt;
static int unshieldedMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(usbOutPowerOffCnt)
	{
		usbOutPowerOffCnt--;
		if(usbOutPowerOffCnt==0)
		{
			printf("sys deal:usb out,auto power off\n");
			functionStart(FUNCTION_POWER_OFF,0);
		}
	}
	if(batlowPowerOffCnt)
	{
		batlowPowerOffCnt--;
		if(batlowPowerOffCnt==0)
		{
			printf("sys deal:battery low,auto power off\n");
			functionStart(FUNCTION_POWER_OFF,0);
		}
	}
	sysAsternCheck();
	return 0;
}

msgDealInfor sysUnshieldedMsgDeal[]=
{
	{KEY_EVENT_OK,unshieldedKeyMsgOk},
	{KEY_EVENT_UP,unshieldedKeyMsgUp},
	{KEY_EVENT_DOWN,unshieldedKeyMsgDown},
	{KEY_EVENT_MENU,unshieldedKeyMsgMenu},
	{KEY_EVENT_MODE,unshieldedKeyMsgMode},
	{KEY_EVENT_POWER,unshieldedKeyMsgPower},
	{KEY_EVENT_POWEROFF,unshieldedKeyMsgPowerOff},
	{SYS_EVENT_SDC,unshieldedMsgSD},
	{SYS_EVENT_USB,unshieldedMsgUSB},
	{SYS_EVENT_BAT,unshieldedMsgBattery},
	{SYS_EVENT_1S,unshieldedMsg1S},
	{EVENT_MAX,NULL},
};






