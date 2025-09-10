#include "app.h"
#include"uiWin.h"



void funcTakePhotoeOpen(uint32 arg1)
{
	calFreeSpace();
	wmm_trans_mode(&workmode, WM_NONE);
	wmm_trans_mode(&workmode, WM_RECORD);
	lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
	lcdPreviewStart();
//	if (arg1 == 1)
//		uiOpenWindow(&photoscoreWindow,0);
//	else
		uiOpenWindow(&takePhotoWindow,0);
}


void funcTakePhotoeClose(uint32 arg)
{
	lcdPreviewStop();
	sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_UNINIT,0); 
}
sysFunction functionTakePhotoe=
{
	"take photo",
	NULL,
	funcTakePhotoeOpen,
	funcTakePhotoeClose,
};


