#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"

#include "gpio_ops/gpio.h"
#include "sys/sys.h"

/* todo
 * 不應該在這裡用define的方式去決定IO
 * 而應交給板級配置
 */

#define LCD_POS_CS_PF13_WR_PF10_RS_PF11_TE_PF12_DATA_PF7_0 5

#if AX329X_TYPE == 3291
#define LCD_POS LCD_POS12//LCD_POS14//LCD_POS9
#else
#define LCD_POS LCD_POS12
#endif
// #define LCD_POS LCD_POS_CS_PF13_WR_PF10_RS_PF14_TE_PF12_DATA_PF7_0
// #define LCD_POS LCD_POS_CS_PF13_WR_PF10_RS_PF11_TE_PF12_DATA_PD15_6

////////////////////////////////////////////////////////////////////////////////
#define IRQNO_LCDC 4

enum
{
	IRQNO_LCDC_FRAME_START,
	IRQNO_LCDC_FRAME_END,
	IRQNO_LCDC_LINE_END,
	IRQNO_LCDC_DATA_END,
	IRQNO_LCDC_TE,
	IRQNO_LCDC_DUMMY5,
	IRQNO_LCDC_DUMMY6,
	IRQNO_LCDC_FIFO_OV
};
////////////////////////////////////////////////////////////////////////////////

typedef struct LCD_SPI_MODE_S
{
	u8 CPOL,   // CPOL = 0,CLK is "Low" when inactive
		CPHA,  // CPHA = 0,sample edge is first edge
		ORDER, // ORDER = 0,MSB
		BITS;  // number of data bits
} LCD_SPI_MODE_T;

#define R_LCD_CTRL LCDCTRL
#define R_LCD_STATUS LCDSTA
#define R_LCD_COMMAND LCDCMD
#define R_LCD_RLINE LCDPREL
#define R_LCD_SIGNAL LCDSCON
#define R_LCD_SIGNAL0 LCDDOE
#define R_LCD_VSYNCF LCDVSRP
#define R_LCD_HSYNCF LCDHSRP
#define R_LCD_VSYNCB LCDVSFP
#define R_LCD_HSYNCB LCDHSFP
#define R_LCD_DEF LCDDERP
#define R_LCD_DEB LCDDEFP
#define R_LCD_DESTART LCDDESL
#define R_LCD_DEEND LCDDEEL
#define R_LCD_POSX LCDFSTX
#define R_LCD_POSY LCDFSTY
#define R_LCD_WIDTH LCDFRMW
#define R_LCD_HEIGHT LCDFRMH
#define R_LCD_WINWIDTH LCDWINX
#define R_LCD_WINHEIGHT LCDWINY
#define R_LCD_MODE LCDDMOD
#define R_LCD_CSSTATUS LCDCST
#define R_LCD_ENDLINE LCDENDL
#define R_LCD_ACTIVELINE LCDBLKL
#define R_LCD_NUMBER LCDDNUM
#define R_LCD_CLKSEL LCDTCON
#define R_LCD_CMMAND LCDCMD
#define R_LCD_MODE1 LCDDMOD1
#define R_LCD_RGBORD LCDRGBORD

#if AX329X_TYPE == 3291
#define  LCD_SPI_CS_CH     GPIO_PA
#define  LCD_SPI_CS_PIN    GPIO_PIN5

#define  LCD_SPI_CLK_CH    GPIO_PD
#define  LCD_SPI_CLK_PIN   GPIO_PIN7

#define  LCD_SPI_DAT_CH    GPIO_PD
#define  LCD_SPI_DAT_PIN   GPIO_PIN8

#define  LCD_MCU_RS_CH     GPIO_PF
#define  LCD_MCU_RS_PIN    GPIO_PIN2

#else
#if 1
#define LCD_SPI_CS_CH GPIO_PD
#define LCD_SPI_CS_PIN GPIO_PIN6

#define LCD_SPI_CLK_CH GPIO_PD
#define LCD_SPI_CLK_PIN GPIO_PIN7

#define LCD_SPI_DAT_CH GPIO_PD
#define LCD_SPI_DAT_PIN GPIO_PIN8
#else
#define LCD_SPI_CS_CH GPIO_PF
#define LCD_SPI_CS_PIN GPIO_PIN4

#define LCD_SPI_CLK_CH GPIO_PF
#define LCD_SPI_CLK_PIN GPIO_PIN1

#define LCD_SPI_DAT_CH GPIO_PF
#define LCD_SPI_DAT_PIN GPIO_PIN3

#endif
#define LCD_MCU_RS_CH GPIO_PF
#define LCD_MCU_RS_PIN GPIO_PIN2

#define LCD_MCU_CS_CH GPIO_PF
#define LCD_MCU_CS_PIN GPIO_PIN4

#define LCD_MCU_WR_CH GPIO_PF
#define LCD_MCU_WR_PIN GPIO_PIN1
#endif
#define  LCD_SPI_PANEL_MOSI_CH		GPIO_PF 	 //data out for 1 wire mode 
#define  LCD_SPI_PANEL_MOSI_PIN 	GPIO_PIN3//GPIO_PIN10

#define  LCD_SPI_PANEL_SCK_CH		 GPIO_PF	 // clk for spi panel
#define  LCD_SPI_PANEL_SCK_PIN		 GPIO_PIN1//GPIO_PIN10

#define  LCD_SPI_PANEL_CS_CH		 GPIO_PF	 // chip select pin, low enable
#define  LCD_SPI_PANEL_CS_PIN		GPIO_PIN4// GPIO_PIN11

#define  LCD_SPI_PANEL_RS_CH		 GPIO_PF // data(1)/cmd(0) select
#define  LCD_SPI_PANEL_RS_PIN		GPIO_PIN2// GPIO_PIN8	

#define LCD_SPI_CS_L ax32xx_gpioDataSet(LCD_SPI_CS_CH, LCD_SPI_CS_PIN, GPIO_LOW)
#define LCD_SPI_CS_H ax32xx_gpioDataSet(LCD_SPI_CS_CH, LCD_SPI_CS_PIN, GPIO_HIGH)

#define LCD_SPI_CLK_L ax32xx_gpioDataSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN, GPIO_LOW)
#define LCD_SPI_CLK_H ax32xx_gpioDataSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN, GPIO_HIGH)

#define LCD_SPI_DAT_L ax32xx_gpioDataSet(LCD_SPI_DAT_CH, LCD_SPI_DAT_PIN, GPIO_LOW)
#define LCD_SPI_DAT_H ax32xx_gpioDataSet(LCD_SPI_DAT_CH, LCD_SPI_DAT_PIN, GPIO_HIGH)

#define LCD_MCU_RS_H ax32xx_gpioDataSet(LCD_MCU_RS_CH, LCD_MCU_RS_PIN, GPIO_HIGH)
#define LCD_MCU_RS_L ax32xx_gpioDataSet(LCD_MCU_RS_CH, LCD_MCU_RS_PIN, GPIO_LOW)

//#define LCD_MCU_CS_H ax32xx_gpioDataSet(LCD_MCU_CS_CH, LCD_MCU_CS_PIN, GPIO_HIGH)
//#define LCD_MCU_CS_L ax32xx_gpioDataSet(LCD_MCU_CS_CH, LCD_MCU_CS_PIN, GPIO_LOW)

//#define LCD_MCU_WR_H ax32xx_gpioDataSet(LCD_MCU_WR_CH, LCD_MCU_WR_PIN, GPIO_HIGH)
//#define LCD_MCU_WR_L ax32xx_gpioDataSet(LCD_MCU_WR_CH, LCD_MCU_WR_PIN, GPIO_LOW)

#define LCD_SPI_DELAY ax32xx_sysCpuNopDelay(100)

static LCD_SPI_MODE_T lcd_spi_cfg = // default set
	{
		.CPOL = 0,
		.CPHA = 0,
		.ORDER = 0,
		.BITS = 8};

void lcd_printf()
{
	printf("LCDCTRL  :%x\n", LCDCTRL);
	printf("LCDSTA   :%x\n", LCDSTA);
	printf("LCDCMD   :%x\n", LCDCMD);
	printf("LCDPREL  :%x\n", LCDPREL);
	printf("LCDSCON  :%x\n", LCDSCON);
	printf("LCDDOE   :%x\n", LCDDOE);
	printf("LCDVSRP  :%x\n", LCDVSRP);
	printf("LCDHSRP  :%x\n", LCDHSRP);
	printf("LCDVSFP  :%x\n", LCDVSFP);
	printf("LCDHSFP  :%x\n", LCDHSFP);
	printf("LCDDERP  :%x\n", LCDDERP);
	printf("LCDDEFP  :%x\n", LCDDEFP);
	printf("LCDDESL  :%x\n", LCDDESL);
	printf("LCDDEEL  :%x\n", LCDDEEL);
	printf("LCDFSTX  :%x\n", LCDFSTX);
	printf("LCDFSTY  :%x\n", LCDFSTY);
	printf("LCDFRMW  :%x\n", LCDFRMW);
	printf("LCDFRMH  :%x\n", LCDFRMH);
	printf("LCDWINX  :%x\n", LCDWINX);
	printf("LCDWINY  :%x\n", LCDWINY);
	printf("LCDDMOD  :%x\n", LCDDMOD);
	printf("LCDCST   :%x\n", LCDCST);
	printf("LCDENDL  :%x\n", LCDENDL);
	printf("LCDBLKL  :%x\n", LCDBLKL);
	printf("LCDDNUM  :%x\n", LCDDNUM);
	printf("LCDTCON  :%x\n", LCDTCON);
	printf("LCDCMD   :%x\n", LCDCMD);
	printf("LCDDMOD1 :%x\n", LCDDMOD1);
	printf("LCDRGBORD:%x\n", LCDRGBORD);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdSPIMode
 * Description    : lcd spi mode set
 * Input          : u8 cpol :cpol 0/1
 *                  u8 cpha : cpha 0/1
 *                  u8 order: 0/1
 *                  u8 bits :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSPIMode(u8 cpol, u8 cpha, u8 order, u8 bits)
{
	lcd_spi_cfg.CPOL = cpol;
	lcd_spi_cfg.CPHA = cpha;
	lcd_spi_cfg.ORDER = order;
	lcd_spi_cfg.BITS = bits;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdSPIInit
 * Description    : lcd spi initial GPIO
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSPIInit(void)
{
	ax32xx_gpioMapSet(LCD_SPI_CS_CH, LCD_SPI_CS_PIN, GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(LCD_SPI_CS_CH, LCD_SPI_CS_PIN, GPIO_PULL_UP);
	ax32xx_gpioDirSet(LCD_SPI_CS_CH, LCD_SPI_CS_PIN, GPIO_OUTPUT);
	LCD_SPI_CS_H;

	ax32xx_gpioMapSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_PULL_UP);
	ax32xx_gpioDirSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_OUTPUT);
	if (lcd_spi_cfg.CPOL == 0)
		ax32xx_gpioDataSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_LOW);
	else
		ax32xx_gpioDataSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_HIGH);
}

void ax32xx_spiLcdnit(void)
{
	ax32xx_gpioMapSet(LCD_SPI_PANEL_CS_CH,LCD_SPI_PANEL_CS_PIN,GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(LCD_SPI_PANEL_CS_CH,LCD_SPI_PANEL_CS_PIN,GPIO_PULL_UP);
	ax32xx_gpioDirSet(LCD_SPI_PANEL_CS_CH,LCD_SPI_PANEL_CS_PIN,GPIO_OUTPUT);
	ax32xx_gpioDataSet(LCD_SPI_PANEL_CS_CH,LCD_SPI_PANEL_CS_PIN,GPIO_HIGH);
#if SOFTSPI_DRIVRE_LCD
	ax32xx_gpioMapSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_PULL_DOWN);
	ax32xx_gpioDirSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_OUTPUT);
	if(lcd_spi_cfg.CPOL == 0)
		ax32xx_gpioDataSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_LOW);
	else
		ax32xx_gpioDataSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_HIGH);

#else
    ax32xx_gpioMapSet(LCD_SPI_PANEL_SCK_CH,LCD_SPI_PANEL_SCK_PIN|LCD_SPI_PANEL_MOSI_PIN,GPIO_FUNC_SFR);
    ax32xx_gpioMapSet(LCD_SPI_PANEL_RS_CH,LCD_SPI_PANEL_RS_PIN,GPIO_FUNC_SFR);
    ax32xx_gpioMapSet(LCD_SPI_PANEL_CS_CH,LCD_SPI_PANEL_CS_PIN,GPIO_FUNC_SFR);

#endif

}

/*******************************************************************************
 * Function Name  : ax32xx_lcdSPIUninit
 * Description    : lcd spi uninitial release GPIO
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSPIUninit(void)
{
	ax32xx_gpioMapSet(LCD_SPI_CLK_CH, LCD_SPI_CLK_PIN | LCD_SPI_DAT_PIN, GPIO_FUNC_SFR);

	LCD_SPI_CS_H;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdSPISendData
 * Description    : lcd spi send data
 * Input          : u32 data : data
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSPISendData(u32 data)
{
	u32 mask = (lcd_spi_cfg.ORDER == 0) ? (1 << (lcd_spi_cfg.BITS - 1)) : 1;
	u8 i = lcd_spi_cfg.BITS;

	LCD_SPI_CS_L;

	while (i)
	{
		if (lcd_spi_cfg.CPHA == 0) // data out check-0
		{
			if (data & mask)
				LCD_SPI_DAT_H;
			else
				LCD_SPI_DAT_L;
		}

		if (lcd_spi_cfg.CPOL == 0) // clk
			LCD_SPI_CLK_H;
		else
			LCD_SPI_CLK_L;

		LCD_SPI_DELAY; // delay

		if (lcd_spi_cfg.CPHA == 1) // data out check -1
		{
			if (data & mask)
				LCD_SPI_DAT_H;
			else
				LCD_SPI_DAT_L;
		}

		if (lcd_spi_cfg.CPOL == 0) // clk
			LCD_SPI_CLK_L;
		else
			LCD_SPI_CLK_H;

		if (lcd_spi_cfg.ORDER == 0) // data shift
			data <<= 1;
		else
			data >>= 1;

		LCD_SPI_DELAY; // delay

		i--;
	}

	LCD_SPI_CS_H;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdMcuSendCmd
 * Description    : lcd i80 send command
 * Input          : u32 cmd : cmdcommand
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdMcuSendCmd(u32 cmd)
{
	LCD_MCU_RS_L;
	ax32xx_sysCpuNopDelay(2); // 00);
	R_LCD_CMMAND = cmd;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;
	ax32xx_sysCpuNopDelay(2); // 00);
	LCD_MCU_RS_H;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdMcuSendData
 * Description    : lcd i80 send data
 * Input          : u32 data : data
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdMcuSendData(u32 data)
{
	LCD_MCU_RS_H;
	ax32xx_sysCpuNopDelay(2); // 00);
	R_LCD_CMMAND = data;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;
	ax32xx_sysCpuNopDelay(2); // 00);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdMcuSendCmd
 * Description    : lcd i80 send command
 * Input          : u32 cmd : cmdcommand
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdMcuSendCmd16(u32 cmd)
{
	LCD_MCU_RS_L;
	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = cmd >> 8;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;

	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = cmd;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;

	LCD_MCU_RS_H;
	// ax32xx_sysCpuNopDelay(200);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdMcuSendData
 * Description    : lcd i80 send data
 * Input          : u32 data : data
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdMcuSendData16(u32 data)
{
	LCD_MCU_RS_H;
	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = data >> 8;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;

	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = data;
	while ((R_LCD_STATUS & (1 << 31)) == 0)
		;
	LCD_MCU_RS_L;
	// ax32xx_sysCpuNopDelay(200);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdInit
 * Description    : lcd initial
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdInit(void)
{
	ax32xx_sysClkSet(SYS_CLK_LCDC, 0);
	//    PCON0 |= (1<<25);
	osal_task_sleep(20);
	PCON0 &= ~(1 << 25);
	osal_task_sleep(50);
	R_LCD_CTRL = 0;

	R_LCD_STATUS = (0 << 31) | // timing controller ready flag
				   (1 << 23) | // fifo underflow int en
				   (0 << 20) | // te interrupt en(mcu)
				   (0 << 19) | // data end int en(mcu)
				   (0 << 18) | // line end int en(rgb)
				   (0 << 17) | // frame end int en(rgb)
				   (0 << 16) | // frame start int en(rgb)
				   (1 << 15) | // clear fifo underflow pending
				   (1 << 12) | // clear te pending
				   (1 << 11) | // clear data end pending
				   (1 << 10) | // clear line end pending
				   (1 << 9) |  // clear frame end pending
				   (1 << 8) |  // clear frame start pending
				   (0 << 7) |  // fifo underflow pending
				   (0 << 4) |  // te pending
				   (0 << 3) |  // data end pending (mcu mode)
				   (0 << 2) |  // line end pending
				   (0 << 1) |  // frame end pending (rgb mode)
				   (0 << 0);   // frame start pending
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdIRQEnable
 * Description    : lcd IRQ enable,TE_IRQ table
 * Input          : u8 itype : irq ttype
 *                  u8 en    : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdIRQEnable(u8 itype, u8 en)
{
	if (en)
		R_LCD_STATUS |= 1 << itype;
	else
		R_LCD_STATUS &= ~(1 << itype);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdLineSet
 * Description    : lcd line width set
 * Input          : u32 line : pre line
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdLineSet(u32 line)
{
	R_LCD_RLINE = ((line - 1) << 0); // pre line
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdSignalSet
 * Description    : lcd signal set for rgb mode
 * Input          : u8 vsync : vsync inactive
 *                  u8 inv_pclk : pclk invactive 1->inv,0->normarl
 *                  u8 inv_de   : data enable invactive
 *                  u8 inv_hs   : hsync invactive
 *                  u8 inv_vs   : vsync invactive
 *                  u8 en_de    : data en sigable enable
 *                  u8 en_hs    : hsync signal enable
 *                  u8 en_vs    : vcyns signal enable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSignalSet(u8 vsync, u8 inv_pclk, u8 inv_de, u8 inv_hs, u8 inv_vs, u8 en_de, u8 en_hs, u8 en_vs)
{
	R_LCD_SIGNAL = (vsync << 16) |	 // vsync inactive timing
				   (inv_pclk << 7) | // pclk(wr) idle polarity
				   (inv_de << 6) |	 // de(cs) idle polarity
				   (inv_hs << 5) |	 // hsync idle polarity
				   (inv_vs << 4) |	 // vsync idle polarity
				   (en_de << 2) |	 // de(cs) en
				   (en_hs << 1) |	 // hsync en
				   (en_vs << 0);	 // vsync en
	R_LCD_SIGNAL0 = (en_de << 27) |	 // de(cs) output en
					(en_hs << 26) |	 // hsync output en
					(en_vs << 25) |	 // vsync output en
					(1 << 24);		 // pclk(wr) output en
}

void ax32xx_lcdMcuSignalSet(u8 CSsetup, u8 CShold, u8 CShigh)
{
	R_LCD_CSSTATUS = (CSsetup << 0) | (CShold << 8) | (CShigh << 16);
}

/*******************************************************************************
 * Function Name  : ax32xx_lcdBusWidth
 * Description    : lcd bus width set
 * Input          : u32 width :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdBusWidth(u32 width)
{
	R_LCD_SIGNAL0 &= ~0xffffff;
	R_LCD_SIGNAL0 |= ((1 << width) - 1) & 0xffffff; // data output en
	// #if AX32_PLATFORM == FPGA
	// 80pin LCD_D0 => USB1.1_DM
	//	R_LCD_SIGNAL0 &= ~1;
	// #endif
}
/*******************************************************************************
* Function Name  : ax32xx_lcdBusEnable
* Description    : lcd bus enable
* Input          : u32 mask : mask word
					  u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_lcdBusEnable(u32 mask, u8 en)
{
	if (en)
		R_LCD_SIGNAL0 |= mask & 0xffffff;
	else
		R_LCD_SIGNAL0 &= ~mask;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdClkSet
 * Description    : lcd clk set
 * Input          : u32 width :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdClkSet(u32 clk_div)
{
	R_LCD_CLKSEL = (0 << 8) |			  // pclk tuning
				   ((clk_div - 1) << 2) | // pclk div(max 63)
				   (0 << 0);			  // pclk source(MDF)
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdSyncSet
 * Description    : lcd sync signal info
 * Input          : u32 hsfp : hsync front phase
 *                  u32 hsbp : hsync back phase
 *                  u32 vsfp : vsync front phase
 *                  u32 hsbp : vsync back phase
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdSyncSet(u32 hsfp, u32 hsbp, u32 vsfp, u32 vsbp)
{
	R_LCD_VSYNCF = vsfp;
	R_LCD_VSYNCB = vsbp;
	R_LCD_HSYNCF = hsfp;
	R_LCD_HSYNCB = hsbp;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdDataSet
 * Description    : lcd data enable signal info
 * Input          : u32 defp : data front phase
 *                  u32 debp : data back phase
 *                  u32 destart : data start
 *                  u32 deend : data end
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdDataSet(u32 defp, u32 debp, u32 destart, u32 deend)
{
	R_LCD_DEF = defp;
	R_LCD_DEB = debp;
	R_LCD_DESTART = destart;
	R_LCD_DEEND = deend;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdPositionSet
 * Description    : lcd position
 * Input          : u32 x : x
 *                  u32 y : y
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdPositionSet(u32 x, u32 y)
{
	R_LCD_POSX = x;
	R_LCD_POSY = y;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdResolutionSet
 * Description    : lcd reslution
 * Input          : u32 width : width
 *                  u32 height : height
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdResolutionSet(u32 width, u32 height)
{
	R_LCD_WIDTH = width;
	R_LCD_HEIGHT = height;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdWindowSizeSet
 * Description    : lcd window size
 * Input          : u32 width : width
 *                  u32 height : height
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdWindowSizeSet(u32 width, u32 height)
{
	R_LCD_WINWIDTH = width;
	R_LCD_WINHEIGHT = height;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdModeSet
 * Description    : lcd mode,RGB,MCU
 * Input          : u32 mode : mode
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdModeSet(u32 mode)
{
	R_LCD_MODE = mode;
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdClkNumberSet
 * Description    : lcd total pixel set
 * Input          : u32 num : pixels
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdClkNumberSet(u32 num)
{
	R_LCD_NUMBER = (num << 0); // how many pclk per frame(only mcu)
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdEndLineSet
 * Description    : lcd enable line
 * Input          : u32 eline : end line
 *                  u32 aline : active line
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdEndLineSet(u32 eline, u32 aline)
{
	R_LCD_ENDLINE = eline; // line end pending active line

	R_LCD_ACTIVELINE = aline; // blanking signal active line
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdPanelMode
 * Description    : lcd interface type
 * Input          : u8 panel : 0-RGB,1-MCU
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdPanelMode(u8 panel)
{
	#if 0
   // R_LCD_CTRL = (R_LCD_CTRL & ~(1<<4)) | ((!!panel)<<4);
   #else
   if(panel)
   {
		R_LCD_CTRL = (R_LCD_CTRL & ~(3<<3));
		if(1==panel)
			R_LCD_CTRL = (R_LCD_CTRL |(1<<4)) ;
		else//spi mode enable
			R_LCD_CTRL = (R_LCD_CTRL |(1<<4) |(1<<3)) ;
	}else// RGB mode
	{ 
		R_LCD_CTRL = (R_LCD_CTRL & ~(1<<4));
	}		
   
   #endif
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdEnable
 * Description    : lcd enable
 * Input          : u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdEnable(u8 en)
{
	if (en)
	{
		R_LCD_CTRL |= 1;
		//enable_irq(IRQNO_LCDC);
	}
	else
	{
		R_LCD_CTRL &= ~1;
		//disable_irq(IRQNO_LCDC);
	}
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdTeMode
 * Description    : lcd TE MODE
 * Input          : u8 mode : mode
 *                : u8 en : 1-enable,0-disable
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdTeMode(u8 mode, u8 en)
{
	R_LCD_CTRL = (R_LCD_CTRL & ~((1 << 7) | (1 << 6) | (1 << 5))) | ((mode & 1) << 6) | ((en & 1) << 5);
}
/*******************************************************************************
 * Function Name  : ax32xx_lcdKick
 * Description    : lcd start frame show once
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdKick(void)
{
	R_LCD_CTRL |= (1 << 1);
}

/*******************************************************************************
 * Function Name  : ax32xx_lcdMCUIOConfig
 * Description    : lcd io configure for i80
 * Input          : u8 bus_width : bus width
 *                  u8 te_enable : te enable
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 ax32xx_lcdMCUIOConfig(u8 bus_width, u8 te_enable)
{
	u32 pins = 0;

	ax32xx_gpioSFRSet(GPIO_MAP_LCD, LCD_POS);

	if (LCD_POS == LCD_POS9)
	{
		// D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,RS=PA5,CS=PA4,TE=PF9
		ax32xx_gpioDigitalSet(GPIO_PB, GPIO_PIN6, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PB, GPIO_PIN6, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PB, GPIO_PIN6, GPIO_FUNC_SFR);

		pins = GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3 | GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN7;
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN5 | GPIO_PIN4; // cs
		ax32xx_gpioDigitalSet(GPIO_PA, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PA, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PA, pins, GPIO_FUNC_SFR);
		ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN5, GPIO_FUNC_GPIO);

		if (te_enable)
		{
			pins = GPIO_PIN9;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_INPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}

		if (bus_width == 8)
			return 0;
		else if (bus_width == 24)
		{
			pins = GPIO_PIN14 | GPIO_PIN13 | GPIO_PIN12 | GPIO_PIN11 | GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7;
			ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

			pins = GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3 | GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}
		else
			return -1;
	}
	else if (LCD_POS == LCD_POS14)
	{
		// D[7:0]={PD[13:6]},CLK/WR=PF7,RS=PF8,CS=PA4,TE=PF9

		pins = GPIO_PIN13 | GPIO_PIN12 | GPIO_PIN11 | GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);
		// PDDRV = 0xffff;

		pins = GPIO_PIN7; // WR
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN8; // RS
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		// ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_GPIO);
		// PFDRV = 0xffff;

		pins = GPIO_PIN4; // cs
		ax32xx_gpioDigitalSet(GPIO_PA, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PA, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PA, pins, GPIO_FUNC_SFR);

		if (te_enable)
		{
			pins = GPIO_PIN9; // TE
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_INPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}

		if (bus_width == 8)
			return 0;
		else
			return -1;
	}
	else if (LCD_POS == LCD_POS7)
	{
		// lcd[7:0] map to PD[10:5] PB[6:5]
		pins = GPIO_PIN5 | GPIO_PIN6;
		ax32xx_gpioDigitalSet(GPIO_PB, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PB, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PB, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3;
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN4;
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_GPIO);
		ax32xx_gpioPullSet(GPIO_PF, pins, GPIO_PULL_DOWN); // pull down LCD_CS pin to make it stable

		if (te_enable != 0xff) // TE
		{
			pins = GPIO_PIN3;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_INPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}
		else
		{
			pins = GPIO_PIN3;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_GPIO);
			ax32xx_gpioDataSet(GPIO_PF, pins, GPIO_HIGH);
		}
		if (bus_width == 8)
			return 0;
		else
			return -1;
	}
	else if (LCD_POS == LCD_POS12)
	{
		// D[7:0]={PD[12:5]},CLK/WR=PF1,HS/RS=PF2,DE/CS=PF4,VS/TE=PF3
		printf("LCD MAP POS12\n");
		pins = GPIO_PIN12 | GPIO_PIN11 | GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN4; // CLK //HS//DE
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		if (te_enable)
		{
			pins = GPIO_PIN3;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_INPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}

		if (bus_width == 8)
			return 0;
		else
		{
			printf("##not support lcd bus_width##\n");
			return -1;
		}
	}
	else if(LCD_POS == LCD_POS2)
	{
	        //D[0]=PF3,CLK/WR=PF1,HS/RS=PF2,DE/CS=PF4,VS/TE=PD10
		pins = GPIO_PIN3; //MOSI
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet(GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet(GPIO_PF,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN1|GPIO_PIN2|GPIO_PIN4;  //CLK //HS//DE
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
	
	    if(te_enable==1)
        {
            pins = GPIO_PIN10;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet(GPIO_PD,pins,GPIO_INPUT);
            ax32xx_gpioMapSet(GPIO_PD,pins,GPIO_FUNC_SFR);
        }
		return 0;
	}
	else
		return -1;

	return 0;
}


/*******************************************************************************
 * Function Name  : ax32xx_lcdRGBIOConfig
 * Description    : lcd io configure for rgb
 * Input          : u8 bus_width : bus width
 *                  u8 de_enable : data enable
 * Output         : None
 * Return         : None
 *******************************************************************************/
s32 ax32xx_lcdRGBIOConfig(u8 bus_width, u8 de_enable)
{
	u32 pins = 0;

	ax32xx_gpioSFRSet(GPIO_MAP_LCD, LCD_POS);

	if (LCD_POS == LCD_POS9)
	{
        //D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,HS=PF8,VS=PF9,DE=PA4
        //ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
        //ax32xx_gpioDirSet    (GPIO_PB,GPIO_PIN6,GPIO_OUTPUT);
        //ax32xx_gpioMapSet    (GPIO_PB,GPIO_PIN6,GPIO_FUNC_SFR);
        
        pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2/*|GPIO_PIN1|GPIO_PIN0*/;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
		//		VS/TE	 HS/RS	   CLK/WR
        pins = GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(de_enable) //de enable
        {
			//DE/CS
            pins = GPIO_PIN4;
            ax32xx_gpioDigitalSet(GPIO_PA,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PA,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PA,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else if(bus_width == 24)
        {
            pins = GPIO_PIN14|GPIO_PIN13|GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
            
            pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2/*|GPIO_PIN1|GPIO_PIN0*/;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        else
            return -1;
    }
	else if(LCD_POS == LCD_POS14)
    {
        //D[15:0]={PF[6:0],PD[14:6]},CLK=PF7,HS=PF8,VS=PF9,DE=PA4
        
        pins = GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN13;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
		//		VS/TE	 HS/RS	   CLK/WR
        pins = GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(0)//(de_enable) //de enable
        {
			//DE/CS
            pins = GPIO_PIN4;
            ax32xx_gpioDigitalSet(GPIO_PA,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PA,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PA,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else if(bus_width == 16)
        {
            pins = GPIO_PIN14;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
            
            pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        else
            return -1;
    }
	else if (LCD_POS == LCD_POS7)
	{
		// D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,HS=PF8,VS=PF9,DE=PA4
		// ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
		// ax32xx_gpioDirSet    (GPIO_PB,GPIO_PIN6,GPIO_OUTPUT);
		// ax32xx_gpioMapSet    (GPIO_PB,GPIO_PIN6,GPIO_FUNC_SFR);
		pins = GPIO_PIN5 | GPIO_PIN6;
		ax32xx_gpioDigitalSet(GPIO_PB, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PB, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PB, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3;
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		if (de_enable) // de enable
		{
			pins = GPIO_PIN4;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}

		if (bus_width == 8)
			return 0;
		else if (bus_width == 24)
		{
			pins = GPIO_PIN14 | GPIO_PIN13 | GPIO_PIN12 | GPIO_PIN11 | GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7;
			ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

			pins = GPIO_PIN6 | GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3 | GPIO_PIN2 | GPIO_PIN1 | GPIO_PIN0;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}
		else
			return -1;
	}
	if (LCD_POS == LCD_POS12)
	{
		// D[7:0]={PD[12:5]},CLK/WR=PF1,HS/RS=PF2,DE/CS=PF4,VS/TE=PF3

		pins = GPIO_PIN12 | GPIO_PIN11 | GPIO_PIN10 | GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6 | GPIO_PIN5;
		ax32xx_gpioDigitalSet(GPIO_PD, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PD, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PD, pins, GPIO_FUNC_SFR);

		pins = GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3; // clk//hs//vs
		ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
		ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
		ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);

		if (de_enable) // de enable
		{
			pins = GPIO_PIN4;
			ax32xx_gpioDigitalSet(GPIO_PF, pins, GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet(GPIO_PF, pins, GPIO_OUTPUT);
			ax32xx_gpioMapSet(GPIO_PF, pins, GPIO_FUNC_SFR);
		}

		if (bus_width == 8)
			return 0;
		else
			return -1;
	}
	else
		return -1;

	return 0;
}






void ax32xx_lcdSpiBaudRateSet(u32 baudrate)
{
    LCDSPIBAUD = baudrate&0xff; // SPI_CLK = lcdc_tclk/((spibaud +1)*2)
}

void ax32xx_lcdSpiShiftDir(u32 shift_dir)// 0:send data from high bit,1:send data from low bit
{
    LCDSPICON = (LCDSPICON&(~(1<<12)))|(shift_dir<<12);
}

void ax32xx_lcdSpiSendCmdBits(u8 bits) // send cmd bit = CMD_BIT +1
{
    LCDSPICON = (LCDSPICON&(~(0x1f<<7)))|(bits<<7);
}

void ax32xx_lcdSpiSendDatBits(u8 bits) // send data bit = DATA_BIT +1
{
    LCDSPICON = (LCDSPICON&(~(0x1f<<2)))|(bits<<2);
}

void ax32xx_lcdSpiSampleEdge(bool edge) // 0: sample at rising edge   1:sample at falling edge
{
    LCDSPICON =(LCDSPICON&(~(1<<1))|(edge<<1));
}

void ax32xx_lcdSpiSetClkIdleState(bool idle_sta)
{
    LCDSPICON =(LCDSPICON&(~(1<<0))|(idle_sta<<0));
}




////////////////////////////////////////////////////////////////////////////////
typedef int (*isr_func)(unsigned int, void *, void *);

isr_func lcdcirq_vector_table[8];
void *lcdcirq_dev_table[8];

////////////////////////////////////////////////////////////////////////////////
int lcdc_isr(unsigned int irqno, void *d, void *regs);
void lcdcirq_register(uint32_t irq, isr_func isr, void *dev_id)
{
	printf("%s enter:%d\n", __func__, irq);
	lcdcirq_vector_table[irq] = isr;
	lcdcirq_dev_table[irq] = dev_id;

	irq_register(IRQNO_LCDC, 0, lcdc_isr, NULL);

	LCDSTA |= BIT(irq + 16);
	// printf ("\xA1\xBE%s\xA1\xBF%s %d %X\r\n", pcTaskGetName(NULL), __FUNCTION__, irq, isr);
}
void lcdcIrqEn(int en)
{
	if(en)
		irq_register(IRQNO_LCDC, 0, lcdc_isr, NULL);
	else
		irq_unregister(IRQNO_LCDC);
}
void lcdcirq_register_frame_start(isr_func isr, void *dev_id)
{
	lcdcirq_register(IRQNO_LCDC_FRAME_START, isr, dev_id);
}
void lcdcirq_register_frame_end(isr_func isr, void *dev_id)
{
	lcdcirq_register(IRQNO_LCDC_FRAME_END, isr, dev_id);
}
void lcdcirq_register_te(isr_func isr, void *dev_id)
{
	lcdcirq_register(IRQNO_LCDC_TE, isr, dev_id);
}

void lcdcirq_register_data_end(isr_func isr, void *dev_id)
{
	lcdcirq_register(IRQNO_LCDC_DATA_END, isr, dev_id);
}
void lcdcIntClear(void)
{
	//LCDSTA |= 0xFF00;
}
////////////////////////////////////////////////////////////////////////////////
int lcdc_isr(unsigned int irqno, void *d, void *regs)
{
	static u32 lastTime;
	//static u32 lastTime1;
	static u32 cnt;
	int i;
	uint32_t pend = LCDSTA;
//	if(os_time_get()-lastTime1>10000)
//	{
//		lastTime1=os_time_get();
//		printf("pend:%x\n",pend);
//	}
	if(pend & (1<<7))
 	{
 		cnt++;
 		if(os_time_get()-lastTime>3000)
		{
			lastTime=os_time_get();
			printf("lcd underflow:%d,pend:%x\n",cnt,pend);
			cnt=0;
		}
 	}
	LCDSTA |= 0xFF00;

	for (i = 0; i < 8; ++i)
	{
		if (pend & BIT(i) && lcdcirq_vector_table[i])
		{
			lcdcirq_vector_table[i](i, lcdcirq_dev_table[i], regs);
		}
	}
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
void lcdc_te_mode(uint32_t mode, int en)
{
	LCDCTRL = (LCDCTRL & ~(BIT(7) | BIT(6) | BIT(5))) | ((mode & 3) << 6) | ((en & 1) << 5);
}

/*******************************************************************************
 * Function Name  : ax32xx_deSet_DE_CCM
 * Description    : set de_ccm
 * Input          :  u32 *p_ccm:pointer of ccm matrix
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_deSet_DE_CCM(u32 *p_ccm)
{
	if (p_ccm[0] == 0)
		return;
	DE_CCMC1_CFG = ((p_ccm[2] & 0x3ff) << 20) | ((p_ccm[1] & 0x3ff) << 10) | (p_ccm[0] & 0x3ff);
	DE_CCMC2_CFG = ((p_ccm[5] & 0x3ff) << 20) | ((p_ccm[4] & 0x3ff) << 10) | (p_ccm[3] & 0x3ff);
	DE_CCMC3_CFG = ((p_ccm[8] & 0x3ff) << 20) | ((p_ccm[7] & 0x3ff) << 10) | (p_ccm[6] & 0x3ff);
	DE_CCMOFFSET_CFG = ((p_ccm[11] & 0x1f) << 10) | ((p_ccm[10] & 0x1f) << 5) | (p_ccm[9] & 0x1f);

	DE_DISPLAY_CTRL |= 1 << 20; // ccm enable
}

/*******************************************************************************
 * Function Name  : ax32xx_deSet_DE_SAJ
 * Description    : set de_saj
 * Input          :  u32 *p_saj:pointer of saj
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_deSet_DE_SAJ(u32 *p_saj)
{
	if (p_saj[4] == 0)
		return;
	DE_SATCOEF1_CFG = (p_saj[4]);
	DE_SATCOEF0_CFG = (p_saj[3] << 24) | (p_saj[2] << 16) | (p_saj[1] << 8) | (p_saj[0]);

	DE_DISPLAY_CTRL |= 1 << 19; // sat enable
}

void ax32xx_lcdPreLineSet(u32 line)
{
	R_LCD_RLINE = ((line - 1) << 0); // pre line
}

/*******************************************************************************
 * Function Name  : ax32xx_lcdDESignalSet
 * Description    : lcd data enable signal info
 * Input          : u32 defp : data front phase
 *                  u32 debp : data back phase
 *                  u32 destart : data start
 *                  u32 deend : data end
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdDESignalSet(u32 defp, u32 debp, u32 destart, u32 deend)
{
	R_LCD_DEF = defp;
	R_LCD_DEB = debp;
	R_LCD_DESTART = destart;
	R_LCD_DEEND = deend;
}

/*******************************************************************************
 * Function Name  : ax32xx_lcdDataModeSet
 * Description    : lcd color coding,fifo mode,shfit
 * Input          : u32 mode : mode
 *                  u32 mode : mode1
 *                  u8 odd_order : RGB order,line 1,3,5...
 *                  u8 even_order: RGB order,line 0,2,4...
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ax32xx_lcdDataModeSet(u32 mode, u32 mode1, u8 even_order, u8 odd_order)
{
	R_LCD_MODE = mode;
	R_LCD_MODE1 = mode1;
	R_LCD_RGBORD = ((odd_order != even_order) << 6) | // odd_even mode en
				   (odd_order << 3) |				  // odd line rgb order
				   (even_order << 0);				  // even line rgb order
}
