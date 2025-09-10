#ifndef __TCP_MESSAGE_H__
#define __TCP_MESSAGE_H__

#include <stdint.h>
#include <stdlib.h>
#include "typedef.h"

#include "debug.h"

#include "tcpControl.h"
#include "systemSetting/sysCommon.h"

// !!! This file sync with iOS and Android !!!

// Message ID indicated message type and its components

/* System */ /* 0x00-0x07 */

#define MSG_ID_HEARTBEAT 0x00 // empty body

#define MSG_ID_REQUEST_REPORT 0x01

#define MSG_ID_REPORT 0x02 // JSON, (Client <-> Server)

#define MSG_ID_DEVICE_STATUS 0x03 // device status
#define DEVICE_STATUS_DISCONNECTED 0x00
#define DEVICE_STATUS_IDLE 0x01
#define DEVICE_STATUS_BUSY 0x02
#define DEVICE_STATUS_NO_SDCARD 0x03
#define DEVICE_STATUS_INSUFFICIENT_STORAGE 0x04

#define MSG_ID_CARD_STATUS 0x04 // sdcard status
#define CARD_STATUS_NONE 0x00
#define CARD_STATUS_OK 0x01
#define CARD_STATUS_UNFORMMATTED 0x02

#define MSG_ID_TIME_CALIBRATION 0x05 // Year/Month/Day Hour:Minute:Second, 1 byte per field, Year from 2000

// 子ID按时最大支持到0x07
#define MSG_ID_DEVICE_FUNCTION 0x06		// 位处理
#define DEVICE_FUNCTION_CARD_PHOTO 0x00 // 支持拍照
#define DEVICE_FUNCTION_CARD_VIDEO 0x01 // 支持录像

/* Preview */ /* 0x08-0x0F */

#define MSG_ID_PREVIEW_RESOLUTION 0x08
#define PREVIEW_RESOLUTION_SD 0x00
#define PREVIEW_RESOLUTION_HD 0x01
#define PREVIEW_RESOLUTION_FHD 0x02

#define MSG_ID_PREVIEW_QUALITY 0x09
#define PREVIEW_QUALITY_LOW 0x00
#define PREVIEW_QUALITY_MID 0x01
#define PREVIEW_QUALITY_HIGH 0x02

#define MSG_ID_PREVIEW_SOUND 0x0A
#define PREVIEW_SOUND_TOGGLE 0x00
#define PREVIEW_SOUND_OFF 0x01
#define PREVIEW_SOUND_ON 0x02

/* Video */ /* 0x10-0x17 */

#define MSG_ID_VIDEO_RESOLUTION 0x10
#define VIDEO_RESOLUTION_SD 0x00
#define VIDEO_RESOLUTION_HD 0x01
#define VIDEO_RESOLUTION_FHD 0x02

#define MSG_ID_VIDEO_QUALITY 0x11
#define VIDEO_QUALITY_LOW 0x00
#define VIDEO_QUALITY_MID 0x01
#define VIDEO_QUALITY_HIGH 0x02

#define MSG_ID_VIDEO_SOUND 0x12
#define VIDEO_SOUND_OFF 0x00
#define VIDEO_SOUND_ON 0x01

#define MSG_ID_CYCLIC_RECORD 0x13
#define CYCLIC_RECORD_OFF 0x00
#define CYCLIC_RECORD_1MIN 0x01
#define CYCLIC_RECORD_2MIN 0x02
#define CYCLIC_RECORD_3MIN 0x03
#define CYCLIC_RECORD_4MIN 0x04
#define CYCLIC_RECORD_5MIN 0x05
#define CYCLIC_RECORD_10MIN 0x06

/* Photo */ /* 0x18-0x1F */

#define MSG_ID_PHOTO_RESOLUTION 0x18
#define PHOTO_RESOLUTION_SD 0x00
#define PHOTO_RESOLUTION_HD 0x01
#define PHOTO_RESOLUTION_FHD 0x02
#define PHOTO_RESOLUTION_QHD 0x03
#define PHOTO_RESOLUTION_UHD 0x04

#define MSG_ID_PHOTO_QUALITY 0x19
#define PHOTO_QUALITY_LOW 0x00
#define PHOTO_QUALITY_MID 0x01
#define PHOTO_QUALITY_HIGH 0x02

#define MSG_ID_PHOTO_BURST 0x1A
#define PHOTO_BURST_OFF 0x00
#define PHOTO_BURST_2 0x01
#define PHOTO_BURST_3 0x02
#define PHOTO_BURST_5 0x03
#define PHOTO_BURST_10 0x04

#define MSG_ID_PHOTO_TIMELAPSE 0x1B
#define PHOTO_TIMELAPSE_OFF 0x00
#define PHOTO_TIMELAPSE_2 0x01
#define PHOTO_TIMELAPSE_3 0x02
#define PHOTO_TIMELAPSE_5 0x03
#define PHOTO_TIMELAPSE_10 0x04
#define PHOTO_TIMELAPSE_15 0x05
#define PHOTO_TIMELAPSE_20 0x06
#define PHOTO_TIMELAPSE_25 0x07
#define PHOTO_TIMELAPSE_30 0x08

/* Visual Effect */ /* 0x20-0x3F */

#define MSG_ID_WHITE_BALANCE 0x20
#define WHITE_BALANCE_AUTO 0x00
#define WHITE_BALANCE_DAYLIGHT 0x01
#define WHITE_BALANCE_CLOUDY 0x02
#define WHITE_BALANCE_SHADE 0x03
#define WHITE_BALANCE_FLASH 0x04
#define WHITE_BALANCE_TUNGSTEN 0x05
#define WHITE_BALANCE_FLUORESCENT 0x06

#define MSG_ID_EXPOSURE_COMPENSATION 0x21
#define EXPOSURE_COMPENSATION_1 0x00
#define EXPOSURE_COMPENSATION_2 0x01
#define EXPOSURE_COMPENSATION_3 0x02
#define EXPOSURE_COMPENSATION_4 0x03
#define EXPOSURE_COMPENSATION_5 0x04

#define MSG_ID_SHARPNESS 0x22
#define SHARPNESS_SOFT 0x00
#define SHARPNESS_NORMAL 0x01
#define SHARPNESS_STRONG 0x02

#define MSG_ID_ISO 0x23 // todo: define

#define MSG_ID_ANTIBANDING 0x24
#define ANTIBANDING_OFF 0x00
#define ANTIBANDING_50HZ 0x01
#define ANTIBANDING_60HZ 0x02
#define ANTIBANDING_AUTO 0x03

#define MSG_ID_WDR 0x25
#define WDR_OFF 0x00
#define WDR_ON 0x01

/* Control */ /* 0x40-0x4F */

#define MSG_ID_RECORD_VIDEO 0x40
#define RECORD_VIDEO_TOGGLE 0x00
#define RECORD_VIDEO_START 0x01
#define RECORD_VIDEO_STOP 0x02

#define MSG_ID_TAKE_PHOTO 0x41			// empty body
#define TAKE_PHOTO_COUNTDOWN_TIMER 0x00 // 数值代表倒计时数值,只有固件使用

#define MSG_ID_GET_SENSOR_RESOLUTION 0x42
#define MSG_ID_SET_SENSOR_RESOLUTION 0x43

/* Common */ /* 0x50-0x7F */

#define MSG_ID_WIFI_SETTINGS 0x50 // JSON

#define MSG_ID_LANGUAGE 0x51
#define LANGUAGE_EN_US 0x00
#define LANGUAGE_ZH_CN 0x01
#define LANGUAGE_ZH_TW 0x02
#define LANGUAGE_JA_JP 0x03

#define MSG_ID_MOTION_DETECTION 0x52
#define MOTION_DETECTION_OFF 0x00
#define MOTION_DETECTION_ON 0x01

#define MSG_ID_ANTI_SHAKE 0x53
#define ANTI_SHAKE_OFF 0x00
#define ANTI_SHAKE_ON 0x01

#define MSG_ID_DATE_STAMP 0x54
#define DATE_STAMP_OFF 0x00
#define DATE_STAMP_ON 0x01

#define MSG_ID_SCREEN_SAVER 0x55
#define SCREEN_SAVER_OFF 0x00
#define SCREEN_SAVER_1MIN 0x01
#define SCREEN_SAVER_3MIN 0x02
#define SCREEN_SAVER_5MIN 0x03

#define MSG_ID_ROTATION 0x56
#define ROTATION_OFF 0x00
#define ROTATION_ON 0x01

#define MSG_ID_AUTO_SHUTDOWN 0x57
#define AUTO_SHUTDOWN_OFF 0x00
#define AUTO_SHUTDOWN_1MIN 0x01
#define AUTO_SHUTDOWN_3MIN 0x02
#define AUTO_SHUTDOWN_5MIN 0x03

#define MSG_ID_BUTTON_SOUND 0x58
#define BUTTON_SOUND_OFF 0x00
#define BUTTON_SOUND_ON 0x01

#define MSG_ID_OSD_MODE 0x59
#define OSD_MODE_OFF 0x00
#define OSD_MODE_ON 0x01

#define MSG_ID_CAR_MODE 0x5A
#define CAR_MODE_OFF 0x00
#define CAR_MODE_ON 0x01

#define MSG_ID_FORMAT_CARD 0x5B // empty body

#define MSG_ID_FACTORY_RESET 0x5C // empty body

#define MSG_ID_BATTERY 0x5D // 后面加一字节数据,最高位为1代表充电,其他位为电量

/* data type */

typedef struct
{
	uint8_t id;
	uint8_t sessionId;
	uint32_t length;
	uint8_t *info;
	uint8_t *name;
	void *context;
} message_t;

// return value: 0 succeed, others failed
typedef int (*MessageHandler)(message_t *message);

typedef struct
{
	uint8_t id;
	const uint8_t *name;
	MessageHandler handler;
} MessageMethod;

/* function */

int process_message(message_t *message);

// return value: 0 succeed, -1 malloc error, -2 network error
int send_message(struct tcpControl *control, message_t *message);
void *copy_message(message_t *message);
uint8_t get_APPID(const char *name);
void *all_device_status();

void *get_device_status(uint8_t value);
void *get_uvc_msg(uint8_t type);
void *need_send_dev_msg(uint8_t type);

#endif
