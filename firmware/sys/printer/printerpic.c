#include "stream/stream.h"

#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "debug.h"
#include "drv/lcd/lcdframe.h"
#include "task/app.h"

#define  PRINTER_W 	384

#define  PRINTER_POWER_CH  GPIO_PD		
#define  PRINTER_POWER_PIN GPIO_PIN15

//==moto==
//#define  MOTO_NFAULT_CH  GPIO_PF		
//#define  MOTO_NFAULT_PIN GPIO_PIN2
#define  MOTO_SLEEP_CH 	 GPIO_PD		
#define  MOTO_SLEEP_PIN	 GPIO_PIN15		// LOW sleep , HIGH run
#define  MOTO_AIN1_CH 	 GPIO_PD
#define  MOTO_AIN1_PIN	 GPIO_PIN1
#define  MOTO_AIN2_CH	 GPIO_PD
#define  MOTO_AIN2_PIN	 GPIO_PIN2
#define  MOTO_BIN1_CH 	 GPIO_PD
#define  MOTO_BIN1_PIN	 GPIO_PIN3
#define  MOTO_BIN2_CH	 GPIO_PD
#define  MOTO_BIN2_PIN	 GPIO_PIN4

#define  MOTO_AIN1_HIGH()  		{GPIO_DATA(MOTO_AIN1_CH) |=	(MOTO_AIN1_PIN);}
#define  MOTO_AIN1_LOW()  		{GPIO_DATA(MOTO_AIN1_CH) &=	~(MOTO_AIN1_PIN);}
#define  MOTO_AIN2_HIGH()  		{GPIO_DATA(MOTO_AIN2_CH) |=	(MOTO_AIN2_PIN);}
#define  MOTO_AIN2_LOW()  		{GPIO_DATA(MOTO_AIN2_CH) &=	~(MOTO_AIN2_PIN);}
#define  MOTO_BIN1_HIGH()  		{GPIO_DATA(MOTO_BIN1_CH) |=	(MOTO_BIN1_PIN);}
#define  MOTO_BIN1_LOW()  		{GPIO_DATA(MOTO_BIN1_CH) &=	~(MOTO_BIN1_PIN);}
#define  MOTO_BIN2_HIGH()  		{GPIO_DATA(MOTO_BIN2_CH) |=	(MOTO_BIN2_PIN);}
#define  MOTO_BIN2_LOW()  		{GPIO_DATA(MOTO_BIN2_CH) &=	~(MOTO_BIN2_PIN);}

//==printer==
#define  PRINTER_STB_NUM  		1//  1 pin , 2pin 
#define  PRINTER_LATCH_CH	 GPIO_PF
#define  PRINTER_LATCH_PIN	 GPIO_PIN0
#define  PRINTER_PSENSOR_CH	 GPIO_PF
#define  PRINTER_PSENSOR_PIN GPIO_PIN5
#define  PRINTER_DATA_CH	 GPIO_PF
#define  PRINTER_DATA_PIN	 GPIO_PIN6
#define  PRINTER_CLK_CH	 	 GPIO_PF
#define  PRINTER_CLK_PIN	 GPIO_PIN7
#define  PRINTER_TM_CH	 	 GPIO_PF
#define  PRINTER_TM_PIN	 	 GPIO_PIN8

#if (1==PRINTER_STB_NUM)
#define  PRINTER_STB_CH	 	 GPIO_PF
#define  PRINTER_STB_PIN	 GPIO_PIN9
#define  PRINTER_STB_HIGH()  		{GPIO_DATA(PRINTER_STB_CH)|=(PRINTER_STB_PIN);}
#define  PRINTER_STB_LOW()  		{GPIO_DATA(PRINTER_STB_CH)&=~(PRINTER_STB_PIN);}
#else
#define  PRINTER_STB_CH	 	 GPIO_PF
#define  PRINTER_STB_PIN	 	 GPIO_PIN9
#define  PRINTER_STB2_CH	 	 GPIO_PD
#define  PRINTER_STB2_PIN		 GPIO_PIN13
#define  PRINTER_STB_HIGH()  		{GPIO_DATA(PRINTER_STB_CH)|=(PRINTER_STB_PIN);}
#define  PRINTER_STB_LOW()  		{GPIO_DATA(PRINTER_STB_CH)&=~(PRINTER_STB_PIN);}
#define  PRINTER_STB2_HIGH()  		{GPIO_DATA(PRINTER_STB2_CH)|=(PRINTER_STB2_PIN);}
#define  PRINTER_STB2_LOW()  		{GPIO_DATA(PRINTER_STB2_CH)&=~(PRINTER_STB2_PIN);}
#endif

#define  PRINTER_LATCH_HIGH()  		{GPIO_DATA(PRINTER_LATCH_CH)|=(PRINTER_LATCH_PIN);}
#define  PRINTER_LATCH_LOW()  		{GPIO_DATA(PRINTER_LATCH_CH)&=~(PRINTER_LATCH_PIN);}
#define  PRINTER_CLK_HIGH()  		{GPIO_DATA(PRINTER_CLK_CH)|=(PRINTER_CLK_PIN);}
#define  PRINTER_CLK_LOW()  		{GPIO_DATA(PRINTER_CLK_CH)&=~(PRINTER_CLK_PIN);}
#define  PRINTER_DAT_HIGH()  		{GPIO_DATA(PRINTER_DATA_CH)|=(PRINTER_DATA_PIN);}
#define  PRINTER_DAT_LOW()  		{GPIO_DATA(PRINTER_DATA_CH)&=~(PRINTER_DATA_PIN);}

#define NOP asm("l.nop");

static void printer_moto_delay(u32 n)
{
	volatile u32 i=500*n;
	while(i--)
	{
		asm("l.nop");
	}
}
static void printer_delay(u32 n)
{
	volatile u32 i=20*n;
	while(i--)
	{
		asm("l.nop");
	}
}
void printer_init()
{
	//==close printer power==

	hal_gpioInit(PRINTER_POWER_CH,PRINTER_POWER_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_POWER_CH,PRINTER_POWER_PIN,GPIO_LOW);
	//==moto==
	hal_gpioInit(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_HIGH);
	printer_delay(5000);
	hal_gpioInit(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_LOW);
	
	hal_gpioInit(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_LOW);
	hal_gpioInit(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);
	hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);
	hal_gpioInit(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);
	//==printer==
	hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	
	hal_gpioInit(PRINTER_DATA_CH,PRINTER_DATA_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_DATA_CH,PRINTER_DATA_PIN,GPIO_LOW);
	hal_gpioInit(PRINTER_CLK_CH,PRINTER_CLK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_CLK_CH,PRINTER_CLK_PIN,GPIO_LOW);
	hal_gpioInit(PRINTER_LATCH_CH,PRINTER_LATCH_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_LATCH_CH,PRINTER_LATCH_PIN,GPIO_HIGH);

	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_LOW);
	
#if (1==PRINTER_STB_NUM)
	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_HIGH);
	printf("printer stb 1pin\n");
#else
	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_HIGH);
	hal_gpioInit(PRINTER_STB2_CH,PRINTER_STB2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB2_CH,PRINTER_STB2_PIN,GPIO_HIGH);
	printf("printer stb 2pin\n");
#endif
	printf("printer_init\n");
}

//==return 0: have paper , 1: have not paper==
static s32 printer_get_paper()
{
	u32 ps = hal_gpioRead(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN);
	return ps;
}
static s32 printer_get_tm()
{
	hal_gpioInit(PRINTER_TM_CH,PRINTER_TM_PIN,GPIO_INPUT,GPIO_PULL_UP);
	//printer_delay(100);
	s32 tm = hal_adcGetChannel(ADC_CH_PF8);
	printf("tm=%d\n",tm);

/*
	if(tm < 288 )// very hot 
	{
      return 1;
	}
	else if(tm < 325)
	{
		return 2;
	}
	else if(tm < 415)
	{
		return 3;
	}
	else if(tm < 487)
	{
		return 4;
	}
	else if(tm < 589)
	{
		return 5;
	}
	else if(tm < 701)
	{
		return 6;
	}
	else if(tm < 789)
	{
		return 7;
	}
	else if(tm >= 789)// very cool 
	{
		return 8;
	}
*/
//==new printer==
	if(tm < 131 )// very hot , 75C
	{
      return 1;
	}
	else if(tm < 177)// 65C
	{
		return 2;
	}
	else if(tm < 236)// 55C
	{
		return 3;
	}
	else if(tm < 307)// 45C
	{
		return 4;
	}
	else if(tm < 403)// 35C
	{
		return 5;
	}
	else if(tm < 512)// 25C
	{
		return 6;
	}
	else if(tm < 682)// 10C
	{
		return 7;
	}
	else if(tm < 785)// 10C
	{
		return 8;
	}
	else if(tm >= 785)// very cool ,0C
	{
		return 9;
	}
	return 0;
}
//==en 0: disable , 1: enable===
static void printer_moto_enable(u8 en)
{
	//==init moto pin==
	hal_gpioWrite(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);

	if(en)
	{
		hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_HIGH);
		printer_moto_delay(20);
	}
	else
	{
		hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_LOW);
	}
}
//==half step==
//dir  0 : forward , 1: reserve===
#define MOTO_TIME  50//50
static s32 printer_moto_move_step(u8 dir)
{
	static u8 half_step = 0;

	if(0 == dir)
	{
		if(0 == half_step)
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			half_step = 1;
		}
		else
		{
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			half_step = 0;
		}

	}
	else
	{
		if(0 == half_step)
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_LOW();
			half_step = 1;
		}
		else
		{
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_LOW();
			half_step = 0;
		}
	}
}
static void printer_strobe(u32 n)
{
	#if (1==PRINTER_STB_NUM)
	PRINTER_STB_HIGH();
	printer_delay(n);
	PRINTER_STB_LOW();
	#else
	PRINTER_STB_LOW();
	PRINTER_STB2_LOW();
	printer_delay(n);
	PRINTER_STB_HIGH();
	PRINTER_STB2_HIGH();
	#endif
}

//==ret : 0 is ok , 8: no paper , 9: tm hot , 10: w h err
//==level    0~4  max~min
//==print mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery 0~4
u8 printer_print(u8* buf,u16 w,u16 h,u8 level,u8 print_mode,u8 batteryValue)
{
	s32 tm;
	u32 tm_hot_time;
	u32 tm_fix = 0;
	u32 tm_battery; 
	u32 all_hot_time;
	s16 i,j,k;
	u8 ret=0;

	printf("printer_print:w=%d,h=%d,level=%d,print_mode=%d\n",w,h,level,print_mode);

	hal_gpioWrite(PRINTER_POWER_CH,PRINTER_POWER_PIN,GPIO_HIGH);	//enable printer power
	printer_delay(500); 

	if(printer_get_paper())	// check paper
	{
		ret=8;
		goto PRINTER_END;
	}
 
	tm = printer_get_tm();//check tm
	if(1 == tm) // hot
	{
		ret=9;
		goto PRINTER_END;
	}

	if(2 == tm)	
	{
		tm_hot_time=0;
	}
	else if(3 == tm)
	{
		tm_hot_time=4;
	}
	else if(4 == tm)
	{
		tm_hot_time=8;
	}
	else if(5 == tm)
	{
		tm_hot_time=12;
	}
	else if(6 == tm)
	{
		tm_hot_time=16;
	}
	else
	{
		tm_hot_time=18;
	}
	//==set battery==
	printf("batteryValue %d,tm=%d\n",batteryValue,tm);
	if( batteryValue >= 4)
	{
       tm_battery = 0;
	}
	else if (3 == batteryValue)
	{
       tm_battery = 4;
	}
	else if(2 == batteryValue)
	{
       tm_battery = 8;
	}
	else if(1 == batteryValue)
	{
       tm_battery = 12;
	}
	else if(0 == batteryValue)
	{
       tm_battery = 16;
	}

	//==set level==
	if(level>4)
	{
       level = 4;
	}

	if(1==print_mode)	// DOT_MATRIX
	{
		u8 *p = buf;
		printer_moto_enable(1);
		all_hot_time = (16+tm_battery+tm_hot_time+level*12)*16;

		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				printer_moto_move_step(1);
				
				for(i=0;i<PRINTER_W;i++)
				{
					if(*(p+i)<=128)
					{
						PRINTER_DAT_HIGH();
					}
					else
					{
						PRINTER_DAT_LOW();
					}
					PRINTER_CLK_HIGH();
					PRINTER_CLK_LOW();
				}
				PRINTER_LATCH_LOW();
				PRINTER_LATCH_HIGH();

				printer_strobe(all_hot_time);
				
				ax32xx_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			u32 offset;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				printer_moto_move_step(1);
				for(offset=len-w;offset<len;offset-=w)
				{
					if(*(p+offset)<=128)
					{
						PRINTER_DAT_HIGH();
					}
					else
					{
						PRINTER_DAT_LOW();
					}
					PRINTER_CLK_HIGH();
					PRINTER_CLK_LOW();
				}
				PRINTER_LATCH_LOW();
				PRINTER_LATCH_HIGH();
				printer_strobe(all_hot_time);
				
				ax32xx_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto PRINTER_END;
		}
	}
	else	// GRAY LEVEL
	{

#define PRINTER_GRAY_STEP  32	// 64  // 128 
#define PRINTER_GRAY_TIME  64//16//25//16//32//64//64//96//192

		s16 i,j,k;
		s16 gray_step = 256/PRINTER_GRAY_STEP;
		u8 *p = buf;
		printer_moto_enable(1);
		u16 gray_tab[PRINTER_GRAY_STEP];
		for(k=0;k<PRINTER_GRAY_STEP;k++)
		{
			gray_tab[k]=k*gray_step;
		}
		
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time+level*12+tm_battery)/2;//20
		printf("hot_time = %d\n", all_hot_time);
		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				printer_moto_move_step(1);
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					for(i=0;i<PRINTER_W;i++)
					{
						if(*(p+i)<=gray_tab[k])
						{
							PRINTER_DAT_HIGH();
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
	
					printer_strobe(all_hot_time);
				}
				p += w;
				
				ax32xx_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				printer_moto_move_step(1);
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					u32 offset;
					for(offset=len-w;offset<len;offset-=w)
					{
						if(*(p+offset)<=gray_tab[k])
						{
							PRINTER_DAT_HIGH();
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					printer_strobe(all_hot_time);
				}
				
				ax32xx_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto PRINTER_END;
		}
		
	}

	//==set paper align==
	for(i= 0;i<60;i++)
	{
		 printer_moto_move_step(1);
		 printer_moto_delay(200);
		 printer_moto_move_step(1);
		 printer_moto_delay(200);
	}
	
PRINTER_END:
	printer_moto_enable(0);

	hal_gpioWrite(PRINTER_POWER_CH,PRINTER_POWER_PIN,GPIO_LOW);	//disable printer power
	return ret;

}

u8 jpgFramePrinter(struct frame* f)
{
	u8 *yuvbuf = NULL;
	u8 ret = 0;
	yuvbuf = (u8*)osal_malloc(672*384*3/2);
	if(NULL == yuvbuf)
	{
		printf("yuvbuf malloc err!\n");
		ret = 1;
		freeFrame(f);
		goto PRINTER_END;
	}
	dcache_writeback_region((u32)yuvbuf, 672*384);
	jpgDecFrame(f, yuvbuf, 672, 384, NULL);
	freeFrame(f);
	ret=printer_print((u8* )yuvbuf, 672, 384, 1, 0, 3);
PRINTER_END:

	if(yuvbuf)
	{
		osal_free(yuvbuf);
	}
	printf("[debug]ret=%d\n",ret);
	return ret;
}







