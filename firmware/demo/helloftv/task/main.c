#include <stdarg.h>

#include "typedef.h"
#include "board_config.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"

#include "osal/osal_os.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "mmc/core/sdio.h"
#include "jpeg/jpeg.h"

#include "fatfs/ff.h"
#include "stream/stream.h"
// #include "hal_auadc.h"
#include "http/http.h"
#include "sys/sys.h"
#include "systemSetting/systemSetting.h"
#include "systemSetting/sysCommon.h"
#include "spi/spi.h"
#include "stdlib.h"

// Only for Debug
#include "spr_defs.h"

#include "wifiProc.h"

#include "eventTimer/eventTimer.h"
#include "sysCommon/commonInclude.h"
#include "mac_fd/mac_fd.h"

#include "hal_watermark/hal_watermark.h"
#include "ax32xx/inc/ax32xx_wdt.h"
// #include "newWay/newWay.h"
#include "drv/diskio/sdiodisk.h"
// #include "ax32xx_dma.h"
#include "task/app.h"

#if FINSH_RX_ENABLE
#include "shell.h"
#endif


extern uint8_t get_wifi_status();
extern void api_backsta11_linking(void);
extern void api_backsta2_linking(void);
void main_task(void *);
void flight_task(void *);
void start_prepare(void *arg);
void main_task(void *);

/************************************************
wifi状态下灯的情况
1、wifi正在打开情况,灯快闪，代表还没有完全打开wifi
2、wifi已经打开,但尚未连接,灯1s闪烁2次(慢闪),
	 代表wifi已经打开,可以连接设备
3、wifi打开情况下,并且已经有设备连接rtsp时,则常亮(理论上有连接的时候,则代表wifi已经打开
		所以不需要判断wifi具体状态)
**************************************************/
u8 rtc_display = 0; // 测试时候控制rtc发送消息，0：不发送，

void check_fd(void *d)
{
	struct timerCommon *common = (struct timerCommon *)d;
	char *mac;
	uint8_t res = 1;
	// 状态改变

	if (mac_num())
	{
		mac = get_one_mac();
		if (mac)
		{
			// 把所有与mac地址一样的fd清除,并执行相应的处理
			while (res)
			{
				res = del_mac_fd(mac);
			}
			// 将mac地址从哈希表删除
			delete_mac(mac);
			printf("have mac exit!\r\n");
		}
	}
}
void deleteMacRtspConn(void)
{
	char *mac;
	uint8_t res;
	printf("mac num:%d\n",mac_num());
	while(mac_num())
	{
		res = 1;
		mac = get_one_mac();
		if (mac)
		{
			// 把所有与mac地址一样的fd清除,并执行相应的处理
			while (res)
			{
				res = del_mac_fd(mac);
			}
			// 将mac地址从哈希表删除
			delete_mac(mac);
			printf("have mac exit!\r\n");
		}
	}
}
extern uint16_t ff_times;
extern uint16_t get_cluster_size();
extern uint32_t get_current_workmode();
int sdCheckEn=1;
void sdCheckEnSet(int en)
{
	sdCheckEn=en;
}
void check_sd(void *d)
{
	static FRESULT lastSdSta = 0xff;
	FRESULT curSdSta;
	struct timerCommon *common = (struct timerCommon *)d;
	if(videoIsRecording() || (takePictrueIsWorking()))
		return;
	if(sdCheckEn==0)
	{
		common->nowTimer = 0;
		common->startTimer_count = common->nowTimer_count;
		return;
	}
	if(videoIsRecording() || (takePictrueIsWorking()))
		return;
	if (!ff_times)
	{
		common->nowTimer = 0;
		common->startTimer_count = common->nowTimer_count;
		ff_times = 1;
	}

	if (common->nowTimer > 1000)
	{
		FATFS *fs;
		curSdSta = f_system_status(&fs, "1:");
		common->nowTimer = 0;
		common->startTimer_count = common->nowTimer_count;
		if(lastSdSta!=curSdSta)
		{
			printf("SD tatus:%d\n",curSdSta);
			if(curSdSta==FR_OK)
				SysCtrl.sdcard=SDC_STAT_NORMAL;
			else
			{
				set_free_cap(0);
				if(curSdSta==FR_NO_FILESYSTEM)
					SysCtrl.sdcard=SDC_STAT_ERROR;
				else
					SysCtrl.sdcard=SDC_STAT_NULL;
			}
			fileScan();
			user_action(MSG_ID_CARD_STATUS);
			sendMsgNonblock(OS_TASK_UI,SYS_EVENT_SDC, get_sd_status());
			lastSdSta=curSdSta;
		}
		// printf("test\r\n");
	}
}
u8 appGetSdStatus(void)
{
	if(SysCtrl.sdcard==SDC_STAT_ERROR)
		return 2;
	if(SysCtrl.sdcard==SDC_STAT_NULL)
		return 0;
	return 1;
}
extern size_t configTOTAL_HEAP_SIZE[];
extern size_t _text_start[];
extern size_t _text_end1[];
u32 textCrc;
u32 rotatingHash(u32 *buff,u32 len)
{
#if 0
    int hash, i;
	//buff=((u32)buff)+0x80000000;
	printf("text crc addr:0x%x,len:%d - %dKB\n",buff,len,len>>8);
	hash=buff[0];
    for(i=0;i<len;i++)
        hash = (hash << 4)^(hash >> 28)^buff[i];
    return hash;
#else
	return 0;
#endif
}
u32 calTextCrc(void)
{
	return rotatingHash(_text_start,((size_t)_text_end1-(size_t)_text_start)>>2);
}
void textCrcCheck(void)
{
#if 0
	u32 curCrc=calTextCrc();
	if(textCrc==curCrc)
	{
		printf("text is ok,crc:0x%x ============\n\n",curCrc);
	}
	else
	{
		printf("text was changed,crc:0x%x !!!!!!!!!!!!\n\n\n\n",curCrc);
	}
#endif
}

extern u32 bss_end;
extern u32 _sdram_heap_start;
void bssEndToHeapCheck(char *tag,u32 d)
{
	u32 *addr=&bss_end;
	u32 *end=&_sdram_heap_start;
	u32 overflow=0;
	printf("=================\n[%s %d]heap check:0x%x-0x%x,size:%d bytes\n",tag,d,addr,end,((u32)end-(u32)addr));
	while(addr<end)
	{
		if(*addr!=0)
		{
			overflow+=4;
			printf("addr:0x%x,val:0x%x\n",addr,*addr);
		}
		addr++;
	}
	printf("overflow %d bytes\n=================\n",overflow);
	
}
void bssEndToHeapClear(void)
{
	u32 *addr=&bss_end;
	u32 *end=&_sdram_heap_start;
	while(addr<end)
	{
		*addr=0;
		addr++;
	}
	addr=&bss_end;
	dcache_flush_region((u32)addr,((u32)end-(u32)addr));
}
#if(USB1_CAMERA&&USB2_CAMERA)
	#define OS_HEAP_SIZE  (1000*1024)
#elif(USB1_CAMERA||USB2_CAMERA)
	#define OS_HEAP_SIZE  (900*1024)
#else
#if (PIC_STICKER_EN || BIG_PIC_EN) 
	#define OS_HEAP_SIZE  (800*1024+/*1500*//*200*/400*1024)
#else
    #define OS_HEAP_SIZE  (800*1024)
#endif
#endif
volatile u32 breakPointTest;
int _init(int r3, int r4, int r5, void (*r6)(int ch))
{
	u32 size;
	ax32xx_sysSRAMClear();
	board_init();
	sysMemInit();
	ax32xx_dmaNocDefault();
	batteryInit();
	bssEndToHeapClear();
	lcdFrameBuffInit();
	size=sysMemRemain()-OS_HEAP_SIZE;
	void *ptr=sysMemMalloc(size,4);
	printf("workmode size:%dKB\n",size>>10);
	while(ptr==NULL)
	{
		printf("malloc workmode memory fail:%dKB\n",size>>10);
	}
//	SPR_LSUBP0=&breakPointTest;
//	SPR_LSUCFG0=0x1;
//	SPR_LSUCFG1=0x1;
//	SPR_LSUCFG2=0x1;
	workModeMemInit(ptr,size);
	sysMemRemain();
	//printf("heap size:%dKB\n",(size_t)configTOTAL_HEAP_SIZE>>10);
	textCrc=calTextCrc();
	printf("text crc:0x%x\n",textCrc);
	if (!osal_thread_create(start_prepare, NULL, 18, 0, 2048, "start_prepare")) // 运动DV的main
	{
		printf("Create MAIN task error!\r\n");
	}
	else
	{
		printf("Create MAIN task ok!\r\n");
	}
	osal_os_start();
	return 2;
}

#ifdef htons
#undef htons
#endif
#ifdef ntohs
#undef ntohs
#endif

__attribute__((weak))
u16_t
htons(u16_t n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

__attribute__((weak))
u16_t
ntohs(u16_t n)
{
	return htons(n);
}
int debugTcpPort(u16 port)
{
	if(port==8080||port==8081)
		return 1;
	return 0;
}
u32 g_ap_sta_num = 4;

// void Set_WLAN_Power_Off          (void) {};
// void Set_WLAN_Power_On           (void) {};
// void wext_get_enc_ext            (void) {};
// void wext_get_mode               (void) {};
// void wext_get_ssid               (void) {};
// void wext_set_gen_ie             (void) {};
// void wext_wlan_indicate          (void) {};
//
// void queue_for_credential        (void) {};
// void paff_array                  (void) {};
//
// void config_method               (void) {};
// void wps_pin_code                (void) {};
// void wps_password_id             (void) {};

extern struct netif *p_netif;


/********************/

// #define _SYSDSK_ "0:"
// #define _DATDSK_ "1:"

#if 1
static FATFS fatfs[2];

// 前提扇区大小是512
// fatfs[1].csize*512/1024 = fatfs[1].csize/2=fatfs[1].csize>>1

void *get_win(int num)
{
	return fatfs[num].win;
}

uint16_t get_cluster_size()
{
	return fatfs[1].csize >> 1;
}

DWORD get_fatbase(int num)
{
	// return NULL;
	return fatfs[num].fatbase;
}

DWORD get_fsize(int num)
{
	// return NULL;
	return fatfs[num].fsize;
}

DWORD get_fats(int num)
{
	// return NULL;
	return fatfs[num].n_fats;
}
FATFS *getFatFs(int num)
{
	if(num>1)
		return NULL;
	return &fatfs[num];
}
static int fatfs_init(void)
{
	// static FATFS fatfs[2];
	int ret;
	printf("++++++++++%s+++++++++++\n", __func__);

	if (ret = f_mount(&fatfs[0], _SYSDSK_, 1))
	{
		p_err("mount %s err %d\r\n",_SYSDSK_, ret);
		//return 1;
	}
	if (ret = f_mount(&fatfs[1], _DATDSK_, 1))
	{
		p_err("mount %s err %d\r\n",_DATDSK_, ret);
		return 1;
	}
	get_cluster_size();
	printf("++++++++++ %s OK++++++++++\n", __FUNCTION__);
	return 0;
}

void remount_sd(void *d)
{
	int ret = f_mount(&fatfs[1], _DATDSK_, 1);
	printf("remount:%d\r\n", ret);
}

#endif

extern void *getMsg_sd_status(uint8_t value);
/*************************************************************************************************
********************************gui end**********************************************************
*************************************************************************************************/

#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV || WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
int is_broadcast_ether_addr(const unsigned char *addr)
{
	return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xff;
}
#endif

u8 rf_get_code = 0;
u8 rf_sem_ok = 0;
extern u32 rx_len;
extern u8 rx_buf[64];
u32 cmd_rx = 0;
void timer_work()
{
	static u8 rx_timeout;
	static u32 rx_len_tmp = 0;
	static u32 lastTime;
	static u32 debug_tmp = 0;
	rx_timeout++;

	if(rx_timeout == 50){			//50ms执行一次
		if(!curFuncWifi())
		{
			rx_len=0;
			rx_timeout = 0;
			goto UART_END;
		}
		if(rx_len == 0) 			//没有获取到一次RX数据
		{
			rx_timeout = 0;
			goto UART_END;
		}

		if(rx_len_tmp == rx_len)	//通过接收超时来判断是否接收完成
		{
			if(os_time_get()-lastTime>1000)
			{
				lastTime=os_time_get();
				printf("rx_len:%d, rx_buf:%s\n", rx_len, rx_buf);
			}
			rx_timeout = 0;
			rx_len_tmp = 0;
			cmd_rx = rx_len;
			rx_len = 0; 
		}else{
			rx_timeout = 0;
		}
		rx_len_tmp =  rx_len;
	}
UART_END:
	#if SUPPORT_RF_RECV
	{
		static u32 itk = 0;
		static u32 itk_sem = 1;
		itk++;
		if(itk_sem){
			if(itk > 20){
				itk = 0;
				if(rf_sem_ok){
					//xSemaphoreGiveFromISR( rf_sema,NULL);
					sendMsgNonblock(OS_TASK_UI, KEY_EVENT_RF_RECV, rf_get_code);
					itk_sem = 0;
				}
			}
		}

		if(itk_sem == 0)
		{
			if(itk > 601){
				rf_sem_ok = 0;
				itk_sem = 1;
				itk = 0;
			}
		}
	}
	#endif
	return;
}

#if SUPPORT_RF_RECV
void port_isr_init(){
	//LDOCON &=(~BIT(21));
	//ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
	ax32xx_gpioMapSet(GPIO_PB,GPIO_PIN6,GPIO_FUNC_GPIO);
	ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN6,GPIO_INPUT);
	isr_port_edg(10);
}

/*void rf_thread(){
	vSemaphoreCreateBinary(rf_sema);
	while(1){
		xSemaphoreTake(rf_sema, portMAX_DELAY);
		printf("rf_thread:%x \n",rf_get_code);
		sendMsgNonblock(OS_TASK_UI, KEY_EVENT_RF_RECV, rf_get_code);	
		rf_get_code = 0;
	}
}*/
u32_t sys_jiffies(void)
{
  return xTaskGetTickCount();
}
void RF_msg_chk(){
	static u32 timer_last = 0;
	static u32 stimer_f = 0;
	static u32 stimer_l = 0;
	u32 timer_idle = 0;
	u32 timer_now = 0;
	static u8 timr_cal_num = 0;
	static u8 timr_cal_start = 0;
	
	//printf("rf\n");

	timer_now = sys_jiffies();
	timer_idle = timer_now - timer_last;
	timer_last = timer_now;

	if(timr_cal_start){
		if(timr_cal_num == 0){
			stimer_f = timer_now;
		}

		if(timr_cal_num == 7){
			stimer_l = timer_now;
		}

		#if 1		
		if((timer_idle >= 6)&&(timer_idle <= 9)){
			rf_get_code |= BIT(timr_cal_num);     //SET
		}else if((timer_idle <= 5)&&(timer_idle >= 3)){
			rf_get_code &= ~BIT(timr_cal_num);	  //CLR
		}else{
			timr_cal_start = 0;
			rf_get_code = 0;
			timr_cal_num = 0;
		}	
		#else
		if((timer_idle >= 6)&&(timer_idle <= 9)){
			rf_get_code |= BIT(timr_cal_num);     //SET
		}else {
			rf_get_code &= ~BIT(timr_cal_num);	  //CLR
		}
		printf("idle:%d\n",timer_idle);
		#endif		
		timr_cal_num++;
		if(timr_cal_num == 8){
			timr_cal_start = 0;
			timr_cal_num = 0;
			printf("rf_get_code:%x   %d\n",rf_get_code,stimer_l - stimer_f);
			
			if( (stimer_l - stimer_f) == 41){
				rf_sem_ok = 1;
			}

			//if(((stimer_l - stimer_f)>=40 ) && ((stimer_l - stimer_f) <= 41)){
			//	rf_sem_ok = 1;
			//}
		}
	}

	if((timer_idle>=17) && (timer_idle <= 19)){
		//printf("rf st\n");
		rf_sem_ok = 0;
		timr_cal_start = 1;
	}
}
#else
void RF_msg_chk(){}
#endif

static void do_config(void)
{
#if WIFI_EN
	rtsp_register();
	rtp_audio_packer_register();
	tcp_register();
	live_register();
	webfile_register();
	paly_net_register();
	rtp_packer_register();
	rtp_sender_register();
#endif
	rtp_jpeg_register();
	rtp_audio_register();
	
	vid_csi0_register();
	vid_csi1_register();
	vid_csiuvc_register();
	usb_yuv_register();
	pic_combine_register();
	pcm_auadc_register();
	
	jpgencAutoRegister();
	//jpgencManu0Register();
	jpgencManu1Register();
	jpgChangeResRegister();

	record_file_register();
	read_file_register();
	paly_video_register();
	
	usb_device_register();
	uvc_stream_register();
	uvc2_stream_register();
	usb1_check_register();
	usb2_check_register();
	pic_printer_register();
//	zbar_steam_register();

	test_steam_register();
	printf("config file start===========\n");
	read_config_file(_SYSDSK_ "Config.dat");
	printf("config file end==============\n\n");
}

/************************************************************************
//给用户使用的一个初始化函数
//主要是软件定时,这里一般添加那些开机需要启动的软定时程序(例如:SD卡检测需要开机就检测)
可以执行一些耗时任务,但需要衡量,如果太耗时,会导致其他事件没有办法立刻执行,
需要权衡
***************************************************************************/
void default_event_timer_LOW()
{
	add_event_timerList(LOW, NULL, check_sd, NULL, -1);	   // sd卡检测
#if WIFI_EN
#if 1//JR_APP_PROTOCOL
	//add_event_timerList(LOW, NULL, check_fd, NULL, -1);		 // fd  socket检测
#endif
#endif
}

/*********************************************
如果需要开机启动默认使用高timerList的任务,需要
考虑任务里面是否有耗时,这个timerList坚决不能执
行耗时任务
**************************************************/
void default_event_timer_HIGH()
{
	// add_event_timerList(HIGH,NULL,get_battery,NULL,-1);
	//add_event_timerList(HIGH, NULL, wifi_led_timer_event, NULL, -1);
	// add_event_timerList(HIGH,NULL,debug_event,NULL,-1);
}

////////////////////////////////////////////////////////////////////////////////
extern void * netdev_get_netif(const char *ifname);
extern int wifi_dev_init_sta;

#if (WIFI_SUPPORT_VERSION==BOARD_WIFI_SV6158)
void wifi_config_sv6xxx(void)
{
	extern struct mmc_host bw_mmc_host;
	struct mmc_host *sd_host;
	sd_host = &bw_mmc_host;

	irq_register(7, 0, sdio_irq_handler, sd_host);
	//ssv6xxx_dev_sta_init();
	ssv_wifi_init_mode(0);
	p_netif=netdev_get_netif("wlan1");
	//ssv_wifi_set_mode(WIFI_MODE_STA);
	if(wifi_dev_init_sta)
		ssv_wifi_enter_ps();
	p_err("init wifi OK:0x%x\n",p_netif);
}

#endif

#if WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV || WIFI_SUPPORT_VERSION == BOARD_WIFI_ATBM
uint32_t lwip_hook_ip4_route(uint32_t dest)
{
	if (netif_default == NULL)
		printf("netif_default == NULL\r\n");

	if (!netif_is_up(netif_default))
		printf("!netif_is_up(netif_default)\r\n");

	if (!netif_is_link_up(netif_default))
		printf("!netif_is_link_up(netif_default)\r\n");

	if (ip4_addr_isany_val(*netif_ip4_addr(netif_default)))
		printf("ip4_addr_isany_val(*netif_ip4_addr(netif_default))\r\n");

	return 0;
}
#endif

//  if ((netif_default == NULL) || !netif_is_up(netif_default) || !netif_is_link_up(netif_default) ||
//      ip4_addr_isany_val(*netif_ip4_addr(netif_default))) {

//extern key_t board_keys;

int sdc_kick;
void donothing(void* arg)
{
}
u32 getPaletteIdx(void)
{
	return RES_PALETTE;
}
u32 getLcdMsg(void)
{
	return RES_LCD_MSG;
}
extern void printer_task(void *d);
extern void power_off_task(void);
extern void wmm_trans_mode(struct workmode *wm, uint32_t t);
extern struct workmode workmode;
// 文件系统的初始化,先准备高优先级的timer,低优先级timer由于可能涉及流操作,因此由后面再初始化
// 启动前的准备,lcd流,sd升级流、ctrl流先初始化
void start_prepare(void *arg)
{
	u8 *head512b = NULL;
	u8 *buf;
	uint32_t readCheckNum = 0;
	uint32_t sdCheckNum = 0;
	void *fp;
	// 看门狗
	ax32xx_wdtEnable(1);
	ax32xx_wdtTimeSet(0x06);
	ax32xx_wdtClear();
	textCrcCheck();
	// 文件系统
	fatfs_init();
	f_chdrive(_DATDSK_);
	resfs_init(_SYSDSK_ "res.bin");
	//font_init();
	//font_setlang(3);
	userConfigInitial();
	fontInit();
	Ebook_Init();  
	iconInit();
	osTasksInit();
	sdCheckEnSet(1);
	// lcd
#if 1 //temp del
	ax32xx_sysLDOSet(SYS_LDO_LSEN, SYS_VOL_V1_8, 1);
	osal_task_sleep(2);
	ax32xx_sysLDOSet(SYS_LDO_HSEN, SYS_VOL_V3_1, 1);
	osal_task_sleep(5);   
#else

	ax32xx_sysLDOSet(SYS_LDO_V1_2,SYS_VOL_V1_3,1);	 //VDDCORE is better to config before clktun
	osal_task_sleep(20);
	ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V3_0,1);	
	osal_task_sleep(20);
	ax32xx_sysLDOSet(SYS_LDO_HSEN,SYS_VOL_V3_0,1);	
#endif	
	textCrcCheck();  
	lcd_bk_ctrl(0);
	setKeyEn(true);
	lcd_reset();
//	sensor_reset(0);
//	sensor_pwdn(1);
	lcdInit(RES_PALETTE, RES_LCD_MSG);//资源里必须要加入特定的数据块，用来存放lcd, isp的相关设定
	logoShow(RES_ICON_POWER_ON);
	textCrcCheck();
	sysMemRemain();
	dacInit();
	dacPlaySound(RES_MUSIC_POWER_ON,getSoundVol());
#if TP_FUCTION 
	touchPanelHardWareInit();
	touchPanelInit();
#endif	
	// 获取bin文件的卷标序列号,用于sd卡升级时作为的校验
	// 如果两个文件的卷标序列号一样,则代表是同一个bin文件
	// 如果说采取手动修改bin文件内容,则需要去修改卷标序列号才能使用sd升级
	// 卷标序列号在第一个扇区偏移43H-46H

	// 卷序列号
	head512b = osal_malloc(80);
	spi_read(0, head512b, 80);
	memcpy(&readCheckNum, head512b + 0x43, 4);
	printf("readCheckNum:%X\n", readCheckNum);
	uartPrintfBuf(head512b + 0x43, 4);
	osal_free(head512b);
	winInit();
	taskMsgInit();
	configLanguage();
	netItemsInit();
	// 检测升级文件
	lcdDeTimeoutRegister();
	fp = osal_fopen(UPDATE_BIN, "rb");
	if (fp)
	{
		buf = osal_malloc(512);
		osal_fread(buf, 512, 1, fp);
		memcpy(&sdCheckNum, buf + 0x43, 4);
		osal_free(buf);
		osal_fclose(fp);

		if (sdCheckNum != readCheckNum)
		{
			sdUpgrade();
		}
	}
	printf("%s end\n", __FUNCTION__);

	// 不需要升级,创建主线程

	//wmm_trans_mode(&workmode, WM_NONE);
	//osal_thread_create(main_task, NULL, 18, 0, 20*1024, "MAIN_RunningDV");
	taskQueueEnable(0);
	createdTaskByFunc(OS_TASK_MAIN, main_task,NULL,19,4*1024,"main",20);
	printf("thread [%s] stack size:%d,use:%d\n",pcTaskGetName(NULL),uxTaskGetStackSize(NULL),uxTaskGetStackSize(NULL)-uxTaskGetStackHighWaterMark(NULL));
	osal_thread_delete(osal_thread_myself());
}

 /* Cpu_task function */
 void Cpu_task(void const * argument)
 {
   /* USER CODE BEGIN Cpu_task */
     uint8_t CPU_RunInfo[512];
   /* Infinite loop */
   for(;;)
   {
       memset(CPU_RunInfo,0,512);
       vTaskList((char *)&CPU_RunInfo); //获取任务运行时间信息
       printf("--------------------------------------------\n");
       printf("name       statr     youxianji     sta     no\n");
       printf("%s", CPU_RunInfo);
       printf("--------------------------------------------\n");
       memset(CPU_RunInfo,0,512);
       vTaskGetRunTimeStats((char *)&CPU_RunInfo);
       printf("taskname       count     %\n");
       printf("%s", CPU_RunInfo);
       printf("---------------------------------------------\n");
       osal_task_sleep(1000);
   }
   /* USER CODE END Cpu_task */
 }

void finsh_hellow_wold(int argc, char **argv)
{
	printf("\n==================help_info=======================\n");
	printf("finsh hellow wold\n");
	printf("\n==================================================\n");
}

MSH_CMD_EXPORT_ALIAS(finsh_hellow_wold, hw, helpinfo);


#if FINSH_RX_ENABLE
	 u8 rxsema_init = 0;
#endif
//volatile u32 *pAlignTest;
void main_task(void *arg)
{
	textCrcCheck();
	printf("breakPointTest start:0x%x\n",breakPointTest);
	//SPR_SR|=BIT(17);
	//printf("sr:0x%x\n",SPR_SR);
	//breakPointTest=0x12345678;
//	pAlignTest=getAlignTestAddr();
//	*pAlignTest=0x12345678;
	printf("breakPointTest end:0x%x\n",breakPointTest);
#if 0//ACTION_MODE == CHILDREN
	ax32xx_sensor_pk(get_sen_mode());
#endif
#if WIFI_EN
#if WIFI_SUPPORT_MODE == DVRUNNING
	wifi_switch_init();
	wifi_switch_on(1);
#endif
#endif
	// 获取bin文件的卷标序列号,用于sd卡升级时作为的校验
	// 如果两个文件的卷标序列号一样,则代表是同一个bin文件
	// 如果说采取手动修改bin文件内容,则需要去修改卷标序列号才能使用sd升级
	// 卷标序列号在第一个扇区偏移43H-46H
	//board_battery_init(); // 电池初始化

	// 在rtc初始化时,先设定rtc起始时间,如果没有接电池,则使用起始时间,如果有电池,则起始时间无效
	get_date(__DATE__);
	hal_rtcInit();
	hal_rtcAlarmSet(0, 0);
	ax32xx_rtcBatDecEnable(1);

	void *i2c = i2c_get_controller(0);
	printf("i2c controller %08X\n", i2c);
	i2cInit(i2c, 40000);
	printf("i2cInit\n");
	sensorInit(i2c);
	csi_setup();
	isp_init_timertask();
	isp_init();
	picStickerInit();
	sdc_kick = 0;
	eloop_init();
	eloop_add_timer(40, 0, donothing, NULL);//this is for eloop,do not delete this timer;
#if WIFI_EN
	wifi_dev_init();

	#if SUPPORT_RF_RECV
	port_isr_init();
	#endif
	
#endif
	sdc_kick = 1;
	do_config();
	setCsiJpgInterval(30);
	//osal_thread_create(event_timerList_thread_HIGH, NULL, 20, 0, 1024, "timerList_HIGH"); // 比较准确,高优先级,用于对时间要求高(定时拍照)
	osal_thread_create(event_timerList_thread_LOW, NULL, 2, 0, 1024, "timerList_LOW");	  // 优先级较低,用于对时间不敏感的事件(sd卡检测、按键音、电池电量)
//	#if SUPPORT_RF_RECV
	//osal_thread_create(Cpu_task, NULL, 2, 0, 1024, "Cpu_task" );
//	#endif
	//osal_task_sleep(100);
	printf("wait 100ms end\r\n");

	//printf("user save data\r\n");
	// 应该在所有流创建完后再赋值
	//global_init(); // 先初始化，再读取
	//default_sensor_setting();
	hal_watermarkInit2();
	lcd_set_crop_csi_to_lcd(0);
	hal_rtcCallBackRegister(api_backsta2_linking);

	printf("2xPortGetFreeHeapSize:%d\r\n", xPortGetFreeHeapSize());

	//os_key_init(&board_keys, guib_msgsnd, 5);
	//printf("%d %d %d %d %d %d\n", MSG_NONE, MSG_MENU_ENTER, MSG_DOC_CHG_POLL, MSG_DOC_CHG, KEY_MENU, MSGID_SYSMAX);
	//stream_check();
#if WIFI_EN
#if JR_APP_PROTOCOL
	config_ftp(21);
	config_http(80);
	config_tcp_control(5000);
#else
	socket_cmd_service_init();
	//socket_cmd_start_service();
#endif
#endif
	// 足够时间启动相关任务
	osal_task_sleep(10);
	configSystem();
	printf("LDOCON:%X\r\n", LDOCON);
	createTasks();
#if FINSH_RX_ENABLE
	 rxsema_init = 1;
	 finsh_system_init();
#endif



main_loop:

	eloop_run(0);

	while (1)
	{
		printf("MAIN task loop.\n");
	}
	osal_task_sleep(5000);
	// errled(0x150F0F15);
}

