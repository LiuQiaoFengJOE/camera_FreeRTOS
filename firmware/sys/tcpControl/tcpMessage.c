#include "tcpMessage.h"

#include "osal/osal_socket.h"
#include "osal/osal_alloc.h"

#include "cjson/cjson.h"

#include "systemSetting/systemSetting.h"
#include "task/app.h"

// ---------------------------------------------------------------------------

static void printJson(cJSON *root)
{
	int i;

	for (i = 0; i < cJSON_GetArraySize(root); i++)
	{
		cJSON *item = cJSON_GetArrayItem(root, i);
		int itemType = item->type;

		if (cJSON_Object == itemType)
			printJson(item);
		else
		{
			printf("%s->", item->string);
			printf("%s\r\n", cJSON_Print(item));
		}
	}
}

// ---------------------------------------------------------------------------
// 1:err		0:normal

/* system */

static int handle_heartbeat(message_t *message)
{
	// heartbeat
	printf("Heartbeat, pulse!\r\n"); // now for debug use

	return 0;
}

static int handle_report(message_t *message)
{
	cJSON *json = cJSON_Parse(message->info);
	if (json == NULL)
	{
		printf("handle_report: cJSON_Parse error\r\n");
		return -1;
	}

	printJson(json);

	cJSON_Delete(json);

	// send test

	struct tcpControl *control = (struct tcpControl *)message->context;
	message_t msg;
	msg.id = MSG_ID_REPORT;
	msg.sessionId = message->sessionId;
	msg.info = message->info;
	msg.length = message->length;
	send_message(control, &msg);

	return 0;
}

// 特殊操作可以不使用handle_cmd,或者将一些特殊操作放在switch最前面,而通用的放在default处理即可
// 基本设置一般只要在msgMethods加入对应的方法即可,其他基本不用修改
// message->name与msgMethods->name一致
// 所谓特殊指的是使用不能用通用的回调函数实现的方法
extern void live_countdown_timer(void *d);
extern void live_add_listen_takephoto_signal(void *d);
struct liveTakePhoto
{
	struct stream *s;
	void *tcpMessage;
	int fd;
};
void user_action(uint8_t type)
{
#if WIFI_EN
#if JR_APP_PROTOCOL
	struct tcpMessage *msg;
	int ret;
	if (tcp_fd_num())
	{
		msg = need_send_dev_msg(type);
		if (msg)
		{
			msg->fd=0xffffffff;
			ret = xQueueSend(get_sender_queue(), msg, 0);
			// printf("battery sendend:%d!\r\n",ret);
			if (!ret)
			{
				osal_free(msg->sendbuf);
			}
			osal_free(msg);
		}
	}
#endif
#endif
}
void deviceState(uint8_t type)
{
#if WIFI_EN
#if JR_APP_PROTOCOL
	struct tcpMessage *msg;
	int ret;
	if (tcp_fd_num())
	{
		msg = get_device_status(type);
		if (msg)
		{
			msg->fd=0xffffffff;
			ret = xQueueSend(get_sender_queue(), msg, 0);
			// printf("battery sendend:%d!\r\n",ret);
			if (!ret)
			{
				osal_free(msg->sendbuf);
			}
			osal_free(msg);
		}
	}
#endif
#endif
}

extern u8 hal_set_timer(u8 *value);
extern int clientTaketPhotoStart(int fd);
extern int menuSetNetMsgVal(u32 netId,u32 Val);
static int handle_cmd(message_t *message)
{
	uint8_t *value = message->info;
	int res = -1;
	struct stream *avi = NULL;
	int gear;
	printf("message->id:%d\r\n", message->id);
	struct tcpControl *tcp = (struct tcpControl *)message->context;
	int fd = tcp->fd;
	struct tcpMessage *tcpMessage;
	switch (message->id)
	{
		// 心跳
	case MSG_ID_HEARTBEAT:
		printf("Heartbeat, free heap:%dKB\n", xPortGetFreeHeapSize()>>10);
		tcpMessage = need_send_dev_msg(MSG_ID_BATTERY);
		if (tcpMessage && tcpMessage->sendbuf)
		{
			int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
			printf("ret:%d\r\n", ret);
			osal_free(tcpMessage->sendbuf);
			osal_free(tcpMessage);
		}
		return 0;
		break;
	// wifi拍照操作
	case MSG_ID_TAKE_PHOTO:
		res = 0;
		if(!workmode_is_live())
			res=DEVICE_BUSY;
		if(get_sd_status()==0)
			res=DEVICE_NO_SDCARD;
		if (res)
		{
			printf("take photo aciton fail!\r\n");
			break;
		}
		if(clientTaketPhotoStart(tcp->fd)==0)
			return 0;
		res=DEVICE_BUSY;
		break;
	case MSG_ID_TIME_CALIBRATION:
		res = hal_set_timer(value);
		break;

	// wifi录像操作
	case MSG_ID_RECORD_VIDEO:
		res = set_rec(value[0]);
		// 如果是正确的,则会填写message,由结束时发送message内容
		// 如果不是正确的值,所有message在结束时赋值
		if (!res)
		{
			if(videoIsRecording())
				value[0] = RECORD_VIDEO_START;
			else
				value[0] = RECORD_VIDEO_STOP;
			res=0;
		}
		break;
	case MSG_ID_GET_SENSOR_RESOLUTION:
		tcpMessage = need_send_dev_msg(message->id);
		if (tcpMessage && tcpMessage->sendbuf)
		{
			int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
			printf("ret:%d\r\n", ret);
			osal_free(tcpMessage->sendbuf);
			osal_free(tcpMessage);
		}

		return 0;
		break;
	case MSG_ID_SET_SENSOR_RESOLUTION:
		res = DEVICE_NORMAL;//global_set_wifi_Pixel(value[0]);
		break;

	default:
		menuSetNetMsgVal(message->id,value[0]);
		res = 0;
		break;
	}

	// 分别对不同情况进行信息返回
	switch (res)
	{
	// 正确,则会在之前设置message的值
	case DEVICE_NORMAL:
		res = send_message(message->context, message); // 正确则回应
		break;
	// 设备忙
	case DEVICE_BUSY:
		tcpMessage = get_device_status(DEVICE_STATUS_BUSY);
		if (tcpMessage && tcpMessage->sendbuf)
		{
			int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
			printf("ret:%d\r\n", ret);
			osal_free(tcpMessage->sendbuf);
			osal_free(tcpMessage);
		}

		break;
	case DEVICE_NO_SDCARD:
		tcpMessage = get_device_status(DEVICE_STATUS_NO_SDCARD);
		if (tcpMessage && tcpMessage->sendbuf)
		{
			int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
			printf("ret:%d\r\n", ret);
			osal_free(tcpMessage->sendbuf);
			osal_free(tcpMessage);
		}

		break;

	case DEVICE_SDCARD_NOT_ENOUGH:
		tcpMessage = get_device_status(DEVICE_STATUS_INSUFFICIENT_STORAGE);
		if (tcpMessage && tcpMessage->sendbuf)
		{
			int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
			printf("ret:%d\r\n", ret);
			osal_free(tcpMessage->sendbuf);
			osal_free(tcpMessage);
		}

		break;

	case DEVICE_NO_REPLY:
		break;
	}
#if 0
		if(!res)
		{
			res = send_message(message->context,message); 		//正确则回应
		}
#endif
	printf("ID:%X\tres:%d\r\n", message->id, res);
	return res;
}

// ---------------------------------------------------------------------------
/* Methods */

static MessageMethod msgMethods[] =
	{
		/* system */
		{MSG_ID_HEARTBEAT, NULL, handle_cmd},
		{MSG_ID_REPORT, NULL, handle_cmd},

		/* Preview */
		{MSG_ID_PREVIEW_RESOLUTION, WIFIDPINAME, handle_cmd},
		{MSG_ID_PREVIEW_QUALITY, WIFIQUALITYNAME, handle_cmd},
		{MSG_ID_PREVIEW_SOUND, NULL, handle_cmd},

		/* Video */
		{MSG_ID_VIDEO_RESOLUTION, DPINAME, handle_cmd},
		{MSG_ID_VIDEO_QUALITY, NULL, handle_cmd},
		{MSG_ID_VIDEO_SOUND, RECSOUNDNAME, handle_cmd},

		/* Photo */
		{MSG_ID_PHOTO_RESOLUTION, NULL, handle_cmd},
		{MSG_ID_PHOTO_QUALITY, QUALITYNAME, handle_cmd},
		{MSG_ID_PHOTO_BURST, PHOTOBURSTNAME, handle_cmd},
		{MSG_ID_PHOTO_TIMELAPSE, PHOTOTIMELAPSENAME, handle_cmd},

		/* Visual Effect */
		{MSG_ID_WHITE_BALANCE, AWBNAME, handle_cmd},
		{MSG_ID_EXPOSURE_COMPENSATION, EVNAME, handle_cmd},
		{MSG_ID_SHARPNESS, NULL, handle_cmd},
		{MSG_ID_ISO, NULL, handle_cmd},
		{MSG_ID_ANTIBANDING, NULL, handle_cmd},
		{MSG_ID_WDR, NULL, handle_cmd},

		/* Control */
		{MSG_ID_RECORD_VIDEO, NULL, handle_cmd},
		{MSG_ID_TAKE_PHOTO, NULL, handle_cmd},

		/* Common */
		{MSG_ID_WIFI_SETTINGS, NULL, handle_cmd},
		{MSG_ID_LANGUAGE, LANGUAGENAME, handle_cmd},
		{MSG_ID_MOTION_DETECTION, MDTNAME, handle_cmd},
		{MSG_ID_ANTI_SHAKE, NULL, handle_cmd},
		{MSG_ID_DATE_STAMP, WATERMARKNAME, handle_cmd},
		{MSG_ID_SCREEN_SAVER, SCREENSAVENAME, handle_cmd},
		{MSG_ID_ROTATION, NULL, handle_cmd},
		{MSG_ID_AUTO_SHUTDOWN, AUTOPOWEROFFNAME, handle_cmd},
		{MSG_ID_BUTTON_SOUND, KEYSOUNDNAME, handle_cmd},
		{MSG_ID_OSD_MODE, NULL, handle_cmd},
		{MSG_ID_CAR_MODE, NULL, handle_cmd},
		{MSG_ID_FORMAT_CARD, NULL, handle_cmd},
		{MSG_ID_FACTORY_RESET, NULL, handle_cmd},
		{MSG_ID_CYCLIC_RECORD, LOOPNAME, handle_cmd},
		{MSG_ID_TIME_CALIBRATION, SETDATENAME, handle_cmd},

		{MSG_ID_GET_SENSOR_RESOLUTION, NULL, handle_cmd},
		{MSG_ID_SET_SENSOR_RESOLUTION, NULL, handle_cmd},

		{0, NULL, NULL},
};

typedef struct
{
	uint8_t netId;
	uint8_t val;
}_netMsgVal;
_netMsgVal netMsgVal[]=
{
	{MSG_ID_PREVIEW_QUALITY,0xff},
	{MSG_ID_PREVIEW_RESOLUTION,0xff},
	{MSG_ID_PHOTO_QUALITY,0xff},
	{MSG_ID_LANGUAGE,0xff},
	{MSG_ID_CYCLIC_RECORD,0xff},
	{MSG_ID_BUTTON_SOUND,0xff},
	{MSG_ID_VIDEO_RESOLUTION,0xff},
	{MSG_ID_VIDEO_SOUND,0xff},
	{MSG_ID_WHITE_BALANCE,0xff},
	{MSG_ID_EXPOSURE_COMPENSATION,0xff},
	{MSG_ID_MOTION_DETECTION,0xff},
	{MSG_ID_PHOTO_BURST,0xff},
	{MSG_ID_PHOTO_TIMELAPSE,0xff},
	{MSG_ID_DATE_STAMP,0xff},
	{MSG_ID_SCREEN_SAVER,0xff},
	{MSG_ID_AUTO_SHUTDOWN,0xff},
};

extern uint32_t menuGetNetMsgVal(uint32_t netItem);
void netMsgValInit(void)
{
	int i;
	for(i=0;i<sizeof(netMsgVal)/sizeof(netMsgVal[0]);i++)
	{
		netMsgVal[i].val=menuGetNetMsgVal(netMsgVal[i].netId);
		printf("netid:0x%x,val:%d\n",netMsgVal[i].netId,netMsgVal[i].val);
	}
}
uint8_t get_APPID(const char *name)
{
	int i;
	for (i = 0; msgMethods[i].handler != NULL; i++)
	{
		if (memcmp(name, msgMethods[i].name, strlen(name)) == 0)
		{
			return msgMethods[i].id;
			break;
		}
	}
	printf("no get_APPID!\r\n");
	return 0;
}

int process_message(message_t *message)
{
	int i;
	int num = sizeof(msgMethods) / sizeof(MessageMethod *);
	for (i = 0; i < num; i++)
	{
		MessageMethod *method = msgMethods + i;
		if (method->id == message->id)
		{
			message->name = method->name;
			return method->handler(message);
		}
	}

	printf("Unknown message ID 0x%x\n", message->id);

	return -1;
}

int send_message(struct tcpControl *control, message_t *message)
{
	uint8_t *sendBuf;
	int fd = control->fd;

	uint8_t msgId = message->id;
	uint8_t sessionId = message->sessionId;
	uint8_t *msgInfo = message->info;
	uint32_t msgLen = message->length;

	// buffer = length(4) + [msgId(1) + sessionId(1) + reserved](4) + message(...)
	uint32_t lenNoHead = msgLen + 4;
	uint32_t bufLen = lenNoHead + 4;

	sendBuf = osal_malloc(bufLen);

	if (sendBuf == NULL)
	{
		printf("send_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return -1;
	}

	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	memcpy(sendBuf + 8, msgInfo, msgLen);

	int ret = osal_socket_send(fd, sendBuf, bufLen, 0);
	printf("send_message:\r\n");
	uartPrintfBuf(sendBuf, bufLen);
	if (ret <= 0)
	{
		printf("tcp_control_send fail, maybe client had closed\r\n");
		osal_free(sendBuf);
		return -2;
	}

	osal_free(sendBuf);

	return 0;
}

// 倒计时
void *take_photo_countdown(uint8_t timer)
{
	uint8_t msgId = MSG_ID_TAKE_PHOTO;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0;		// reserved
	sendBuf[7] = 0;		// reserved
	sendBuf[8] = timer; // reserved
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *get_device_status(uint8_t value)
{
	uint8_t msgId = MSG_ID_DEVICE_STATUS;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0;		// reserved
	sendBuf[7] = 0;		// reserved
	sendBuf[8] = value; // reserved
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

extern u8 getBatLev(void);

void *get_dev_battery_msg(uint8_t type)
{
	uint8_t msgId = type;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	uint8_t battery = getBatLev();
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	if (battery == 0xff)
	{
		sendBuf[8] = battery;
	}
	else
	{
		sendBuf[8] = (battery + 1) * 20;
	}
	//printf("sendBuf:%X\r\n", sendBuf[8]);
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *get_take_photo_msg(uint8_t type)
{
	uint8_t msgId = type;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	sendBuf[8] = 0;
	//printf("sendBuf:%X\r\n", sendBuf[8]);
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *get_rec_msg(uint8_t type)
{
	uint8_t msgId = type;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	if(videoIsRecording())
	{
		sendBuf[8] = RECORD_VIDEO_START;
	}
	else
	{
		sendBuf[8] = RECORD_VIDEO_STOP;
	}
	printf("record msg:%X\r\n", sendBuf[8]);
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *get_uvc_msg(uint8_t type)
{
	uint8_t msgId = type;
	uint8_t sessionId = 0; // 不判断
	uint32_t content_len = 0;
	uint32_t bufLen = 0;
	uint32_t lenNoHead = 0;
	uint8_t *content_locate = NULL;

	content_len = get_uvc_des_frame_num() * 5; // W:2 H:2 Index:1

	bufLen = 4 + 4 + content_len;
	lenNoHead = bufLen - 4;

	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	content_locate = sendBuf + 8;							  // 调到cotent处
	set_uvc_app_msg(content_locate, get_uvc_des_frame_num()); // 填充sendBuf后面内容

	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	//printf("bufLen:%d\r\n", bufLen);
	//uartPrintfBuf(sendBuf, bufLen);
	return tcpMessage;
}

// 查询支持哪种模式(例如支持录卡、不支持录卡)
void *get_support_mode_msg(uint8_t type)
{
	uint8_t msgId = type;
	uint8_t sessionId = 0; // 不判断
	uint32_t content_len = 0;
	uint32_t bufLen = 0;
	uint32_t lenNoHead = 0;
	uint8_t *content_locate = NULL;

	content_len = 1; // 实际内容长度为一个字节

	bufLen = 4 + 4 + content_len;
	lenNoHead = bufLen - 4;

	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	sendBuf[8] = SUPPORT_MODE;
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	//printf("bufLen:%d\r\n", bufLen);
	//uartPrintfBuf(sendBuf, bufLen);
	return tcpMessage;
}

extern void *getMsg_sd_status(uint8_t value);

void *need_send_dev_msg(uint8_t type)
{
	switch (type)
	{
	case MSG_ID_BATTERY:
		return get_dev_battery_msg(type);
		break;
	case MSG_ID_GET_SENSOR_RESOLUTION:
		return get_uvc_msg(type);
		break;
	case MSG_ID_TAKE_PHOTO:
		return get_take_photo_msg(type);
		break;
	case MSG_ID_RECORD_VIDEO:
		return get_rec_msg(type);
		break;
	case MSG_ID_DEVICE_FUNCTION:
		return get_support_mode_msg(type);
		break;
	case MSG_ID_CARD_STATUS:
		return getMsg_sd_status(get_sd_status());
		break;

	default:
		printf("need_send_dev_msg is err:%X", type);
		return NULL;
		break;
	}
}

// 设置需要发送的msg的内容
void *set_msg_to_TCPcontrol(uint8_t msgid, uint8_t value)
{
	uint8_t msgId = msgid;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0;		// reserved
	sendBuf[7] = 0;		// reserved
	sendBuf[8] = value; // reserved
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *getMsg_sd_status(uint8_t value)
{
	uint8_t msgId = MSG_ID_CARD_STATUS;
	uint8_t sessionId = 0; // 不判断
	uint32_t bufLen = 4 + 4 + 1;
	uint32_t lenNoHead = bufLen - 4;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	if (value)
	{
		sendBuf[8] = CARD_STATUS_OK; // reserved
	}
	else
	{
		sendBuf[8] = value;
	}
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

// 固件的所有设置状态
void *all_device_status()
{
	uint8_t msgId = MSG_ID_REPORT;
	uint8_t sessionId = 0; // 不判断
	uint32_t lenNoHead = 4 + (sizeof(netMsgVal)/sizeof(netMsgVal[0]))*2;
	uint32_t bufLen = 4 + lenNoHead;
	uint8_t *sendBuf = osal_malloc(bufLen);
	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}
	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	int i = 0;
	for(i=0;i<sizeof(netMsgVal)/sizeof(netMsgVal[0]);i++)
	{
		sendBuf[2*i+8] = netMsgVal[i].netId; // reserved
		sendBuf[2*i+1+8] = netMsgVal[i].val; // reserved
	}
	struct tcpMessage *tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}

void *copy_message(message_t *message)
{
	uint8_t *sendBuf = NULL;
	struct tcpMessage *tcpMessage = NULL;
	uint8_t msgId = message->id;
	uint8_t sessionId = message->sessionId;
	uint8_t *msgInfo = message->info;
	uint32_t msgLen = message->length;

	uint32_t lenNoHead = msgLen + 4;
	uint32_t bufLen = lenNoHead + 4;
	sendBuf = osal_malloc(bufLen);

	if (sendBuf == NULL)
	{
		printf("copy_message: osal_malloc error\r\n");
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
		return NULL;
	}

	sendBuf[0] = lenNoHead >> 24;
	sendBuf[1] = lenNoHead >> 16;
	sendBuf[2] = lenNoHead >> 8;
	sendBuf[3] = lenNoHead;
	sendBuf[4] = msgId;
	sendBuf[5] = sessionId;
	sendBuf[6] = 0; // reserved
	sendBuf[7] = 0; // reserved
	memcpy(sendBuf + 8, msgInfo, msgLen);
	tcpMessage = osal_malloc(sizeof(struct tcpMessage));
	if(tcpMessage==NULL)
	{
		osal_free(sendBuf);
		return NULL;
	}
	tcpMessage->length = bufLen;
	tcpMessage->sendbuf = sendBuf;
	return tcpMessage;
}
