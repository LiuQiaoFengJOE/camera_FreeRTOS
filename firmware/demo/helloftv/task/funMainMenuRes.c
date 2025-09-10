#include"uiWin.h"

enum
{
	MAINMENU_PHOTO_ID=0,
	MAINMENU_VIDEO_ID,
	MAINMENU_MUSIC_ID,
	MAINMENU_PLAYBACK_ID,
	MAINMENU_GAME_ID,
	MAINMENU_SETTING_ID,
	MAINMENU_BATERRY_ID,
	MAIN_ID1,
	MAIN_ID2,
	MAIN_ID3,
	MAIN_ID4,
	MAIN_ID5,
	MAIN_ID6,
	MAINMENU_MAX_ID
};

static widgetCreateInfor mainMenuWin[] =
{
/*
{12,18},	
{12+100,18},
{12+100+100,18},
{12,18+108},
{12+100,18+108},
{12+100+100,18+108},

*/
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	createRect(MAIN_ID1,       Rx(12),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(MAIN_ID2,       Rx(12+100),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(MAIN_ID3,       Rx(12+100+100),Ry(18), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(MAIN_ID4,       Rx(12),Ry(18+108), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(MAIN_ID5,       Rx(12+100),Ry(18+108), Rw(70),Rh(70),R_COLOR_TRANSFER),
	createRect(MAIN_ID6,       Rx(12+100+100),Ry(18+108), Rw(70),Rh(70),R_COLOR_TRANSFER),
	
	createImageIcon(MAINMENU_BATERRY_ID,Rx(285), Ry(2), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

	widgetEnd(),
};

static void mainMenuStrShow(winHandle handle,bool show)
{
}

static void mainMenuBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,MAINMENU_BATERRY_ID),true);
}



