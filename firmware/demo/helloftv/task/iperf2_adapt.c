/**
* iperf-liked network performance tool
*
*/

/* include */
#include "FreeRTOS.h"

#include "task.h"
#include "iperf2.h"
#include "iperf2_adapt.h"


#define IPERF2_LOG_DUG  0
#define IPERF2_LOG_INF  1
#define IPERF2_LOG_WRN  2
#define IPERF2_LOG_ERR  3

#define IPERF2_HZ configTICK_RATE_HZ


/* macro */
#define IPERF2_LOG_LEVEL  IPERF2_LOG_DUG
#define IPERF2_LOG(level,fmt, ...)       \
if(IPERF2_LOG_LEVEL <= level){		 \
	printf("[%s,%d]" fmt, __FUNCTION__,__LINE__,##__VA_ARGS__);\
};


#define IPERF2_PORT              5001

static iperf2_set s_iperf2_set={
	.enable = 0,
	.server_mode = 0,
	.server_ipaddr[0] = '\0',
	.server_port = IPERF2_PORT,
	.udp_mode = 0,
	.send_bps = 20,
	.time_duration = 30,
	.info_printf_duration = 1
};

#define OPENSOURCE_IPERF_SET 0

#if OPENSOURCE_IPERF_SET

static char iperf_tx[16][24];
static int cmd_param_idx = 0;


void iperf2_init(iperf2_set*set)
{
	if(set == &s_iperf2_set){
		return;
	}

	s_iperf2_set.enable = set->enable;
	s_iperf2_set.server_mode = set->server_mode;
	strcpy(s_iperf2_set.server_ipaddr,set->server_ipaddr);
	s_iperf2_set.server_port = set->server_port;
	s_iperf2_set.udp_mode = set->udp_mode;
	s_iperf2_set.send_bps = set->send_bps;
	s_iperf2_set.time_duration = set->time_duration;
	s_iperf2_set.info_printf_duration = set->info_printf_duration;


	printf("========================== s_iperf2_set ===================================\n");
	printf("s_iperf2_set.enable:%u\n",s_iperf2_set.enable);
	printf("s_iperf2_set.server_mode:%u\n",s_iperf2_set.server_mode);
	printf("s_iperf2_set.server_ipaddr:%s\n",s_iperf2_set.server_ipaddr);
	printf("s_iperf2_set.server_port:%u\n",s_iperf2_set.server_port);
	printf("s_iperf2_set.udp_mode:%u\n",s_iperf2_set.udp_mode);
	printf("s_iperf2_set.send_bps:%u\n",s_iperf2_set.send_bps);
	printf("s_iperf2_set.time_duration:%u\n",s_iperf2_set.time_duration);
	printf("s_iperf2_set.info_printf_duration:%u\n",s_iperf2_set.info_printf_duration);
	
}

void iperf2_start()
{
	int ret2 = 0;
	IPERF2_LOG(IPERF2_LOG_INF,"Iperf test begin,enable:%u.\n",s_iperf2_set.enable);	
	memset(iperf_tx,0,16*32);
	cmd_param_idx = 0;
	strcpy(iperf_tx[cmd_param_idx],"iperf");
	cmd_param_idx++;
	int i = 0;
	if(1 == s_iperf2_set.enable){

		if(1 == s_iperf2_set.server_mode){//服务端模式

			strcpy(iperf_tx[cmd_param_idx],"-s");
			cmd_param_idx++;
			
			if(0 == s_iperf2_set.udp_mode){
			    // {"iperf", "-s", "-p", "5001", "-i", "1"},			
				IPERF2_LOG(IPERF2_LOG_INF,"==== iperf2_tcp_server mode,for tcp rx test.\n");
			
				
			}else{//UDP模式
				// { "iperf", "-s", "-u", "-p", "5001",},
				strcpy(iperf_tx[cmd_param_idx],"-u");
				cmd_param_idx++;
				IPERF2_LOG(IPERF2_LOG_INF,"==== iperf2_udp_server mode,for udp rx test.\n");
				
			}
			strcpy(iperf_tx[cmd_param_idx],"-i");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%u",s_iperf2_set.info_printf_duration);
			cmd_param_idx++;	

			strcpy(iperf_tx[cmd_param_idx],"-p");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%u",s_iperf2_set.server_port);
			cmd_param_idx++;

		}else{//客户端模式
			strcpy(iperf_tx[cmd_param_idx],"-c");
			cmd_param_idx++;
			strcpy(iperf_tx[cmd_param_idx],s_iperf2_set.server_ipaddr);
			cmd_param_idx++;
			strcpy(iperf_tx[cmd_param_idx],"-i");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%u",s_iperf2_set.info_printf_duration);
			cmd_param_idx++;
			strcpy(iperf_tx[cmd_param_idx],"-t");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%u",s_iperf2_set.time_duration);
			cmd_param_idx++;

			if(0 == s_iperf2_set.udp_mode){
			//{ "iperf", "-c", "192.168.2.17", "-i", "1", "-t", "60", "-p", "5001"},
				IPERF2_LOG(IPERF2_LOG_INF,"==== iperf2_tcp_client mode,for tcp tx text.\n");
				
			}else{//UDP模式
			//{"iperf", "-c", "192.168.2.17", "-i", "1", "-t", "60", "-b", "20M", "-u", "-p", "5001",},

			strcpy(iperf_tx[cmd_param_idx],"-b");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%uM",s_iperf2_set.send_bps);
			cmd_param_idx++;
			strcpy(iperf_tx[cmd_param_idx],"-u");
			cmd_param_idx++;

			IPERF2_LOG(IPERF2_LOG_INF,"==== iperf2_udp_client mode,for udp tx text.\n");
				
			}

			strcpy(iperf_tx[cmd_param_idx],"-p");
			cmd_param_idx++;
			sprintf(iperf_tx[cmd_param_idx],"%u",s_iperf2_set.server_port);
			cmd_param_idx++;
		}
		char**argv = malloc(cmd_param_idx * sizeof(char*));
		IPERF2_LOG(IPERF2_LOG_INF,"\niperf cmd:\r\n");
		for(i=0;i<cmd_param_idx;i++){
			argv[i]=iperf_tx[i];
			printf("%s ",iperf_tx[i]);
		}

		printf("\r\n======= Iperf begin ======= \r\n");
		IPERF2_LOG(IPERF2_LOG_INF,"cmd_param_idx:%d \n",cmd_param_idx);
		iperf2(cmd_param_idx, argv);

		mem_free(argv);
	}
}
//static char *iperf_tcp_tx[] = {"iperf", "-Q", "all"};

void iperf2_stop()
{
	IPERF2_LOG(IPERF2_LOG_INF,"Iperf test stop.\n");	

	static const char*argv[]={"iperf","-Q","all"};
	iperf2(3, argv);
}
void iperf2_deinit()
{

}
#else

#include "lwip/sockets.h"

#define IPERF2_MODE_STOP         0
#define IPERF2_MODE_SERVER       1
#define IPERF2_MODE_CLIENT       2

#define UDP_SWITCH               0
#define ip_addr_t          ip_addr
#define IPERF2_BUFSZ             (4 * 1024)

static unsigned char *g_iperf_buf = NULL;


typedef struct
{
	volatile unsigned char status;
	volatile unsigned char exited;
	volatile unsigned int num;
	iperf2_set*param;
	TaskHandle_t task_handle;
} iperf2_context;
static iperf2_context s_iperf2_context={
	.status = IPERF2_MODE_STOP,
	.exited =1,
	.num = 0,
	.param = &s_iperf2_set
};

static void iperf2_udp_client (void *thread_param)
{
	iperf2_context* context= (iperf2_context*)thread_param;
	iperf2_set* param = context->param;
	context->exited = 0;
    int sock;
    int *buffer = (int*)g_iperf_buf;
    struct sockaddr_in server;
    int packet_count = 0;
    int tick;
    int send_size;
    int sys_tick_one_second = IPERF2_HZ;
	int tick1, tick2;
	int ret;
	unsigned long long sentlen = 0;
    send_size = IPERF2_BUFSZ > 1470 ? 1470 : IPERF2_BUFSZ;

    if (buffer == NULL)
    {
        context->status = IPERF2_MODE_STOP;
        goto _exit;
    }
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"can't create socket! exit!");
        context->status = IPERF2_MODE_STOP;
        goto _exit;
    }
    server.sin_family = PF_INET;
    server.sin_port = htons(param->server_port);
    server.sin_addr.s_addr = inet_addr(param->server_ipaddr);
    IPERF2_LOG(IPERF2_LOG_INF,"iperf udp mode run...");
	
	tick1 = xTaskGetTickCount();
	sentlen = 0;
	while (context->status != IPERF2_MODE_STOP)
	{
		tick2 = xTaskGetTickCount();
		if (tick2 - tick1 >= sys_tick_one_second * 5)
		{
			long data;
	
			context->num += 1;
			data = sentlen * sys_tick_one_second / 125 / (tick2 - tick1);
			long mbytes_int = (data * 100) / 8000;
			long mbits_int = (data * 100) / 1000;
			printf("%d.0-%d.0 sec  %ld.%02ld MBytes  %ld.%02ld Mbits/sec\n\r",
				   context->num - 1, context->num,
				   mbytes_int / 100, mbytes_int % 100,
				   mbits_int / 100, mbits_int % 100);
			tick1 = tick2;
			sentlen = 0;
		}

        packet_count++;
        tick = xTaskGetTickCount();
        buffer[0] = htonl(packet_count);
        buffer[1] = htonl(tick / sys_tick_one_second);
        buffer[2] = htonl((tick % sys_tick_one_second) * 1000);
        ret = sendto(sock, buffer, send_size, 0, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
        if (ret > 0)
        {
            sentlen += ret;
        }	
		//vTaskDelay(1);
	}

_exit:
    closesocket(sock);
    IPERF2_LOG(IPERF2_LOG_DUG,"Iperf stopped successfully");
	context->exited = 1;

	vTaskDelete(NULL);
//    /* thread exit, then resource will be release by idle task */
//    osThreadExit();
}

static void iperf2_udp_server (void *thread_param)
{
	iperf2_context* context= (iperf2_context*)thread_param;
	iperf2_set* param = context->param;
	context->exited = 0;

    int sock;
    int *buffer = (int*)g_iperf_buf;
    struct sockaddr_in server;
    struct sockaddr_in sender;
    int sender_len, r_size;
    unsigned long long sentlen;
    int pcount = 0, last_pcount = 0;
    int lost, total;
    int tick1, tick2;
    struct timeval timeout;
    int sys_tick_one_second = IPERF2_HZ;
    context->num = 0;
    if (buffer == NULL)
    {
        context->status = IPERF2_MODE_STOP;
        goto _exit;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"can't create socket! exit!");
        goto _exit;
    }

    server.sin_family = PF_INET;
    server.sin_port = htons(param->server_port);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"setsockopt failed!");
        goto _exit;
    }

    if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"iperf server bind failed! exit!");
        goto _exit;
    }

    while (context->status != IPERF2_MODE_STOP)
    {
        tick1 = xTaskGetTickCount();
        tick2 = tick1;
        lost = 0;
        total = 0;
        sentlen = 0;

        while ((tick2 - tick1) < (sys_tick_one_second))
        {
            r_size = recvfrom(sock, buffer, IPERF2_BUFSZ, 0, (struct sockaddr *)&sender, (socklen_t*)&sender_len);
            if (r_size > 12)
            {
                pcount = ntohl(buffer[0]);
                if (last_pcount < pcount)
                {
                    lost += pcount - last_pcount - 1;
                    total += pcount - last_pcount;
                    last_pcount = pcount;
                }
                sentlen += r_size;
            }
            tick2 = xTaskGetTickCount();
        }

        if (sentlen > 0)
        {
            long data;

            context->num += 1;
            data = sentlen * sys_tick_one_second / 125 / (tick2 - tick1);
            long mbytes_int = (data * 100) / 8000;
            long mbits_int = (data * 100) / 1000;
            long lost_percent = lost * 100 / (total > 0 ? total : 1);
            printf("%d.0-%d.0 sec  %ld.%02ld MBytes  %ld.%02ld Mbits/sec  lost:%d (%ld%%)  total:%d/sec\n\r",
                   context->num - 1, context->num,
                   mbytes_int / 100, mbytes_int % 100,
                   mbits_int / 100, mbits_int % 100,
                   lost, lost_percent, total);
        }

		//vTaskDelay(1);
    }
_exit:
    if (sock >= 0) closesocket(sock);

	context->exited = 1;

    IPERF2_LOG(IPERF2_LOG_DUG,"Iperf stopped successfully");

	vTaskDelete(NULL);
    /* thread exit, then resource will be release by idle task */
}

static void iperf2_tcp_client (void *thread_param)
{
    int i;
    int sock;
    int ret;
    int tips = 1;
    unsigned char *send_buf = (unsigned char *)g_iperf_buf;
    unsigned long long sentlen;
    int tick1, tick2;
    struct sockaddr_in addr;
    int sys_tick_one_second = IPERF2_HZ;
	iperf2_context* context= (iperf2_context*)thread_param;
	iperf2_set* param = context->param;
	context->exited = 0;
	
    context->num = 0;
    if (!send_buf)
    {
        context->status = IPERF2_MODE_STOP;
		context->exited = 1;
        return;
    }

    for (i = 0; i < IPERF2_BUFSZ; i ++)
    {
        send_buf[i] = i & 0xff;
    }

    while (context->status != IPERF2_MODE_STOP)
    {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0)
        {
            IPERF2_LOG(IPERF2_LOG_ERR,"create socket failed!");
            vTaskDelay(sys_tick_one_second);
            continue;
        }

        addr.sin_family = PF_INET;
        addr.sin_port = htons(param->server_port);
        addr.sin_addr.s_addr = inet_addr((char *)param->server_ipaddr);

        ret = connect(sock, (const struct sockaddr *)&addr, sizeof(addr));
        if (ret == -1)
        {
            if (tips)
            {
                IPERF2_LOG(IPERF2_LOG_ERR,"Connect to iperf server faile, Waiting for the server to open!");
                tips = 0;
            }
            closesocket(sock);
            vTaskDelay(sys_tick_one_second);
            continue;
        }
        IPERF2_LOG(IPERF2_LOG_INF,"Connect to iperf server successful!");
        {
            int flag = 1;

            setsockopt(sock,
                       IPPROTO_TCP,     /* set option at TCP level */
                       TCP_NODELAY,     /* name of option */
                       (void *) &flag,  /* the cast is historical cruft */
                       sizeof(int));    /* length of option value */
        }
        sentlen = 0;
        tick1 = xTaskGetTickCount();
        while (context->status != IPERF2_MODE_STOP)
        {
            tick2 = xTaskGetTickCount();
            if (tick2 - tick1 >= sys_tick_one_second * 5)
            {
                long data;

                context->num += 1;
                data = sentlen * sys_tick_one_second / 125 / (tick2 - tick1);
                long mbytes_int = (data * 100) / 8000;
                long mbits_int = (data * 100) / 1000;
                printf("%d.0-%d.0 sec  %ld.%02ld MBytes  %ld.%02ld Mbits/sec\n\r",
                       context->num - 1, context->num,
                       mbytes_int / 100, mbytes_int % 100,
                       mbits_int / 100, mbits_int % 100);
                tick1 = tick2;
                sentlen = 0;
            }

            ret = send(sock, send_buf, IPERF2_BUFSZ, 0);
            if (ret > 0)
            {
                sentlen += ret;
            }

            if (ret < 0) break;

			 //vTaskDelay(1);
        }

        closesocket(sock);

        vTaskDelay(sys_tick_one_second * 2);
        IPERF2_LOG(IPERF2_LOG_WRN,"Disconnected, iperf server shut down!");
        tips = 1;
    }
   
    IPERF2_LOG(IPERF2_LOG_DUG,"Iperf stopped successfully");
	context->exited = 1;
    /* thread exit, then resource will be release by idle task */
	vTaskDelete(NULL);
}

void iperf2_tcp_server (void *thread_param)
{
    unsigned char *recv_data = (unsigned char *)g_iperf_buf;
    socklen_t sin_size;
    int tick1, tick2;
    int sock = -1, connected, bytes_received;
    unsigned long long recvlen;
    struct sockaddr_in server_addr, client_addr;
    fd_set readset;
    struct timeval timeout;
    int sys_tick_one_second = IPERF2_HZ;

	iperf2_context* context= (iperf2_context*)thread_param;
	iperf2_set* param = context->param;
	context->exited = 0;
	
    context->num = 0;
    if (recv_data == NULL)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"No memory!");
        goto _exit;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"Socket error!");
        goto _exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(param->server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0x0, sizeof(server_addr.sin_zero));

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"Unable to bind!");
        goto _exit;
    }

    if (listen(sock, 5) == -1)
    {
        IPERF2_LOG(IPERF2_LOG_ERR,"Listen error!");
        goto _exit;
    }

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    while (context->status != IPERF2_MODE_STOP)
    {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);
        if (select(sock + 1, &readset, NULL, NULL, &timeout) == 0)
            continue;
        sin_size = sizeof(struct sockaddr_in);

        connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);

        IPERF2_LOG(IPERF2_LOG_INF,"new client connected from (%s, %d)",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        {
            int flag = 1;

            setsockopt(connected,
                       IPPROTO_TCP,     /* set option at TCP level */
                       TCP_NODELAY,     /* name of option */
                       (void *) &flag,  /* the cast is historical cruft */
                       sizeof(int));    /* length of option value */
        }

        recvlen = 0;
        tick1 = xTaskGetTickCount();
        while (context->status != IPERF2_MODE_STOP)
        {
            bytes_received = recv(connected, recv_data, IPERF2_BUFSZ, 0);
            if (bytes_received <= 0) break;

            recvlen += bytes_received;

            tick2 = xTaskGetTickCount();
            if (tick2 - tick1 >= sys_tick_one_second)
            {
                long data;
                context->num += 1;
                data = recvlen * sys_tick_one_second / 125 / (tick2 - tick1);
                long mbytes_int = (data * 100) / 8000;
                long mbits_int = (data * 100) / 1000;
                printf("%d.0-%d.0 sec  %ld.%02ld MBytes  %ld.%02ld Mbits/sec\n\r",
                       context->num - 1, context->num,
                       mbytes_int / 100, mbytes_int % 100,
                       mbits_int / 100, mbits_int % 100);
                tick1 = tick2;
                recvlen = 0;
            }

			//vTaskDelay(1);
        }
        IPERF2_LOG(IPERF2_LOG_WRN,"client disconnected (%s, %d)",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        context->num = 0;
        if (connected >= 0) closesocket(connected);
        connected = -1;
    }

_exit:
    if (sock >= 0) closesocket(sock);

    context->status = IPERF2_MODE_STOP;
    IPERF2_LOG(IPERF2_LOG_DUG,"Iperf stopped successfully");
	context->exited = 1;
	vTaskDelete(NULL);

    /* thread exit, then resource will be release by idle task */
//    osThreadExit();
}

unsigned int wait_thread_exit(int time_out)
{
	const int delay_ms = 100;
	s_iperf2_context.status = IPERF2_MODE_STOP;
	int try  = 0;
	int loop = (time_out <=delay_ms)?1:time_out/delay_ms;
	while(1 != s_iperf2_context.exited &&  try <= loop){
		try ++;
		vTaskDelay(delay_ms);
	}
	if(try >= loop){
		IPERF2_LOG(IPERF2_LOG_DUG,"Iperf exit timeout.");
		return 0;
	}else{
		IPERF2_LOG(IPERF2_LOG_DUG,"Iperf exit success.");
		return 1;
	}
}

void iperf2_init(iperf2_set*set)
{
	if(set == &s_iperf2_set){
		return;
	}
	s_iperf2_context.status = IPERF2_MODE_STOP;
	wait_thread_exit(5000);

	s_iperf2_set.enable = set->enable;
	s_iperf2_set.server_mode = set->server_mode;
	strcpy(s_iperf2_set.server_ipaddr,set->server_ipaddr);
	s_iperf2_set.server_port = set->server_port;
	s_iperf2_set.udp_mode = set->udp_mode;
	s_iperf2_set.send_bps = set->send_bps;
	s_iperf2_set.time_duration = set->time_duration;
	s_iperf2_set.info_printf_duration = set->info_printf_duration;

	printf("========================== s_iperf2_set ===================================\n");
	printf("s_iperf2_set.enable:%u\n",s_iperf2_set.enable);
	printf("s_iperf2_set.server_mode:%u\n",s_iperf2_set.server_mode);
	printf("s_iperf2_set.server_ipaddr:%s\n",s_iperf2_set.server_ipaddr);
	printf("s_iperf2_set.server_port:%u\n",s_iperf2_set.server_port);
	printf("s_iperf2_set.udp_mode:%u\n",s_iperf2_set.udp_mode);
	printf("s_iperf2_set.send_bps:%u\n",s_iperf2_set.send_bps);
	printf("s_iperf2_set.time_duration:%u\n",s_iperf2_set.time_duration);
	printf("s_iperf2_set.info_printf_duration:%u\n",s_iperf2_set.info_printf_duration);


	if(g_iperf_buf == NULL){		
		g_iperf_buf = (unsigned char *) mem_malloc(IPERF2_BUFSZ);
	}


}
void (*iperf2_proc)(void *parameter);
void iperf2_start()
{
	printf("========================== iperf2_start ===================================\n");

	if(g_iperf_buf == NULL){		
		g_iperf_buf = (unsigned char *) mem_malloc(IPERF2_BUFSZ);
	}

	int ret2 = 0;
	wait_thread_exit(5000);

	if(1 == s_iperf2_set.enable && s_iperf2_context.status == IPERF2_MODE_STOP){

		if(1 == s_iperf2_set.server_mode){//服务端模式
		    s_iperf2_context.status = IPERF2_MODE_SERVER;
			if(0 == s_iperf2_set.udp_mode){
				printf("iperf2_tcp_server mode.\n");
				iperf2_proc = iperf2_tcp_server;
			}else{//UDP模式
			printf("iperf2_udp_server mode.\n");
				iperf2_proc = iperf2_udp_server;
			}
		}else{//客户端模式
			s_iperf2_context.status = IPERF2_MODE_CLIENT;
			if(0 == s_iperf2_set.udp_mode){
				printf("iperf2_tcp_client mode.\n");
				iperf2_proc = iperf2_tcp_client;
			}else{//UDP模式
			printf("iperf2_udp_client mode.\n");
				iperf2_proc = iperf2_udp_client;
			}
		}
		ret2 = xTaskCreate( iperf2_proc, 
							"iperf2_proc", 
							2048, 
							&s_iperf2_context, 
							22, 
							&(s_iperf2_context.task_handle));
		if(pdPASS != ret2){
			iperf2_stop();
			IPERF2_LOG(IPERF2_LOG_ERR,"Iperf create task fail.");
		}
	}
}
void iperf2_stop()
{
	printf("========================== iperf2_stop ===================================\n");
	wait_thread_exit(100000);
}
void iperf2_deinit()
{
	wait_thread_exit(10000);
	s_iperf2_set.enable = 0;
	s_iperf2_context.num = 0;

	if(g_iperf_buf != NULL){
		mem_free(g_iperf_buf);
		g_iperf_buf = NULL;
	}
}


#endif


