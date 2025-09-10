#ifndef _UVC_H_
#define _UVC_H_
#include "../../usb_manage.h"
#include "uvc_fun.h"
#include "typedef.h"
#include "debug.h"

typedef struct
{
	u8 *cpbuff; // 当前写入数据的地址
	//	u8 *frame;      //
	u8 uvc_head[16]; // one pack head data
	u32 tgl;
	u32 sta;
	u32 frame_len; // 帧长度
	u32 lastTime;
	//	u32 is_buys;
	//	u32 fifo_indx;
	u32 data_len; // one packe data len
	u32 head_len; // one packe head len
	u32 re_space; // 当前所写的buff剩余空间
	struct frame_buf *buff;
	struct jpg_src *src;
} UVC_MANAGE;

#endif /*_UVC_H_*/
