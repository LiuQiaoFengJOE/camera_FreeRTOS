#include "typedef.h"
#include "board_config.h"

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030)
#include <rtos.h>
#include <porting.h>
#include "app/cli/cli.h"
#endif

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV)
#include "rtl8189ftv/include/wifi_constants.h"
#endif

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_SV6030)
void customer_cmd_cfg(int isUdp)
{
    int i = 0;

    /*ÖØ·¢°üÊýÁ¿µ÷½Ú£¬"iw src 1 4" ,ÖØ´«4´Î*/
    if (isUdp)
    {
        char config[][64] = {
#if (PHY_SUPPORT_HT == PHY_NB_MODE)
            //"iw txduty 1 60 40"
            "rc mask 0xfff",
            "rc per 20 40",
            "rc resent 0",
            "rc direct 1",
            "ampdu tx 1",
            "ampdu maintry 1",
            "ampdu maxretry 3",
            "ampdu lastretryb 1",
            "ampdu lastbrate 3",
            "ampdu lastbonce 0",
            "iw src 1 8",
            "iw src 2 8",
#else
            "rc mask 0x55c",
            "rc per 12 27",
            "rc resent 0",
#endif
            "cfg ts 1",
            "cfg tx_retry_cnt 200",
            "cfg txtick 1",

            "w ce0071bc 90908080",

            "w ce004010 3f304905",
            "w ce000004 217f",
            "w ce0043fc 104e5",
            "w ce0020a0 160400",
            "w ce002008 20380050",

            "w ce0040c0 7f000280",
        };

        for (i = 0; i < sizeof(config) / sizeof(config[0]); i++)
        {
            printf("cfg: %s\r\n", config[i]);
            Cli_RunCmd(config[i]);
        }
    }
    else
    {
        char config[][32] = {
            "rc mask 0x1e0",
            "rc per 20 40",
            "rc resent 1",
            "rc direct 1",
            "ampdu tx 1",
            "ampdu maintry 1",
            "ampdu maxretry 3",
            "ampdu lastbrate 3",
            "ampdu lastbonce 0",
            "iw src 1 4",
            "iw src 2 4",
            "cfg ts 1",
            "cfg tx_retry_cnt 200",
            "cfg txtick 1",
            "w ce0071bc 90908080",
            "w ce004010 3f304905",
            "w ce000004 217f",

        };

        for (i = 0; i < sizeof(config) / sizeof(config[0]); i++)
        {
            printf("cfg: %s\r\n", config[i]);
            Cli_RunCmd(config[i]);
        }
    }

    printf("customer cmd cfg finish (%s) !!\r\n", isUdp ? "udp" : "tcp");
}
#endif

#if (WIFI_SUPPORT_VERSION == BOARD_WIFI_RTL8189FTV)
/*------------------------use before wifi_start
_ap----------------------------------------------------
*/
int rltk_wlan_wireless_mode(unsigned char mode);
int wifi_set_network_mode(rtw_network_mode_t mode)
{
    if ((mode == RTW_NETWORK_B) || (mode == RTW_NETWORK_BG) || (mode == RTW_NETWORK_BGN))
        return rltk_wlan_wireless_mode((unsigned char)mode);

    return -1;
}
#endif
