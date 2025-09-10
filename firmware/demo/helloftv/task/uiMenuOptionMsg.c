#include "touchPanel.h"
#include "uiMenuOptionRes.c"

static menuItem* currentItem=NULL;
static uint32 getOptionResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=currentItem->pOption[item].image;
	if(str)
	{
		*str=currentItem->pOption[item].str;
		if(*str==R_ID_STR_RES_480FHD)
			*str="848x480";
	}
}

static int menuOptionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(currentItem)
		{
			configSet(currentItem->configId,currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str);
			if(currentItem->configId==CONFIG_ID_TIMESTAMP){
				if(currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str==R_ID_STR_COM_OFF)
					SysCtrl.timeStamp=0;
				else
					SysCtrl.timeStamp=1;
			}
			configSys(currentItem->configId);
		}
		winDestroy(&handle);
	}
	return 0;
}
static int menuOptionKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
static int menuOptionKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
static int menuOptionKeyMsgSetting(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuOptionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuOptionSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int menuOptionTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("menuOptionWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		setNotplayKeySoundFlag(1);
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, KEY_PRESSED);
	}
	return 0;
}
static int menuOptionSlidRelease(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=1)
	{
		printf("playBackSlidRelease, parame num error %d\n",parameNum);
		return 0;
	}
	if(parame[0]==SLID_DIRECT_UP)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_DOWN, KEY_PRESSED);
	else if(parame[0]==SLID_DIRECT_DOWN)
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_UP, KEY_PRESSED);
	
	return 0;
}
static int menuOptionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 itemNum=0,config,i;
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	currentItem=(menu*)parame[0];
	printf("menuOption Open Win!!!\n");
	itemNum=itemManageSetHeightAvgGap(winItem(handle,OPTION_SELECT_ID),R1h(32));
	//for(i=0;i<itemNum;i++)
	//	itemManageSetColumnMargin(winItem(handle,OPTION_SELECT_ID),i,1,5);
	itemManageCreateItem(winItem(handle,OPTION_SELECT_ID),itemCreatemenuOption,getOptionResInfor,currentItem->optionSum);
	itemManageSetCharInfor(winItem(handle,OPTION_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_BLACK);
	config=configGet(currentItem->configId);
	itemNum=0;
	while(itemNum<currentItem->optionSum)
	{
		if(currentItem->pOption[itemNum].str==config)
			break;
		itemNum++;
	}
	if(itemNum>=currentItem->optionSum)
		itemNum=0;
	itemManageSetCurItem(winItem(handle,OPTION_SELECT_ID),itemNum);
	winSetResid(winItem(handle,OPTION_TITLE_ID),currentItem->str);
	return 0;
}
static int menuOptionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("menuOption Close Win!!!\n");
	currentItem=NULL;
	return 0;
}
static int menuOptionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("menuOption WinChild Close!!!\n");
	return 0;
}
static int menuOptionSysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor menuOptionMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuOptionOpenWin},
	{SYS_CLOSE_WINDOW,menuOptionCloseWin},
	{SYS_CHILE_COLSE,menuOptionWinChildClose},
	{SYS_TOUCH_WINDOW,menuOptionTouchWin},
	{SYS_SLID_RELEASE,menuOptionSlidRelease},
	{KEY_EVENT_OK,menuOptionKeyMsgOk},
	{KEY_EVENT_LEFT,menuOptionKeyMsgUp},
	{KEY_EVENT_RIGHT,menuOptionKeyMsgDown},
	{KEY_EVENT_SETTING,menuOptionKeyMsgSetting},
	{KEY_EVENT_MODE,menuOptionKeyMsgMode},
	{SYS_EVENT_SDC,menuOptionSysMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(menuOptionWindow,menuOptionMsgDeal,menuOptionWin)


