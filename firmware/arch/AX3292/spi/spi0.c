#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"

#include "hal/hal_spi.h"
#include "gpio_ops/gpio.h"
#include "sys/sys.h"

extern UINT32 APB_CLK; // defined by board

#define SPI_CLK_EN() PCON0 &= ~(1 << 20) /*spi0 clk en*/
#define SPI_2W_PIN_CFG()                   \
	do                                     \
	{                                      \
		PMAPCON0 &= ~(3 << 12);            \
		PMAPCON0 |= (1 << 12);             \
		PAFMAP |= (1 << 13) | (1 << 12);   \
		PADIR &= ~((1 << 13) | (1 << 12)); \
	} while (0)
#define SPI_3W_PIN_CFG()                             \
	do                                               \
	{                                                \
		PMAPCON0 &= ~(3 << 12);                      \
		PMAPCON0 |= (1 << 12);                       \
		PAFMAP |= (1 << 13) | (1 << 12) | (1 << 10); \
		PADIR &= ~((1 << 13) | (1 << 12));           \
		PADIR |= (1 << 10);                          \
	} while (0)

// #define _SPICON SPI0CON
// #define _SPIBAUD SPI0BAUD
// #define _SPIBUF SPI0BUF
// #define _SPIDMAADR SPI0DMAADR
// #define _SPIDMACNT SPI0DMACNT

enum spi0_group_e
{
	SPI0_POS_NONE_ = 0,
	SPI0_1_LINE,  // clk/DATX
	SPI0_2_LINE0, // clk/DAT0/DAT1
	SPI0_2_LINE1, // clk/DAT_IN/DAT_OUT
	SPI0_4_LINE,  // clk/DAT0/DAT1/DAT2/DAT3
};

static void spi0_pin_config(int bus_mode)
{
	ax32xx_gpioSFRSet(GPIO_MAP_SPI0, bus_mode);

	// cs/clk
	ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN11, GPIO_FUNC_SFR);
	ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN10, GPIO_FUNC_GPIO);
	ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN11 | GPIO_PIN10, GPIO_OUTPUT);
	ax32xx_gpioDataSet(GPIO_PA, GPIO_PIN10, GPIO_HIGH);

	if (bus_mode == SPI0_1_LINE) // DATX
	{
		ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN8, GPIO_FUNC_SFR);
		ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN8, GPIO_OUTPUT);
	}
	else if (bus_mode == SPI0_2_LINE0) // clk/DAT0/DAT1
	{
		ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8, GPIO_FUNC_SFR);
		ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8, GPIO_INPUT);
		ax32xx_gpioPullSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8, GPIO_PULL_UP);
	}
	else if (bus_mode == SPI0_2_LINE1) // clk/DAT_IN/DAT_OUT
	{
		ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8, GPIO_FUNC_SFR);
		ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN8, GPIO_OUTPUT);
		ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN9, GPIO_INPUT);
		ax32xx_gpioPullSet(GPIO_PA, GPIO_PIN9, GPIO_PULL_UP);
	}
	else if (bus_mode == SPI0_4_LINE) // clk/DAT0/DAT1/DAT2/DAT3
	{
		ax32xx_gpioMapSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6, GPIO_FUNC_SFR);
		ax32xx_gpioDirSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6, GPIO_INPUT);
		ax32xx_gpioPullSet(GPIO_PA, GPIO_PIN9 | GPIO_PIN8 | GPIO_PIN7 | GPIO_PIN6, GPIO_PULL_UP);
	}
}

static u32 ax32xx_spi0ClkConfig(u8 clk_src, u8 div)
{
	u32 spif_clk = 0;

	CLKCON2 &= ~(0xf << 28);	 // clear spi_tclk source setting
	CLKCON2 |= (clk_src << 30) | // spi_tclk source = master clock
			   (div << 28);
	PCON1 &= ~(1 << 3);

	if (clk_src == 0)
	{
		spif_clk = APB_CLK / (div + 1);
	}

	return spif_clk;
}

static void delay_5ms(u16 n)
{

	volatile u32 i = 5000 * n;
	while (i--)
	{
		// asm("nop");
	}
}

void spi0_init(uint32_t u32SPIBaud, int bus_mode)
{
	u32 spif_clk = ax32xx_spi0ClkConfig(0, 3);
	// printf("%s enter\n",__func__);
	// delay_5ms(200*500);
	// printf("%s delay finish\n",__func__);
	ax32xx_sysClkSet(SYS_CLK_SPI0, 1);
	// printf("bus_mode:%d \n",bus_mode);
	spi0_pin_config(bus_mode);

	SPIFGCON = (1 << 24) |					 // cs min high time
			   (1 << 20) |					 // cs min hold time
			   (1 << 16) |					 // cs min setup time
			   (0 << 15) |					 // spi_csp mode select
			   (1 << 14) |					 // spi_csf mode select
			   (0 << 13) |					 // spi_csp en(for psram)
			   (1 << 12) |					 // spi_csf en(for flash)
			   (0 << 10) |					 // cs special mode en(auto mode)
			   (((bus_mode - 1) & 3) << 8) | // data bus width(manual mode)
			   (0 << 7) |					 // transfer dir(manual mode)
			   (0 << 5) |					 // LFSR enc/dec en
			   (0 << 4) |					 // crc16 enable
			   (0 << 3) |					 // sample edge,0:raise,1:fall
			   (1 << 2) |					 // spi_clk idle polarity
			   (0 << 1) |					 // auto mode en
			   (1 << 0);					 // spif en

	SPIFFACON = (0 << 20) | // dummy bytes count(auto mode)
				(0 << 18) | // addr bytes count(auto mode)
				(0 << 14) | // mode bus width(auto mode)
				(0 << 12) | // cmd bus width(auto mode)
				(0 << 10) | // addr bus width(auto mode)
				(0 << 8) |	// data bus width(auto mode)
				(0 << 6) |	// dummy phase enable(auto mode)
				(0 << 5) |	// mode phase enable(auto mode)
				(0 << 4) |	// cmd phase enable(auto mode)
				(0 << 2) |	// dma device en
				(0 << 1) |	// cpu data en
				(0 << 0);	// cpu instruction en

	SPIFPACON = (0 << 20) | // dummy bytes count(auto mode)
				(0 << 18) | // addr bytes count(auto mode)
				(0 << 14) | // mode bus width(auto mode)
				(0 << 12) | // cmd bus width(auto mode)
				(0 << 10) | // addr bus width(auto mode)
				(0 << 8) |	// data bus width(auto mode)
				(0 << 6) |	// dummy phase enable(auto mode)
				(0 << 5) |	// mode phase enable(auto mode)
				(0 << 4);	// cmd phase enable(auto mode)

	SPIFDCON = (0 << 24) |					 // package size
			   (0 << 20) |					 // dummy bytes count(dma mode)
			   (0 << 18) |					 // addr bytes count(dma mode)
			   (0 << 14) |					 // mode bus width(dma mode)
			   (0 << 12) |					 // cmd bus width(dma mode)
			   (0 << 10) |					 // addr bus width(dma mode)
			   (((bus_mode - 1) & 3) << 8) | // data bus width(dma mode)
			   (0 << 7) |					 // transfer dir(dma mode)
			   (0 << 6) |					 // dummy phase enable(dma mode)
			   (0 << 5) |					 // mode phase enable(dma mode)
			   (0 << 4) |					 // cmd phase enable(dma mode)
			   (0 << 3) |					 // addr phase enable(dma mode)
			   (0 << 2) |					 // data phase en
			   (0 << 1) |					 // auto package en
			   (0 << 0);					 // dma mode en

	SPIFACT = (1 << 25) | // clear flag
			  (1 << 24) | // clear flag
			  (1 << 23) | // clear flag
			  (1 << 22) | // clear flag
			  (1 << 21) | // clear flag
			  (1 << 20) | // clear flag
			  (1 << 19) | // clear flag
			  (1 << 18) | // clear flag
			  (1 << 17) | // clear flag
			  (1 << 16) | // clear flag
			  (0 << 7) |  // dam device function lock(psram)
			  (0 << 6) |  // cpu data function lock(psram)
			  (0 << 5) |  // cpu instruction function lock(psram)
			  (0 << 4) |  // dam device function lock(flash)
			  (0 << 3) |  // cpu data function lock(flash)
			  (0 << 2) |  // cpu instruction function lock(flash)
			  (0 << 1) |  // dma mode kick start
			  (0 << 0);	  // buffer mode kick start

	SPIFSTA = (0 << 31) | // dma device hold flag(pasram)
			  (0 << 30) | // cpu data hold flag(pasram)
			  (0 << 29) | // cpu instruction hold flag(pasram)
			  (0 << 28) | // dma device hold flag(flash)
			  (0 << 27) | // cpu data hold flag(flash)
			  (0 << 26) | // cpu instruction hold flag(flash)
			  (0 << 25) | // dma device wr err int en(flash)
			  (0 << 24) | // cpu data wr err int en(flash)
			  (0 << 23) | // dma device int en(psram)
			  (0 << 22) | // cpu data int en(psram)
			  (0 << 21) | // cpu intstruction int en(psram)
			  (0 << 20) | // dma device int en(flash)
			  (0 << 19) | // cpu data int en(flash)
			  (0 << 18) | // cpu intstruction int en(flash)
			  (0 << 17) | // dma mode int en
			  (0 << 16) | // manual mode int en
			  (0 << 10) | // crc idle flag
			  (0 << 9) |  // dma device wr err flag(flash)
			  (0 << 8) |  // cpu data wr err flag(flash)
			  (0 << 7) |  // dma device err flag(psram)
			  (0 << 6) |  // cpu data err flag(psram)
			  (0 << 5) |  // cpu intstruction err flag(psram)
			  (0 << 4) |  // dma device err flag(flash)
			  (0 << 3) |  // cpu data err flag(flash)
			  (0 << 2) |  // cpu intstruction err flag(flash)
			  (0 << 1) |  // rx/tx done for dma mode
			  (0 << 0);	  // rx/tx done for manual mode

	SPIFBAUD = spif_clk / u32SPIBaud - 1;
	SPIFDBUF = (0 << 0);   //
	SPIFDCNT = (0 << 0);   //
	SPIFBADR = (0 << 2);   // sdram base address (dma device)
	SPIFSADR = (0 << 0);   // spi address (dma device)
	SPIFCRC = 0xffff;	   // crc16 value
	SPIFPOLY = 0xffffffff; // poly
	SPIFLFSR = 0;		   // LFSR
	SPIFFAMSK = 0xfffffff; // address mask(flash,auto mode)
	SPIFPAMSK = 0xfffffff; // address mask(psram,auto mode)

	SPIFFACMD = (0 << 16) | // mode value(flash,auto mode)
				(0 << 8) |	// write cmd(flash,auto mode)
				(0 << 0);	// read cmd(flash,auto mode)

	SPIFPACMD = (0 << 16) | // mode value(psram,auto mode)
				(0 << 8) |	// write cmd(psram,auto mode)
				(0 << 0);	// read cmd(psram,auto mode)

	SPIFDCMD = (0 << 16) | // mode value(dma device,auto mode)
			   (0 << 8) |  // write cmd(dma device,auto mode)
			   (0 << 0);   // read cmd(dma device,auto mode)
}

void spi0_enable(bool enable)
{
	if (enable)
		SPIFGCON |= BIT(0);
	else
		SPIFGCON &= ~BIT(0);
}

void spi0_send_byte(uint8_t u8Data)
{
	// printf("sb:%x\n",u8Data);
	SPIFGCON &= ~BIT(7);
	SPIFDBUF = u8Data;
	SPIFACT = 1;
	while ((SPIFSTA & 1) == 0)
		;
}

uint8_t spi0_receive_byte(void)
{
	// printf("rb");
	SPIFGCON |= BIT(7);
	SPIFDBUF = 0xff;
	SPIFACT = 1;
	while ((SPIFSTA & 1) == 0)
		;
	return SPIFDBUF;
}

void spi0_recevie_dma(void *pDataBuf, uint32_t u32DataLen)
{

	dcache_flush_region(pDataBuf, u32DataLen);
	if (u32DataLen < 8)
		return;
	SPIFGCON |= (1 << 1);
	SPIFDCON |= (0x0 << 24) | // package size
				(0 << 20) |	  // dummy bytes count(dma mode)
				(0 << 18) |	  // addr bytes count(dma mode)
				(0 << 14) |	  // mode bus width(dma mode)
				(0 << 12) |	  // cmd bus width(dma mode)
				(0 << 10) |	  // addr bus width(dma mode)
				(0 << 8) |	  // data bus width(dma mode)
				(1 << 7) |	  // transfer dir(dma mode)
				(0 << 6) |	  // dummy phase enable(dma mode)
				(0 << 5) |	  // mode phase enable(dma mode)
				(0 << 4) |	  // cmd phase enable(dma mode)
				(0 << 3) |	  // addr phase enable(dma mode)
				(1 << 2) |	  // data phase en
				(0 << 1) |	  // auto package en
				(1 << 0);	  // dma mode en

	SPIFDCNT = u32DataLen / 4 - 1;
	SPIFBADR = (u32)pDataBuf;
	SPIFSADR = 0x6000000 | 0;

	SPIFACT = BIT(17); // dmapnd clear
	SPIFACT = BIT(1);  // dma mode kick start
	while ((SPIFSTA & BIT(1)) == 0)
		;

	SPIFGCON &= ~(1 << 1);
	SPIFDCON &= ~((3 << 18) | (0xff << 0));

	// Uart_PrintfBuf("rd:\n",pDataBuf,u32DataLen);
}

void spi0_send_dma(void *pDataBuf, uint32_t u32DataLen)
{
	// printf("sd");
	dcache_writeback_region(pDataBuf, u32DataLen);
	if (u32DataLen < 8)
		return;
	SPIFGCON |= (1 << 1);
	SPIFDCON |= (0 << 24) | // package size
				(0 << 20) | // dummy bytes count(dma mode)
				(0 << 18) | // addr bytes count(dma mode)
				(0 << 14) | // mode bus width(dma mode)
				(0 << 12) | // cmd bus width(dma mode)
				(0 << 10) | // addr bus width(dma mode)
				(0 << 8) |	// data bus width(dma mode)
				(0 << 7) |	// transfer dir(dma mode)
				(0 << 6) |	// dummy phase enable(dma mode)
				(0 << 5) |	// mode phase enable(dma mode)
				(0 << 4) |	// cmd phase enable(dma mode)
				(0 << 3) |	// addr phase enable(dma mode)
				(1 << 2) |	// data phase en
				(0 << 1) |	// auto package en
				(1 << 0);	// dma mode en

	SPIFDCNT = u32DataLen / 4 - 1;
	SPIFBADR = (u32)pDataBuf;
	SPIFSADR = 0x6000000 | 0;

	SPIFACT = BIT(17); // dmapnd clear
	SPIFACT = BIT(1);  // dma mode kick start
	while ((SPIFSTA & BIT(1)) == 0)
		;

	SPIFGCON &= ~(1 << 1);
	SPIFDCON &= ~((3 << 18) | (0xff << 0));
}

static inline void spi0_cs_high(void)
{
	REG32(PORTA) |= BIT(10);
}

static inline void spi0_cs_low(void)
{
	REG32(PORTA) &= ~BIT(10);
	// printf("PORT:%x   MAP:%x  DIR:%x\n",REG32(PORTA),REG32(PAFMAP),REG32(PADIR));
	// REG32(PFFMAP) &= ~BIT(2);
	// REG32(PFDIR)  &= ~BIT(2);
	// REG32(PORTF) |= BIT(2);
}

void spi0_chip_sel(int level)
{
	if (level)
		spi0_cs_high();
	else
		spi0_cs_low();
}

struct spi_device spi0_device = {
	.init = spi0_init,
	.send_byte = spi0_send_byte,
	.recv_byte = spi0_receive_byte,
	.send_chunk = spi0_send_dma,
	.recv_chunk = spi0_recevie_dma,
	.chip_sel = spi0_chip_sel};
