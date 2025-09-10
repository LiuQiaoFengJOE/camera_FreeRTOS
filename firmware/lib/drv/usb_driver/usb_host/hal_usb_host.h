#ifndef   HAL_USB_HOST_H_
#define   HAL_USB_HOST_H_


#include "typedef.h"
#include "ax329x.h"
#include "AX329x_IRQ.h"
#include "usb/usb11.h"
#include "drv/usb_driver/usb_host/hal_usb_host_api.h"
#include "drv/usb_driver/usb_host/hal_usb_host_enum.h"
#include "usb_dev/usb_dev.h"
extern int usbCameraConn;
struct uvc_src
{
    struct stream_src_self self; // with a chunklist heap
    int framerate;
	struct frame *curFrame;
	u8 sta;
	u32 tgl;
	u32 nextTgl;
	u32 lastTime;
	u32 re_space;
	u32 frame_len;
	u32 data_len;
	u32 head_len;
	u8 *cpbuff;
	struct frame_buf *buff;
};
#define STA_START_RECV   0
#define STA_RECV_DATA    1
#define STA_NEXT_FRAME   2

extern struct uvc_src *puvc_src;

#endif

