
/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
#include "usb_dev.h"
#include "diskio/sdiodisk.h"



extern s32 usb_device_check(void);


bool usb_uvciso_kick(u32 adr, u16 len)
{
	return _usb_iso_kick(__UVC_EPX__,adr,len);
}
bool usb_uvciso_wait_txdown(void)
{
	return _iso_wait_txdown(__UVC_EPX__);
}
bool usb_uaciso_kick(u32 adr, u16 len)
{
	//return _usb_iso_kick(UAC_EPX,adr,len);
}
bool usb_uaciso_wait_txdown(void)
{
	//return _iso_wait_txdown(UAC_EPX);
}

bool usb_bulk_tx(u8 epx, u32 adr, u16 len)//
{
	return _usb_bulk_tx(epx,adr,len,__MSC_SIZE__);
}
bool usb_bulk_rx(u8 epx, u32 adr, u16 len)
{
	return _usb_bulk_rx(epx,adr,len,__MSC_SIZE__);
}
void uac_epx_cfg(void);


void hal_usbdReset(void)
{
	
	enum_epx_cfg(0);
	msc_epx_cfg();
	uvc_epx_cfg();
	uac_epx_cfg();
}

void uac_on(void)
{
	//hal_auadcStartForUAC(16000,100);
}
void uac_off(void)
{
	//hal_auadcStop();
}
u32 hal_sdExist(void)
{
	if(get_sd_status() == SD_OFFLINE)
		return 0;
	return 1;
}
void sdFuncNULL(void)
{
}
extern SD_INFO g_stcSDInfo;
extern sdcard_dev sdc_dev;

u32 get_tf_cap()
{
	return g_stcSDInfo.dwCap;
}
void sd_scsi_read2(u8 *buf, u32 lba, u32 count)
{
	rtos_sd_read(&sdc_dev, buf, lba, count);
}
void sd_scsi_write2(u8 *buf, u32 lba, u32 count)
{
	rtos_sd_write(&sdc_dev, buf, lba, count);
}
void usb_fun_reg(void)
{
    ax32xx_usbCallbackRegister(USB_IRQ_RESET,hal_usbdReset);
	ax32xx_usbCallbackRegister(USB_IRQ_EP0,usb_ep0_hal);

	//reg for void usb_burst_reset(void)
//	fp_enum_epx_cfg= enum_epx_cfg;
//	fp_msc_epx_cfg = msc_epx_cfg;
//	fp_uvc_epx_cfg = uvc_epx_cfg;
//	fp_uac_epx_cfg = uac_epx_cfg;
	
	//reg for void usb_isr(void)
	
	
//-------------mass storage---------------------	
	//reg for void msc_epx_cfg(void)
	fp_disk_rd = sd_scsi_read2;
	fp_disk_wr = sd_scsi_write2;
	fp_usb_bulk_tx = usb_bulk_tx;
	fp_usb_bulk_rx = usb_bulk_rx;	
	fp_disk_online = hal_sdExist; 
	fp_disk_cap = get_tf_cap;
	fp_disk_free = sdFuncNULL;
	fp_disk_busy = sdFuncNULL;
	fp_disk_stop = sdFuncNULL;
//-------------uvc-------------------------------
	//reg for void uvc_epx_cfg(void)
	fp_uvc_on_hal = uvc_on;//mjp_actech_as_jpguvc;
	fp_uvc_off_hal = uvc_off;//mjp_stop;
	fp_uac_on_hal = uac_start;//uac_on;//mjp_actech_as_jpguvc;
	fp_uac_off_hal = uac_stop;//uac_off;//mjp_stop;
}
extern void usb_dev_enum_reg(u8 tpe);
extern void usbDevIRQHanlder(unsigned int irq_num, void *arg, void *reg);
void hal_usbdInit(u8 mod)
{
	usb_dev_enum_reg(mod);
	usb_fun_reg();
	usb_phyreset_as_typeb();
	usb_burst_reset();
	hal_usbdReset();
	cb_ram_func_init();
	//ax32xx_intEnable(IRQ_USB20,1);
	irq_register(IRQ_USB20, 0, usbDevIRQHanlder, NULL);
}
/*******************************************************************************
* Function Name  : hal_usbdunInit
* Description    : hal layer.usb device uninitial 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdUninit(void)
{
	uac_off();
	uvc_off();
	irq_unregister(IRQ_USB20);
	/*
	delay_5ms(900);
	memset((u8 *)&usb_dev,0,sizeof(usb_dev));
	memset((u8 *)&usb_rqu,0,sizeof(usb_rqu));
	*/
}

void hal_usbdEnable(u8 en)
{
	ax32xx_intEnable(IRQ_USB20,en);	
}

s32 hal_usbdCheck(void)
{
	return usb_device_check();
}

typedef struct
{
	bool astern11;
	bool astern2;
}BACKREC_ADPT;

BACKREC_ADPT backrecadpt; 
bool api_astern11_status(void)
{
	return backrecadpt.astern11;
}
void api_astern11_set_status(bool sta)
{
	backrecadpt.astern11 = sta;
}
static u8 reverse11_sta;
void api_backsta11_linking(void)
{
	static s8 sync11;
	if(getUsbCameraConnect()){
		/**/
		if(sync11 == false){
			sync11 = true;
			return;
		}
		uhost_backsta_cmd();
		if(reverse11_sta != check_backsta())
		{
			if(check_backsta() == true)
			{
				backrecadpt.astern11 = true;
			}
			else
			{
				backrecadpt.astern11 = false;
			}			
		}
		reverse11_sta = check_backsta();
		
	}
	else{
		reverse11_sta = -1;
		sync11 = false;	
	}
}


bool api_astern2_status(void)
{
	return backrecadpt.astern2;
}
void api_astern2_set_status(bool sta)
{
	backrecadpt.astern2 = sta;
}
static u8 reverse_sta;
void api_backsta2_linking(void)
{
	static s8 sync2;
	if(getUsb2CameraConnect()){
		/**/
		if(sync2 == false){
			sync2 = true;
			return;
		}
		uhost2_backsta_cmd();
		if(reverse_sta != check2_backsta())
		{
			if(check2_backsta() == true)
			{
				backrecadpt.astern2 = true;
			}
			else
			{
				backrecadpt.astern2 = false;
			}			
		}
		reverse_sta = check2_backsta();
		
	}
	else{
		reverse_sta = -1;
		sync2 = false;	
	}
}
