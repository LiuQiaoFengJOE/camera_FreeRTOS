#include "uiMenuDelAllRes.c"

static uint32 getdelAllResInfor(uint32 item,uint32* image,uint32* str)
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

static int delAllKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	int i,cnt;
	char *name;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELALL_SELECT_ID));
		if(item==0)
		{
			if(!curFloderHasFile())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return 0;
			}
			uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_COM_WAITING,5);			
			delAllVidJpgFile();
			fileScanCurFolder();
			if(curFloderHasFile())
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);
			uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
			//uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int delAllKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELALL_SELECT_ID));
	}
	return 0;
}
static int delAllKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELALL_SELECT_ID));
	}
	return 0;
}
static int delAllKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delAllKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delAllTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("delAllWin, parame num error %d\n",parameNum);
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
static int delAllOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delAll Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELALL_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,DELALL_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,DELALL_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,DELALL_SELECT_ID),itemCreatemenuOption,getdelAllResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELALL_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELALL_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELALL_SELECT_ID),R_COLOR_GRAY);
	itemManageSetCurItem(winItem(handle,DELALL_SELECT_ID),1);
	return 0;
}
static int delAllCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delAll Close Win!!!\n");
	return 0;
}
static int delAllWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delAll WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delAllMsgDeal[]=
{
	{SYS_OPEN_WINDOW,delAllOpenWin},
	{SYS_CLOSE_WINDOW,delAllCloseWin},
	{SYS_CHILE_COLSE,delAllWinChildClose},
	{SYS_TOUCH_WINDOW,delAllTouchWin},
	{KEY_EVENT_OK,delAllKeyMsgOk},
	{KEY_EVENT_LEFT,delAllKeyMsgUp},
	{KEY_EVENT_RIGHT,delAllKeyMsgDown},
	{KEY_EVENT_SETTING,delAllKeyMsgMenu},
	{KEY_EVENT_MODE,delAllKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(delAllWindow,delAllMsgDeal,delAllWin)


