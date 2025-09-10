/**@file board_def.h
 * @brief 板級定義文件
 * @details 本文件定義在board_config.h中能夠使用的宏
 */

#ifndef __BOARD_DEF_H__
#define __BOARD_DEF_H__

/* 簡單UART、有DMA的UART */
#define UARTTYPE_SIMPLE 0
#define UARTTYPE_BT 1

/* 簡單UART可以配置到3組IO */
#define UART0_GRP_NONE 0
#define UART0_GRP_1 1		// PA0
#define UART0_GRP_2 2		// PA1
#define UART0_GRP_3 3		// PA4
#define UART0_GRP_4 4		// PA5
#define UART0_GRP_5 5		// PA6
#define UART0_GRP_6 6		// PA7
#define UART0_GRP_7 7		// PA9
#define UART0_GRP_8 8		// PA12
#define UART0_GRP_9 9		// PB0
#define UART0_GRP_10 10	// PB1
#define UART0_GRP_11 11	// PB5
#define UART0_GRP_12 12	// PB6
#define UART0_GRP_13 13	// PD1
#define UART0_GRP_14 14	// PD2
#define UART0_GRP_15 15	// PD3
#define UART0_GRP_16 16	// PD7
#define UART0_GRP_17 17	// PD10
#define UART0_GRP_18 18	// PF0
#define UART0_GRP_19 19	// PF2
#define UART0_GRP_20 20	// PF8
#define UART0_GRP_21 21	// PF10
#define UART0_GRP_22 22	// PF11
#define UART0_GRP_23 23	// PE0
#define UART0_GRP_24 24	// PE1
#define UART0_GRP_25 25	// PE2
#define UART0_GRP_26 26	// PE3



/* 有DMA的UART可以配置到3組IO */
#define BTUART_GRP_NONE 0
#define BTUART_GRP_0 1 	// PA4
#define BTUART_GRP_1 2		// PA5
#define BTUART_GRP_2 3		// PA6
#define BTUART_GRP_3 4		// PA7
#define BTUART_GRP_4 5		// PA9
#define BTUART_GRP_5 6		// PA10
#define BTUART_GRP_6 7		// PA12
#define BTUART_GRP_7 8		// PB0
#define BTUART_GRP_8 9		// PB4
#define BTUART_GRP_9 10	// PB5
#define BTUART_GRP_10 11	// PB6
#define BTUART_GRP_11 12	// PD0
#define BTUART_GRP_12 13	// PD8
#define BTUART_GRP_13 14	// PD9
#define BTUART_GRP_14 15	// PD10
#define BTUART_GRP_15 16	// PF0
#define BTUART_GRP_16 17	// PF1
#define BTUART_GRP_17 18	// PF8
#define BTUART_GRP_18 19	// PF10
#define BTUART_GRP_19 20	// PF11
#define BTUART_GRP_20 21	// PE0
#define BTUART_GRP_21 22	// PE1
#define BTUART_GRP_22 23	// PE2
#define BTUART_GRP_23 24	// PE3
#define BTUART_GRP_24 25	// PE15




////////////////////////////////////////////////////////////////////////////////
// TIMER
/*  */
#define TIMERTYPE_16BIT 0

/* 16-bit Timer可以有4個選擇 */
#define TIMER16BIT_0 0
#define TIMER16BIT_1 1
#define TIMER16BIT_2 2
#define TIMER16BIT_3 3

#endif /* __BOARD_DEF_H__ */
