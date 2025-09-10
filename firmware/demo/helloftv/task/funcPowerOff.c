#include "app.h"
#include"uiWin.h"

void startPoweroff(void)
{
	functionStart(FUNCTION_POWER_OFF,0);
}
void power_off2()
{
	ax32xx_WKI1mode(1);		   // 设置上升沿触发
	ax32xx_WKOEnable(0);	   // 关闭wki
	ax32xx_WKI1CleanPending(); // 清pending
	while(1)
	{
	}
}

void funcPowerOffOpen(uint32 arg1)
{
	userConfigSave();
	lcdc_osd_enable(0, 0);
	lcdc_osd_enable(1, 0);
	logoShow(RES_ICON_POWER_OFF);
	dacPlaySound(RES_MUSIC_POWER_OFF,getSoundVol());
	printf("display_photo finish\r\n");
	//playmusic_select(RESID_PCM); // 开机播放开机声音
	osal_task_sleep(1000);		 // 暂时在这里加上延时
	while(dacIsPlaying())
	{
		osal_task_sleep(5);
	}
	lcd_bk_ctrl(0);
	ax32xx_WKI1mode(1); // 设置上升沿触发
	ax32xx_WKOEnable(0); // 关闭wki
	printf("ax32xx_WKI1Read:%d\n", ax32xx_WKI1Read());
	printf("ax32xx_WKI1Read:%d\n", ax32xx_WKI1Read());
	ax32xx_WKI1CleanPending(); // 清pending
	while(1)
	{
	}
}

sysFunction functionPowerOff=
{
	"powerOff",
	NULL,
	funcPowerOffOpen,
	NULL,
};


