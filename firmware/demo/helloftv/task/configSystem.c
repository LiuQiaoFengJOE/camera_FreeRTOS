#include "app.h"

extern void winUpdateAllResId(void);
int configLanguage(void)
{
	static uint32 lastSetting;
	fontSetLanguage(configValue2Int(CONFIG_ID_LANGUAGE));
	if(configGet(CONFIG_ID_LANGUAGE)!=lastSetting)
	{
		lastSetting=configGet(CONFIG_ID_LANGUAGE);
		winUpdateAllResId();
	}
	return 0;
}
void configSys(uint32 configId)
{
	INT32U value = configValue2Int(configId);
    switch(configId)
	{
		case CONFIG_ID_KEYSOUND:
			  SysCtrl.f_keysound = value;break;	   
		case CONFIG_ID_RESOLUTION:

			   break;
		case CONFIG_ID_PRESLUTION:	
			   break;
		case CONFIG_ID_MOTIONDECTION:
			   break;
		case CONFIG_ID_PARKMODE:
			   break;
		case CONFIG_ID_AUDIOREC:
			   break;
	    case CONFIG_ID_TIMESTAMP	: 
		case CONFIG_ID_PHOTOTIMESTAMP	: 
			   break;
	    case CONFIG_ID_LOOPTIME : 
			  break;
	    case CONFIG_ID_LANGUAGE:	
			  configLanguage();  
			  break;
		case CONFIG_ID_GSENSOR:
			  break;
	    case CONFIG_ID_EV:
			  sensor_compensate_exp_set(value);
			  break;
		case CONFIG_ID_WBLANCE:
			  awb_scene_mod_set(value);
			  break;
		case CONFIG_ID_FREQUNCY:
			  //sensor_frame_rate_adapt(value,25);
			  break;
		case CONFIG_ID_IR_LED:
			 break;
		case CONFIG_ID_SHARPNESS:
			 break;
		case CONFIG_ID_ISO:
			 break;
		case CONFIG_ID_DEBOUNCE:
			 break;
		case CONFIG_ID_DATEFORMAT:
			 break;	
		case CONFIG_ID_PHOTOQUALITY:
			 break;		
		case CONFIG_ID_WDR:
			 break;		

	}	/**/
}



void configSystem(void)
{
	uint32 i;
	for(i=0;i<CONFIG_ID_MAX;i++)
		configSys(i);
}



