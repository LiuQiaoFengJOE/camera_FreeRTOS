#ifndef _FLY_H_
#define _FLY_H_
#include "type.h"
/*support fly control command 1:use 0:no use*/
#define FLY_CONTROL 1
#define FLY_FIRMWARE_MODIFY 1
/*fly control protocol*/
#define FLY_CONTROL_8_BYTE
// #define FLY_CONTROL_13_BYTE
// #define FLY_CONTROL_2017UFO
// #define FLY_CONTROL_JJRC

/*fly control head and tail*/
#define FLY_CONTROL_H66_T99
// #define FLY_CONTROL_HCC_T33

#if defined(FLY_CONTROL_8_BYTE) || defined(FLY_CONTROL_2017UFO) || defined(FLY_CONTROL_JJRC)
#define FLY_COMMAND_LEN 11
#define FLY_COMMAND_BAUD 19200UL
#elif defined(FLY_CONTROL_13_BYTE)
#define FLY_COMMAND_LEN 13
#define FLY_COMMAND_BAUD 115200UL
#else
#endif

#define TASK_FLY_COMMAND 15

void uart_fly_command_putc(u8 ch);
u8 fly_command_check(u8 *buf, int len);
void fly_command_transform(int i, u8 *buf);
void fly_command_receive(u8 *buf, int len);
void fly_command_thread(void *parameter);
void uart0_isr(void *param);
void uart_fly_command_init(void);
extern void uart0_init(u32 fly_baud);
extern void clear_pending();
extern u8 uart0_putc(u8 ch);

#endif
