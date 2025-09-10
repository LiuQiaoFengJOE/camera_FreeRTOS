#include "app.h"
#include "drv/lcd/lcdframe.h"
#include "touchPanel.h"

#include "funcCreativeToolsRes.c"


#define CREATIVETOOLS_TASK_NUM 		2

static u8 show_bk;
static u32 CreativeTools_show_time;

static u8* CreativeTools_bk_buf=NULL;
static u16 bk_w,bk_h;

static u32 CreativeTools_jgp_id[CREATIVETOOLS_TASK_NUM]={RES_CREATIVETOOLS_BK_1,RES_CREATIVETOOLS_BK_2};
static u32 CreativeTools_str_id[CREATIVETOOLS_TASK_NUM]={R_ID_STR_SET_PLAY,R_ID_STR_PHOTO_SCORE};

static u8 CreativeTools_state;



static void CreativeTools_showbuf(void)	 
{
#if 1
	disp_frame_t *p_lcd_buffer;
	INT16U screenW=0,screenH=0;
	INT16U lcdW=0,lcdH=0,cnt;
	
	p_lcd_buffer = lcdVideoFrameMalloc();
	if(p_lcd_buffer==NULL)
	{
		printf("malloc lcdframe error\r\n");
		return;
	}
	lcdGetRes(&screenW, &screenH);
	lcdGetVideoRes(&lcdW,&lcdH);
	lcdframeVideoCfg(p_lcd_buffer,(screenW-lcdW)/2,(screenH-lcdH)/2,lcdW,lcdH);
	memcpy(p_lcd_buffer->y_addr,CreativeTools_bk_buf,bk_w*bk_h*3/2);
	dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
	lcdDealVideBuff(p_lcd_buffer);
#else	
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		memcpy(p_lcd_buffer->y_addr,CreativeTools_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
#endif	
}

static int CreativeToolsKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(CreativeTools_state==0)
		{
			//taskStart(TASK_PLAY_BACK,1);
			functionStart(FUNCTION_PLAY_BACK,1);
		}
		else
		{
			//taskStart(TASK_PHOTO_ENCODE,2);
			functionStart(FUNCTION_PHOTO_ENCODE,0);
		}
	}
	return 0;
}
static int CreativeToolsKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(CreativeTools_state==0)
		{
			CreativeTools_state=1;
			
		}
		else
		{
			CreativeTools_state=0;
		}
		
		if(NULL!=CreativeTools_bk_buf)
		{
			jpgdecById(CreativeTools_jgp_id[CreativeTools_state], CreativeTools_bk_buf,modeFreeSpace());
		}
		osal_task_sleep(30);
		CreativeTools_showbuf();
		CreativeTools_show_time=os_time_get();
		show_bk=1;
		
		winSetResid(winItem(handle,VCREATIVETOOLS_STR_ID),CreativeTools_str_id[CreativeTools_state]);
	}
	return 0;
}
static int CreativeToolsKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(CreativeTools_state==0)
		{
			CreativeTools_state=1;
		}
		else
		{
			CreativeTools_state=0;
		}
		
		if(NULL!=CreativeTools_bk_buf)
		{
			jpgdecById(CreativeTools_jgp_id[CreativeTools_state], CreativeTools_bk_buf,modeFreeSpace());
		}
		osal_task_sleep(30);
		CreativeTools_showbuf();
		CreativeTools_show_time=os_time_get();
		show_bk=1;
		
		winSetResid(winItem(handle,VCREATIVETOOLS_STR_ID),CreativeTools_str_id[CreativeTools_state]);
	}
	return 0;
}

static int CreativeToolsKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		
	}
	return 0;
}
static int CreativeToolsKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	
	}
	return 0;
}
static int  CreativeToolsEditKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		printf("functionStart:FUNCTION_MAINMENU\n");
		functionStart(FUNCTION_MAINMENU,0);
		//winDestroy(&handle);
		//uiOpenWindow(&mainMenuWindow,0);
		/*taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}*/
	}
	return 0;
}

static int CreativeToolsOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("video Open Win:%d\n",os_time_get());
	
	//==init==
	lcdGetVideoRes(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment

	CreativeTools_bk_buf = getCurModeFreeBuffBase();
	jpgdecById(RES_CREATIVETOOLS_BK_0, CreativeTools_bk_buf,modeFreeSpace());

//	SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
	//hal_dacSetVolume(SysCtrl.volume_level*10);

	osal_task_sleep(30);
	CreativeTools_showbuf();
	CreativeTools_state=0;

	return 0;
}

static int CreativeToolsCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("CreativeTools Close Win!!!\n");
	return 0;
}

static int CreativeToolsTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/
	if(parameNum!=3)
	{
		printf(" CreativeToolsWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	
	
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==VCREATIVETOOLS_EXE_MAIN_ID){
			functionStart(FUNCTION_MAINMENU,0);
		}
	}
	return 0;
}
static int CreativeToolsSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(os_time_get()<CreativeTools_show_time+500)	// too short not show it
	{
		return 0;
	}

	if(show_bk)
	{
		jpgdecById(RES_CREATIVETOOLS_BK_0, CreativeTools_bk_buf,modeFreeSpace());
		show_bk=0;
	}
	else
	{
		jpgdecById(CreativeTools_jgp_id[CreativeTools_state], CreativeTools_bk_buf,modeFreeSpace());
		show_bk=1;
		
	}
	CreativeTools_showbuf();
	CreativeTools_show_time=os_time_get();
	return 0;
}
static int CreativeToolsSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
msgDealInfor CreativeToolsMsgDeal[]=
{
	{SYS_OPEN_WINDOW,CreativeToolsOpenWin},
	{SYS_CLOSE_WINDOW,CreativeToolsCloseWin},
	{SYS_TOUCH_WINDOW,CreativeToolsTouchWin},
	//{KEY_EVENT_UP,CreativeToolsKeyMsgUp},
	//{KEY_EVENT_DOWN,CreativeToolsKeyMsgDown},
	{KEY_EVENT_LEFT,CreativeToolsKeyMsgUp},
	{KEY_EVENT_RIGHT,CreativeToolsKeyMsgDown},
	//{KEY_EVENT_POWER,CreativeToolsKeyMsgUp/*CreativeToolsEditKeyMsgExit*/},
	{KEY_EVENT_OK,CreativeToolsKeyMsgOk},
	{SYS_EVENT_500MS,CreativeToolsSysMsg500MS},
	{SYS_EVENT_1S,CreativeToolsSysMsg1S},
	{EVENT_MAX,NULL},
};
WINDOW(CreativeToolsWindow,CreativeToolsMsgDeal,CreativeToolsWin)

