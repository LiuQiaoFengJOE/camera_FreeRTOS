#include "app.h"
#include "funAppRes.c"

#include "drv/lcd/lcdframe.h"
#include "touchPanel.h"
#include "mediaFile/userInterface.h"
#include"mediaFile/picIdentify.h"

extern menu MENU(movie);

struct BtDeploy{
	OSAL_TASK	App_task_ctrl;
	TASK_HDL    App_task_hdl_ctrl;
	int 		stat;

};

static void App_showbuf(u8 bk);	// bk  0: show idx , 1: show bk 
extern void jpgdecById(u32 id,u8* buf);

static u8 show_bk;
static u8* app_bk_buf;
static u16 bk_w,bk_h;   
static u8 jpgdecFlag;
static u8 statcnt;
static struct BtDeploy Deploy_ctrl_t;

void Bt_connect_task(void)
{
	while(1)
	{
		if(Deploy_ctrl_t.stat==APP_DEPLOY_CONNECTING)
		{
			Deploy_ctrl_t.stat = bt_recWifiConfigData();  
			printf("Deploy_ctrl_t.stat:%d\r\n",Deploy_ctrl_t.stat);
			switch(Deploy_ctrl_t.stat)
			{
				case APP_DEPLOY_CONNECTING:
					break;
				case APP_DEPLOY_SUCCESS:
					printf("Bluetooth data received and processed %d\n",Deploy_ctrl_t.stat);
					break;
				default:
					printf("Bluetooth data received and processed %d\n",Deploy_ctrl_t.stat);
					break;
			}	
		}
		osal_task_sleep(500);
	}
}

void BtDeployInit(void)
{
	memset(&Deploy_ctrl_t,0,sizeof(Deploy_ctrl_t));
	statcnt = 0;
	Deploy_ctrl_t.stat = APP_DEPLOY_CONNECTING;
	Deploy_ctrl_t.App_task_ctrl.stk_size = 4096;
	Deploy_ctrl_t.App_task_ctrl.tsk_prio = 24;
	Deploy_ctrl_t.App_task_ctrl.param = 0;
	Deploy_ctrl_t.App_task_ctrl.name[0] = 'A';
	Deploy_ctrl_t.App_task_ctrl.name[1] = 'P';
	Deploy_ctrl_t.App_task_ctrl.name[2] = 'P';
	Deploy_ctrl_t.App_task_ctrl.tsk_entry_func = (OSAL_TSK_PROC)Bt_connect_task;
	Deploy_ctrl_t.App_task_hdl_ctrl = osal_task_create(&Deploy_ctrl_t);
	if (OSAL_INVALID_ID == Deploy_ctrl_t.App_task_hdl_ctrl)
	{
		/*error*/printf("task create failed!");
		return -1;
	}
	return;
}

void BtDeployUnit(void)
{
	statcnt = 0;
	osal_thread_delete(Deploy_ctrl_t.App_task_hdl_ctrl);
	memset(&Deploy_ctrl_t,0,sizeof(Deploy_ctrl_t));
	return;
}

static int AppOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	int i;
	printf("App Open Win:%d, modeMem:0x%x, memBase:0x%x\n",os_time_get(), modeFreeSpace(), getCurModeFreeBuffBase());
	jpgdecFlag = 0;
	show_bk=1;

	//==init==
	lcdGetVideoRes(&bk_w,&bk_h);
	app_bk_buf=(getCurModeFreeBuffBase()+0x3f)&(~0x3f);//hal_sysMemMalloc(bk_w*bk_h*3/2,32);

	if(NULL!=app_bk_buf)
	{
		jpgdecById(RES_APP_BK, app_bk_buf);   
	}
	else
	{
		printf("mem err!\n");
		return 0;
	}
#if 1
{
		disp_frame_t *p_lcd_buffer;
		p_lcd_buffer = (disp_frame_t *)lcdVideoFrameMalloc();//dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			lcdGetRes(&lcd_w,&lcd_h);
			lcdframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
			memcpy(p_lcd_buffer->y_addr,app_bk_buf,lcd_w*lcd_h*3/2);
			dcache_writeback_region((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			lcdDealVideBuff(p_lcd_buffer);
		}
		else
		{
			printf("video buf null!\n");
		}
}
#endif	
	// winSetResid(winItem(handle,APP_CONNEC_ID),strToResId("Wating wifi Config ..."));
	// winSetVisible(winItem(handle,APP_CONNEC_ID),true); 
	BtDeployInit();
	return 0;
}

static int AppCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	printf("main menu Close Win!!!\n");
    //winSetResid(winItem(handle,APP_CONNEC_ID),strToResId(" "));
    winSetVisible(winItem(handle,APP_CONNEC_ID),false); 
	if(NULL!=app_bk_buf)
	{
		app_bk_buf=NULL;
	}
	BtDeployUnit();
	return 0;
}
   
static int AppSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
    Deploy_ctrl_t.stat=AppWifiShow(handle,Deploy_ctrl_t.stat);
	if(Deploy_ctrl_t.stat==APP_DEPLOY_SUCCESS || Deploy_ctrl_t.stat<APP_DEPLOY_SUCCESS)
	{
	    statcnt++;
		if(statcnt==4)
		{
			 Deploy_ctrl_t.stat = APP_DEPLOY_CONNECTING;
			statcnt=0;
		}
	}
	return 0;
}

static int AppSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int AppSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int AppsysMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
    functionStart(FUNCTION_MAINMENU,2);
	return 0;
}
  

static int AppMsgLongOk(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
	if(parameNum==1)
		keyState = parame[0];
	if(keyState==KEY_PRESSED){

	}
    return 0;
}

static int AppsysMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

msgDealInfor AppMsgDeal[]=     
{
	{SYS_OPEN_WINDOW,AppOpenWin},  
	{SYS_CLOSE_WINDOW,AppCloseWin},
	{KEY_EVENT_OK,AppMsgLongOk},
	{SYS_EVENT_500MS,AppSysMsg500MS},
	{SYS_EVENT_USB,AppSysMsgUSB},
	{SYS_EVENT_BAT,AppSysMsgBattery},   
	{KEY_EVENT_POWER,AppsysMsgPower},
	{KEY_EVENT_SETTING,AppsysMsgMenu},
	{KEY_EVENT_MODE,AppsysMsgPower},

	{EVENT_MAX,NULL},
};

WINDOW(AppWindow,AppMsgDeal,AppWin)



