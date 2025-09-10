#include "uiNoFileRes.c"

static int noFileSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(hasFile())
	{
		sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);
		winDestroy(&handle);
	}
	return 0;
}
static int noFileTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/

	if(parameNum!=3)
	{
		printf("videoRecordTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==NO_FILE_EXE_MAIN_ID)
		{
			printf("==back to mainmenu\n");

			functionStart(FUNCTION_MAINMENU3,0);
		}
	}
	return 0;
}
static int noFileOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("noFile open Win!!!\n");
	if(parameNum==1)
		winSetResid(winItem(handle,NO_FILE_TIP),parame[0]);
	return 0;
}
static int noFileCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("noFile Close Win!!!\n");
	return 0;
}
static int noFileWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("noFile WinChild Close!!!\n");
	return 0;
}
extern menu MENU(movie);
static int noFileKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionChange();
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));
	}
	return 0;
}

static int noFileKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
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

msgDealInfor noFileMsgDeal[]=
{
	{KEY_EVENT_MODE,noFileKeyMsgMode},
	{SYS_OPEN_WINDOW,noFileOpenWin},
	{SYS_CLOSE_WINDOW,noFileCloseWin},
	{SYS_TOUCH_WINDOW,noFileTouchWin},
	{SYS_CHILE_COLSE,noFileWinChildClose},
	{KEY_EVENT_MODE,noFileKeyMsgPower},
	{SYS_EVENT_SDC,noFileSysMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(noFileWindow,noFileMsgDeal,noFileWin)



