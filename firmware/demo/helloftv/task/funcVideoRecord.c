#include "app.h"
#include"uiWin.h"



void funcVideoRecordeOpen(uint32 arg1)
{
	calFreeSpace();
	wmm_trans_mode(&workmode, WM_NONE);
	wmm_trans_mode(&workmode, WM_RECORD);
	lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
	lcdPreviewStart();
	uiOpenWindow(&videoRecordWindow,0);
}


void funcVideoRecordeClose(uint32 arg)
{
	lcdPreviewStop();
	sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_UNINIT,0); 
}
sysFunction functionVideoRecorde=
{
	"Video Recorde",
	NULL,
	funcVideoRecordeOpen,
	funcVideoRecordeClose,
};


