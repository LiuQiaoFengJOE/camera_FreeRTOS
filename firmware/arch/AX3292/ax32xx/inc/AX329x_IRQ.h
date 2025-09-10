#ifndef AX329X_IRQ_H
#define AX329X_IRQ_H
typedef enum
{
	IRQ_TIMER0 = 0, // 0
	IRQ_TIMER1,		// 1
	IRQ_TIMER2,		// 2
	IRQ_TIMER3,		// 3
	IRQ_LCDC,		// 4
	IRQ_RTC_WDT,	// 5
	IRQ_DAC,		// 6
	IRQ_GPIO,		// 7
	IRQ_UART0,		// 8
	IRQ_SPI0,		// 9
	IRQ_SDC0,		// 10
	IRQ_SDC1,		// 11
	IRQ_SPI1,		// 12
	IRQ_LVD,		// 13
	IRQ_CSI,		// 14
	IRQ_USB20,		// 15
	IRQ_JPGA,		// 16
	IRQ_IIC0_IIC1,	// 17
	IRQ_AUADC,		// 18
	IRQ_MCP0,		// 19
	IRQ_BTUART,		// 20
	IRQ_DE,			// 21
	IRQ_JPGB,		// 22
	IRQ_ROTATE,		// 23
	IRQ_MCP1,		// 24
	IRQ_MCP2,		// 25
	IRQ_EMI,		// 26
	IRQ_OSDCMP,		// 27
	IRQ_PMU,		// 28
	IRQ_USB11,		// 29
	IRQ_USB11MCP,	// 30
	IRQ_TT_MIPI,	// 31

	IRQ_MAX
} IRQ_IDX_E;
#endif