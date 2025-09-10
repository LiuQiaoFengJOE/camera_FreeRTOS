#include "stdio.h"
#include "string.h"

#include "osal/osal_task.h"
#include "socket_cmd_wifi.h"
#include "socket_cmd.h"
#include "osal/osal_alloc.h"
#include "fatfs/fileOps.h"
#include "fatfs/ff.h"
#include "task/app.h"
#include "lwip/sockets.h"
#include "drv/rtc/rtc.h"

#if 1
#define JRX_CMD_ID_OFFSET   8
#define JRX_CMD_SIZE_OFFSET   4

#define JRX_CMD_GET_STATUS   1
#define JRX_CMD_KEEP_ALIVE   2
#define JRX_CMD_SET_TIME     3
#define JRX_CMD_SET_AUD_TRANS     4

static int drop_clientEx = 0;
static int cmdex_service_enable = 0;	
static int cmdex_service_start = 0;	
extern u8 *getCurModeFreeBuffBase(void);
static jrxCmdSvr cmd_jrxSvr =
{
		-1, /* Server connection */
		-1, /* Client connection */
};
static INT8U cmdex_tx_buf[SOCKET_CMD_BUF_SIZE] = {0};
static INT8U cmdex_rx_buf[SOCKET_CMD_BUF_SIZE] = {0};
#define GET_WORD(n)  (((u16)((n)[1])<<8)|(u16)((n)[0]))
static u16 jrxFillTxCmd(u8 *buff,u16 size,u8 *tx,u16 txLen,u8 id)
{
	int i;
	if(10+txLen>size)
	{
		printf("tx buff small:%d,%d\n",size,10+txLen);
		return 0;
	}
	buff[0]=0x5a;
	buff[1]=0xa5;
	buff[2]=0x5a;
	buff[3]=0xa5;
	buff[4]=(u8)((txLen+2)&0xff);
	buff[5]=((txLen+2)>>8)&0xff;
	buff[6]=0xff-buff[4];
	buff[7]=0xff-buff[5];
	buff[8]=0x00;
	buff[9]=id;
	for(i=0;i<txLen;i++)
	{
		buff[10+i]=tx[i];
	}
	return 10+txLen;
}
static u16 jrxCmdGetStatus(jrxCmdSvr *jrxSvr,u8*inBuf,u16 inSize,u8 *outBuf,u16 *outSize)
{
	u8 audioTrans=rtpAudSendGet();
	return jrxFillTxCmd(outBuf,outSize,&audioTrans,1,JRX_CMD_GET_STATUS);
}
static u16 jrxCmdSetTime(jrxCmdSvr *jrxSvr,u8*inBuf,u16 inSize,u8 *outBuf,u16 *outSize)
{
	u8 response=1;
	DATE_TIME_T rtcTime;
	rtcTime.year = GET_WORD(inBuf);
	rtcTime.month =inBuf[2];
	rtcTime.day = inBuf[3];
	rtcTime.hour = inBuf[4];
	rtcTime.min = inBuf[5];
	rtcTime.sec = inBuf[6];		
	hal_rtcTimeSet(&rtcTime);
	printf("time:%d-%d-%d %d:%d:%d\n",rtcTime.year,inBuf[2],inBuf[3],inBuf[4],inBuf[5],inBuf[6]);
	return jrxFillTxCmd(outBuf,outSize,&response,1,JRX_CMD_SET_TIME);
}
static u16 jrxCmdSetAudTrans(jrxCmdSvr *jrxSvr,u8*inBuf,u16 inSize,u8 *outBuf,u16 *outSize)
{
	u8 response=1;
	printf("Aud:%d\n",inBuf[0]);
	if(inBuf[0])
		rtpAudSendSet(1);
	else
		rtpAudSendSet(0);
	return jrxFillTxCmd(outBuf,outSize,&response,1,JRX_CMD_SET_AUD_TRANS);
}

static int jrx_cmd_process(jrxCmdSvr *jrxSvr, INT8U *rxCmdBuff)
{
	INT32 ret = NAK_OK, ret1 = NAK_OK;
	u8 jrxCmdID,temp;
	u16 len = 0;
	u16 cmdSize;
	int i;

	// Check tag
	jrxCmdID = rxCmdBuff[JRX_CMD_ID_OFFSET];
	cmdSize = (u16)(rxCmdBuff[JRX_CMD_SIZE_OFFSET+1]<<8)|(u16)rxCmdBuff[JRX_CMD_SIZE_OFFSET];
	if ((rxCmdBuff[0]!=0x5a)||(rxCmdBuff[1]!=0xa5)||(rxCmdBuff[2]!=0x5a)|(rxCmdBuff[3]!=0xa5))
	{
		ret = -1;
		printf("cmdex error:skip\r\n");
		goto GP_CMD_PROCESS_END;
	}
	cmdSize+=8;
	//printf("jrx cmd:0x%x\n", jrxCmdID);
	switch (jrxCmdID)
	{
	case JRX_CMD_GET_STATUS:
		len = jrxCmdGetStatus(jrxSvr,&rxCmdBuff[JRX_CMD_ID_OFFSET+1],cmdSize,cmdex_tx_buf,SOCKET_CMD_BUF_SIZE);
		break;
	case JRX_CMD_KEEP_ALIVE:
		len=0;
		break;
	case JRX_CMD_SET_TIME:
		len = jrxCmdSetTime(jrxSvr,&rxCmdBuff[JRX_CMD_ID_OFFSET+1],cmdSize,cmdex_tx_buf,SOCKET_CMD_BUF_SIZE);
		break;
	case JRX_CMD_SET_AUD_TRANS:
		len = jrxCmdSetAudTrans(jrxSvr,&rxCmdBuff[JRX_CMD_ID_OFFSET+1],cmdSize,cmdex_tx_buf,SOCKET_CMD_BUF_SIZE);
		break;
	default:
		printf("unknown cmd:0x%x\n", jrxCmdID);
		temp=0;
		len = jrxFillTxCmd(cmdex_tx_buf,SOCKET_CMD_BUF_SIZE,&temp,1,jrxCmdID);
	}
	if(len)
	{
		ret1 = send(jrxSvr->client, (void*)cmdex_tx_buf, len, 0);	
		if(ret1 < 0)
		{
			ret = NAK_Write_Fail;
		}
	}

GP_CMD_PROCESS_END:
	return ret;
}
static int backup_cmdex_client = -1;
extern uint32_t rtsp_conn_num;
extern resetTxRetryCnt=1;
extern int drop_client;
int wifiOffCheckEx;
int stopRtpTransEx=0;
INT32U wifiOffCheckExTimeEx;

static INT32 process_cmdex(jrxCmdSvr *jrxSvr)
{
	//INT32 polling_cnt = 0;
	INT32 ret = 0;
	INT32S rxlen;
	INT8U *rx_buf;
	INT32U rx_buf_size;
	INT16 jrxCmdID;
	int err;
	wifiOffCheckEx=0;
	stopRtpTransEx=0;
	printf("new client:%d,%d\n",cmdex_service_enable,drop_clientEx);
	while (cmdex_service_enable&&drop_clientEx==0)
	{
		rx_buf = cmdex_rx_buf;
		rx_buf_size = SOCKET_CMD_BUF_SIZE;
		rxlen = recv(jrxSvr->client, (void *)rx_buf, rx_buf_size, 0);
		err=lwip_getSockError(jrxSvr->client);
		if(rxlen > 0)
		{
			wifiOffCheckEx=0;
			stopRtpTransEx=0;
			wifiOffCheckExTimeEx=os_time_get();
			jrx_cmd_process(jrxSvr, rx_buf);
		}			
		else if ( (rxlen == -1) && ((err==EAGAIN)||(err==ENOMEM)||(err==ENOBUFS)||(err==EINTR)||(err == 0)) )	//timeout
		{
			#if 1
			if(wifiOffCheckEx==0)
			{
				if(wifiOffCheckExTimeEx>os_time_get())
					wifiOffCheckExTimeEx=os_time_get();
				if(os_time_get()-wifiOffCheckExTimeEx>=3000)
				{
					//ssv6xxx_wifi_set_tx_task_retry_cnt(1);
					//ssv6xxx_set_tx_retry_cnt(1);
					resetTxRetryCnt=1;
					stopRtpTransEx=1;
					wifiOffCheckEx=1;
				}
			}
			if(wifiOffCheckEx&&!wifiStaActive())
			{
				printf("tx buff:%d,drop connect!!!\n\n",wifiGetTxCnt());
				drop_client=1;
				break;
			}
			#endif
		
			if(socket_cmd_wifi_new_client_conn(jrxSvr)==0)
				break;
			continue;			
		}
		else  // rxlen = 0 => close
		{
			printf("cmd socke:%d,:err%d,l3_errno=%d\n",jrxSvr->client,rxlen,err);
			break;
		}
	}

PROCESS_SOCKET_CMD_END:
	//dropConnAndWaite();
	/* Exit this socket to accept a new connection */
	stopRtpTransEx=0;
	printf("Exit %s\r\n", __func__);

	return ret;
}

void cmdex_task()
{
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	INT32 err;
	socklen_t client;
	struct sockaddr_in server_addr, cli_addr;
	jrxCmdSvr *jrxSvr = &cmd_jrxSvr;
	int status;
	int	enable = 0;
	int keepalive = 0;

	
	jrxSvr->server = socket(AF_INET, SOCK_STREAM, 0);
	if(jrxSvr->server < 0)
	{
		printf("SCKET_CMD failed\r\n");
		goto SOCKET_CMD_TASK_END;
	}
	status = setsockopt(jrxSvr->server, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enable, sizeof(enable));
	if(status < 0)
	{
		printf("SOCKET_CMD SO_KEEPALIVE error\r\n");
		goto SOCKET_CMD_TASK_END;
	}
	/* Set socket address reusable */
	status = setsockopt(jrxSvr->server, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(enable));
	if(status < 0)
	{
		printf("SOCKET_CMD REUSEADDR error %d\r\n", status);
		goto SOCKET_CMD_TASK_END;
	}
	/* Bind the TCP server socket address */
	memset((INT8S *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_len         = sizeof(server_addr);
	server_addr.sin_port        = htons(8082);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	status = bind(jrxSvr->server, (struct sockaddr *)&server_addr,  sizeof(server_addr));
	if(status < 0)
	{
		printf("SOCKET_CMD bind error\r\n");
		goto SOCKET_CMD_TASK_END;
	}
	/* Listen */
	if(listen(jrxSvr->server , 1) < 0)
	{
		printf("SOCKET_CMD listen error\r\n");
		goto SOCKET_CMD_TASK_END;
	}
	
	backup_cmdex_client = -1;
 	while(1)
	{
		jrxSvr->client=-1;
		while(1)
		{
			// detect next connection
			if (backup_cmdex_client < 0)
			{
				printf("wait client connect\n");
				client = sizeof(cli_addr);
				backup_cmdex_client = accept(jrxSvr->server, (struct sockaddr *)&cli_addr, &client);
				printf("client OK = %d\r\n",backup_cmdex_client);
			}
			else
				break;
		}
		jrxSvr->client = backup_cmdex_client;
		backup_cmdex_client = -1;

		setsockopt(jrxSvr->client, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
		printf("1client keepalive:%d\r\n",keepalive);
		{
			enable = SO_RCVTIMEO_TIMEOUT;		// timeout ticks
			if(setsockopt(jrxSvr->client, SOL_SOCKET, SO_RCVTIMEO, &enable, sizeof(enable)) < 0 )
			{
				printf("SO_RCVTIMEO error!\r\n");
			}
		}

		if (jrxSvr->client>=0)
		{
			memset(cmdex_tx_buf, 0, sizeof(cmdex_tx_buf));
			memset(cmdex_rx_buf, 0, sizeof(cmdex_rx_buf));
			drop_clientEx=0;
			/* A client connected */
			process_cmdex(jrxSvr);
				
			///Wifi_Disconnect();		//unlock the AD key 

			/* Close connected client socket then wait for new client connection */
			if(lwip_close(jrxSvr->client) != 0)
			{
				printf("close cmdex client error\r\n");
			}
			drop_clientEx=0;
			jrxSvr->client = -1;
			tcp_debug_print_pcbs();
		}
		else
		{
			printf("gp cmd: socket accept fail, netconn 0x%x 0x%x\r\n",jrxSvr->server, jrxSvr->client);
		}
	}
SOCKET_CMD_TASK_END:
		printf("%s error\r\n",__func__);
}


void cmdex_service_init(void)
{
	osal_thread_create(cmdex_task, NULL, 22, 0, 1024, "cmdex");
}

void cmdex_start_service(void)
{
#if 1
	cmdex_service_enable = 1;
#endif
}

void cmdex_stop_service(void)
{
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	int times = 100;
	cmdex_service_enable = 0;
	//if (cmdex_service_start == 1)
	if(cmd_jrxSvr.client>=0)
	{
		cmdex_service_enable = 0;
		drop_clientEx=1;
		/* waiting for mpjeg service stop */
		//while (cmdex_service_start == 1)
		while(cmd_jrxSvr.client>=0)
		{
			drop_clientEx=1;
			osal_task_sleep(30);
			if (--times <= 0)
			{
				break;
			}
		}

		if (times <= 0)
		{
			printf("cmd service stop failed\r\n");
		}
		else
		{
			printf("cmd service stop success\r\n");
		}
	}
	else
	{
		printf("cmd service already stop\r\n");
	}
}
void cmdex_drop_client(void)
{
#if (JR_APP_PROTOCOL==0)
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	int times = 100;
	drop_clientEx=1;
	if(cmd_jrxSvr.client>=0)
	{
		drop_clientEx=1;
		while(cmd_jrxSvr.client>=0)
		{
			drop_clientEx=1;
			osal_task_sleep(10);
			if (--times <= 0)
			{
				break;
			}
		}

		if (times <= 0)
		{
			printf("cmd service stop failed\r\n");
		}
		else
		{
			printf("cmd service stop success\r\n");
		}
	}
	else
	{
		printf("cmd service already stop\r\n");
	}
#endif
}

#endif
