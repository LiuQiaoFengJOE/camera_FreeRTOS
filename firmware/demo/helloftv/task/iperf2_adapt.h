
#ifndef __IPERF2_ADAPT_H__
#define __IPERF2_ADAPT_H__

/*
"iperf_set":{
	"enable":1,
	"server_mode":0,
	"server_ipaddr":"192.168.88.25",
	"server_port":5001,
	"udp_mode":0,
	"send_bps":50M,
	"time_duration":60,
	"info_printf_duration":1,
}

*/
typedef struct _iperf2_set
{
	unsigned char 	enable;
	unsigned char 	server_mode;
	char 			server_ipaddr[32];
	unsigned int 	server_port;
	unsigned char 	udp_mode;
	unsigned int 	send_bps;
	unsigned char 	time_duration;
	unsigned short 	info_printf_duration;
}iperf2_set;


void iperf2_init(iperf2_set*set);
void iperf2_start();
void iperf2_stop();
void iperf2_deinit();

#endif




