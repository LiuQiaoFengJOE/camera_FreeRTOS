#include "../app.h"

#if GAME_FUNC_EN

#include "task/uiWin.h"
#include "funcGame.h"

enum
{
	GAMEMENU_SEL_LEFT_ID=0,
	GAMEMENU_SEL_RIGHT_ID,
	GAMEMENU_MAX_ID
};

static widgetCreateInfor gameMenuWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

//	createImageIcon(GAMEMENU_SEL_LEFT_ID,Rx(32), Ry(110), Rw(20), Rh(20), R_ID_ICON_MTGLEFT,ALIGNMENT_CENTER),
//	createImageIcon(GAMEMENU_SEL_RIGHT_ID,Rx(320-32), Ry(110), Rw(20), Rh(20), R_ID_ICON_MTGRIGHT,ALIGNMENT_CENTER),

//	createStringIcon(MAINMENU_PHOTO_ID,Rx(0), Ry(96), Rw(106), Rh(25),strToResId("PHOTO"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


#include "task/userMedia.h"

#define GAME_NUM 	5
#define GAMEMENU_PIXEL_W 		120
#define GAMEMENU_PIXEL_H 		120
#define GAMEMENU_BFWARD_W 	48
#define GAMEMENU_BFWARD_H 	32

static res_img_t gamemenu_bg_img;
static res_img_t gamemenu_backward;
static res_img_t gamemenu_forward;
static res_img_t gamemenu_icon_img[GAME_NUM];

static u32 gamemenu_icon_id[GAME_NUM]={
	//RES_GAMEMENU_MUSIC,
	RES_GAMEMENU_SNAKE,
	RES_GAMEMENU_MAZE,
	RES_GAMEMENU_PUSHBOX,
	RES_GAMEMENU_RB,
	RES_GAMEMENU_BOLL
};

static u8 game_idx=0;
static u8* gamemenu_show_buf;

#if 0
static 	u8 *rgb24_buf;
static u32 game_bmp_id[GAME_NUM]={RES_GAMEMENU_MUSIC,RES_GAMEMENU_SNAKE,RES_GAMEMENU_MAZE,RES_GAMEMENU_PUSHBOX,RES_GAMEMENU_RB,RES_GAMEMENU_BOLL};

static u8* gamemenu_bk_buf;
static u16 bk_w,bk_h;

static u8* gamemenu_id_buf[GAME_NUM];
static u16 id_w,id_h;

static u8* gamemenu_backward_buf;
static u8* gamemenu_forward_buf;
static u16 bfward_w,bfward_h;

//==dir 0: left move, 1: right move==
static void gameMenu_show_move(u8 dir)
{
	disp_frame_t *p_lcd_buffer;
	u16 x,step;
	u8 next_game_idx;

	if(0==dir)
	{
		if(game_idx<(GAME_NUM-1))
		{
			next_game_idx=game_idx+1;
		}
		else
		{
			next_game_idx=0;
		}
	}
	else
	{
		if(game_idx>0)
		{
			next_game_idx=game_idx-1;
		}
		else
		{
			next_game_idx=GAME_NUM-1;
		}
	}

	if(NULL==gamemenu_id_buf[next_game_idx])
	{
		gamemenu_id_buf[next_game_idx]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(gamemenu_id_buf[next_game_idx])
		{
			bmp24_to_yuv420_buf(game_bmp_id[next_game_idx],rgb24_buf,gamemenu_id_buf[next_game_idx],gamemenu_id_buf[next_game_idx]+id_w*id_h,id_w,id_h);
			//deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*gamemenu_id_buf[game_idx],*(gamemenu_id_buf[game_idx]+id_w*id_h),*(gamemenu_id_buf[game_idx]+1),*(gamemenu_id_buf[game_idx]+id_w*id_h+1));
		}
	}

	for(step=0;step<11;step++)
	{
		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			hal_lcdGetResolution(&lcd_w,&lcd_h);
			//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

			//==draw bk==
			memcpy(p_lcd_buffer->y_addr,gamemenu_bk_buf,bk_w*bk_h*3/2);
			if(0==dir)
			{
				//==draw game cur==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100-step*20,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100+220-step*20,60,id_w,id_h,gamemenu_id_buf[next_game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else
			{
				//==draw game cur==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100+step*20,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,step*20-120,60,id_w,id_h,gamemenu_id_buf[next_game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			//==draw forward backward==
			if(0==dir)
			{
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,0,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-80,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else
			{
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,32,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-48,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}

			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
			XOSTimeDly(20);
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}

}


static void gameMenu_showbuf()
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		//==draw bk==
		memcpy(p_lcd_buffer->y_addr,gamemenu_bk_buf,bk_w*bk_h*3/2);
		//==draw game==
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		//==draw forward backward==
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,32,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-80,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);

		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}

#endif

#define YUV_ALPHA_Y	    0x8A
#define YUV_ALPHA_UV	0x81

#include "drv/lcd/lcdframe.h"

static void gameMenu_showbuf()
{
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		printf("malloc lcdframe error\r\n");
		return -1;
	}
	lcdframeVideoCfg(lcdFrame, 0, 0, gamemenu_bg_img.width, gamemenu_bg_img.height);
	
	memcpy(lcdFrame->y_addr, gamemenu_show_buf, gamemenu_bg_img.yuvbuf_len);
	
	ax32xx_sysDcacheFlush((u32)lcdFrame->y_addr,lcdFrame->w*lcdFrame->h*3/2);
	int cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}
	lcdDealVideBuff(lcdFrame);
	osal_task_sleep(60);

}

static void gameMenuUpdateIcon(s8 dir)
{

	u16 id_w=(GAMEMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	u16 id_h=(GAMEMENU_PIXEL_H+0x1)&(~0x1);

	u16 bfward_w=(GAMEMENU_BFWARD_W+0x1)&(~0x1);// bmp must 2pixel align
	u16 bfward_h=(GAMEMENU_BFWARD_H+0x1)&(~0x1);


	if(dir==0)//静止
	{
	//画静止(动画归位)
	{
		//画背景
		memcpy(gamemenu_show_buf,gamemenu_bg_img.yuvbuf_ptr, gamemenu_bg_img.yuvbuf_len);
		
		//画game图标
		yuv420_draw_buf(gamemenu_show_buf, TFT_WIDTH, TFT_HEIGHT, 100, 60, id_w, id_h,
			gamemenu_icon_img[game_idx].yuvbuf_ptr, id_w, id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
		//画左右方向的图标
		yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
			gamemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
			gamemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
	}
	}
	else
	{
		u8 next_game_idx;
		
		if(dir<0 )//左移
		{
			if(game_idx<(GAME_NUM-1))
			{
				next_game_idx=game_idx+1;
			}
			else
			{
				next_game_idx=0;
			}
		}
		else//右移
		{
			if(game_idx>0)
			{
				next_game_idx=game_idx-1;
			}
			else
			{
				next_game_idx=GAME_NUM-1;
			}
		}
		
		u16 x,step;
		for(step=0;step<11;step+=3)
		{

			//画背景
			memcpy(gamemenu_show_buf,gamemenu_bg_img.yuvbuf_ptr,gamemenu_bg_img.yuvbuf_len);

			//画game图标
			if(dir<0)//左移
			{
				//==draw game cur==
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,100-step*20,60,id_w,id_h,
					gamemenu_icon_img[game_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,100+220-step*20,60,id_w,id_h,
					gamemenu_icon_img[next_game_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				//==draw game cur==
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,100+step*20,60,id_w,id_h,
					gamemenu_icon_img[game_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,step*20-120,60,id_w,id_h,
					gamemenu_icon_img[next_game_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			
			//画左右方向的图标
			if(dir<0)//左移
			{
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					gamemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					gamemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					gamemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(gamemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					gamemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}

//			osal_task_sleep(20);
			gameMenu_showbuf();
		}
	}

	
}


static int gameMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
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


static int gameMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	msgGameFunc gameList[] = {game_snake_entry,game_maze_entry, game_sokoban_entry, game_tetris_entry,game_block_entry};
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("game idx=%d, starting it...", game_idx);
		gameEntry = gameList[game_idx];
		if(NULL == gameEntry)
			return 0;

		functionStart(FUNCTION_GAME,0);

	}
	return 0;
}

static int gameMenuKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
//		gameMenu_show_move(1);
		gameMenuUpdateIcon(1);
		gameMenu_showbuf();

		if(game_idx>0)
		{
			game_idx--;
		}
		else
		{
			game_idx=GAME_NUM-1;
		}
		gameMenuUpdateIcon(0);
		gameMenu_showbuf();
	}
	return 0;
}

static int gameMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
//		gameMenu_show_move(0);
		gameMenuUpdateIcon(-1);
		gameMenu_showbuf();

		if(game_idx<(GAME_NUM-1))
		{
			game_idx++;
		}
		else
		{
			game_idx=0;
		}
		gameMenuUpdateIcon(0);
		gameMenu_showbuf();
	}
	return 0;
}

static int gameMenuKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}

static int gameMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("game menu Open Win %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	u8 i;

	u16 lcdW=0,lcdH=0;
	//lcdGetRes(&lcdW,&lcdH);
	gamemenu_show_buf=hal_sysMemMalloc(TFT_WIDTH*TFT_HEIGHT*3/2,32);
	if(NULL==gamemenu_show_buf)
	{
		printf("malloc fail %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
		return -1;
	}

	img_load(&gamemenu_bg_img, RES_GAMEMENU_BK, 1, TFT_WIDTH, TFT_HEIGHT, 0);

	img_load(&gamemenu_forward, RES_ICON_FORWARD, 0, GAMEMENU_BFWARD_W, GAMEMENU_BFWARD_H, 0);
	img_load(&gamemenu_backward, RES_ICON_BACKWARD, 0, GAMEMENU_BFWARD_W, GAMEMENU_BFWARD_H, 0);
	for(i=0; i<GAME_NUM; i++)
	{
		img_load(&gamemenu_icon_img[i], gamemenu_icon_id[i], 0, GAMEMENU_PIXEL_W, GAMEMENU_PIXEL_H, 0);
	}

//	game_idx=0;
	gameMenuUpdateIcon(0);
	gameMenu_showbuf();
	return 0;
}

static int gameMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("game menu Close Win!!!\n");

	img_release(&gamemenu_bg_img);
	img_release(&gamemenu_forward);
	img_release(&gamemenu_backward);
	u8 i;
	for(i=0; i<GAME_NUM; i++)
	{
		img_release(&gamemenu_icon_img[i]);
	}
	if(NULL!=gamemenu_show_buf)
	{
		hal_sysMemFree_sdr(gamemenu_show_buf);
		//lcdframeFree((disp_frame_t *)gamemenu_bk_buf);
		gamemenu_show_buf=NULL;
	}
	return 0;
}

static int gameMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int gameMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int gameMenuMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU3,0);
	}
	return 0;
}

msgDealInfor gameMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,gameMenuOpenWin},
	{SYS_CLOSE_WINDOW,gameMenuCloseWin},
	{KEY_EVENT_MODE,  gameMenuKeyMsgMode},
	{KEY_EVENT_MENU,gameMenuKeyMsgMenu},
	{KEY_EVENT_OK,gameMenuKeyMsgOk},
	{KEY_EVENT_UP,gameMenuKeyMsgExit},
	{KEY_EVENT_DOWN,gameMenuKeyMsgExit},
	{KEY_EVENT_POWER,gameMenuMsgPower},
	{SYS_EVENT_500MS,gameMenuSysMsg500MS},
	{SYS_EVENT_1S,gameMenuSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(gameMenuWindow,gameMenuMsgDeal,gameMenuWin)

#endif
