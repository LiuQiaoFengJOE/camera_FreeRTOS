#include "app.h"
#include "drv/lcd/lcdframe.h"
#include "touchPanel.h"
#include "mediaFile/userInterface.h"
#include "funMainMenu3Res.c"

#define MAINMENU_TASK_NUM 		3
#define MAINMENU_PIXEL_W 		96
#define MAINMENU_PIXEL_H 		96
extern menu MENU(movie);
extern void jpgdecById(u32 id,u8* buf/*,u32 size*/);
static void mainMenuIdxBuf(void);
static void mainMenu_showbuf(u8 bk);	// bk  0: show idx , 1: show bk 


#define UI_JPG 		1
static u32 mainmenu_show_time;
static u8 show_bk;
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU3_PLAYBACK,RES_MAINMENU3_GAME, RES_MAINMENU3_SETTING};

static u8* mainmenu_bk_buf;
static u16 bk_w,bk_h;

static u8* mainmenu_id_buf[MAINMENU_TASK_NUM]= {NULL};
static u16 id_w,id_h;
//static u8 curIndex = 0xff,preIndex = 0xff;
static u8 mainmenu_taskidx=0;
static u8 jpgdecFlag;

typedef	struct
{
	u16 x;
	u16 y; 
}MainMenu_Pos;
#if 0
void jpgdecById(u32 id,u8* buf/*,u32 size*/)
{
	u32 id_addr = res_open(id);
	uint32_t jpgbufsize = res_size(id);
	INT16U lcdW=0,lcdH=0,cnt;
	
	if(buf==NULL)
	{
		return;
	}
	lcdGetVideoRes(&lcdW,&lcdH);
	//jpgbufsize += 64;
	/*if(size<lcdW*lcdH*3/2)
	{
		printf("jpgdecById err,no memory,%dKB\n",size>>10);
		return ;
	}*/
	printf("jpgbufsize:%d,lcdw:%d,lcdh:%d\r\n", jpgbufsize,lcdW,lcdH);
	void *jpgbuf = osal_malloc(jpgbufsize+64);
	if(jpgbuf==NULL)
	{
		printf("malloc jpgbuf:%d error\r\n", jpgbufsize);
		return;
	}
	res_read(id_addr, jpgbuf, jpgbufsize);
	dcache_writeback_region((u32)jpgbuf, jpgbufsize);
	not_isr_jpgdec2(buf, jpgbuf, jpgbufsize, lcdW, lcdH);
	osal_free(jpgbuf);
}
#endif
static void mainMenu_showbuf(u8 bk)	// bk  0: show idx , 1: show bk 
{
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
	if(0==bk)
	{
	#if UI_JPG
		memcpy(p_lcd_buffer->y_addr,mainmenu_id_buf[mainmenu_taskidx],id_w*id_h*3/2);
	#else
		memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,mainMenu_showpos[mainmenu_taskidx].x,mainMenu_showpos[mainmenu_taskidx].y,id_w,id_h,mainmenu_id_buf[mainmenu_taskidx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
	#endif
		dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
	}
	else
	{
		memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
		dcache_writeback_region((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
	}	
	/*cnt=0;
	while(lcdDeBusy())
	{
		osal_task_sleep(10);
		if(cnt++>=5)
			break;
	}*/
	lcdDealVideBuff(p_lcd_buffer);
}




static int mainMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
#if 1	
		mainMenuStrShow(handle,false);
		//drawUIService(true);		// clean str

#if 0		
		mainmenu_taskidx++;
		if (mainmenu_taskidx>=MAINMENU_TASK_NUM)
			mainmenu_taskidx = 0;
		
		show_bk=0;
		mainMenuIdxBuf();
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=os_time_get();		
#else		
		if(0==mainmenu_taskidx)
		{
			functionStart(FUNCTION_PLAY_BACK,0);
		}
		else if(1==mainmenu_taskidx)
		{
			functionStart(FUNCTION_AUDIO_PLAYER,0);
			//functionStart(FUNCTION_GAME_MENU,0);
		}
		else if(2==mainmenu_taskidx)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
		/*else if(3==mainmenu_taskidx)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}*/
#endif
		printf("id_idx:%d\n",mainmenu_taskidx);
#endif
	}
	return 0;
}



static void mainMenuIdxBuf(void)
{
#if 0
	if(preIndex != mainmenu_taskidx && curIndex !=  mainmenu_taskidx){
		if(preIndex!=0xff){
			//hal_sysMemFree(mainmenu_id_buf[preIndex]);	
		}
		preIndex = curIndex;
		curIndex = mainmenu_taskidx;
		jpgdecById(mainmenu_bmp_id[mainmenu_taskidx],mainmenu_id_buf[mainmenu_taskidx], modeFreeSpace()-(mainmenu_taskidx+1)*bk_w*bk_h*3/2);
	}else{
		if(preIndex == mainmenu_taskidx){
			preIndex = curIndex;
			curIndex = mainmenu_taskidx;
		}else{
			return;
		}
	}
#else
	if ((jpgdecFlag&(1<<mainmenu_taskidx)) == 0)
	{
		jpgdecById(mainmenu_bmp_id[mainmenu_taskidx],mainmenu_id_buf[mainmenu_taskidx]);
		jpgdecFlag |= (1<<mainmenu_taskidx);
		printf("==%s,%d, taskidx:%d�� jpgdecFlag:0x%x\n", __FUNCTION__, __LINE__, mainmenu_taskidx, jpgdecFlag);
	}
#endif	
	return;
}


static int mainMenuKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
#if 1
		functionStart(FUNCTION_MAINMENU2,1);
#else		
		if(mainmenu_taskidx>2)
		{
			mainmenu_taskidx-=3;
		}
		else
		{
			mainmenu_taskidx+=3;
		}
		printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, mainmenu_taskidx);
		show_bk=0;
		mainMenuIdxBuf();
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=os_time_get();
#endif
	}
	return 0;
}

static int mainMenuKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
#if 1
		functionStart(FUNCTION_MAINMENU,2);
#else		
		if(mainmenu_taskidx>2)
		{
			mainmenu_taskidx-=3;
		}
		else
		{
			mainmenu_taskidx+=3;
		}
		printf("==%s,%d, taskidx:%d\n", __FUNCTION__, __LINE__, mainmenu_taskidx);
		show_bk=0;
		mainMenuIdxBuf();
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=os_time_get();
#endif		
	}
	return 0;
}

static int mainMenuKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(mainmenu_taskidx>0)
		{
			mainmenu_taskidx--;
		}
		else
		{
			mainmenu_taskidx=MAINMENU_TASK_NUM-1;;
		}
		show_bk=0;
		mainMenuIdxBuf();
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=os_time_get();
	}
	return 0;
}
static int mainMenuKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(mainmenu_taskidx >= MAINMENU_TASK_NUM-1)
		{
			mainmenu_taskidx=0;
		}
		else
		{
			mainmenu_taskidx++;
		}
		show_bk=0;
		mainMenuIdxBuf();
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=os_time_get();
	}
	return 0;
}

static int mainMenusysMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));
	}
	return 0;
}

static int mainMenusysMsgAIInit(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
	if(parameNum==1)
		keyState = parame[0];
	if(keyState==KEY_PRESSED){
        const uint32 timeout_ms = 30000; // 10 秒超时
        const uint32 check_interval_ms = 500; // 每 100ms 检查一次
        uint32 elapsed_ms = 0;

        while (elapsed_ms < timeout_ms)
        {
            if (bt_recWifiConfigData() == 1)
            {
                printf("Bluetooth data received and processed\n");
                return 0; 
            }
            printf("@");
            osal_task_sleep(check_interval_ms);
            elapsed_ms += check_interval_ms;
        }
	}
    return 0;
}


static int mainMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	int i;
	printf("main menu Open Win:%d, modeMem:0x%x, memBase:0x%x\n",os_time_get(), modeFreeSpace(), getCurModeFreeBuffBase());
	jpgdecFlag = 0;
	//curIndex = 0xff;preIndex = 0xff;
	show_bk=1;
	if(mainmenu_taskidx>=MAINMENU_TASK_NUM)
	{
		mainmenu_taskidx=0;
	}
	printf("mainmenu_taskidx=%d\n",mainmenu_taskidx);
	//==init==
	lcdGetVideoRes(&bk_w,&bk_h);
	mainmenu_bk_buf=(getCurModeFreeBuffBase()+0x3f)&(~0x3f);//hal_sysMemMalloc(bk_w*bk_h*3/2,32);

	if(NULL!=mainmenu_bk_buf)
	{
		jpgdecById( RES_MAINMENU3_BK, mainmenu_bk_buf/*,modeFreeSpace()*/);
		for (i=0; i<MAINMENU_TASK_NUM; i++)
		{
			mainmenu_id_buf[i] = mainmenu_bk_buf+(i+1)*bk_w*bk_h*3/2;
		}
	}
	else
	{
		printf("mem err!\n");
		return 0;
	}

	#if UI_JPG
	lcdGetVideoRes(&id_w,&id_h);
	#else
	id_w=(MAINMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	id_h=(MAINMENU_PIXEL_H+0x1)&(~0x1);
	#endif
	mainMenuIdxBuf();
	//==show==
#if 1
{
	//printf("=3==parame[0]:%d\n", parame[0]);
	if (parame[0] == 1)
		animationInit(BOTTOM2UPPER);
	else
		animationInit(UPPER2BOTTOM);
	
	bool change_finir = false;
	while(1){
		disp_frame_t *p_lcd_buffer;

		p_lcd_buffer = (disp_frame_t *)lcdVideoFrameMalloc();//dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			lcdGetRes(&lcd_w,&lcd_h);
			lcdframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,lcd_w*lcd_h*3/2);
			change_finir = animationRun(p_lcd_buffer->y_addr);//changeUserInterface.run(&changeUserInterface, p_lcd_buffer->y_addr);
			dcache_writeback_region((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			lcdDealVideBuff(p_lcd_buffer);
			osal_task_sleep(30);
			if(change_finir == true)
				break;
		}
		else
		{
			printf("video buf null!\n");
		}
	}
}
#else
	mainMenu_showbuf(show_bk);
#endif
	mainmenu_show_time=os_time_get();
	printf("show time:%d\n",os_time_get());

	mainMenuStrShow(handle,false);
	mainMenuBaterryShow(handle);
//	ax32xx_dacVolumeSet((10*SysCtrl.volume_level*0x7fff)/100);

	return 0;
}

static int mainMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("main menu Close Win!!!\n");
	if(NULL!=mainmenu_bk_buf)
	{
		mainmenu_bk_buf=NULL;
	}
	u8 i;
	for(i=0;i<MAINMENU_TASK_NUM;i++)
	{
		if(NULL!=mainmenu_id_buf[i])
		{
			mainmenu_id_buf[i]=NULL;
		}
	}

	return 0;
}

static int mainMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 1
	if(os_time_get()<mainmenu_show_time+300)	// too short not show it
	{
		return 0;
	}

	if(show_bk)
	{
		show_bk=0;
	}
	else
	{
		show_bk=1;
	}
	mainMenu_showbuf(show_bk);
	mainmenu_show_time=os_time_get();
#endif	
	return 0;
}
volatile INT32U temp_a;

static int mainMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{

	/*if(SysCtrl.usb == USB_STAT_DCIN)
	{
		mainMenuBaterryShow(handle);	
	}*/
	return 0;
}

static int mainMenuSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	mainMenuBaterryShow(handle);
	return 0;
}
static int mainMenuSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		mainMenuBaterryShow(handle);
	return 0;
}
static int mainMenusysMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int uiMainTouchWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
parame[0]: widget id;
parame[1]: selected item id(for createItemManage widget)
parame[2]: touch state
*/
	if(parameNum!=3)
	{
		printf("uiMainTouchWin, parame num error %d\n",parameNum);
		return 0;
	}
	printf("ID:%d, item:%d, state:%d\n",parame[0],parame[1],parame[2]);
	
	
	if(parame[2]==TOUCH_RELEASE)
	{
		if(parame[0]==MAIN_ID1){
			mainmenu_taskidx = 0;
			functionStart(FUNCTION_TAKE_PHOTO,0);
		}
		else if(parame[0]==MAIN_ID2){
			mainmenu_taskidx = 1;
			functionStart(FUNCTION_VIDEO_RECORD,0);
		}
		else if(parame[0]==MAIN_ID3){
			mainmenu_taskidx = 2;
			functionStart(FUNCTION_PLAY_BACK,0);
		}
		else if(parame[0]==MAIN_ID4){
			mainmenu_taskidx = 3;
			functionStart(FUNCTION_WIFI,0);
		}
		else if(parame[0]==MAIN_ID5){
			mainmenu_taskidx = 4;
			functionStart(FUNCTION_GAME_MENU,0);
			//uiOpenWindow(&CreativeToolsWindow,0);
		}
		else if(parame[0]==MAIN_ID6){
			mainmenu_taskidx = 5;
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
	}
	return 0;
}
static int uiMainSlidRelease(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum!=1)
	{
		printf("uiMainSlidRelease, parame num error %d\n",parameNum);
		return 0;
	}
	if(parame[0]==SLID_DIRECT_RIGHT||parame[0]==SLID_DIRECT_LEFT)
	{
	/*
		if(uiMain==RES_MAIN1)
			uiMain=RES_MAIN2;
		else
			uiMain=RES_MAIN1;
		imageShow(uiMain);
	*/	
	}

	return 0;
}

msgDealInfor mainMenu3MsgDeal[]=
{
	{SYS_OPEN_WINDOW,mainMenuOpenWin},
	{SYS_CLOSE_WINDOW,mainMenuCloseWin},
	{SYS_TOUCH_WINDOW,uiMainTouchWin},
	{SYS_SLID_RELEASE,uiMainSlidRelease},
	{KEY_EVENT_UP,mainMenuKeyMsgUp},
	{KEY_EVENT_DOWN,mainMenuKeyMsgDown},
	{KEY_EVENT_LEFT,mainMenuKeyMsgLeft},
	{KEY_EVENT_RIGHT,mainMenuKeyMsgRight},	
	{KEY_EVENT_OK,mainMenuKeyMsgOk},
	{KEY_EVENT_SETTING,mainMenusysMsgMenu},
	//{KEY_EVENT_MODE,mainMenusysMsgAIInit},
	{SYS_EVENT_500MS,mainMenuSysMsg500MS},
	{SYS_EVENT_1S,mainMenuSysMsg1S},
	{SYS_EVENT_USB,mainMenuSysMsgUSB},
	{SYS_EVENT_BAT,mainMenuSysMsgBattery},
	{KEY_EVENT_POWER,mainMenusysMsgPower},
	{EVENT_MAX,NULL},
};

WINDOW(mainMenu3Window,mainMenu3MsgDeal,mainMenu3Win)



