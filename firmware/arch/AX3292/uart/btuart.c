#include "AX329x.h"
#include "basic_defs.h"
#include "spr_defs.h"
#include "typedef.h"
#include "ax329x_irq.h"
#include "sys/sys.h"
#include "gpio_ops/gpio.h"
#include "portmacro.h"
#include "board_config.h"

extern UINT32 APB_CLK; // defined by board
//===========uart1 ===========
//#define  R_UART_CTRL1         BUCON
//#define  R_UART_BAUDRATE1     BUBAUD
//#define  R_UART_PEND1         BUSTA
//#define  R_UART_DATA1         BUDATA

struct btuart_spec_t
{
	volatile __sfr unsigned long *pufmap;
	volatile __sfr unsigned long *pudir;
	UINT32 pin;
};

static const struct btuart_spec_t btuart_spec[] = {
	{NULL, NULL, 0},
	{&PAFMAP, &PADIR, 4},  // group 0 
	{&PAFMAP, &PADIR, 5},  // group 1
	{&PAFMAP, &PADIR, 6},  // group 2
	{&PAFMAP, &PADIR, 7},  // group 3
	{&PAFMAP, &PADIR, 9},  // group 4
	{&PAFMAP, &PADIR, 10}, // group 5
	{&PAFMAP, &PADIR, 12}, // group 6
	{&PBFMAP, &PBDIR, 0},  // group 7
	{&PBFMAP, &PBDIR, 4},  // group 8
	{&PBFMAP, &PBDIR, 5},  // group 9
	{&PBFMAP, &PBDIR, 6},  // group 10
	{&PDFMAP, &PDDIR, 0},  // group 11
	{&PDFMAP, &PDDIR, 8},  // group 12
	{&PDFMAP, &PDDIR, 9},  // group 13
	{&PDFMAP, &PDDIR, 10}, // group 14
	{&PFFMAP, &PFDIR, 0},  // group 15
	{&PFFMAP, &PFDIR, 1},  // group 16
	{&PFFMAP, &PFDIR, 8},  // group 17
	{&PFFMAP, &PFDIR, 10}, // group 18
	{&PFFMAP, &PFDIR, 11}, // group 19
	{&PEFMAP, &PEDIR, 0},  // group 20
	{&PEFMAP, &PEDIR, 1},  // group 21
	{&PEFMAP, &PEDIR, 2},  // group 22
	{&PEFMAP, &PEDIR, 3},  // group 23
	{&PEFMAP, &PEDIR, 15} // group 24
	
};

// 0 10  0开始计算

/**
 * @brief 初始化uart1的IO
 * 指定UART0的Tx/Rx分組，進行IO初始化
 * @param tx_grp [in] UART Tx的組號+1，0表示没映射
 * @param rx_grp [in] UART Tx的組號+1，0表示没映射
 */
void btuart_io_init(int32_t tx_grp, int32_t rx_grp) // tx->PA4 G0,  rx->PB6	G10 即传入 1 11
{
	printf("\n APB_CLK \n");
	PMAPCON0 |= (PMAPCON0 & ~(0x3FF<<10)) | ((tx_grp&0x1F) << 15) | ((rx_grp&0x1F) << 10); // 串口组选择

	if (tx_grp)
	{
		*btuart_spec[tx_grp].pufmap |= BIT(btuart_spec[tx_grp].pin); //
		*btuart_spec[tx_grp].pudir &= ~BIT(btuart_spec[tx_grp].pin);
	}
	if (rx_grp)
	{
		*btuart_spec[rx_grp].pufmap |= BIT(btuart_spec[rx_grp].pin);
		*btuart_spec[rx_grp].pudir |= BIT(btuart_spec[rx_grp].pin);
	}
}

/**
 * @brief 初始化btuart，設定波特率
 * @param baud [in] 指定波特率
 */
typedef int (*isr_func)(unsigned int, void *, void *);
void btuart_register_tx(isr_func isr, void *dev_id);

u8 btuart_send_finish = 0;
void btuart_tx_finish()
{
	btuart_send_finish = 0;
	// BUDMATXCNT = 0;
	BUCON &= ~BIT(3); // 关中断tx
	 printf("BUSTA_tx:%x\n",BUSTA);
}

static void (*uart1Callback)(u8 data);
void btuart_rx_finish(void)
{
    BUSTA &= ~BIT(3);	// clear rec int flag
	if(uart1Callback)
		uart1Callback(REG32(BUDATA)&0xff);
}

void btuart_init(UINT32 baud,void (*isr)(u8 data))
{
	printf("btuart_init\n");
	uart1Callback = isr;
	BUCON = 0;
	BUBAUD = APB_CLK / baud - 1;
	BUSTA &= ~BIT(3);
	btuart_register_tx(btuart_tx_finish, NULL);
	btuart_register_rx(btuart_rx_finish, NULL);
	BUCON |= BIT(0); // 自动DMA
	BUCON |= BIT(4); // 使能
	//  BUCON |= BIT(2);	   //中断  rx
	//	BUCON |= BIT(3);     //中断tx
}


// 要DMA的地址要放在SRAM，不能放到sdram
void btuart_start_DMA(void *addr, u8 data_len)
{
	// test_io_put(1);
	if ((BUSTA & BIT(2)) == 0)
		return;
	BUDMATXADR = (u32)addr;	   // buffer地址
	BUDMATXCNT = data_len - 1; // 启动DMA
	BUCON |= BIT(3);		   // 开中断tx

	// test_io_put(0);
	// gpio = 0;
}

u8 btuart_get_DMATX_pending(void)
{
	if (BUSTA & BIT(2)) // 完成
		return 1;
	else
		return 0;
}

// 发送数据，测试用
void btuart_putchar(UINT32 c)
{
	BUDATA = c;
}

// 接收数据，测试用
void btuart_receive(u8 *ubuf)
{
	*ubuf = (BUDATA); // 读取
}

#if 1
#define  UART1_TX_POS	  UART1_POS_PB5//UART1_POS_PA7
#define  UART1_RX_POS	  UART1_POS_PB0//UART1_POS_NONE

/*******************************************************************************
* Function Name  : ax32xx_uart0IOCfg
* Description    : uart0 rx tx config
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1IOCfg(u8 en)
{
	u8 ch;
	u32 pin;

	if(en)
	{
		if(UART1_TX_POS == UART1_POS_PB6)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN6;
		}
		else if(UART1_TX_POS == UART1_POS_PB5)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN5;
		}else if(UART1_TX_POS == UART1_POS_PA7)
		{
			ch = GPIO_PA;
			pin = GPIO_PIN7;
		}else if(UART1_TX_POS == UART1_POS_PE1)
		{
			ch = GPIO_PE;
			pin = GPIO_PIN1;
		}
		else
		{
			ch = 0;
			pin = 0;
		}
		if (pin != 0)
		{
			ax32xx_gpioDirSet (ch,pin,GPIO_OUTPUT);
			ax32xx_gpioPullSet(ch,pin,GPIO_PULL_UP);
			ax32xx_gpioDataSet(ch,pin,GPIO_HIGH);
			ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_SFR);	  // set as gpio func
			ax32xx_gpioSFRSet(GPIO_MAP_UARTTX1,UART1_TX_POS);	
        }

		if(UART1_RX_POS == UART1_POS_PB6)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN6;
		}
		else if(UART1_RX_POS == UART1_POS_PB5)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN5;
		}else if(UART1_RX_POS == UART1_POS_PB0)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN0;
		}
		else
		{
			ch = 0;
			pin = 0;
		}
		if (pin != 0)
		{
			ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
			ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_SFR);	  // set as gpio func
			ax32xx_gpioDirSet(ch,pin,GPIO_INPUT);					  // set direction
			ax32xx_gpioLedPull(ch,pin,GPIO_PULLE_FLOATING);  // set LED pull level, 
			ax32xx_gpioSFRSet(GPIO_MAP_UARTRX1,UART1_RX_POS);
		}
	}
	else
	{
		ax32xx_gpioSFRSet(GPIO_MAP_UARTTX1,UART1_POS_NONE);
		ax32xx_gpioSFRSet(GPIO_MAP_UARTRX1,UART1_POS_NONE);
	}
}

/*******************************************************************************
* Function Name  : ax32xx_uart0Init
* Description    : uart0 initial 
* Input          : u32 baudrate : uart0 baudrate
                      void (*isr)(u8 data) : rx callback
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1Init(u32 baudrate,void (*isr)(u8 data))
{
    uart1Callback = isr;
	//btuart_register_tx(btuart_tx_finish, NULL);
	btuart_register_rx(btuart_rx_finish, NULL);
	
	BUCON = 0;
	BUBAUD = APB_CLK/baudrate-1; //baud rate 
	//BUSTA |= 1<<3;    
	BUCON |= (1<<4);         
	BUCON |= (1<<2); //RX int enable
	
	ax32xx_intEnable(IRQ_BTUART,1); // enable irq
}

/*******************************************************************************
* Function Name  : ax32xx_uart1SendByte
* Description    : uart1 send data
* Input          : u8 data : send data
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1SendByte(u8 data)
{
	(BUDATA) = data;
	while(((BUSTA) & 0x4)==0);
	return;
}

#endif
////////////////////////测试飞控用//////////////////////////////
#if 0
void fly_putchar(char ch)
{
	btuart_putchar(ch);
	BUCON |= BIT(3);     //en int
}

#define FLY_COMMAND_LEN 10
u8 fly_command[FLY_COMMAND_LEN] = {1,2,3,4,5,6,7,8,9,10};


void btuart_tx_isr1(uint32_t irq, void *d, void *reg)
{
	static u32 tx_i = 1;
	static u32 re_len=FLY_COMMAND_LEN-1;
	
	btuart_putchar(fly_command[tx_i]);    //需要重新封装
  tx_i ++;
  re_len --;
  if(!re_len)
	{
		BUCON &= ~BIT(3);
		
    tx_i = 1;
    re_len=FLY_COMMAND_LEN-1;
  }
}

#define IO_GROUP 1		// GPIOB
#define PUT_IO_PIN 0x40 // pin6


void test_io_init(void)
{
	u32 pins = PUT_IO_PIN;	//BL
	ax32xx_gpioDigitalSet(IO_GROUP,pins,1);
	ax32xx_gpioDirSet 	 (IO_GROUP,pins,0);
	ax32xx_gpioMapSet 	 (IO_GROUP,pins,0);
	hal_gpioWrite 			 (IO_GROUP,pins,0);
}

void test_io_put(u8 hl)   
{
	hal_gpioWrite(IO_GROUP,PUT_IO_PIN,hl);
}

#endif
/////////////////////////////////////////////////////////////////

/////////////////////////中断注册相关函数/////////////////////////////
#define IRQ_BTUART 20

typedef enum
{
	BTUART_IRQ_RX = 0,
	BTUART_IRQ_TX
} BTUART_IRQ_E;

isr_func btuart_vector_table[2];
void *btuart_dev_table[2];

void btuart_irq(unsigned int irqno, void *d, void *regs);
void btuartirq_register(uint32_t irq, isr_func isr, void *dev_id)
{
	irq_register(IRQ_BTUART, 0, btuart_irq, NULL);
	btuart_vector_table[irq] = isr;
	btuart_dev_table[irq] = dev_id;

	printf("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, IRQ_BTUART, isr);
}

void btuart_register_rx(isr_func isr, void *dev_id)
{
	btuartirq_register(BTUART_IRQ_RX, isr, dev_id);
}

void btuart_register_tx(isr_func isr, void *dev_id)
{
	btuartirq_register(BTUART_IRQ_TX, isr, dev_id);
}

void btuart_irq(unsigned int irqno, void *d, void *regs)
{
	//	printf("BUSTA:%x  PIC:%x\n", BUSTA, SPR_PICVR);
	BUDMATXADR = 0;
	if ((BUSTA & BIT(3)))
	{
		BUSTA &= ~BIT(3); // clear rx sta
		if (btuart_vector_table[BTUART_IRQ_RX])
			btuart_vector_table[BTUART_IRQ_RX](BTUART_IRQ_RX, btuart_dev_table[BTUART_IRQ_RX], regs);
	}

	else if ((BUSTA & BIT(2)))
	{
		if (btuart_vector_table[BTUART_IRQ_TX])
			btuart_vector_table[BTUART_IRQ_TX](BTUART_IRQ_TX, btuart_dev_table[BTUART_IRQ_TX], regs);
	}
}
