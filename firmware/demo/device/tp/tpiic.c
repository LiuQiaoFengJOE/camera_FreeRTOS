#include "tpiic.h"
//#include "../../xos/xos.h"

#include "spr_defs.h"
#include "AX329x.h"
#include "typedef.h"

#include "gpio_ops/gpio.h"
#include "sys/sys.h"


#define TP_SDA_CH    GPIO_PD//GPIO_PA
#define TP_SDA_PIN   GPIO_PIN1
#define TP_SCL_CH    GPIO_PA
#define TP_SCL_PIN   GPIO_PIN9

#define TP_IIC_DELAY   6

static void tp_iic_delay_us(u32 dtime)
{
	volatile int cnt=dtime<<1;
	while(cnt--)
	{
		asm("l.nop");
	}
}

static void tp_iic_gpioInit(u8 ch,u32 pin,u8 dir,u8 pull)
{
	ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
	ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_GPIO);     // set as gpio func
	ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);   // set normal drving
	ax32xx_gpioDirSet(ch,pin,dir);                    // set direction
	ax32xx_gpioPullSet(ch,pin,pull);                 // set pull level,
}
static void tp_iic_sdaout(void)
{
	tp_iic_gpioInit(TP_SDA_CH,TP_SDA_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
	tp_iic_gpioInit(TP_SCL_CH,TP_SCL_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
}

static void tp_iic_sdain(void)
{
	tp_iic_gpioInit(TP_SDA_CH,TP_SDA_PIN,GPIO_INPUT,GPIO_PULL_UP);
	tp_iic_gpioInit(TP_SCL_CH,TP_SCL_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
}

static void tp_iic_sda_set(u8 data)
{
	ax32xx_gpioDataSet(TP_SDA_CH,TP_SDA_PIN,data);
}
static void tp_iic_scl_set(u8 data)
{
	ax32xx_gpioDataSet(TP_SCL_CH,TP_SCL_PIN,data);
}
static u8 tp_iic_sda_get(void)
{
	return ax32xx_gpioDataGet(TP_SDA_CH,TP_SDA_PIN);
} 

static void tp_iic_start(void)
{
	tp_iic_sdaout();
	tp_iic_sda_set(1);
	tp_iic_scl_set(1);
	tp_iic_delay_us(TP_IIC_DELAY*2);
	tp_iic_sda_set(0);
	tp_iic_delay_us(TP_IIC_DELAY*2);
	tp_iic_scl_set(0);
}
 
static void tp_iic_stop(void)
{
	tp_iic_sdaout();
	tp_iic_scl_set(0);
	tp_iic_sda_set(0);
	tp_iic_delay_us(TP_IIC_DELAY*2);
	tp_iic_scl_set(1);
	tp_iic_sda_set(1);
	tp_iic_delay_us(TP_IIC_DELAY*2);
}
 
static u8 tp_iic_wait_ack(void)//0:ACK 1: NACK
{
	u8 time=0;
	tp_iic_sdain();
	tp_iic_sda_set(1);
	tp_iic_delay_us(TP_IIC_DELAY/2);
	tp_iic_scl_set(1);
	tp_iic_delay_us(TP_IIC_DELAY/2);
	while(tp_iic_sda_get())
	{
	  time++;
		if(time>250)
		{
			tp_iic_stop();
			return 1;
		}
	}
	tp_iic_scl_set(0);
	return 0;

}

static void tp_iic_ack(void)
{
	tp_iic_scl_set(0);
	tp_iic_sdaout();
	tp_iic_sda_set(0);
	tp_iic_delay_us(TP_IIC_DELAY);
	tp_iic_scl_set(1);
	tp_iic_delay_us(TP_IIC_DELAY);
	tp_iic_scl_set(0);
}

static void tp_iic_nack(void)
{
	tp_iic_scl_set(0);
	tp_iic_sdaout();
	tp_iic_sda_set(1);
	tp_iic_delay_us(TP_IIC_DELAY);
	tp_iic_scl_set(1);
	tp_iic_delay_us(TP_IIC_DELAY);
	tp_iic_scl_set(0);
}
 
static void tp_iic_sendbyte(u8 w)
{
	u8 i;
	tp_iic_sdaout();
	tp_iic_scl_set(0);
	for(i=0;i<8;i++)
	{
		tp_iic_sda_set((w&0x80)>>7);//iicsda=(w&0x80)>>7;
		w<<=1;
		tp_iic_delay_us(TP_IIC_DELAY);
		tp_iic_scl_set(1);
		tp_iic_delay_us(TP_IIC_DELAY);
		tp_iic_scl_set(0);
	}
	tp_iic_delay_us(TP_IIC_DELAY);
}

static u8 tp_iic_readbyte(unsigned char ack)
{
	unsigned char i=0,r=0;
	tp_iic_sdain();
	for(i=0;i<8;i++)
	{
		tp_iic_scl_set(0);
		tp_iic_delay_us(TP_IIC_DELAY);
		tp_iic_scl_set(1);
		r<<=1;
		if(tp_iic_sda_get())
			r++;
		tp_iic_delay_us(TP_IIC_DELAY/2);
	}
	if(ack)
		tp_iic_ack();
	else
		tp_iic_nack();
	return r;
}
 
 void tp_iic_init()
 {
	 tp_iic_sdaout();
	 tp_iic_sda_set(1);
	 tp_iic_scl_set(1);
 }

int iicWrite1(unsigned char slaveid,unsigned char addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicWrite2 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite1 wait ack error 1\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite1 wait ack error 2\n");
		return -1;
	}
	
	while(len-->0)
	{
	 	tp_iic_sendbyte(*buf++);
		if(tp_iic_wait_ack())
		{
			printf("iicWrite1 wait ack error 3\n");
			return -1;
		}
	}
	tp_iic_stop();
	return 0;
}

int iicRead1(unsigned char slaveid,unsigned char addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicRead1 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead1 wait ack error 1\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead1 wait ack error 2\n");
		return -1;
	}

	tp_iic_stop();                 // stop
	tp_iic_start();				 // send Start singal

	tp_iic_sendbyte(slaveid|1);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
		printf("iicRead1 wait ack error 3\n");
	len--;
	while(len-->0)
	 	*buf++ = tp_iic_readbyte(1);
	*buf++ = tp_iic_readbyte(0);
	tp_iic_stop();
	return 0;
}

int iicWrite2(unsigned char slaveid,unsigned short addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicWrite2 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite2 wait ack error 1\n");
		return -1;
	}

	tp_iic_sendbyte(addr>>8);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite2 wait ack error 2\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite2 wait ack error 3\n");
		return -1;
	}
	
	while(len-->0)
	{
	 	tp_iic_sendbyte(*buf++);
		if(tp_iic_wait_ack())
		{
			printf("iicWrite2 wait ack error 4\n");
			return -1;
		}
	}
	
	tp_iic_stop();
	return 0;
}

int iicRead2(unsigned char slaveid,unsigned short addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicRead2 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead2 wait ack error 1\n");
		return -1;
	}

	tp_iic_sendbyte(addr>>8);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead2 wait ack error 2\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead2 wait ack error 3\n");
		return -1;
	}

	tp_iic_stop();                 // stop
	tp_iic_start();				 // send Start singal

	tp_iic_sendbyte(slaveid|1);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
		printf("iicRead2 wait ack error 4 0x%x\n",slaveid|1);
	len--;
	while(len-->0)
	 	*buf++ = tp_iic_readbyte(1);
	*buf++ = tp_iic_readbyte(0);
	tp_iic_stop();
	return 0;
}

int iicWrite3(unsigned char slaveid,unsigned int addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicWrite3 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite3 wait ack error 1\n");
		return -1;
	}

	tp_iic_sendbyte(addr>>16);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite3 wait ack error 2\n");
		return -1;
	}
	tp_iic_sendbyte(addr>>8);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite3 wait ack error 3\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicWrite3 wait ack error 4\n");
		return -1;
	}
	
	while(len-->0)
	{
	 	tp_iic_sendbyte(*buf++);
		if(tp_iic_wait_ack())
		{
			printf("iicWrite3 wait ack error 5\n");
			return -1;
		}
	}
	
	tp_iic_stop();
	return 0;
}

int iicRead3(unsigned char slaveid,unsigned int addr, unsigned char *buf, unsigned int len)
{
	if(len==0)
		return -1;
	//printf("iicRead3 addr:[0x%x], register[0x%x]\n",slaveid,addr);
	tp_iic_start();

	tp_iic_sendbyte(slaveid);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead3 wait ack error 1\n");
		return -1;
	}

	tp_iic_sendbyte(addr>>16);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead3 wait ack error 2\n");
		return -1;
	}
	tp_iic_sendbyte(addr>>8);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead3 wait ack error 3\n");
		return -1;
	}
	tp_iic_sendbyte(addr);     // send address
	if(tp_iic_wait_ack())             // recv ack singal
	{
		printf("iicRead3 wait ack error 4\n");
		return -1;
	}

	tp_iic_stop();                 // stop
	tp_iic_start();				 // send Start singal

	tp_iic_sendbyte(slaveid|1);  // send slaveid
	if(tp_iic_wait_ack())             // recv ack singal
		printf("iicRead3 wait ack error 5\n");
	len--;
	while(len-->0)
	 	*buf++ = tp_iic_readbyte(1);
	*buf++ = tp_iic_readbyte(0);
	tp_iic_stop();
	return 0;
}


int _touch_panel_read(unsigned char w_chip_id, unsigned char r_chip_id, unsigned int register_address, unsigned char *buf, unsigned int len)
{
	int ret = 0;
	//printf("read w_chip_id = 0x%x \n",w_chip_id);
	//printf("read register_address = 0x%x \n",register_address);
	//printf("read buf = 0x%x \n",*buf);
	//printf("read len = %d \n",len);
	ret = iicRead2(w_chip_id,register_address, buf, len);

	if(ret >= 0)
		return 1;
	else
		return 0;
}

int _touch_panel_write(unsigned char w_chip_id, unsigned short register_address, unsigned char *buf, unsigned int len)
{
	int ret = 0;

	//printf("write w_chip_id = 0x%x \n",w_chip_id);
	//printf("write register_address = 0x%x \n",register_address);
	//printf("write buf = 0x%x \n",*buf);
	//printf("write len = %d \n",len);
	ret = iicWrite2(w_chip_id,register_address, buf, len);

	if(ret >= 0)
		return 1;
	else
		return 0;
}

int _touch_prog_read(unsigned char w_chip_id, unsigned char r_chip_id, unsigned int register_address, unsigned char *buf, unsigned int len)
{
	int ret = 0;
	ret = iicRead3(w_chip_id,register_address, buf, len);

	if(ret >= 0)
		return 1;
	else
		return 0;
}

int _touch_prog_write(unsigned char w_chip_id, unsigned short register_address, unsigned char *buf, unsigned int len)
{
	int ret = 0;
	ret = iicWrite3(w_chip_id,register_address, buf, len);

	if(ret >= 0)
		return 1;
	else
		return 0;
}

void delay_2ms(int delay)
{
	/*XOSTimeDly*/osal_task_sleep(2*delay);
}
















