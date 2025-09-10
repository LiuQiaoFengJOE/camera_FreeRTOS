#include "typedef.h"
#include "ax329x.h"
#include "AX329x_IRQ.h"
#include "usb/usb11.h"
#include "usb/usb.h"
#include "board_config.h"


u8 (*fp11_epout_isr_hal)(void);
u8 (*fp_host11_connect_hal)(void);
u8 (*fp_host11_remove_hal)(void);

void usb11_registers_write(u8 addr, u8 reg_data)
{
	USB11CON4 = (reg_data<<8)|     //write data
				(1<<7)|            //kick
				(0<<6)|            //write
				(addr&0x3f);			   //reg addr
}
u8 usb11_registers_read(u8 addr)
{
	USB11CON4 = (1<<7)|            //kick
				(1<<6)|            //read
				(addr&0x3f);			   //reg addr
	//printf("USB11CON0:%x\n",USB11CON0);
	//while((USB11CON0 & 0x02)==0x00){;}  //wait done
	return USB11DATA;              //read data
}
void usb_phyreset_as_typea11(void)
{
	//disable upll, uosc ,fix usb hot insert bug
	USB11CON0 = USB11CON1 =USB11CON2 = USB11CON3 = 0;

	
	osal_task_sleep(200); 
	//printf("husb:<init as usb 1.0 typA...>\n");
	PCON1 &= ~BIT(0);
	//CLKCON1 |= BIT(26);
	//CLKCON2 |= BIT(27); //DDLCLK 
	CLKCON2 &= ~BIT(27); //SYSCLK 
	CLKCON2 &= ~(BIT(26)|BIT(25)|BIT(24));
    CLKCON2 |= (((PLL_CLK/48000000)-1)<<24);
//	CLKCON2 |= (1<<24); //96M/(1+1) = 48M
	USB11CON1 |= (1<<3);                      //RESET	
	while((USB11CON1 &BIT(3))==0); //wait for usb1.1 reset release
	
	/* {USBTM,DPPSEL,PHYEN,PHYSPDSEL,PHYSPD,RNW,RWDONE,RWKICK} */
	USB11CON0 = 0xf0;//0xF0; 	"IC 建议改成F0"
	//USB11CON2 [18:8]EPSIZE, [7:4]EP3TXVAL~EP0TXVAL
	USB11CON2 = 0x00;
	USB11CON1 &= ~(BIT(4)|BIT(6));	//15k pullup
	//USB11CON1 &= ~(BIT(7)|BIT(5));	//15k pulldown
	USB11CON1 |= BIT(7)|BIT(5)//15k pulldown
			  |BIT(2)|BIT(1)|BIT(0);		//VBUS
	USB11CON3 |= (1<<12); //sof mask
	USB11CON3 &= ~BIT(11);   //CID : 0 : host 1 : device	
	
	//usb11_registers_write(DevCtl,0x03); 
	
	// printf("USB11CON0:%x, USB11CON1:%x, USB11CON2:%x, USB11CON3:%x\n",USB11CON0,USB11CON1,USB11CON2,USB11CON3);
	//printf("DevCtl:%x\n",usb11_registers_read(DevCtl));
}
void usb11_host_reset(void)
{
	usb11_registers_write(IntrTx1E,BIT(0));//ep0 isr en;
	usb11_registers_write(IntrUSBE,BIT(4)|BIT(5));//connect	isr en//disconnect isr en
	usb11_registers_write(DevCtl,0x03); 
	//usb11_registers_write(IntrUSBE,usb11_registers_read(IntrUSBE)&(~BIT(2)));//connect	isr en//disconnect isr en
	//USB20_IntrUSBE &=~BIT(2);  //babble err int disable
	usb11_registers_write(FAddr,0);
	//printf("FAddr:%d\n",usb11_registers_read(FAddr));
	//printf("IntrTx1E:%x\n",usb11_registers_read(IntrTx1E));
	//printf("IntrTx1E:%x\n",usb11_registers_read(IntrUSBE));
	USB11CON0 |= BIT(5);   //usb phy power on
}

void ax32xx_usb11_hostIRQHanlder(unsigned int n, void *d, void *r)
{
	bool transta = api_backrec_status();
	u8 intrusb = usb11_registers_read(IntrUSB);//& read_usb_reg(IntrUSBE);
    u8 introul = usb11_registers_read(IntrRx1);
    u8 intrin1 = usb11_registers_read(IntrTx1);
	
    if (introul){ //USB epxout
		if(fp11_epout_isr_hal)
			(*fp11_epout_isr_hal)();
    }

	//remove
	if(intrusb & BIT(5)){
		printf("husb11: <remove>\n");
		(*fp_host11_remove_hal)();
		return;		
	}
	//insert
	if(intrusb & BIT(4)){
		USB11CON0 &=~BIT(6);
		printf("husb11: <insert>:0x%x,0x%x\n",USB11CON0,intrusb);
		(*fp_host11_connect_hal)();
		return;
	}
	
	
	
	//USB bable err
    if (intrusb & BIT(2)){ 
		
		if(transta){ //epx tran state
			printf("ERR:-bable <reset usb dev>\n");
			(*fp_host11_remove_hal)();
		}else{ //ep0 enum state
			/*if(fp_mjpBcodec_linking) //usensor had acteched ok but 1st mjp decode is not ok
			{
				printf("ERR:-bable <reset usb dev>\n");
				(*fp_host11_remove_hal)();
			}
			else*/
			{
				printf("ERR:-bable <enum retry >\n");
				usb_stack_init();
				husb_kick_ep0isr();//husb_ep0isr();
			}
		}
		return;
    }
	
	if(intrin1 & BIT(0)){
		husb_ep0isr();	
	}	
}









/////////////////////////USB2.0////////////////////////////
void usb_phyreset_as_typea(void)
{
	//disable upll, uosc ,fix usb hot insert bug
	USB20CON0 = 0;
	osal_task_sleep(100); 
#if 1  
	USB20CON0 |= 0x80;	//osc enable
	combo_delay(10);
	USB20CON0 |= 0x40;	//pll enable
	combo_delay(10);
	USB20CON0 |= 0x6400;//vbusvalid adevice downstream_port
	combo_delay(10);
	USB20CON0 |= 0x1;	//release sie reset
	combo_delay(10);
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay(10);
#endif
	// initial USB host phy
	//printf("husb:<init as usb typA...>\n");
	//osal_task_sleep(1000); 
	#if TESTXOSC12
		PFDIE   &= ~(3<<8);  // die pf8,pf9 for xosc12M
		CLKCON2 |= 1<<3;     // xosc12M en
		USB20CON0 |= (1<<26);  // sel test clk in from pa13
	#else
/*
		CLKCON2 |= BIT(16);
		PMAPCON0 = 0;
		PMAPCON1 &= ~(0xF00);
		PADIR &= ~BIT(2);
		CLKCON0 = (CLKCON0 & ~(BIT(14)|BIT(15))) | (BIT(16)|BIT(14));
*/
	#endif
	USB20CON0 &= ~(BIT(22)|BIT(23) //pll div2
				|BIT(21)|BIT(20)
				);
	USB20CON0 |= BIT(22)
				|BIT(18) |BIT(19) //10k pulldown
				|BIT(13)			//VBUS
				//|BIT(9)			//VBUS
				//|BIT(12)
				//|BIT(11)
				;
				
	USB20CON0 &= ~(BIT(17)			//PULL R control by SIE
				| BIT(16)			//type A
				| BIT(24) | BIT(25))	//USB OSC
				;
	USB20_Power = 0x70; // Software Connection Control Enable, High-Speed Enable, High-Speed Mode Enable
}
void usb_host_reset(void)
{	
	//USB20_IntrOutel=0;
	USB20_IntrInel |= BIT(0);	//ep0 isr en;
	//USB20_IntrUSBE &=~BIT(2);  //when usb 2.0 line 10m,disable "Babble Error int"
	USB20_IntrUSBE |= 
					 BIT(4) //connect	isr en	
					|BIT(5);//disconnect isr en
	USB20_Devctl=0x03;
	(USB20_FAddr) = 0;
}

u8 (*fp_epout_isr_hal)(void);
u8 (*fp_host_connect_hal)(void);
u8 (*fp_host_remove_hal)(void);
void ax32xx_usb_hostIRQHanlder(unsigned int n, void *d, void *r)
{
	bool transta = api_backrec_status();
    u8 intrusb = USB20_IntrUSB;//& read_usb_reg(IntrUSBE);
    u8 intrin1 = USB20_IntrInl;
	USB20_IntrInh;//u8 intrinh = USB20_IntrInh;
	u8 introul = USB20_IntrOutl;
	USB20_IntrOuth;//u8 introuh = USB20_IntrOuth;
    if (introul){ //USB epxout
    //printf("1");
		if(fp_epout_isr_hal)
			(*fp_epout_isr_hal)();
    }	
	//remove
	if(intrusb & BIT(5)){
		printf("husb: <remove>\n");
		(*fp_host_remove_hal)();	
		return;	
	}
	//insert
	if(intrusb & BIT(4)){
		printf("husb: <insert>\n");
		(*fp_host_connect_hal)();
		return;
	}
		
	//USB bable err
    if (intrusb & BIT(2)){ 
		
		if(transta){ //epx tran state
			printf("ERR:-bable <reset usb dev>\n");
			(*fp_host_remove_hal)();
		}else{ //ep0 enum state
			/*if(fp_mjpBcodec_linking) //usensor had acteched ok but 1st mjp decode is not ok
			{
				printf("ERR:-bable <reset usb dev>\n");
				(*fp_host_remove_hal)();
			}else*/
			{
				printf("ERR:-bable <enum retry >\n");
				usb2_stack_init();
				husb2_kick_ep0isr();//husb_ep0isr();
			}
		}
		return;
    }
	if(intrin1 & BIT(0)){
		//husb_ep0isr();
		husb2_ep0isr();
	}	

}

void usb_memcpy(uint8_t dir,uint32_t dst,uint32_t src,uint32_t len)
{
	 USB11MCPCON |= BIT(0);
	 USB11MCPCON &= ~(1<<2);
	 USB11MCPCON |= (dir<<2);
	 USB11MCPSCADR = src;
	 USB11MCPTGADR = dst;
	 USB11MCPLENGTH = len -1; 
	 while(!(USB11MCPPND & BIT(0)));
// INT_STR (" wait\r\n");
}


