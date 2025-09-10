#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_R61509V

#define  STRENGTH  5

#if STRENGTH == 0
#define HBIC_TAB0    0xff043904
#define HBIC_TAB1    0xfc0c3800
#define HBIC_TAB2    0xfa1536fb
#define HBIC_TAB3    0xf91f2ff9
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf92f1ff9
#define HBIC_TAB6    0xfb3615fa
#define HBIC_TAB7    0x00380cfc
#define WBIC_TAB0    0x3308fd01,0x0101fd08
#define WBIC_TAB1    0x3302ff01,0x0001fb0f
#define WBIC_TAB2    0x2ffe0000,0x0002fa17
#define WBIC_TAB3    0x2bfb0000,0x0002f91f
#define WBIC_TAB4    0x26f90100,0x0001f926
#define WBIC_TAB5    0x1ff90200,0x0000fb2b
#define WBIC_TAB6    0x17fa0200,0x0000fe2f
#define WBIC_TAB7    0x0ffb0100,0x01ff0233
#elif STRENGTH == 1
#define HBIC_TAB0    0xfc093209
#define HBIC_TAB1    0xfb103203
#define HBIC_TAB2    0xf91830ff
#define HBIC_TAB3    0xf9202bfc
#define HBIC_TAB4    0xfa2527fa
#define HBIC_TAB5    0xfc2b20f9
#define HBIC_TAB6    0xff3018f9
#define HBIC_TAB7    0x033210fb
#define WBIC_TAB0    0x2e0cfb02,0x0002fb0c
#define WBIC_TAB1    0x3006fc01,0x0001fa12
#define WBIC_TAB2    0x2c01fe01,0x0001fa19
#define WBIC_TAB3    0x29feff01,0x0000fa1f
#define WBIC_TAB4    0x25fc0000,0x0000fc23
#define WBIC_TAB5    0x1ffa0000,0x01fffe29
#define WBIC_TAB6    0x19fa0100,0x01fe012c
#define WBIC_TAB7    0x12fa0100,0x01fc0630
#elif STRENGTH == 2
#define HBIC_TAB0    0xfb0c2d0c
#define HBIC_TAB1    0xfa122d07
#define HBIC_TAB2    0xfa182b03
#define HBIC_TAB3    0xfb1e2700
#define HBIC_TAB4    0xfd2224fd
#define HBIC_TAB5    0x00271efb
#define HBIC_TAB6    0x032b18fa
#define HBIC_TAB7    0x072d12fa
#define WBIC_TAB0    0x2c0ffa01,0x0001fa0f
#define WBIC_TAB1    0x2c0afb01,0x0000fa14
#define WBIC_TAB2    0x2805fc01,0x0100fb1a
#define WBIC_TAB3    0x2501fd01,0x01fffd1f
#define WBIC_TAB4    0x24fffe01,0x01feff20
#define WBIC_TAB5    0x1ffdff01,0x01fd0125
#define WBIC_TAB6    0x1afb0001,0x01fc0528
#define WBIC_TAB7    0x14fa0000,0x01fb0a2c
#elif STRENGTH == 3
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14250b
#define HBIC_TAB2    0xfd182407
#define HBIC_TAB3    0xff1c2203
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03221cff
#define HBIC_TAB6    0x072418fd
#define HBIC_TAB7    0x0b2514fc
#define WBIC_TAB0    0x2312fc00,0x0100fc12
#define WBIC_TAB1    0x250dfb00,0x01fffd16
#define WBIC_TAB2    0x2509fb00,0x01fefe1a
#define WBIC_TAB3    0x2206fb01,0x01fd001e
#define WBIC_TAB4    0x2202fc01,0x01fc0220
#define WBIC_TAB5    0x1e00fd01,0x01fb0622
#define WBIC_TAB6    0x1afefe01,0x00fb0925
#define WBIC_TAB7    0x16fdff01,0x00fb0d25
#elif STRENGTH == 4
#define HBIC_TAB0    0xfd112111
#define HBIC_TAB1    0xff14200d
#define HBIC_TAB2    0x00171f0a
#define HBIC_TAB3    0x021a1d07
#define HBIC_TAB4    0x041c1c04
#define HBIC_TAB5    0x071d1a02
#define HBIC_TAB6    0x0a1f1700
#define HBIC_TAB7    0x0d2014ff
#define WBIC_TAB0    0x1f14fffd,0x01fdff14
#define WBIC_TAB1    0x2010fdfe,0x01fd0017
#define WBIC_TAB2    0x210dfcff,0x00fc011a
#define WBIC_TAB3    0x1d0afc00,0x00fc041d
#define WBIC_TAB4    0x1f07fc00,0x00fc071b
#define WBIC_TAB5    0x1d04fc00,0x00fc0a1d
#define WBIC_TAB6    0x1a01fc00,0xfffc0d21
#define WBIC_TAB7    0x1700fd01,0xfefd1020
#elif STRENGTH == 5
#define HBIC_TAB0    0x00121c12
#define HBIC_TAB1    0x02131c0f
#define HBIC_TAB2    0x03151c0c
#define HBIC_TAB3    0x05171a0a
#define HBIC_TAB4    0x07191907
#define HBIC_TAB5    0x0a1a1705
#define HBIC_TAB6    0x0c1c1503
#define HBIC_TAB7    0x0f1c1302
#define WBIC_TAB0    0x1a1402fd,0x00fd0214
#define WBIC_TAB1    0x1b1200fd,0x00fc0416
#define WBIC_TAB2    0x190f00fd,0xfffd0619
#define WBIC_TAB3    0x190dfffd,0xfefd081b
#define WBIC_TAB4    0x1c0afefe,0xfefe0a18
#define WBIC_TAB5    0x1b08fdfe,0xfdff0d19
#define WBIC_TAB6    0x1906fdff,0xfd000f19
#define WBIC_TAB7    0x1604fc00,0xfd00121b
#elif STRENGTH == 6
#define HBIC_TAB0    0x04121812
#define HBIC_TAB1    0x06131710
#define HBIC_TAB2    0x0714170e
#define HBIC_TAB3    0x0915160c
#define HBIC_TAB4    0x0a16160a
#define HBIC_TAB5    0x0c161509
#define HBIC_TAB6    0x0e171407
#define HBIC_TAB7    0x10171306
#define WBIC_TAB0    0x161206fe,0xfefe0612
#define WBIC_TAB1    0x151104fe,0xfeff0714
#define WBIC_TAB2    0x140f03fe,0xfe000915
#define WBIC_TAB3    0x160e02fd,0xfd000a16
#define WBIC_TAB4    0x170c00fd,0xfd000c17
#define WBIC_TAB5    0x160a00fd,0xfd020e16
#define WBIC_TAB6    0x150900fe,0xfe030f14
#define WBIC_TAB7    0x1407fffe,0xfe041115
#elif STRENGTH == 7
#define HBIC_TAB0    0x08111611
#define HBIC_TAB1    0x09121510
#define HBIC_TAB2    0x0a12150f
#define HBIC_TAB3    0x0b13140e
#define HBIC_TAB4    0x0c14140c
#define HBIC_TAB5    0x0e14130b
#define HBIC_TAB6    0x0f15120a
#define HBIC_TAB7    0x10151209
#define WBIC_TAB0    0x110f0801,0xff01080f
#define WBIC_TAB1    0x110e0700,0xff020910
#define WBIC_TAB2    0x110e0600,0xff020a10
#define WBIC_TAB3    0x100d0500,0xff030b11
#define WBIC_TAB4    0x110c0400,0x00040c0f
#define WBIC_TAB5    0x110b03ff,0x00050d10
#define WBIC_TAB6    0x100a02ff,0x00060e11
#define WBIC_TAB7    0x100902ff,0x00070e11
#endif

#define CMD(x)     {MCU_CMD,x}
#define DAT(x)   {MCU_DAT,x}

BEGIN_INIT_TAB
      CMD(0x00),CMD(0x00),CMD(0x00),CMD(0x00),
      {DELAY_MS,100},

      
      CMD(0x04),CMD(0x00),DAT(0x6a),DAT(0x00),
      CMD(0x00),CMD(0x08),DAT(0x08),DAT(0x08),

      {DELAY_MS,200},
      
    
  
      CMD(0x03),CMD(0x00),DAT(0x05),DAT(0x04),
      CMD(0x03),CMD(0x01),DAT(0x16),DAT(0x1c),
      CMD(0x03),CMD(0x02),DAT(0x05),DAT(0x01),
      CMD(0x03),CMD(0x03),DAT(0x11),DAT(0x04),
      CMD(0x03),CMD(0x04),DAT(0x11),DAT(0x30),
      CMD(0x03),CMD(0x05),DAT(0x04),DAT(0x11),
      CMD(0x03),CMD(0x06),DAT(0x11),DAT(0x05),
      CMD(0x03),CMD(0x07),DAT(0x1c),DAT(0x06),
      CMD(0x03),CMD(0x08),DAT(0x04),DAT(0x05),
      CMD(0x03),CMD(0x09),DAT(0x13),DAT(0x30),


      {DELAY_MS,20},
       
      CMD(0x00),CMD(0x10),DAT(0x00),DAT(0x17),
      {DELAY_MS,15},
      CMD(0x01),CMD(0x00),DAT(0x03),DAT(0x30),
      CMD(0x01),CMD(0x01),DAT(0x02),DAT(0x37), // 2,37
      CMD(0x01),CMD(0x02),DAT(0xf9),DAT(0xB0),
      CMD(0x01),CMD(0x03),DAT(0x0c),DAT(0x00),
      {DELAY_MS,15},
      CMD(0x00),CMD(0x11),DAT(0x01),DAT(0x01),
      CMD(0x00),CMD(0x12),DAT(0x00),DAT(0x00),
      CMD(0x00),CMD(0x13),DAT(0x00),DAT(0x01),

      {DELAY_MS,120},
      CMD(0x02),CMD(0x80),DAT(0xc2),DAT(0x00),
      

      {DELAY_MS,15},

      CMD(0x00),CMD(0x01),DAT(0x00),DAT(0x00),
      CMD(0x00),CMD(0x02),DAT(0x01),DAT(0x00),
      CMD(0x00),CMD(0x03),DAT(0x10),DAT(0xA0),
      CMD(0x00),CMD(0x09),DAT(0x00),DAT(0x01),
      CMD(0x00),CMD(0x0C),DAT(0x00),DAT(0x00),
      CMD(0x00),CMD(0x90),DAT(0x80),DAT(0x08),
      CMD(0x00),CMD(0x0F),DAT(0x00),DAT(0x00),
      {DELAY_MS,20},
      CMD(0x02),CMD(0x10),DAT(0x00),DAT(0x00),
      CMD(0x02),CMD(0x11),DAT(0x00),DAT(0xef),
      CMD(0x02),CMD(0x12),DAT(0x00),DAT(0x00),
      CMD(0x02),CMD(0x13),DAT(0x01),DAT(0x8f),
      {DELAY_MS,20},
      CMD(0x05),CMD(0x00),DAT(0x00),DAT(0x00),
      CMD(0x05),CMD(0x01),DAT(0x00),DAT(0x00),
      CMD(0x05),CMD(0x02),DAT(0x00),DAT(0x5f),
      {DELAY_MS,20},
      CMD(0x04),CMD(0x01),DAT(0x00),DAT(0x01),
      CMD(0x04),CMD(0x04),DAT(0x00),DAT(0x00),
      {DELAY_MS,15},
      CMD(0x00),CMD(0x07),DAT(0x01),DAT(0x00),
      {DELAY_MS,100},

      CMD(0x02),CMD(0x02),
END_INIT_TAB

LCD_OP_SECTION lcddev_t r61509v = 
{
    .name = "R61509V",
    .lcd_bus_type = LCD_BUS_I80,
    .bus_width = LCD_BUS_WIDTH_8,
    .bus_cmd = 0,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_270,
    .te_mode =  1,//te mode, 0xff:disable
    //f(wr) = source_clk/div/2
    //f(wr) >= screen_w * screen_h * clk_per_pixel * 60
    .pclk_div = PLL_CLK/(240*400*2*60)/2,
    .even_order = 0,
    .odd_order = 0,
    .lcd_data_mode = (0<<31)|//data inversion mode
                     (2<<24)|//data compress mode
                     (1<<20)|//fifo mode
                     (0<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (0<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (0<<5)| //output cycle 0 shift direction
                     (8<<0), //output cycle 0 shift bit
    .screen_w = 240,// 400,
    .screen_h = 400,// 240,
    .video_x  = 0,
    .video_y  = 0,
    .video_w  = 400,
    .video_h  = 240,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 0, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

    .clk_per_pixel = 2,
    
    .brightness = -12,
    .saturation = 7,
    .contrast   = 7,
    .contra_index = 8,
    
    .gamma_red = 3,
    .gamma_green=3,
    .gamma_blue=3,

    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
};

#endif







