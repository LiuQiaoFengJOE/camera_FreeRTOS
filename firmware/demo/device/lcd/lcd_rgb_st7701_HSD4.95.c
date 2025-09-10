#include "tft_lcd.h"


#define CMD(x)	 {SPI_DAT,x}
#define DAT(x)   {SPI_DAT,((x)|(1<<8))}
#define DELAY(x) {DELAY_MS,(x)}

#define  STRENGTH  3

#if STRENGTH == 0
#define HBIC_TAB0    0x01fe43fe
#define HBIC_TAB1    0x01f84601
#define HBIC_TAB2    0x00f04c04
#define HBIC_TAB3    0x00eb5005
#define HBIC_TAB4    0x031d1d03
#define HBIC_TAB5    0x0550eb00
#define HBIC_TAB6    0x044cf000
#define HBIC_TAB7    0x0146f801
#define WBIC_TAB0    0x42fe0100,0x000001fe
#define WBIC_TAB1    0x46010000,0x000001f8
#define WBIC_TAB2    0x4c040000,0x000000f0
#define WBIC_TAB3    0x5206fe00,0x01ff00ea
#define WBIC_TAB4    0x2203fb01,0x01fb0320
#define WBIC_TAB5    0xea00ff01,0x00fe0652
#define WBIC_TAB6    0xf0000000,0x0000044c
#define WBIC_TAB7    0xf8010000,0x00000146
#elif STRENGTH == 1
#define HBIC_TAB0    0x01fa4bfa
#define HBIC_TAB1    0x00f54dfe
#define HBIC_TAB2    0x00f34b02
#define HBIC_TAB3    0x01fc3f04
#define HBIC_TAB4    0x041c1c04
#define HBIC_TAB5    0x043ffc01
#define HBIC_TAB6    0x024bf300
#define HBIC_TAB7    0xfe4df500
#define WBIC_TAB0    0x4afa0100,0x000001fa
#define WBIC_TAB1    0x4cfe0100,0x000000f5
#define WBIC_TAB2    0x49020101,0x000000f3
#define WBIC_TAB3    0x3f050000,0x00ff01fc
#define WBIC_TAB4    0x1d040000,0x0000041b
#define WBIC_TAB5    0xfc01ff00,0x0000053f
#define WBIC_TAB6    0xf3000000,0x01010249
#define WBIC_TAB7    0xf5000000,0x0001fe4c
#elif STRENGTH == 2
#define HBIC_TAB0    0x00f556f5
#define HBIC_TAB1    0x00f453f9
#define HBIC_TAB2    0x02f947fe
#define HBIC_TAB3    0x03063601
#define HBIC_TAB4    0x031d1d03
#define HBIC_TAB5    0x01360603
#define HBIC_TAB6    0xfe47f902
#define HBIC_TAB7    0xf953f400
#define WBIC_TAB0    0x56f50000,0x000000f5
#define WBIC_TAB1    0x53f90000,0x000000f4
#define WBIC_TAB2    0x47fe0100,0x000001f9
#define WBIC_TAB3    0x34010100,0x01000306
#define WBIC_TAB4    0x1b030101,0x0101031b
#define WBIC_TAB5    0x06030001,0x00010134
#define WBIC_TAB6    0xf9010000,0x0001fe47
#define WBIC_TAB7    0xf4000000,0x0000f953
#elif STRENGTH == 3
#define HBIC_TAB0    0x01f457f4
#define HBIC_TAB1    0x02f851f5
#define HBIC_TAB2    0x030044f9
#define HBIC_TAB3    0x020e33fd
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0xfd330e02
#define HBIC_TAB6    0xf9440003
#define HBIC_TAB7    0xf551f802
#define WBIC_TAB0    0x55f40100,0x010001f4
#define WBIC_TAB1    0x50f50000,0x000102f8
#define WBIC_TAB2    0x43f90000,0x00010300
#define WBIC_TAB3    0x32fd0000,0x0001020e
#define WBIC_TAB4    0x20000100,0x0001001e
#define WBIC_TAB5    0x0e020100,0x0000fd32
#define WBIC_TAB6    0x00030100,0x0000f943
#define WBIC_TAB7    0xf8020100,0x0000f550
#elif STRENGTH == 4
#define HBIC_TAB0    0x02f94cf9
#define HBIC_TAB1    0x020048f6
#define HBIC_TAB2    0x010940f6
#define HBIC_TAB3    0xff1632f9
#define HBIC_TAB4    0xfc2325fc
#define HBIC_TAB5    0xf93216ff
#define HBIC_TAB6    0xf6400901
#define HBIC_TAB7    0xf6480002
#define WBIC_TAB0    0x48f90201,0x000102f9
#define WBIC_TAB1    0x44f60201,0x00010200
#define WBIC_TAB2    0x3ef60101,0x00000109
#define WBIC_TAB3    0x32f90000,0x0000ff16
#define WBIC_TAB4    0x25fc0000,0x0000fc23
#define WBIC_TAB5    0x16ff0000,0x0000f932
#define WBIC_TAB6    0x09010000,0x0101f63e
#define WBIC_TAB7    0x00020100,0x0102f644
#elif STRENGTH == 5
#define HBIC_TAB0    0x00004000
#define HBIC_TAB1    0xff083efb
#define HBIC_TAB2    0xfc123af8
#define HBIC_TAB3    0xfa1e31f7
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf7311efa
#define HBIC_TAB6    0xf83a12fc
#define HBIC_TAB7    0xfb3e08ff
#define WBIC_TAB0    0x40000000,0x00000000
#define WBIC_TAB1    0x3cfc0100,0x0000ff08
#define WBIC_TAB2    0x36f90200,0x0100fd11
#define WBIC_TAB3    0x2df80201,0x0101fa1c
#define WBIC_TAB4    0x26f90201,0x0102f922
#define WBIC_TAB5    0x1cfa0101,0x0102f82d
#define WBIC_TAB6    0x11fd0001,0x0002f936
#define WBIC_TAB7    0x08ff0000,0x0001fc3c
#elif STRENGTH == 6
#define HBIC_TAB0    0xfc093209
#define HBIC_TAB1    0xfb103203
#define HBIC_TAB2    0xf91830ff
#define HBIC_TAB3    0xf9202bfc
#define HBIC_TAB4    0xfa2527fa
#define HBIC_TAB5    0xfc2b20f9
#define HBIC_TAB6    0xff3018f9
#define HBIC_TAB7    0x033210fb
#define WBIC_TAB0    0x3409fc01,0x0001fc09
#define WBIC_TAB1    0x3103fe01,0x0002fb10
#define WBIC_TAB2    0x2fffff01,0x0002f917
#define WBIC_TAB3    0x2bfc0000,0x0001f91f
#define WBIC_TAB4    0x26fa0100,0x0001fa24
#define WBIC_TAB5    0x1ff90100,0x0000fc2b
#define WBIC_TAB6    0x17f90200,0x01ffff2f
#define WBIC_TAB7    0x10fb0200,0x01fe0331
#elif STRENGTH == 7
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14250b
#define HBIC_TAB2    0xfd182407
#define HBIC_TAB3    0xff1c2203
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03221cff
#define HBIC_TAB6    0x072418fd
#define HBIC_TAB7    0x0b2514fc
#define WBIC_TAB0    0x2910fb00,0x0100fb10
#define WBIC_TAB1    0x270cfb01,0x0100fb15
#define WBIC_TAB2    0x2707fb01,0x01fffc1a
#define WBIC_TAB3    0x2403fc01,0x01fefe1f
#define WBIC_TAB4    0x2300fd01,0x01fd0021
#define WBIC_TAB5    0x1ffefe01,0x01fc0324
#define WBIC_TAB6    0x1afcff01,0x01fb0727
#define WBIC_TAB7    0x15fb0001,0x01fb0c27
#endif

#if DEV_LCD  == DEV_LCD_ST7701HSD495

BEGIN_INIT_TAB

CMD(0x11),
DELAY(120),
CMD(0xFF),
DAT(0x77),
DAT(0x01),
DAT(0x00),
DAT(0x00),
DAT(0x10),
CMD(0xC0),
DAT(0xE9),
DAT(0x03),
CMD(0xC1),
DAT(0x11),
DAT(0x02),
CMD(0xC2),
DAT(0x37),
DAT(0x08),
CMD(0xCC),
DAT(0x10),
CMD(0xB0),
DAT(0x00),
DAT(0x0D),
DAT(0x14),
DAT(0x0D),
DAT(0x10),
DAT(0x05),
DAT(0x02),
DAT(0x08),
DAT(0x08),
DAT(0x1E),
DAT(0x05),
DAT(0x13),
DAT(0x11),
DAT(0xA3),
DAT(0x29),
DAT(0x18),
CMD(0xB1),
DAT(0x00),
DAT(0x0C),
DAT(0x14),
DAT(0x0C),
DAT(0x10),
DAT(0x05),
DAT(0x03),
DAT(0x08),
DAT(0x07),
DAT(0x20),
DAT(0x05),
DAT(0x13),
DAT(0x11),
DAT(0xA4),
DAT(0x29),
DAT(0x18),
CMD(0xFF),
DAT(0x77),
DAT(0x01),
DAT(0x00),
DAT(0x00),
DAT(0x11),
CMD(0xB0),
DAT(0x6C),
CMD(0xB1),
DAT(0x33),//43
CMD(0xB2),
DAT(0x07),
CMD(0xB3),
DAT(0x80),
CMD(0xB5),
DAT(0x47),
CMD(0xB7),
DAT(0x85),
CMD(0xB8),
DAT(0x20),
CMD(0xB9),
DAT(0x10),
CMD(0xC1),
DAT(0x78),
CMD(0xC2),
DAT(0x78),
CMD(0xD0),
DAT(0x88),

DELAY(100),
CMD(0xE0),
DAT(0x00),
DAT(0x00),
DAT(0x02),
CMD(0xE1),
DAT(0x08),
DAT(0x00),
DAT(0x0A),
DAT(0x00),
DAT(0x07),
DAT(0x00),
DAT(0x09),
DAT(0x00),
DAT(0x00),
DAT(0x33),
DAT(0x33),
CMD(0xE2),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0x00),
CMD(0xE3),
DAT(0x00),
DAT(0x00),
DAT(0x33),
DAT(0x33),
CMD(0xE4),
DAT(0x44),
DAT(0x44),
CMD(0xE5),
DAT(0x0E),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x10),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x0A),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x0C),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
CMD(0xE6),
DAT(0x00),
DAT(0x00),
DAT(0x33),
DAT(0x33),
CMD(0xE7),
DAT(0x44),
DAT(0x44),
CMD(0xE8),
DAT(0x0D),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x0F),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x09),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
DAT(0x0B),
DAT(0x60),
DAT(0xA0),
DAT(0xA0),
CMD(0xEB),
DAT(0x02),
DAT(0x01),
DAT(0xE4),
DAT(0xE4),
DAT(0x44),
DAT(0x00),
DAT(0x40),
CMD(0xEC),
DAT(0x02),
DAT(0x01),
CMD(0xED),
DAT(0xAB),
DAT(0x89),
DAT(0x76),
DAT(0x54),
DAT(0x01),
DAT(0xFF),
DAT(0xFF),
DAT(0xFF),
DAT(0xFF),
DAT(0xFF),
DAT(0xFF),
DAT(0x10),
DAT(0x45),
DAT(0x67),
DAT(0x98),
DAT(0xBA),
CMD(0xFF),
DAT(0x77),
DAT(0x01),
DAT(0x00),
DAT(0x00),
DAT(0x00),
CMD(0x36),
DAT(0x00),
  
CMD(0x29),
DELAY(50), 

END_INIT_TAB



LCD_OP_SECTION lcddev_t st7701_hsd495 = {
	.name = "st7701-hsd495",
	.lcd_bus_type = 0,
	.bus_width = 24,
	.red_width = 8,
	.green_width = 8,
	.blue_width = 8,
	.scan_mode = LCD_ROTATE_270,
	.pclk_div = PLL_CLK/24000000,//pclk div
    .even_order = 0,
    .odd_order = 0,
	.lcd_data_mode = (0<<31)|//data inversion mode
				     (0<<24)|//data compress mode
				     (0<<20)|//fifo mode
				     (0<<17)|//output cycle 2 shift direction
				     (0<<12)|//output cycle 2 shift bit
				     (0<<11)|//output cycle 1 shift direction
				     (0<<6)| //output cycle 1 shift bit
				     (0<<5)| //output cycle 0 shift direction
				     (0<<0), //output cycle 0 shift bit
	.screen_w = 480,
	.screen_h = 854,
	.video_x  = 0,
	.video_y  = 0,
	.video_w  = 854,
    .video_h  = 480,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 0, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

	.clk_per_pixel = 1,
	.pclk_inv = 1,
	
    .vlw = 1,
	.vbp = 10,
	.vfp = 20,
	.hlw = 1,
	.hbp = 3,
	.hfp = 40,

	.de_inv = 0,
	.hs_inv = 1,
	.vs_inv = 1,
	
	.spi_cpol  = 0,
	.spi_cpha  = 0,
	.spi_order = 0,
	.spi_bits  = 9,
    
	.brightness = 4,
	.saturation = 10,
	.contrast   = 10,
    .contra_index = 10,
	
	.gamma_red = 4,
	.gamma_green=4,
	.gamma_blue=4,
	
	.de_ccm = {0x100,0x0,0x0,
           0x0,0x100,0x0,
           0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
	
	INIT_TAB_INIT
};
#endif

