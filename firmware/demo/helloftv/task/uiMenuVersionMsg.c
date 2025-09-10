#include "uiMenuVersionRes.c"

static uint32 getVersionResInfor(uint32 item,uint32* image,uint32* str)
{
/*
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
*/
	return 0;
}

static int versionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
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

static int versionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int versionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int versionTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("versionWin, parame num error %d\n",parameNum);
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
static int versionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("version Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,VERSION_TIPS_ID),1,R1h(32));
	itemManageSetColumnSumWithGap(winItem(handle,VERSION_TIPS_ID),0,2,R1w(50),R1w(12));

	return 0;
}
static int versionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("version Close Win!!!\n");
	return 0;
}
static int versionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("version WinChild Close!!!\n");
	return 0;
}

   
msgDealInfor verisonMsgDeal[]=
{
	{SYS_OPEN_WINDOW,versionOpenWin},
	{SYS_CLOSE_WINDOW,versionCloseWin},
	{SYS_CHILE_COLSE,versionWinChildClose},
	{SYS_TOUCH_WINDOW,versionTouchWin},
	{KEY_EVENT_OK,versionKeyMsgOk},
	//{KEY_EVENT_MENU,versionKeyMsgMenu},
	{KEY_EVENT_MODE,versionKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(versionWindow,verisonMsgDeal,versionWin)


