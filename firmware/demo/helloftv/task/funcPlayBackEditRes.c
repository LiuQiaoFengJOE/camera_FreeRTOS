#include"uiWin.h"
#include "mediaFile/demuxer.h"

enum
{
	PLAYBACK_MODE_ID=0,
	PLAYBACK_BK_ID,
	PLAYBACK_HIDE_RECT_ID,
	PLAYBACK_IRLED_ID,
	PLAYBACK_SD_ID,
	PLAYBACK_BATERRY_ID,
	PLAYBACK_POWERON_TIME_ID,
	PLAYBACK_FILE_NAME_ID,
	PLAYBACK_FILE_INDEX_ID,
   	PLAYBACK_EXE_PRE_ID,
	PLAYBACK_EXE_NEXT_ID,
	PLAYBACK_EXE_SET_ID,
	PLAYBACK_EXE_MAIN_ID,	

	PLAYBACKEDIT_LEFT_ID,
	PLAYBACKEDIT_RIGHT_ID,
	PLAYBACKEDIT_UP_ID,
	PLAYBACKEDIT_DOWN_ID,
	
	PLAYBACKEDIT_FRAME0_ID,
	PLAYBACKEDIT_FRAME1_ID,
	PLAYBACKEDIT_EFFECT_ID,
	PLAYBACKEDIT_CANCEL_ID,
	PLAYBACKEDIT_SAVE_ID,
	
	PLAYBACK_MAX_ID
};
static widgetCreateInfor playBackEditWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createRect(PLAYBACK_BK_ID,              Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK),
	createRect(PLAYBACK_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(PLAYBACK_MODE_ID,           Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTPLAY2,ALIGNMENT_LEFT),

	createImageIcon(PLAYBACK_SD_ID,                  Rx(258), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_BATERRY_ID,       Rx(300), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(5), Ry(25), Rw(160), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(170), Ry(25), Rw(120), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
#if TP_FUCTION
	createImageIcon(PLAYBACK_EXE_PRE_ID,    Rx(150), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTBACK1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_NEXT_ID,   Rx(190), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTFORWARD1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_SET_ID,    Rx(230), Ry(200), Rw(48), Rh(48), R_ID_ICON_MTMENU1,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACK_EXE_MAIN_ID,   Rx(270), Ry(200), Rw(48), Rh(48), R_ID_ICON_MENUBACK,ALIGNMENT_CENTER),
#endif
	createImageIcon(PLAYBACKEDIT_LEFT_ID ,         Rx(0),Ry(104), Rw(32), Rh(32), R_ID_ICON_MTLEFT,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_RIGHT_ID ,        Rx(288),Ry(104), Rw(32), Rh(32), R_ID_ICON_MTRIGHT,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_UP_ID ,         Rx(140),Ry(0), Rw(32), Rh(32), R_ID_ICON_MTU,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_DOWN_ID ,        Rx(140),Ry(168), Rw(32), Rh(32), R_ID_ICON_MTU2,ALIGNMENT_CENTER),


	createImageIcon(PLAYBACKEDIT_FRAME0_ID ,       Rx(50), Ry(50), Rw(40), Rh(40), R_ID_ICON__PHOTOEDIT_FRAME0_UNSEL,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_FRAME1_ID ,       Rx(140),Ry(50), Rw(40), Rh(40), R_ID_ICON__PHOTOEDIT_FRAME1_UNSEL,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_EFFECT_ID ,       Rx(230),Ry(50), Rw(40), Rh(40), R_ID_ICON__PHOTOEDIT_FRAME2_UNSEL,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_CANCEL_ID ,       Rx(80), Ry(110), Rw(40), Rh(40), R_ID_ICON__PHOTOEDIT_FRAME3_UNSEL,ALIGNMENT_CENTER),
	createImageIcon(PLAYBACKEDIT_SAVE_ID ,         Rx(180),Ry(110), Rw(40), Rh(40), R_ID_ICON__PHOTOEDIT_FRAME4_UNSEL,ALIGNMENT_CENTER),

	widgetEnd(),
};

static void playbackBKShow(winHandle handle,u8 en)
{
	if(en)
		winSetVisible(winItem(handle,PLAYBACK_BK_ID),true);
	else
		winSetVisible(winItem(handle,PLAYBACK_BK_ID),false);
}

static void playbackIrLedShow(winHandle handle)
{
	winSetVisible(winItem(handle,PLAYBACK_IRLED_ID),false);
}

static void playbackSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
}


static void playbackBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}

static void playbackFileNameShow(winHandle handle)
{
	int idx;
	static char indexStr[10];
	static char fileNameStr[15];
	getCurFileName(fileNameStr,15);
	winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(fileNameStr));
}


