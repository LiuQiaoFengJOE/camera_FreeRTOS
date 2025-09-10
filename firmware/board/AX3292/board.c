
#include "ax329x.h"
#include "ax329x_soc_drv.h"
/* Debug調試打印用的UART */
#define DBG_UARTTYPE UARTTYPE_SIMPLE
#if AX329X_TYPE == 3297
#define DBG_UARTGRP_TX UART0_GRP_6
//#define DBG_UARTGRP_TX UART0_GRP_7
#elif AX329X_TYPE == 3291
#define DBG_UARTGRP_TX UART0_GRP_6
#endif
#define DBG_UARTGRP_RX UART0_GRP_12
#define DBG_UARTBAUD 115200


//#define BTUARTGRP_TX BTUART_GRP_9
//#define BTUARTGRP_RX BTUART_GRP_7
#define  UART1_BAUDRATE   9600//460800

UINT32 APB_CLK = APB_CLK_CONFIG;
int dbg_level;

////////////////////////////////////////////////////////////////////////////////
// UART for Debug
extern void bt_rxRecv(u8 data);

void uart_init(void)
{

#if DBG_UARTTYPE == UARTTYPE_SIMPLE
  uart0_io_init(DBG_UARTGRP_TX, DBG_UARTGRP_RX);
  uart0_init(DBG_UARTBAUD);
#endif
#if 1//DBG_UARTTYPE == UARTTYPE_BT
//  btuart_io_init(BTUARTGRP_TX, BTUARTGRP_RX);
//  btuart_init(BTUARTBAUD, bt_rxRecv);

	ax32xx_uart1IOCfg(1);
	ax32xx_uart1Init(UART1_BAUDRATE,bt_rxRecv);
#endif
}


int putchar(int ch)
{
#if 1
#if DBG_UARTTYPE == UARTTYPE_SIMPLE
  uart0_putchar(ch);
#endif

#if DBG_UARTTYPE == UARTTYPE_BT
  btuart_putchar(ch);
#endif
#endif
  return ch;
}

////////////////////////////////////////////////////////////////////////////////
extern void boot_table_init(uint32_t *); // import from ld-scripts     MASK的代码
__attribute__((section(".text.bootbsp.init"))) void boot_uart_init(void)
{
#if DBG_UARTTYPE == UARTTYPE_BT
#if DBG_UARTGRP_TX == BTUART_GRP_1
  extern uint32_t btuart_bootinit_1[];
  boot_table_init(btuart_bootinit_1); // 这是调用mask的代码，初始化寄存器组
#elif DBG_UARTGRP_TX == BTUART_GRP_2
  extern uint32_t btuart_bootinit_2[];
  boot_table_init(btuart_bootinit_2);
#elif DBG_UARTGRP_TX == BTUART_GRP_3
  extern uint32_t btuart_bootinit_3[];
  boot_table_init(btuart_bootinit_3);
#else /* DBG_UARTGRP_TX */
#error "DBG_UARTGRP_TX"
#endif /* DBG_UARTGRP_TX */
  btuart_baud_init(APB_CLK_CONFIG / DBG_UARTBAUD - 1);
#elif DBG_UARTTYPE == UARTTYPE_SIMPLE
#if DBG_UARTGRP_TX == UART0_GRP_1
  extern uint32_t uart0_bootinit_1[];
  boot_table_init(uart0_bootinit_1);
#elif DBG_UARTGRP_TX == UART0_GRP_2
  extern uint32_t uart0_bootinit_2[];
  boot_table_init(uart0_bootinit_2);
#elif DBG_UARTGRP_TX == UART0_GRP_3
  extern uint32_t uart0_bootinit_3[];
  boot_table_init(uart0_bootinit_3);
#elif DBG_UARTGRP_TX == UART0_GRP_4
  extern uint32_t uart0_bootinit_4[];
  boot_table_init(uart0_bootinit_4);
#elif DBG_UARTGRP_TX == UART0_GRP_5
  extern uint32_t uart0_bootinit_5[];
  boot_table_init(uart0_bootinit_5);
#elif DBG_UARTGRP_TX == UART0_GRP_6
  extern uint32_t uart0_bootinit_6[];
  boot_table_init(uart0_bootinit_6);
#elif DBG_UARTGRP_TX == UART0_GRP_7
  extern uint32_t uart0_bootinit_7[];
  boot_table_init(uart0_bootinit_7);
#elif DBG_UARTGRP_TX == UART0_GRP_8
  extern uint32_t uart0_bootinit_8[];
  boot_table_init(uart0_bootinit_8);
#elif DBG_UARTGRP_TX == UART0_GRP_9
  extern uint32_t uart0_bootinit_9[];
  boot_table_init(uart0_bootinit_9);
#elif DBG_UARTGRP_TX == UART0_GRP_10
  extern uint32_t uart0_bootinit_10[];
  boot_table_init(uart0_bootinit_10);
#elif DBG_UARTGRP_TX == UART0_GRP_11
  extern uint32_t uart0_bootinit_11[];
  boot_table_init(uart0_bootinit_11);
#elif DBG_UARTGRP_TX == UART0_GRP_12
  extern uint32_t uart0_bootinit_12[];
  boot_table_init(uart0_bootinit_12);
#elif DBG_UARTGRP_TX == UART0_GRP_12
  extern uint32_t uart0_bootinit_12[];
  boot_table_init(uart0_bootinit_12);
#elif DBG_UARTGRP_TX == UART0_GRP_13
  extern uint32_t uart0_bootinit_13[];
  boot_table_init(uart0_bootinit_13);
#elif DBG_UARTGRP_TX == UART0_GRP_14
  extern uint32_t uart0_bootinit_14[];
  boot_table_init(uart0_bootinit_14);
#elif DBG_UARTGRP_TX == UART0_GRP_15
  extern uint32_t uart0_bootinit_15[];
  boot_table_init(uart0_bootinit_15);
#elif DBG_UARTGRP_TX == UART0_GRP_16
  extern uint32_t uart0_bootinit_16[];
  boot_table_init(uart0_bootinit_16);
#elif DBG_UARTGRP_TX == UART0_GRP_17
  extern uint32_t uart0_bootinit_17[];
  boot_table_init(uart0_bootinit_17);
#elif DBG_UARTGRP_TX == UART0_GRP_18
  extern uint32_t uart0_bootinit_18[];
  boot_table_init(uart0_bootinit_18);
#elif DBG_UARTGRP_TX == UART0_GRP_19
  extern uint32_t uart0_bootinit_19[];
  boot_table_init(uart0_bootinit_19);
#elif DBG_UARTGRP_TX == UART0_GRP_20
  extern uint32_t uart0_bootinit_20[];
  boot_table_init(uart0_bootinit_20);
#elif DBG_UARTGRP_TX == UART0_GRP_21
  extern uint32_t uart0_bootinit_21[];
  boot_table_init(uart0_bootinit_21);
#elif DBG_UARTGRP_TX == UART0_GRP_22
  extern uint32_t uart0_bootinit_22[];
  boot_table_init(uart0_bootinit_22);
#elif DBG_UARTGRP_TX == UART0_GRP_23
  extern uint32_t uart0_bootinit_23[];
  boot_table_init(uart0_bootinit_23);
#elif DBG_UARTGRP_TX == UART0_GRP_24
  extern uint32_t uart0_bootinit_24[];
  boot_table_init(uart0_bootinit_24);
#elif DBG_UARTGRP_TX == UART0_GRP_25
  extern uint32_t uart0_bootinit_25[];
  boot_table_init(uart0_bootinit_25);
#elif DBG_UARTGRP_TX == UART0_GRP_26
  extern uint32_t uart0_bootinit_26[];
  boot_table_init(uart0_bootinit_26);


#else /* DBG_UARTGRP_TX */
#error "DBG_UARTGRP_TX"
#endif /* DBG_UARTGRP_TX */
  uart0_baud_init(APB_CLK_CONFIG / DBG_UARTBAUD - 1);
#else
#error "unknown DBG_UARTTYPE"
#endif /* DBG_UARTTYPE */
}

////////////////////////////////////////////////////////////////////////////////
// Timer for Tick
extern void timer0_clrpnd(void);
extern void timer1_clrpnd(void);
extern int os_ticktimer_isr(uint32_t irq, void (*clr_pnd)(void), uint32_t *regs);

void tick_timer_init(uint32_t baud)
{
#if TICKTIMER_TYPE == TIMERTYPE_16BIT
#if TICKTIMER_SELECT == TIMER16BIT_0
  timer0_init(baud);
  irq_register(TICKTIMER_IRQNO, 0, os_ticktimer_isr, timer0_clrpnd);
#else
#error "unknown TIMER16BIT TYPE"
#endif
#else
#error "unknown TICKTIMER_TYPE"
#endif
}

extern void *sdc0_io_init(void);
////////////////////////////////////////////////////////////////////////////////
void board_early_init(void)
{
  wdt_disable();
  icache_init();
  dcache_init();
  // dma_memcpy_init (); // 此時堆棧在sram
}

////////////////////////////////////////////////////////////////////////////////
// lcd TODO
#if AX329X_TYPE == 3297
#define LCD_BK_CH 		GPIO_PA
#if DBG_UARTGRP_TX==UART0_GRP_6
#define LCD_BK_PIN 		GPIO_PIN9
#else
#define LCD_BK_PIN 		GPIO_PIN7
#endif
#elif AX329X_TYPE == 3291
#define LCD_BK_CH  		GPIO_PA
#define LCD_BK_PIN 		GPIO_PIN9
#endif

//#define LCD_SEL_CH  		GPIO_PA
//#define LCD_SEL_PIN 		GPIO_PIN5
#if 0
#define LCD_RST_CH  		GPIO_PA
#if DBG_UARTGRP_TX==UART0_GRP_6
#define LCD_RST_PIN 		GPIO_PIN9
#else
#define LCD_RST_PIN 		GPIO_PIN7
#endif
#endif
#define BACK_SENSOR_PW_CH 		GPIO_PD
#define BACK_SENSOR_PW_PIN 		GPIO_PIN4

#define SENSOR_RESET_CH  GPIO_PB
#define SENSOR_RESRT_PIN GPIO_PIN0

#define  SENSOR_PWDN_CH 	 GPIO_PB
#define  SENSOR_PWDN_PIN	GPIO_PIN6  //GPIO_PIN14

void lcd_ioinit(void)
{
  //return;
  #if LCD_EN
  printf("==lcd_ioinit ok\n");
  hal_gpioInit(LCD_BK_CH, LCD_BK_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
  hal_gpioWrite(LCD_BK_CH, LCD_BK_PIN, GPIO_LOW);
  #endif
}

// 背光控制
void lcd_bk_ctrl(u8 en)
{
	#if LCD_EN
	if (en)
	{
		hal_gpioInit(LCD_BK_CH, LCD_BK_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
  		hal_gpioWrite(LCD_BK_CH, LCD_BK_PIN, GPIO_HIGH);
	}
	else
	{
		hal_gpioInit(LCD_BK_CH, LCD_BK_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
  		hal_gpioWrite(LCD_BK_CH, LCD_BK_PIN, GPIO_LOW);
		setKeyEn(false);
	}
	#endif
}
int lcd_bk_on(void)
{
	#if LCD_EN
	if(hal_gpioRead(LCD_BK_CH, LCD_BK_PIN))
		return 1;
	return 0;
	#else
	return 1;
	#endif
}

void lcd_reset(void)
{
#if 1//LCD_EN
//#if 1
//	hal_gpioInit(LCD_RST_CH, LCD_RST_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
  //hal_gpioInit(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);

  //--------------LCD & SENSOR RESET----------------
  ax32xx_dacHPSet(1, HP_VDD_2_8V);
//  hal_gpioWrite(LCD_RST_CH, LCD_RST_PIN, GPIO_HIGH);
 // hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
  osal_task_sleep(50);

  ax32xx_dacHPSet(0, 0);
  //hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_LOW);
//  hal_gpioWrite(LCD_RST_CH, LCD_RST_PIN, GPIO_LOW);
  osal_task_sleep(100);

  ax32xx_dacHPSet(1, HP_VDD_2_8V);
//  hal_gpioWrite(LCD_RST_CH, LCD_RST_PIN, GPIO_HIGH);
 // hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
  osal_task_sleep(50);
//#endif
#endif
}


void sensor_reset(u8 no)
{
	printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
#if 1//LCD_EN
	hal_gpioInit(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
	if (no == 0)
	{
		printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);

		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_LOW);
		osal_task_sleep(100);

		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);
	} 
	else
	{
		printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_LOW);
	}
#else
	hal_gpioInit(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	hal_gpioInit(SENSOR2_RESET_CH,SENSOR2_RESRT_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
	if (no == 0)
	{
		printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
		hal_gpioWrite(SENSOR2_RESET_CH,SENSOR2_RESRT_PIN,GPIO_LOW);
		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);

		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_LOW);
		osal_task_sleep(100);

		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);
	} 
	else
	{
		printf("==@#@@@==%s,%d\n", __FUNCTION__, __LINE__);
		hal_gpioWrite(SENSOR_RESET_CH,SENSOR_RESRT_PIN,GPIO_LOW);
		hal_gpioWrite(SENSOR2_RESET_CH,SENSOR2_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);

		hal_gpioWrite(SENSOR2_RESET_CH,SENSOR2_RESRT_PIN,GPIO_LOW);
		osal_task_sleep(100);

		hal_gpioWrite(SENSOR2_RESET_CH,SENSOR2_RESRT_PIN,GPIO_HIGH);
		osal_task_sleep(50);

	}
#endif
}

void sensor_pwdn(u8 no)
{
#if 1
	hal_gpioInit(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	if (no == 0)
	{
		hal_gpioWrite(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_HIGH);
	}
	else
	{
		hal_gpioWrite(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_LOW);
	}
#else
	hal_gpioInit(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	hal_gpioInit(SENSOR2_PWDN_CH,SENSOR2_PWDN_PIN,GPIO_OUTPUT,GPIO_PULLE_FLOATING);
	if (no == 0)
	{
		hal_gpioWrite(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_HIGH);
		hal_gpioWrite(SENSOR2_PWDN_CH,SENSOR2_PWDN_PIN,GPIO_LOW);
	}
	else
	{
		hal_gpioWrite(SENSOR_PWDN_CH,SENSOR_PWDN_PIN,GPIO_LOW);
		hal_gpioWrite(SENSOR2_PWDN_CH,SENSOR2_PWDN_PIN,GPIO_HIGH);
	}
#endif
}

void usb1CameraPowerEn(int en)
{
	//printf("usb1 camera power:%d\n",en);
	if (en)
	{
		//hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_OUTPUT, GPIO_PULL_UP);// UVC
		//hal_gpioWrite(GPIO_PA, GPIO_PIN7, GPIO_HIGH);
	}
	else
	{
		//hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_OUTPUT, GPIO_PULL_FLOATING);// UVC
		//hal_gpioWrite(GPIO_PA, GPIO_PIN7, GPIO_LOW);
	}
}
void usb2CameraPowerEn(int en)
{
	//printf("usb2 camera power:%d\n",en);
#if 0	
	if (en)
	{
		hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_OUTPUT, GPIO_PULL_UP);// UVC
		hal_gpioWrite(GPIO_PA, GPIO_PIN7, GPIO_HIGH);
	}
	else
	{
		hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_OUTPUT, GPIO_PULL_FLOATING);// UVC
		hal_gpioWrite(GPIO_PA, GPIO_PIN7, GPIO_LOW);
	}
#endif	
}

////////////////////////////////////////////////////////////////////////////////
//extern keys_t board_keys;
void board_init()
{
  u8 a[20], b[20], c[20];
  a[0] = 1;
  b[0] = 2;

  ax32xx_WKOEnable(1);        // 将WKO来一直拉高
  ax32xx_WKI1WakeupEnable(1); // 使能
  ax32xx_WKI1mode(0);         // 设置为高电平触发
  ax32xx_WKI1CleanPending();  // 清除pending,如果后续还有pengding的话,代表usb供电

  dbg_level = 1;
  irq_init();
  spidisk_register(0, &spidisk_device); // as root disk

  register_sdcard(1, 1, sdc0_io_init);//sdno define 1:SDC1, other: SDC0

  // irq_register(18, 0, ax32xx_auadcIRQHandler, NULL);	//音频中断注册
#if LED_MODE == STARTUART
  uart_init();

#else if LED_MODE == STARTRECLED
#endif


  ax32xx_first_mem_dma(a, b, 20);
  irqTimerInit();
  tick_timer_init(1000);
  lcd_ioinit();
//	  hal_gpioInit(BACK_SENSOR_PW_CH, BACK_SENSOR_PW_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
//	  hal_gpioWrite(BACK_SENSOR_PW_CH, BACK_SENSOR_PW_PIN, GPIO_HIGH);
  //key_init(&board_keys);
}
