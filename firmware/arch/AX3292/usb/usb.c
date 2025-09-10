#include "usb.h"

//__FIFO_USB__ usbfifo  __attribute__ ((section("._sram_use")));

/*USB_CSR0*/
#define SERVRICE_RXPKTRDY BIT(6)
#define DATAEND BIT(3)
#define TXPAKTRDY BIT(1)
#define SENDSTALL BIT(5)

#define USB_FIFO0 USB20_FIFO0
#define USB_FIFO1 USB20_FIFO1
#define USB_FIFO2 USB20_FIFO2
#define USB_FIFO3 USB20_FIFO3
#define USB_FIFO4 USB20_FIFO4
#define USB_FIFO5 USB20_FIFO5
#define USB_FIFO6 USB20_FIFO6

#define USB_Index USB20_Index

#define USB_FAddr USB20_FAddr
#define U20E0ADR USB20E0ADR
#define USB_CSR0 USB20_CSR0
#define USB_OutCount1 USB20_OutCount1
#define USB_IntrUSBE USB20_IntrUSBE
#define USBCON0 USB20CON0
#define USB_Power USB20_Power
#define USBTST0 USB20TST0

#define USB_Devctl USB20_Devctl
#define USB_RxType USB20_RxType
#define U20E1RADR USB20E1RADR
#define U20E2RADR USB20E2RADR
#define U20E3RADR USB20E3RADR
#define U20E4RADR USB20E4RADR

#define USB_IntrOutel USB20_IntrOutel
#define USB_OutCSR1 USB20_OutCSR1
#define USB_OutCSR2 USB20_OutCSR2

#define USB_InMaxPl USB20_InMaxPl
#define USB_InMaxPh USB20_InMaxPh
#define USB_OutMaxPl USB20_OutMaxPl
#define USB_OutMaxPh USB20_OutMaxPh
#define USB_OutCount2 USB20_OutCount2
#define USB_IntrUSB USB20_IntrUSB
#define USB_IntrOutl USB20_IntrOutl
#define USB_IntrInl USB20_IntrInl
#define USB_IntrInh USB20_IntrInh
#define USB_InCSR1 USB20_InCSR1
#define USB_InCSR2 USB20_InCSR2

void usb_ep_init(dev_mesg *usb_dev, u8 in_ep);
void kick_usb(dev_mesg *usb_dev);
void usb_host_speed(u8 speed);
void usb_init(void);
TSTA usb_setup_request(dev_mesg *stack, u8 *ppkg);
TSTA usb_txrx_pkg(u8 epx, dev_mesg *ustack);

void delay_5ms(u16 n)
{

	volatile u32 i = 5000 * n;
	while (i--)
	{
		// asm("nop");
	}
}

static void delay(uint32_t cnt)
{
	while (cnt--)
		asm("l.nop");
}

static u8 set_command(dev_mesg *ustack, u32 tlen)
{
	// u32 tlen = ustack->len;
	u8 cmd = 0;
	u32 i;
	if (ustack->trans_type != TRANSF_SETUP)
	{
		cmd |= 0x40;
	}

	switch (ustack->trans_type)
	{
	// TxPktRdy, SetupPkt
	case TRANSF_SETUP:
		for (i = 0; i < tlen; i++)
		{
			USB_FIFO0 = ustack->buffer[i];
		}
		// memcpy(USB_FIFO0 ,ustack -> buffer,tlen);
		cmd = 0x0A;
		break;

	// ReqPkt
	case TRANSF_IN:
		cmd |= 0x20;
		break;

	// TxPktRdy
	case TRANSF_OUT:
		cmd |= 0x02;
		for (i = 0; i < tlen; i++)
		{
			USB_FIFO0 = ustack->buffer[i];
		}
		// memcpy(USB_FIFO0 ,ustack -> buffer,tlen);
		break;

	default:
		return _PIDER_;
		break;
	}
	// printf("cmd %x\n",cmd);
	return cmd;
}

TSTA usb_txrx_pkg(u8 epx, dev_mesg *ustack)
{

	USB_Index = epx;
	USB_FAddr = ustack->addr;
	U20E0ADR = (u32)ustack->ep0_fifo; // reset USB_FIFO0 to ep0fifo

	u8 i;
	u32 tmout = 0xffffff;
	u8 tlen = (ustack->len >= ustack->payload) ? ustack->payload : ustack->len;
	u8 cmd = 0;

	cmd = set_command(ustack, tlen);
	USB_CSR0 = cmd;

	while (1)
	{
		u8 intr = USB_CSR0;
		if (intr & 0x10)
		{
			printf("ep0: <send stall er>\n");
			return _STALL_;
		}

		if (intr & 0x04)
		{
			printf("ep0: <stall detected>\n");
			USB_CSR0 &= ~0x04;
			return _STALL_; // for unsupported request.
		}
		if (ustack->trans_type == TRANSF_IN)
		{

			if ((intr & 0x01) == 0x01)
			{
				u32 rlen = USB_OutCount1;
				for (i = 0; i < rlen; i++)
				{
					ustack->buffer[i] = USB_FIFO0;
				}
				ustack->buffer += rlen;
				ustack->len -= tlen;
				USB_CSR0 &= ~0x01; // Clear INPktRdy bit

				U20E0ADR = (u32)ustack->ep0_fifo; // reset USB_FIFO0 to ep0fifo
				if (ustack->len)
				{
					tlen = (ustack->len >= ustack->payload) ? ustack->payload : ustack->len;
					USB_CSR0 = 0x20;
				}
				else
				{
					return _OK_;
				}
			}
		}
		// packet has send
		else
		{
			if ((intr & 0x02) == 0x00)
				return _OK_;
		}
		if (!--tmout)
		{
			printf("ep0: <step tmout>\n");
			return _TOUT_;
		}
	}

	return _UNKWN_;
}

TSTA usb_setup_request(dev_mesg *stack, u8 *ppkg)
{
	TSTA ret = _OK_;
	u8 epx = 0;
	u32 i;
	request_pack *setup = &stack->class_req;
	// setup
	stack->trans_type = TRANSF_SETUP;
	stack->len = 8;
	stack->buffer = (u8 *)setup;
	if (_OK_ != (ret = usb_txrx_pkg(epx, stack)))
	{
		return ret;
	}

	stack->trans_type = TRANSF_IN;
	stack->len = setup->length;

	if (stack->len)
	{

		if (0x80 == (setup->request_type & 0x80))
		{

			stack->trans_type = TRANSF_IN;
			stack->len = setup->length;
			stack->buffer = (u8 *)ppkg;
			if (_OK_ != (ret = usb_txrx_pkg(epx, stack)))
			{
				return ret;
			}
			stack->trans_type = TRANSF_OUT;
		}
		else
		{

			stack->trans_type = TRANSF_OUT;
			stack->len = setup->length;
			stack->buffer = (u8 *)ppkg;
			if (_OK_ != (ret = usb_txrx_pkg(epx, stack)))
			{
				return ret;
			}
			stack->trans_type = TRANSF_IN;
		}
	}

	stack->len = 0;
	if (_OK_ != (ret = usb_txrx_pkg(epx, stack)))
	{
		return ret;
	}

	return ret;
}

#define TESTXOSC12 0 // 1: use exusbclk 0:use intlusbclk

void usb_reset(void)
{
	USB_IntrUSBE |=
		BIT(4); // connect	isr en
	//|BIT(5);//disconnect isr en
	// ep0 cfg
	(USB_FAddr) = 0;
}
#if 0
void usb_init(void)
{
	// PCON0 &= ~BIT(28);
	printf("USBCON0:%X\n", USBCON0);
	USBCON0 = 0X21306C00;
	USBCON0 |= (1 << 17);
	USBCON0 &= ~BIT(18);
	USBCON0 &= ~BIT(19);
	CLKCON2 &= ~BIT(16);
	combo_delay(10);

	// initial USB host phy
	printf("usb_host_init\n");

	USBCON0 |= 0x80; // osc enable
	combo_delay(10);
	USBCON0 |= 0x40; // pll enable
	combo_delay(10);
	USBCON0 |= 0X10000; // vbusvalid adevice downstream_port
	combo_delay(10);
	USBCON0 |= 0x1; // release sie reset
	combo_delay(10);
	USBCON0 |= 0x4; // release phy reset
	combo_delay(10);

	USBCON0 &= ~(BIT(22) | BIT(23) // pll div2
				 | BIT(21) | BIT(20));
	USBCON0 |= BIT(22) | BIT(18) | BIT(19) // 10k pulldown
			   | BIT(13)				   // VBUS
		//|BIT(9)			//VBUS
		;

	USBCON0 &= ~(BIT(17)			  // PULL R control by SIE
				 | BIT(16)			  // type A
				 | BIT(24) | BIT(25)) // USB OSC
		;

	USB_Power = 0x70;	// Software Connection Control Enable, High-Speed Enable, High-Speed Mode Enable
	USBTST0 |= BIT(26); // DISABLE DISCONNECT
	// #endif
	USB_Devctl = 0x03;
	USB20_IntrInel = USB_INEP_INT(0);
	usb_reset();
}
#else
void usb_init(void)
{
	//disable upll, uosc ,fix usb hot insert bug
	USB20CON0 = 0;
	//printf("husb:<init as usb typA...>\n");
	osal_task_sleep(100); 
	USB20CON0 &= ~(BIT(22)|BIT(23) //pll div2
				|BIT(21)|BIT(20)
				);
	USB20CON0 |= BIT(22)
				|BIT(18) |BIT(19) //10k pulldown
				|BIT(13)			//VBUS
				//|BIT(9)			//VBUS
				;
				
	USB20CON0 &= ~(BIT(17)			//PULL R control by SIE
				| BIT(16)			//type A
				| BIT(24) | BIT(25))	//USB OSC
				;
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
	USB20_Power = 0x70; // Software Connection Control Enable, High-Speed Enable, High-Speed Mode Enable

	USB20_IntrInel |= BIT(0);	//ep0 isr en;
	//USB20_IntrUSBE &=~BIT(2);  //when usb 2.0 line 10m,disable "Babble Error int"
	USB20_IntrUSBE |= 
					 BIT(4) //connect	isr en	
					|BIT(5);//disconnect isr en
	USB20_Devctl=0x03;
	//ep0 cfg
	(USB20_FAddr) = 0;
}

#endif
/////////////////////////////////////////////////////////////////
void usb_phyreset_as_typeb(void)
{
	USB20CON0   = 0x21307400;
	//CLKCON2 &= ~BIT(16);
	combo_delay (100);
	
	printf("Usb2.0 Init\r\n");
	#if TESTXOSC12
		PFDIE   &= ~(3<<8);  // die pf8,pf9 for xosc12M
		CLKCON2 |= 1<<3;     // xosc12M en
		USB20CON0 |= (1<<26);  // sel test clk in from pa13
	#else
		CLKCON2 |= BIT(16);
		//PMAPCON0 = 0;
		//PMAPCON1 &= ~(0xF00);
		//PADIR &= ~BIT(2);
		CLKCON0 = (CLKCON0 & ~(BIT(14)|BIT(15))) | (BIT(16)|BIT(14));
	#endif

#if TESTXOSC12
	//PADIR   |= 1<<13;   // PA13 input test clock
	//USB20CON0 |= 1<<9 ;   // test clk en
#endif	
 //////////////////////////////////////////////////////
   
	USB20CON0 |= BIT(16); //type B device
	combo_delay (1);
	USB20CON0 &= ~(BIT(17)                   //PULL R control by SIE
			   |BIT(22)|BIT(23)            //USB PLL SEL
			   |BIT(24)|BIT(25)            //USB  OSC
			   |BIT(11)
               |BIT(10)|BIT(20)|BIT(21)) ;                  //no down stream port
    USB20CON0 |= BIT(12)|BIT(28);
    //USB20CON0 |= BIT(15)|BIT(14)|BIT(13);
	
	//复位USB PHY
	// 使能USB PHY OSC, USBCONUSB20CON0等待1uS穩定
	USB20CON0 |= 0x80;	// OSC enable
	combo_delay (1);
	// 使能USB PHY PLL, USBCONUSB20CON0等待200uS穩定
	USB20CON0 |= 0x40;	// PLL enable
	combo_delay (1000);
	// 釋放USB SIE復位,USBCON0[0]=1
	USB20CON0 |= 0x1;	//release sie reset
	combo_delay (1);
	// 釋放USB PHY復位,USBCON0[2]=1,等待300uS使ClkTun穩定 modi @20131216
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay (100);
}
void usb_burst_reset(void)
{	 
		//初始化SIE寄存器
	USB20_Power = 0x60;
	USB20_FAddr = 0x00;
	USB20_Testmode = 0x00;
	USB20_Index = 0;
	
	//usb sie中断使能
	USB20_IntrUSBE  = USB_RESET | USB_SUSPEND | USB_SOF;
	USB20_IntrInel = USB_INEP_INT(0)|USB_OUTEP_INT(3);//USB_INEP_INT(0)|USB_INEP_INT(1)|USB_INEP_INT(2)|USB_OUTEP_INT(3);
	USB20_IntrOutel = USB_OUTEP_INT(1);//USB_OUTEP_INT(1)|USB_OUTEP_INT(2)|USB_OUTEP_INT(3);
	//usbIsrCnt = 0;
/*
	//msc init
	(*fp_enum_epx_cfg)(0);
	(*fp_msc_epx_cfg)();
	(*fp_uvc_epx_cfg)();
	(*fp_uac_epx_cfg)();

*/	
}
static void (*ax32xxUSBIRQ[USB_IRQ_MAX])(void);
void ax32xx_usbCallbackRegister(u8 type,void (*callback)(void))
{
	if(type<USB_IRQ_MAX)
	{
		ax32xxUSBIRQ[type] = callback;
	}
}
static u16 u32softcnt;
u16 get_u32softcnt(void)
{
	return u32softcnt;
}
void usbDevIRQHanlder(unsigned int irq_num, void *arg, void *reg)
{
	//printf("uisr\n");
    u8 intrusb = USB20_IntrUSB;//& read_usb_reg(IntrUSBE);
    //u8 introut1 = read_usb_reg(IntrRx1);
    u8 intrin1 = USB20_IntrInl;
	//printf(".%x,%x\n",intrin1,USB20_IntrUSB);
    if (intrusb & BIT(2)) 
	{
       // printf("USB -reset\n");
        usb_burst_reset();
		if(ax32xxUSBIRQ[USB_IRQ_RESET])
			ax32xxUSBIRQ[USB_IRQ_RESET]();
    }
	else{
		
        if (intrusb & BIT(0)) {
            //USB Suspend
            USB20_IntrUSBE = (BIT(2) | BIT(1) | BIT(3));  //Enable Resume int
            printf("USB -suspend\n");
			//usb_init();
        } else if (intrusb & BIT(1)) {
            //USB Resume
            USB20_IntrUSBE = BIT(2) | BIT(3);  //Only Reset
            printf("USB -resume\n");
			
        } else if (intrusb & BIT(3)) {
            //USB sof
            USB20_IntrUSBE = BIT(2) | BIT(3);  //Only Reset //Only Reset
			//printf("- USB sof \n");
			//usb_insert();
			u32softcnt++;
        }		
	}
    if(USB20_IntrOutl & BIT(1))
	{
		sendMsgToUsbDev();
	}
    if (intrin1 & BIT(0)) {
		if(ax32xxUSBIRQ[USB_IRQ_EP0])
			ax32xxUSBIRQ[USB_IRQ_EP0]();
		
    }
    if (intrin1 & BIT(4)) {
		//printf("uvc_isr\n");
    }	
}
#if 0
s32 usb_device_check(void)
{
	static u32 usbCheckStat=0;
	static u32 lastTime=0;
	static u32 tmp0=0, tmp1=0;
if(usbCheckStat==1)
	goto USB_CHECK_STATE1;
if(usbCheckStat==2)
	goto USB_CHECK_STATE2;

	#define _TEST_JK_MOD0_	0
	REG32(USB20CON0)  = 0;
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) |= ((1<<18)|(1<<19));//enable dp,dm 10K pdown
	REG32(USB20CON0) &= ~((1<<21)|(1<<20)); //disable dp,dm 120K pullup

	usbCheckStat=1;
	tmp0=0;
	tmp1=0;
	lastTime=os_time_get();
USB_CHECK_STATE1:	
	if(os_time_get()<lastTime)
	{
		lastTime=os_time_get();
		return -1;
	}
	if(os_time_get()-lastTime<60)
		return -1;
	//ax32xx_sysCpuMsDelay(60);
	tmp0 = (REG32(USB20CON0) >> 30) & 0x03 ;
	printf("<<<J,K :%x\n",tmp0);
	
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) &= ~((1<<18)|(1<<19));//disable dp,dm 10K pdown
	REG32(USB20CON0) |= ((1<<21)|(1<<20)); //enable dp,dm 120K pullup

	usbCheckStat=2;
	lastTime=os_time_get();
USB_CHECK_STATE2:	
	if(os_time_get()<lastTime)
	{
		lastTime=os_time_get();
		return -1;
	}
	if(os_time_get()-lastTime<60)
		return -1;
	//ax32xx_sysCpuMsDelay(60);
	tmp1 = (REG32(USB20CON0) >> 30) & 0x03 ; 
	
	printf(">>>J,K :%x\n",tmp1);
	if(_TEST_JK_MOD0_ == (tmp0 | tmp1))// && (usbIsrCnt)
	{
         return 1;
	}
	return 0;
}
#else
s32 usb_device_check(void)
{
	u32 tmp0,tmp1;
	#define _TEST_JK_MOD0_	0
	REG32(USB20CON0)=0;
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) |= ((1<<18)|(1<<19));//enable dp,dm 10K pdown
	REG32(USB20CON0) &= ~((1<<21)|(1<<20)); //disable dp,dm 120K pullup
	osal_task_sleep(60);
	tmp0 = (REG32(USB20CON0) >> 30) & 0x03 ;
	printf("<<<J,K :%x\n",tmp0);
	
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) &= ~((1<<18)|(1<<19));//disable dp,dm 10K pdown
	REG32(USB20CON0) |= ((1<<21)|(1<<20)); //enable dp,dm 120K pullup
	osal_task_sleep(60);
	tmp1 = (REG32(USB20CON0) >> 30) & 0x03 ; 
	
	printf(">>>J,K :%x\n",tmp1);
	if(_TEST_JK_MOD0_ == (tmp0 | tmp1))// && (usbIsrCnt)
	{
         return 1;
	}	
	return 0;
}

#endif

////////////////////////////////////////////////////////////////
void usb_open_clk(void)
{
	USBCON0 &= ~(BIT(22) | BIT(23) // pll div2
				 | BIT(24) | BIT(25));
	USBCON0 |= 0x80; // osc enable
	combo_delay(1);
	USBCON0 |= 0x40; // pll enable
	combo_delay(1000);
	USBCON0 |= 0x4; // release phy reset
	combo_delay(100);
}

INTSTACK intstack;

void uvc_actech_as_control(void)
{
	// kick
	u8 epindex = USB_Index;
	printf("control endpoint is :%d\n", intstack.episoin);
	if (intstack.episoin == 0xff)
		return;
	intstack.addr = 4;
	U20E1RADR = (u32)ep1_fifo;
	// printf("uvc_rx_fifo=%X\n",uvc_rx_fifo);
	USB_Index = 1;									// select use ep
	USB_RxType = (TT_INTERRUPT | intstack.episoin); // select send ep
	USB_FAddr = intstack.addr;						// select send addr
	USB_IntrOutel |= BIT(1);
	// kick start
	USB_OutCSR1 |= 0x20; // request pkt,send a in packet
	USB_Index = epindex;
}

void kick_uvc_control()
{
	u8 epindex = USB_Index;

	U20E1RADR = (u32)ep1_fifo;
	USB_Index = 1; // select use ep
	//	printf("ISOTX_EP=%d\n",ISOTX_EP);
	USB_RxType = (TT_INTERRUPT | intstack.episoin); // select send ep
	USB_FAddr = intstack.addr;
	// select send addr
	USB_OutCSR1 = 0x30;
	USB_Index = epindex;
}

void usb_host_speed(u8 speed)
{
#define FULL_SPEED 0
#define HIGH_SPEED 1

	u8 usb_type = HIGH_SPEED;
	// u8 usb_type = FULL_SPEED;
	if (usb_type == HIGH_SPEED)
		USB_Power = 0x28;
	else
		USB_Power = 0x08;

	delay_5ms(500);

	if (usb_type == HIGH_SPEED)
		USB_Power = 0x20;
	else
		USB_Power = 0x00;

	delay_5ms(200);
}

u8 start_data(dev_mesg *usb_dev)
{

	// printf("he :%d , ce :%d ,addr %d \n", usb_dev->host_ep,usb_dev->client_ep,usb_dev->addr);
	volatile __sfr unsigned long *addr;
	switch (usb_dev->host_ep)
	{
	case 1:
		addr = &U20E1RADR;
		break;
	case 2:
		addr = &U20E2RADR;
		break;
	case 3:
		addr = &U20E3RADR;
		break;
	case 4:
		addr = &U20E4RADR;
		break;
	}
	// kick
	*addr = (u32)usb_dev->data_fifo;
	USB_Index = usb_dev->host_ep;						   // select use ep
	USB_RxType = (usb_dev->usb_type | usb_dev->client_ep); // select send ep
	USB_FAddr = usb_dev->addr;							   // select send addr

	// kick start
	USB_IntrOutel |= BIT(usb_dev->client_ep); // isr en
	USB_OutCSR1 |= 0x20;					  // request pkt,send a in packet
	return 0;
}

void kick_usb(dev_mesg *usb_dev)
{
	volatile __sfr unsigned long *addr;
	switch (usb_dev->host_ep)
	{
	case 1:
		addr = &U20E1RADR;
		break;
	case 2:
		addr = &U20E2RADR;
		break;
	case 3:
		addr = &U20E3RADR;
		break;
	case 4:
		addr = &U20E4RADR;
		break;
	}
	USB_Index = usb_dev->host_ep; // select use ep
	*addr = (u32)usb_dev->data_fifo;

	//	printf("ISOTX_EP=%d\n",ISOTX_EP);
	// USB_RxType = (usb_dev->usb_type|usb_dev->client_ep);	//select send ep
	USB_RxType = ((1 << 4) | usb_dev->client_ep); // select send ep
	USB_FAddr = usb_dev->addr;
	// select send addr
	USB_OutCSR1 = 0x10;
	USB_OutCSR1 = 0x30;
}

void usb_ep_init(dev_mesg *usb_dev, u8 in_ep)
{

	(USB_Index) = usb_dev->host_ep; // mscstack.epbulkin;//EP_BULKTX;

	if (in_ep)
	{
		(USB_InCSR1) = BIT(6) | BIT(3);
		(USB_InCSR2) = 0;
		(USB_InMaxPl) = (usb_dev->max_pack_len & 0xff) + 4;
		(USB_InMaxPh) = (usb_dev->max_pack_len >> 8) & 0xff;
	}
	else
	{
		(USB_OutCSR1) = BIT(7) | BIT(4);
		(USB_OutCSR2) = 0;
		(USB_OutMaxPl) = (usb_dev->max_pack_len & 0xff) + 4;
		(USB_OutMaxPh) = (usb_dev->max_pack_len >> 8) & 0xff;
	}
	/*

	(USB_Index)= usb_dev->host_ep;//mscstack.epbulkin;//EP_BULKTX;
	(USB_InCSR1)=BIT(6) | BIT(3);
	(USB_InCSR2)=0;
	(USB_InMaxPl)=4;
	(USB_InMaxPh)=0x0c;//
	(USB_OutCSR1)=BIT(7) | BIT(4);
	(USB_OutCSR2) = 0;
	(USB_OutMaxPl)=4;
	(USB_OutMaxPh)=0x0c;//

	(USB_Index)= usb_dev->host_ep;//mscstack.epbulkout;//EP_BULKTX;
	(USB_InCSR1)=BIT(6) | BIT(3);
	(USB_InCSR2)=0;
	(USB_InMaxPl)=4;
	(USB_InMaxPh)=0x0c;//
	(USB_OutCSR1)=BIT(7) | BIT(4);
	(USB_OutCSR2) = 0;
	(USB_OutMaxPl)=4;
	(USB_OutMaxPh)=0x0c;
*/
}

void enable_ep_irq(dev_mesg *usb_dev)
{
	// USB_Index = usb_dev->host_ep;//select use ep
	USB_IntrOutel |= BIT(usb_dev->host_ep);
	// USB_OutCSR1 |= 0x20;
}

u32 usb_IntrUSB()
{
	u32 reg = USB_IntrUSB;
	return reg;
}

u32 usb_IntrOutl()
{
	u32 introul = USB_IntrOutl;
	return introul;
}

u32 usb_IntrInl()
{
	u32 intrinl = USB_IntrInl;
	return intrinl;
}
u32 usb_IntrInh()
{
	u32 intrinh = USB_IntrInh;
	return intrinh;
}

u32 usb_outcsr1()
{
	u32 outcsr1 = USB_OutCSR1;
	return outcsr1;
}
u32 is_usb_data_error(dev_mesg *usb_dev)
{
	USB_Index = usb_dev->host_ep;
	u32 outcsr1 = USB_OutCSR1;
	u32 outcsr2 = USB_OutCSR2;
	if (outcsr1 & 0x04)
	{
		printf("usb data error \r\n");
		USB_OutCSR1 = 0;
		return 1;
	}
	if (outcsr1 & 0x08)
	{
		printf("usb iso data error \r\n");
		USB_OutCSR1 = 0;
		return 1;
	}
	if (outcsr1 & 0x40)
	{
		printf("usb stall \r\n");
		return 1;
	}
	if (outcsr2 & 0x01)
	{
		printf("incomplete iso packe \n");
		return 1;
	}

	if (outcsr1 & 0x01) // Get one packet
	{
		return 0; // OK
	}
	return 2;
}
u32 usb_data_len(dev_mesg *usb_dev)
{

	u32 len;
	USB_Index = usb_dev->host_ep; // select use ep
	len = (USB_OutCount2 << 8) + USB_OutCount1;
	return len;
}
