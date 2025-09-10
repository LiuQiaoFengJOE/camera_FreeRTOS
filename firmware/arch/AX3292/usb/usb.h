#ifndef _USB_H_
#define _USB_H_

#include "typedef.h"
#include "spr_defs.h"
#include "AX329x.h"
#include "usb_driver/usb_manage.h"

typedef struct
{
	u8 addr;
	u8 episoin;
} INTSTACK;
typedef enum
{
	USB_IRQ_RESET=0,
	USB_IRQ_EP0,

	USB_IRQ_MAX
}USB_IRQ_E;

#define USB_SUSPEND (1 << 0)
#define USB_RESUME (1 << 1)
#define USB_RESET (1 << 2)
#define USB_BABBLE (1 << 2)
#define USB_SOF (1 << 3)
#define USB_CONN (1 << 4) // device connection is detected,only valid in host mode
#define USB_DISCON (1 << 5)
#define USB_SESS_REQ (1 << 6)
#define USB_VBUS_ERR (1 << 7)

#define USB_INEP_INT(n) (1 << n)
#define USB_OUTEP_INT(n) (1 << n) // 0 not include
#define combo_delay(t)          \
	do                          \
	{                           \
		volatile int i = (t)*2; \
		while (i--)             \
			;                   \
	} while (0)

#endif /*_USB_H_*/