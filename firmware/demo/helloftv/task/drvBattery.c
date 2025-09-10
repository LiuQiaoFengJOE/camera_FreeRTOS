#include "task/app.h"
#include "gpio_ops/gpio.h"

#define ADC_BAT_CH 		ADC_CH_MVOUT	// rtc ch
#define ADC_BGP_CH 		ADC_CH_BGOP

#define BAT_LEV_MAX 5
#define BAT_AVG_CNT 10
#define BATTERY_INV 10
static u32 vbg_param;
static int lastBatLev;
static int lastStableBatLev;
const static u16 batteryValueTable[BAT_LEV_MAX] = {3400, 3500, 3700, 3850, 0xffff};
#if 1
u32 ax32xx_vddrtcCalculate(u32 Artc, u32 Abg)
{
	u32 Vrtc = 0;
	Vrtc = (Artc * vbg_param) * 2 / Abg; // vbg_param
	return Vrtc;
}
#endif
static int curBatteryADC=0;
int getCurBatteryADC(void)
{
	return curBatteryADC;
}
int getCurBatteryLevel(void)
{
	return lastBatLev;
}

//0,3?¦Ì??D; 1,¨°?3??¨²
int getChargeStatus()
{
#if BOARD_TYPE == BOARD_TYPE_WIFI_PRINTER
	return hal_gpioRead(GPIO_BAT_CHARG_CH, GPIO_BAT_CHARG_PIN);
#endif
	return 0;
}

int batteryInit(void)
{
	u16 batValue=0,bgpValue=0;
	int value,i;

	u8 vbg_ofs = (*(u32 *)(0x5000 - 140 + 0x30 + 4) & 0x7f);
	if (vbg_ofs != 0)
		vbg_param = vbg_ofs + 737;
	else
		vbg_param = 800;
	
	ax32xx_rtcBatDecEnable(1);
	i = 3;
    while(i--)
    {
        batValue += hal_adcGetChannel(ADC_BAT_CH);
        bgpValue += hal_adcGetChannel(ADC_BGP_CH);
    }
	if(bgpValue==0)
		value = 0;
	else
		value = ax32xx_vddrtcCalculate(batValue,bgpValue);
	for(i=0;i<BAT_LEV_MAX;i++)
	{
		if(value<=batteryValueTable[i])
			break;
	}
	SysCtrl.battery=lastStableBatLev=lastBatLev=i;
	//printf("battery init:%d\n",SysCtrl.battery);
	if(value<batteryValueTable[0])
	{
		if(0==getUsbConnect())
		{
			printf("battery low,need power off:%d\n",value);
			power_off2();
		    return -1;
		}
	}
	return 0;
}

int getBatteryLevel(void)
{
	static u8 step = 0;
	static u32 batValue=0,bgpValue=0;
	static u32 lastTime = 0;
	int i,value;
	
	batValue += hal_adcGetChannel(ADC_BAT_CH);
	bgpValue += hal_adcGetChannel(ADC_BGP_CH);
	step++;
	if(step>=BAT_AVG_CNT)
	{
		if(bgpValue==0)
			value = 0;
        else
            value = ax32xx_vddrtcCalculate(batValue,bgpValue);

		for(i=0;i<BAT_LEV_MAX;i++)
		{
			if(value<=batteryValueTable[i])
				break;
		}
		if(lastBatLev!=i)
		{
			if(lastBatLev==0xff)
				lastBatLev = i;
			else
			{
				if(lastBatLev>i)
				{
					if(value<batteryValueTable[i]-BATTERY_INV)
						lastBatLev=i;
				}
				else
				{
					if(value>batteryValueTable[lastBatLev]+BATTERY_INV)
						lastBatLev=i;
				}
			}
		}
		curBatteryADC=value;
		if(os_time_get() - lastTime > 1000)
		{
			lastTime=os_time_get();
			//printf("bat:%d,lev:%d\n",value,lastBatLev);
		}
		step = 0;
		bgpValue=0;
		batValue=0;
	}
	return lastBatLev;
}

int batteryDet(void)
{
	static int checkStable=0;
	int batLev;
    batLev = getBatteryLevel();  
	if(lastStableBatLev != batLev&&checkStable==0)
		checkStable = 5;
	else if(lastStableBatLev == batLev)
	{
		checkStable=0;
		return ;
	}
	if(checkStable>0)
		checkStable--;

	if(checkStable == 0)
	{
		if(lastStableBatLev != batLev)
		{
			SysCtrl.battery=lastStableBatLev=batLev;
			sendMsgNonblock(OS_TASK_UI,SYS_EVENT_BAT, batLev);
		}

		if(batLev == 0)
			functionStart(FUNCTION_POWER_OFF,0);
	}
}

u8 getBatLev(void)
{
	return lastStableBatLev;
}







