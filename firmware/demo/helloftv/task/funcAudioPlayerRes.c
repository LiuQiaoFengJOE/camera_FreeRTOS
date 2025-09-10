#include "task/uiWin.h"

#include "mp3_dec_wrap.h"
#include "mp3_filelist.h"

enum
{
	AUDIOPLAYER_ICON_ID=0,
	AUDIOPLAYER_SD_ID,
	AUDIOPLAYER_BATERRY_ID,

	//==file list==
	AUDIOPLAYER_LIST_BK_ID,
	AUDIOPLAYER_LIST_TIPS_ID,
	AUDIOPLAYER_LIST_ITEM1_ID,
	AUDIOPLAYER_LIST_ITEM2_ID,
	AUDIOPLAYER_LIST_ITEM3_ID,
	AUDIOPLAYER_LIST_ITEM4_ID,
	AUDIOPLAYER_LIST_ITEM5_ID,
	AUDIOPLAYER_LIST_ITEM6_ID,
	AUDIOPLAYER_LIST_STR1_ID,
	AUDIOPLAYER_LIST_STR2_ID,
	AUDIOPLAYER_LIST_STR3_ID,
	AUDIOPLAYER_LIST_STR4_ID,
	AUDIOPLAYER_LIST_STR5_ID,
	AUDIOPLAYER_LIST_STR6_ID,
	AUDIOPLAYER_LIST_FILE_NUM_ID,

	AUDIOPLAYER_VOLUME_NUM_ID,

	AUDIOPLAYER_VOLUME_ICON_ID,
	AUDIOPLAYER_FORWARD_ID,
	AUDIOPLAYER_BACK_ID,
	
	AUDIOPLAYER_STA_ID,
	AUDIOPLAYER_PLAY_TIME_ID,
	AUDIOPLAYER_PLAY_TOTAL_TIME_ID,

	AUDIOPLAYER_FILE_NAME_ID,
	AUDIOPLAYER_MAX_ID
};


//列表单次显示最大数量
#define AUDIOPLAYER_LIST_NUM		6

static u8 audio_player_mp3_num[]="000/000";
static u8 audio_player_mp3_time[]="00:00";
static u8 audio_player_mp3_toatl_time[]="00:00";
static u8 audio_player_mp3_file_name[256]={0};

static u8 audio_player_mp3_Volume[]="0";//"VOL:0";
static winHandle audioPlayerProgressBar;


static widgetCreateInfor audioPlayerWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	
	//createImageIcon(AUDIOPLAYER_ICON_ID,Rx(2), Ry(0), Rw(33), Rh(32), R_ID_ICON_MENUMUSIC,ALIGNMENT_LEFT),
	createImageIcon(AUDIOPLAYER_SD_ID, Rx(150), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(AUDIOPLAYER_BATERRY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

	createStringIcon(AUDIOPLAYER_VOLUME_NUM_ID,Rx(228),Ry(38),Rw(24),Rh(32),strToResId(audio_player_mp3_Volume),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
		
	// 3 8 12 48
	createImageIcon(AUDIOPLAYER_BACK_ID,Rx(168), Ry(140-10), Rw(32), Rh(32), R_ID_ICON_MTBACK,ALIGNMENT_CENTER),
	createImageIcon(AUDIOPLAYER_FORWARD_ID,Rx(264), Ry(140-10), Rw(32), Rh(32), R_ID_ICON_MTFORWARD,ALIGNMENT_CENTER),
	createImageIcon(AUDIOPLAYER_VOLUME_ICON_ID,Rx(200), Ry(40), Rw(32), Rh(32), R_ID_ICON_MENUVOLUME,ALIGNMENT_CENTER),
	//==file list==
/*	createRect(AUDIOPLAYER_LIST_BK_ID,Rx(32),Ry(32),Rw(320-64),Rh(240-64),R_COLOR_BLACK),
	createStringIcon(AUDIOPLAYER_LIST_TIPS_ID,Rx(32),Ry(32),Rw(320-64),Rh(28),strToResId("Music List"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createRect(AUDIOPLAYER_LIST_ITEM1_ID,Rx(32),Ry(62),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM2_ID,Rx(32),Ry(82),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM3_ID,Rx(32),Ry(102),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM4_ID,Rx(32),Ry(122),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM5_ID,Rx(32),Ry(142),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createRect(AUDIOPLAYER_LIST_ITEM6_ID,Rx(32),Ry(162),Rw(320-64),Rh(20),R_COLOR_BLACK),
	createStringIcon(AUDIOPLAYER_LIST_STR1_ID,Rx(32+8),Ry(62),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR2_ID,Rx(32+8),Ry(82),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR3_ID,Rx(32+8),Ry(102),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR4_ID,Rx(32+8),Ry(122),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR5_ID,Rx(32+8),Ry(142),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(AUDIOPLAYER_LIST_STR6_ID,Rx(32+8),Ry(162),Rw(320-64-8),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
*/
	createStringIcon(AUDIOPLAYER_LIST_FILE_NUM_ID,Rx(320-110), Ry(240-37), Rw(120), Rh(32),strToResId(audio_player_mp3_num),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	//==end file list==
	createImageIcon(AUDIOPLAYER_STA_ID,Rx(220-4), Ry(142-12), Rw(32), Rh(32), R_ID_ICON_MENUMUSICPLAY,ALIGNMENT_CENTER),
	createStringIcon(AUDIOPLAYER_PLAY_TIME_ID, Rx(150-6),Ry(240-58),Rw(60),Rh(28),strToResId(audio_player_mp3_time),ALIGNMENT_CENTER, R_COLOR_WHITE,0),//R_COLOR_BLACK
	createStringIcon(AUDIOPLAYER_PLAY_TOTAL_TIME_ID, Rx(320-64),Ry(240-58),Rw(60),Rh(28),strToResId(audio_player_mp3_toatl_time),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	createStringIcon(AUDIOPLAYER_FILE_NAME_ID,Rx(150), Ry(80), Rw(310-150), Rh(28),strToResId(audio_player_mp3_file_name),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
		
	widgetEnd(),
};
static void audioPlayerBaterryShow(winHandle handle)
{

	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(SysCtrl.usb == USB_STAT_DCIN)winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		#if 0
		if( hal_gpioRead(CHECK_FULL_CH,CHECK_FULL_PIN))
		{
			winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
			//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
		}
		else
		{
			winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		}
		#endif
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,AUDIOPLAYER_BATERRY_ID),true);
}

static void audioplayer_update_sdcard_icon(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
}


static void audioplayer_update_volume(winHandle handle)
{
	if(/*MP3_is_playing()*/1)
	{
		s8 audio_Volume = MP3_get_volume();
		printf("audio_Volume=%d\n",audio_Volume);
		//audio_player_mp3_Volume[4]=audio_Volume+'0';
		audio_player_mp3_Volume[0]=audio_Volume+'0';
		winSetResid(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),strToResId(audio_player_mp3_Volume));
		winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),false);
	}
}

static void audioplayer_update_playstatus(winHandle handle)
{
	if(MP3_is_playing())
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPAUSE);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPLAY);
}


static void audioplayer_update_playtime(winHandle handle)
{

	u16 min,sec;
	u32 play_time = MP3_get_playtime();
	u32 total_time = MP3_get_totaltime();
	u8 *file_name=NULL;

	min=play_time/60;
	sec=play_time%60;
	
	audio_player_mp3_time[0]=((min/10)%10)+'0';
	audio_player_mp3_time[1]=(min%10)+'0';
	audio_player_mp3_time[3]=((sec/10)%10)+'0';
	audio_player_mp3_time[4]=(sec%10)+'0';

	min=total_time/60;
	sec=total_time%60;
	audio_player_mp3_toatl_time[0]=((min/10)%10)+'0';
	audio_player_mp3_toatl_time[1]=(min%10)+'0';
	audio_player_mp3_toatl_time[3]=((sec/10)%10)+'0';
	audio_player_mp3_toatl_time[4]=(sec%10)+'0';

	winSetResid(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),strToResId(audio_player_mp3_time));
	winSetResid(winItem(handle,AUDIOPLAYER_PLAY_TOTAL_TIME_ID),strToResId(audio_player_mp3_toatl_time));

	file_name=MP3_get_item_name(MP3_get_item_idx());
	//printf("mp3 file_name=%s\n",file_name);
	memset(audio_player_mp3_file_name,0,sizeof(audio_player_mp3_file_name));
	if(SDC_STAT_NORMAL==SysCtrl.sdcard)
		strcpy(audio_player_mp3_file_name,file_name);
	//printf("mp3 audio_player_mp3_file_name=%s\n",audio_player_mp3_file_name);
	winSetResid(winItem(handle,AUDIOPLAYER_FILE_NAME_ID),strToResId(audio_player_mp3_file_name));
	winSetVisible(winItem(handle,AUDIOPLAYER_FILE_NAME_ID),true);
	
	winSetPorgressRate(audioPlayerProgressBar, play_time*100/total_time);
}

static void audioplayer_update_playlist(winHandle handle)
{
	u8 i;
	//u16 count = MP3_get_filelist(&mp3_filelist);

	int curfile=MP3_get_item_idx();
	int total_files=MP3_get_total_files();
	
	if(1)//audio_player_list.list_show_en)
	{
		/*for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),R_COLOR_BLACK);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),true);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),true);
			if(curfile>=(AUDIOPLAYER_LIST_NUM-1))
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(MP3_get_item_name(curfile-(AUDIOPLAYER_LIST_NUM-1)+i)));
			}
			else
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(MP3_get_item_name(i)));
			}
		}

		if(curfile>=(AUDIOPLAYER_LIST_NUM-1))
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+AUDIOPLAYER_LIST_NUM-1),R_COLOR_BLUE2);
		}
		else
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+curfile),R_COLOR_BLUE2);
		}
		
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),true);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),true);
*/
		//==set file num str==
		if(total_files>0)
		{
			u16 sel=curfile+1;//audio_player_list.list_idx+1;
			audio_player_mp3_num[0]=((sel/100)%10)+'0';
			audio_player_mp3_num[1]=((sel/10)%10)+'0';
			audio_player_mp3_num[2]=sel%10+'0';
			audio_player_mp3_num[4]=((total_files/100)%10)+'0';
			audio_player_mp3_num[5]=((total_files/10)%10)+'0';
			audio_player_mp3_num[6]=total_files%10+'0';
			winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
		}
		else
		{
			strcpy(audio_player_mp3_num,"000/000");
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);
		
	}
	else
	{
		/*for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),false);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),false);
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),false);*/

		for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),R_COLOR_BLACK);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),true);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),false);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),false);
		}
		
		winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID),R_COLOR_BLUE2);
		
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),true);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),true);

		//==set file num str==
		strcpy(audio_player_mp3_num,"000/000");
		winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);
	}
}

static int audioplayer_update_icon(winHandle handle)
{
	//==osd show==
	audioplayer_update_sdcard_icon(handle);
	audioPlayerBaterryShow(handle);//CommUpdateBaterryIcon(handle, AUDIOPLAYER_BATERRY_ID);
	audioplayer_update_playstatus(handle);
	audioplayer_update_volume(handle);
	audioplayer_update_playtime(handle);
	audioplayer_update_playlist(handle);
}

static int audioplayer_hide_icon(winHandle handle)
{
	winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_NUM_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_BACK_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_FORWARD_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_VOLUME_ICON_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_PLAY_TOTAL_TIME_ID),false);
	winSetVisible(winItem(handle,AUDIOPLAYER_FILE_NAME_ID),false);
	winSetVisible(audioPlayerProgressBar, false);
}