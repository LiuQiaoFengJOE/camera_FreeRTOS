#ifndef APP_CFG_MODULE_PRESENT
#define APP_CFG_MODULE_PRESENT

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "typedef.h"
// #include "sys_config.h"

// STM32F103不支持音频
#define SUPPORT_AUDIO 0
#define ENABLE_MP3 0		// 需消耗63k flash
#define SUPPORT_WEBSERVER 0 // 需消耗37k flash
#define MACHTALK_ENABLE 0	// MACHTALK平台远程控制

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
* UCOS每个线程优先级不能相同，放在这里统一管理
*********************************************************************************************************
*/
#define WORK_QUEUE_MAX_SIZE 4

#ifdef _UCOSII

enum TASK_PRIO
{
	TASK_UNUSED_PRIO = 10,
	TASK_MOAL_WROK_PRIO,
	TASK_SDIO_THREAD_PRIO,
	TASK_TCPIP_THREAD_PRIO,
	TASK_TIMER_TASKLET_PRIO,
	TASK_MONITOR_PRIO,
	REASSOCIATION_THREAD_PRIO,

	// 以上顺序勿随意改动

	// user task
	TASK_MAIN_PRIO,
	TASK_ADC_RECV_PKG_PRIO,
	TASK_TCP_RECV_PRIO,
	TASK_TCP_ACCEPT_PRIO,
	TASK_WEB_PRIO,
	TASK_TCP_SEND_PRIO,

	OS_TASK_TMR_PRIO
};

#else

enum TASK_PRIO
{

	OS_TASK_TMR_PRIO = 2,

	TASK_TCP_SEND_PRIO,
	TASK_WEB_PRIO,

	TASK_SPOOK_SACN_PRIO,
	TASK_SPOOK_SEND_PRIO,
#ifdef WIFI_MVL8801_SUPPORT
	TASK_SPOOK_PRIO,
#endif

#ifdef WIFI_RTL8189_SUPPORT
	TASK_SPOOK_PRIO,
#endif
#ifdef WIFI_SV6030P_SUPPORT
	TASK_SPOOK_PRIO = 6,
#endif
#ifdef WIFI_RTL8189FTV_SUPPORT
	TASK_SPOOK_PRIO,
#endif

#ifdef WIFI_XR819_SUPPORT
	TASK_SPOOK_PRIO,
#endif

#ifdef WIFI_ATBM_SUPPORT
	TASK_SPOOK_PRIO,
#endif

#if (SYS_SENSOR_TYPE == U_SENSOR)
	TASK_UVC_PRIO,
#endif

	TASK_WIFI_COMMAND,

	TASK_FLY_COMMAND,

	TASK_HTTP_SERVER_PRIO,
	TASK_HTTP_CLIENT_PRIO,

	TASK_TCP_ACCEPT_PRIO,
	TASK_TCP_RECV_PRIO,
	TASK_ADC_RECV_PKG_PRIO,
	TASK_MAIN_PRIO,

	REASSOCIATION_THREAD_PRIO,
	TASK_MONITOR_PRIO,
	TASK_TIMER_TASKLET_PRIO,
#ifdef WIFI_MTK7601_SUPPORT
	TASK_SPOOK_PRIO,
#endif
	TASK_TCPIP_THREAD_PRIO,

	// TASK_SPOOK_PRIO,

	TASK_SDIO_THREAD_PRIO,
	TASK_TIMER_SYS,
	TASK_MOAL_WROK_PRIO,

#ifdef WIFI_RTL8189_SUPPORT
	TASK_RTW_CMD_PRIO,

	// TASK_RTW_MP_XMIT_PRIO,

	// TASK_TIMER_TASKLET_PRIO,
	TASK_RTW_SDIO_INTERRUPT,
	// TASK_TIMER_TASKLET_PRIO,

	TASK_RTW_SDIO_XMIT_PRIO = 22,
	TASK_RTW_XMIT_PRIO,
	TASK_HIGH_PRI_CTL_PRIO,
#endif

	TASK_UNUSED_PRIO,

};

#ifdef WIFI_RTL8189_SUPPORT
#define TASK_RTW_XMIT_SIZE 512 * 8
#define TASK_RTW_SDIO_XMIT_SIZE 512 * 8
#define TASK_RTW_CMD_SIZE 512 * 4
#define TASK_RTW_MP_XMIT_SIZE 512 * 8
#endif

#endif

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*单位WORD
*********************************************************************************************************
*/
// #define TASK_TCPIP_THREAD_STACK_SIZE		512
#if (SYS_CHIP_MODULE == APPO_TIGA)
#define TASK_TCPIP_THREAD_STACK_SIZE 512 * 2
#define TIMER_TASKLET_STACK_SIZE 1024
#define MOAL_WROK_STACK_SIZE 1024
#define TASK_MONITOR_STACK_SIZE 128
#define REASSOCIATION_THREAD_STACK_SIZE 1024
#elif (SYS_CHIP_MODULE == APPO_VISION)
#define TASK_TCPIP_THREAD_STACK_SIZE 1024
#define TIMER_TASKLET_STACK_SIZE 1024
#define MOAL_WROK_STACK_SIZE 512 * 3
#define TASK_MONITOR_STACK_SIZE 512
#define REASSOCIATION_THREAD_STACK_SIZE 2048
#endif

// 以上数值勿随意改动
// yuguang debg
// #define TASK_MAIN_STACK_SIZE				4096
#define TASK_MAIN_STACK_SIZE 2048

#define TASK_ADC_RECV_PKG_STACK_SIZE 256
#if ENABLE_MP3
#define TASK_TCP_RECV_STACK_SIZE (1024 + 512)
#else
#define TASK_TCP_RECV_STACK_SIZE 512
#endif
#define TASK_TCP_SEND_STACK_SIZE 256
#define TASK_ACCEPT_STACK_SIZE 256
#define TASK_WEB_STK_SIZE 256
#define TASK_HTTP_SERVER_STK_SIZE 256
#define TASK_HTTP_CLIENT_STK_SIZE 2048

/*
*********************************************************************************************************
*                               		IRQ_PRIORITY
*如果中断函数里需要打印信息，则此中断优先级要低于uart中断
*********************************************************************************************************
*/

enum IRQ_PRIORITY
{

	USART1_IRQn_Priority = 1,
	SDIO_IRQn_Priority,
	TIM2_IRQn_Priority,
	USART3_IRQn_Priority,
	DMA2_Stream5_IRQn_Priority,
	DMA2_Stream7_IRQn_Priority,
	ADC_IRQn_Priority,
	DMA1_Stream5_IRQn_Priority,
	SPI3_IRQn_Priority,
	DMA2_Stream0_IRQn_Priority,
	TIM3_IRQn_Priority,
	TIM4_IRQn_Priority,
	DMA2_Stream1_IRQn_Priority,
	DCMI_IRQn_Priority,
	SysTick_IRQn_Priority
};

/*
*********************************************************************************************************
*                                      kernel 里面常用资源值定义
*建议配合monitor软件确定合适的数值
*********************************************************************************************************
*/
#define EVENTS_MAX 48	 // EVENTS_MAX是所有事件的数量之和，包括邮箱、消息队列、信号量、互斥量等
#define TIMER_MAX 20	 // 定时器
#define MSG_QUEUE_MAX 16 // 消息队列
#define TASKS_MAX 14	 // 任务

/*
*********************************************************************************************************
*                                      定义中断向量表地址
* 因低16k地址用于bootloader，我们的代码需要从16k开始，所以需要在启动时将中断向量重新映射到16k地址
* 目前只对F205有效，F103版本不使用bootloader
*********************************************************************************************************
*/
#define IVECTOR_ADDR (16 * 1024)
// #define IVECTOR_ADDR 					(0)		//如果不使用bootloaer（同时将IROM1的start改为0x8000000）

#endif
