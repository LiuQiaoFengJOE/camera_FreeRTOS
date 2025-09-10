#ifndef  __APP_H
#define  __APP_H

#include "task/taskInfor.h"
#include "stream/stream.h"
#include "sysCommon/sysCommonEnum.h"
#include "task/resInfor.h"
#include "task/config.h"
#include "resource/resource.h"
#include "osal/osal_alloc.h"
#include "ax329x_soc_drv.h"
//#include "task/game/xos/xmsgq.h"
//#include "game/display.h"


extern struct workmode workmode;


enum
{
	SYS_OPEN_WINDOW =1,
	SYS_CLOSE_WINDOW,
	SYS_CHILE_COLSE,
	SYS_CHILE_OPEN,
	SYS_TOUCH_WINDOW,
	SYS_DRAW_UI,
	SYS_EVENT_1S,
	SYS_EVENT_GAME_TICK,
	SYS_EVENT_50MS,
	SYS_EVENT_500MS,
	SYS_TIME_UPDATE,
	SYS_EVENT_BAT,
	SYS_EVENT_MD,
	SYS_EVENT_SDC,
	SYS_EVENT_USB,
	SYS_EVENT_USENSOR,
	SYS_EVENT_ASTERN,
	VID_EVENT_REC_START,
	VID_EVENT_REC_STOP,
	VID_EVENT_REC_RESTART,
	VID_EVENT_REC_ERROR,
	VID_EVENT_LOCK_FILE,
	VID_EVENT_IDENTIFY_UPDATE,
	VID_EVENT_IDENTIFY_FINISH,
	VID_EVENT_LIVE_PREVIEW,
	PLAY_EVENT_PLAYSTART,
	PLAY_EVENT_PLAYPAUSE,
	PLAY_EVENT_PLAYRESUME,
	PLAY_EVENT_PLAYSTOP,
	SYS_WIFI_CHANGE,
	SYS_SLIDING_GESTRUE,
	SYS_SLID_RELEASE,
	SYS_WIFI_CONFIG,
	SYS_ZBAR_IDENTIFY_RESUALT,
	SYS_ZBAR_IDENTIFY_END,
	VID_EVENT_MP3_PLAY,
	SYS_EVENT_MAX
};
enum
{
	KEY_EVENT_START=SYS_EVENT_MAX,
	KEY_EVENT_OK=KEY_EVENT_START,
	KEY_EVENT_UP,
	KEY_EVENT_LEFT,
	KEY_EVENT_RIGHT,
	KEY_EVENT_TOUCH_OK,
	KEY_EVENT_TOUCH_BACK,
	KEY_EVENT_OUT,
	KEY_EVENT_DOWN,
	KEY_EVENT_MENU,
	KEY_EVENT_MODE,
	KEY_EVENT_POWER,
	KEY_EVENT_RETURN,
	KEY_EVENT_POWEROFF,
	KEY_EVENT_RF_RECV,
	KEY_EVENT_LONG_OK,
	KEY_EVENT_RECORD_START,
	KEY_EVENT_RECORD_STOP,
	KEY_EVENT_PHOTO,
	KEY_EVENT_LONG_PHOTO,
	KEY_EVENT_SETTING,
	KEY_EVENT_ZOOMIN,
	KEY_EVENT_ZOOMOUT,
	KEY_EVENT_AF,
	KEY_EVENT_END,
	EVENT_MAX=KEY_EVENT_END,
};
enum
{
	KEY_PRESSED=0,
	KEY_CONTINUE,
	KEY_RELEASE,
	KEY_STATE_INVALID,
};

enum
{
	TASK_MSG_DATA_INPUT=1,
	TASK_MSG_WAIT_DONE,
	TASK_MSG_EXT_CLIENT_FUNC,
	TASK_MSG_TAKE_PHOTO_START,
	TASK_MSG_VID_REC_START,
	TASK_MSG_VID_REC_STOP,
	TASK_MSG_VID_LOCK,
	TASK_MSG_READ_IMAGE,
	TASK_MSG_READ_AUDIO,
	TASK_MSG_READ_START,
	TASK_MSG_READ_STOP,
	TASK_MSG_READ_NEXT,
	TASK_MSG_READ_PRE,
	TASK_MSG_READ_NEXT_JPG,
	TASK_MSG_READ_PRE_JPG,
	TASK_MSG_READ_CUR_FILE,	
	TASK_MSG_READ_FASTFORWARD,
	TASK_MSG_READ_FASTBACKWARD,
	TASK_MSG_READ_SEEKAUDIO,
	TASK_MSG_PLAY_IMAGE,
	TASK_MSG_PLAY_AUDIO,
	TASK_MSG_PLAY_START,
	TASK_MSG_PLAY_STOP,
	TASK_MSG_PLAY_PAUSE,
	TASK_MSG_PLAY_RESUME,
	TASK_MSG_PLAY_NEXT,
	TASK_MSG_PLAY_PRE,
	TASK_MSG_PLAY_NEXT_JPG,
	TASK_MSG_PLAY_PRE_JPG,
	TASK_MSG_REPLAY_CUR_FILE,
	TASK_MSG_PLAY_FASTFORWARD,
	TASK_MSG_PLAY_FASTBACKWARD,
	TASK_MSG_USBDEV_MSC,
	TASK_MSG_USBDEV_UVC_ON,
	TASK_MSG_USBDEV_UVC_OFF,
	TASK_MSG_UNINIT,
	TASK_MSG_AUD_REC_START,
	TASK_MSG_AUD_REC_STOP,
	TASK_MSG_READ_AUD_NEXT,
	TASK_MSG_READ_AUD_PRE,
	TASK_MSG_PLAY_AUD_START,
	TASK_MSG_PLAY_AUD_STOP,
	TASK_MSG_PLAY_AUD_PAUSE,
	TASK_MSG_PLAY_AUD_RESUME,
	TASK_MSG_PLAY_AUD_NEXT,
	TASK_MSG_PLAY_AUD_PRE,
	TASK_MSG_DOWNLOAD_READ,
	TASK_MSG_DOWNLOAD_READ_WAIT,
	TASK_MSG_PRINTER_JPG,
	TASK_MSG_FRAME_DOWN,
	TASK_MSG_TAKE_PHOTO_FINISH,
	TASK_MSG_YUV_DUMP,
	TASK_MSG_ZBAR_START,
	TASK_MSG_ZBAR_IDENTIFY,
	TASK_MSG_ZBAR_STOP,
};

enum
{
	PLAY_BACK_STOP=0,
	PLAY_BACK_PLAYING,
	PLAY_BACK_PAUSE,
	PLAY_BACK_MAX
};


#define makeEvent(type,value)  ((type)|((value)<<16))
#define getType(msg)        (((uint32)msg)&0xffff)
#define getValue(msg)        ((((uint32)msg)>>16)&0xffff)


typedef struct System_Ctrl_S
{
	INT8U powerflag;   // power on flag.  key/dcin/gsensor/RTC/...
	INT8U battery;     // battery state;
	INT8U t_usb;           // usb last stat. no usb,dcin,usb-pc
	INT8U usb;           // usb stat. no usb,dcin,usb-pc
	INT8U usensor;     // usensor state
	
	INT8U sdcard;      // sdcard stat.no sdc,sdc unstable,sdc error,sdc full,sdc normal
	INT8U g_lock;      // g sensor active flag
	INT8U f_keysound;  //keysound flag
	INT8U f_update;   //flag update 
	
	INT8U pip_mode;
	INT8U crop_level;	// lcd crop image scale show,
	INT8U astern;     // astern
	INT8U usensorshow;  // usensor show flag
	INT8U bfolder;    // videob folder flag
	INT8U photo_effect_idx;
	
	INT32S t_screensave;      // screen save flag
//	INT32S t_autooff;       // auto power off time
	INT32U t_remain;	     // sdc record time
	INT32U t_rec;           // loop rec time
	INT32U t_mrec;
//	INT32U t_ir_flag;		//ir flag: 0: ir close , 1: ir open
	INT32U t_ir_auto;		//ir flag: 0: ir close , 1: ir auto 
	INT32U powerOnTime;

	INT32U sd_freesize;  //sdc free size KB
	INT32U fs_clustsize; // fs cluster size

	INT32U t_wait;    // wait csi & isp stable
	
	u8 volume_level;	// 0~10

	u8 cartoon_mode;	// 0: not cartoon , 1: cartoon in mdoe , 2: cartoon out mode
	u8 cartoon_show_cnt;	
	u8 photoMode;
	u8 ailangid;
	u8 aiuserRole;
	u8 ainetType;
	u8 timeStamp;
	//char *aiPicName;
	void *sysQ;
}System_Ctrl_T;
extern System_Ctrl_T SysCtrl;
enum
{
	BATTERY_STAT_0=0,
	BATTERY_STAT_1,
	BATTERY_STAT_2,
	BATTERY_STAT_3,
	BATTERY_STAT_4,
	BATTERY_STAT_5,
	BATTERY_STAT_MAX
};
enum
{
	USB_STAT_NULL = 0,
	USB_STAT_DCIN,
	USB_STAT_PC,
	USB_STAT_MAX
};
enum
{
	SDC_STAT_NULL = 0,
	SDC_STAT_UNSTABLE,
	SDC_STAT_ERROR,
	SDC_STAT_FULL,
	SDC_STAT_NORMAL,
	SDC_STAT_MAX
};

//-----------astern stat-----------------
enum
{
	ASTERN_OFF=0,
	ASTERN_ON
};

enum PHOTO_MODE
{
	PHOTO_NORMAL=0,
    PHOTO_IDCONTENT,
    PHOTO_LIFTOON,
    PHOTO_LIF,

	PHOTO_SMALL_MODE,
	PHOTO_AI_CHAT,
	
    PHOTO_MODE_MAX,
};


#define USB1_CAMERA  0 //If not using USB1_CAMERA, define 0,Otherwise, it will use memory 
#define USB2_CAMERA  0 //If not using USB2_CAMERA, define 0,Otherwise, it will use memory
#define WIFI_EN   1
#define JR_APP_PROTOCOL  0

#define TP_FUCTION 	 0
#define PIC_STICKER_EN   1
#define BIG_PIC_EN   1
#define EFFECTS_ANIMATION_EN 1
#define GAME_FUNC_EN 1
#define CROPLEVEMAX 4
void getVidRes(u16 *w,u16 *h);
void getPhotoRes(u16 *w,u16 *h);
u32 getFrameSizeByRes(u32 w,u32 h);
u32 getVidRecTime(void);
u32 getVidRecLoopEn(void);
u32 getVidRecAudioEn(void);
u32 getWifiVidRes(u16 *w,u16 *h);
u32 getWifiVidQualiy(void);
u32 getVidWaterMarkEn(void);
u32 getPhotoWaterMarkEn(void);
u32 getNetWaterMarkEn(void);
int uintToString(char* str,uint8 len,uint32 data);
int intToStringFill0(char* str,uint8 len,uint32 data,u32 sum);
char* debugDataString(char* str,u32 (*getData)(u32,u32*));
int usbCameraRes(u16 *w,u16 *h);
int usbCameraSum(void);
int useUsbCamera(void);
int usb1CameraWork(void);
int usb2CameraWork(void);
void usb1CameraPowerEn(int en);
void usb2CameraPowerEn(int en);
u32 getJpgencOrUsbMem(u32 jpgenc);
int jrAppProtocol(void);
int support_rf_recv(void);
u32 getMdEn(void);
void mdSetEn(u32 d);
u32 curFuncPhoto(void);
int curFuncWifi(void);
int takePhotoHighResEn(void);
void setNotplayKeySoundFlag(u8 flag);



#endif


