#include "uiWin.h"


static msgDealFunc taskMsgDeal[EVENT_MAX];
static msgDealFunc unshieldedMsgDeal[EVENT_MAX];
extern msgDealInfor sysRecommendMsgDeal[];

void msgFuncRegister(msgDealInfor* infor)
{
	uint32 i;
	for(i=0;i<EVENT_MAX;i++)
		taskMsgDeal[i]=NULL;
	i=0;
	for(;;)
	{
		if(sysRecommendMsgDeal[i].msgId>=EVENT_MAX||sysRecommendMsgDeal[i].func==NULL)
			break;
		taskMsgDeal[sysRecommendMsgDeal[i].msgId]=sysRecommendMsgDeal[i].func;
		i++;
	}
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		taskMsgDeal[infor[i].msgId]=infor[i].func;
		i++;
	}
}
void unshieldedMsgFuncRegister(msgDealInfor* infor)
{
	uint32 i;
	for(i=0;i<EVENT_MAX;i++)
		unshieldedMsgDeal[i]=NULL;
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		unshieldedMsgDeal[infor[i].msgId]=infor[i].func;
		i++;
	}
}
u8 playKeySoundLater;
u8 fNotplayKeySound = 0;
void setNotplayKeySoundFlag(u8 flag)
{
	fNotplayKeySound = flag;
}
void msgDeal(uint32 msgType,uint32 msgData)
{
	if(msgType<EVENT_MAX)
	{
		if(msgType!=KEY_EVENT_POWEROFF&&msgType>=KEY_EVENT_START&&msgType<KEY_EVENT_END&&msgData==KEY_PRESSED)
		{
			if((configGet(CONFIG_ID_KEYSOUND)==R_ID_STR_COM_ON) && (0 == fNotplayKeySound))
			{
				if(dacIsPlaying())
					playKeySoundLater=1;
				else
					dacPlaySound(RES_MUSIC_KEY_SOUND,getSoundVol());
			}
		}
		if ((msgType>=KEY_EVENT_START) && (msgType<KEY_EVENT_END))
		{
			fNotplayKeySound = 0;
		}
		if(taskMsgDeal[msgType])
			taskMsgDeal[msgType](winGetCurrent(),1,&msgData);
		if(unshieldedMsgDeal[msgType])
			unshieldedMsgDeal[msgType](winGetCurrent(),1,&msgData);
	}
}

void msgDealByType(uint32 msgType,void* handle,uint32 parameNum,uint32* parame)
{
	if(taskMsgDeal[msgType])
		taskMsgDeal[msgType](handle,parameNum,parame);
	if(unshieldedMsgDeal[msgType])
		unshieldedMsgDeal[msgType](handle,parameNum,parame);
}
void msgDealByMsgInfor(msgDealInfor* infor,uint32 msgType,void* handle,uint32 parameNum,uint32* parame)
{
	uint32 i;
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		if(infor[i].msgId==msgType)
		{
			if(infor[i].func)
				infor[i].func( handle,parameNum,parame);
			break;
		}
			
		i++;
	}
}







