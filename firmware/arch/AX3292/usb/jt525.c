#include "typedef.h"
#include "drv/usb_driver/usb_host/hal_usb_host_enum.h"

const u32 exmand_pfun_cfg_jt525[][2]={ 
		{0x00000724,24000000},//720p mclk
		{0x00000710,24000000},//480p mclk
};

void usensor_exsrc_update_jt525(void) 
{
	exmand.code = 0;
	exmand.clen = 0;
	exmand.inram = 0;
	exmand.pfun = (	u32 (*)[2])exmand_pfun_cfg_jt525;
	exmand.npfun= sizeof(exmand_pfun_cfg_jt525)/8;
	//exmand.pfun =0;
	//exmand.npfun= 0;

	exmand.iensor = 0;
	exmand.psensor = sensor_cmd_tab[exmand.iensor];
}
void usensor2_exsrc_update_jt525(void) 
{
	usb2Exmand.code = 0;
	usb2Exmand.clen = 0;
	usb2Exmand.inram = 0;
	usb2Exmand.pfun = (	u32 (*)[2])exmand_pfun_cfg_jt525;
	usb2Exmand.npfun= sizeof(exmand_pfun_cfg_jt525)/8;
	//exmand.pfun =0;
	//exmand.npfun= 0;

	usb2Exmand.iensor = 0;
	usb2Exmand.psensor = sensor_cmd_tab[usb2Exmand.iensor];
}


