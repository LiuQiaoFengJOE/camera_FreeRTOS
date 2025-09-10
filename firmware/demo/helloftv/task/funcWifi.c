#include "app.h"
#include"uiWin.h"


extern menu MENU(movie);
void funcWifiOpen(uint32 arg1)
{
#if WIFI_EN
	uiOpenWinAndShow(&wifiWindow,0);
#if LCD_EN
	u32 size;
	u32 *ptr;
	wmm_trans_mode(&workmode, WM_NONE);
	lcdPreviewStop();
	lcdVidFrameFree();
	sysMemFree(get_workmode_heap());
	size=sysMemRemain();
	ptr=sysMemMalloc(size,4);
	printf("enter wifi mode:addr:0x%x, %dKB\n",(u32)ptr,size>>10);
	workModeMemInit(ptr,size);
	wmm_reinit(&workmode,ptr,size);
#endif
	wmm_trans_mode(&workmode, WM_LIVE);
	#if JR_APP_PROTOCOL==0
	socket_cmd_start_service();
	#endif
	//uiOpenWindow(&wifiWindow,0);
#endif
}


extern void conn_drop_all(void *e,void* d);
extern uint8_t get_conn_list_num();
extern uint32_t rtsp_conn_num;
void dropConnFromEloop(void)
{
	if(get_conn_list_num())
	{
		printf("add eloop conn_drop_all\n");
		eloop_add_alarm(0,0,conn_drop_all,NULL);
	}
}
void dropConnAndWaite(void)
{
	u32 working=1;
	if(get_conn_list_num())
	{
		eloop_add_alarm(0,0,conn_drop_all,&working);// 和rtsp放到同一线程，避免释放时，还在处理rtsp协议
		while(working)
		{
			osal_task_sleep(5);
		}
	}
	if(rtsp_conn_num)// lyc debug
	{
		while(rtsp_conn_num)
		{
			printf("rtsp connet error!!!:%d\n\n\n\n",rtsp_conn_num);
			osal_task_sleep(20);
		}
	}
}
void funcWifiClose(uint32 arg)
{
#if WIFI_EN
	u32 size;
	u32 *ptr;
	#if JR_APP_PROTOCOL==0
	socket_cmd_stop_service();
	#endif
	dropConnAndWaite();
	configLanguage();
	sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_UNINIT,0); 
#if LCD_EN
	wmm_trans_mode(&workmode, WM_NONE);
	sysMemFree(get_workmode_heap());
	lcdVidFrameRealloc();
	size=sysMemRemain();
	ptr=sysMemMalloc(size,4);
	//printf("wifi mode:addr:0x%x, %dKB\n",(u32)ptr,size>>10);
	workModeMemInit(ptr,size);
	wmm_reinit(&workmode,ptr,size);
#endif
#endif
}

sysFunction functionWifi=
{
	"wifi",
	NULL,
	funcWifiOpen,
	funcWifiClose,
};


