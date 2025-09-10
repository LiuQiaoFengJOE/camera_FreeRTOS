#include "tpiic.h"
#include "touchPanel.h"
#include "icn85xx.h"

#define TP_INT_CH    GPIO_PF
#define TP_INT_PIN   GPIO_PIN0
#define ICNT81_W     640//854
#define ICNT81_H     360//480

//#define TP_GT968
#define TP_XYH8725

#ifdef TP_GT968
#define ICNT81_IIC_ADDR 0xBA
#define GT911_MAX_TOUCH_POINT	5
void icnt81Init(void)
{
	//unsigned char buff[2];
	//hal_gpioInit(TP_INT_CH,TP_INT_PIN,GPIO_INPUT,GPIO_PULL_UP);
	//if(iicRead2(ICNT81_IIC_ADDR,0x000c,buff,2)>=0)
	//{
	//	deg_Printf("ICNT81 version:0x%x\n",((unsigned short)buff[0]<<8)|buff[1]);
	//}
}
int icnt81Match(void)
{
	unsigned char data[3];
	if(iicRead2(ICNT81_IIC_ADDR,0x8140,data,3)>=0)
	{
		deg_Printf("icnt81Match data[0]=0x%x \n",data[0]);
		deg_Printf("icnt81Match data[1]=0x%x \n",data[1]);
		deg_Printf("icnt81Match data[2]=0x%x \n",data[2]);
		if(data[0]==0x39)
			return 1;
	}
	if(iicRead2(ICNT81_IIC_ADDR,0x8140,data,1)>=0)
	{
		deg_Printf("icnt81Match data[0]=0x%x \n",data[0]);
		deg_Printf("icnt81Match data[1]=0x%x \n",data[1]);
		deg_Printf("icnt81Match data[2]=0x%x \n",data[2]);
			if(data[0]==0x39)
				return 1;
	}

	if(iicRead2(ICNT81_IIC_ADDR,0x8140,data,1)>=0)
	{
		deg_Printf("icnt81Match data[0]=0x%x \n",data[0]);
		deg_Printf("icnt81Match data[1]=0x%x \n",data[1]);
		deg_Printf("icnt81Match data[2]=0x%x \n",data[2]);
			if(data[0]==0x39)
				return 1;
	}

	return 0;
}

int icnt81getPoint(point* p)
{
	u8 touch_info;
	u8 touch_num = 0;
    u8 tid;
	u8 point_data[8 * GT911_MAX_TOUCH_POINT] = {0};
    
	int ret;
	int i;

	ret=iicRead2(ICNT81_IIC_ADDR,0x814E, &touch_info, 1);
	if(ret<0)
	{
		//deg_Printf("icnt81getPoint read iic error \n");
		goto exit_work_func;
	}

	//deg_Printf("icnt81getPoint touch_info=0x%x \n",touch_info);

	if((touch_info & 0x80) == 0) 
	{
	   //deg_Printf("icnt81getPoint (touch_info & 0x80) error \n");
       goto exit_work_func;
    }
	
    touch_num = touch_info & 0x0f;
    if (touch_num > 1)
	{
        touch_num = 1;
    }

	//deg_Printf("icnt81getPoint touch_num=0x%x \n",touch_num);

	if (touch_num > 0) {
        ret = iicRead2(ICNT81_IIC_ADDR,0x814E, point_data, 8 * touch_num);
        for (i = 0; i < touch_num; i++) {
            tid = point_data[8 * i + 1] & 0x0F;

            if (tid > 1 - 1) {
                continue;
            }

			if (0) {//_touch_panel_data->_X2Y_EN
				p->x = point_data[8 * i + 3] << 8 | point_data[8 * i + 2];
				p->y = point_data[8 * i + 5] << 8 | point_data[8 * i + 4];
			}else{
				p->y = point_data[8 * i + 3] << 8 | point_data[8 * i + 2];
				p->x = point_data[8 * i + 5] << 8 | point_data[8 * i + 4];
			}
			deg_Printf("icnt81getPoint p->x=%d,p->y=%d \n",p->x,p->y);
        }
    }

exit_work_func:
	ret = iicWrite2(ICNT81_IIC_ADDR,0x814E, (u8 *)"\0", 1);	
    if (ret <= 0) {
        //deg_Printf("I2C write end_cmd  error!\n");
        touch_num = 0;
    }

	return 1;
}
#endif


#ifdef TP_XYH8725
void icnt81Init(void)
{

}

int icnt81Match(void)
{	
	int ret = 0;
//	unsigned char buff[1];
	
	ret = icn85xx_init();

	if(ret == 1)
	{
		return 1;
	}
	else 
	{
		return 0;
	}	
		
	return 0;
}


int icnt81getPoint(point* p)
{
	int touchnum = 0;
    int x,y;
	touchnum = icn85xx_event_handler(&x,&y);
	//deg_Printf("simon touchnum=%d x=%d,y=%d\n",touchnum,x,y);

	p->x=x;
	p->y=y;
	return touchnum;
}

#endif

TOUCH_PANEL(icnt81TP,ICNT81_W,ICNT81_H,icnt81Init,icnt81Match,icnt81getPoint)


