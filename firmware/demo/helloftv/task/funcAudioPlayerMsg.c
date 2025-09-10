#include "funcAudioPlayerRes.c"

#include "userMedia.h"
res_img_t audioplayer_bg_img;
static uint32 lastPlayIdx = -1;


void CommUpdateBaterryIcon(winHandle hDialog, uint16 idItem)
{
	static u8 bat_charge_idx=0;
	
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(getChargeStatus()==0)
		{
			//3?��??D��?1??����?3?��?����??����
			bat_charge_idx++;
			if(bat_charge_idx>3)
			{
				bat_charge_idx=0;
			}
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY1+bat_charge_idx);
		}
		else
		{
			//??��??��??
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY4);
		}
	}
	else
	{
		//USB?��2?��?��???��?����?����?��?
		
		if(SysCtrl.battery == BATTERY_STAT_4)
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY4);
		else if(SysCtrl.battery == BATTERY_STAT_3)
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY3);
		else if(SysCtrl.battery == BATTERY_STAT_2)
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY2);
		else if(SysCtrl.battery == BATTERY_STAT_1)
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY1);
		else if(SysCtrl.battery == BATTERY_STAT_0)
			winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY0);
		else
		    winSetResid(winItem(hDialog,idItem),R_ID_ICON_MTBATTERY4); 
	}
	winSetVisible(winItem(hDialog,idItem),true);


}

static int audioPlayerKeyMsgList(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}

static int audioPlayerKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	//static u8 ListEnFlag = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//MP3_stop();
		//MP3_play();
		if (lastPlayIdx != MP3_get_item_idx())
		{
			MP3_play();
			lastPlayIdx = MP3_get_item_idx();
			printf("111111\n");
		}
		else if(MP3_is_playing()==1)
		{
			MP3_pause();
			printf("2222222\n");
			osal_task_sleep(500);
		}
		else if(MP3_is_playing()==0)
		{
			while(dacIsPlaying()){osal_task_sleep(10);}
			if (MP3_get_totaltime())
			{
				MP3_resume();
				printf("33333333\n");
			}
			else
			{
				MP3_play();
				printf("444444444\n");
			}
		}

		audioplayer_update_playstatus(handle);
	}
	return 0;
}


static int audioPlayerKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	static u8 key_continue=0;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED/*KEY_RELEASE*/)
	{
		//debug("KEY_RELEASE");
		//if(!key_continue)
		//if(MP3_is_playing()==0)
		{
			MP3_stop();
			osal_task_sleep(100);
			MP3_move_item(OP_PREV);
			MP3_play();
			//MP3_move_item();
			lastPlayIdx = MP3_get_item_idx();
			audioplayer_update_playlist(handle);
		}
		key_continue=0;
	}
	else if(keyState==KEY_CONTINUE)
	{
		//debug("KEY_CONTINUE");
		key_continue=1;

		//MP3_volume_add();
		//audioplayer_update_volume(handle);

	}
	return 0;
}

static int audioPlayerKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	static u8 key_continue=0;
	if(parameNum==1)
		keyState=parame[0];
	
	if(keyState==KEY_PRESSED/*KEY_RELEASE*/)
	{
		//debug("KEY_RELEASE");
		//if(!key_continue)
		//if(MP3_is_playing()==0)
		{
			MP3_stop();
			osal_task_sleep(100);
			MP3_move_item(OP_NEXT);
			MP3_play();
			lastPlayIdx = MP3_get_item_idx();
			audioplayer_update_playlist(handle);
		}
		key_continue=0;

	}
	else if(keyState==KEY_CONTINUE)
	{
		//debug("KEY_CONTINUE");
		key_continue=1;

		//MP3_volume_dec();

		//audioplayer_update_volume(handle);
	}

	return 0;
}

static int audioPlayerKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	//	functionStart(FUNCTION_GAME_MENU,0);
//		functionStart(FUNCTION_MAIN_MENU,0);
	}
	return 0;
}
static int audioPlayerKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		MP3_volume_dec();
		audioplayer_update_volume(handle);
	}
	return 0;
}
static int audioPlayerKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		MP3_volume_add();
		audioplayer_update_volume(handle);
	}
	return 0;
}
static int audioPlayerSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioplayer_update_sdcard_icon(handle);

	if((SDC_STAT_NULL==SysCtrl.sdcard)||(SDC_STAT_ERROR==SysCtrl.sdcard))
	{
		MP3_stop();
	}
	else if(SDC_STAT_NORMAL==SysCtrl.sdcard)
	{
		MP3_filelist_reload("1:/MP3");
	}

	audioplayer_update_playstatus(handle);			
	audioplayer_update_playtime(handle);
	audioplayer_update_playlist(handle);

	//CommUpdateBaterryIcon(handle, AUDIOPLAYER_BATERRY_ID);
	audioPlayerBaterryShow(handle);
	return 0;
}


static int audioPlayerOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audioPlayer Open Win\n");

	img_load(&audioplayer_bg_img, RES_AUDIO_BK, 1, 320, 240, 0);
	img_direct_show(&audioplayer_bg_img, 0, 0, audioplayer_bg_img.width, audioplayer_bg_img.height);

	audioPlayerProgressBar = progressBarCreate(Rx(150), Ry(186-16), Rw(310-150), Rh(6+4), INVALID_HANDLE, WIN_ABS_POS, 0);
	winSetfgColor(audioPlayerProgressBar, R_COLOR_WHITE);
	winSetbgColor(audioPlayerProgressBar, R_COLOR_GRAY3);

	MP3_init();
	
	MP3_filelist_reload("1:/MP3");
	lastPlayIdx = -1;

	audioplayer_update_playlist(handle);
	audioplayer_update_icon(handle);

	return 0;
}

static int audioPlayerCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("audioPlayer Close Win!!!\n");

	MP3_uninit();
	lastPlayIdx = -1;
	img_release(&audioplayer_bg_img);
	//audioplayer_hide_icon(handle);
	//drawUIService(true);

	return 0;
}
static int audioPlayerMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("%s...\n",__func__);
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		MP3_stop();
		audioplayer_hide_icon(handle);
		drawUIService(true);
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}

static int audioPlayerSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioPlayerBaterryShow(handle);//CommUpdateBaterryIcon(handle, AUDIOPLAYER_BATERRY_ID);
	return 0;
}

static int audioPlayerSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioPlayerBaterryShow(handle);//CommUpdateBaterryIcon(handle, AUDIOPLAYER_BATERRY_ID);
	return 0;
}

static int audioPlayerSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioplayer_update_playtime(handle);
	audioplayer_update_playstatus(handle);

	return 0;
}

static int audioPlayerSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static int count=0;
	static int count2=0;
	
	audioPlayerBaterryShow(handle);//CommUpdateBaterryIcon(handle, AUDIOPLAYER_BATERRY_ID);
#if 0
	if ((MP3_get_totaltime()>0) && (MP3_get_totaltime()< MP3_get_playtime()+10))
	{
		count++;
	}

	if ((count>5) && (MP3_get_totaltime()==0)&&(MP3_is_playing()==0))
	{
		count2++;
		if (count2>1)
		{
			printf("===play next \n");
			MP3_move_item(OP_NEXT);
			audioplayer_update_playlist(handle);
			MP3_play();
			lastPlayIdx = MP3_get_item_idx();
			count = 0;
			count2 = 0;
		}
	}
#endif	
	return 0;
}

static int audioPlayerSysMsgPlayFinish(winHandle handle,uint32 parameNum,uint32* parame)
{
	u8 ret=0;
	if(parameNum==1)
		ret=parame[0];
	
	printf("audioPlayerSysMsgPlayFinish:%d,%d\n",parameNum,ret);
	if(ret)
	{
		mp3_play_next();
		audioplayer_update_icon(handle);
		
	}
	return 0;
}


msgDealInfor audioPlayerMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioPlayerOpenWin},
	{SYS_CLOSE_WINDOW,audioPlayerCloseWin},
	{KEY_EVENT_POWER,audioPlayerMsgPower},
	{KEY_EVENT_MODE,audioPlayerMsgPower},
	{KEY_EVENT_LEFT,audioPlayerKeyMsgUp},
	{KEY_EVENT_RIGHT,audioPlayerKeyMsgDown},
	
	{KEY_EVENT_OK, audioPlayerKeyMsgOk},
	
	{KEY_EVENT_UP,audioPlayerKeyMsgMode},
	{ KEY_EVENT_DOWN,audioPlayerKeyMsgMenu},
	
	{SYS_EVENT_500MS,audioPlayerSysMsg500MS},
	{SYS_EVENT_1S,audioPlayerSysMsg1S},
	{SYS_EVENT_SDC,audioPlayerSysMsgSD},
	{SYS_EVENT_USB,audioPlayerSysMsgUSB},
	{SYS_EVENT_BAT,audioPlayerSysMsgBattery},
//	{MP3_EVENT_PLAY_FINISH,audioPlayerSysMsgPlayFinish},
	
	{EVENT_MAX,NULL},
};

WINDOW(audioPlayerWindow,audioPlayerMsgDeal,audioPlayerWin)


