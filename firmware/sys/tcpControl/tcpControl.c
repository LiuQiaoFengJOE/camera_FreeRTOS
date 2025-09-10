#include "config/conf_parse.h"
#include "osal/osal_socket.h"
#include "osal/osal_wait.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "cjson/cjson.h"
#include "osal/osal_list.h"

#include "debug.h"
#include "tcpControl.h"
#include "mac_fd/mac_fd.h"

// 以后需要将这部分分离

struct tcpControl_fd *tcp_fd = NULL;

struct tcpControl_fd *find_fd(int fd)
{
	struct tcpControl_fd *s;
	portINIT_CRITICAL();
	portENTER_CRITICAL();
	HASH_FIND_INT(tcp_fd, &fd, s);
	portEXIT_CRITICAL();
	return s;
}

struct tcpControl_fd *add_fd(int fd)
{
	struct tcpControl_fd *found_s;
	struct tcpControl_fd *s;
	found_s = find_fd(fd);
	if (found_s)
	{
		delete_fd(found_s->fd);
	}
	else
	{
		printf("no found fd:%d\r\n", fd);
	}
	s = osal_malloc(sizeof(struct tcpControl_fd));
	s->fd = fd;
	s->type = 0;
	s->self = NULL;
	memset(s->mac, 0, 6);
	printf("add_fd:%d\r\n", fd);
	portINIT_CRITICAL();
	portENTER_CRITICAL();
	HASH_ADD_INT(tcp_fd, fd, s);
	portEXIT_CRITICAL();
	return s;
}

void delete_fd(int fd)
{
	struct tcpControl_fd *s;
	s = find_fd(fd);
	if (s)
	{
		portINIT_CRITICAL();
		portENTER_CRITICAL();
		HASH_DEL(tcp_fd, s);
		portEXIT_CRITICAL();
		printf("delete_fd:%d\r\n", fd);
		osal_free(s);
	}
	else
	{
		printf("delete_fd:%d already delete!\r\n", fd);
	}
}

// 匹配到对应mac地址一样时就删除,并且处理完这一项后停止遍历
// 0:遍历所有也没有找到对应项	1:找到了一项
uint8_t del_mac_fd(const char *mac)
{
	struct tcpControl_fd *s, *tmp;
	portINIT_CRITICAL();
	portENTER_CRITICAL();

	HASH_ITER(hh, tcp_fd, s, tmp)
	{
		if (s)
		{
			printf("del fd:%d,type:%d\n",s->fd, s->type);
			if (memcmp(mac, s->mac, 6) == 0)
			{
				HASH_DEL(tcp_fd, s);
				portEXIT_CRITICAL();

				// 对类型进行判断处理
				switch (s->type)
				{
				case 0: // 默认,不需要任何动作
					break;
				case 1: // sender,没有额外动作
					break;
				case 2: // rtsp,drop,self就是conn
					if (s->self)
					{
						dropConnAndWaite();
						socket_cmd_drop_client();
						s->self = NULL;
					}
					break;
				default:
					break;
				}
				printf("s->fd2 finish:%d\r\n", s->fd);
				osal_free(s);
				return 1;
			}
		}
	}
	portEXIT_CRITICAL();
	return 0;
}

// 匹配到对应mac地址一样时就删除
int get_all_fd(int *fd, const int max)
{
	struct tcpControl_fd *s, *tmp;
	int i = 0;
	portINIT_CRITICAL();
	portENTER_CRITICAL();

	HASH_ITER(hh, tcp_fd, s, tmp)
	{
		if (s && s->type == 1)
		{
			fd[i] = s->fd;
			i++;
			if (i >= max)
			{
				break;
			}
		}
	}
	portEXIT_CRITICAL();
	return i;
}

uint32_t tcp_fd_num()
{
	return HASH_COUNT(tcp_fd);
}

static LIST_HEAD(sender_fd_list);

void *get_sender_fd_list()
{
	return &sender_fd_list;
}

#define HEADER_LEN 4
static int recv_message_length(struct tcpControl *control)
{
	int fd = control->fd;
	int times = 0;

	uint8_t recvBuf[HEADER_LEN];
	int recvLen = 0;

	do
	{
		int l = osal_socket_recv(fd, recvBuf + recvLen, HEADER_LEN - recvLen, 0);
		// printf("socket_errno:%d\tl:%d\r\n",socket_errno,l);
		if (l > 0)
		{
			recvLen += l;
			times = 0;
		}
		else if (l == 0)
		{
			// failed
			return 0;
		}
		else
		{
			void *is = find_fd(fd);
			times++;

			if (times > 5)
			{
				printf("tcpControl timeout1!\r\n");
				return 0;
			}

			// 如果不存在,则返回错误,否则一直重新接收
			if (!is)
			{
				return 0;
			}

			continue;
		}
	} while (recvLen < HEADER_LEN);

	// big endian in buffer
	return (recvBuf[0] << 24) | (recvBuf[1] << 16) | (recvBuf[2] << 8) | (recvBuf[3] << 0);
}

static int recv_message_content(struct tcpControl *control, uint8_t *recvBuf, int bufLen)
{
	int fd = control->fd;
	int recvLen = 0;
	int times = 0;

	int msgLen = recv_message_length(control);
	if (msgLen > 0)
	{
		// recv message
		if (msgLen <= bufLen)
		{
			do
			{
				int l = osal_socket_recv(fd, recvBuf + recvLen, msgLen - recvLen, 0);
				if (l > 0)
				{
					recvLen += l;
					times = 0;
				}
				else if (l == 0)
				{
					return 0;
				}
				else
				{
					void *is = find_fd(fd);
					times++;
					// 如果不存在,则返回错误,否则一直重新接收
					if (!is)
					{
						return 0;
					}
					if (times > 5)
					{
						printf("tcpControl timeout1!\r\n");
						return 0;
					}
					continue;
				}

			} while (recvLen < msgLen);
		}
		// message is too large
		else
		{
			printf("tcpControl: message is larger than receive buffer!\r\n");
		}
	}
	else
	{
		return 0;
	}

	return recvLen;
}

#include "tcpMessage.h"

// 刚连接需要发送设备的设置以及一些状态
static void tcp_control_rec_thread(struct tcpControl *control)
{
	// static uint32_t requestTimes = 0;
	printf("xPortGetFreeHeapSize:%d\r\n", xPortGetFreeHeapSize());

	uint8_t *recBuf;
	int fd = control->fd;
	int i;
	char *out = NULL;
	recBuf = osal_malloc(1024);
	if (!recBuf)
	{
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
	}
	message_t message;
	// 初始状态
	struct tcpMessage *tcpMessage = get_device_status(DEVICE_STATUS_IDLE);
	netMsgValInit();
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("app ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}
	// 所有设置信息,例如分辨率、质量等,app上有的都会发送
	tcpMessage = all_device_status();
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("status ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}

	// sd卡状态
	tcpMessage = need_send_dev_msg(MSG_ID_DEVICE_FUNCTION);
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("MSG_ID_DEVICE_FUNCTION ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}

	// sd卡状态
	tcpMessage = getMsg_sd_status(get_sd_status()); // need_send_dev_msg
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("sd_status ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}

	// 电池信息
	tcpMessage = get_dev_battery_msg(MSG_ID_BATTERY);
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("dev_battery ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}

	tcpMessage = need_send_dev_msg(MSG_ID_RECORD_VIDEO);
	if (tcpMessage && tcpMessage->sendbuf)
	{
		int ret = osal_socket_send(fd, tcpMessage->sendbuf, tcpMessage->length, 0);
		//printf("dev_battery ret:%d\r\n", ret);
		//uartPrintfBuf(tcpMessage->sendbuf, ret);
		osal_free(tcpMessage->sendbuf);
		osal_free(tcpMessage);
	}

	while (1)
	{
		int recvLen = recv_message_content(control, recBuf, 1024);
		uartPrintfBuf(recBuf, recvLen);
		printf("recvLen: %d\r\n", recvLen);
		if (recvLen > 0)
		{
			// message with id = msgId(1) + sessionId(1) + reserved(2) + message(...)
			message.id = recBuf[0];
			message.sessionId = recBuf[1];
			message.info = recBuf + 4;
			message.length = recvLen - 4;
			// NOTE: add information needed
			message.context = (void *)control; // 'control' is only the thing we need now

			if (process_message(&message) != 0)
			{
				printf("process_message error\r\n");
			}
		}
		else
		{
			break;
		}
	}

	// free_request(http_request);
	osal_free(recBuf);
	osal_free(control);
	delete_fd(fd);
	osal_socket_close(fd);
	printf("thread [%s] stack size:%d,use:%d\n",pcTaskGetName(NULL),uxTaskGetStackSize(NULL),uxTaskGetStackSize(NULL)-uxTaskGetStackHighWaterMark(NULL));
	osal_thread_delete(osal_thread_myself());
	return;
}

static xQueueHandle sender_Queue;

xQueueHandle get_sender_queue()
{
	return sender_Queue;
}

#define MAX_SEND_NUM 10 // 最大主动发送的fd数量
void sender_center(void *d)
{
	long res = 0;
	int ret;
	int fd[MAX_SEND_NUM];
	int fd_num;
	int i;
	int j;
	uint32_t length;
	sender_Queue = xQueueCreate(10, sizeof(struct tcpMessage));
	struct tcpMessage *msg;
	msg = osal_malloc(sizeof(struct tcpMessage));
	while (1)
	{
		res = xQueueReceive(sender_Queue, msg, portMAX_DELAY);
		if(msg->fd==0xffffffff)
		{
			i = 0;
			i = get_all_fd(fd, MAX_SEND_NUM);
			for (j = 0; j < i; j++)
			{
				ret = osal_socket_send(fd[j], msg->sendbuf, msg->length, 0);
				//printf("ret:%d\tfd:%d\r\n", ret, fd[j]);
				//uartPrintfBuf(msg->sendbuf, msg->length);
			}
		}
		else
		{
			if(find_fd(msg->fd))
			{
				ret = osal_socket_send(msg->fd, msg->sendbuf, msg->length, 0);
			}
		}
		osal_free(msg->sendbuf);
	}
}

/********************* Initial ********************/
static void do_accept(struct event_info *ei, void *d)
{
	SOCK_HDL fd;
	unsigned int i;
	struct sockaddr_in addr;
	ERR_STR strerr;

	i = sizeof(addr);
	if ((fd = osal_socket_accept((int)d, (struct sockaddr *)&addr, &i)) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_warn("error accepting TCP connection: %s", strerr);
		return;
	}
	p_verb("accepted connection from %s:%d\r\n",
		   inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	unsigned char mac[8];

	if (1)
	{

		osal_socket_getmac(fd, mac);
		p_info("MAC %02x-%02x-%02x-%02x-%02x-%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}

	if (osal_set_tcp_nodelay(fd, 1) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_info("error setting TCP_NODELAY on socket: %s", strerr);
	}
	printf("create new thread fd:%d\r\n", fd);

	printf("111xPortGetFreeHeapSize:%d\r\n", xPortGetFreeHeapSize());

	struct tcpControl *control = osal_malloc(sizeof(struct tcpControl));
	if (!control)
	{
		printf("%s osal_malloc no space:%d!\r\n", __FUNCTION__, xPortGetFreeHeapSize());
	}

	control->fd = fd;
	struct tcpControl_fd *s;
	s = add_fd(fd);
	memcpy(s->mac, mac, 6);
	s->type = 1;
	printf("fd:%d\r\n", fd);
	int nNetTimeout = 5000; // 2秒
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int)) < 0)
	{
		printf("socket_errno:%d\r\n", socket_errno);
	}
	osal_thread_create(tcp_control_rec_thread, control, 20, 0, 1024, "tcp_control_rec");
}

static int tcp_control_listen(int port)
{
	struct sockaddr_in addr;
	SOCK_HDL fd;
	ERR_STR strerr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(port);

	if ((fd = osal_socket_open(SOCK_STREAM)) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_err("error creating listen socket: %s\r\n", strerr);
		return -1;
	}

	if (osal_set_reuseaddr(fd, 1) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_warn("ignoring error on setsockopt: %s\r\n", strerr);
	}
	if (osal_socket_bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_err("unable to bind to tcp socket: %s\r\n", strerr);
		osal_socket_close(fd);
		return -1;
	}
	if (osal_socket_listen(fd, 32) < 0)
	{
		osal_socket_strerr(strerr, socket_errno);
		p_err("error when attempting to listen on tcp socket: %s\r\n", strerr);
		osal_socket_close(fd);
		return -1;
	}
	eloop_add_fd(fd, 0, 0, do_accept, (void *)fd);
	osal_thread_create(sender_center, NULL, 20, 0, 512, "sender_center");

	p_info("listening on tcp_control_listen port %s:%d\r\n", inet_ntoa(addr.sin_addr), port);

	return 0;
}

/********************* GLOBAL CONFIGURATION DIRECTIVES ********************/
int config_tcp_control(int port)
{
	if (port <= 0 || port > 65535)
	{
		p_err("invalid listen port %d", port);
		return -1;
	}

	return tcp_control_listen(port);
}

int tcp_server_fd = -1;

int InitSrv(uint16_t port)
{
	int srvsock;
	struct sockaddr_in server;
	srvsock = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(srvsock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Binderror.");
		return -1;
	}
	if (listen(srvsock, 3) == -1)
	{
		perror("listen()error\n");
		return -1;
	}
	return srvsock;
}

#define RECV_MAX_SIZE 1024

enum TcpServerCmd
{
	REQ_NET_UPGRADE = 0xA0,
	ACK_NET_UPGRADE = 0xA1,
};

typedef struct tcpServerHeader
{
	unsigned int Command;
	unsigned int Reserved[8];
} TCP_SERVER_H;

typedef struct tcpNetUpgradeInfo
{
	char version[16];
	int fileSize;
} NetUpgrade_Info;

bool SPI_NetUpgrade(char *upgrade_file_buf, int upgrade_file_size)
{
	int ret = -1;

	//	spi_init();
	ret = spi_write_64K(0, upgrade_file_buf, upgrade_file_size);
	if (ret < 0)
	{
		printf("%s spi_write error\n", __FUNCTION__);
		return false;
	}

	return true;
}
uint8_t *get_mode_heap_last();

void tcpNetUpgrade(int tcp_connect_fd)
{

	char *upgrade_file_buf = NULL;
	char *tcp_rcv_buff = NULL;
	int size = 0;
	int offset = 0;

	tcp_rcv_buff = (char *)osal_malloc(RECV_MAX_SIZE + 4);

	upgrade_file_buf = get_mode_heap_last(); //(char*)osal_malloc(1024*1024);

	if (!upgrade_file_buf || !tcp_rcv_buff)
	{
		printf("malloc error !!!\n");
	}
	memset(tcp_rcv_buff, 0, RECV_MAX_SIZE + 4);

	memset(upgrade_file_buf, 0x0, 1024 * 1024);

	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));
	tcpHeader.Command = ACK_NET_UPGRADE;

	if (send(tcp_connect_fd, (char *)&tcpHeader, sizeof(TCP_SERVER_H), 0) > 0)
	{
		NetUpgrade_Info netUpgradeInfo;
		memset(&netUpgradeInfo, 0, sizeof(NetUpgrade_Info));
		recv(tcp_connect_fd, (char *)&netUpgradeInfo, sizeof(NetUpgrade_Info), 0);
		printf("netUpgradeInfo size %dK%dB ###\n", netUpgradeInfo.fileSize / 1024, netUpgradeInfo.fileSize % 1024);
		while (offset < netUpgradeInfo.fileSize)
		{
			memset(tcp_rcv_buff, 0, RECV_MAX_SIZE + 4);
			size = recv(tcp_connect_fd, tcp_rcv_buff, RECV_MAX_SIZE, 0);
			memcpy(upgrade_file_buf + offset, tcp_rcv_buff, size);
			offset += size;
			printf("recv size: %d, offset: %d(%d%%)\n", size, offset, offset * 100 / netUpgradeInfo.fileSize);
		};
		if (offset == netUpgradeInfo.fileSize)
		{
			SPI_NetUpgrade(upgrade_file_buf, offset);
			size = send(tcp_connect_fd, "Upgrade_ok", 11, 0);
			printf("NetUpgrade ok ###\n");
		}
		else
		{
			size = send(tcp_connect_fd, "Upgrade_err", 12, 0);
			printf("NetUpgrade err offset = %d, fileSize = %d ###\n", offset, netUpgradeInfo.fileSize);
		}
	}

	// free(upgrade_file_buf);
	free(tcp_rcv_buff);
}

void tcpServerAccept(struct event_info *e, void *d)
{
	int tcp_connect_fd = -1;
	struct sockaddr_in client;
	socklen_t addrlen;

	TCP_SERVER_H tcpHeader;
	memset(&tcpHeader, 0, sizeof(TCP_SERVER_H));

	addrlen = sizeof(client);
	tcp_connect_fd = accept(tcp_server_fd, (struct sockaddr *)&client, &addrlen);
	if (tcp_connect_fd < 0)
	{
		perror("accept()error\n");
	}

	if (recv(tcp_connect_fd, (char *)&tcpHeader, sizeof(TCP_SERVER_H), 0) > 0)
	{
		printf("tcpHeader.Command = %x ###\n", tcpHeader.Command);
		switch (tcpHeader.Command)
		{
		case REQ_NET_UPGRADE:
		{
			tcpNetUpgrade(tcp_connect_fd);
			break;
		}
		default:
			break;
		}
	}
	close(tcp_connect_fd);
}

void testTcpServer()
{
	uint16_t port = 5007;

	printf("[test] init tcp server: port: %d\n", port);

	tcp_server_fd = InitSrv(port);
	eloop_add_fd(tcp_server_fd, 0, 0, tcpServerAccept, 0);
}
