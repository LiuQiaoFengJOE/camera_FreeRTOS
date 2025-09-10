#include "task/uiWin.h"
//#if UI_STYLE==UI_STYLE_PRINTCAM

void funcAudioPlayerOpen(uint32 arg)
{
	printf("AudioPlayer open.\n");

#if 0
	s32 ret=mp3_dac_memInit();
	if(0!=ret)
	{
		error("mp3 malloc err\n");
	}
#endif


//	wmm_trans_mode(&workmode, WM_PLAYBACK);
//	fileInforInit(MP3_FILE);
//	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE,0);

	uiOpenWindow(&audioPlayerWindow,0);
}

void funcAudioPlayerClose(uint32 arg)
{
	printf("AudioPlayer exit.\n");

//	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_UNINIT,0); 
//	wmm_trans_mode(&workmode, WM_NONE);

#if 0
	mp3_dac_memUinit();
#endif
}

sysFunction functionAudioPlayer=
{
	"Audio Player",
	NULL,
	funcAudioPlayerOpen,
	funcAudioPlayerClose,
};

//#endif
