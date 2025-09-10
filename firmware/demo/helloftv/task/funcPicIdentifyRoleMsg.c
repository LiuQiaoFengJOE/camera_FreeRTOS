#include "app.h"

#include "uiWin.h"
#include"mediaFile/picIdentify.h"

#define TFT_WIDTH		320
#define TFT_HEIGHT		240

static widgetCreateInfor roleMenuWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	widgetEnd(),
};


#include "task/userMedia.h"

#define ROLE_NUM 	PICID_ROLE_MAX-1
#define ROLEMENU_PIXEL_W 		160
#define ROLEMENU_PIXEL_H 		200
#define ROLEMENU_BFWARD_W 	48
#define ROLEMENU_BFWARD_H 	32

static res_img_t rolemenu_bg_img;
static res_img_t rolemenu_backward;
static res_img_t rolemenu_forward;
static res_img_t rolemenu_icon_img[ROLE_NUM];

static u32 rolemenu_icon_id[ROLE_NUM]={
	RES_ROLEDOCTOR,
	RES_ROLEPOET,
	RES_ROLESTORYTELLING,
	RES_ROLETEACHER,
	//RES_ROLEBREVITY
};

static u8 role_idx=0;
static u8* rolemenu_show_buf;


#define YUV_ALPHA_Y	    0x8A
#define YUV_ALPHA_UV	0x81

#include "drv/lcd/lcdframe.h"

static void roleMenu_showbuf()
{
	disp_frame_t *lcdFrame=lcdVideoFrameMalloc();
	if(lcdFrame==NULL)
	{
		printf("malloc lcdframe error\r\n");
		return -1;
	}
	lcdframeVideoCfg(lcdFrame, 0, 0, rolemenu_bg_img.width, rolemenu_bg_img.height);
	
	memcpy(lcdFrame->y_addr, rolemenu_show_buf, rolemenu_bg_img.yuvbuf_len);
	
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

static void roleMenuUpdateIcon(s8 dir)
{

	u16 id_w=(ROLEMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	u16 id_h=(ROLEMENU_PIXEL_H+0x1)&(~0x1);

	u16 bfward_w=(ROLEMENU_BFWARD_W+0x1)&(~0x1);// bmp must 2pixel align
	u16 bfward_h=(ROLEMENU_BFWARD_H+0x1)&(~0x1);


	if(dir==0)//静止
	{
	//画静止(动画归位)
	{
		//画背景
		memcpy(rolemenu_show_buf,rolemenu_bg_img.yuvbuf_ptr, rolemenu_bg_img.yuvbuf_len);
		
		//画role图标
		yuv420_draw_buf(rolemenu_show_buf, TFT_WIDTH, TFT_HEIGHT, 80, 20, id_w, id_h,
			rolemenu_icon_img[role_idx].yuvbuf_ptr, id_w, id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
		//画左右方向的图标
		yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
			rolemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
			rolemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
	}
	}
	else
	{
		u8 next_role_idx;
		
		if(dir<0 )//左移
		{
			if(role_idx<(ROLE_NUM-1))
			{
				next_role_idx=role_idx+1;
			}
			else
			{
				next_role_idx=0;
			}
		}
		else//右移
		{
			if(role_idx>0)
			{
				next_role_idx=role_idx-1;
			}
			else
			{
				next_role_idx=ROLE_NUM-1;
			}
		}
		
		u16 x,step;
		for(step=0;step<10;step+=3)
		{

			//画背景
			memcpy(rolemenu_show_buf,rolemenu_bg_img.yuvbuf_ptr,rolemenu_bg_img.yuvbuf_len);

			//画role图标
			if(dir<0)//左移
			{
				//==draw role cur==
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,80-step*24,20,id_w,id_h,
					rolemenu_icon_img[role_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw role next==
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,80+240-step*24,20,id_w,id_h,
					rolemenu_icon_img[next_role_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				//==draw role cur==
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,80+step*24,20,id_w,id_h,
					rolemenu_icon_img[role_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw role next==
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,step*24-160,20,id_w,id_h,
					rolemenu_icon_img[next_role_idx].yuvbuf_ptr,id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			
			//画左右方向的图标
			if(dir<0)//左移
			{
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					rolemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					rolemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else//右移
			{
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,32,110,bfward_w,bfward_h,
					rolemenu_backward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(rolemenu_show_buf,TFT_WIDTH,TFT_HEIGHT,320-80,110,bfward_w,bfward_h,
					rolemenu_forward.yuvbuf_ptr,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}

//			osal_task_sleep(20);
			roleMenu_showbuf();
		}
	}

	
}


static int roleMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
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


static int roleMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		SysCtrl.aiuserRole=role_idx+1;
		printf("role idx=%d, starting it...", SysCtrl.aiuserRole);
        if(SysCtrl.aiuserRole>=PICID_ROLE_MAX)
            SysCtrl.aiuserRole = PICID_ROLE_DOCTOR;
		functionStart(FUNCTION_TAKE_PHOTO,0);
	}
	return 0;
}

static int roleMenuKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		roleMenuUpdateIcon(1);
		roleMenu_showbuf();

		if(role_idx>0)
		{
			role_idx--;
		}
		else
		{
			role_idx=ROLE_NUM-1;
		}
		roleMenuUpdateIcon(0);
		roleMenu_showbuf();
	}
	return 0;
}

static int roleMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		roleMenuUpdateIcon(-1);
		roleMenu_showbuf();

		if(role_idx<(ROLE_NUM-1))
		{
			role_idx++;
		}
		else
		{
			role_idx=0;
		}
		roleMenuUpdateIcon(0);
		roleMenu_showbuf();
	}
	return 0;
}

static int roleMenuKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
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

static int roleMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("role menu Open Win %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
	u8 i;

	u16 lcdW=0,lcdH=0;
	//lcdGetRes(&lcdW,&lcdH);
	rolemenu_show_buf=osal_malloc(TFT_WIDTH*TFT_HEIGHT*3/2);
	if(NULL==rolemenu_show_buf)
	{
		printf("malloc fail %d,PortGetFreeHeapSize:%d\r\n", __LINE__, xPortGetFreeHeapSize());
		return -1;
	}
	u8 *memBase = ((u32)getCurModeFreeBuffBase()+256)&(~0x3f);
	img_load(&rolemenu_bg_img, RES_AIBACK, 1, TFT_WIDTH, TFT_HEIGHT, memBase);
	memBase += rolemenu_bg_img.yuvbuf_len;
	
	img_load(&rolemenu_forward, RES_ICON_FORWARD, 0, ROLEMENU_BFWARD_W, ROLEMENU_BFWARD_H, memBase);
	memBase += rolemenu_forward.yuvbuf_len;
	
	img_load(&rolemenu_backward, RES_ICON_BACKWARD, 0, ROLEMENU_BFWARD_W, ROLEMENU_BFWARD_H, memBase);
	memBase += rolemenu_backward.yuvbuf_len;
	for(i=0; i<ROLE_NUM; i++)
	{
		img_load(&rolemenu_icon_img[i], rolemenu_icon_id[i], 0, ROLEMENU_PIXEL_W, ROLEMENU_PIXEL_H, memBase);
		memBase += rolemenu_icon_img[i].yuvbuf_len;
	}

	roleMenuUpdateIcon(0);
	roleMenu_showbuf();
	
	return 0;
}

static int roleMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("role menu Close Win!!!\n");

	img_release(&rolemenu_bg_img);
	img_release(&rolemenu_forward);
	img_release(&rolemenu_backward);
	u8 i;
	for(i=0; i<ROLE_NUM; i++)
	{
		img_release(&rolemenu_icon_img[i]);
	}
	if(NULL!=rolemenu_show_buf)
	{
		osal_free(rolemenu_show_buf);
		rolemenu_show_buf=NULL;
	}
	return 0;
}

static int roleMenuMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		functionStart(FUNCTION_MAINMENU,0);
	}
	return 0;
}

/*static int roleMenuMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if (SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		if(configGet(CONFIG_ID_PICIDENTIFY)==R_ID_STR_COM_ON)
		{
			wifi_start(1);
		}
	}

	return 0;
}
*/
msgDealInfor roleMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,roleMenuOpenWin},
	{SYS_CLOSE_WINDOW,roleMenuCloseWin},
	{KEY_EVENT_LEFT,  roleMenuKeyMsgMode},
	{KEY_EVENT_RIGHT,roleMenuKeyMsgMenu},
	{KEY_EVENT_OK,roleMenuKeyMsgOk},
	{KEY_EVENT_MODE,roleMenuKeyMsgExit},
	{KEY_EVENT_SETTING,roleMenuKeyMsgExit},
	{KEY_EVENT_MODE,roleMenuMsgPower},
	//{SYS_EVENT_SDC,roleMenuMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(roleMenuWindow,roleMenuMsgDeal,roleMenuWin)

