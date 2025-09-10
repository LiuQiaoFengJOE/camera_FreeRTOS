/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: free_rtos.c
*
* Description:
* Implement the initialization, configuration , etc.  for freeRTOS
*
* Author: CHEN<chen@buildwin.com.cn>
*
* Create date: June. 15, 2016
*
*********************************************************************************
*
* Revision history:
---------------------------------------------------------------------------------
Date                    Modification                 Revision            Name
---------------------------------------------------------------------------------
June. 15, 2016      Created                       V0.1                 CHEN

********************************************************************************/
/**
 * @File free_rtos.c
 * @brief Implement the initialization, configuration , etc.  for freeRTOS
 */

#include "api.h"
#include "app_cfg.h"
// #include <sys_config.h>
// #include <sys_define.h>
// #include <interrupt.h>
// #include "hld/led/led.h"
#include <osal/osal.h>

#if (SYS_CHIP_MODULE == APPO_VISION)
#include "jpeg_v1.1/lld/mjp_dc.h"
#include "csi_v1.1/csi_dc.h"
#endif

UINT32 heap_start = 0;

void *heap_ptr = NULL;

void *RAMSIZE = NULL;

extern int _heap_start;
extern int _heap_end;

extern int rompri[40];
extern int freepri[40];
extern char namepri[40][200];
extern int addsize[40];
extern int name_num;
__ATTRIBUTE_REUSE_ void main_task(DWORD dwpara1, DWORD dwpara2)
{
	int i = 0;
	_ENTER();

	timer_Init();

	OSStatInit();

	app_init();
	p_dbg("FW version:%s", VERSION);
	p_dbg("boot time: %d.%d S", os_time_get() / 1000, os_time_get() % 1000);
	// #if (SYS_CHIP_MODULE == APPO_VISION)
	while (1)
	{
		sleep(3000);
		// printf("--");
		// task_debg();
		// printf("3s:%d  %d\n",xPortGetMinimumEverFreeHeapSize()/1024,xPortGetFreeHeapSize()/1024);
		/*
		for(i = 0;i < 40;i++)
		{
			printf("task_%d====>use:%d  free:%d\n",i,rompri[i]/1024,freepri[i]/1024);
		}
		printf("*******************************************\n");
		for(i = 0;i < name_num;i++)
		{
			printf("%s  use:%d\n",namepri[i],addsize[i]/1024);
		}
		printf("*******************************************\n");
		printf("total_free:%d\n",mem_get_free()/1024);
		printf("*******************************************\n");
		*/
#if 0		
        if(is_wifi_connected()){ 
            led_flash();
          }
        else{
            led_set(1,1);
        }
#endif
	}
	// #endif
	thread_exit(thread_myself());
	return;
}

void timer0_isr(void *param);
void timer1_isr(void);
void timer2_isr(void);
void timer3_isr(void *param);
void Port_Isr(void *param);
void csi_Isr(void *param);
void jpeg_enc_Isr(void *param);
void dma_memcpy_first(void *dst, void *src, int cnt);
void usb_host_isr(void);
void uart0_isr(void *param);
void sd0_isr();
__ATTRIBUTE_REUSE_ void _init(void)
{
	TASK_HDL main_task_hdl;
	OSAL_TASK main_task_ctrl;

	UINT32 size = 0;

	int i = 0;
	u8 a[20], b[20];
	a[0] = 1;
	b[0] = 2;
	dma_memcpy_first(a, b, 20);

	/* add by jornny */
	heap_start = (UINT32)&_heap_start;

	heap_ptr = (void *)&heap_ptr;

	RAMSIZE = (void *)&_heap_end;

	size = RAMSIZE - heap_start;

	sys_Init();

	debg(">>_init\n");

	OSClearBss();
	OSInit();

	OSInitHook();

#if 0
	void timer0_isr(void);
	void timer1_isr(void);
	void timer2_isr(void);
	void Port_Isr(void);
	void csi_Isr(void *param);
	void jpeg_enc_Isr(void);
#endif

#if (SYS_CHIP_MODULE == APPO_TIGA)
	__gie_en(); // Interrupt Exception Enable
#elif (SYS_CHIP_MODULE == APPO_VISION)
	ClrIntSrc(USB_MC_INT);

	isr_pr_init();
	__LGIE_EN__();
	__HGIE_EN__();
#endif
	BSP_IntVecReg(TIMER0_INT, 0, timer0_isr, NULL);
#if (SYS_CHIP_MODULE == APPO_TIGA)

#if (SYS_SENSOR_TYPE == SENSOR)
	BSP_IntVecReg(JPEG_INT, 0, jpeg_enc_Isr, NULL);
	BSP_IntVecReg(CSI_INT, 0, NULL, NULL); // Set CSI Int Mask Bit to 1
#endif

#elif (SYS_CHIP_MODULE == APPO_VISION)

#if (SYS_SENSOR_TYPE == SENSOR)
	BSP_IntVecReg(TIMER3_INT, 0, timer3_isr, NULL);
	BSP_IntVecReg(JPEG_INT, BSP_INT_PRIO_THIRD, mjpec_isr, NULL);
	BSP_IntVecReg(CSI_INT, BSP_INT_PRIO_THIRD, csi_mjp_isr_hal, NULL);
#endif
#endif

	BSP_IntVecReg(UART0_INT, 0, uart0_isr, NULL);

	// BSP_IntVecReg(TIMER1_INT, 0, timer1_isr);
	// BSP_IntVecReg(TIMER2_INT, 0, timer2_isr);
#if (SYS_SENSOR_TYPE == U_SENSOR)
	BSP_IntVecReg(USB_MC_INT, 0, usb_host_isr, NULL); // Set USB Mask Bit to 1
#else
	BSP_IntVecReg(USB_MC_INT, 0, NULL, NULL); // Set USB Mask Bit to 1
#endif
	BSP_IntVecReg(IO_INT, 0, Port_Isr, NULL);

	BSP_IntVecReg(32, 0, vTaskSwitchContext, NULL);
#if (SYS_CHIP_MODULE == APPO_VISION)
//    BSP_IntVecReg(SDC0_INT, 0, sd0_isr, NULL);//fatfs
#endif

	// vPortEnableInterrupts();
	// VGA_init();while(1);

	// thread_create(main_thread, 0, TASK_MAIN_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");

	MEMSET(&main_task_ctrl, 0, sizeof(OSAL_TASK));

	main_task_ctrl.stk_size = TASK_MAIN_STACK_SIZE * 3;
	// hub_task_ctrl.tsk_prio = OSAL_PRI_NORMAL;
	main_task_ctrl.tsk_prio = 6; // TASK_TIMER_TASKLET_PRIO;
	main_task_ctrl.param = 0;
	main_task_ctrl.name[0] = 'M';
	main_task_ctrl.name[1] = 'A';
	main_task_ctrl.name[2] = 'I';
	main_task_ctrl.name[3] = 'N';
	main_task_ctrl.tsk_entry_func = (OSAL_TSK_PROC)main_task;

	main_task_hdl = osal_task_create(&main_task_ctrl);
	if (OSAL_INVALID_ID == main_task_hdl)
	{
		return;
	}

	// thread_create(main_thread, 0, TASK_TIMER_TASKLET_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");

	// thread_create(usb_test_task, 0, TASK_TIMER_TASKLET_PRIO, 0, TASK_MAIN_STACK_SIZE, "usb_test_task");

	// thread_create(usb_test_task1, 0, TASK_TCPIP_THREAD_PRIO, 0, TASK_MAIN_STACK_SIZE, "usb_test_task1");
#ifndef WIFI_RTL8189FTV_SUPPORT
#ifndef WIFI_SV6030P_SUPPORT
#if (BOARD_TYPE != AX3268B)
	(void)osal_setup_msgq_task();
#endif
#endif
#endif
	debg(">>OSStart\n");

	// xTaskStartScheduler();
	OSStart();

	return;
}
