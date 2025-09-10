#ifndef _SYSTEMSETTING_H
#define _SYSTEMSETTING_H

#include "tcpControl/tcpMessage.h"
#include "sysCommon.h"
#include "fatfs/ff.h"
#include "sysCommon/sysCommonEnum.h"
#include "sysCommon/CommonInclude.h"

/*
在tcpMessage.h中定义
//子ID按时最大支持到0x07
#define MSG_ID_DEVICE_FUNCTION      0x06		//位处理
#define     DEVICE_FUNCTION_CARD_PHOTO  0x00		//支持拍照
#define     DEVICE_FUNCTION_CARD_VIDEO  0x01		//支持录像


demo:
			#define SUPPORT_MODE	(1<<DEVICE_FUNCTION_CARD_PHOTO | 1<<DEVICE_FUNCTION_CARD_VIDEO)
*/

// app上是否显示拍照与录像的按钮(告诉app该固件是否支持本地拍照与录像)
// 暂时最大支持8种模式
// #define SUPPORT_MODE	(1<<DEVICE_FUNCTION_CARD_PHOTO | 1<<DEVICE_FUNCTION_CARD_VIDEO)
#define SUPPORT_MODE (0)

#define DEVICE_NORMAL 0x00
#define DEVICE_BUSY 0x01
#define DEVICE_NO_SDCARD 0x02
#define DEVICE_SDCARD_NOT_ENOUGH 0x03
#define DEVICE_NO_REPLY 0xff

struct tcpMessage
{
	uint8_t *sendbuf;
	uint32_t length;
	int fd;
};



#endif
