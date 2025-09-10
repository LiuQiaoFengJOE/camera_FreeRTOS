#include <stdio.h>

#include "wlan/wlan_test_inc.h"

// The following three define should be inputed as parameters
#define CONFIG_WEP 0
#define CONFIG_WPA 0
#define CONFIG_WPA2 0

#define CONFIG_SOCKET_TEST 0
#define CONFIG_SOCKET_TRAFFIC 0
#define CONFIG_TCPECHO 0
#define CONFIG_UDPECHO 0
#define CONFIG_DEINIT_TEST 0
#define CONFIG_POWER_SAVE_TEST 0
#define CONFIG_THROUGHPUT_TEST_RECV 0
#define CONFIG_THROUGHPUT_TEST_XMIT 0
#define CONFIG_CLOUD_LIGHT 0
#define CONFIG_CLOUD_SENSOR 0
#define CONFIG_SSL_CLIENT 0

void post_init(unsigned int wifi_mode)
{

#if CONFIG_SOCKET_TEST
	/* Connect to web server in network to test socket */
	reset_socket_test_cb();
	do_socket_test(1, 10, 1);
#endif

#if CONFIG_SOCKET_TRAFFIC
#if CONFIG_WEP
	create_socket_traffic(1);
#elif CONFIG_WPA2
	create_socket_traffic(2);
#else
	create_socket_traffic(0);
#endif
#endif

#if CONFIG_DEINIT_TEST
#if CONFIG_WEP
	do_deinit_test(1, wifi_mode);
#elif CONFIG_WPA
	do_deinit_test(3, wifi_mode);
#elif CONFIG_WPA2
	do_deinit_test(2, wifi_mode);
#else
	do_deinit_test(0, wifi_mode);
#endif
#endif

#if CONFIG_TCPECHO
	/* Initialize tcp echo server */
	tcpecho_init();
#endif
#if CONFIG_UDPECHO
	/* Initialize udp echo server */
	udpecho_init();
#endif

#if CONFIG_POWER_SAVE_TEST
#if CONFIG_WEP
	do_power_save_test(1);
#elif CONFIG_WPA2
	do_power_save_test(2);
#else
	do_power_save_test(0);
#endif
#endif

#if CONFIG_THROUGHPUT_TEST_RECV
	ttcp_test(0);
#endif
#if CONFIG_THROUGHPUT_TEST_XMIT
	ttcp_test(1);
#endif

#if CONFIG_CLOUD_LIGHT
	start_cloud_light(NULL, NULL);
#elif CONFIG_CLOUD_SENSOR
	start_cloud_sensor(NULL, NULL);
#endif

#if CONFIG_SSL_CLIENT
	start_ssl_client();
#endif
}
