
#ifndef __WLAN_API_H__
#define __WLAN_API_H__

#ifdef __cplusplus
extern "C" {
#endif


#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPS_ENABLED     0x10000000
#define WEP_ENABLED     0x0001
#define TKIP_ENABLED    0x0002
#define AES_ENABLED     0x0004
#define WSEC_SWFLAG     0x0008


/**
 * Tells the driver what extra actions need to be done at initialization
 * will be used in COB  currently
 */
enum mcu_config_type_t
{
    MCU_CONFIG_POWER_CAL     = 0x01,  /* bit 0 */
    MCU_CONFIG_FREQUENCY_CAL = 0x02,  /* bit 1 */
    MCU_CONFIG_PERIODIC_LCK  = 0x04   /* bit 2 */
};

/**
 * Tells the driver which actions to perform
 * and indicates the entry parameters for each action
 */
struct mcu_config_t
{
    unsigned long config_mask;
    const char *power_cal_data;
    unsigned long freq_value;
    unsigned long lck_switch;
};

enum wlan_app_security_t
{
    WLAN_APP_SECURITY_OPEN           = 0,                                                    /* Open security                           */

    WLAN_APP_SECURITY_WEP_PSK        = WEP_ENABLED,                                          /* WEP Security with open authentication   */
    WLAN_APP_SECURITY_WEP_SHARED     = (SHARED_ENABLED   | WEP_ENABLED),                     /* WEP Security with shared authentication */

    WLAN_APP_SECURITY_WPA_TKIP_PSK   = (WPA_SECURITY     | TKIP_ENABLED),                    /* WPA Security with TKIP                  */
    WLAN_APP_SECURITY_WPA_AES_PSK    = (WPA_SECURITY     | AES_ENABLED),                     /* WPA Security with AES                   */
    WLAN_APP_SECURITY_WPA_MIXED_PSK  = (WPA_SECURITY     | AES_ENABLED   | TKIP_ENABLED),    /* WPA Security with AES & TKIP           */

    WLAN_APP_SECURITY_WPA2_AES_PSK   = (WPA2_SECURITY    | AES_ENABLED),                     /* WPA2 Security with AES                  */
    WLAN_APP_SECURITY_WPA2_TKIP_PSK  = (WPA2_SECURITY    | TKIP_ENABLED),                    /* WPA2 Security with TKIP                 */
    WLAN_APP_SECURITY_WPA2_MIXED_PSK = (WPA2_SECURITY    | AES_ENABLED   | TKIP_ENABLED),    /* WPA2 Security with AES & TKIP           */

    WLAN_APP_SECURITY_WPS_OPEN       = WPS_ENABLED,                                          /* WPS with open security                  */
    WLAN_APP_SECURITY_WPS_SECURE     = (WPS_ENABLED      | AES_ENABLED),                     /* WPS with AES security                   */
    WLAN_APP_SECURITY_UNKNOWN        = -1,                                                   /* May be returned by scan function if security is unknown.
                                                                                    Do not pass this to the join function! */
};

/**
 * store the detail infos of scan result
 */
struct wlan_app_info_t
{
    /* ssid */
    unsigned char ssid[33];
    unsigned char ssid_len;

    /* ssid hidden */
    unsigned char hidden;
    /* hwaddr */
    unsigned char bssid[6];
    /* security type */
    enum wlan_app_security_t sec;
    /* radio channel */
    unsigned short channel;
    /* 2.4G/5G */
    unsigned long band; //0 = 5G, 1 = 2.4G, 0x7fffffff = unknown
    /* maximal data rate */
    unsigned long datarate;
    /* signal strength */
    char rssi;
};

/**
 * A set of callback hooks provided to user
 */
struct user_notify_hook_t
{
    void (*hook_sta_port_authorized)(unsigned char *pMacAddr);  /* in AP mode, when a sta auth sucess */
    void (*hook_sta_associated)(unsigned char *pMacAddr);       /* in AP mode, when a sta assoc sucess */
    void (*hook_sta_disassociated)(unsigned char *pMacAddr);    /* in AP mode, when a sta disassoc */
    void (*hook_sta_disconnect)(unsigned char *pMacAddr);       /* in AP mode, when a sta disconneted */
    void (*hook_sta_ap_ready)(void);                            /* start ap successfully */
    void (*hook_link2ap_connectd)(char *pIntfName, int status); /* in STA mode, connected:1; disconnected:0 */
};

/**
 * @brief start driver and make the hw and sw be ready
 *
 * @return 0:succ, others:fail
 */
int wlan_app_init(void);

/**
 * @brief power down hw, stop each work and release related sw resources
 *
 * @return ignore
 */
int wlan_app_exit(void);

/**
 * @brief set the mcu config
 *
 * @param config: the mcu config
 * @return 0:succ; others:fail
 */
int wlan_app_user_special_config(struct mcu_config_t *config);

/**
 * @brief try to get a net interface resource which is bind to the input wlan network card name
 *
 * @param name: wlan network card name, like "wlan0"
 * @return a net interface resource, It typically points to a Layer 3 network interface, like "struct netif*"
 */
void *wlan_app_netif_get(const char *name);


/**
 * @brief try to start a full-channel scanning
 *
 * @param name: wlan network card name, like "wlan0"
 * @param dump_scan_result: if you want to wait the scan resutls;
                            0: dont wait, than you can get the result by func 'wlan_app_scan_result'
                            others:wait, the scan results will be dumped,
                                and then you will get nothing by call wlan_app_scan_result
 * @return 0:succ; others:fail
 */
int wlan_app_scan(const char *name, int dump_scan_result);

/**
 * @brief get the scan result after calling the func 'wlan_app_scan' (note:the dump_scan_result must be 0)
 *
 * @param name: wlan network card name, like "wlan0"
 * @param result: to save one scan result, you can call the func until it return 0 if you want to get all results
 * @return 0:get nothing; 1:get a valid result
 */
int wlan_app_scan_result(const char *name, struct wlan_app_info_t *result);

/**
 * @brief try to connect to a ap
 *
 * @param name: wlan network card name, like "wlan0"
 * @param ssid: ssid of the target ap
 * @param passwd: password of the target bss, if the bss is open, input NULL(0)
 * @return 0:succ; others:fail
 */
int wlan_app_join(const char *name, const char *ssid, const char *passwd);

/**
 * @brief try to disconnect to a ap
 *
 * @param name: wlan network card name, like "wlan0"
 * @return 0:succ; others:fail
 */
int wlan_app_disc(const char *name);

/**
 * @brief try to start ap
 *
 * @param name: wlan network card name, like "wlan0"
 * @param channel: 2g channel only (1~13)
 * @param ssid: ssid of the target ap
 * @param passwd: password of the target bss, if the bss is open, input NULL(0)
 * @return 0:succ; others:fail
 */
int wlan_app_ap_start(const char *name, unsigned short channel, const char *ssid, const char *passwd);

/**
 * @brief try to stop ap
 *
 * @param name: wlan network card name, like "wlan0"
 * @return 0:succ; others:fail
 */
int wlan_app_ap_stop(const char *name);

/**
 * @brief register some interested cb by user
 *
 * @param phook: ref@struct user_notify_hook_t
 * @note called before wlan_app_init
 * @return 0:succ; others:fail
 */
int wlan_app_register_hook(struct user_notify_hook_t *phook);

/**
 * @brief AP MODE:  get the number of STAs connected to the AP
 *        STA MODE: 0:disconnected;  1:connected
 * @param name: wlan network card name, like "wlan0"
 * @return  user count
 */
int wlan_app_get_wdn_cnt(const char *name);

/**
 * @brief if work on AP mode, get the max number of STAs that can be connected at the same time
 *
 * @param name: wlan network card name, like "wlan0"
 * @return  max STAs number
 */
int wlan_app_ap_userlimit_get(void);

/**
 * @brief if work on AP mode, limit the max number of STAs that can be connected at the same time
 *
 * @param wdn_num: max STAs number
 * @note called before wlan_app_init
 * @return 0:succ; others:fail
 */
int wlan_app_ap_userlimit_set(int wdn_num);

/**
 * @brief get connection status, if the ap node is connected in sta mode, or if the ap mode is started normally in ap mode
 *
 * @param ifname: wlan network card name, like "wlan0"
 * @return 0: disconnected; 1: connected
 */
int wlan_app_get_connect_status(const char *ifname);

/**
 * @brief get the connected AP info. this api only supports STA mode.
 *
 * @param ifname: wlan network card name, like "wlan0"
 * @param wdn_app_info: the connected ap info
 * @return 0:succ; others:fail
 */
int wlan_app_get_wdn_info(const char *ifname, struct wlan_app_info_t *wdn_app_info);

/**
 * @brief get the mac address.
 *
 * @param ifname: wlan network card name, like "wlan0"
 * @param mac_addr: mac address data
 * @return 0:succ; others:fail
 */
int wlan_get_mac_address(const char *ifname, unsigned char* mac_addr);

/**
 * @brief set the mac address
 *
 * @param ifname: network interface name, like "wlan0"
 * @param mac_addr: mac address data, length is 6 bytes, like {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}
 * @note called before wlan_app_init
 * @return 0:succ; others:fail
 */
int wlan_set_mac_address(const char *ifname, const unsigned char* mac_addr);

/**
 * @brief Users set the thread priority
 * @param priority: sdio irq thread priority
 * @note configMIN_PRIORITIES+1~configMAX_PRIORITIES-1,called before wlan_app_init
 * @return 0:succ; others:fail
 */
int wlan_set_thread_priority(int priority);

/**
 * @brief enter wifi low power status to save power
 *
 * @param ifname: network interface name, like "wlan0"
 * @note in low power statue, wifi function is disabled
 * @return 0:succ; others:fail
 */
int wlan_entry_low_power(const char *ifname);

/**
 * @brief exit wifi low power status, and wifi will work in normal status
 *
 * @param ifname: network interface name, like "wlan0"
 * @note  wifi function will work normally
 * @return 0:succ; others:fail
 */
int wlan_exit_low_power(const char *ifname);

#ifdef __cplusplus
}
#endif

#endif
