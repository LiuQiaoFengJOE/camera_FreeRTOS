#ifndef _HUB_FUN_H_
#define _HUB_FUN_H_

#include "hub_fun.h"
#include "hub.h"
#include "../../usb_manage.h"

/**hub clear feature **/
#define PORT_CONNECT 0x10
#define PORT_ENABLE 0X01
#define CLEAR_PORT_REST 0x14
/**hub set feature **/
#define PORT_POWER 0X08
#define PORT_REST 0x04

extern void printf_desc(u8 *buff, u32 desclen);
extern u8 get_hub_port_num(u8 *desc);
extern u8 get_int_enpoint(u8 *desc);

#endif /* _HUB_FUN_H_ */
