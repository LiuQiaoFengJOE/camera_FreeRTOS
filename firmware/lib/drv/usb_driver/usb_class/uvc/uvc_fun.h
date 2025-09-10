#ifndef _UVC_FUBC_H_
#define _UVC_FUBC_H_
#include "typedef.h"
#include "../../usb_manage.h"
#include "uvc.h"
#include "../../usb_host/usb_host.h"
/**************************************************/
#define INTERFACE_Type 0x24
#define STRAME_O_CONTORL 0x04
#define ENDPOINT_DESC 0x05

#define VS_FAME_TYPE_MAXNUM 20
#define VS_FORMAT_TYPE_MAXNUM 2

#define IS_UVC_JPEG 1
#define UVC_DEBG 0
/************************************************/

#define UVC_SET_REQ 0x21
#define UVC_GET_REQ 0xA1
#define UVC_SET_CUR 0x01
#define UVC_GET_CUR 0x81
#define UVC_GET_MIN 0x82
#define UVC_GET_MAX 0x83
#define UVC_GET_RES 0x84
#define UVC_GET_LEN 0x85
#define UVC_GET_INFO 0x86
#define UVC_GET_DEF 0x87

typedef struct
{
	u8 ctyp;

	u8 cv;
	u8 epctl;
	u8 ctl_intfs;
	u8 ctl_altset;
	u8 ctl_pload;
	u8 ctl_ttyp;
	u8 ctl_interval;

	u8 epstrm;
	u8 strm_intfs;
	u8 strm_altset;
	u16 strm_pload;
	u8 strm_ttyp;
	u8 strm_interval;
} UVCDEV;

typedef struct
{
	u16 bmhint;
	u8 bFormatIndex;
	u8 bFrameIndex;
	u32 dwFrameInterval;
	u16 wKeyFrameRate;
	u16 wPFrameRate;
	u16 wCompQuality;
	u16 wCompWindowSize;
	u16 wDelay;
	u32 dwMaxVideoFrameSize;
	u32 dwMaxPayloadTSize;
} VIDEO_COMMIT;
typedef enum
{
	VC_DESCRIPTOR_UNDEFINED = 0,
	VC_HEADER,
	VC_INPUT_TERMINAL,
	VC_OUTPUT_TERMINAL,
	VC_SELECTOR_UNIT,
	VC_PROCESSING_UNIT,
	VC_EXTENSION_UNIT,
	VC_ENCODING_UNIT

} VC_Subtype;

typedef enum
{
	VS_UNDEFINED = 0,
	VS_INPUT_HEADER,
	VS_OUTPUT_HEADER,
	VS_STILL_IMAGE_FRAME,
	VS_FORMAT_UNCOMPRESSED,
	VS_FRAME_UNCOMPRESSED,
	VS_FORMAT_MJPEG,
	VS_FRAME_MJPEG,
	Reserved,
	Reserved1,
	VS_FORMAT_MPEG2TS,
	Reserved2,
	VS_FORMAT_DV,
	VS_COLORFORMAT,
	Reserved3,
	Reserved4,
	VS_FORMAT_FRAME_BASED,
	VS_FRAME_FRAME_BASED,
	VS_FORMAT_STREAM_BASED,
	VS_FORMAT_H264,
	VS_FRAME_H264,
	VS_FORMAT_H264_SIMULCAST
} VS_Subtype;

typedef struct
{
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bFrameIndex;
	u8 bmCapabilities;
	u16 wWidth;
	u16 wHeight;

} VS_FRAME;
typedef struct
{
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bFrameIndex;
	u8 bNumFrameDescriptors;
} VS_FORMAT;
typedef struct
{
	u8 bLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bNumFormats;
} VS_HEAD;

typedef struct
{
	u8 length;
	u8 type;
	u8 num;
	u8 alt_tring;
	u8 end_points;
	u8 iclass;
	u8 sub;
	u8 proto;
	u8 index;
} SINTF_DESC, *P_INTF_DESC;

typedef struct
{
	VS_FRAME vs_frame;
	VS_FORMAT vs_format;
} UVC_SLEST;

// Standard EndPoint Descriptor
typedef struct
{
	u8 length;
	u8 type;
	u8 ep_addr;
	u8 attr;
	u16 pay_load; // low-speed this must be 0x08
	u8 interval;
} SEP_DESC, *P_EP_DESC;
#define __INTFS_STACK__ 16

typedef struct
{
	u8 DesLen;
	u8 DescriptorType;
	u8 Subtype;
	u8 control_endpoint;
	VS_HEAD vs_head;
	VS_FRAME vs_frame[VS_FAME_TYPE_MAXNUM];
	VS_FORMAT vs_format[VS_FORMAT_TYPE_MAXNUM];
	u8 vs_frame_num;
	u8 vs_format_num;
	u32 intfs_count;
	u32 edpt_count;
	SINTF_DESC intfs[__INTFS_STACK__];
	SEP_DESC edpt[__INTFS_STACK__];

} UVC_DES;

typedef struct
{
	u32 hight;
	u32 width;
	u32 index;
} UVC_DAT_APP;

extern UVC_DES uvc_des;
extern UVCDEV uvc;
TSTA enum_set_resolution2(struct usb_dev *udev, u8 resolution);
void printf_uvc2_vsdesc(UVC_DES *ud);
void select_uvc2_intfs(UVC_DES *ud, UVCDEV *uvc);
u8 select_uvc_format(UVC_DES *ud, UVC_SLEST *us, u8 index);
void analysis_uvc2_desc(UVC_DES *uvc_des, u8 *desc, u32 desclen);
void uvc_memcpy(void *dst, void *src, int cnt);
void uvc_fifo2buff(u8 *des, u8 *sour, u32 len);

#endif