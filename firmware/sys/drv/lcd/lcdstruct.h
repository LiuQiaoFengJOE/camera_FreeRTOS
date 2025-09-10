#ifndef __LCDSTRUCT_H__
#define __LCDSTRUCT_H__

#include <stdint.h>
#include "typedef.h"
#include "drv/lcd/lcdframe.h"
#include "stream/stream.h"
#include "eloop/irqTimer.h"


#ifndef NULL
#define NULL (void *)0
#endif

enum init_tab_token_e
{
    MCU_CMD,
    MCU_DAT,
    SPI_DAT,
    DELAY_MS,
    LCD_TAB_END = -1UL,
};
typedef struct _lcd_lsawtooth_s
{
    uint32_t anti_lsawtooth[3][24]; // 0: all lcd	1:half lcd 2:small window
} lcd_lsawtooth_t;
typedef struct _lcd_bsawtooth_s
{
    uint32_t anti_bsawtooth[3][8]; // 0: all lcd  1:half lcd 2:small window
} lcd_bsawtooth_t;

typedef struct _lcd_desc_s
{
    char name[32];
    uint8_t lcd_bus_type;
    uint8_t bus_width;
    uint8_t bus_cmd;
    uint8_t red_width;
    uint8_t green_width;
    uint8_t blue_width;

    uint8_t scan_mode;
    // only mcu-bus use
    //-1,te disable.
    // 0,te just interrupt,no kick TX.用户(写APP的人)自己编写TE中断服务函数,自己控制kick tx.
    // 1,te interrupt and auto kick TX,中断被内部使用,用户不应该操作TE中断
    uint8_t te_mode;
    // for mcu data-bus, cs_setup and cs_hold eq 0 default,so freq(wr) = src_clk/div/2.
    // for rgb bus,freq(pclk) = src_clk/div;
    uint8_t pclk_div;

    // the 'lcd_data_mode' eq LCDDMOD
    uint32_t lcd_data_mode;
    uint32_t lcd_data_mode1;

    uint16_t screen_w, video_x, video_w;
    uint16_t screen_h, video_y, video_h;

    uint16_t osd_x, osd_y, osd_w, osd_h;

    uint32_t (*init_table)[2],
        (*uninit_table)[2],
        init_table_size,
        uninit_table_size;

    uint8_t clk_per_pixel;
    uint8_t even_order;
    uint8_t odd_order;
    //------rgb-bus----------
    uint8_t pclk_inv;
    // unit:line,real vfp=vfp+1(line)
    uint8_t vlw, vbp, vfp;
    // unit:pclk
    uint8_t hlw;
    // unit:pixel,real hfp=.hfp(pixel) + 1(pclk)
    uint8_t hbp, hfp;

    uint8_t de_inv; // de默认是高电平有效,如果de_inv是1，则将其反转,0xff:disable
    uint8_t hs_inv; // hs_inv同de_inv
    uint8_t vs_inv; // vs_inv同de_inv

    uint8_t spi_cpol;  // CPOL = 0,CLK is "Low" when inactive
    uint8_t spi_cpha;  // CPHA = 0,sample edge is first edge
    uint8_t spi_order; // ORDER = 0,MSB
    uint8_t spi_bits;  // number of data bits

    uint32_t contrast;

    int32_t brightness;
    uint32_t saturation;
    uint32_t contra_index;

    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;

    uint32_t de_ccm[12];
    uint32_t de_saj[5];
    lcd_lsawtooth_t lcd_lsawtooth;
    lcd_bsawtooth_t lcd_bsawtooth;
} lcddev_t;

typedef struct
{
    char name[32];
    uint16_t screen_w;
    uint16_t screen_h;
} lcd_common_t;

typedef struct
{
    uint32_t contrast;
    int32_t brightness;
    uint32_t saturation;
} lcd_vde_t;

typedef struct
{
    uint32_t contra_index;
    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;
} lcd_gamma_t;

typedef struct
{
    lcd_common_t lcd_common;
    lcd_vde_t lcd_vde;
    lcd_gamma_t lcd_gamma;
    uint32_t de_ccm[12];
    uint32_t de_saj[5];
    lcd_lsawtooth_t lcd_lsawtooth;
} usb_lcddev_t;

typedef struct lcd_buffer_s
{
    struct lcd_buffer_s *next;
    uint32_t lcd_buffer_stride;
    uint8_t *p_lcd_y_buffer; // 指向buffer起始
    uint8_t *p_lcd_uv_buffer;
    uint8_t *_p_lcd_y_buffer_; // 供de使用,de不一定使用起始地址，例如ratate
    uint8_t *_p_lcd_uv_buffer_;
    uint8_t status;
    uint8_t id;
} lcd_buffer_t;

typedef struct
{
    uint32_t lcd_order;
    uint32_t lcd_struct_adr;
    uint32_t lcd_struct_len;
    uint32_t lcd_gamma_tab_adr;
    uint32_t lcd_gamma_tab_len;
    uint32_t lcd_gamma_tab_unit;
    uint32_t lcd_contra_tab_adr;
    uint32_t lcd_contra_tab_len;
    uint32_t lcd_contra_tab_unit;
    uint32_t align[5];
} _Lcd_Header_;

typedef struct
{
    uint32_t y_off;
    uint32_t uv_off;
    uint32_t video_w;
    uint32_t video_h;
    uint32_t y_size;
    uint32_t stride;
    uint32_t x;
    uint32_t y;

} Video_Frame;

struct lcd_infors
{
	lcddev_t *lcdpanel;
	u16 x;
	u16 y;
	u16 w;
	u16 h;
	u16 osdx;
	u16 osdy;
	u16 osdw;
	u16 osdh;
	u16 crop_x;
	u16 crop_y;
	u16 crop_w;
	u16 crop_h;
	u8 crop_set;	// 1: crop setting
	u8 running;
	struct frame *curFrame;
	//u8 normal_refresh;	// 正常刷屏,就是刷屏方式与屏幕方式一样(游戏的时候)
	u8 osdUpdate;
	u8 osdCopyEn;
	u8 scan_mode;
	int showMode;
	int showModeSw;
	int previewEn;
	u8 disp_drop_frame_num;	// for csi to lcd frame drop disp 
	disp_frame_t * displaying;
	disp_frame_t * ready;
	disp_frame_t * preview;
	disp_frame_t * curInputData;
	
	disp_frame_t * osdDrawFrame;
	disp_frame_t * osdDisplaying;
	IRQ_TIMER  *lcdTimer;
	IRQ_TIMER  *lcdDeTimer;
};


#define LCD_BUS_RGB 0 // rgb ,srgb,prgb
#define LCD_BUS_I80 1 // i80-mcu-cpu

#define LCD_BUS_WIDTH_16  16
#define LCD_BUS_WIDTH_8 8 // 8-bit p
#define LCD_BUS_WIDTH_6 6 // 8-bit p
#define LCD_BUS_WIDTH_5 5 // 8-bit p
#define LCD_BUS_WIDTH_4 4 // 8-bit p

#define LCD_CMDBUS_WIDTH8 0
#define LCD_CMDBUS_WIDTH16 1

#define LCD_ROTATE_0 0
#define LCD_ROTATE_VMIRROR 1
#define LCD_ROTATE_HMIRROR 2
#define LCD_ROTATE_180 3
#define LCD_ROTATE_90 4
#define LCD_ROTATE_270 5
#define LCD_ROTATE_MASK 7

#endif /* __LCDSTRUCT_H__ */
