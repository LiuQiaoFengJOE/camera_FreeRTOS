#ifndef __BTCOM_USER__
#define __BTCOM_USER__

#define AT_RESET            "AT+RESET\n"          //¸´Î»
#define AT_FACTORYRESET     "AT+FACTORYRESET\n"   //»Ö¸´³ö³§ÉèÖÃ
#define AT_ATMODE           "AT+ATMODE\n"         //½øÈëATÄ£Ê½
//----check
#define AT_CHECK_BTIEEENAME "AT+IEEENAME?"      //²éÑ¯À¶ÑÀÄ£¿éµÄÀ¶ÑÀ´òÓ¡Éè±¸Ãû
#define AT_CHECK_BTPIN 	    "AT+PIN?\n"           //²éÑ¯À¶ÑÀPINÂë
#define AT_CHECK_BTNAME 	"AT+NAME?\n"          //²éÑ¯À¶ÑÀÄ£¿éµÄÀ¶ÑÀ¹ã²¥Ãû
#define AT_CHECK_BAUD 	    "AT+BAUD?\n"          //²éÑ¯À¶ÑÀµÄ´®¿Ú²¨ÌØÂÊ
#define AT_CHECKMODE 	    "AT+CHECKMODE"      //²éÑ¯À¶ÑÀÄ£Ê½
#define AT_CHECKLBDADDR 	"AT+LBDADDR?"       //²éÑ¯À¶ÑÀMACµØÖ·
//----set
#define AT_SET_BTNAME           "AT+NAME="      //ÉèÖÃÀ¶ÑÀÄ£¿éµÄÀ¶ÑÀ¹ã²¥Ãû
#define AT_SET_BTPIN            "AT+PIN="       //ÉèÖÃÀ¶ÑÀPINÂë
#define AT_SET_BAUD             "AT+BAUD="      //ÉèÖÃÀ¶ÑÀµÄ´®¿Ú²¨ÌØÂÊ
#define AT_SET_BTIEEENAME       "AT+IEEENAME="  //ÉèÖÃÀ¶ÑÀÄ£¿éµÄÀ¶ÑÀ´òÓ¡Éè±¸Ãû
typedef enum {
    BT_WIFI_CONFIG_SUCCESS = 0,                  // 配置成功：JSON 解析正常、配置保存成功、WiFi 连接就绪（WIFI_OPEN_SUCCESS 且 DHCP_SUCCESS）。
    BT_WIFI_CONFIG_ERR_JSON_PARSE = -1,          // JSON 解析失败：cJSON_Parse_or1k 返回 NULL，或格式无效（基于 cJSON_GetErrorPtr_or1k）。
    BT_WIFI_CONFIG_ERR_INVALID_PARAMS = -2,      // 无效参数：SSID 或密码缺失、类型非字符串，或长度超过缓冲区（基于 cJSON_GetObjectItem_or1k 和 strlen 检查）。
    BT_WIFI_CONFIG_ERR_WIFI_INIT_FAIL = -3,      // WiFi 初始化失败：wifi_init 返回非零值（可能因 SD 卡配置覆盖或模式无效）。
    BT_WIFI_CONFIG_ERR_START_WIFI_FAIL = -4,     // 启动 WiFi 失败：start_wifi 返回非零值（任务创建失败或模式不支持）。
    BT_WIFI_CONFIG_ERR_CONNECTION_TIMEOUT = -5,  // 连接超时：监控 get_wifi_statue 超过 max_tries（代码中为 60 次，每次 500ms），未达到 OPEN_SUCCESS。
    BT_WIFI_CONFIG_ERR_DHCP_FAIL = -6,           // DHCP 失败：WiFi 打开成功，但 DHCP 状态为 DHCP_ERR 或未达到 DHCPC_SUCCESS/DHCPD_SUCCESS。
    BT_WIFI_CONFIG_ERR_WIFI_OPEN_ERR = -7,       // WiFi 打开错误：wifi_status 为 WIFI_OPEN_ERR（基于芯片特定事件，如 ATBM_WIFI_DEASSOC_EVENT）。
    BT_WIFI_CONFIG_ERR_FILE_OPEN_FAIL = -8,      // 文件打开失败：f_open 返回非 FR_OK（SD 卡不可用或路径无效）。
    BT_WIFI_CONFIG_ERR_FILE_WRITE_FAIL = -9,     // 文件写入失败：f_write 返回非 FR_OK 或写入字节不匹配（FATFS 错误）。
    BT_WIFI_CONFIG_ERR_JSON_CREATE_FAIL = -10,   // JSON 创建失败：cJSON_CreateObject_or1k 或 cJSON_Print_or1k 返回 NULL。
    BT_WIFI_CONFIG_ERR_CLEANUP_FAIL = -11,       // 清理失败：连接失败后调用 wifi_close，但状态未恢复到 WIFI_CLOSE。
    BT_WIFI_CONFIG_ERR_UNKNOWN = -12,            // 未知错误：其他未捕获异常（如内存不足或系统中断）。
    BT_WIFI_CONFIG_ERR_INVALID_PASSWORD = -13    // 无效 WiFi 密码：长度不为 0 且小于 8 个字符。
} E_BT_WIFI_CONFIG_STATUS;

#if 0
/**
 * cmd£ºA3. return printer state to app
 *
 */
void Bt_Get_Printer_State(const u8 *data, int len);

/**
 * cmd£ºA4.  low 4 bit set print level :range 1~5.
 *
 */
void Bt_Set_Quality(const u8 *data, int len);

void Bt_Get_Print_Line_Data(const u8 *data, int len);

void Bt_Set_Lcd_Screen(const u8 *data, int len);

void Bt_Feed(const u8 *data, int len);

void Bt_Set_Printer_State(const u8 *data, int len);

void Bt_App_Ientity(const u8 *data, int len);

u16 reset_pic_data(void);

void encodeAction(char * Rc);

void string_to_hex_manual(const char *str,u8 *out_data);

void Bt_GetGrayZip_Data(const u8 *data, int len);

void Bt_Flow_Ctrl(u8 en);

void Bt_Set_Energer(const u8 *data, int len);

u8 *get_dot_tmp_add(void);

void Bt_Motor_Step_Time(const u8 *data, int len);

void Bt_WriteDevice_ID(u8 *data,int len);


void Bt_Printer_Set(const u8 *data, int len);

u32 Bt_Get_Printer_Moto_Speed(void);
#endif

#endif  // __BTCOM_USER__

