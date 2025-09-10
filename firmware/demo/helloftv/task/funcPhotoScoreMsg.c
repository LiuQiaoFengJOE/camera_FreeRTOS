#include "app.h"
#include "application.h"
#include "funcPhotoScoreRes.c"
#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"
#include "drv/csi/hal_csi.h"
//#include "mediaFile/demuxer.h"
//#include "fatfs/fileOps.h"
#include "lcd/lcdframe.h"
#include "userMedia.h"
#include "mediaFile/userInterface.h"
//#include "drv/csi/ax32xx_csi.h"
extern disp_frame_t *LcdGetVideoFrame(void);

static u8 *p_scr_buff=NULL;
static u16 bk_w,bk_h;

#define SCORE_BMP_PIXEL_W 		64
#define SCORE_BMP_PIXEL_H 		64
static u16 id_w,id_h;

static u8* score_id_buf;
static 	u8 *rgb24_buf;
static u8* score_bmp_id_buf;


static int16 score_draw_x=/*90*/260,score_draw_y=/*55*/100;

static u8 *focus_sound = 0;
static s32 focus_sound_size = 0;

static u8 score_key_flag=0,score_move_en=0;
static u8 score_move_state=0;

static u8 score_table[4]={0};

static u8 Random_array1[10]={1,8,9,7,6,0,3,5,4,2};
static u8 Random_array2[10]={0,1,5,2,8,4,7,6,3,9};
static u8 RandomScore=0;

static void photoscoreDisplay(winHandle handle,uint8 data)
{
	if(data>100)
		data=100;

	if(data==100)
	{
		score_table[0] = ((data/100)%10)+'0'; 
		score_table[1] = ((data/10)%10)+'0';
		score_table[2] = (data%10)+'0'; 
		score_table[3] = 0;
	}
	else if(data>=10)
	{
		score_table[0] = ((data/10)%10)+'0';
		score_table[1] = ((data)%10)+'0'; 
		score_table[2] = 0;
	}
	else
	{
		score_table[0] = (data%10)+'0'; 
		score_table[1] = 0;
	}
	winSetResid(winItem(handle,PHOTO_SCORE_STR2_ID),strToResId(score_table));
	winSetVisible(winItem(handle,PHOTO_SCORE_STR2_ID),true);
}



static void photoscoreMoveShow(winHandle handle,uint16 draw_x,uint16 draw_y)
{
	disp_frame_t *p_lcd_buffer;
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0,cnt;

	p_lcd_buffer = lcdVideoFrameMalloc();
	if(p_lcd_buffer)
	{
		lcdGetRes(&screenW, &screenH);
		lcdGetVideoRes(&lcdW,&lcdH);
		lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
		memcpy(p_lcd_buffer->y_addr,p_scr_buff,bk_w*bk_h*3/2);
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,draw_x,draw_y,id_w,id_h,score_bmp_id_buf,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		lcdDealVideBuff(p_lcd_buffer);
	}	
}

static int photoscoreKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(score_move_en){
			return -1;
		}
		if(score_key_flag==0)
		{
			score_key_flag=1;
			while(dacIsPlaying())
			{
				osal_task_sleep(5);
			}
			dacPlaySound(RES_TAKE_PHOTO_MUSIC,getSoundVol());
			hal_csiEnable(0);
			disp_frame_t *p_lcd_buffer;
			p_lcd_buffer = LcdGetVideoFrame();
			if (p_lcd_buffer)
			{
				memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
			}
			lcdPreviewStop();
			
			winSetVisible(winItem(handle,PHOTO_SCORE_MD_ID),false);
			score_move_en=1;
			score_move_state=0;
			score_draw_x=260;//90;
			score_draw_y=100;//55;
		}
		else
		{
			score_key_flag=0;
			hal_csiEnable(1);
			lcdPreviewStart();
			winSetVisible(winItem(handle,PHOTO_SCORE_MD_ID),true);
			winSetVisible(winItem(handle,PHOTO_SCORE_STR2_ID),false);
			winSetResid(winItem(handle,PHOTO_SCORE_STR1_ID),R_ID_STR_PHOTO_DETECTION);
		}
		
	}
	
	return 0;
}
static int photoKeyMsgLongOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if (takePictrueIsWorking()==0)
		{
			freePicStickerA();
			animationInit(LEFTBOTTOM2RIGHTUPPER);
			osal_task_sleep(40);
			sensor_change();
		}
	}
	return 0;
}


static int photoKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}

static int photoscoreKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(score_key_flag==0){
			functionStart(FUNCTION_MAINMENU,0);
			/*if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=2;
				SysCtrl.cartoon_show_cnt=0;
			}*/
		}
	}
	return 0;
}

static int photoscoreTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/

	if(parameNum!=3)
	{
		printf("takephotoTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		/*if(parame[0]==PHOTO_SCORE_EXE_MAIN_ID)
		{
			printf("==back to mainmenu\n");
			functionStart(FUNCTION_MAINMENU,0);
		}*/
	}
	return 0;
}


static int photoscoreOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("photo Open Win!!!\n");
	mdSetEn(0);

	yuv_rgb_table();

	//==init==
	lcdGetVideoRes(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment
	p_scr_buff=getCurModeFreeBuffBase();
	if(modeFreeSpace() < bk_w*bk_h*3/2)
	{
		printf("photoscoreOpenWin err,no memory,%dKB\n",modeFreeSpace()>>10);
		return 0;
	}
	/*if(p_scr_buff == NULL)
	{
		p_scr_buff=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	}*/
	id_w=(SCORE_BMP_PIXEL_W+0x1)&(~0x1); // bmp must 2pixel align
	id_h=(SCORE_BMP_PIXEL_H+0x1)&(~0x1);
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		printf("mem err!\n");
		return 0;
	}

	if(NULL==score_bmp_id_buf)
	{
		score_bmp_id_buf=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(score_bmp_id_buf)
		{
			bmp24_to_yuv420_buf(RES_SCORING,rgb24_buf,score_bmp_id_buf,score_bmp_id_buf+id_w*id_h,id_w,id_h);
		}
	}

	winSetResid(winItem(handle,PHOTO_SCORE_TIPS_ID),R_ID_ICON_MTPHOTOSCORETIPS);
	winSetResid(winItem(handle,PHOTO_SCORE_MD_ID),R_ID_ICON_MTPHOTOFOCUSRED);

	winSetResid(winItem(handle,PHOTO_SCORE_STR1_ID),R_ID_STR_PHOTO_DETECTION);
	winSetVisible(winItem(handle,PHOTO_SCORE_STR2_ID),false);
	return 0;
}
static int photoscoreCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("photo Close Win!!!\n");

	/*if(p_scr_buff)
	{
		hal_sysMemFree(p_scr_buff);
		p_scr_buff=NULL;
	}*/
	
	if(score_bmp_id_buf)
	{
		hal_sysMemFree(score_bmp_id_buf);
		score_bmp_id_buf=NULL;
	}
	
	if(rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}

	if(focus_sound)
	{
		hal_sysMemFree(focus_sound);
		focus_sound = 0;
	}
	
//	yuv_rgb_table_uninit();
	
	return 0;
}

static int photoscoreSysMsg200MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(score_move_en)
	{
		if(score_move_state==0)  // letf to right
		{
			score_draw_x+=20;
			if(score_draw_x>=/*200*/340)
			{
				score_move_state=1;
			}
			
		}
		else if(score_move_state==1)  // up to down
		{
			score_draw_y+=10;
			if(score_draw_y>=/*100*/160)
			{
				score_move_state=2;
			}
		}
		else if(score_move_state==2)  // right to left
		{
			score_draw_x-=20;
			if(score_draw_x<=/*90*/260)
			{
				score_move_state=3;
			}
		}
		else if(score_move_state==3)  // down to up
		{
			score_draw_y-=10;
			if(score_draw_y<=/*55*/100)
			{
				score_move_state=4;
			}
		}

		if(score_move_state==4)
		{
			score_move_en=0;
			disp_frame_t *p_lcd_buffer;
			INT16U screenW=0,screenH=0;
			INT16U lcdW=0,lcdH=0,cnt;
			p_lcd_buffer = lcdVideoFrameMalloc();
			if(p_lcd_buffer)
			{
				lcdGetRes(&screenW, &screenH);
				lcdGetVideoRes(&lcdW,&lcdH);
				lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
				memcpy(p_lcd_buffer->y_addr,p_scr_buff,bk_w*bk_h*3/2);
				dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
				lcdDealVideBuff(p_lcd_buffer);
			}
			else
			{
				printf("==lcd frame malloc err\n");
			}
			winSetResid(winItem(handle,PHOTO_SCORE_STR1_ID),R_ID_STR_PHOTO_SCORE);
			photoscoreDisplay(handle, Random_array1[Random_array2[RandomScore]]*10+Random_array2[Random_array1[RandomScore]]);
		}
		else
		{
			photoscoreMoveShow(handle,score_draw_x,score_draw_y);
		}
	}

	 RandomScore++;
	if(RandomScore>=10)
		RandomScore=0;
	
	return 0;
}

static int photoscoreSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor photoscoreMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoscoreOpenWin},
	{SYS_CLOSE_WINDOW,photoscoreCloseWin},
	{SYS_TOUCH_WINDOW,photoscoreTouchWin},
	{KEY_EVENT_MODE,photoscoreKeyMsgExit},
	{KEY_EVENT_OK,photoscoreKeyMsgOk},
	{SYS_EVENT_500MS,photoscoreSysMsg200MS},
	{SYS_EVENT_1S,photoscoreSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(photoscoreWindow,photoscoreMsgDeal,photoscoreWin)


