#include"uiWin.h"

enum
{
	USBDEV_MODE_ID=0,
	USBDEV_SYSTIME_ID,
	USBDEV_MAX_ID
};
static widgetCreateInfor usbDevWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createStringIcon(USBDEV_SYSTIME_ID, Rx(5), Ry(215), Rw(300), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	widgetEnd(),
};


