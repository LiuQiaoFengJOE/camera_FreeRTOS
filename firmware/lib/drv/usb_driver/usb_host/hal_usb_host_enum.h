/*
 * usb_host_enum.h
 *
 *  Created on: 2013Äê10ÔÂ24ÈÕ
 *      Author: jingcunxu
 */

#ifndef USB_HOST_ENUM_H_
#define USB_HOST_ENUM_H_

#define 	__DEV_ADDR__	4
// USB specific request
typedef struct
{
    u8  mrequest;
    u8  request;
    u16 value;
    u16 index;
    u16 length;
} SETUP_PKG, *P_SETUP_PKG;
typedef struct
{
	u8 addr;
	u8 pidphase;
	u8 t_nkg;
	u8 payload;
}SETUP_TRAN;

typedef struct
{
	u8 addr;
    u8 pid;
    u8 payload;
    u16 len;
    u8 *buffer;	// usb transfer buffer must be
    u8 epbulkin;
    u8 epbulkout;
} XXGPKG, *P_XXGPKG;

typedef struct
{
    u8 max_lun;
    u8 cbw_lun;
//  u8 device_online;
    u8 usb_enum_sta;
//  u8 usb_dev_addr;
//  u8 config_index;
//  u8 usb_connect;
//  u8 pkt_per_sec;		// Ã¿¸öÊµ¼ÊSecotrµÄ Óë512µÄ±¶Êý¹ØÏµ
}USB_VA;

typedef union
{
    SETUP_PKG setup;
    u8 set_pkt[8];
} SETUPPKT;
typedef struct
{
	u8 ctyp;
	u8 cv;
	u8 intfs;
	u8 altset;
	u8 endpts;
	
	u8 epin;
	u8 attrin;
	u16 inpload;
	
	u8 epout;
	u8 attrout;
	u16 outpload;

}MSCDEV;
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
}UVCDEV;

//±ê×¼ÇëÇóÀàÐÍ
#define GET_STATUS      0x00
#define CLEAR_FEATURE   0x01
#define SET_FEATURE     0x03
#define SET_ADDRESS     0x05
#define GET_DESCRIPTOR  0x06
#define SET_DESCRIPTOR  0x07
#define GET_CONFIG      0x08
#define SET_CONFIG      0x09
#define GET_INTERFACE   0x0a
#define SET_INTERFACE   0x0b
#define SYNCH_FRAME     0x0c

//¸ßByte´ú±íÃèÊö·ûÀàÐÍ¼°±àºÅ¡¢µÍByte´ú±íË÷ÒýºÅ
#define DEVICE          0x0100
#define CONFIGURATION   0x0200
#define STRING          0x0300
#define INTERFACE       0x0400
#define ENDPOINT        0x0500


#define PID_SETUP       0xD0
#define PID_IN          0x90
#define PID_OUT         0x10

#define EP0				0
#define EP1				1
#define EP2				2
#define EP3				3


#define MASS_STORAGE_CLASS    0x08
#define UVC_CLASS    0x0e
#define UAC_CLASS    0x01


extern u8 bs_buf[64];

void delay_5ms(u32 n);
u8 enum_usb_dev();


typedef struct {
	u8 w_cmd,r_cmd,id,addr_num,data_num;
	u16 id_reg;//id 寄存器地址
	//u8 const (*init)[];
    u8 const *init;
	u8 mclk;
    u8 hldo_val,lldo_val;
    u8 pclk_filter_en,pclk_filter_sel;
	u8 hsyn_filter_en,hsyn_filter_sel;
    u32 mclk720p;
    u32 mclk480p;
} sensor_cmd_t;
extern const sensor_cmd_t * sensor_cmd_tab[];
typedef struct
{
	u8 *code;
	u32 clen;
	u32 inram;
	
	u32 (*pfun)[2];
	u32 npfun;
	
	u32 iensor;
	sensor_cmd_t const*psensor;
	
}EX_MAND;
extern EX_MAND exmand;
extern EX_MAND uisp;
extern EX_MAND usb2Exmand;
extern EX_MAND usb2Isp;

void usb_stack_init(void);
void husb_kick_ep0isr(void);
void husb_ep0isr(void);
#endif /* USB_HOST_ENUM_H_ */
