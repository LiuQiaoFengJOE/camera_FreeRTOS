#include <stdarg.h>

#include "typedef.h"
#include "board_config.h"


#include "osal/osal_os.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "mmc/core/sdio.h"
#include "jpeg/jpeg.h"
//#include "csi/yuv-csi.h"

#include "fatfs/ff.h"
#include "stream/stream.h"
// #include "hal_auadc.h"
#include "http/http.h"
#include "sys/sys.h"
#include "systemSetting/systemSetting.h"
#include "systemSetting/sysCommon.h"
#include "spi/spi.h"
#include "stdlib.h"
#include "sd_config.h"

#include "wifiProc.h"

// Only for Debug
#include "spr_defs.h"
// #include "AX3268.h"

#include "eventTimer/eventTimer.h"
#include "sysCommon/commonInclude.h"
#include "mac_fd/mac_fd.h"

#include "drv/isp/ax32xx_isp.h"
#include "drv/csi/ax32xx_csi.h"

#include "drv/csi/hal_csi.h"
#include "task/app.h"
#include "drv/lcd/lcdframe.h"


#define DEF_MODE_PARENT(n, m, p) \
	static path_type n[WM_MAX] = {WM_NONE, p};
extern struct workmode workmode;

extern void wmm_adjusStreamMemStart(struct workmode *wm, struct stream_src *s);

u32 getJpgencMem(void)
{
	return workmode.remainMem;
}
static int wmm_push_usb(struct workmode *wm, void *d)
{
	printf("*** %s\n", __func__);
	uint32_t mallocSize = 0;
	wmm_enter(wm, WM_USB);
	//struct stream *yuvCsi1Stream = stream_find("yuv-csi1");
	//wmm_push_stream(wm, yuvCsi1Stream);"usb-device"
	struct stream *usbDev = stream_find("usb-device");
	struct stream *jpgauto = stream_find("jpg-auto");
	struct stream_dest *usbDevInput = stream_self_get_input(usbDev->self, 0);
	stream_dest_plugin(usbDevInput, jpgauto);
	wmm_adjusStreamMemStart(wm, jpgauto);
	wmm_push_stream(wm, jpgauto);
	wmm_dynalloc(wm, WM_USB);

	return 1;
}
u32 getJpgencOrUsbMem(u32 jpgenc)
{
	u32 canUsedMemLen;
	u32 reservedMemLen = 0;
	u32 srcW,srcH;
	
	if (picStickerSupport() || bigPicSupport())
	{
		//sensorGetRes(&srcW,&srcH);
		reservedMemLen = 1280*720*3/2+0x1000;//srcW*srcH*3/2+0x1000;
		if (workmode.remainMem<=reservedMemLen)
			return 0;
	}
	
	if(functionCurrent() == FUNCTION_USB_DEVICE)
	{
		return workmode.remainMem*2/3;
	}
	if(usbCameraSum()==2)
	{
		if(jpgenc)
			return (workmode.remainMem-reservedMemLen)*7/10;
		return (workmode.remainMem-reservedMemLen)*3/10/2;
	}
	else if(usbCameraSum()==1)
	{
		if(jpgenc)
			return (workmode.remainMem-reservedMemLen)*4/5;
		return (workmode.remainMem-reservedMemLen)*1/5;
	}
	else if(usbCameraSum()==0)
	{
		if(jpgenc)
			return workmode.remainMem-reservedMemLen;
	}
	return 0;
}
static int wmm_pop_usb(struct workmode *wm, void *d)
{
	printf("*** %s\n", __func__);
	wmm_dynfree(wm, WM_USB);
	wmm_leave(wm, WM_USB);
	return 1;
}

static int wmm_push_record(struct workmode *wm, void *d)
{
	printf("*** %s\n", __func__);
	wmm_enter(wm, WM_RECORD);

	struct stream *usb1check = stream_find("usb1-check");
	struct stream *usb2check = stream_find("usb2-check");
	struct stream *usbUvc = stream_find("usb-uvc");
	
	struct stream *picCom = stream_find("picCombine");
	wmm_push_stream(wm, picCom);
	struct stream_dest *picComInput = stream_self_get_input(picCom->self, 0);
	stream_dest_plugin(picComInput, usbUvc);
	struct stream *jpgAuto = stream_find("jpg-auto");	
	struct stream *adPcm = stream_find("auadc-pcm");
	wmm_push_stream(wm,adPcm );
	struct stream *record = stream_find("recorder");
	wmm_push_stream(wm, record);

	struct stream_dest *recordInput = stream_self_get_input(record->self, 0);
	stream_dest_plugin(recordInput, jpgAuto);
	recordInput = stream_self_get_input(record->self, 2);
	stream_dest_plugin(recordInput, usb1check);

	struct stream *usbUvc2 = stream_find("usb-uvc2");
	
	recordInput = stream_self_get_input(record->self, 3);
	stream_dest_plugin(recordInput, usb2check);
	struct stream_dest *picComInput1 = stream_self_get_input(picCom->self, 1);
	stream_dest_plugin(picComInput1, usbUvc2);
#if (0 == LCD_EN)
	struct stream *yuvCsiUvcStream = stream_find("yuv-csiuvc");
	wmm_push_stream(wm, yuvCsiUvcStream);
	struct stream_dest *yuvCsiUvcInput = stream_self_get_input(yuvCsiUvcStream->self, 0);
	stream_dest_plugin(yuvCsiUvcInput, usbUvc);
	yuvCsiUvcInput = stream_self_get_input(yuvCsiUvcStream->self, 1);
	stream_dest_plugin(yuvCsiUvcInput, usbUvc2);
	struct stream *test = stream_find("test-stream");
	struct stream_dest *testInput = stream_self_get_input(test->self, 0);
	stream_dest_plugin(testInput, yuvCsiUvcStream);
#endif	

	printf("%s malloc memery\n", __func__);
	wmm_adjusStreamMemStart(wm, jpgAuto);
	wmm_push_stream(wm, jpgAuto);
#if(USB1_CAMERA)
	wmm_push_stream(wm, usbUvc);
#endif
#if(USB2_CAMERA)
	wmm_push_stream(wm, usbUvc2);
#endif
	wmm_dynalloc(wm, WM_RECORD);
#if LCD_EN
	streamSetRuning(picComInput);
	streamSetRuning(picComInput1);
#else
	streamSetRuning(testInput);
#endif
	mdSetEn(getMdEn());
	return 0;
}
static int wmm_pop_record(struct workmode *wm, void *d)
{	
	mdSetEn(0);
#if 1
#if (0 == LCD_EN)
	struct stream *test = stream_find("test-stream");
	struct stream_dest *testInput = stream_self_get_input(test->self, 0);
	streamSetPause(testInput);
	streamDestFreeFrames(testInput, 0);
	stream_dest_plugout(testInput);
#endif
	struct stream *picCom = stream_find("picCombine");
	struct stream_dest *picComInput = stream_self_get_input(picCom->self, 0);
	streamSetPause(picComInput);
	streamDestFreeFrames(picComInput, 0);
	stream_dest_plugout(picComInput);
	picComInput = stream_self_get_input(picCom->self, 1);
	streamSetPause(picComInput);
	streamDestFreeFrames(picComInput, 0);
	stream_dest_plugout(picComInput);
	//struct stream *test = stream_find("test-stream");
	//struct stream_dest *testInput = stream_self_get_input(test->self, 0);
	//streamSetPause(testInput);
#endif
	recordUnintMsg();
	wmm_dynfree(wm, WM_RECORD);
	wmm_leave(wm, WM_RECORD);
	printf("%s end\n", __FUNCTION__);
}

static int wmm_push_wifi(struct workmode *wm, void *d)
{
	int len, i;
	u8 mac[6];
	printf("*** %s\n", __func__);
	wmm_enter(wm, WM_WIFI);
	//struct stream *usbUvc = stream_find("usb-uvc");
	//wmm_push_stream(wm, usbUvc);
	// CONFIG & ALLOC
	wmm_dynalloc(wm, WM_WIFI);
	//streamSetSatus1(usbUvc,S_INPUT_RUNNING);
	// test_wifi_open(NULL);
	// 判断wifi是否还在打开
#if WIFI_EN

	//wifi_init();
	//wifi_start(0);
#endif
	return 1;
}
static int wmm_pop_wifi(struct workmode *wm, void *d)
{
//#if WIFI_EN
//	wifi_stop();
//	wifi_deinit();
//#endif

	// TODO : 雖然wmm_push_wifi不需要set_running，但rtsp會接入conn；因此這裡需要set_running這些conn
	wmm_dynfree(wm, WM_WIFI);
	//  stream_src_dynfree (wm->trans[WM_WIFI].stream);
	wmm_leave(wm, WM_WIFI);
}

static int wmm_push_live(struct workmode *wm, void *d)
{
	printf("%s\n", __FUNCTION__);
	wmm_enter(wm, WM_LIVE);
	struct stream *usb1check = stream_find("usb1-check");
	struct stream *usb2check = stream_find("usb2-check");
	struct stream *usbUvc = stream_find("usb-uvc");
	struct stream *usbUvc2 = stream_find("usb-uvc2");
	struct stream *yuvCsiUvcStream = stream_find("yuv-csiuvc");
	wmm_push_stream(wm, yuvCsiUvcStream);
	struct stream_dest *yuvCsiUvcInput = stream_self_get_input(yuvCsiUvcStream->self, 0);
	stream_dest_plugin(yuvCsiUvcInput, usbUvc);
	yuvCsiUvcInput = stream_self_get_input(yuvCsiUvcStream->self, 1);
	stream_dest_plugin(yuvCsiUvcInput, usbUvc2);
	struct stream *jpgManu1 = stream_find("jpg-manu1");
	wmm_push_stream(wm, jpgManu1);
	struct stream_dest *jpgManu1Input = stream_self_get_input(jpgManu1->self, 0);
	stream_dest_plugin(jpgManu1Input, yuvCsiUvcStream);
	struct stream *jpgAuto = stream_find("jpg-auto");
	
	struct stream *packer = stream_find("rtp-packer");
	wmm_push_stream(wm, packer);
	struct stream *adPcm = stream_find("auadc-pcm");
	wmm_push_stream(wm,adPcm );
	struct stream *packerAudio = stream_find("rtp-audio-packer");
	wmm_push_stream(wm, packerAudio);
	wmm_push_stream(wm, stream_find("rtsp-sender"));
	struct stream *record = stream_find("recorder");
	wmm_push_stream(wm, record);
	
	struct stream_dest *packerInput = stream_self_get_input(packer->self, 0);
	stream_dest_plugin(packerInput, jpgManu1);
	//packerInput = stream_self_get_input(packer->self, 1);
	//stream_dest_plugin(packerInput, jpgAuto);
	packerInput = stream_self_get_input(packer->self, 1);
	stream_dest_plugin(packerInput, usb1check);
	packerInput = stream_self_get_input(packer->self, 2);
	stream_dest_plugin(packerInput, usb2check);
	struct stream_dest *packerAudacInput = stream_self_get_input(packerAudio->self, 0);
	stream_dest_plugin(packerAudacInput, adPcm);
	struct stream_dest *recordInput = stream_self_get_input(record->self, 0);
	stream_dest_plugin(recordInput, jpgAuto);
	recordInput = stream_self_get_input(record->self, 2);
	stream_dest_plugin(recordInput, usb1check);
	recordInput = stream_self_get_input(record->self, 3);
	stream_dest_plugin(recordInput, usb2check);
	struct stream *test = stream_find("test-stream");
	struct stream_dest *testInput = stream_self_get_input(test->self, 0);
	stream_dest_plugin(testInput, jpgManu1);
	//struct stream_dest *testInput1 = stream_self_get_input(test->self, 1);
	//stream_dest_plugin(testInput1, usbUvc2);

	wmm_adjusStreamMemStart(wm, jpgAuto);
	wmm_push_stream(wm, jpgAuto);
#if(USB1_CAMERA)
	wmm_push_stream(wm, usbUvc);
#endif
#if(USB2_CAMERA)
	wmm_push_stream(wm, usbUvc2);
#endif
	wmm_dynalloc(wm, WM_LIVE);
	streamSetRuning(testInput);
	//streamSetRuning(testInput1);
	mdSetEn(getMdEn());
}
static int wmm_pop_live(struct workmode *wm, void *d)
{
	struct stream *test = stream_find("test-stream");
	struct stream *packer = stream_find("rtp-packer");
	struct stream_dest *testInput = stream_self_get_input(test->self, 0);
	//struct stream_dest *testInput1 = stream_self_get_input(test->self, 1);
	mdSetEn(0);
	streamSetPause(testInput);
	//streamSetPause(testInput1);
	stream_dest_plugout(testInput);
	//stream_dest_plugout(testInput1);
	struct stream_dest *packerInput = stream_self_get_input(packer->self, 0);
	streamSetPause(packerInput);
	streamDestFreeFrames(packerInput, 0);
	stream_dest_plugout(packerInput);
	packerInput = stream_self_get_input(packer->self, 1);
	streamSetPause(packerInput);
	streamDestFreeFrames(packerInput, 0);
	stream_dest_plugout(packerInput);
	packerInput = stream_self_get_input(packer->self, 2);
	streamSetPause(packerInput);
	streamDestFreeFrames(packerInput, 0);
	stream_dest_plugout(packerInput);
//	packerInput = stream_self_get_input(packer->self, 3);
//	streamSetPause(packerInput);
//	streamDestFreeFrames(packerInput, 0);
//	stream_dest_plugout(packerInput);
	recordUnintMsg();
	wmm_dynfree(wm, WM_LIVE);
	wmm_leave(wm, WM_LIVE);
	printf("%s end\n", __FUNCTION__);
}

static int wmm_push_webfile(struct workmode *wm, void *d)
{
	wmm_enter(wm, WM_WEBFILE);
	struct stream *readFile = stream_find("read-mediafile");
	wmm_push_stream(wm, readFile);
	struct stream *jpgFile = stream_find("jpg-file");
	wmm_push_stream(wm, jpgFile);
	
	struct stream *jpgSwRes = stream_find("jpg-swres");
	wmm_push_stream(wm, jpgSwRes);
	
	struct stream *packer = stream_find("rtp-packer");
	wmm_push_stream(wm, packer);
	struct stream *adPcm = stream_find("auadc-file");
	wmm_push_stream(wm,adPcm );
	struct stream *packerAudio = stream_find("rtp-audio-packer");
	wmm_push_stream(wm, packerAudio);
	wmm_push_stream(wm, stream_find("rtsp-sender"));

	struct stream_dest *jpgSwResInput = stream_self_get_input(jpgSwRes->self, 0);
	stream_dest_plugin(jpgSwResInput, jpgFile);
	
	struct stream_dest *packerInput = stream_self_get_input(packer->self, 0);
	//stream_dest_plugin(packerInput, jpgFile);
	stream_dest_plugin(packerInput, jpgSwRes);
	struct stream_dest *packerAudacInput = stream_self_get_input(packerAudio->self, 0);
	stream_dest_plugin(packerAudacInput, adPcm);
	wmm_dynalloc(wm, WM_WEBFILE);
}

static int wmm_pop_webfile(struct workmode *wm, void *d)
{
	wmm_dynfree(wm, WM_WEBFILE);
	wmm_leave(wm, WM_WEBFILE);
	printf("%s end\n", __FUNCTION__);
}

extern struct workmode workmode;

static int wmm_push_playback(struct workmode *wm, void *d)
{
	printf("*** %s\n", __func__);
	wmm_enter(wm, WM_PLAYBACK);
	wmm_push_stream(wm, stream_find("read-mediafile"));
	wmm_dynalloc(wm, WM_PLAYBACK);
	return 0;
}

static int wmm_pop_playback(struct workmode *wm, void *d)
{
	wmm_dynfree(wm, WM_PLAYBACK);
	wmm_leave(wm, WM_PLAYBACK);
}


DEF_MODE_PARENT(wmp_none, WM_NONE, WM_NONE);
DEF_MODE_PARENT(wmp_record, WM_RECORD, WM_NONE);
DEF_MODE_PARENT(wmp_wifi, WM_WIFI, WM_NONE);
DEF_MODE_PARENT(wmp_live, WM_LIVE, WM_WIFI);
DEF_MODE_PARENT(wmp_webfile, WM_WEBFILE, WM_WIFI);
DEF_MODE_PARENT(wmp_playback, WM_PLAYBACK, WM_NONE);
DEF_MODE_PARENT(wmp_usb, WM_USB, WM_NONE);

static struct mode_trans mode_trans[] = {
	[WM_NONE] = {
		.path = wmp_none,
	},
	[WM_RECORD] = {
		.path = wmp_record,
		.push = wmm_push_record,
		.pop = wmm_pop_record,
		.priv = NULL,
	},
	[WM_WIFI] = {
		.path = wmp_wifi,
		.push = wmm_push_wifi,
		.pop = wmm_pop_wifi,
		.priv = NULL,
	},
	[WM_LIVE] = {
		.path = wmp_live,
		.push = wmm_push_live,
		.pop = wmm_pop_live,
		.priv = NULL,
	},
	[WM_WEBFILE] = {
		.path = wmp_webfile,
		.push = wmm_push_webfile,
		.pop = wmm_pop_webfile,
		.priv = NULL,
	}, //
	[WM_PLAYBACK] = {
		.path = wmp_playback,
		.push = wmm_push_playback,
		.pop = wmm_pop_playback,
		.priv = NULL,
	},

	[WM_USB] = {
		.path = wmp_usb,
		.push = wmm_push_usb,
		.pop = wmm_pop_usb,
		.priv = NULL,
	},

};
struct workmode workmode = {
	.mode_num = WM_MAX,
	.trans = mode_trans,
};
	
uint8_t *workmode_heap;
u32 WORKMODE_HEAPSIZE;

uint8_t wmm_init_flag = 0;
struct stream_src *stream_heap[64];
void workModeMemInit(u8 *addr,u32 size)
{
	workmode_heap=addr;
	WORKMODE_HEAPSIZE=size;
}
u8 *get_workmode_heap()
{
	return workmode_heap;
}

uint8_t *get_mode_heap_last()
{
	return workmode_heap + WORKMODE_HEAPSIZE - 2 * 1024 * 1024;
}

void test_workmode(void)
{
	wmm_init(&workmode, stream_heap, workmode_heap, workmode_heap + WORKMODE_HEAPSIZE);
	wmm_init_flag = 1;
	// printf("test_workmod end!\r\n");
}

void flight_workmode(void) // 飞控模式
{
	// default_sensor_setting();     //目前不知道这里改变了什么
	wmm_init(&workmode, stream_heap, workmode_heap, workmode_heap + WORKMODE_HEAPSIZE);
	wmm_trans_mode(&workmode, WM_WIFI);
	wmm_init_flag = 1;
}

extern void check_mdt_rec_live(void *d);
void live_mode_trans(void)
{
	printf("*** %s\n", __func__);
	if (workmode.curr_mode != WM_LIVE)
	{
		workmode.trans[WM_LIVE].priv = workmode.curr_mode;
		wmm_trans_mode(&workmode, WM_LIVE);
	}
}

void print_current_workmode()
{
	printf("workmode.curr_mode:%d\r\n", workmode.curr_mode);
}

void live_mode_pop(void)
{
	printf("*** %s\n", __func__);

	if (workmode.curr_mode == WM_LIVE)
	{
		wmm_trans_mode(&workmode, WM_WIFI);
	}
}

u8 workmode_is_live()
{
	if (workmode.curr_mode == WM_LIVE)
		return 1;
	else
		return 0;
}

u8 workmode_is(u8 mode)
{
	// printf("curr_mode:%d\t%d\r\n",workmode.curr_mode,mode);
	if (workmode.curr_mode == mode)
		return 1;
	else
		return 0;
}

uint32_t get_current_workmode()
{
	return workmode.curr_mode;
}

void webfile_mode_trans(void)
{
	printf("*** %s\n", __func__);
	workmode.trans[WM_WEBFILE].priv = workmode.curr_mode;
	wmm_trans_mode(&workmode, WM_WEBFILE);
}
void wifi_mode_trans(void)
{
	printf("*** %s\n", __func__);
	wmm_trans_mode(&workmode, WM_WIFI);
}

u8 *getCurModeFreeBuffBase(void)
{
	return workmode.curr_base;
}

u32 modeFreeSpace(void)
{
	return workmode.end-workmode.curr_base;
}

