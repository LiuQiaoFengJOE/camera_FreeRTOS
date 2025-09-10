#include "touchPanel.h"

#include "uiMenuItemRes.c"

#define currentPage()    (currentMenu->pPage[currentMenu->curPage])
#define getItem(n)   (currentPage().pItem[n])
static menu* currentMenu=NULL;
static bool menuOpened=false;

bool menuIsOpen(void)
{
	return menuOpened;
}

static uint32 getItemResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=getItem(item).image;
	if(str)
		*str=getItem(item).str;
}

static uint32 getItemResInforEx(uint32 item,uint32* image,uint32* str,uint32* selectImage,uint32* selectStr)
{
	uint32 config;
	menuItem* pItem=&getItem(item);
	if(image)
		*image=pItem->image;
	if(str)
		*str=pItem->str;
	if(pItem->configId==0&&pItem->optionSum==0)
	{
		if(selectImage)
			*selectImage=INVALID_RES_ID;
		if(selectStr)
			*selectStr=INVALID_RES_ID;
	}
	else if(pItem->configId==0)
	{
		if(selectImage)
			*selectImage=INVALID_RES_ID;
		if(selectStr)
			*selectStr=pItem->pOption[0].str;
	}
	else
	{
		config=configGet(pItem->configId);
		if(pItem->optionSum==2&&(config==R_ID_STR_COM_OFF||config==R_ID_STR_COM_ON))
		{
			if(selectImage)
			{
				if(config==R_ID_STR_COM_OFF)
					*selectImage=R_ID_ICON_MTOFF;
				else
					*selectImage=R_ID_ICON_MTON;
			}
			if(selectStr)
				*selectStr=INVALID_RES_ID;
		}
		else
		{
			if(selectImage)
				*selectImage=R_ID_ICON_MTMORE;
			if(selectStr)
				*selectStr=config;
		}
	}
	
}

static int menuItemKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuItem* pItem;
	uint32 config;	
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		pItem=&getItem(itemManageGetCurrentItem(winItem(handle,ITEM_SELECT_ID)));
		if(pItem->optionSum==0)
			((menuItemProc)pItem->pOption)(handle,parameNum,parame);
		else if(pItem->optionSum>1)
		{
			#if SMALL_PANEL_SUPPORT==0// only 2 option and one of them is "OFF" and another is "ON"
			config=configGet(pItem->configId);
			if(pItem->optionSum==2&&(config==R_ID_STR_COM_OFF||config==R_ID_STR_COM_ON))
			{
				if(config==R_ID_STR_COM_OFF)
					configSet(pItem->configId,R_ID_STR_COM_ON);
				else
					configSet(pItem->configId,R_ID_STR_COM_OFF);
				configSys(pItem->configId);
				
				menuMDShow(handle);
				menuMonitorShow(handle);
				menuMicShow(handle);
				itemManageUpdateCurItem(winItem(handle,ITEM_SELECT_ID));
			}
			else
			#endif
				uiOpenWindow(&menuOptionWindow,1,pItem);
		}
	}
	return 0;
}
static int menuItemKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
	}
	return 0;
}
static int menuItemKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,ITEM_SELECT_ID));
	}
	return 0;
}
static int menuItemKeyMsgSetting(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		currentMenu->curPage++;
		if(currentMenu->curPage<currentMenu->pageSum)
		{
			winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
			itemManageUpdateRes(winItem(handle,ITEM_SELECT_ID),currentPage().itemSum,0);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int menuItemKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;  
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionChange();
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}
static int menuItemKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//functionStart(FUNCTION_MAINMENU3,0);
		winDestroy(&handle);
	}
	return 0;
}
static int menuItemSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuSDShow(handle);
	return 0;
}
static int menuSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuBaterryShow(handle);
	return 0;
}
static int menuSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		menuBaterryShow(handle);
	return 0;
}
static int menuItemTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=3)
	{
		printf("menuItemWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0] == ITEM_MODE_ID){
			winDestroy(&handle);
		}
		else
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, KEY_PRESSED);
		}
	}
	return 0;
}
static int menuItemSlidRelease(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuItemOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	resInfor infor;
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	printf("menuItem Open Win!!!\n");
	currentMenu=(menu*)parame[0];
	currentMenu->curPage=0;
	itemManageSetItemHeight(winItem(handle,ITEM_SELECT_ID),R1h(35));
	#if SMALL_PANEL_SUPPORT>0
	itemManageCreateItem(winItem(handle,ITEM_SELECT_ID),itemCreateMenuItem,NULL,currentPage().itemSum);// no sub menu preview 
	#else
	itemManageCreateItem(winItem(handle,ITEM_SELECT_ID),itemCreateMenuItemEx,NULL,currentPage().itemSum);
	#endif
	itemManageSetResInforFuncEx(winItem(handle,ITEM_SELECT_ID),getItemResInforEx);
#if 1	
	itemManageSetCharInfor(winItem(handle,ITEM_SELECT_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,ITEM_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,ITEM_SELECT_ID),R_COLOR_GRAY);
#else
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_LEFT;
	infor.fontColor=R_COLOR_WHITE;
	infor.color=R_COLOR_BLUE1;
	winSetSelectInfor(winItem(handle,ITEM_SELECT_ID),&infor);
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_LEFT;
	infor.fontColor=R_COLOR_WHITE;
	infor.color=R_COLOR_GRAY;
	winSetUnselectInfor(winItem(handle,ITEM_SELECT_ID),&infor);
#endif
	itemManageNextItem(winItem(handle,ITEM_SELECT_ID));

	winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
	menuResolutionShow(handle);
	menuMDShow(handle);
	menuMonitorShow(handle);
	menuIrLedShow(handle);
	menuSDShow(handle);
	menuMicShow(handle);
	menuBaterryShow(handle);
	#if SMALL_PANEL_SUPPORT==0
	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	menuOpened=true;
	return 0;
}
static int menuItemCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("menuItem Close Win!!!\n");
	userConfigSave();
	currentMenu=NULL;
	menuOpened=false;
	return 0;
}
static int menuItemWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("menuItem WinChild Close!!!\n");
	winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
	menuResolutionShow(handle);
	menuMDShow(handle);
	menuMonitorShow(handle);
	menuIrLedShow(handle);
	menuSDShow(handle);
	menuMicShow(handle);
	menuBaterryShow(handle);
	//itemManageUpdateCurItem(winItem(handle,ITEM_SELECT_ID));
	itemManageUpdateAllitem(winItem(handle,ITEM_SELECT_ID));
	#if SMALL_PANEL_SUPPORT==0
	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	return 0;
}
static int menuItemSysMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		if(winIsVisible(winItem(handle,ITEM_BATERRY_ID)))
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),false);
		else
		{
			winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),true);
		}	
	}
	#if SMALL_PANEL_SUPPORT==0
	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
	#endif
	return 0;
}

msgDealInfor menuItemMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuItemOpenWin},
	{SYS_CLOSE_WINDOW,menuItemCloseWin},
	{SYS_CHILE_COLSE,menuItemWinChildClose},
	{SYS_TOUCH_WINDOW,menuItemTouchWin},
	{SYS_SLID_RELEASE,menuItemSlidRelease},
	{KEY_EVENT_OK,menuItemKeyMsgOk},
	{KEY_EVENT_UP,menuItemKeyMsgUp},
	{KEY_EVENT_DOWN,menuItemKeyMsgDown},
	{KEY_EVENT_SETTING,menuItemKeyMsgSetting},
	{KEY_EVENT_MODE,menuItemKeyMsgMode},
	{KEY_EVENT_POWER,menuItemKeyMsgPower},
	{SYS_EVENT_SDC,menuItemSysMsgSD},
	{SYS_EVENT_USB,menuSysMsgUSB},
	{SYS_EVENT_BAT,menuSysMsgBattery},
	{SYS_EVENT_1S,menuItemSysMsg1s},
	{EVENT_MAX,NULL},
};

WINDOW(menuItemWindow,menuItemMsgDeal,menuItemWin)


