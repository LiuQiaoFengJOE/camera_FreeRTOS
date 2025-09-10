#include "app.h"
#include "application.h"
#include "funcPlayBackEditRes.c"
//#include "drv/isp/ax32xx_isp.h"
//#include "drv/csi/ax32xx_csi.h"
//#include "drv/csi/hal_csi.h"
//#include "hal_sys.h"
#include"fatfs/fileManage.h"
#include "fatfs/fileOps.h"
#include "drv/lcd/lcdframe.h"
#include "userMedia.h"
#include "mediaFile/userInterface.h"
#include "touchPanel.h"


extern int playbackIsRecording(void);
extern menu MENU(movie);
#if 1
static u8 *p_scr_buff;
static u16 bk_w,bk_h;

#define PLAYBACK_EDIT_BMP_NUM 	8
#define PLAYBACK_EDIT_PIXEL_W 		120
#define PLAYBACK_EDIT_PIXEL_H 		120
static u16 id_w,id_h;

static u32 playback_edit_bmp_id[PLAYBACK_EDIT_BMP_NUM]={RES_PLAYBACK_FRAME1_0,RES_PLAYBACK_FRAME1_1,RES_PLAYBACK_FRAME1_2,RES_PLAYBACK_FRAME1_3,
														RES_PLAYBACK_FRAME1_4,RES_PLAYBACK_FRAME1_5,RES_PLAYBACK_FRAME1_6,RES_PLAYBACK_FRAME1_7};
static u8* playback_edit_id_buf=NULL;
static 	u8 *rgb24_buf=NULL;

static int16 playback_edit_draw_x=260,playback_edit_draw_y=120;


static uint8 playback_edit_menu_en=0,playback_edit_menu_count=0;
static uint8 custom_frame_add_state=0; // 0: 1

static uint8 playback_edit_menu_mov_state=0; // 0: 1: 2
static uint8 playback_edit_bmp_count=0;

static uint8 playback_edit_menu_mov_direction=0; // 0: right_letf  1:up_down
static u8 playback_edit_menu_count_temp=0xff;

extern void changeDir(u32 fileType);
extern u8 *getCurModeFreeBuffBase(void);
extern u32 not_isr_jpgEnc(u8* yuvBuf,u16 w,u16 h,u16 jpgw,u16 jpgh,u8* jpgStart,u8* jpgEnd);

int image_take_photo2(u8 *src_ybuf,u16 src_width,u16 src_height)
{
	#define PHOTO_W  640
	#define PHOTO_H  480
	INT32U size;
	INT8U *buf = 0;
	void *fp;
	
	changeDir(JPG_FILE);
	buf = getCurModeFreeBuffBase()+bk_w*bk_h*3/2;
	if(modeFreeSpace() < bk_w*bk_h*3/2+100*1024)
	{
		printf("jpgEnc err,no memory,%dKB\n",modeFreeSpace()>>10);
		return -1;
	}
	size=not_isr_jpgEnc(src_ybuf,src_width,src_height,PHOTO_W,PHOTO_H,buf,buf+100*1024);
	
	printf("file write size:%d\n",size);
	fp=osal_fopen(CREATE_PIC_NEW, "w");
	osal_fwrite(buf, 1, size, fp);
	osal_fclose(fp);
	printf("write size:%d\n",size);
	return 0;
}
static int playBackEdit_menu_select(winHandle handle,uint8 select)
{
	winSetResid(winItem(handle,PLAYBACKEDIT_FRAME0_ID),R_ID_ICON__PHOTOEDIT_FRAME0_UNSEL);
	winSetResid(winItem(handle,PLAYBACKEDIT_FRAME1_ID),R_ID_ICON__PHOTOEDIT_FRAME1_UNSEL);
	winSetResid(winItem(handle,PLAYBACKEDIT_EFFECT_ID),R_ID_ICON__PHOTOEDIT_FRAME2_UNSEL);
	winSetResid(winItem(handle,PLAYBACKEDIT_CANCEL_ID),R_ID_ICON__PHOTOEDIT_FRAME3_UNSEL);
	winSetResid(winItem(handle,PLAYBACKEDIT_SAVE_ID),R_ID_ICON__PHOTOEDIT_FRAME4_UNSEL);

	if(select==0)
	{
		winSetResid(winItem(handle,PLAYBACKEDIT_FRAME0_ID),R_ID_ICON__PHOTOEDIT_FRAME0_SEL);
	}
	else if(select==1)
	{
		winSetResid(winItem(handle,PLAYBACKEDIT_FRAME1_ID),R_ID_ICON__PHOTOEDIT_FRAME1_SEL);
	}
	else if(select==2)
	{
		winSetResid(winItem(handle,PLAYBACKEDIT_EFFECT_ID),R_ID_ICON__PHOTOEDIT_FRAME2_SEL);
	}
	else if(select==3)
	{
		winSetResid(winItem(handle,PLAYBACKEDIT_CANCEL_ID),R_ID_ICON__PHOTOEDIT_FRAME3_SEL);
	}
	else if(select==4)
	{
		winSetResid(winItem(handle,PLAYBACKEDIT_SAVE_ID),R_ID_ICON__PHOTOEDIT_FRAME4_SEL);
	}
	else
	{

	}
	return 0;
}

static int playBackEdit_mov_en(winHandle handle,uint8 en)
{
	if(en)
	{
		winSetVisible(winItem(handle,PLAYBACKEDIT_LEFT_ID),true);
		winSetVisible(winItem(handle,PLAYBACKEDIT_RIGHT_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACKEDIT_LEFT_ID),false);
		winSetVisible(winItem(handle,PLAYBACKEDIT_RIGHT_ID),false);
	}
	return 0;
}

static int playBackEdit_movUP_en(winHandle handle,uint8 en)
{
	if(en)
	{
		winSetVisible(winItem(handle,PLAYBACKEDIT_UP_ID),true);
		winSetVisible(winItem(handle,PLAYBACKEDIT_DOWN_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACKEDIT_UP_ID),false);
		winSetVisible(winItem(handle,PLAYBACKEDIT_DOWN_ID),false);
	}
	return 0;
}

static int playBackEdit_menu_en(winHandle handle,uint8 en)
{
	uint8 i;
	if(en)
	{
		for(i=0;i<5;i++)
		{
			winSetVisible(winItem(handle,PLAYBACKEDIT_FRAME0_ID+i),true);
		}

		playBackEdit_menu_select(handle,playback_edit_menu_count);
	}
	else
	{
		for(i=0;i<5;i++)
		{
			winSetVisible(winItem(handle,PLAYBACKEDIT_FRAME0_ID+i),false);
		}
	}
	return 0;
}

static int playBackEditeffectShow(void)
{
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0;
	disp_frame_t *p_lcd_buffer = lcdVideoFrameMalloc();
	if (p_lcd_buffer == NULL)
	{
		printf("lcdFrame is NULL\n");
		return -1;
	}	
	memcpy(p_lcd_buffer->y_addr,p_scr_buff,bk_w*bk_h*3/2);
	yuv420_effect_handle(p_lcd_buffer->y_addr,p_lcd_buffer->uv_addr,bk_w,bk_h);

	if(p_lcd_buffer)
	{
		lcdGetRes(&screenW, &screenH);
		lcdGetVideoRes(&lcdW,&lcdH);
		lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
		dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		lcdDealVideBuff(p_lcd_buffer);

		/*hal_dispframeVideoCfg(p_lcd_buffer,0,0,bk_w,bk_h);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);*/
	}
	else
	{
		printf("video buf null!\n");
	}
	return 0;
}


static int playBackEditFrameShow(void)
{
	if(custom_frame_add_state==1)
	{
		INT16U screenW=0,screenH=0;
		INT16U lcdW=0,lcdH=0;		
		disp_frame_t *lcdFrame = lcdVideoFrameMalloc();
		if (lcdFrame == NULL)
		{
			printf("lcdFrame is NULL\n");
			return -1;
		}	
		lcdGetRes(&screenW, &screenH);
		lcdGetVideoRes(&lcdW,&lcdH);
		lcdframeVideoCfg(lcdFrame,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
		
		memcpy(lcdFrame->y_addr,p_scr_buff,bk_w*bk_h*3/2);
		addPicStickerB(lcdFrame->y_addr);
		lcdDealVideBuff(lcdFrame);
	}

	return 0;
}

static int playBackEditExitFrame(void)
{
#if 0
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0;		
	disp_frame_t *lcdFrame = lcdVideoFrameMalloc();
	if (lcdFrame == NULL)
	{
		printf("lcdFrame is NULL\n");
		return -1;
	}	
	lcdGetRes(&screenW, &screenH);
	lcdGetVideoRes(&lcdW,&lcdH);
	lcdframeVideoCfg(lcdFrame,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
	
	memcpy(lcdFrame->y_addr,p_scr_buff,bk_w*bk_h*3/2);
	dcache_writeback_region((u32)lcdFrame->y_addr,lcdFrame->w*lcdFrame->h*3/2);
	lcdDealVideBuff(lcdFrame);
#else
	sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_REPLAY_CUR_FILE,0);	
#endif
	//video_playback_start(SysCtrl.file_index,SysCtrl.jpg_list);
	return 0;
}

static void playBackEdit_frame_show_move(u16 draw_x,u16 draw_y)
{
	disp_frame_t *p_lcd_buffer;
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0;
			
	p_lcd_buffer = lcdVideoFrameMalloc();
	if(p_lcd_buffer)
	{
		lcdGetRes(&screenW, &screenH);
		lcdGetVideoRes(&lcdW,&lcdH);
		lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);

		if(draw_x>bk_w-id_w/*520*/)
			draw_x=bk_w-id_w;
		if(draw_y>bk_h-id_h)
			draw_y=bk_h-id_h;
		memcpy(p_lcd_buffer->y_addr,p_scr_buff,bk_w*bk_h*3/2);
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,draw_x,draw_y,id_w,id_h,playback_edit_id_buf,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		lcdDealVideBuff(p_lcd_buffer);
	}
	else
	{
		printf("video buf null!\n");
	}
}

u8  playback_edit_flag=0;
u8  playback_edit_menu_mov_flag=0;
#endif
static int playbackKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0;	
	disp_frame_t *p_lcd_buffer;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("-----=%d,,,%d-----%d-\n",playback_edit_menu_en,playback_edit_menu_count,playback_edit_menu_mov_state);

		if(playback_edit_menu_en)
		{	
			if (playback_edit_menu_count == 0)
				playback_edit_flag=1;
			else
				playback_edit_flag=0;
			playback_edit_menu_en=0;
			playBackEdit_menu_en(handle,0);
		}
		if(playback_edit_menu_count==0)
		{
			if(playback_edit_flag)
			{
				playback_edit_menu_count_temp = playback_edit_menu_count;
				if(custom_frame_add_state==0)
				{
					custom_frame_add_state = 1;
					p_lcd_buffer = LcdGetVideoFrame();
					if (p_lcd_buffer)
					{
						memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
					}
					osal_task_sleep(50);
					
					picStickerLoop();
					playBackEditFrameShow();
				}
			}
		}
		else if(playback_edit_menu_count==1)
		{
			playback_edit_menu_count_temp = playback_edit_menu_count;
			if(playback_edit_menu_mov_state ==0)
			{
				playback_edit_menu_mov_state=1;
				playback_edit_menu_mov_flag=0;
				
				p_lcd_buffer = LcdGetVideoFrame();
				if (p_lcd_buffer)
				{
					memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
				}
				p_lcd_buffer = lcdVideoFrameMalloc();
				if (p_lcd_buffer)
				{
					lcdGetRes(&screenW, &screenH);
					lcdGetVideoRes(&lcdW,&lcdH);
					lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
					memcpy(p_lcd_buffer->y_addr,p_scr_buff,bk_w*bk_h*3/2);
					playback_edit_draw_x=(bk_w-id_w)/2;//260;
					playback_edit_draw_y=(bk_h-id_h)/2;//120;
					
					yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,playback_edit_draw_x,playback_edit_draw_y,id_w,id_h,playback_edit_id_buf,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
					dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
					lcdDealVideBuff(p_lcd_buffer);
				}
				else
				{
					printf("video buf null!\n");
				}
			}
			else if(playback_edit_menu_mov_state ==1)
			{
				playback_edit_menu_mov_state=2;
				playBackEdit_mov_en(handle,1);
				playBackEdit_movUP_en(handle,1);
			}
			else if(playback_edit_menu_mov_state ==2)
			{
				playback_edit_menu_mov_state=1;//0;
				playback_edit_menu_mov_flag=1;
				playBackEdit_mov_en(handle,0);
				playBackEdit_movUP_en(handle,0);
			}
		}
		else if(playback_edit_menu_count==2&&playback_edit_menu_count_temp != playback_edit_menu_count)  //effects
		{
			playback_edit_menu_count_temp = playback_edit_menu_count;
			playBackEditExitFrame();
			osal_task_sleep(20);
			p_lcd_buffer = LcdGetVideoFrame();
			if (p_lcd_buffer)
			{
				memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
			}
				
		//	dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);

			SysCtrl.photo_effect_idx=0;
			playBackEditeffectShow();
		}
		else if(playback_edit_menu_count==3) //cancel
		{
			playback_edit_menu_count_temp = playback_edit_menu_count;
			playback_edit_menu_en=0;
			playback_edit_menu_mov_state=0;
			playback_edit_menu_count=0;
			playback_edit_bmp_count=0;
			if(custom_frame_add_state==1) 
			{
				custom_frame_add_state = 0;
				picStickerClose();
			}
			playBackEditExitFrame();
			playBackEdit_mov_en(handle,0);
			playBackEdit_movUP_en(handle,0);
		}
		else if(playback_edit_menu_count==4)// save
		{
			playback_edit_menu_count_temp = playback_edit_menu_count;
			//p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
			p_lcd_buffer = LcdGetVideoFrame();
			image_take_photo2(p_lcd_buffer->y_addr,bk_w,bk_h);

			//dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);

			playback_edit_menu_en=0;
			playback_edit_menu_mov_state=0;
			playback_edit_menu_count=0;
			playback_edit_bmp_count=0;
			if(custom_frame_add_state==1) 
			{
				custom_frame_add_state = 0;
				picStickerClose();
			}
			
			playBackEdit_mov_en(handle,0);
			playBackEdit_movUP_en(handle,0);
			playBackEditExitFrame();

			osal_task_sleep(5);
			playbackFileNameShow(handle);
		}
	}
	return 0;
}
static int playbackKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("上\n");

		if(playback_edit_menu_en)
		{
			playback_edit_menu_count--;
			printf("playback_edit_menu_count=%d\n",playback_edit_menu_count);
			if(playback_edit_menu_count>=250)
				playback_edit_menu_count=4;

			playBackEdit_menu_select(handle,playback_edit_menu_count);
		}
		
		else if(playback_edit_menu_count==0&&(custom_frame_add_state==0))
		{
			printf("1\n");
			sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_PRE_JPG,0);
		}
		else if(playback_edit_menu_count==0&&(custom_frame_add_state==1))
		{
			printf("2\n");
			picStickerLoop();

			playBackEditFrameShow();
		}
		else if(playback_edit_menu_count==1)
		{
			printf("3\n");
			if(playback_edit_menu_mov_state ==1)
			{
				printf("4\n");
				if(playback_edit_menu_mov_flag==1)return 0;
				playback_edit_bmp_count++;
				if(playback_edit_bmp_count>=PLAYBACK_EDIT_BMP_NUM)
					playback_edit_bmp_count=0;
					
				bmp24_to_yuv420_buf(playback_edit_bmp_id[playback_edit_bmp_count],rgb24_buf,playback_edit_id_buf,playback_edit_id_buf+id_w*id_h,id_w,id_h);

				osal_task_sleep(50);
				playback_edit_draw_x=(bk_w-id_w)/2;//260;
				playback_edit_draw_y=(bk_h-id_h)/2;//120;				
				playBackEdit_frame_show_move(playback_edit_draw_x,playback_edit_draw_y);
			}
			else if(playback_edit_menu_mov_state ==2)
			{
				printf("5=%d\n",playback_edit_menu_mov_direction);
				if(playback_edit_menu_mov_direction==0)  //right
				{
					playback_edit_draw_x-=10;
					if(playback_edit_draw_x<0)
						playback_edit_draw_x=0;
				}
				else  // up
				{
					playback_edit_draw_y-=10;
					if(playback_edit_draw_y<=0)
						playback_edit_draw_y=0;
				}

				playBackEdit_frame_show_move(playback_edit_draw_x,playback_edit_draw_y);
			}
		}
		else if(playback_edit_menu_count==2) //effect
		{
			SysCtrl.photo_effect_idx++;
			if(SysCtrl.photo_effect_idx>=CAM_EFFECT_MAX)
				SysCtrl.photo_effect_idx=0;
			
			playBackEditeffectShow();
		}
	}		
	return 0;
}

static int playbackKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("下\n");

		if(playback_edit_menu_en)
		{
			playback_edit_menu_count++;
			
			printf("playback_edit_menu_count=%d\n",playback_edit_menu_count);
			if(playback_edit_menu_count>=5)
				playback_edit_menu_count=0;

			playBackEdit_menu_select(handle,playback_edit_menu_count);
		}
		else if(playback_edit_menu_count==0&&(custom_frame_add_state==0))
		{
			sendMsgAndWait(OS_TASK_VIDEOPLAY, TASK_MSG_PLAY_NEXT_JPG,0);
		}
		else if(playback_edit_menu_count==0&&(custom_frame_add_state==1))
		{
			picStickerLoop();
			playBackEditFrameShow();
		}
		else if(playback_edit_menu_count==1)
		{
			if(playback_edit_menu_mov_state ==1)
			{
				if(playback_edit_menu_mov_flag==1)return 0;
				playback_edit_bmp_count--;
				if(playback_edit_bmp_count>=250)
					playback_edit_bmp_count=PLAYBACK_EDIT_BMP_NUM-1;
					
				bmp24_to_yuv420_buf(playback_edit_bmp_id[playback_edit_bmp_count],rgb24_buf,playback_edit_id_buf,playback_edit_id_buf+id_w*id_h,id_w,id_h);

				osal_task_sleep(50);
				playback_edit_draw_x=(bk_w-id_w)/2;//260;
				playback_edit_draw_y=(bk_h-id_h)/2;//120;
				playBackEdit_frame_show_move(playback_edit_draw_x,playback_edit_draw_y);
			}
			else if(playback_edit_menu_mov_state ==2)
			{
				printf("5=%d\n",playback_edit_menu_mov_direction);
				if(playback_edit_menu_mov_direction==0)  //left    //小头贴方向
				{
					playback_edit_draw_x+=10;
					if(playback_edit_draw_x>bk_w-id_w)
						playback_edit_draw_x=bk_w-id_w;
				}
				else
				{
					playback_edit_draw_y+=10;
					if(playback_edit_draw_y>=bk_h-id_h)
						playback_edit_draw_y=bk_h-id_h;
				}

				playBackEdit_frame_show_move(playback_edit_draw_x,playback_edit_draw_y);
			}
		}
		else if(playback_edit_menu_count==2) //effect
		{
			SysCtrl.photo_effect_idx--;
			if(SysCtrl.photo_effect_idx>=250)
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-1;;
			
			playBackEditeffectShow();
		}
	}
	return 0;
}
extern menu MENU(playBack);
static int playbackKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(getPlayStat()!=PLAY_BACK_PLAYING)
			uiOpenWindow(&menuItemWindow,1,&MENU(playBack));
	}
	return 0;
}
static int playbackKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(custom_frame_add_state==1)
		{
			custom_frame_add_state = 0;
			picStickerClose();
		}
		playback_edit_flag=0;
		functionStart(FUNCTION_MAINMENU,0);
		/*if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}*/
	}
	return 0;
}
static int playbackSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	playbackSDShow(handle);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}

static int playbackSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	playbackBaterryShow(handle);
	return 0;
}

static int playbackSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int playBackTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/
	
	if(parameNum!=3)
	{
		printf("playBackTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==PLAYBACK_EXE_MAIN_ID)
		{
			if(playback_edit_menu_mov_state ==2)
			{
				if(playback_edit_menu_mov_direction==0) 
				{
					playback_edit_menu_mov_direction=1;
				}
				else
				{
					playback_edit_menu_mov_direction=0;
				}
			}
			else
			{
				if(getPlayStat()!=PLAY_BACK_PLAYING)
				{
					playback_edit_flag=0;
					functionStart(FUNCTION_MAINMENU,0);
				}
			}
		}
		else if(parame[0]==PLAYBACK_EXE_PRE_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_UP, KEY_PRESSED);
		}
		else if(parame[0]==PLAYBACK_EXE_NEXT_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_DOWN, KEY_PRESSED);
		}
		else if(parame[0]==PLAYBACK_EXE_SET_ID)
		{
			setNotplayKeySoundFlag(1);
			sendMsgNonblock(OS_TASK_UI, KEY_EVENT_MENU, KEY_PRESSED);
			
			if(playback_edit_menu_en)
			{
				playback_edit_menu_en=0;
				playBackEdit_menu_en(handle,0);
			}
			else
			{
				playback_edit_menu_en=1;
				playBackEdit_menu_en(handle,1);
			}			
		}
	}
	return 0;
}
static int playBackSlidRelease(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=1)
	{
		printf("playBackSlidRelease, parame num error %d\n",parameNum);
		return 0;
	}

	return 0;
}
static int playbackOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	resInfor infor;
	uint8 i;
	disp_frame_t *p_lcd_buffer;
	
	printf("playback edit  Open Win!!!\n");

	yuv_rgb_table();

	//==init==
	lcdGetVideoRes(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment

	p_scr_buff=getCurModeFreeBuffBase();
	if(modeFreeSpace() < bk_w*bk_h*3/2+100*1024)
	{
		printf("playbackEditOpenWin err,no memory,%dKB\n",modeFreeSpace()>>10);
		return 0;
	}	

	p_lcd_buffer = LcdGetVideoFrame();
	if (p_lcd_buffer)
	{
		memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
	}	
	/*if(p_scr_buff == NULL)
	{
		p_scr_buff=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	}*/

	id_w=(PLAYBACK_EDIT_PIXEL_W+0x1)&(~0x1); // bmp must 2pixel align
	id_h=(PLAYBACK_EDIT_PIXEL_H+0x1)&(~0x1);
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		printf("mem err!\n");
		return 0;
	}

	if(NULL==playback_edit_id_buf)
	{
		playback_edit_id_buf=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(playback_edit_id_buf)
		{
			bmp24_to_yuv420_buf(playback_edit_bmp_id[playback_edit_bmp_count],rgb24_buf,playback_edit_id_buf,playback_edit_id_buf+id_w*id_h,id_w,id_h);
		}
	}

	/*if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}*/

	playback_edit_menu_en=0;
	playback_edit_menu_mov_state=0;
	playback_edit_menu_count=0;
	playback_edit_bmp_count=0;
	playback_edit_menu_mov_direction=0;

	for(i=0;i<9;i++)
	{
		winSetVisible(winItem(handle,PLAYBACKEDIT_LEFT_ID+i),false);
	}
	
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_LEFT;
	infor.fontColor=R_COLOR_WHITE;
	infor.rimColor=R_COLOR_BLACK;
	winSetUnselectInfor(winItem(handle,PLAYBACK_FILE_NAME_ID),&infor);
	resInforInit(&infor);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackBaterryShow(handle);
	playbackFileNameShow(handle);
	playbackBKShow(handle, 0);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}
static int playbackCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback Close Win!!!\n");

	if(custom_frame_add_state==1)
	{
		custom_frame_add_state = 0;
		picStickerClose();
	}
	playback_edit_menu_count_temp = 0;

	/*if(p_scr_buff)
	{
		hal_sysMemFree(p_scr_buff);
		p_scr_buff=NULL;
	}*/

	if(playback_edit_id_buf)
	{
		hal_sysMemFree(playback_edit_id_buf);
		playback_edit_id_buf=NULL;
	}

	if(rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}

	return 0;
}
static int playbackWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback WinChild Close!!!\n");
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackBaterryShow(handle);
	playbackFileNameShow(handle);
	if(!curFloderHasFile())
		uiOpenWindow(&noFileWindow,0);
	return 0;
}
static int playbackWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("playback WinChild Open!!!\n");
	return 0;
}

static int playbackStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	disp_frame_t *p_lcd_buffer;
	
	playbackFileNameShow(handle);
	p_lcd_buffer = LcdGetVideoFrame();
	if (p_lcd_buffer)
	{
		memcpy(p_scr_buff,p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
	}
	return 0;
}
static int playbackPause(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playbackResume(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playbackStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor playBackEditMsgDeal[]=
{
	{SYS_OPEN_WINDOW,playbackOpenWin},
	{SYS_CLOSE_WINDOW,playbackCloseWin},
	{SYS_CHILE_COLSE,playbackWinChildClose},
	{SYS_CHILE_OPEN,playbackWinChildOpen},
	{SYS_TOUCH_WINDOW,playBackTouchWin},
	{SYS_SLID_RELEASE,playBackSlidRelease},
	{KEY_EVENT_OK,playbackKeyMsgOk},
	{KEY_EVENT_UP,playbackKeyMsgUp},
	{KEY_EVENT_DOWN,playbackKeyMsgDown},
	//{KEY_EVENT_MENU,playbackKeyMsgMenu},
	//{KEY_EVENT_MODE,playbackKeyMsgMode},
	{KEY_EVENT_POWER,playbackKeyMsgMode},
	{SYS_EVENT_SDC,playbackSysMsgSD},
	{SYS_EVENT_USB,playbackSysMsgUSB},
	{SYS_EVENT_BAT,playbackSysMsgBattery},
	{PLAY_EVENT_PLAYSTART,playbackStarted},
	{EVENT_MAX,NULL},
};

WINDOW(playBacEditkWindow,playBackEditMsgDeal,playBackEditWin)


