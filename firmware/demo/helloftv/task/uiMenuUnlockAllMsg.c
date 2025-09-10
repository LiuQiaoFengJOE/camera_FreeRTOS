#include "uiMenuUnlockAllRes.c"

static uint32 getunlockAllResInfor(uint32 item,uint32* image,uint32* str)
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

static int unlockAllKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	int i,cnt;
	char *name;
	char *srcName;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,UNLOCKALL_SELECT_ID));
		if(item==0)
		{
			if(!curFloderHasFile())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return 0;
			}
			uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_COM_WAITING,5);	
			unlockCurFolderFile();
			sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_NEXT,0);
			sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);
			uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int unlockAllKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,UNLOCKALL_SELECT_ID));
	}
	return 0;
}
static int unlockAllKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,UNLOCKALL_SELECT_ID));
	}
	return 0;
}
static int unlockAllKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int unlockAllKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int unlockAllTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("unlockAllWin, parame num error %d\n",parameNum);
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
static int unlockAllOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockAll Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,UNLOCKALL_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKALL_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKALL_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,UNLOCKALL_SELECT_ID),itemCreatemenuOption,getunlockAllResInfor,2);
	itemManageSetCharInfor(winItem(handle,UNLOCKALL_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,UNLOCKALL_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,UNLOCKALL_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,UNLOCKALL_SELECT_ID),1);
	
	return 0;
}
static int unlockAllCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockAll Close Win!!!\n");
	return 0;
}
static int unlockAllWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("unlockAll WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor unlockAllMsgDeal[]=
{
	{SYS_OPEN_WINDOW,unlockAllOpenWin},
	{SYS_CLOSE_WINDOW,unlockAllCloseWin},
	{SYS_CHILE_COLSE,unlockAllWinChildClose},
	{SYS_TOUCH_WINDOW,unlockAllTouchWin},
	{KEY_EVENT_OK,unlockAllKeyMsgOk},
	{KEY_EVENT_LEFT,unlockAllKeyMsgUp},
	{KEY_EVENT_RIGHT,unlockAllKeyMsgDown},
	{KEY_EVENT_SETTING,unlockAllKeyMsgMenu},
	{KEY_EVENT_MODE,unlockAllKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(unlockAllWindow,unlockAllMsgDeal,unlockAllWin)


