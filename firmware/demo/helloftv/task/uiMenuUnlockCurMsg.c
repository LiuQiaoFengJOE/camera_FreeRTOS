#include "uiMenuUnlockCurRes.c"

static uint32 getunlockCurResInfor(uint32 item,uint32* image,uint32* str)
{
	if(item==0)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_OK;
	}
	else if(item==1)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_CANCEL;
	}
	return 0;
}

static int unlockCurKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	char *srcName;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,UNLOCKCUR_SELECT_ID));
		if(item==0)
		{
			if(!curFloderHasFile())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return 0;
			}
//			if(!curPlayFileIsVid())
//			{
//				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
//				return 0;
//			}
			if(!curFileIsLock())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				return 0;
			}
			setCurPlayFileLock(0);
			uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int unlockCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,UNLOCKCUR_SELECT_ID));
	}
	return 0;
}
static int unlockCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,UNLOCKCUR_SELECT_ID));
	}
	return 0;
}
static int unlockCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int unlockCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}
static int unlockCurTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("unlockCurWin, parame num error %d\n",parameNum);
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
static int unlockCurOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,UNLOCKCUR_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKCUR_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKCUR_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,UNLOCKCUR_SELECT_ID),itemCreatemenuOption,getunlockCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,UNLOCKCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,UNLOCKCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,UNLOCKCUR_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,UNLOCKCUR_SELECT_ID),1);
	
	return 0;
}
static int unlockCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockCur Close Win!!!\n");
	return 0;
}
static int unlockCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor unlockCurMsgDeal[]=
{
	{SYS_OPEN_WINDOW,unlockCurOpenWin},
	{SYS_CLOSE_WINDOW,unlockCurCloseWin},
	{SYS_CHILE_COLSE,unlockCurWinChildClose},
	{SYS_TOUCH_WINDOW,unlockCurTouchWin},
	{KEY_EVENT_OK,unlockCurKeyMsgOk},
	{KEY_EVENT_LEFT,unlockCurKeyMsgUp},
	{KEY_EVENT_RIGHT,unlockCurKeyMsgDown},
	{KEY_EVENT_SETTING,unlockCurKeyMsgMenu},
	{KEY_EVENT_MODE,unlockCurKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(unlockCurWindow,unlockCurMsgDeal,unlockCurWin)


