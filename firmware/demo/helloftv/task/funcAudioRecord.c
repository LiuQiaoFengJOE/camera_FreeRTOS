#include "app.h"
#include"uiWin.h"

#if SUPPORT_AUD_REC

void funcAudioRecordOpen(uint32 arg1)
{
	//lcd_set_crop_csi_to_lcd(SysCtrl.crop_level);
	//lcdPreviewStart();
	calFreeSpace();
	wmm_trans_mode(&workmode, WM_RECORD);
	uiOpenWindow(&audioRecordWindow,0);
}


void funcAudioRecordClose(uint32 arg)
{
	//lcdPreviewStop();
	sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_UNINIT,0); 
}
sysFunction functionAudioRecord=
{
	"Audio Record",
	NULL,
	funcAudioRecordOpen,
	funcAudioRecordClose,
};

#endif

