#include "uiUpgradeRes.c"


static int upgradeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int upgradeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int upgradeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int upgradeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int upgradeKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int upgradeSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int upgradeSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int upgradeSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int upgradeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("upgrade Open Win!!!\n");
	return 0;
}
static int upgradeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("upgrade Close Win!!!\n");
	return 0;
}
static int upgradeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("upgrade WinChild Close!!!\n");
	return 0;
}
static int upgradeSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int upgradeMsgUpdateTime(winHandle handle,uint32 parameNum,uint32* parame)
{
	winSetPorgressRate(winItem(handle,UPGRADE_BAR_ID),parame[0]);
	return 0;
}
void upgradeUpdatePercent(u32 percent)
{
	sendMsgNonblock(OS_TASK_UI,SYS_TIME_UPDATE,percent);
}
void upgradeWinOpen(void)
{
	uiOpenWindow(&upgradeWindow,0);
}
msgDealInfor upgradeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,upgradeOpenWin},
	{SYS_CLOSE_WINDOW,upgradeCloseWin},
	{SYS_CHILE_COLSE,upgradeWinChildClose},
	{KEY_EVENT_OK,upgradeKeyMsgOk},
	{KEY_EVENT_UP,upgradeKeyMsgUp},
	{KEY_EVENT_DOWN,upgradeKeyMsgDown},
	{KEY_EVENT_MENU,upgradeKeyMsgMenu},
	{KEY_EVENT_MODE,upgradeKeyMsgMode},
	{SYS_EVENT_SDC,upgradeSysMsgSD},
	{SYS_EVENT_USB,upgradeSysMsgUSB},
	{SYS_EVENT_BAT,upgradeSysMsgBattery},
	{SYS_EVENT_1S,upgradeSysMsg1S},
	{KEY_EVENT_POWER,upgradeKeyMsgMode},
	{SYS_TIME_UPDATE,upgradeMsgUpdateTime},
	{EVENT_MAX,NULL},
};

WINDOW(upgradeWindow,upgradeMsgDeal,upgradeWin)



