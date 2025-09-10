#include"uiWin.h"

enum
{
	UPGRADE_STRING_ID=0,
	UPGRADE_BAR_ID,
};
static widgetCreateInfor upgradeWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(UPGRADE_STRING_ID,Rx(50),Ry(125),Rw(220),Rh(35),"upgrade,do not power off",ALIGNMENT_CENTER, R_COLOR_BLUE,0),
	createProgressBar(UPGRADE_BAR_ID,Rx(70),Ry(117),Rw(180),Rh(6),R_COLOR_GRAY3,R_COLOR_BLUE),
	widgetEnd(),
};



