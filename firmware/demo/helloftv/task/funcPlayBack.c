#include "app.h"
#include"uiWin.h"
#include "mediaFile/demuxer.h"
#include "fatfs/fileOps.h"

void funcPlayBackOpen(uint32 arg1)
{
	wmm_trans_mode(&workmode, WM_PLAYBACK);
	fileInforInit(AVI_FILE);
/*	if(arg1==1)
	{
		sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE_JPG,0);
		uiOpenWindow(&playBacEditkWindow,0);
	}
	else*/
	{
		sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);
		uiOpenWindow(&playbackWindow,0);
	}
}


void funcPlayBackClose(uint32 arg)
{
	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_UNINIT,0); 
}
sysFunction functionPlayBack=
{
	"playBack",
	NULL,
	funcPlayBackOpen,
	funcPlayBackClose,
};


