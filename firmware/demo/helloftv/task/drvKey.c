#include "task/app.h"
#include "gpio_ops/gpio.h"
#include "touchPanel.h"
#include"uiWin.h"
    
#define KEY_SCAN_UNFINISHED 0xffffffff
#define ADD_KEY(key,keyADCvalue)  {key,keyADCvalue,0,0}
#define ADC_KEY_DEVIATION     20
typedef struct _keyInfor
{
	u8   keyType;
	u16  keyADCvalue;
	u16  keyADCmin;
	u16  keyADCmax;
}keyInfor;
keyInfor adckeyInfor[]=
{
#if 0//==for game develop board===
	ADD_KEY(KEY_EVENT_OK,  			0),
	ADD_KEY(KEY_EVENT_MENU,  		435),
	ADD_KEY(KEY_EVENT_UP,			658),
	ADD_KEY(KEY_EVENT_DOWN,			815),
	ADD_KEY(KEY_EVENT_MODE,		904),
#elif 0 // no lcd
	ADD_KEY(KEY_EVENT_DOWN,	510),
	ADD_KEY(KEY_EVENT_OK,		0),
#elif 0 // running DV
	ADD_KEY(KEY_EVENT_OK,     340),
	ADD_KEY(KEY_EVENT_UP,   680),
	ADD_KEY(KEY_EVENT_DOWN,   1024),
#elif 1 //printer
	ADD_KEY(KEY_EVENT_POWER,  0),
	ADD_KEY(KEY_EVENT_PHOTO,  936),
	ADD_KEY(KEY_EVENT_AF,  90),
	ADD_KEY(KEY_EVENT_ZOOMIN,  496),
	ADD_KEY(KEY_EVENT_ZOOMOUT,  47),
	ADD_KEY(KEY_EVENT_UP,  240),
	ADD_KEY(KEY_EVENT_DOWN,416),
	ADD_KEY(KEY_EVENT_LEFT,775),
	ADD_KEY(KEY_EVENT_RIGHT,  329),
	ADD_KEY(KEY_EVENT_OK,  710),
	ADD_KEY(KEY_EVENT_SETTING,518),
	ADD_KEY(KEY_EVENT_MODE,620),

#elif 0// ailaika 3 lu
	ADD_KEY(KEY_EVENT_MENU, 		320),
	ADD_KEY(KEY_EVENT_OK,			810),
	ADD_KEY(KEY_EVENT_UP,			660),

	ADD_KEY(KEY_EVENT_DOWN, 		480),
	ADD_KEY(KEY_EVENT_MODE, 		150),

	ADD_KEY(KEY_EVENT_OK, 		0),
#else//====for JRX develop board====
	ADD_KEY(KEY_EVENT_MENU,  		660),
	ADD_KEY(KEY_EVENT_UP,			484),
	#if AX329X_TYPE == 3291 
	ADD_KEY(KEY_EVENT_DOWN,  			0),
	ADD_KEY(KEY_EVENT_OK,			304),
	#else
	ADD_KEY(KEY_EVENT_OK,  			0),
	ADD_KEY(KEY_EVENT_DOWN,			304),
	#endif
	ADD_KEY(KEY_EVENT_MODE,			817),
#endif
};
void keyInit(void)
{
	u32 i;
	for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
	{
		if(adckeyInfor[i].keyADCvalue<=ADC_KEY_DEVIATION)
			adckeyInfor[i].keyADCmin=0;
		else
			adckeyInfor[i].keyADCmin=adckeyInfor[i].keyADCvalue-ADC_KEY_DEVIATION;
		adckeyInfor[i].keyADCmax=adckeyInfor[i].keyADCvalue+ADC_KEY_DEVIATION;
		if(adckeyInfor[i].keyADCmax>1024)   
			adckeyInfor[i].keyADCmax=1024;
	}
	//hal_gpioInit(GPIO_PB, GPIO_PIN0, GPIO_INPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(GPIO_PB, GPIO_PIN1, GPIO_INPUT, GPIO_PULL_FLOATING);
}
static uint32 keyADCvalue=0;
uint32 getKeyADCvalue(void)
{
	return keyADCvalue;
}
uint32 getKeyVal(void)
{
	static uint32 lastKey = KEY_EVENT_END;
	static uint32 keyScanTimes = 0;
	uint32 value, i, key = 0;
#if 0	
	if (hal_gpioRead(GPIO_PB, GPIO_PIN0))
	{
		key = KEY_EVENT_POWER;
	}
	else
#endif		
	{
		value = hal_adcGetChannel(ADC_CH_PB1);
		keyADCvalue=value;
		//printf("==key:%d\n", keyADCvalue);
		for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
		{
			if(value>=adckeyInfor[i].keyADCmin&&value<=adckeyInfor[i].keyADCmax)
		   	  key = adckeyInfor[i].keyType;
		}
	}
	
	if (lastKey != key)
	{
		lastKey = key;
		keyScanTimes = 1;
	}
	else
		keyScanTimes++;
	if (keyScanTimes >= 2)
	{
		keyScanTimes = 0;
		return key;
	}
	return KEY_SCAN_UNFINISHED;
}

uint32 getKey(void)
{
	static uint32 keyPowerPressCnt = 0;
	uint32 keyVal;
	keyVal = getKeyVal();
//	static uint32 lastTime = 0;
//	if(os_time_get()-lastTime>1000)
//	{
//		printf("key:%d\n", keyVal);
//		lastTime=os_time_get();
//	}
	if (keyVal == KEY_SCAN_UNFINISHED)
		return KEY_SCAN_UNFINISHED;

	static uint32 keyOkPressCnt=0;
	if(keyVal==KEY_EVENT_OK)
	{
		keyOkPressCnt++;
		if(keyOkPressCnt>=15)
		{
			keyVal=KEY_EVENT_LONG_OK;
		}
		else
			keyVal=0;	
	}
	else
	{
		if((keyOkPressCnt>0)&&(keyOkPressCnt<15))
			keyVal = KEY_EVENT_OK;
		keyOkPressCnt=0;
	}

	static uint32 keyPhotoPressCnt=0;
	if(keyVal==KEY_EVENT_PHOTO)
	{
		keyPhotoPressCnt++;
		if(keyPhotoPressCnt>=15)
		{
			keyVal=KEY_EVENT_LONG_PHOTO;
		}
		else
			keyVal=0;	
	}
	else
	{
		if((keyPhotoPressCnt>0)&&(keyPhotoPressCnt<15))
			keyVal = KEY_EVENT_PHOTO;
		keyPhotoPressCnt=0;
	}	
	
	#if 1
	if (keyVal == KEY_EVENT_POWER)
	{
		keyPowerPressCnt++;
		if (keyPowerPressCnt >= 30)
		{
			keyVal = KEY_EVENT_POWEROFF;
		}
		else
			keyVal = 0;
	}
	else
	{
		if (keyPowerPressCnt > 0 && keyPowerPressCnt < 30)
			keyVal = KEY_EVENT_POWER;
		keyPowerPressCnt = 0;
	}
	#endif
	return keyVal;
}
#define SCAN_KEY_CONTINUE_INTERVAL (150)
uint32 keyScanKey(void)
{
	static uint32 LastKeyVal = 0;
	static uint32 lastTime = 0;
	u32 curTime;
	uint32 CurKeyVal;
	uint32 KeySta = 0;
	CurKeyVal = getKey();
	if (CurKeyVal == KEY_SCAN_UNFINISHED)
		return 0;
	if (LastKeyVal != CurKeyVal)
	{
		if (CurKeyVal)
			KeySta = makeEvent(CurKeyVal, KEY_PRESSED);
		else
			KeySta = makeEvent(LastKeyVal, KEY_RELEASE);
		LastKeyVal = CurKeyVal;
		lastTime = os_time_get() + 500;
	}
	else
	{
		if (CurKeyVal)
		{
			curTime = os_time_get();
			if ((curTime > lastTime) && (curTime - lastTime >= SCAN_KEY_CONTINUE_INTERVAL))
			{
				lastTime = curTime;
				KeySta = makeEvent(CurKeyVal, KEY_CONTINUE);
			}
			else
				KeySta = 0;
		}
		else
			KeySta = 0;
	}
	return KeySta;
}
static bool keyEn=true;
void setKeyEn(bool en)
{
	keyEn=en;
}
void taskScanKey(void)
{
	uint32 msgType;
	uint32 msgData;
	uint32 KeySta = keyScanKey();
	if(KeySta)
	{
		msgType=getType(KeySta);
		msgData=getValue(KeySta);
		if(msgType==KEY_EVENT_POWEROFF||msgType==KEY_EVENT_POWER)
			sendMsgNonblock(OS_TASK_UI,msgType, msgData);
		else
		{
			if(keyEn&&lcd_bk_on())
				sendMsgNonblock(OS_TASK_UI,msgType, msgData);
			else
			{
				sysScreenSave(1);
				if(msgData==KEY_RELEASE)
					keyEn=true;
			}
		}
	}
}
extern u8 playKeySoundLater;
extern uint16 LCD_WIDTH;
extern uint16 LCD_HEIGHT;
static BOOL quickResponsePress=FALSE;
static int moveThreshod=8;
BOOL getTPQuickResponsePress(void)
{
	return quickResponsePress;
}
void setTPQuickResponsePress(BOOL en)
{
	quickResponsePress=en;
	if(en)
		moveThreshod=1000;
	else
		moveThreshod=8;
}
//º¯ÊýÖ¸Õë
typedef void (*TP_cb )(uiRect *rect);
TP_cb TP_key_cb = NULL;

void register_TP_callback(TP_cb cb)
{
	TP_key_cb = cb;
}
void taskScanTP(void)
{
#if TP_FUCTION
	static uint32 lastTime = 0,cnt=0;
	static uint32 scanInterval=20;
	static uint32 lastGsType = GESTRUE_TYPE_NONE;
	gestrueInfor* gsInfor;
	uiRect rect;
	/*u8 lcdon = get_expand_backlight(); ¡À31a1?¡À?2???DD¡ä£¤?t
	if(!lcdon)
	{
		return;
	}*/
	if((os_time_get()-lastTime)<=scanInterval)
		return ;
	lastTime = os_time_get();
	cnt++;
	gsInfor=touchPanelGetInfor(moveThreshod);
	if(gsInfor->type==GESTRUE_TYPE_PRESS)
	{
		if(quickResponsePress==TRUE&&lastGsType!=GESTRUE_TYPE_PRESS)
		{
			rect.x0=gsInfor->x<2?0:gsInfor->x-1;
			rect.y0=gsInfor->y<2?0:gsInfor->y-1;
			rect.x1=gsInfor->x+1;
			rect.y1=gsInfor->y+1;
			rect.x1=rect.x1>=LCD_WIDTH?LCD_WIDTH-1:rect.x1;
			rect.y1=rect.y1>=LCD_HEIGHT?LCD_HEIGHT-1:rect.y1;
			//deg_Printf("taskScanTP .1 \n");
			//printf("===1==[%d,%d],[%d,%d]\n", rect.x0,rect.y0, rect.x1, rect.y1);
			if(winTouch(&rect,TOUCH_PRESS))
			{
				sysAutoPoweroff(1);
				sysScreenSave(1);
				/*if(taskCurrent()!=TASK_PLAY_BACK && taskCurrent()!=TASK_AUDIO_PLAYER)
				deamon_keysound_play();*/
				if(configGet(CONFIG_ID_KEYSOUND)==R_ID_STR_COM_ON)
				{
					if(dacIsPlaying())
						playKeySoundLater=1;
					else
						dacPlaySound(RES_MUSIC_KEY_SOUND,getSoundVol());
				}
			}
				
		}
		scanInterval=80;
	}
	else if(gsInfor->type==GESTRUE_TYPE_MOVE)
	{
		//deg_Printf("taskScanTP .1.11 \n");
		if(gsInfor->speed>0&&scanInterval!=60)
		{
			//deg_Printf("sliding:%d\n",gsInfor->slidDiretion);
			//XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_SLIDING_GESTRUE,gsInfor->slidDiretion));
			//printf("===2==slidDiretion:[%d]\n", gsInfor->slidDiretion);
			sendMsgNonblock(OS_TASK_UI,SYS_SLIDING_GESTRUE,gsInfor->slidDiretion);
		}
		if(scanInterval==60)
			scanInterval=140;
		else
			scanInterval=200;
	}
	else if(gsInfor->type==GESTRUE_TYPE_NONE)
	{
		//printf("taskScanTP .--%d \n", lastGsType);
		if(lastGsType==GESTRUE_TYPE_PRESS)
		{
			if(quickResponsePress==FALSE)
			{
				
				rect.x0=gsInfor->x<2?0:gsInfor->x-1;
				rect.y0=gsInfor->y<2?0:gsInfor->y-1;
				rect.x1=gsInfor->x+1;
				rect.y1=gsInfor->y+1;
				rect.x1=rect.x1>=LCD_WIDTH?LCD_WIDTH-1:rect.x1;
				rect.y1=rect.y1>=LCD_HEIGHT?LCD_HEIGHT-1:rect.y1;
				//deg_Printf("taskScanTP .3 \n");
				//printf("===3==[%d,%d],[%d,%d]---\n", rect.x0,rect.y0, rect.x1, rect.y1);
				if(winTouch(&rect,TOUCH_PRESS))
				{
					sysAutoPoweroff(1);
					sysScreenSave(1);
					/*if(taskCurrent()!=TASK_PLAY_BACK&& taskCurrent()!=TASK_AUDIO_PLAYER)
						deamon_keysound_play(); */
					if(configGet(CONFIG_ID_KEYSOUND)==R_ID_STR_COM_ON)
					{
						if(dacIsPlaying())
							playKeySoundLater=1;
						else
							dacPlaySound(RES_MUSIC_KEY_SOUND,getSoundVol());
					}
				}	
				winTouch(NULL,TOUCH_RELEASE);

				//send rect to game 
				if(TP_key_cb)
					TP_key_cb(&rect);
				//gameScanTPKey_event_proc(&rect);
			}
			else
			{
				//deg_Printf("taskScanTP .4 \n");
				winTouch(NULL,TOUCH_RELEASE);
			}
		}
		else if(lastGsType==GESTRUE_TYPE_MOVE)
		{
			//deg_Printf("taskScanTP .5 \n");
			if(quickResponsePress==TRUE)
			{
				//Reset_PowerOffTime();
				winTouch(NULL,TOUCH_OVER);
			}
			//printf("===3.2==slidDiretion:[%d]\n", gsInfor->slidDiretion);	
			if(gsInfor->speed>0)
				sendMsgNonblock(OS_TASK_UI,SYS_SLID_RELEASE,gsInfor->slidDiretion);
				//XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_SLID_RELEASE,gsInfor->slidDiretion));
		}
		if(videoIsRecording())
			scanInterval=60;
		else
			scanInterval=30;
	}
	lastGsType=gsInfor->type;
#endif	
}

