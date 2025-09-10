#include "app.h"

#include "uiWin.h"
#include"mediaFile/picIdentify.h"

#define TFT_WIDTH		320
#define TFT_HEIGHT		240

static widgetCreateInfor ljfNetTypeMenuWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	widgetEnd(),
};


#include "task/userMedia.h"

#define NTYPE_NUM 	AIPIC_STYLE_MAX
#define MENU_PIXEL_W 		160
#define MENU_PIXEL_H 		200
#define MENU_BFWARD_W 	48
#define MENU_BFWARD_H 	32

static res_img_t menu_bg_img;
static res_img_t menu_backward;
static res_img_t menu_forward;
static res_img_t menu_icon_img[NTYPE_NUM];

static u32 menu_icon_id[NTYPE_NUM]={
	RES_NETTYPE0,
	RES_NETTYPE1,
	RES_NETTYPE2,
	RES_NETTYPE3,
	RES_NETTYPE4,
	RES_NETTYPE5,
	RES_NETTYPE6,
	RES_NETTYPE7,	
	RES_NETTYPE8
};

static u8 ntype_idx=0;
static u8* menu_show_buf;


#define YUV_ALPHA_Y	    0x8A
#define YUV_ALPHA_UV	0x81

#include "drv/lcd/lcdframe.h"

static void ljfNetTypeMenu_showbuf()
{
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		printf("malloc lcdframe error\r\n");
		return -1;
	}
	lcdframeVideoCfg(lcdFrame, 0, 0, menu_bg_img.width, menu_bg_img.height);
	
	memcpy(lcdFrame->y_addr, menu_show_buf, menu_bg_img.yuvbuf_len);
	
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

static void ljfNetTypeMenuUpdateIcon(s8 dir)
{

	u16 id_w=(MENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	u16 id_h=(MENU_PIXEL_H+0x1)&(~0x1);

	u16 bfward_w=(MENU_BFWARD_W+0x1)&(~0x1);// bmp must 2pixel align
	u16 bfward_h=(MENU_BFWARD_H+0x1)&(~0x1);


	if(dir==0)//静止
	{
	//画静止(动画归位)
	{
		//画背景
		memcpy(menu_show_buf,menu_bg_img.yuvbuf_ptr, menu_bg_img.yuvbuf_len);
		
		//画ntype图标
		yuv420_draw_buf(menu_show_buf, TFT_WIDTH, TFT_HEIGHT, 80, 20, id_w, id_h,
			menu_icon_img[ntype_idx].yuvbuf_ptr, id_w, id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
		//画左右方向的图标
		yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
			menu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
			menu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
	}
	}
	else
	{
		u8 next_ntype_idx;
		
		if(dir<0 )//左移
		{
			if(ntype_idx<(NTYPE_NUM-1))
			{
				next_ntype_idx=ntype_idx+1;
			}
			else
			{
				next_ntype_idx=0;
			}
		}
		else//右移
		{
			if(ntype_idx>0)
			{
				next_ntype_idx=ntype_idx-1;
			}
			else
			{
				next_ntype_idx=NTYPE_NUM-1;
			}
		}
		
		u16 x,step;
		for(step=0;step<10;step+=3)
		{

			//画背景
			memcpy(menu_show_buf,menu_bg_img.yuvbuf_ptr,menu_bg_img.yuvbuf_len);

			//画ntype图标
			if(dir<0)//左移
			{
				//==draw ntype cur==
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,80-step*24,20,id_w,id_h,
					menu_icon_img[ntype_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw ntype next==
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,80+240-step*24,20,id_w,id_h,
					menu_icon_img[next_ntype_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				//==draw ntype cur==
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,80+step*24,20,id_w,id_h,
					menu_icon_img[ntype_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw ntype next==
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,step*24-160,20,id_w,id_h,
					menu_icon_img[next_ntype_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			
			//画左右方向的图标
			if(dir<0)//左移
			{
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					menu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					menu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					menu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(menu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					menu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}

//			osal_task_sleep(20);
			ljfNetTypeMenu_showbuf();
		}
	}

	
}


static int ljfNetTypeMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
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


static int ljfNetTypeMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		SysCtrl.ainetType=ntype_idx;
		printf("ntype idx=%d, starting it...", SysCtrl.ainetType);
        if(SysCtrl.ainetType>=AIPIC_STYLE_MAX)
            SysCtrl.ainetType = AIPIC_STYLE_WATERCOLORPINK;
		
		if (SysCtrl.ainetType == 0)
		{
			SysCtrl.photoMode=PHOTO_LIFTOON;
			functionStart(FUNCTION_TAKE_PHOTO,0);
			return 0;
		}		
		functionStart(FUNCTION_TAKE_PHOTO,0);
	}
	return 0;
}

static int ljfNetTypeMenuKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		ljfNetTypeMenuUpdateIcon(1);
		ljfNetTypeMenu_showbuf();

		if(ntype_idx>0)
		{
			ntype_idx--;
		}
		else
		{
			ntype_idx=NTYPE_NUM-1;
		}
		ljfNetTypeMenuUpdateIcon(0);
		ljfNetTypeMenu_showbuf();
	}
	return 0;
}

static int ljfNetTypeMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		ljfNetTypeMenuUpdateIcon(-1);
		ljfNetTypeMenu_showbuf();

		if(ntype_idx<(NTYPE_NUM-1))
		{
			ntype_idx++;
		}
		else
		{
			ntype_idx=0;
		}
		ljfNetTypeMenuUpdateIcon(0);
		ljfNetTypeMenu_showbuf();
	}
	return 0;
}

static int ljfNetTypeMenuKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU2,0);
	}
	return 0;
}

static int ljfNetTypeMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("ntype menu Open Win %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	u8 i;

	u16 lcdW=0,lcdH=0;
	//lcdGetRes(&lcdW,&lcdH);
    if(SysCtrl.ainetType>=AIPIC_STYLE_MAX)
        SysCtrl.ainetType = AIPIC_STYLE_WATERCOLORPINK;
	printf("ntype idx=%d", SysCtrl.ainetType);
	ntype_idx = SysCtrl.ainetType;

	menu_show_buf=osal_malloc(TFT_WIDTH*TFT_HEIGHT*3/2);
	if(NULL==menu_show_buf)
	{
		printf("malloc fail %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
		return -1;
	}
	
	u8 *memBase = ((u32)getCurModeFreeBuffBase()+256)&(~0x3f);
	printf ("==modemem base:0x%x, size:%d\n", memBase, modeFreeSpace());
	img_load(&menu_bg_img, RES_AIBACK, 1, TFT_WIDTH, TFT_HEIGHT, memBase);
	memBase += menu_bg_img.yuvbuf_len;
	
	img_load(&menu_forward, RES_ICON_FORWARD, 0, MENU_BFWARD_W, MENU_BFWARD_H, memBase);
	memBase += menu_forward.yuvbuf_len;
	
	img_load(&menu_backward, RES_ICON_BACKWARD, 0, MENU_BFWARD_W, MENU_BFWARD_H, memBase);
	memBase += menu_backward.yuvbuf_len;
	
	for(i=0; i<NTYPE_NUM; i++)
	{
		img_load(&menu_icon_img[i], menu_icon_id[i], 0, MENU_PIXEL_W, MENU_PIXEL_H, memBase);
		memBase += menu_icon_img[i].yuvbuf_len;
	}
	printf ("=@=modemem base:0x%x\n", memBase);

	ljfNetTypeMenuUpdateIcon(0);
	ljfNetTypeMenu_showbuf();
	
	return 0;
}

static int ljfNetTypeMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("ntype menu Close Win!!!\n");

	img_release(&menu_bg_img);
	img_release(&menu_forward);
	img_release(&menu_backward);
	u8 i;
	for(i=0; i<NTYPE_NUM; i++)
	{
		img_release(&menu_icon_img[i]);
	}
	if(NULL!=menu_show_buf)
	{
		osal_free(menu_show_buf);
		menu_show_buf=NULL;
	}
	return 0;
}

static int ljfNetTypeMenuMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU2,0);
	}
	return 0;
}

/*static int ljfNetTypeMenuMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if (SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		if(configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)
		{
			wifi_start(1);
		}
	}

	return 0;
}*/

msgDealInfor ljfNetTypeMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,ljfNetTypeMenuOpenWin},
	{SYS_CLOSE_WINDOW,ljfNetTypeMenuCloseWin},
	{KEY_EVENT_LEFT,  ljfNetTypeMenuKeyMsgMode},
	{KEY_EVENT_RIGHT,ljfNetTypeMenuKeyMsgMenu},
	{KEY_EVENT_OK,ljfNetTypeMenuKeyMsgOk},
	{KEY_EVENT_MODE,ljfNetTypeMenuKeyMsgExit},
	{KEY_EVENT_SETTING,ljfNetTypeMenuKeyMsgExit},
	{KEY_EVENT_MODE,ljfNetTypeMenuMsgPower},
	//{SYS_EVENT_SDC,ljfNetTypeMenuMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(ljfNetTypeMenuWindow,ljfNetTypeMenuMsgDeal,ljfNetTypeMenuWin)

