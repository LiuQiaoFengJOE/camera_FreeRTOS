#include "uiWin.h" 

static sysFunction* functionArray[FUNCTION_MAX];
static sysFunction* curFunction;
static functionID   curFunctionId;
static functionID   nextFunctionId;
static functionID   preFunctionId;

extern msgDealInfor sysUnshieldedMsgDeal[];

void sendDrawUIMsg(void)
{
	msgDealByType(SYS_DRAW_UI,winGetCurrent(),0,NULL);
};

void functionInit(void)
{
	int i;
	for(i=0;i<FUNCTION_MAX;i++)
	{
		functionArray[i]=NULL;
	}
	curFunction=NULL;
	curFunctionId=FUNCTION_POWER_ON;
	nextFunctionId=FUNCTION_MAX;
	preFunctionId=FUNCTION_MAX;
}
void functionInfor(int n)
{
	int i;
	printf("function infor[%d]:\n",n);
	for(i=0;i<FUNCTION_MAX;i++)
	{
		if(functionArray[i])
			printf("function [%s] normal\n",functionArray[i]->name);
		else
			printf("function [%d] abnormal\n",i);
	}
}
void functionRegister(functionID id,sysFunction* function)
{
	if(id<FUNCTION_MAX)
	{
		if(functionArray[id])
			printf("waring: function[%s]already registered\n",functionArray[id]->name);
		functionArray[id]=function;
		printf("function[%s] register\n",functionArray[id]->name);
	}
	else
	{
		printf("function id[%d] too large,max id:%d\n",id,FUNCTION_MAX);
	}
}
functionID functionCurrent(void)
{
	return curFunctionId;
}
functionID functionNext(void)
{
	return nextFunctionId;
}
functionID functionPre(void)
{
	return preFunctionId;
}


void functionStart(functionID id,uint32 arg)
{
	if(id<FUNCTION_MAX)
	{
		//if(id==FUNCTION_WIFI)
		//	lcdOsdClear(R_COLOR_BLACK);
		if(nextFunctionId==FUNCTION_POWER_OFF)
		{
			printf("function [%s] has highest priority,function [%d] was ignored\n",functionArray[FUNCTION_POWER_OFF]->name,id);
			return;
		}
		if(functionArray[id])
		{
			functionArray[id]->arg=arg;
			preFunctionId=curFunctionId;
			nextFunctionId=id;
			printf("will start function [%s]\n",functionArray[id]->name);
		}
		else
		{
			if(id==FUNCTION_USB_UPDATE)
			{
				nextFunctionId=id;
				printf("start usb update\n");
			}
			else
			{
				printf("start function [%d] not registered\n",id);
			}
		}
	}
}
void functionChange(void)
{
	//return ;
	switch(curFunctionId)
	{
		case FUNCTION_VIDEO_RECORD:
			#if SUPPORT_AUD_REC
			functionStart(FUNCTION_AUDIO_RECORD,0);
			break;
		case FUNCTION_AUDIO_RECORD:
			#endif
			functionStart(FUNCTION_TAKE_PHOTO,0);
			break;
		case FUNCTION_TAKE_PHOTO:
			functionStart(FUNCTION_PLAY_BACK,0);
			break;
		case FUNCTION_PLAY_BACK:
			functionStart(FUNCTION_VIDEO_RECORD,0);
			break;
		default:
			printf("function change,start default video function\n");
			functionStart(FUNCTION_VIDEO_RECORD,0);
			break;
	}
}

extern sysFunction functionPowerOff;
extern sysFunction functionVideoRecorde;
#if SUPPORT_AUD_REC
extern sysFunction functionAudioRecord;
#endif
extern sysFunction functionWifi;
extern sysFunction functionPlayBack;
extern sysFunction functionUSBDev;
extern sysFunction functionTakePhotoe;
extern sysFunction functionMainMenu;
extern sysFunction functionMainMenu2;
extern sysFunction functionMainMenu3;
extern sysFunction functionAudioPlayer;
#if GAME_FUNC_EN
extern sysFunction functionGameMenu;
extern sysFunction functionGame;
#endif
extern sysFunction functionRoleMenu;
extern sysFunction functionljfNetTypeMenu;
//extern sysFunction functionSettingMenu;
extern sysFunction functionPhotoScore;
extern sysFunction functionApp;
void registerFunction(void)
{
	functionRegister(FUNCTION_POWER_OFF,&functionPowerOff);
	functionRegister(FUNCTION_VIDEO_RECORD,&functionVideoRecorde);
	#if SUPPORT_AUD_REC
	functionRegister(FUNCTION_AUDIO_RECORD,&functionAudioRecord);
	#endif
	functionRegister(FUNCTION_WIFI,&functionWifi);
#if GAME_FUNC_EN
	functionRegister(FUNCTION_GAME_MENU,&functionGameMenu);
	functionRegister(FUNCTION_GAME,&functionGame);
#endif
	functionRegister(FUNCTION_PLAY_BACK,&functionPlayBack);
	functionRegister(FUNCTION_TAKE_PHOTO,&functionTakePhotoe);
	functionRegister(FUNCTION_USB_DEVICE,&functionUSBDev);
	functionRegister(FUNCTION_MAINMENU,&functionMainMenu);
	functionRegister(FUNCTION_MAINMENU2,&functionMainMenu2);
	functionRegister(FUNCTION_MAINMENU3,&functionMainMenu3);
	functionRegister(FUNCTION_APP,&functionApp);
	functionRegister(FUNCTION_AUDIO_PLAYER,&functionAudioPlayer);
	functionRegister(FUNCTION_PHOTO_ENCODE,&functionPhotoScore);	
	functionRegister(FUNCTION_ROLE,&functionRoleMenu);
	functionRegister(FUNCTION_NETTYPE,&functionljfNetTypeMenu);
}

System_Ctrl_T SysCtrl;

void taskUI(void* arg)
{
	uint32_t msg_t, msg_v;
	uint32 n=0;
	u32 msgType,msgData;
	printf("taskUI enter\n");
	test_workmode();
	unshieldedMsgFuncRegister(sysUnshieldedMsgDeal);
	functionInit();
	registerFunction();
	functionStart(FUNCTION_MAINMENU,2);
	configSet(CONFIG_ID_WIFIPREVIERES,R_ID_STR_RES_VGA);
	while(1)
	{
		if(curFunction)
		{
			printf("function [%s] closed\n",curFunction->name);
			if(curFunction->functionClose)
				curFunction->functionClose(curFunction->arg);
			winDestroyDeskTopChildWin();
		}
		uiHeapInfor(n++);
		curFunctionId=nextFunctionId;
		nextFunctionId=FUNCTION_MAX;
		curFunction=functionArray[curFunctionId];
		if(curFunction==NULL)
		{
			printf("function [%d] not registered,start [%s] function\n",curFunctionId,functionArray[FUNCTION_POWER_OFF]->name);
			curFunctionId=FUNCTION_POWER_OFF;
			curFunction=functionArray[curFunctionId];
		}
		printf("function [%s] open\n",curFunction->name);
		if(curFunction->functionOpen)
			curFunction->functionOpen(curFunction->arg);
		//sendMsg(OS_FUNCTION_UI,SYS_DRAW_UI, 0);
		while(1)
		{
			if(nextFunctionId<FUNCTION_MAX)
				break;
			drawUIService(false);
			msgType=EVENT_MAX;
			msgData=0;
			if(recMsg(OS_TASK_UI,&msgType,&msgData)>0)
			{
				//deg_Printf("ui msgid:%d, data:%d\n",msgType,msgData);
				msgDeal(msgType,msgData);
			}
		}	
	}
}











