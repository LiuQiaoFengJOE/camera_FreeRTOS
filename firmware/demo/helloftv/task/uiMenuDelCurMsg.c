#include "uiMenuDelCurRes.c"

static uint32 getdelCurResInfor(uint32 item,uint32* image,uint32* str)
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

static int delCurKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELCUR_SELECT_ID));
		if(item==0)
		{
			if(!curFloderHasFile())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return 0;
			}
			if(curFileIsLock())
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_SET_LOCKED,2);
				return 0;
			}
			if(delCurFile()==0)
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_NEXT,0);
				saveCurFile();
				fileScanCurFolder();
				restoreCurFile();
			}
			else
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int delCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delCurTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("delCurWin, parame num error %d\n",parameNum);
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
static int delCurOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELCUR_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,DELCUR_SELECT_ID),itemCreatemenuOption,getdelCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_GRAY);
	itemManageSetCurItem(winItem(handle,DELCUR_SELECT_ID),1);
	return 0;
}
static int delCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delCur Close Win!!!\n");
	return 0;
}
static int delCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("delCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delCurMsgDeal[]=
{
	{SYS_OPEN_WINDOW,delCurOpenWin},
	{SYS_CLOSE_WINDOW,delCurCloseWin},
	{SYS_CHILE_COLSE,delCurWinChildClose},
	{SYS_TOUCH_WINDOW,delCurTouchWin},
	{KEY_EVENT_OK,delCurKeyMsgOk},
	{KEY_EVENT_LEFT,delCurKeyMsgUp},
	{KEY_EVENT_RIGHT,delCurKeyMsgDown},
	{KEY_EVENT_SETTING,delCurKeyMsgMenu},
	{KEY_EVENT_MODE,delCurKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(delCurWindow,delCurMsgDeal,delCurWin)


