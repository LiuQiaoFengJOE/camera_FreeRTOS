/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
 * File Name          : uart.c
 * Author             : pengtao_zhang
 * Version            :
 * Date               : 07/19/2013
 * Description        : This file provides all the uart firmware functions.
 ***************************************************************************/

// #include "api.h"
// #include "sys_define.h"#include "AX329x.h"
#include "basic_defs.h"
#include "spr_defs.h"
#include "typedef.h"
#include "ax329x_irq.h"
#include "sys/sys.h"
#include "osal/osal_sema.h"
#include "fly.h"
#include "arch/sys_arch.h"

sys_sem_t fly_command_event;

__attribute__((section(".sram_comm"))) u8 fly_dma_buf[20];

void clear_pending() // tx
{
	// BUSTA &= ~BIT(2); // clear tx sta
}

u8 fly_command[2][FLY_COMMAND_LEN];
volatile unsigned int fly_buf_changer;
// wait_event_t uart0_event;
// wait_event_t fly_command_event;

void uart_fly_command_putc(u8 ch)
{
#if (FLY_CONTROL)
	btuart_putchar(ch);
	// BUCON |= BIT(3);   //中断tx
#endif
}

u8 fly_command_check(u8 *buf, int len)
{

	int i = 0;
	u8 buf_chksum = 0;
	for (i = 1; i < len - 2; i++)
	{
		buf_chksum ^= buf[i];
	}

	if (buf[len - 2] == (buf_chksum & 0xff))
	{
		return true;
	}

	return false;
}

#if defined(FLY_CONTROL_8_BYTE)
void fly_command_transform(int i, u8 *buf)
{
	u8 left_ritht, forward_back, direction;

	left_ritht = buf[1] + buf[9] * 62 / 255 - 31;
	if (left_ritht > 255)
		left_ritht = 255;
	if (left_ritht < 0)
		left_ritht = 0;

	forward_back = buf[2] + buf[8] * 62 / 255 - 31;
	if (forward_back > 255)
		forward_back = 255;
	if (forward_back < 0)
		forward_back = 0;

	/* valve 0x80/8 or 0x80/16 */
	if ((buf[4] >= (0x80 - 0x80 / 8)) && (buf[4] <= (0x80 + 0x80 / 8)))
	{
		direction = 0x80;
	}
	else
	{
		direction = buf[4]; // direction
	}
	direction = direction + buf[10] * 62 / 255 - 31;
	if (direction > 255)
		direction = 255;
	if (direction < 0)
		direction = 0;

	fly_command[i][0] = 0x66;																										// head
	fly_command[i][1] = left_ritht;																									// left and right
	fly_command[i][2] = forward_back;																								// forward and back
	fly_command[i][3] = buf[3];																										// throttle
	fly_command[i][4] = direction;																									// direction
	fly_command[i][5] = buf[11];																									// mark
	fly_command[i][6] = (fly_command[i][1] ^ fly_command[i][2] ^ fly_command[i][3] ^ fly_command[i][4] ^ fly_command[i][5]) & 0xff; // check
	fly_command[i][7] = 0x99;																										// trail
}

#elif defined(FLY_CONTROL_13_BYTE)
void fly_command_transform(int i, u8 *buf)
{
	fly_command[i][0] = 0xFF; // head
	fly_command[i][1] = 0x00;
	fly_command[i][2] = 0x08;

	fly_command[i][3] = buf[3]; // throttle
	fly_command[i][4] = buf[2]; // forward and back
	fly_command[i][5] = buf[1]; // left and right
	fly_command[i][6] = buf[4]; // direction

	fly_command[i][7] = buf[5]; // fine forward and back
	fly_command[i][8] = buf[6]; // fine left and right
	fly_command[i][9] = buf[7]; // fine direction

	fly_command[i][10] = 0x00; // mark

	fly_command[i][11] = fly_command[i][1] + fly_command[i][2] + fly_command[i][3] + fly_command[i][4] + fly_command[i][5] + fly_command[i][6] + fly_command[i][7] + fly_command[i][8] + fly_command[i][9] + fly_command[i][10]; // check

	fly_command[i][12] = 0xFE; // trail
}

#elif defined(FLY_CONTROL_2017UFO) || defined(FLY_CONTROL_JJRC)

void fly_command_transform(int i, u8 *buf)
{
	s16 left_ritht, forward_back, direction;
	u8 mark, bit2_val, bit3_val, bit5_val, bit6_val, bit7_val;

	/* valve 0x80/8 or 0x80/16 */
	if ((buf[1] >= (0x80 - 0x80 / 16)) && (buf[1] <= (0x80 + 0x80 / 16)))
	{
		left_ritht = 0x80;
	}
	else
	{
		left_ritht = buf[1];
	}
	left_ritht = left_ritht + buf[9] * 62 / 255 - 31;
	if (left_ritht > 255)
		left_ritht = 255;
	if (left_ritht < 0)
		left_ritht = 0;

	/* valve 0x80/8 or 0x80/16 */
	if ((buf[2] >= (0x80 - 0x80 / 16)) && (buf[2] <= (0x80 + 0x80 / 16)))
	{
		forward_back = 0x80;
	}
	else
	{
		forward_back = buf[2];
	}
	forward_back = forward_back + buf[8] * 62 / 255 - 31;
	if (forward_back > 255)
		forward_back = 255;
	if (forward_back < 0)
		forward_back = 0;

	/* valve 0x80/8 or 0x80/16 */
	if ((buf[4] >= (0x80 - 0x80 / 8)) && (buf[4] <= (0x80 + 0x80 / 8)))
	{
		direction = 0x80;
	}
	else
	{
		direction = buf[4]; // direction
	}
	direction = direction + buf[10] * 62 / 255 - 31;
	if (direction > 255)
		direction = 255;
	if (direction < 0)
		direction = 0;

#if defined(FLY_CONTROL_2017UFO)

	mark = buf[11] & (~BIT(6)) & (~BIT(7));
	bit6_val = (buf[5] & BIT(0)) << 6;
	bit7_val = (buf[11] & BIT(6)) << 1;
	mark = mark | bit6_val | bit7_val;

#elif defined(FLY_CONTROL_JJRC)

	mark = buf[11] & (~BIT(2)) & (~BIT(3)) & (~BIT(5)) & (~BIT(6));
	bit2_val = (buf[11] & BIT(6)) >> 4;
	bit3_val = (buf[12] & BIT(0)) << 3;
	bit5_val = (buf[11] & BIT(2)) << 3;
	bit6_val = (buf[5] & BIT(0)) << 6;
	mark = mark | bit2_val | bit3_val | bit5_val | bit6_val;

#endif

#if defined(FLY_CONTROL_H66_T99)
	fly_command[i][0] = 0x66;																										// head
#elif defined(FLY_CONTROL_HCC_T33)
	fly_command[i][0] = 0xcc; // head
#endif
	fly_command[i][1] = left_ritht;																									// left and right
	fly_command[i][2] = forward_back;																								// forward and back
	fly_command[i][3] = buf[3];																										// throttle
	fly_command[i][4] = direction;																									// direction
	fly_command[i][5] = mark;																										// mark
	fly_command[i][6] = (fly_command[i][1] ^ fly_command[i][2] ^ fly_command[i][3] ^ fly_command[i][4] ^ fly_command[i][5]) & 0xff; // check
#if defined(FLY_CONTROL_H66_T99)
	fly_command[i][7] = 0x99;																										// trail
#elif defined(FLY_CONTROL_HCC_T33)
	fly_command[i][7] = 0x33; // trail
#endif
}

#endif

// volatile u8 is_uart_sending = 0;
u8 fly_control_enable = 0;
static u8 fly_start = 0;
extern u8 btuart_send_finish;
void fly_command_receive(u8 *buf, int len) // 这个在rtp read中被调用
{

	if (fly_command_check(buf, len) == false)
	{
		return;
	}
	if (btuart_send_finish == 1) // BTUART有数据在进行发送，现有的数据就丢掉
		return;
#if (FLY_CONTROL)
	if (fly_buf_changer == 0)
	{
#if (FLY_FIRMWARE_MODIFY)
		fly_command_transform(0, buf);
#else
		memcpy(fly_command[0], buf, FLY_COMMAND_LEN);
#endif
	}
	else
	{
#if (FLY_FIRMWARE_MODIFY)
		fly_command_transform(1, buf);
#else
		memcpy(fly_command[1], buf, FLY_COMMAND_LEN);
#endif
	}
	fly_buf_changer ^= 1; // change buffer    //改变buffer

	sys_sem_signal(&fly_command_event);
	// printf("---rec cmd\n");
	// 接收到数据，通知线程可以将网络接收到的数据通过串口发送出去
	// wake_up_legacy(fly_command_event);
#endif
}

// 创建一个线程，用于将网络接收到的数据通过串口DMA发送出去
void fly_command_thread(void *parameter)
{
	int i = 0;
	BaseType_t err = pdFALSE;
	sys_sem_new(&fly_command_event, 0);
	while (1)
	{
		// printf("---fly---\n");
		err = sys_arch_sem_wait(&fly_command_event, 0); // 获取信号量
		if (err != -1)
		{
			btuart_send_finish = 1;
			if (fly_buf_changer == 1)
			{
				// rt_fly_command_putc(fly_command[0][0]);    //这里改成DMA方式
				memcpy(fly_dma_buf, fly_command[0], FLY_COMMAND_LEN);
				btuart_start_DMA(fly_dma_buf, FLY_COMMAND_LEN);
			}
			else
			{
				// rt_fly_command_putc(fly_command[1][0]);
				memcpy(fly_dma_buf, fly_command[1], FLY_COMMAND_LEN);
				btuart_start_DMA(fly_dma_buf, FLY_COMMAND_LEN);
			}
			// printf("---send cmd\n");
		}
	}
}

////////////////////////////////////////////////////////////////////

#if 0
u8 ubuf[16];
extern xQueueHandle uart_queue;
void uart0_isr(void *param)
{
	static u8 i = 0;
	static u32 tx_i = 1;;
	static u32 re_len=FLY_COMMAND_LEN-1;

#ifdef FLY_CONTROL_RECEIVE
	if(UART0PND & BIT(0)) 
	{
		clear_rx_pending();
		uart0_receive(&ubuf[i++]);
		if(i == 16) {
			i = 0;	
			xQueueSendToBackFromISR(uart_queue, ubuf, 0);
			my_memset(ubuf,0,16);
		}
	}
#endif
	if(UART0PND & BIT(1)) 
	{
		clear_pending();
		
    if (fly_buf_changer == 1 ) 
		{
     	//uart0_putc(fly_command[0][tx_i]);
			btuart_putchar(fly_command[0][tx_i]);    //需要重新封装
    }
    else
		{
    	//uart0_putc(fly_command[1][tx_i]);
			btuart_putchar(fly_command[1][tx_i]);
    }
    tx_i ++;
    re_len --;
    if(!re_len)
		{
    	//UART0CON &= ~BIT(1);//transport interrupt disable
			BUCON &= ~BIT(3);
			
      tx_i = 1;
      re_len=FLY_COMMAND_LEN-1;
      is_uart_sending = 0;	   
    }
    
	//if(re_len == FLY_COMMAND_LEN-2)
	//	wake_up_FromISR(uart0_event);
		
	}
}


//串口接收完成中断
void fly_receive_isr(uint32_t irq, void *d, void *reg)
{
	static u8 i = 0;

#ifdef FLY_CONTROL_RECEIVE
	uart0_receive(&ubuf[i++]);
	if(i == 16) 
	{
		i = 0;	
		xQueueSendToBackFromISR(uart_queue, ubuf, 0);
		my_memset(ubuf,0,16);
	}
#else
	printf("not def fly rec\n");
#endif
}

//串口发送完成中断，计算长度，转换buffer
void fly_transport_isr(uint32_t irq, void *d, void *reg)
{

	static u32 tx_i = 1;;
	static u32 re_len=FLY_COMMAND_LEN-1;
	
	if (fly_buf_changer == 1 ) 
	{
    	//uart0_putc(fly_command[0][tx_i]);
		btuart_putchar(fly_command[0][tx_i]);    //需要重新封装
  }
  else
	{
   	//uart0_putc(fly_command[1][tx_i]);
		btuart_putchar(fly_command[1][tx_i]);
  }
   tx_i ++;
   re_len --;
  if(!re_len)
	{
   	//UART0CON &= ~BIT(1);//transport interrupt disable
		BUCON &= ~BIT(3);
		
    tx_i = 1;
    re_len=FLY_COMMAND_LEN-1;
    is_uart_sending = 0;	   
  }
}

#endif

void uart_fly_command_init(void)
{
#if (FLY_CONTROL)
	thread_create(fly_command_thread, NULL, TASK_FLY_COMMAND, 0, 1024, "fly_command");
	fly_buf_changer = 0;
	board_btuart_init();

#endif
}

////////////////////////////////////////////////////////////////////////
#if 0
u8 rxBUF[2048];
extern wait_event_t uart0_queue;
void uart0_isr_test(void *param)
{
	static u8 i = 0;
	//static u8 rxBUF[2048];
	u8 getdat;
	static u8 savebuf = 0; 
	static int bufnum = 0;
	static int buflen = 0; 
	if(UART0PND & BIT(0)) {
		clear_rx_pending();
		uart0_receive(&getdat);
#if 1
		if(savebuf == 1)
		{
			rxBUF[bufnum] = getdat;
			bufnum++;

			
			{
				if(10 == bufnum)
				{
#ifdef WIFI_MVL8801_SUPPORT	
					wake_up_FromISR(uart0_queue);
#endif					
					savebuf = 0;
					bufnum = 0; 
					buflen = 0;
				}
			}
		}
		else
		{
			if((getdat == 0x35)&&(bufnum == 0))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
			}
			else if((getdat == 0x35)&&(bufnum == 1))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
			}
			else if((getdat == 0x41)&&(bufnum == 2))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
			}
			else if((getdat == 0x61)&&(bufnum == 2))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
			}
			else if((getdat == 0x41)&&(bufnum == 3))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
				savebuf = 1;
			}
			else if((getdat == 0x61)&&(bufnum == 3))
			{
				rxBUF[bufnum] = getdat;
				bufnum++;
				savebuf = 1;
			}
			else
			{
				bufnum = 0;
				savebuf = 0;
			}
		}

#endif
	}
}
#endif
