#include "uiMenuFormatRes.c"
#include "fatfs/ff.h"
static uint32 getformatResInfor(uint32 item,uint32* image,uint32* str)
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
int formatSD(void)
{
	INT8U *mem;
	INT8U res;
	u32 cluster;
	if(SysCtrl.sdcard==SDC_STAT_NULL)
	{
		uiOpenWindow(&tipsWindow,2,TIPS_SD_NOT_INSERT,2);
		return 1;
	}
	mem = (INT8U *)osal_malloc(FF_MAX_SS);
	if(mem == NULL)
	{
		printf("malloc mem for format fail.\n");
		uiOpenWindow(&tipsWindow,2,"memory malloc failed",2);
		return 1;
	}
	uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_COM_WAITING,5);
	res = f_mount(NULL, _DATDSK_, 1);
	cluster=64*512;
	printf("function:[%s],line:%d,cluster:%dKB\n",__FUNCTION__,__LINE__,cluster>>10);
	res =f_mkfs(_DATDSK_,FM_FAT32|FM_SFD, cluster,(void *)mem,FF_MAX_SS);
	osal_free(mem);
	if(res==FR_OK)
	{
		remount_sd(NULL);
		fileScan();
		uiOpenWindow(&tipsWindow,2,R_ID_STR_FMT_SUCCESS,2);
		return 0;
	}
	else
	{
		printf ("error : %d\n",res);
		uiOpenWindow(&tipsWindow,2,R_ID_STR_FMT_FAIL,2);
		return 1;
	}
}
static int formatKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,FORMAT_SELECT_ID));
		if(item==0)
		{
			
			formatSD();
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int formatKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,FORMAT_SELECT_ID));
	}
	return 0;
}
static int formatKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,FORMAT_SELECT_ID));
	}
	return 0;
}
static int formatKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int formatKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int formatTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("formatWin, parame num error %d\n",parameNum);
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
static int formatOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("format Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,FORMAT_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,FORMAT_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,FORMAT_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif
	itemManageCreateItem(winItem(handle,FORMAT_SELECT_ID),itemCreatemenuOption,getformatResInfor,2);
	itemManageSetCharInfor(winItem(handle,FORMAT_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,FORMAT_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,FORMAT_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,FORMAT_SELECT_ID),1);
	
	return 0;
}
static int formatCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("format Close Win!!!\n");
	return 0;
}
static int formatWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("format WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor formatMsgDeal[]=
{
	{SYS_OPEN_WINDOW,formatOpenWin},
	{SYS_CLOSE_WINDOW,formatCloseWin},
	{SYS_CHILE_COLSE,formatWinChildClose},
	{SYS_TOUCH_WINDOW,formatTouchWin},
	{KEY_EVENT_OK,formatKeyMsgOk},
	{KEY_EVENT_UP,formatKeyMsgUp},
	{KEY_EVENT_DOWN,formatKeyMsgDown},
	{KEY_EVENT_LEFT,formatKeyMsgUp/*formatKeyMsgMenu*/},
	{KEY_EVENT_RIGHT,formatKeyMsgDown/*formatKeyMsgMode*/},
	{EVENT_MAX,NULL},
};

WINDOW(formatWindow,formatMsgDeal,formatWin)


