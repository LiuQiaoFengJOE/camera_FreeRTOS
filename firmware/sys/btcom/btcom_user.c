#include "task/app.h"
#include "btcom_user.h"
#include <string.h>
#include "drv/lcd/lcdframe.h"
#include "cjson/cjson.h"
// #include "net/atbm/api/atbm_wifi_driver_api.h"

extern int bt_configWifi(const u8 *wifiParm, u32 len);

#define RECVBUF_SIZE 1024          // 接收缓冲区大小
#define MIN_JSON_LEN 20           // 最小 JSON 长度
#define RECV_TIMEOUT_MS 10000     // 接收超时 10 秒

static u8 recvbuf[RECVBUF_SIZE];
static int recv_len = 0;
static uint32_t recv_start_time = 0; // 接收开始时间
static bool waiting_for_packet = false; // 是否在等待分包

void bt_txSend(const u8 *data, int len)
{
    const u8 *fragment = data;
    int frag_len = len;
    printf("len:%d\n", len);
	int j;
    for (j = 0; j < frag_len; j++) {
        ax32xx_uart1SendByte(fragment[j]);
    }
}

void BLESendWifiConfigstateCode(u8 status){
    u8 buf[] = {status};
    bt_txSend(buf, sizeof(buf));
}

void bt_rxRecv(u8 data)
{
    // 防止缓冲区溢出
    if (recv_len >= RECVBUF_SIZE - 1) {
        printf("[BTCOMTEST] Buffer overflow, resetting\n");
        recv_len = 0;
        waiting_for_packet = false;
        return;
    }

    // 追加数据
    recvbuf[recv_len++] = data;
    recvbuf[recv_len] = 0; // 确保字符串以 \0 结尾

    // 记录首次接收时间
    if (!waiting_for_packet) {
        waiting_for_packet = true;
        recv_start_time = os_time_get(); // 假设系统提供时间函数
    }
}

bool is_json_complete(const char* buf, int len) {
    int n;
    // 增强JSON完整性检查：检查长度、最小要求、以'{'开头，以'}'结尾
    if (len < 20 || buf[0] != '{' || buf[len - 1] != '}') {  // 假设最小JSON长度为20
        printf("[BTCOMTEST] Waiting for complete JSON, len:%d\n", len);
        // 添加缓冲区转储以诊断已接收内容
        if (len > 0) {
            printf("[BTCOMTEST] Waiting buffer dump:\n");
            for (n = 0; n < len; n++) {
                printf("%x ", (unsigned char)buf[n]);  // 使用unsigned char以避免符号问题
            }
            printf("\n");
        }
        return false; // 未满足基本结构
    }
    int brace_count = 0;    // 用于 {}
    int bracket_count = 0;  // 用于 []，丰富检查以支持数组
    bool in_string = false; // 跟踪是否在字符串内
    for (n = 0; n < len; n++) {
        char ch = buf[n];
        if (ch == '"' && !in_string) {
            in_string = true;  // 进入字符串
        } else if (ch == '"' && in_string) {
            // 检查转义：如果前一个是反斜杠，则仍为字符串内
            if (n > 0 && buf[n - 1] != '\\') {
                in_string = false;  // 退出字符串
            }
        }
        if (!in_string) {
            if (ch == '{') {
                brace_count++;
            } else if (ch == '}') {
                brace_count--;
                if (brace_count < 0) {
                    break;
                }
            } else if (ch == '[') {
                bracket_count++;
            } else if (ch == ']') {
                bracket_count--;
                if (bracket_count < 0) {
                    break;
                }
            }
        }
    }
    if (brace_count != 0 || bracket_count != 0 || in_string) {  // 检查所有括号是否平衡及字符串是否闭合
        printf("[BTCOMTEST] JSON brace/bracket mismatch or unclosed string (braces:%d, brackets:%d), waiting for complete JSON, len:%d\n", brace_count, bracket_count, len);
        // 添加缓冲区转储
        printf("[BTCOMTEST] Mismatch buffer dump:\n");
        for (n = 0; n < len; n++) {
            printf("%x ", (unsigned char)buf[n]);  // 使用unsigned char以避免符号问题
        }
        printf("\n");
        return false; // 括号不匹配或字符串未闭合
    }
    return true;  // JSON结构完整
}

int bt_recWifiConfigData(void)
{
    int n;
    // 没有数据且不在等待分包，直接返回
    if (recv_len == 0 && !waiting_for_packet) {
        return 1;
    }

    // 检查超时
    if (waiting_for_packet && (os_time_get() - recv_start_time >= RECV_TIMEOUT_MS)) {
        printf("[BTCOMTEST] Receive timeout, resetting\n");
        if (recv_len > 0) {
            printf("[BTCOMTEST] Timeout buffer dump, len:%d\n", recv_len);
            for (n = 0; n < recv_len; n++) {
                printf("%x ", recvbuf[n]);
            }
            printf("\n");
        }
        recv_len = 0;
        waiting_for_packet = false;
        BLESendWifiConfigstateCode(0x01);
        return BT_WIFI_CONFIG_ERR_CONNECTION_TIMEOUT;  // 返回枚举值以便上层处理
    }

    // 使用新函数进行JSON完整性检查
    if (!is_json_complete(recvbuf, recv_len)) {
        return 1;  // 未完整，继续等待
    }

    // 收到完整 JSON，处理数据
    printf("[BTCOMTEST] Complete JSON detected, len:%d\n", recv_len);  // 统一日志以匹配您的日志示例
    for (n = 0; n < recv_len; n++) {
        printf("%x ", recvbuf[n]);
    }
    printf("\n");

    // 调用 WiFi 配置（假设 bt_configWifi 已返回 E_BT_WIFI_CONFIG_STATUS）
    E_BT_WIFI_CONFIG_STATUS result = bt_configWifi(recvbuf, recv_len);

    // 根据结果发送状态码（映射枚举到 u8）
    u8 ble_status;
    switch (result) {
        case BT_WIFI_CONFIG_SUCCESS:
            ble_status = 0x00;  // 成功
            // sendMsgNonblock(OS_TASK_UI, WIFI_CONFIG_FINISH, 0);
            break;
        case BT_WIFI_CONFIG_ERR_JSON_PARSE:
            ble_status = 0x01;  // 解析失败
            break;
        case BT_WIFI_CONFIG_ERR_INVALID_PARAMS:
            ble_status = 0x02;  // 无效参数
            break;
        case BT_WIFI_CONFIG_ERR_WIFI_INIT_FAIL:
            ble_status = 0x03;  // WiFi初始化失败，请重试
            break;
        case BT_WIFI_CONFIG_ERR_START_WIFI_FAIL:
            ble_status = 0x04;  // 启动WiFi失败
            break;
        case BT_WIFI_CONFIG_ERR_CONNECTION_TIMEOUT:
            ble_status = 0x05;  // 连接超时，请检查WIFI是否存在
            break;
        case BT_WIFI_CONFIG_ERR_DHCP_FAIL:
            ble_status = 0x06;  // IP获取失败，请重新连接
            break;
        case BT_WIFI_CONFIG_ERR_WIFI_OPEN_ERR:
            ble_status = 0x07;  // WiFi 打开错误, 请重启设备
            break;
        case BT_WIFI_CONFIG_ERR_FILE_OPEN_FAIL:
            ble_status = 0x08;  // 失败，请检查是否插入TF卡
            break;
        case BT_WIFI_CONFIG_ERR_FILE_WRITE_FAIL:
            ble_status = 0x09;  // 失败，请检查是否插入TF卡
            break;
        case BT_WIFI_CONFIG_ERR_JSON_CREATE_FAIL:
            ble_status = 0x0A;  // wifi配置文件创建失败，请检查是否插入TF卡
            break;
        case BT_WIFI_CONFIG_ERR_CLEANUP_FAIL:
            ble_status = 0x0B;  // 清理旧信息失败，请检查是否插入TF卡
            break;
        case BT_WIFI_CONFIG_ERR_INVALID_PASSWORD: 
            ble_status = 0x0D;  // 无效 WiFi 密码，请检查WIFI密码是否正确
            break;
        default:
            ble_status = 0x0C;  // 未知错误
            break;
    }

    // 发送 BLE 状态码
    BLESendWifiConfigstateCode(ble_status);

    // 打印结果日志
    printf("\r\r\r\n[BTCOMTEST] result:%d (BLE status: 0x%02X)\r\r\r\n", result, ble_status);

    // 重置状态
    recv_len = 0;
    waiting_for_packet = false;
    return (int)result;  // 返回枚举值（强制转换为 int 以兼容原有接口，如果需要）
}
