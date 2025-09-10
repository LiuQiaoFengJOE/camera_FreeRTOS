#ifndef __ICN85XX_H__
#define __ICN85XX_H__

//#include "../../hal/inc/hal.h"
#include "typedef.h"


/**IIC ADDR**/

#define ICN85XX_IIC_WADDR 0x90
#define ICN85XX_IIC_RADDR 0x91

#define ICN85XX_PROG_WADDR 0x60
#define ICN85XX_PROG_RADDR 0x61


//RW
#define ICN85XX_COMMAND_ADDR        0x0004

//RO
#define ICN85XX_REG_CHIP_ID		    0x000A //chip id
#define ICN85XX_REG_FW_MVER         0x000C //fw main version
#define ICN85XX_REG_FW_SVER         0x000D //fw sub version

#define ICN85XX_REG_NUM_POINTER	    0x1001 //number of pointers 0-10
#define ICN85XX_REG_POINTER_ID	    0x1002 // 0-9
#define ICN85XX_REG_POINTER_XL	    0x1003 //x l
#define ICN85XX_REG_POINTER_XH	    0x1004 //x h
#define ICN85XX_REG_POINTER_YL	    0x1005 //y l
#define ICN85XX_REG_POINTER_YH	    0x1006 //y h
#define ICN85XX_REG_PRESSURE	    0x1007 //pressure level on pointer[0] 0~255

#define ICN85XX_REG_EVENT_ID	    0x1008  

#define ICN85XX_CHIP_ID_IS          0x85
#define ICN88XX_CHIP_ID1            0x88
#define ICN88XX_CHIP_ID2            0x86
#define ICN88XX_CHIP_ID3            0x87



#define ICN85XX_MAX_TOUCH_POINT	    5


#define ICN85XX_CNT_DOWN            3
#define ICN85XX_CNT_LONG            40
#define ICN85XX_CNT_HOLD            0

#define ICN85XX_EVENT_ID_NONE          0
#define ICN85XX_EVENT_ID_DOWN          1
#define ICN85XX_EVENT_ID_MOVE          2
#define ICN85XX_EVENT_ID_STAY          3
#define ICN85XX_EVENT_ID_UP            4

/**COMMAND DEFINE**/
#define ICN85XX_COMMAND_RESET       0x01 // MCU 复位
#define ICN85XX_COMMAND_SLEEP       0x02 //进入休眠

#define TIMEROUT_MSEC               20 //ms

typedef struct {
    u16 x;
    u16 y;
    u8  event;
    u8 fresh;
} POINT;

extern u8 icntype;

extern int icn85xx_flash_check(void);
extern int icn87xx_flash_check(void);
extern int icn85xx_fw_update(void);
extern int icn87xx_fw_update(void);

extern char icn85xx_init(void);
extern u8 icn85xx_event_handler(int *x,int *y);


//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define COL_NUM                          128//24
#define ROW_NUM                          128//36

#define MD25D40_ID1                      0x514013
#define MD25D40_ID2                      0xC84013
#define MD25D20_ID1                      0x514012
#define MD25D20_ID2                      0xC84012
#define GD25Q10_ID                       0xC84011
#define MX25L512E_ID                     0xC22010
#define MD25D05_ID                       0x514010
#define MD25D10_ID                       0x514011

#define ICN85XX_WITHOUT_FLASH              0x11
#define ICN85XX_WITH_FLASH_85              0x22
#define ICN85XX_WITH_FLASH_86              0x33
#define ICN85XX_WITHOUT_FLASH_87           0x55
#define ICN85XX_WITH_FLASH_87              0x66


#define FLASH_TOTAL_SIZE                0x00010000
#define FLASH_PAGE_SIZE                 0x1000
#define FLASH_AHB_BASE_ADDR             0x00100000
#define FLASH_PATCH_PARA_BASE_ADDR      (FLASH_TOTAL_SIZE - FLASH_PAGE_SIZE)          //  allocate 1 page for patch para, 0xff00
#define FLASH_CODE_INFO_BASE_ADDR       (FLASH_PATCH_PARA_BASE_ADDR - FLASH_PAGE_SIZE)        //  0xfe00,allocate 1 page for system para
#define FLASH_CRC_ADDR                  (FLASH_AHB_BASE_ADDR + FLASH_CODE_INFO_BASE_ADDR + 0x00) //  0xfe00
#define FLASH_CODE_LENGTH_ADDR          (FLASH_AHB_BASE_ADDR + FLASH_CODE_INFO_BASE_ADDR + 0x04) //  0xfe04

#endif //__ICN85XX_H__
