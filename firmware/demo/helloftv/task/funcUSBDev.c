#include "app.h"
#include"uiWin.h"
#include "fatfs/ff.h"

int usbDevMode(void)
{
	if(functionCurrent()==FUNCTION_USB_DEVICE)
		return 1;
	return 0;
}
void funcUSBDevOpen(uint32 arg1)
{
	u8 mode;
	//f_mount(NULL, _DATDSK_, 1);
	wmm_trans_mode(&workmode, WM_USB);
	mode=1;
	if(arg1==R_ID_STR_SET_USBMASS)
	{
		mode=0;
		logoShow(RES_ICON_USB_MODE);
	}
	else
	{
		mode=1;
		logoShow(RES_ICON_PCCAM_MODE);
	}
	hal_usbdInit(mode);
	uiOpenWindow(&usbDevWindow,0);
	lcdOsdEn(1);
	//init_usb_dev_mod();
}


void funcUSBDevClose(uint32 arg)
{
	sendMsgAndWait(OS_TASK_USB_DEV, TASK_MSG_UNINIT,0);
	//remount_sd();
	fileScan();
}
sysFunction functionUSBDev=
{
	"usbDev",
	NULL,
	funcUSBDevOpen,
	funcUSBDevClose,
};


