#include"uiWin.h"
enum
{
	NO_FILE_TIP=0,
	NO_FILE_EXE_MAIN_ID,
};
static widgetCreateInfor noFileWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_GRAY3,WIN_ABS_POS),
	createStringIcon(NO_FILE_TIP, Rx(0),Ry(105),Rw(320),Rh(30),R_ID_STR_FIL_NULL,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
#if TP_FUCTION
	createImageIcon(NO_FILE_EXE_MAIN_ID,  Rx(270), Ry(200), Rw(48), Rh(48), R_ID_ICON_MENUBACK,ALIGNMENT_CENTER),
#endif
	widgetEnd(),
};



