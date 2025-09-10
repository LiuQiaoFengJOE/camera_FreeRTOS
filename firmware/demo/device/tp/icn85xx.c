#include "icn85xx.h"
#include "tpiic.h"
//#include "../../ax32_platform_demo/application.h"



extern void delay(unsigned int i);
extern void delay_2ms(int cnt);

//static POINT point[ICN85XX_MAX_TOUCH_POINT];

//static int _touch_panel_timer_reg;

u8 icntype = 0;

/*
static void TCH_RST_H()
{
    if (_touch_panel_data) {
        gpio_direction_output(_touch_panel_data->rst_pin, 1);
    }
}

static void TCH_RST_L()
{
    if (_touch_panel_data) {
        gpio_direction_output(_touch_panel_data->rst_pin, 0);
    }
}
*/

u8 icn85xx_iic_write(u16 addr, u8 *buf, u32 len)
{
    return _touch_panel_write(ICN85XX_IIC_WADDR, addr, buf, len);
}

u8 icn85xx_iic_read(u16 addr, u8 *buf, u32 len)
{
    return _touch_panel_read(ICN85XX_IIC_WADDR, ICN85XX_IIC_RADDR, addr, buf, len);
}

#if 0
static u8 command_sleep(void)
{
    u8 command = ICN85XX_COMMAND_SLEEP;
    u8 ret = 0;
    ret = icn85xx_iic_write(ICN85XX_COMMAND_ADDR, &command, 1);
    delay_2ms(50);
    return ret;
}
#endif

static u8 command_reset(void)
{
    u8 command = ICN85XX_COMMAND_RESET;
    u8 ret = 0;
    ret = icn85xx_iic_write(ICN85XX_COMMAND_ADDR, &command, 1);
    //delay_2ms(50);
    return ret;
}

static void icn85xx_reset(s32 ms)
{
  /* puts("\nicn85xxf_reset!\n");
	puts("\nicn85xxf_reset!.0\n");
    TCH_RST_L();
	puts("\nicn85xxf_reset!.0.1\n");
    //delay_2ms(ms);
	puts("\nicn85xxf_reset!.1\n");

    delay_2ms(2);
    TCH_RST_H();
    delay_2ms(6);
	puts("\nicn85xxf_reset!.2\n");
*/
    //command reset
    command_reset();
	printf("\nicn85xxf_reset!.3\n");
}

static u8 icn85xx_iic_read_dbl_check(u16 addr, u8 *rxbuf, u8 len)
{
    u8 buf[16] = {0};
    u8 confirm_buf[16] = {0};
    u8 retry = 0;

    while (retry++ < 3) {
        memset(buf, 0xAA, 16);
        icn85xx_iic_read(addr, buf, len);
        memset(confirm_buf, 0xAB, 16);
        icn85xx_iic_read(addr, confirm_buf, len);

        if (!memcmp(buf, confirm_buf, len)) {
            memcpy(rxbuf, confirm_buf, len);
            return 1;
        }
    }

    printf("\ni2c read 0x%04X, %d bytes, double check failed!", addr, len);
    return 0;
}

static u8 icn85xx_read_id(void)
{
//    u8 ret = 0;

    u8 chip_id = 0;

    icn85xx_reset(20);

    if (icn85xx_iic_read_dbl_check(ICN85XX_REG_CHIP_ID, &chip_id, 1)) {
        printf("\nicn85xxf  read 0x000A ,CHIP_ID = 0x%x\n", chip_id);
        if(chip_id == ICN88XX_CHIP_ID3 ||chip_id == ICN85XX_CHIP_ID_IS || chip_id == ICN88XX_CHIP_ID1 || chip_id == ICN88XX_CHIP_ID2){
            return 1;
        }
    }

    if(icn85xx_flash_check()){
        return 1;
    }


    return 0;

}
POINT point[1];
u8 icn85xx_event_handler(int *x,int *y)
{
    POINT pit = {0};
    u8 value;
    u8 touch_num = 0;
    u8 event_id = 0;

    static u8 fresh_comm = 0;

    int i;
    u8 ret = 0;

    ret = icn85xx_iic_read(ICN85XX_REG_NUM_POINTER, &touch_num, 1);
    if (ret != 1){
        printf("read num pointer error\n");
        goto exit_work_func;
    }
    //printf("touch_num = %d\n",touch_num);

    if (touch_num > 1) {
        goto exit_work_func;
    }

    fresh_comm ++ ;

    if (touch_num > 0){
        ret = icn85xx_iic_read(ICN85XX_REG_EVENT_ID, &event_id, 1);
        if (ret != 1){
            printf("read num event id error\n");
            return ret;
        }
        //printf("event id = %d\n",event_id);
        pit.event = event_id;

        ret = icn85xx_iic_read(ICN85XX_REG_POINTER_XL, &value, 1);
        if (ret != 1){
            printf("read num pointer xl error\n");
            return ret;
        }
        pit.x = value;
        ret = icn85xx_iic_read(ICN85XX_REG_POINTER_XH, &value, 1);
        if (ret != 1){
            printf("read num pointer xh error\n");
            return ret;
        }
        pit.x |= value << 8;
        //printf("x = %d,",pit.x);

        ret = icn85xx_iic_read(ICN85XX_REG_POINTER_YL, &value, 1);
        if (ret != 1){
            printf("read num pointer yl error\n");
            return ret;
        }
        pit.y = value;
        ret = icn85xx_iic_read(ICN85XX_REG_POINTER_YH, &value, 1);
        if (ret != 1){
            printf("read num pointer yh error\n");
            return ret;
        }
        pit.y |= value << 8;
        //printf("y = %d\n",pit.y);
    }

    if(touch_num){

        //point[0].fresh ++ ;
        //point[0].x = pit.x;
        //point[0].y = pit.y;
        //point[0].event = pit.event;

		//pit.x+=120;
		//pit.x = (360 - 1 - pit.x) + ((480-360)/2);
		//pit.x = (360 - 1 - pit.x) + ((480-360)/2);
		//pit.y = 640 - pit.y;
		//pit.x = (480 - pit.x)*360/480;

		/*if(X2Y_EN)
		{
			 *x = pit.y;
             *y = pit.x;
		}*/

		*x = pit.x;
		*y = pit.y;//pit.y*480/360;
		
		printf("YS x = %d,y = %d\n",pit.x,pit.y);
 		//printf("ZH x = %d,y = %d\n",point[0].x,point[0].y);
        //point[0].x = point[0].x * _touch_panel_data->_MAX_X / TP_MAX_X;
        //point[0].y = point[0].y * _touch_panel_data->_MAX_Y / TP_MAX_Y;
    }

    for (i = 0; i < 1; i ++) {
        if (point[i].fresh != fresh_comm) {
            point[i].fresh = fresh_comm ;
            point[i].x = 0x7FFF;
            point[i].y = 0x7FFF;
        }
    }

exit_work_func:

    return touch_num;
}



#if 0
static void icn85xx_wakeup(void)
{
    icn85xx_reset(50);
}

static char icn85xx_check(void)
{
    //_touch_panel_data = data;
    //return icn85xx_read_id();
	return 0;
}
#endif

char icn85xx_init(void)
{
    u8 ret = 0;
//    int reset_count = 0;
	
    printf("icn85xx_init start!!!\n");

    //TCH_RST_L();
    //delay_2ms(10);
	printf("icn85xx_reset \n");

    icn85xx_reset(20);
	printf("icn85xx_read_id start!!!\n");

    ret = icn85xx_read_id();
	printf("icn85xx_read_id start ret = %d \n",ret);

	printf("simon icntype = 0x%x \n",icntype);

    if(icntype == ICN85XX_WITHOUT_FLASH){
        ret = icn85xx_fw_update();
        if (ret != 1) {
            //goto g_end;
        }
    }
    else if(icntype == ICN85XX_WITHOUT_FLASH_87){
		printf("icn87xx_fw_update 1\n");
        ret = icn87xx_fw_update();
		printf("icn87xx_fw_update 1 finish\n");
        if (ret != 1) {
           // goto g_end;
        }
    }

	printf("icn87xx_fw_update 2 finish \n");


    printf("icn85xxf_init success!!!\n");
    return ret;
}


