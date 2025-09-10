#ifndef _USB_HOST_H_
#define _USB_HOST_H_
#include "typedef.h"
#include "../usb_manage.h"
/*usb request_type*/
#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03
#define SET_ADDRESS 0x05
#define GET_DESCRIPTOR 0x06
#define SET_DESCRIPTOR 0x07
#define GET_CONFIG 0x08
#define SET_CONFIG 0x09
#define GET_INTERFACE 0x0a
#define SET_INTERFACE 0x0b
#define SYNCH_FRAME 0x0c

/*usb request*/
#define DEVICE 0x0100
#define CONFIGURATION 0x0200
#define STRING 0x0300
#define INTERFACE 0x0400
#define ENDPOINT 0x0500

typedef struct
{
    u8 length;
    u8 descriptor_type;
    u16 bcd_usb;
    u8 device_class;
    u8 device_subclass;
    u8 device_protocol;
    u8 maxpacket_size0;
    u16 id_vendor;
    u16 id_product;
    u16 bcd_device;
    u8 manufacturer;
    u8 product;
    u8 serial_number;
    u8 num_configurations;
} SDEV_DESC, *P_DEV_DESC;
u32 usb_data_len(dev_mesg *usb_dev);
u32 usb_IntrUSB();
u32 usb_IntrOutl();
u32 usb_IntrInl();
u32 usb_IntrInh();
u32 is_usb_data_error(dev_mesg *usb_dev);
void register_usb_host(usb_host *host);
void usb_ep_init(dev_mesg *usb_dev, u8 in_ep);
void kick_usb(dev_mesg *usb_dev);
void usb_host_speed(u8 speed);
void usb_init(void);
TSTA usb_setup_request(dev_mesg *stack, u8 *ppkg);
TSTA usb_txrx_pkg(u8 epx, dev_mesg *ustack);
void enable_ep_irq(dev_mesg *usb_dev);
#endif /*_USB_HOST_H_*/