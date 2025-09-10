#include "task/app.h"
#include "gpio_ops/gpio.h"
#include "drv/rtc/rtc.h"

void sysAsternCheck(void)
{
	bool ret;

	ret = api_astern11_status()|api_astern2_status();
	if((SysCtrl.astern==ASTERN_OFF) && (ret == true))
	{
		printf("astern on\n");
		SysCtrl.astern = ASTERN_ON;
		sendMsgNonblock(OS_TASK_UI,SYS_EVENT_ASTERN,ASTERN_ON);
	}
	else if((SysCtrl.astern==ASTERN_ON) && (ret == false))
	{
		printf("astern off\n");
		SysCtrl.astern = ASTERN_OFF;
		sendMsgNonblock(OS_TASK_UI,SYS_EVENT_ASTERN,ASTERN_OFF);
	}
}


void timerMsgSend(void)
{
	static u8 lastSec=255;
	static uint32 game_time = 0;
	static uint32 lastTime = 0;
	HAL_RTC_T* rtcTime;
	rtcTime=hal_rtcTimeGet();
	if(lastSec!=rtcTime->sec)
	{
		lastSec=rtcTime->sec;
		sendMsgNonblock(OS_TASK_UI,SYS_EVENT_1S, 0);
		resBuffConut();
	}
#if 1	
#if GAME_FUNC_EN
	if((FUNCTION_GAME == functionCurrent())&&(os_time_get()-game_time)>50)
	{
		sendMsgNonblock(OS_TASK_UI,SYS_EVENT_50MS, 0);
		game_time = os_time_get();
	}
#endif	
	if((os_time_get()-lastTime)<=500)
		return;
	lastTime = os_time_get();
	sendMsgNonblock(OS_TASK_UI,SYS_EVENT_500MS, 0);
#endif
}
extern s32 hal_usbdCheck(void);
u32 getUsbConnect(void)
{
	return ax32xx_WKI1Read();
}
extern int usexosc;
void sysScanUsb()
{
	//if(USB2_CAMERA||usexosc==0)
	#if (USB2_CAMERA)
	{
		if(getUsbConnect())
		{
			SysCtrl.usb=USB_STAT_DCIN;
		}
		else
		{
			batteryDet();
		}
	}
	#else
	{
		s32 ret;
		static u8 usbdev=0;
		if(getUsbConnect())	// dc 5V  check
		{
			if(USB_STAT_NULL==SysCtrl.usb)
			{
				ret = hal_usbdCheck();
				if(ret>=0)
				{
					SysCtrl.usb=USB_STAT_DCIN;
					sendMsgNonblock(OS_TASK_UI,SYS_EVENT_USB,SysCtrl.usb);
					if(ret>0)
						hal_usbdInit(0);
				}
			}
			else 
			{
				if((USB_STAT_DCIN==SysCtrl.usb)&&usbdev_rqu()&&(usbdev==0))
				{
					SysCtrl.usb = USB_STAT_PC;
					usbdev = 1;
					sendMsgNonblock(OS_TASK_UI,SYS_EVENT_USB,SysCtrl.usb);
				}
			}
		}
		else
		{
			if(USB_STAT_NULL!=SysCtrl.usb)
			{
				SysCtrl.usb=USB_STAT_NULL;  // dc out
				printf("usb out\n");
				usbdev=0;
				usbdev_reset();
				sendMsgNonblock(OS_TASK_UI,SYS_EVENT_USB,SysCtrl.usb);
			}
			//==check bat==
			batteryDet();
		}
	}
	#endif
}
int sysAutoPoweroff(int ClearPowerOffTime)
{
#define TIME_INTERNAL        500
	INT8U funcID;
	INT32U AutoPowerOffTime;
	static INT32U LastTime=0,PowerOffTime=0;
	if(ClearPowerOffTime)
		goto RESET_POWER_OFF_TIME;
	if((INT32U)(os_time_get()-LastTime)<TIME_INTERNAL)
		return 0;
	LastTime=os_time_get();

	funcID = functionCurrent();
	if(funcID == FUNCTION_POWER_OFF)
		return 0;
	if((funcID == FUNCTION_WIFI && (videoIsRecording()||clientConnect()))||\
		(funcID == FUNCTION_VIDEO_RECORD && videoIsRecording()) || \
		(funcID == FUNCTION_PLAY_BACK && getPlayStat()==PLAY_BACK_PLAYING)||\
		(funcID == FUNCTION_USB_DEVICE)||(funcID == FUNCTION_POWER_ON))
	{
		goto RESET_POWER_OFF_TIME;
	}
		
	AutoPowerOffTime=configValue2Int(CONFIG_ID_AUTOOFF)*1000;
	if(AutoPowerOffTime==0)
		goto RESET_POWER_OFF_TIME;

	PowerOffTime+=TIME_INTERNAL;
	if(PowerOffTime>=AutoPowerOffTime)
	{
		printf("Auto power off\n");
		functionStart(FUNCTION_POWER_OFF,0);
	}
	return 0;
RESET_POWER_OFF_TIME:
	PowerOffTime=0;
	return 0;
}
int sysScreenSave(int on)
{
	static INT32U LastTime=0;
	uint32 cfg_save;
	uint32 lcdOn=0;
	if(functionCurrent() == FUNCTION_POWER_OFF)
		return 0;
	cfg_save = configValue2Int(CONFIG_ID_SCREENSAVE);
   	if(SysCtrl.astern == ASTERN_ON||on)
	{
		lcd_bk_ctrl(1);			
		LastTime=os_time_get();
		return 1;
	}
	if(cfg_save==0)
		return 0;
	if((os_time_get()-LastTime)>cfg_save*1000)
	{
		LastTime=os_time_get();
		lcd_bk_ctrl(0);
	}
	return 0;
}
int mdirqFunc(unsigned int n, void * d, void * r)
{
	//printf("md irq\n");
	if(functionCurrent() != FUNCTION_WIFI&&functionCurrent() != FUNCTION_VIDEO_RECORD)
		return 0;
	if(get_current_workmode()!=WM_LIVE&&get_current_workmode()!=WM_RECORD)
		return 0;
	mdRecordCntReset();
	if(!videoIsRecording())
		sendMsgNonblock(OS_TASK_UI,SYS_EVENT_MD,0);
}

void mdSetEn(u32 d)
{
	if(d)
	{
		csiirq_register_mdt_action(mdirqFunc, NULL);
		ax32xx_mdEnable(1);
	}
	else
	{
		ax32xx_mdEnable(0);
	}
}

extern u8 playKeySoundLater;
u8 soundVolume=100;
u32 getSoundVol(void)
{
	return soundVolume;
}
void taskSysScan(void)
{
	isp_process_check();
	taskScanKey();
	taskScanTP();
	timerMsgSend();
	sysScanUsb();
	sysAutoPoweroff(0);
	sysScreenSave(0);
	usbEnumTry();
	usb2EnumTry();
	if(playKeySoundLater&&!dacIsPlaying())
	{
		playKeySoundLater=0;
		dacPlaySound(RES_MUSIC_KEY_SOUND,getSoundVol());
	}
}

void taskScan(void *arg)
{
	u32 lastTime = 0;
	u32 spendTime, delayTime;
	keyInit();
	printf("taskScan enter\n");
	while (1)
	{
		if (os_time_get() < lastTime)
			lastTime = os_time_get();
		spendTime = os_time_get() - lastTime;
		if (spendTime < 25)
		{
			delayTime = 25 - spendTime;
			if (delayTime > 3)
				osal_task_sleep(delayTime);
		}
		lastTime = os_time_get();
		taskSysScan();
	}
}

