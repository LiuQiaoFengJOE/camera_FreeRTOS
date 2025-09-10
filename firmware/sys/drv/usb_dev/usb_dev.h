#ifndef _USB_DEV_H_
#define _USB_DEV_H_

#include "typedef.h"
#include "task/app.h"
#include "usb/usb.h"
#include "hal_usb_dev_enum.h"
#include "hal_usb_msc.h"
#include "hal_usb_uac.h"
#include "hal_usb_uvc.h"
#include "hal/hal_mmc.h"
#include "hal_usb_api.h"
#include "hal_watermark/hal_watermark.h"
#include "osal/osal_common.h"
#define 	__MSCTADR__		USB20E1TADR
#define 	__MSCRADR__		USB20E1RADR

extern u8 hub_fifo[16];
extern u8 uvc_fifo[1024 * 3];
extern u8 ep0_fifo[64];
extern u8 ep1_fifo[64];


#define _ep0_fifo_ ep0_fifo
#define _msc_txfifo_ uvc_fifo
#define _msc_rxfifo_ ((u32)uvc_fifo+512+16)
#define _uvc_txfifo_ ((u32)uvc_fifo+(512+16)*2)
#define _uac_txfifo_ ((u32)uvc_fifo+(512+16)*2)


#endif
