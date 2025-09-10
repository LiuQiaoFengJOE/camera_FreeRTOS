#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_ILI9328

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
#define WBIC_TAB0    0x2a10fa01,0x0001fa10
#define WBIC_TAB1    0x280bfb01,0x0100fb15
#define WBIC_TAB2    0x2706fb01,0x0100fc1a
#define WBIC_TAB3    0x2502fc01,0x01fffd1f
#define WBIC_TAB4    0x2300fe01,0x01fe001f
#define WBIC_TAB5    0x1ffdff01,0x01fc0225
#define WBIC_TAB6    0x1afc0001,0x01fb0627
#define WBIC_TAB7    0x15fb0001,0x01fb0b28
#elif STRENGTH == 1
#define HBIC_TAB0    0xfc093209
#define HBIC_TAB1    0xfb103203
#define HBIC_TAB2    0xf91830ff
#define HBIC_TAB3    0xf9202bfc
#define HBIC_TAB4    0xfa2527fa
#define HBIC_TAB5    0xfc2b20f9
#define HBIC_TAB6    0xff3018f9
#define HBIC_TAB7    0x033210fb
#define WBIC_TAB0    0x2312fc00,0x0100fc12
#define WBIC_TAB1    0x250dfb00,0x01fffd16
#define WBIC_TAB2    0x2509fb00,0x01fefe1a
#define WBIC_TAB3    0x2206fb01,0x01fd001e
#define WBIC_TAB4    0x2202fc01,0x01fc0220
#define WBIC_TAB5    0x1e00fd01,0x01fb0622
#define WBIC_TAB6    0x1afefe01,0x00fb0925
#define WBIC_TAB7    0x16fdff01,0x00fb0d25
#elif STRENGTH == 2
#define HBIC_TAB0    0xfb0c2d0c
#define HBIC_TAB1    0xfa122d07
#define HBIC_TAB2    0xfa182b03
#define HBIC_TAB3    0xfb1e2700
#define HBIC_TAB4    0xfd2224fd
#define HBIC_TAB5    0x00271efb
#define HBIC_TAB6    0x032b18fa
#define HBIC_TAB7    0x072d12fa
#define WBIC_TAB0    0x2113fefe,0x01fefe13
#define WBIC_TAB1    0x2010fdff,0x01fdff17
#define WBIC_TAB2    0x220cfcff,0x01fc001a
#define WBIC_TAB3    0x1f09fc00,0x00fc031d
#define WBIC_TAB4    0x2006fc00,0x00fc061c
#define WBIC_TAB5    0x1d03fc00,0x00fc091f
#define WBIC_TAB6    0x1a00fc01,0xfffc0c22
#define WBIC_TAB7    0x17fffd01,0xfffd1020
#elif STRENGTH == 3
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14250b
#define HBIC_TAB2    0xfd182407
#define HBIC_TAB3    0xff1c2203
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03221cff
#define HBIC_TAB6    0x072418fd
#define HBIC_TAB7    0x0b2514fc
#define WBIC_TAB0    0x1e1400fd,0x00fd0014
#define WBIC_TAB1    0x1e11fffd,0x00fc0217
#define WBIC_TAB2    0x1d0ffefd,0x00fc0419
#define WBIC_TAB3    0x1c0cfdfe,0xfffc061c
#define WBIC_TAB4    0x1e09fdff,0xfffd0918
#define WBIC_TAB5    0x1c06fcff,0xfefd0c1c
#define WBIC_TAB6    0x1904fc00,0xfdfe0f1d
#define WBIC_TAB7    0x1702fc00,0xfdff111e
#elif STRENGTH == 4
#define HBIC_TAB0    0xfd112111
#define HBIC_TAB1    0xff14200d
#define HBIC_TAB2    0x00171f0a
#define HBIC_TAB3    0x021a1d07
#define HBIC_TAB4    0x041c1c04
#define HBIC_TAB5    0x071d1a02
#define HBIC_TAB6    0x0a1f1700
#define HBIC_TAB7    0x0d2014ff
#define WBIC_TAB0    0x1b1303fd,0xfffd0313
#define WBIC_TAB1    0x191202fd,0xfffd0515
#define WBIC_TAB2    0x191000fd,0xfefe0717
#define WBIC_TAB3    0x180d00fd,0xfefe0919
#define WBIC_TAB4    0x1a0bfffd,0xfdff0b18
#define WBIC_TAB5    0x1909fefe,0xfd000d18
#define WBIC_TAB6    0x1707fefe,0xfd001019
#define WBIC_TAB7    0x1505fdff,0xfd021219
#elif STRENGTH == 5
#define HBIC_TAB0    0x00121c12
#define HBIC_TAB1    0x02131c0f
#define HBIC_TAB2    0x03151c0c
#define HBIC_TAB3    0x05171a0a
#define HBIC_TAB4    0x07191907
#define HBIC_TAB5    0x0a1a1705
#define HBIC_TAB6    0x0c1c1503
#define HBIC_TAB7    0x0f1c1302
#define WBIC_TAB0    0x141206ff,0xfeff0612
#define WBIC_TAB1    0x131005ff,0xfe000813
#define WBIC_TAB2    0x140f04fe,0xfe000914
#define WBIC_TAB3    0x140e02fe,0xfe000b15
#define WBIC_TAB4    0x160c01fe,0xfe010c14
#define WBIC_TAB5    0x150b00fe,0xfe020e14
#define WBIC_TAB6    0x140900fe,0xfe040f14
#define WBIC_TAB7    0x130800fe,0xff051013
#elif STRENGTH == 6
#define HBIC_TAB0    0x04121812
#define HBIC_TAB1    0x06131710
#define HBIC_TAB2    0x0714170e
#define HBIC_TAB3    0x0915160c
#define HBIC_TAB4    0x0a16160a
#define HBIC_TAB5    0x0c161509
#define HBIC_TAB6    0x0e171407
#define HBIC_TAB7    0x10171306
#define WBIC_TAB0    0x110f0801,0xff01080f
#define WBIC_TAB1    0x110e0700,0xff020910
#define WBIC_TAB2    0x110e0600,0xff020a10
#define WBIC_TAB3    0x100d0500,0xff030b11
#define WBIC_TAB4    0x110c0400,0x00040c0f
#define WBIC_TAB5    0x110b03ff,0x00050d10
#define WBIC_TAB6    0x100a02ff,0x00060e11
#define WBIC_TAB7    0x100902ff,0x00070e11
#elif STRENGTH == 7
#define HBIC_TAB0    0x08111611
#define HBIC_TAB1    0x09121510
#define HBIC_TAB2    0x0a12150f
#define HBIC_TAB3    0x0b13140e
#define HBIC_TAB4    0x0c14140c
#define HBIC_TAB5    0x0e14130b
#define HBIC_TAB6    0x0f15120a
#define HBIC_TAB7    0x10151209
#define WBIC_TAB0    0x0c0d0904,0x0004090d
#define WBIC_TAB1    0x0f0c0803,0x0004090d
#define WBIC_TAB2    0x0d0c0703,0x01050a0d
#define WBIC_TAB3    0x0e0b0702,0x01060a0d
#define WBIC_TAB4    0x0e0b0602,0x02060b0c
#define WBIC_TAB5    0x0d0a0601,0x02070b0e
#define WBIC_TAB6    0x0d0a0501,0x03070c0d
#define WBIC_TAB7    0x0d090400,0x03080c0f
#endif

#define CMD(x)    {MCU_CMD,x}
#define DAT(x)    {MCU_DAT,x}
#define DLAY(x)    {DELAY_MS,x}

BEGIN_INIT_TAB
    CMD(0x00E3),DAT( 0x3008),
    CMD(0x00E7),DAT( 0x0012),
    CMD(0x00EF),DAT( 0x1231),

    CMD(0x0001),DAT( 0x0100),// set SS and SM bit (S720 -> S1)
    CMD(0x0002),DAT( 0x0700),// set 1 line inversion  
    CMD(0x0003),DAT( 0x1028), // set GRAM write direction and BGR=1. 65K
    CMD(0x0004),DAT( 0x0000), // Resize register
    CMD(0x0008),DAT( 0x0207),// set the back porch and front porch
    CMD(0x0009),DAT( 0x0000),// set non-display area refresh cycle ISC[3:0]
    CMD(0x000A),DAT( 0x0000), // FMARK function
    CMD(0x000C),DAT( 0x0000), // RGB interface setting
    CMD(0x000D),DAT( 0x0000), // Frame marker Position
    CMD(0x000F),DAT( 0x0000),// RGB interface polarity
                                                                 //Power On sequence 
    CMD(0x0010),DAT( 0x0000), // SAP),DAT( BT[3:0]),DAT( AP),DAT( DSTB),DAT( SLP),DAT( STB
    CMD(0x0011),DAT( 0x0007), // DC1[2:0]),DAT( DC0[2:0]),DAT( VC[2:0]
    CMD(0x0012),DAT( 0x0000), // VREG1OUT voltage
    CMD(0x0013),DAT( 0x0000), // VDV[4:0] for VCOM amplitude
    //CMD(0x0007),DAT( 0x0001,
    DLAY(50),              // Dis-charge capacitor power voltage 
    CMD(0x0010),DAT( 0x1290), // SAPF1, BT[3:0]F1, APF1, DSTBF1, SLPF1, STB
    CMD(0x0011),DAT( 0x0227), // DC1[2:0]F1, DC0[2:0]F1, VC[2:0]
    DLAY(50),              
    CMD(0x0012),DAT( 0x0019), //001C// Internal reference voltage= Vci;
    DLAY(50),             
    CMD(0x0013),DAT( 0x0900), //Set VDV[4:0] for VCOM amplitude
    CMD(0x0029),DAT( 0x0000), //Set VCM[5:0] for VCOMH
    CMD(0x002B),DAT( 0x000c), // Set Frame Rate   000C
    DLAY(50),
    CMD(0x0020),DAT( 0x00ef), // GRAM horizontal Address
    CMD(0x0021),DAT( 0x0000), // GRAM Vertical Address
                                                                 // Adjust the Gamma Curve
    CMD(0x0030),DAT( 0x0000),
    CMD(0x0031),DAT( 0x0603),
    CMD(0x0032),DAT( 0x0206),
    CMD(0x0035),DAT( 0x0206),
    CMD(0x0036),DAT( 0x0004), 
    CMD(0x0037),DAT( 0x0105), 
    CMD(0x0038),DAT( 0x0401), 
    CMD(0x0039),DAT( 0x0707), 
    CMD(0x003C),DAT( 0x0602), 
    CMD(0x003D),DAT( 0x0004),

                                                             //Set GRAM area
    CMD(0x0050),DAT( 0x0000), // Horizontal GRAM Start Address
    CMD(0x0051),DAT( 0x00EF),// Horizontal GRAM End Address
    CMD(0x0052),DAT( 0x0000), // Vertical GRAM Start Address
    CMD(0x0053),DAT( 0x013F), // Vertical GRAM Start Address
    CMD(0x0060),DAT( 0xA700), // Gate Scan Line
    CMD(0x0061),DAT( 0x0001),// NDL),DAT(VLE),DAT( REV
    CMD(0x006A),DAT( 0x0000), // set scrolling line

                                                                 //Partial Display Control
    CMD(0x0080),DAT( 0x0000), 
    CMD(0x0081),DAT( 0x0000), 
    CMD(0x0082),DAT( 0x0000),
    CMD(0x0083),DAT( 0x0000), 
    CMD(0x0084),DAT( 0x0000),
    CMD(0x0085),DAT( 0x0000), 
                                                                 //Panel Control
    CMD(0x0090),DAT( 0x0010), 
    CMD(0x0092),DAT( 0x0600),
    CMD(0x0093),DAT( 0x0003), 
    //CMD(0x0095),DAT( 0x0110, 
    // CMD(0x0097),DAT( 0x0000, 
    // CMD(0x0098),DAT( 0x0000, 
    CMD(0x0007),DAT( 0x0133), // 262K color and display ON
    CMD(0x0022),
END_INIT_TAB

LCD_OP_SECTION lcddev_t ili9328 = 
{
    .name = "ili9328",
    .lcd_bus_type = LCD_BUS_I80,
    .bus_width = LCD_BUS_WIDTH_8,
    .bus_cmd = 1,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_0,
    .te_mode =  0xff,//1,//te mode, 0xff:disable
    //f(wr) = source_clk/div/2
    //f(wr) >= screen_w * screen_h * clk_per_pixel * 60
    .pclk_div = PLL_CLK/(240*320*2*60)/2,
    .even_order = 0,
    .odd_order = 0,
    .lcd_data_mode = (DATA_NOT_INVER<<31)|//data inversion mode
                     (RGB565<<24)|//data compress mode
                     (MODE24BIT_S<<20)|//fifo mode
                     (SDIR2_UP<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (SDIR1_UP<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (SDIR0_UP<<5)| //output cycle 0 shift direction
                     (8<<0), //output cycle 0 shift bit
    .screen_w = 320,// 400,
    .screen_h = 240,// 240,
    .video_x  = 0,
    .video_y  = 0,
    .video_w  = 320,
    .video_h  = 240,
    .osd_x = 0,
	.osd_y = 0,
	.osd_w = 0,	// 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
    .clk_per_pixel = 2,
    
    .brightness = 4,
    .saturation = 11,
    .contrast    = 7,
    
    .contra_index = 2,
    .gamma_red = 1,
    .gamma_green=1,
    .gamma_blue=1,

    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
};



#endif


