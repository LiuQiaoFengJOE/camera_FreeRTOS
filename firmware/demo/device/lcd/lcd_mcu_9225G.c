#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_iLi9225G
#define  STRENGTH  0

#if STRENGTH == 0
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14260a
#define HBIC_TAB2    0xfd182506
#define HBIC_TAB3    0xfe1c2303
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03231cfe
#define HBIC_TAB6    0x062518fd
#define HBIC_TAB7    0x0a2614fc
#define WBIC_TAB0    0x1c1402fc,0x00fc0214
#define WBIC_TAB1    0x1c1200fd,0x00fc0316
#define WBIC_TAB2    0x1a0f00fd,0xfffd0519
#define WBIC_TAB3    0x190dfefd,0xfffd081b
#define WBIC_TAB4    0x1c0afefe,0xfefe0a18
#define WBIC_TAB5    0x1b08fdff,0xfdfe0d19
#define WBIC_TAB6    0x1905fdff,0xfd000f1a
#define WBIC_TAB7    0x1603fc00,0xfd00121c
#elif STRENGTH == 1
#define HBIC_TAB0    0xfd102310
#define HBIC_TAB1    0xfe14220c
#define HBIC_TAB2    0xff172109
#define HBIC_TAB3    0x001a2006
#define HBIC_TAB4    0x031d1d03
#define HBIC_TAB5    0x06201a00
#define HBIC_TAB6    0x092117ff
#define HBIC_TAB7    0x0c2214fe
#define WBIC_TAB0    0x191304fd,0xfffd0413
#define WBIC_TAB1    0x191202fd,0xfefd0615
#define WBIC_TAB2    0x181001fd,0xfefe0717
#define WBIC_TAB3    0x160e00fd,0xfeff0919
#define WBIC_TAB4    0x1a0cfffd,0xfdff0c16
#define WBIC_TAB5    0x1909fffe,0xfd000e16
#define WBIC_TAB6    0x1707fefe,0xfd011018
#define WBIC_TAB7    0x1506fdfe,0xfd021219
#elif STRENGTH == 2
#define HBIC_TAB0    0xff111f11
#define HBIC_TAB1    0x00141e0e
#define HBIC_TAB2    0x01161e0b
#define HBIC_TAB3    0x03191c08
#define HBIC_TAB4    0x051b1b05
#define HBIC_TAB5    0x081c1903
#define HBIC_TAB6    0x0b1e1601
#define HBIC_TAB7    0x0e1e1400
#define WBIC_TAB0    0x141206ff,0xfeff0612
#define WBIC_TAB1    0x161104fe,0xfeff0713
#define WBIC_TAB2    0x140f03fe,0xfe000915
#define WBIC_TAB3    0x140e02fe,0xfd000b16
#define WBIC_TAB4    0x170c01fd,0xfd010c15
#define WBIC_TAB5    0x160b00fd,0xfe020e14
#define WBIC_TAB6    0x150900fe,0xfe030f14
#define WBIC_TAB7    0x1307fffe,0xfe041116
#elif STRENGTH == 3
#define HBIC_TAB0    0x01121b12
#define HBIC_TAB1    0x02131c0f
#define HBIC_TAB2    0x04151b0c
#define HBIC_TAB3    0x0617190a
#define HBIC_TAB4    0x08181808
#define HBIC_TAB5    0x0a191706
#define HBIC_TAB6    0x0c1b1504
#define HBIC_TAB7    0x0f1c1302
#define WBIC_TAB0    0x14100700,0xfe000710
#define WBIC_TAB1    0x140f0600,0xfe000811
#define WBIC_TAB2    0x130e05ff,0xfe010a12
#define WBIC_TAB3    0x120d04ff,0xfe020b13
#define WBIC_TAB4    0x140c03ff,0xff030c10
#define WBIC_TAB5    0x130b02fe,0xff040d12
#define WBIC_TAB6    0x120a01fe,0xff050e13
#define WBIC_TAB7    0x110800fe,0x00060f14
#elif STRENGTH == 4
#define HBIC_TAB0    0x04121812
#define HBIC_TAB1    0x05131810
#define HBIC_TAB2    0x0714170e
#define HBIC_TAB3    0x0815170c
#define HBIC_TAB4    0x0a16160a
#define HBIC_TAB5    0x0c171508
#define HBIC_TAB6    0x0e171407
#define HBIC_TAB7    0x10181305
#define WBIC_TAB0    0x0f0f0802,0xff02080f
#define WBIC_TAB1    0x110e0701,0xff02090f
#define WBIC_TAB2    0x100d0600,0x00030a10
#define WBIC_TAB3    0x100c0500,0x00040b10
#define WBIC_TAB4    0x110c0500,0x00050c0d
#define WBIC_TAB5    0x100b0400,0x00050c10
#define WBIC_TAB6    0x100a0300,0x00060d10
#define WBIC_TAB7    0x0f0902ff,0x01070e11
#elif STRENGTH == 5
#define HBIC_TAB0    0x07111711
#define HBIC_TAB1    0x08121610
#define HBIC_TAB2    0x0913160e
#define HBIC_TAB3    0x0a14150d
#define HBIC_TAB4    0x0c14140c
#define HBIC_TAB5    0x0d15140a
#define HBIC_TAB6    0x0e161309
#define HBIC_TAB7    0x10161208
#define WBIC_TAB0    0x0e0d0903,0x0003090d
#define WBIC_TAB1    0x0f0c0803,0x0004090d
#define WBIC_TAB2    0x0f0c0702,0x00050a0d
#define WBIC_TAB3    0x0e0b0702,0x01050a0e
#define WBIC_TAB4    0x0e0b0601,0x01060b0e
#define WBIC_TAB5    0x0e0a0501,0x02070b0e
#define WBIC_TAB6    0x0d0a0500,0x02070c0f
#define WBIC_TAB7    0x0d090400,0x03080c0f
#elif STRENGTH == 6
#define HBIC_TAB0    0x09111511
#define HBIC_TAB1    0x0a121410
#define HBIC_TAB2    0x0b12140f
#define HBIC_TAB3    0x0c12140e
#define HBIC_TAB4    0x0d13130d
#define HBIC_TAB5    0x0e14120c
#define HBIC_TAB6    0x0f14120b
#define HBIC_TAB7    0x1014120a
#define WBIC_TAB0    0x0c0b0905,0x0205090b
#define WBIC_TAB1    0x0d0b0805,0x0205090b
#define WBIC_TAB2    0x0c0b0804,0x0206090c
#define WBIC_TAB3    0x0c0a0704,0x03060a0c
#define WBIC_TAB4    0x0c0a0703,0x03070a0c
#define WBIC_TAB5    0x0c0a0603,0x04070a0c
#define WBIC_TAB6    0x0c090602,0x04080b0c
#define WBIC_TAB7    0x0b090502,0x05080b0d
#elif STRENGTH == 7
#define HBIC_TAB0    0x0c111211
#define HBIC_TAB1    0x0c111310
#define HBIC_TAB2    0x0d11130f
#define HBIC_TAB3    0x0e12110f
#define HBIC_TAB4    0x0e12120e
#define HBIC_TAB5    0x0f11120e
#define HBIC_TAB6    0x0f13110d
#define HBIC_TAB7    0x1013110c
#define WBIC_TAB0    0x0c0a0806,0x0406080a
#define WBIC_TAB1    0x0a0a0806,0x0407090a
#define WBIC_TAB2    0x0a0a0806,0x0407090a
#define WBIC_TAB3    0x0b090805,0x0507090a
#define WBIC_TAB4    0x0a090705,0x0507090c
#define WBIC_TAB5    0x0a090705,0x0508090b
#define WBIC_TAB6    0x0a090704,0x06080a0a
#define WBIC_TAB7    0x0a090704,0x06080a0a
#endif

#define CMD(x)	 {MCU_CMD,x}
#define DAT(x)   {MCU_DAT,x}
#define DLY(n)  {DELAY_MS,n}


BEGIN_INIT_TAB

 
   #if 1

	//************* Start Initial Sequence **********//
	CMD(0x00),CMD(0x01),DAT(0x01),DAT(0x1C), // set SS and NL bit
	CMD(0x00),CMD(0x02),DAT(0x00),DAT(0x00), // set 1 line inversion
	CMD(0x00),CMD(0x03),DAT(0x10),DAT(0x30), // set GRAM write direction and BGR=1.
	CMD(0x00),CMD(0x08),DAT(0x02),DAT(0x02), // set BP and FP
	CMD(0x00),CMD(0x0C),DAT(0x00),DAT(0x00), // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for

	CMD(0x00),CMD(0x0F),DAT(0x0d),DAT(0x01), // Set frame rate
	CMD(0x00),CMD(0x20),DAT(0x00),DAT(0x00), // Set GRAM Address
	CMD(0x00),CMD(0x21),DAT(0x00),DAT(0x00), // Set GRAM Address
	//*************Power On sequence ****************//
	DLY(50),
	CMD(0x00),CMD(0x10),DAT(0x0A),DAT(0x00), // Set SAP,DSTB,STB
	CMD(0x00),CMD(0x11),DAT(0x10),DAT(0x38), // Set APON,PON,AON,VCI1EN,VC
	DLY(50),
	CMD(0x00),CMD(0x12),DAT(0x11),DAT(0x21), // Internal reference voltage= Vci,
	CMD(0x00),CMD(0x13),DAT(0x00),DAT(0x66), // Set GVDD
	CMD(0x00),CMD(0x14),DAT(0x5F),DAT(0x45), // Set VCOMH/VCOML voltage  ¦Ì¨ª¡ã???¦Ì¡Â??¡À¨¨?¨¨ ¦Ì¡ÂD?¡À?¨¢¨¢¦Ì¡Â¡ä¨®¡À?¡ã¦Ì
	//------------------------ Set GRAM area --------------------------------//
	CMD(0x00),CMD(0x30),DAT(0x00),DAT(0x00),
	CMD(0x00),CMD(0x31),DAT(0x00),DAT(0xDB),
	CMD(0x00),CMD(0x32),DAT(0x00),DAT(0x00),
	
	
	CMD(0x00),CMD(0x33),DAT(0x00),DAT(0x0),
	CMD(0x00),CMD(0x34),DAT(0x00),DAT(0xDB),
	
	CMD(0x00),CMD(0x35),DAT(0x00),DAT(0x00),
	CMD(0x00),CMD(0x36),DAT(0x00),DAT(0xAF),

   
	CMD(0x00),CMD(0x37),DAT(0x00),DAT(0x00),
	CMD(0x00),CMD(0x38),DAT(0x00),DAT(0xDB),
	CMD(0x00),CMD(0x39),DAT(0x00),DAT(0x00),
	// ----------- Adjust the Gamma Curve ----------//
	CMD(0x00),CMD(0x50),DAT(0x04),DAT(0x00),
	CMD(0x00),CMD(0x51),DAT(0x06),DAT(0x0B),
	CMD(0x00),CMD(0x52),DAT(0x0C),DAT(0x0A),
	CMD(0x00),CMD(0x53),DAT(0x01),DAT(0x05),
	CMD(0x00),CMD(0x54),DAT(0x0A),DAT(0x0C),
	CMD(0x00),CMD(0x55),DAT(0x0B),DAT(0x06),
	CMD(0x00),CMD(0x56),DAT(0x00),DAT(0x04),
	CMD(0x00),CMD(0x57),DAT(0x05),DAT(0x01),
	CMD(0x00),CMD(0x58),DAT(0x0E),DAT(0x00),
	CMD(0x00),CMD(0x59),DAT(0x00),DAT(0x0E),
	DLY(50),
	CMD(0x00),CMD(0x07),DAT(0x10),DAT(0x17),//BIT[12]:0--TE OFF,1--TE ON
#else
//************* Start Initial Sequence **********//   
//************* Start Initial Sequence **********//        
CMD(0x0001),DAT( 0x011C), // set SS and NL bit        
CMD(0x0002),DAT( 0x0000), // set 1 line inversion        
CMD(0x0003),DAT( 0x1030), // set GRAM write direction and BGR=1.        
CMD(0x0008),DAT( 0x0808), // set BP and FP        
CMD(0x000C),DAT( 0x0000), // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit                
CMD(0x000F),DAT( 0x1fff), // Set frame rate        
CMD(0x0020),DAT( 0x0000), // Set GRAM Address        
CMD(0x0021),DAT( 0x0000), // Set GRAM Address        //*************Power On sequence ****************//        
DLY(50),        
CMD(0x0010),DAT( 0x0A00), // Set SAP),DAT(DSTB),DAT(STB        
CMD(0x0011),DAT( 0x103B), // Set APON),DAT(PON),DAT(AON),DAT(VCI1EN),DAT(VC        
DLY(50),        
CMD(0x0012),DAT( 0x3121), // Internal reference voltage= Vci,        
CMD(0x0013),DAT( 0x006b),//66 // Set GVDD        
CMD(0x0014),DAT( 0x4660), // Set VCOMH/VCOML voltage   4660  5260  //4860 4460//        
CMD(0x0013),DAT(0x007f),//03//        
CMD(0x0014),DAT(0x5158),       
//------------------------ Set GRAM area --------------------------------//        
CMD (0x30),DAT( 0x0000),        
CMD (0x31),DAT( 0x00DB),        
CMD (0x32),DAT( 0x0000),        
CMD (0x33),DAT( 0x0000),        
CMD (0x34),DAT( 0x00DB),        
CMD (0x35),DAT( 0x0000),        
CMD (0x36),DAT( 0x00AF),        
CMD (0x37),DAT( 0x0000),        
CMD (0x38),DAT( 0x00DB),        
CMD (0x39),DAT( 0x0000),        
// ----------- Adjust the Gamma Curve ----------//        
CMD(0x0050),DAT( 0x0400),        
CMD(0x0051),DAT( 0x080B),        
CMD(0x0052),DAT( 0x0E0C),        
CMD(0x0053),DAT( 0x0103),        
CMD(0x0054),DAT( 0x0C0E),        
CMD(0x0055),DAT( 0x0B08),        
CMD(0x0056),DAT( 0x0004),        
CMD(0x0057),DAT( 0x0301),        
CMD(0x0058),DAT( 0x0800),        
CMD(0x0059),DAT( 0x0008),        
DLY(50),
//solve 9201 line        
CMD(0x00FE),DAT(0x0075),        
CMD(0x00FF),DAT(0x00a5),        
CMD(0x00FC),DAT(0x0311),        
CMD(0x0007),DAT( 0x1017),        
CMD(0x0020),DAT( 0x0000), // RAM Address
CMD(0x0021),DAT( 0x0000),
CMD(0x0022),

#endif

CMD(0x00),CMD(0x22),
 
	

END_INIT_TAB


LCD_OP_SECTION lcddev_t ili9225g = 
{
	.name = "iLi9225G",
		//.bus_cmd  = 1,
	.lcd_bus_type = LCD_BUS_I80,
	.bus_width = LCD_BUS_WIDTH_8,
	.red_width = LCD_BUS_WIDTH_5,  //6
	.green_width = LCD_BUS_WIDTH_6,
	.blue_width = LCD_BUS_WIDTH_5,  //6
	.scan_mode = LCD_ROTATE_270,
    .te_mode =  1,//0xff,//1,//te mode, 0xff:disable
	//f(wr) = source_clk/div/2
	//f(wr) >= screen_w * screen_h * clk_per_pixel * 60
	.pclk_div = PLL_CLK/(176*220*2*70)/2,
    .even_order = RGB,
    .odd_order = RGB,
    .lcd_data_mode = (DATA_NOT_INVER<<31)|//data inversion mode
                     (RGB565<<24)|//data compress mode
                     (MODE24BIT_S<<20)|//fifo mode
                     (SDIR2_UP<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (SDIR1_UP<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (SDIR0_UP<<5)| //output cycle 0 shift direction
                     (8<<0), //output cycle 0 shift bit
	.screen_w = 176,
	.screen_h = 220,
	.video_x  = 0,
	.video_y  = 26,
	.video_w  = 220,
    .video_h  = 124,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 220, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 176, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

	.clk_per_pixel = 2,
	/*
	.brightness = 4,
	.saturation = 11,
	.contrast   = 10,
	.contra_index = 6,

	.gamma_red = 5,
	.gamma_green=5,
	.gamma_blue=4,
	*/
	.brightness = -12,
	.saturation = 7,
	.contrast   = 7,
	.contra_index = 6,

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




























