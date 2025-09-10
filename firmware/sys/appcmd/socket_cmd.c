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

#if 1
// josephhsieh_debug
#define PLAYFILE_INDEX_TABLE_SIZE (9999 / 512 + 1) // Each 512 files creates a record.

INT8U upgrade_exit(INT32U upgrade_type)
{
}

#define MJ_STREAM_TIMEOUT 10000 // unit: ms
#define SOCKET_CMD_TIMEOUT 1200 // unit: 100ms

// extern INT8U ap_setting_right_menu_active(STRING_INFO *str, INT8U type, INT8U *sub_tag);
// extern INT16U present_state;

char wifi_ssid[JRXSOCK_WiFi_Name_Length + 1] = "GPLUSPRO";
char mac_addr[6];
char wifi_password[JRXSOCK_WiFi_Passwd_Length + 1] = "12345678";
INT32U mjpeg_service_ready_flag = 1;	// mjpeg service �ǳƦn�F
INT32U mjpeg_service_playstop_flag = 0; // ����^�񪺦w���X��
INT32U mjpeg_service_playjpeg_flag = 0; // �U�O�^�� JPEG�A��ѥX JPEG
INT32U g_wifi_notify_pic_done = 0;
INT32U g_wifi_sd_full_flag = 0;

static int wifi_mode_bak = -1;

int drop_client = 0;
static int cmd_service_enable = 0;							   /* cmd service enable flag */
static int cmd_service_start = 0;							   /* cmd service status flag */
static INT32U firmware_download_size = 0;					   // frimware total size
static INT32U firmware_download_checksum = 0;
static INT32U firmware_upgrade_flag = 0;
static INT8U *pfirmware = NULL;
static INT8U *pfirmware_cur = NULL;
static INT32U firmware_cur_size = 0;
static INT8U *prx_firmware_buf;
extern u8 *getCurModeFreeBuffBase(void);

const INT8U LFSR_LUT[16] =
	{
		0x0E, 0x47, 0xDB, 0x46, 0x46, 0x8D, 0x38, 0xE5,
		0xFC, 0x52, 0x7A, 0xDE, 0x6F, 0xC5, 0x05, 0xE6};

INT8U g_LFSRseed[4];
INT8U g_LFSR_Key[6];
INT8U _Gen_LFSR_32Bit_Key(void)
{
	INT8U tc_period, q;
	INT16U tw_LFSR_Key;
	INT16U tw_LFSR_MagicNum;

	g_LFSR_Key[0] = 0xe6; //(INT8U)(*((volatile INT32U *) 0xD0500380));
	g_LFSR_Key[1] = 0x9b; //(INT8U)(*((volatile INT32U *) 0xD0500384));
	g_LFSR_Key[2] = 0xc7; //(INT8U)(*((volatile INT32U *) 0xD0500380));
	g_LFSR_Key[3] = 0xa2; //(INT8U)(*((volatile INT32U *) 0xD0500384));

	((INT8U *)(&tw_LFSR_MagicNum))[0] = g_LFSRseed[0] ^ g_LFSR_Key[0];
	((INT8U *)(&tw_LFSR_MagicNum))[1] = g_LFSRseed[1] ^ g_LFSR_Key[1];
	((INT8U *)(&tw_LFSR_Key))[0] = g_LFSRseed[2] ^ g_LFSR_Key[2];
	((INT8U *)(&tw_LFSR_Key))[1] = g_LFSRseed[3] ^ g_LFSR_Key[3];

	tc_period = (((INT8U)tw_LFSR_MagicNum) ^ ((INT8U)(tw_LFSR_MagicNum >> 8))) & 0x0F;
	tc_period = ((LFSR_LUT[tc_period] ^ g_LFSRseed[0]) + (LFSR_LUT[15 - tc_period] ^ g_LFSRseed[1]));
	for (q = 0; q < tc_period; q++)
	{
		INT8U tb_bit = (INT8U)((tw_LFSR_Key ^ (tw_LFSR_Key >> 2) ^ (tw_LFSR_Key >> 3) ^
							(tw_LFSR_Key >> 5)) &
						   0x01);
		tw_LFSR_Key = (((INT16U)tb_bit) << 15) | (tw_LFSR_Key >> 1);
	}
	if (0) //(((*((volatile INT32U *) 0xC015000C)) & 0x00000100) == 0)
	{
		// patch area
		tw_LFSR_Key += 77;
		g_LFSR_Key[1] += 0x77;
		g_LFSR_Key[4] = ((INT8U *)(&tw_LFSR_Key))[0];
		g_LFSR_Key[5] = ((INT8U *)(&tw_LFSR_Key))[1];
		g_LFSR_Key[2] -= 0x76;
	}
	else
	{
		g_LFSR_Key[4] = ((INT8U *)(&tw_LFSR_Key))[0];
		g_LFSR_Key[5] = ((INT8U *)(&tw_LFSR_Key))[1];
	}
	return 0;
}
INT8U _Gen_32Bit_Key(INT8U *seed)
{
	INT32U i, idx;
	INT8U t_checksum, t_ret;

	for (i = 0; i < 4; ++i)
	{
		idx = (seed[i] + (seed[i] >> 4)) & 0xF;
		if (0) //(((*((volatile INT32U *) 0xC015000C)) & 0x00000100) == 0)
		{
			seed[i] &= 0x80;
			seed[i] |= 0x55;
		}
		else
		{
			t_ret = 1; //_drv_l1_jpeg_staus_get();
			if (t_ret)
			{
				t_checksum += seed[i];
				t_checksum += g_LFSRseed[i];
				t_checksum += LFSR_LUT[i];
				// R_RANDOM0 = t_checksum;
				g_LFSR_Key[i] = g_LFSRseed[i] ^ LFSR_LUT[idx];
			}
			else
			{
				seed[i] &= 0x55;
				seed[i] |= 0x78;
			}
		}
	}
	return 0;
}

INT8U jrx_wifi_upgrade_check(INT8U **p_Firmware, INT32U *p_Size)
{
	INT8U ret = 0;

	if ((pfirmware != NULL) && (firmware_download_size != 0))
	{
		ret = 1;
		*p_Firmware = pfirmware;
		*p_Size = firmware_download_size;
	}

	return ret;
}

void jrx_capture_card_full(void)
{
	g_wifi_notify_pic_done = 0;
}
int curPlayBackMode(void)
{
	if (get_current_workmode()==WM_WEBFILE)
		return 1;
	return 0;
}
void modeReset(void)
{
	wifi_mode_bak=RECORD_MODE;
	live_mode_trans();
}

static int jrx_mode_set(int wifi_mode)
{
	printf("%s:wifi_mode:%d, cur_mode:%d\n", __func__, wifi_mode, wifi_mode_bak);
	INT32 ret = 0;
	wifi_mode_bak = wifi_mode;
	if((wifi_mode==BROWSE_MODE&&(get_current_workmode()!=WM_WEBFILE))||\
		(((wifi_mode==RECORD_MODE)||(wifi_mode==CAPTURE_MODE))&&(get_current_workmode()!=WM_LIVE)))
	{
		dropConnAndWaite();
		if (wifi_mode == BROWSE_MODE)
		{
			webfile_mode_trans();
		}
		else
		{
			live_mode_trans();
		}
	}
	return 0;
}

static jrxCmdSvr socket_socket_ctl_blk =
	{
		-1, /* Server connection */
		-1, /* Client connection */
};

static INT8U socket_cmd_tx_buf[SOCKET_CMD_BUF_SIZE+1] = {0};
static INT8U socket_cmd_rx_buf[SOCKET_CMD_BUF_SIZE] = {0};
static INT32U socket_cmd_stack[SOCKET_CMD_TASK_STACKSIZE];

//
// Purpose: Prepare the server->client response buffer
// Return: Length of the response buffer
// Note:
//
// Server -> client response format:
//
// Low byte                                                                  High byte
// +-------------+--------------+-----------------+----------------+-----------------+
// | JRX_SOCK_Tag | JRX_SOCK_Type | JRX_SOCK_Mode_ID | JRX_SOCK_Cmd_ID | JRX_SOCK_Payload |
// +-------------+--------------+-----------------+----------------+-----------------+
//      8byte         2byte           1byte              1byte            Nbyte
//
// JRX_SOCK_Type   : 2(ACK), 3(NAK)
// JRX_SOCK_Mode_ID: Same as client packet
// JRX_SOCK_Cmd_ID : Same as client packet
// JRX_SOCK_Payload(ACK): Byte[0]~[1]: Payload size
//                       Byte[2]~[N]: Payload data
// JRX_SOCK_Payload(NAK): Byte[0]~[1]: Error code
//
static INT32 jrx_resp_set(INT32U type, INT16U payload_attr, INT8U *payload_data, INT32U payload_len)
{
	INT8U *resp_type = (INT8U *)(socket_cmd_tx_buf + JRX_SOCK_RESP_TYPE_OFFSET);
	INT16U *resp_payload_attr = (INT16U *)(socket_cmd_tx_buf + JRX_SOCK_RESP_PAYLOAD_SIZE_OFFSET);
	CHAR *resp_payload_data = (CHAR *)(socket_cmd_tx_buf + JRX_SOCK_RESP_PAYLOAD_OFFSET);
	CHAR *p = (CHAR *)(&type);

	// Write "JRX_SOCK_Type + JRX_SOCK_Mode_ID + JRX_SOCK_Cmd_ID"
	// endian convert
	resp_type[0] = p[2];
	resp_type[1] = p[3];
	resp_type[2] = p[1];
	resp_type[3] = p[0];

	// Write "JRX_SOCK_Payload"
	*resp_payload_attr = payload_attr;
	memcpy((INT8 *)resp_payload_data, (INT8 *)payload_data, (INT32U)payload_len);

	return (payload_len + JRX_SOCK_RESP_PAYLOAD_OFFSET);
}

static INT32 JRXSOCK_General_SetMode_Decode(INT16U jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
	INT32 mode = 0;

	// Byte 0 of SetMode's payload is 'mode'
	mode = (INT32)rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET];
	jrx_mode_set(mode);

	return ret;
}
#if 0
static int General_GetParameterFile_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U* rxstring)
{
	INT32 ret = NAK_OK;
#if 1
	INT16* pGP_Cmd = (INT16 *)(socket_cmd_tx_buf+JRX_SOCK_CMD_TYPE_OFFSET);
	INT8U  *pGP_ID = (INT8U *)(socket_cmd_tx_buf+JRX_SOCK_CMD_MODE_ID_OFFSET);
	INT8U  *p = (INT8U*)(&jrx_sock_cmd);
	u32 xml_len;
	u32 xml_fd;

	xml_fd = res_open(RES_WIFI_CONFIG);
	if (xml_fd == 0)
	{
		ret = -1;
		printf("Open XML(Wifi configuration) Error\r\n");
		goto CMD_GETPARAMETERFILE_DECODE_END;
	}

	xml_len = res_size(RES_WIFI_CONFIG);
	*pGP_Cmd = JRX_SOCK_TYPE_ACK;
	*pGP_ID++ = p[1];		// MODE
	*pGP_ID++ = p[0];		// CMD_ID
	pGP_Cmd += 2;	// point to payload
	printf("xml_len = %d\r\n",xml_len);
	while (xml_len > 0)
	{
		INT32 err = 0;
		if (xml_len > (SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET))
		{
			*pGP_Cmd = (short)(SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);		// continuous frame
			res_read(xml_fd,  (u32)(socket_cmd_tx_buf+JRX_SOCK_RESP_PAYLOAD_OFFSET), SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
			xml_fd += SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET;
			xml_len -= (SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
		}
		else
		{
			*pGP_Cmd = (short)xml_len;		// last frame
			res_read(xml_fd,  (u32)(socket_cmd_tx_buf+JRX_SOCK_RESP_PAYLOAD_OFFSET), xml_len);
			xml_len = 0;
		}
		// printf("flag = %d\r\n",flag);

		err = send(ctl_blk->client, (void*)socket_cmd_tx_buf, (*pGP_Cmd+JRX_SOCK_RESP_PAYLOAD_OFFSET), 0);
		if (err <0)
		{
			printf("xml get error\r\n");
		}
	}
	printf("xml  finish\r\n");
#endif
CMD_GETPARAMETERFILE_DECODE_END:
	return ret;
}
#else
static int General_GetParameterFile_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U* rxstring)
{
	INT32S ret = NAK_OK;
	INT16S* pGP_Cmd = (INT16S *)(socket_cmd_tx_buf+JRX_SOCK_CMD_TYPE_OFFSET);
	INT8U  *pGP_ID = (INT8U *)(socket_cmd_tx_buf+JRX_SOCK_CMD_MODE_ID_OFFSET);
	INT8U  *p = (INT8U*)(&jrx_sock_cmd);
	INT32S xml_len;
	int xmlBuffPos=0;
	int xmlSize=xmlExpectedSize();
	u8 *menuXml=osal_malloc(xmlSize);
	if(menuXml==NULL)
	{
		return NAK_Invalid_command;
	}
	xml_len = xmlCreate(menuXml,xmlSize);
	*pGP_Cmd = JRX_SOCK_TYPE_ACK;
	*pGP_ID++ = p[1];		// MODE
	*pGP_ID++ = p[0];		// CMD_ID
	pGP_Cmd += 2;	// point to payload
	printf("xml_len = %d\r\n",xml_len);
	while (xml_len > 0)
	{
		INT32S err = 0;
		int flag = 0;

		xml_len -= (SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
		if (xml_len > 0)
		{
			*pGP_Cmd = (short)(SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);		// continuous frame
			memcpy((socket_cmd_tx_buf+JRX_SOCK_RESP_PAYLOAD_OFFSET),&menuXml[xmlBuffPos], SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
			xmlBuffPos+=(SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
		}
		else
		{
			*pGP_Cmd = (short)(xml_len + SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);		// last frame
			memcpy((socket_cmd_tx_buf+JRX_SOCK_RESP_PAYLOAD_OFFSET),&menuXml[xmlBuffPos], xml_len + SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
			xmlBuffPos+=(xml_len + SOCKET_CMD_BUF_SIZE-JRX_SOCK_RESP_PAYLOAD_OFFSET);
		}
		err = send(ctl_blk->client, (void*)socket_cmd_tx_buf, (*pGP_Cmd+JRX_SOCK_RESP_PAYLOAD_OFFSET), 0);
		if (err <0)
			printf("gp cmd: netconn_write fail, err %d\r\n", err);

	}
	osal_free(menuXml);
	printf("xml  finish\r\n");
CMD_GETPARAMETERFILE_DECODE_END:
	return ret;
}

#endif
static int General_GetDeviceStatus_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned char *buf)
{
	int ret = NAK_OK;
#if 1
	int val;
	char *p_val = (char *)(&val);
	//char adp_status = 0;
	//unsigned char flag = 0;
	char device_status = 0;

	if (wifi_mode_bak == RECORD_MODE)	// record mode
	{
		if(videoIsRecording())
			device_status = 1;
	}
	else if (wifi_mode_bak == CAPTURE_MODE)
	{
		if (takePictrueIsWorking())
			device_status = 1;
	}
	else if (wifi_mode_bak == BROWSE_MODE)		// browser mode
	{
		if (!getPlayPause())
			device_status = 1;
	}
	if (menuGetNetMsgVal(Record_RecordAudio))
	{
		device_status |= 0x2;
	}
	// buf[3]
	//device_status=0;
	buf[0] = wifi_mode_bak; // current mode
	buf[1] = device_status;
	buf[2] = getCurBatteryLevel();
	buf[2] |= 0x80; // Notify GoPlusCam to use rstp flow.
	if(getUsbConnect())
		buf[3] = 1;
	else
		buf[3] = 0;
#if 0 // SUPPORT_APP_DEL_FILE == CUSTOM_ON
	buf[3] |= 0x80;
#endif

	/////////// Add recording status
	// ap_display_timer();
	if(videoIsRecording())
		val = videoRecordGetTime();
	else
		val = vidRecordRemainTime();
	buf[4] = menuGetNetMsgVal(Record_Resolution);
	buf[5] = p_val[0];
	buf[6] = p_val[1];
	buf[7] = p_val[2];
	buf[8] = p_val[3];
	// printf("%d ",val);

	/////////// Add capture status
	val = takePhotoRemainSum();	// josephhsieh_debug
	buf[9] = menuGetNetMsgVal(Capture_Resolution); // ap_state_config_pic_size_get();
	buf[10] = p_val[0];
	buf[11] = p_val[1];
	buf[12] = p_val[2];
	buf[13] = p_val[3];
	buf[15] = appGetSdStatus();
// printf("%d\r\n",val);
#endif
	return ret;
}

static int JRXSOCK_General_PowerOff_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
#if 0
	msgQSend(ApQ, MSG_APQ_POWER_KEY_ACTIVE, NULL, NULL, MSG_PRI_NORMAL);
#endif
	return NAK_OK;
}

static int General_RestarStreaming_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	int ret = NAK_Request_timeout;
	int i;
	//dropConnAndWaite();
	return NAK_OK;
	for (i = 0; i < 30; ++i)
	{
		if (mjpeg_service_ready_flag)
		{
			ret = NAK_OK;
			break;
		}
		printf("General_RestarStreaming_Decode\r\n");
		osal_task_sleep(10);
	}
	osal_task_sleep(30); // �����W�c�� mode �y���������`
	return ret;
}

static int JRXSOCK_General_AuthDevice_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned char *buf)
{
#if 1
	INT8U *seed = (INT8U *)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET]);
	g_LFSRseed[0] = seed[0];
	g_LFSRseed[1] = seed[1];
	g_LFSRseed[2] = seed[2];
	g_LFSRseed[3] = seed[3];
	_Gen_LFSR_32Bit_Key();
	//printf("Auth:g_LFSRseed: [%x,%x,%x,%x]\r\n", g_LFSRseed[0], g_LFSRseed[1], g_LFSRseed[2], g_LFSRseed[3]);
	buf[0] = g_LFSR_Key[0];
	buf[1] = g_LFSR_Key[1];
	buf[2] = g_LFSR_Key[2];
	buf[3] = g_LFSR_Key[3];
	buf[4] = g_LFSR_Key[4];
	buf[5] = g_LFSR_Key[5];
	//printf("Auth:g_LFSR_Key: [%x,%x,%x,%x,%x,%x]\r\n", g_LFSR_Key[0], g_LFSR_Key[1], g_LFSR_Key[2], g_LFSR_Key[3], g_LFSR_Key[4], g_LFSR_Key[5]);
#endif
	return 0;
}

static int JRXSOCK_General_RTSP_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned char *buf)
{
#if 1
	INT8U *seed = (INT8U *)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET]);
	g_LFSRseed[0] = seed[0];
	g_LFSRseed[1] = seed[1];
	g_LFSRseed[2] = seed[2];
	g_LFSRseed[3] = seed[3];
	_Gen_32Bit_Key(seed);
	//printf("RTSP:g_LFSRseed: [%x,%x,%x,%x]\r\n", g_LFSRseed[0], g_LFSRseed[1], g_LFSRseed[2], g_LFSRseed[3]);
	buf[0] = g_LFSR_Key[0];
	buf[1] = g_LFSR_Key[1];
	buf[2] = g_LFSR_Key[2];
	buf[3] = g_LFSR_Key[3];
	//printf("RTSP:g_LFSR_Key: [%x,%x,%x,%x]\r\n", g_LFSR_Key[0], g_LFSR_Key[1], g_LFSR_Key[2], g_LFSR_Key[3]);
#endif
	return 0;
}

static int JRXSOCK_Record_Start_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
#if 1
	INT16U cnt = 0;

	if(get_current_workmode()!=WM_LIVE)
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	if(get_sd_status()==0)
	{
		ret = NAK_No_storage;
		goto JRXSOCK_RECORD_START_DECODE_END;
	}
	if (videoIsRecording())
		sendMsgFrontAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
	else
		sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);

JRXSOCK_RECORD_START_DECODE_END:
#endif
	return ret;
}

static int JRXSOCK_Record_Audio_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
#if 1
	INT8U flag = 0;

	flag = menuGetNetMsgVal(Record_RecordAudio);
	if(flag)
	{
		menuSetNetMsgVal(Record_RecordAudio,0);;
	}
	else
	{
		menuSetNetMsgVal(Record_RecordAudio,1);;
	}
#endif
JRXSOCK_RECORD_AUDIO_DECODE_END:
	return ret;
}

static int JRXSOCK_CapturePicture_Capture_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
#if 1
	int cnt = 0;

	if(get_current_workmode()!=WM_LIVE)
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	if(get_sd_status()==0)
	{
		ret = NAK_No_storage;
		goto JRXSOCK_RECORD_START_DECODE_END;
	}
	localTaketPhotoStart();
	int delay=getTakePhotoTime();
	delay=delay*1000+2000;
	while(takePictrueIsWorking())
	{
		osal_task_sleep(10);
		cnt+=10;
		if(cnt>delay)
		{
			ret = NAK_Request_timeout;
			break;
		}
	}

JRXSOCK_RECORD_START_DECODE_END:
#endif
	return ret;
}


#define PLAYBACK_START_TIME_OUT 200
static u32 playFileIndex;
u32 getPlayFileIndex(void)
{
	return playFileIndex;
}
static int JRXSOCK_Playback_Start_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32U i;
	INT32 ret = NAK_OK;
#if 1
	if (get_sd_status() == 0)
	{
		ret = NAK_No_storage;
		goto JRXSOCK_PLAYBACK_START_DECODE_END;
	}
	playFileIndex = rxstring[13] << 8 | rxstring[12];
#endif
JRXSOCK_PLAYBACK_START_DECODE_END:

	printf("Playback_Start ....%d\n", playFileIndex);

	return ret;
}

static int JRXSOCK_Playback_Pause_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
	INT16U cnt = 0;

#if 1 // josephhsieh_debug
	if (rxstring[12] == 'J')
	{
		// JPEG ��
		//ap_peripheral_ok_key_exe(&cnt);
		//ap_peripheral_ok_key_exe(&cnt);
		printf("JRXSOCK_JPG_Pause_Resume\r\n");
	}
	else
	{
		// AVI ��
		//msgQSend(ApQ, MSG_APQ_MJPEG_DECODE_PAUSE, NULL, NULL, MSG_PRI_NORMAL);
		if(getPlayPause())
		{
			playPauseSet(0);
			printf("paly resume\n");
		}
		else
		{
			playPauseSet(1);
			printf("paly pause\n");
		}
		
	}
#endif

	return ret;
}

static int Menu_GetParameter_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned char *buf)
{
	int ret = 1;
#if 1
	int *pID = (int*)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET]);
	INT32U ssid_length;

	switch (*pID)
	{
	// ===============================================
	// RECORD
	// ===============================================
	case Record_Resolution:
	case Record_Exposure:
	case Record_MotionDetection:
	case Record_Loop_Recording:
	case Record_RecordAudio:
	case Record_DateStamp:
	case Record_WDR:
	// ===============================================
	// CAPTURE
	// ===============================================
	case Capture_Resolution:
	case Capture_Exposure:
	case Capture_Quality:
	case Capture_Sequence:
	case Capture_Sharpness:
	case Capture_ISO:
	case Capture_AntiShaking:
	case Capture_DateTime:
	case Capture_WhiteBalance:
	// ===============================================
	// SYSTEM
	// ===============================================
	case System_Frequency:
	case System_ScreenSaver:
	case System_AutoPowerOff:
	case System_Language:
	case System_BeepSound:
	case System_DataTime:
		buf[0] = menuGetNetMsgVal(*pID);
		break;
	case WiFi_Name:
		ret = ssid_length=strlen(get_wifi_ssid_string())+1;
		memcpy(buf,get_wifi_ssid_string(),ssid_length);
		//JRXSOCK_WiFi_Name_Length
		break;
	case WiFi_Passwd:
		ret = ssid_length=strlen(getWifiPwString())+1;
		memcpy(buf,getWifiPwString(),ssid_length);
		break;
	case System_ClearBuffer:
	case System_Format:
	case System_DefaultSetting:
	case System_Version:
	default:
		buf[0] = 0;
	}
	printf("ID = 0x%x  Data:0x%x\r\n",*pID,buf[0]);
#endif
	return ret;
}

enum
{
	MENU_FUNCTION_ACTIVE = 0x2,
	SetParameterDataOffset = 0x5
};
static int Menu_SetParameter_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	int ret = NAK_OK;
#if 1
	int id = *((int*)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET]));
	unsigned char value = rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET+SetParameterDataOffset];
	char *pValue = &rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET+SetParameterDataOffset];
	printf("set:0x%x, 0x%x\r\n",id,value);
	switch (id)
	{
	// ===============================================
	// RECORD
	// ===============================================
	case Record_Resolution:
	case Record_Exposure:
	case Record_MotionDetection:
	case Record_Loop_Recording:
	case Record_WDR:
	case Record_RecordAudio:
	case Record_DateStamp:
	// ===============================================
	// CAPTURE
	// ===============================================
	case Capture_Resolution:
	case Capture_Exposure:
	case Capture_Quality:
	case Capture_Sequence:
	case Capture_Sharpness:
	case Capture_ISO:
	case Capture_AntiShaking:
	case Capture_DateTime:
	case Capture_WhiteBalance:
	// ===============================================
	// SYSTEM
	// ===============================================
	case System_Frequency:
	case System_ScreenSaver:
	case System_AutoPowerOff:
	case System_Language:
	case System_BeepSound:
	case System_DataTime:
		menuSetNetMsgVal(id,value);
		break;
//        case System_TVMode:
//            ap_state_config_tv_out_set(value);
//            break;
	case System_ClearBuffer:
		break;
	case System_Format:
		if(appGetSdStatus()==0)
		{
			ret = NAK_No_storage;
			break;
		}
		if(formatSD())
			ret = NAK_Server_is_busy;
		break;
	case System_DefaultSetting:
		userConfigReset();
		configSystem();	
		break;
	case System_Version:
		break;
	case WiFi_Name:
		setWifiSSID(pValue);
	break;
	case WiFi_Passwd:
		setWifiPWD(pValue);
	break;
	default:
		ret = NAK_OK;
		break;
	}
#endif
	return ret;
}
static int JRXSOCK_Playback_GetFileCount_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
	INT32 i;
	INT8U packet_const[12] = {0x47, 0x50, 0x53, 0x4F, 0x43, 0x4B, 0x45, 0x54, 0x02, 0x00, 0x03, 0x02};
	memcpy((INT8 *)socket_cmd_tx_buf, (INT8 *)packet_const, 12);
	changeDir(AVI_FILE);
	ret = getFileSum();
	printf("total  %d  files\r\n", ret);
	if(get_sd_status()==0)
	{
		ret = NAK_No_storage;
	}
	return ret;
}

// rxstring : input stream
// socket_cmd_tx_buf : output steram
// return: <0  ===> error
//         >=0 ===> length of output steram
#define GET_FILE_TIME   0
static int JRXSOCK_Playback_GetFileList_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	char *name;
	FILINFO fno;
	INT32 size,avitime;
	void *fp;
	u8 *p_file_info;
	INT16U play_file_index,fileSum;
	int type;
	int number;
	INT8U packet_const[12] = {0x47, 0x50, 0x53, 0x4F, 0x43, 0x4B, 0x45, 0x54, 0x02, 0x00, 0x03, 0x03};
	if(get_current_workmode()!=WM_WEBFILE)
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	if(*(rxstring+12) == 1)
	{
		play_file_index = 0xffff;
	}
	else
	{
		play_file_index = *(rxstring+14);
		play_file_index = play_file_index<<8;
		play_file_index |= *(rxstring+13);
		if(play_file_index >= getFileSum())
		{
			return NAK_Invalid_command;
		}
	}
	memcpy((INT8 *)socket_cmd_tx_buf, (INT8 *)packet_const, 12);
	p_file_info = &socket_cmd_tx_buf[12+3];
	
	//*(p_file_info++) = 1 + 13 * FILE_SUM;
	//*(p_file_info++) = 0;
	//*(p_file_info++) = FILE_SUM;
	play_file_index++;
	fileSum=0;
	for (; play_file_index < getFileSum(); play_file_index++)
	{
		name=getFileByIdx(play_file_index,&type);
		//name=getFileByIdxEx(play_file_index,&number,&type);
		if(name==NULL)
			break;
		#if GET_FILE_TIME
		avitime=0;
		if(type==1||type==2)
		{
			fp = osal_fopen(name, "rb");
			if(fp)
			{
				avitime=getAviTotalTime(fp);
				printf("%d:%d\n",play_file_index,avitime);
				osal_fclose(fp);
			}
		}
		#endif
		f_stat(name, &fno);
		if(type==1)
			*(p_file_info++) = 'A';
		else if(type==2)
			*(p_file_info++) = 'S';
		else
			*(p_file_info++) = 'J';
		fno.fsize=fno.fsize>>10;
		*(p_file_info++) = play_file_index & 0xFF;
		*(p_file_info++) = (play_file_index >> 8) & 0xFF;
		*(p_file_info++) = ((fno.fdate >> 9) & ((1 << 7) - 1)) + 1980 - 2000;
		*(p_file_info++) = (fno.fdate >> 5) & ((1 << 4) - 1);
		*(p_file_info++) = (fno.fdate >> 0) & ((1 << 5) - 1);
		*(p_file_info++) = (fno.ftime >> 11) & ((1 << 5) - 1);
		*(p_file_info++) = (fno.ftime >> 5) & ((1 << 6) - 1);
		*(p_file_info++) = ((fno.ftime >> 0) & ((1 << 5) - 1)) * 2;
		*(p_file_info++) = fno.fsize & 0xFF;
		*(p_file_info++) = (fno.fsize >> 8) & 0xFF;
		*(p_file_info++) = (fno.fsize >> 16) & 0xFF;
		*(p_file_info++) = (fno.fsize >> 24) & 0xFF;
		#if GET_FILE_TIME
		*(p_file_info++) = avitime & 0xFF;
		*(p_file_info++) = (avitime>>8) & 0xFF;
		*(p_file_info++) = (avitime>>16) & 0xFF;
		*(p_file_info++) = (avitime>>24) & 0xFF;
		#endif
		fileSum++;
		if(fileSum>=16)
			break;
	}
	p_file_info = &socket_cmd_tx_buf[12];
	#if GET_FILE_TIME
	size = 1 + 17 * fileSum;
	#else
	size = 1 + 13 * fileSum;
	#endif
	*(p_file_info++) = size&0xff;
	*(p_file_info++) = (size>>8)&0xff;
	*(p_file_info++) = fileSum;
	
	if (fileSum)
	{
		size = (size + JRX_SOCK_RESP_PAYLOAD_OFFSET); 
	}
	else
	{
		printf("GetFileList Error!!\r\n");
		return NAK_Get_file_list_fail;
	}
	printf("GetFileList sum:%d\n",fileSum);
	return size;
}
extern uint32 getAviFirstPicTime(void *fp,uint32 *pos,uint32 *time);
static int JRXSOCK_Playback_GetThumbnail_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	#define THUMBNAIL1_W  320
	#define THUMBNAIL1_H  240
	INT32 ret = NAK_OK, sc_ret;
#if 1
	char *name;
	INT8U *mBuf = NULL;
	u8 *yuvBuff=NULL;
	INT32U size, pos;
	INT32 avitime=0;
	INT8U *buf = 0;
	INT16U play_file_index;
	int type;
	INT8U jpgFE[14]={0,0xFF, 0xFE, 0x00, 0x0B,'T','I','M','E'};
	INT8U packet_const[12] = {0x47, 0x50, 0x53, 0x4F, 0x43, 0x4B, 0x45, 0x54, 0x02, 0x00, 0x03, 0x04};
	INT8U app[12];
	play_file_index = *(rxstring + 13);
	play_file_index = play_file_index << 8;
	play_file_index |= *(rxstring + 12);
	void *fp;
	if(get_current_workmode()!=WM_WEBFILE)
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	changeDir(AVI_FILE);
	printf("thumbnail index:%d\n", play_file_index);
	//sprintf(filePath, "VID%05d.AVI", play_file_index);
	name=getFileByIdx(play_file_index,&type);
	if (name == NULL)
		return NAK_Get_thumbnail_fail;
	fp = osal_fopen(name, "rb");
	if (fp == NULL)
		return NAK_Get_thumbnail_fail;
	if(type==3)
	{
		size = osal_fsize(fp);
		pos=0;

		osal_fseek(fp, osal_fsize(fp)-sizeof(app));
		osal_fread(app, 1, sizeof(app), fp);
		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			//offset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
			pos = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
			size = osal_fsize(fp) - pos;
		}
		else
		{
			osal_fseek(fp, 0);	
		}
	}
	else
		//size = getAviFirstPic(fp, &pos);
		size = getAviFirstPicTime(fp, &pos,&avitime);
	if (size == 0|| size>800*1024)
	{
		osal_fclose(fp);
		return NAK_Get_thumbnail_fail;
	}
	if (modeFreeSpace()<size+MALIG(THUMBNAIL1_W,32)*THUMBNAIL1_H*3/2+1024)
	{
		mBuf = osal_malloc(size+MALIG(THUMBNAIL1_W,32)*THUMBNAIL1_H*3/2+1024);
		while(mBuf == NULL)
		{
			printf("no memory for generate Thumbnail!!!,%dKB\n\n",modeFreeSpace()>>10);
		}
		buf = ((u32)mBuf+0x3f)&(~0x3f);
	}
	else
	{
		buf = ((u32)getCurModeFreeBuffBase()+0x3f)&(~0x3f);
	}
	
	if(size<20*1024)
	{
		osal_fseek(fp, pos);	
		osal_fread(buf, 1, size, fp);
	}
	else
	{
		yuvBuff=buf;
		buf=((u32)(buf+MALIG(THUMBNAIL1_W,32)*THUMBNAIL1_H*3/2+0x3f))&(~0x3f);
		osal_fseek(fp, pos);
		u32 time=os_time_get();
		dcache_flush_region((u32)buf,size);
		osal_fread(buf, 1, size, fp);
		//printf("jpg size:%dKB\n",size>>10);
		not_isr_jpgdec2(yuvBuff,buf,size,THUMBNAIL1_W,THUMBNAIL1_H);
		size=not_isr_jpgEnc(yuvBuff,THUMBNAIL1_W,THUMBNAIL1_H,THUMBNAIL1_W,THUMBNAIL1_H,buf,buf+20*1024);
		//printf("get thumbnail time:%d,size:%dKB\n",os_time_get()-time,size>>10);
	}
	
	buf-=JRX_SOCK_RESP_PAYLOAD_OFFSET;
	memcpy(buf, (INT8 *)packet_const, 12);
	size+=14;
	buf[12] = (size)&0xff;
	buf[13] = (size >> 8) & 0xff;
	buf[JRX_SOCK_RESP_PAYLOAD_OFFSET+5]=0x11;
	sc_ret = send(ctl_blk->client, buf, 20 + JRX_SOCK_RESP_PAYLOAD_OFFSET,0);
	jpgFE[8+1] = (INT8U)((avitime >> 0x00)&0xFF);
	jpgFE[9+1] = (INT8U)((avitime >> 0x08)&0xFF);
	jpgFE[10+1] = (INT8U)((avitime >> 0x10)&0xFF);
	jpgFE[11+1] = (INT8U)((avitime >> 0x18)&0xFF);
	jpgFE[12+1] = 0x72;
	sc_ret = send(ctl_blk->client, jpgFE, 14,0);
	sc_ret = send(ctl_blk->client, buf+ 20 + JRX_SOCK_RESP_PAYLOAD_OFFSET, size-20-14,0);
	if(sc_ret < 0)
	{
		if (mBuf)
			osal_free(mBuf);
		mBuf = NULL;
		osal_fclose(fp);
		printf("thumbnail get error\r\n");
		return NAK_Get_thumbnail_fail;
	}
	osal_fclose(fp);
	if (mBuf)
		osal_free(mBuf);
	mBuf = NULL;	
#endif
	printf("thumbnail %d\n", size);
	// printf("Thnail:%d(%d)\r\n",play_file_index,ret);
	return ret;
}
extern uint32 getAviThumbnailTime(void *fp,uint32 *pos,uint32 *time);
static int JRXSOCK_Playback_GetThumbnailfile(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK, sc_ret;
#if 1
	char *name;
	INT32U size, pos;
	INT32 avitime=0;
	INT8U *buf = 0;
	INT16U play_file_index;
	int type;
	INT8U packet_const[12] = {0x47, 0x50, 0x53, 0x4F, 0x43, 0x4B, 0x45, 0x54, 0x02, 0x00, 0x03, 0x04};
	INT8U jpgFE[14]={0,0xFF, 0xFE, 0x00, 0x0B,'T','I','M','E'};
	play_file_index = *(rxstring + 13);
	play_file_index = play_file_index << 8;
	play_file_index |= *(rxstring + 12);
	void *fp;
	printf("thumbnail index:%d\n", play_file_index);
	if(get_current_workmode()!=WM_WEBFILE)
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	changeDir(AVI_FILE);
	//sprintf(filePath, "VID%05d.AVI", play_file_index);
	name=getFileByIdx(play_file_index,&type);
	if (name == NULL)
		return NAK_Get_thumbnail_fail;
	fp = osal_fopen(name, "rb");
	if (fp == NULL)
		return NAK_Get_thumbnail_fail;
	if(type==3)
		size = getJpgThumbnail(fp, &pos);
	else
		//size = getAviThumbnail(fp, &pos);
		size = getAviThumbnailTime(fp, &pos,&avitime);
	if (size == 0|| size>20*1024)
	{
		osal_fclose(fp);
		//return NAK_Get_thumbnail_fail;
		return JRXSOCK_Playback_GetThumbnail_Decode(jrx_sock_cmd,ctl_blk,rxstring);
	}
	osal_fseek(fp, pos);
	buf = ((u32)getCurModeFreeBuffBase()+256)&(~0x3f);
	while(modeFreeSpace()<20*1024+256+32)
	{
		printf("no memory for Thumbnail!!!,%dKB\n\n",modeFreeSpace()>>10);
	}
	osal_fread(buf, 1, size, fp);
	buf-=JRX_SOCK_RESP_PAYLOAD_OFFSET;
	memcpy(buf, (INT8 *)packet_const, 12);
	size+=14;
	buf[12] = (size)&0xff;
	buf[13] = (size >> 8) & 0xff;
	buf[JRX_SOCK_RESP_PAYLOAD_OFFSET+5]=0x11;
	sc_ret = send(ctl_blk->client, buf, 20 + JRX_SOCK_RESP_PAYLOAD_OFFSET,0);
	jpgFE[8+1] = (INT8U)((avitime >> 0x00)&0xFF);
	jpgFE[9+1] = (INT8U)((avitime >> 0x08)&0xFF);
	jpgFE[10+1] = (INT8U)((avitime >> 0x10)&0xFF);
	jpgFE[11+1] = (INT8U)((avitime >> 0x18)&0xFF);
	jpgFE[12+1] = 0x72;
	sc_ret = send(ctl_blk->client, jpgFE, 14,0);
	sc_ret = send(ctl_blk->client, buf+ 20 + JRX_SOCK_RESP_PAYLOAD_OFFSET, size-20-14,0);
	if(sc_ret < 0)
	{
		osal_fclose(fp);
		printf("thumbnail get error\r\n");
		return NAK_Get_thumbnail_fail;
	}
	osal_fclose(fp);
#endif
	printf("thumbnail %d,time:%d\n", size,avitime);
	// printf("Thnail:%d(%d)\r\n",play_file_index,ret);
	return ret;
}

#if 1 // SUPPORT_APP_DEL_FILE == CUSTOM_ON
static int JRXSOCK_Playback_DelFile_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK, sc_ret;
#if 0
	INT8U packet_const[12] = {0x47,0x50,0x53,0x4F,0x43,0x4B,0x45,0x54,0x02,0x00,0x03,0x08};
	INT16U play_file_index;

	memcpy((INT8*)socket_cmd_tx_buf,(INT8*)packet_const,12);
	if ( jrx_mode_remap()!=BROWSE_MODE )
	{
		printf("MODE(0x%x) error\r\n",present_state);
		ret = NAK_Server_is_busy;
		goto JRXSOCK_PLAYBACK_DELFILE_DECODE_END;
	}

	play_file_index = *(rxstring+13);
	play_file_index = play_file_index<<8;
	play_file_index |= *(rxstring+12);

	//printf("Thnail=%d\r\n",play_file_index);
	sc_ret = ap_storage_service_given_file_del(play_file_index);
	if (sc_ret == -1)
	{
		// maybe remove SD card
		ret = NAK_Invalid_command;
		goto JRXSOCK_PLAYBACK_DELFILE_DECODE_END;
	}
#endif
JRXSOCK_PLAYBACK_DELFILE_DECODE_END:
	// printf("Del:%d(%d)\r\n",play_file_index,ret);
	return ret;
}
#endif

#define DOWNLOAD_RETRY 50000
#define DOWNLOAD_SD_BLOCK_SIZE (32*1024)
#define DOWNLOAD_PACKET_SIZE (32 * 1024)
typedef struct __downLoadInfor
{
	void *fp;
	u8 *buff;
	u32 size;
	u32 dataLen;
	u8 ready;
}downLoadInfor;
u32 downloadSpeed;
static int JRXSOCK_Playback_GetRawData_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	#define BUF_SUM   2
	INT8U *buf = 0;
	INT16U play_file_index,i;
	INT8U packet_const[12] = {0x47, 0x50, 0x53, 0x4F, 0x43, 0x4B, 0x45, 0x54, 0x02, 0x00, 0x03, 0x05};
	INT32U sd_get_size,sd_get_size_next; //
	INT32U sd_get_idx = 0;
	INT32 ret;
	INT32U cur_size = 0, cur_t, str_t, readsd_t = 0, netwr_t = 0,lastSize=0;
	INT8U *pBuf;	   // download current buffer
	INT32 err = NAK_OK;
	INT16U available;
	void *fp=NULL;
	char *name;
	INT32U fileSize;
	u32 lastTime;
	u8 writeHead;
	u8 *headBuff[BUF_SUM];
	u8 *dataBuff[BUF_SUM];
	u8 *head;
	u8 useBuff=0;
	INT32U wlen, tlen, size;
	downLoadInfor infor[BUF_SUM];
#if 1
	// mm_dump();
	if(get_current_workmode()!=WM_WEBFILE)// lyc debug
	{
		printf("work mode err[0x%x]:%d %d!!!!!!!!\n\n\n\n",jrx_sock_cmd,get_current_workmode(),wifi_mode_bak);
		return NAK_Mode_error;
	}
	play_file_index = *(rxstring+13);
	play_file_index = play_file_index<<8;
	play_file_index |= *(rxstring+12);		// file index
	buf = ((u32)getCurModeFreeBuffBase()+128)&(~0x3f);
	if (buf == NULL)
	{
		printf("JRXSOCK_Playback_GetRawData memory alloc fail\r\n");
		err = NAK_Invalid_command;
		goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
	}
	name=getFileByIdx(play_file_index,NULL);
	if(name==NULL)
	{
		printf("get %d file err\n",play_file_index);
		err = NAK_Invalid_command;
		goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
	}
	fp=osal_fopen(name, "rb");
	if(name==NULL)
	{
		printf("open[%s] %d file err\n",name,play_file_index);
		err = NAK_Invalid_command;
		goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
	}
	while(modeFreeSpace()<64*2+(DOWNLOAD_SD_BLOCK_SIZE+256)*2+128+64)
	{
		printf("no memory for date buff!!!,%dKB\n\n",modeFreeSpace()>>10);
	}
	headBuff[0]=buf;
	headBuff[1]=buf+64;
	//headBuff[2]=buf+64*2;
	dataBuff[0]=buf+64*2;
	dataBuff[1]=buf+64*2+DOWNLOAD_SD_BLOCK_SIZE+256;
	//dataBuff[2]=buf+64*3+(DOWNLOAD_SD_BLOCK_SIZE+256)*2;
	dcache_flush_region((u32)buf,64*2+(DOWNLOAD_SD_BLOCK_SIZE+256)*2);
	memcpy(headBuff[0],packet_const,12);
	memcpy(headBuff[1],packet_const,12);
	infor[0].fp=fp;
	infor[1].fp=fp;
	infor[0].size=DOWNLOAD_SD_BLOCK_SIZE;
	infor[1].size=DOWNLOAD_SD_BLOCK_SIZE;
	infor[0].ready=0;
	infor[1].ready=0;
	infor[0].buff=dataBuff[0];
	infor[1].buff=dataBuff[1];
	//memcpy(headBuff[2],packet_const,12);
	fileSize=osal_fsize(fp);
	useBuff=0;
	infor[useBuff].ready=0;
	sendMsgAndWait(OS_TASK_DOWNLOAD_READ, TASK_MSG_DOWNLOAD_READ_WAIT,&infor[useBuff]);
	//////////// TODO ///////////
	printf("start download:0x%x\n",buf);
	do
	{
		// New data detected (i.e. cancel download) during download.
		// Leave the while loop to handle it.
		ret = recv(ctl_blk->client, (void *)socket_cmd_rx_buf, SOCKET_CMD_BUF_SIZE, MSG_DONTWAIT);
		if(ret > 0)
		{
			printf("Cancel download\r\n");
			err = NAK_Invalid_command;
			goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
		}
		while(infor[useBuff].ready==0)
		{
			ret = recv(ctl_blk->client, (void *)socket_cmd_rx_buf, SOCKET_CMD_BUF_SIZE, MSG_DONTWAIT);
			if(ret > 0)
			{
				printf("Cancel download\r\n");
				err = NAK_Invalid_command;
				goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
			}
			osal_task_sleep(3);
		}
		str_t = os_time_get();
		buf=infor[useBuff].buff;
		head=headBuff[useBuff];
		sd_get_size=infor[useBuff].dataLen;
		cur_t = os_time_get();
		if(sd_get_size)
		{
			if(useBuff)
				useBuff=0;
			else
				useBuff=1;
			infor[useBuff].ready=0;
			sendMsg(OS_TASK_DOWNLOAD_READ, TASK_MSG_DOWNLOAD_READ,&infor[useBuff]);
			sd_get_idx = 0;
			// each DOWNLOAD_PACKET_SIZE
			while (sd_get_size != 0)
			{
				readsd_t += (cur_t-str_t);
				pBuf = (buf+sd_get_idx*DOWNLOAD_PACKET_SIZE);
				sd_get_idx++;
				if (sd_get_size > DOWNLOAD_PACKET_SIZE)
				{
					size = DOWNLOAD_PACKET_SIZE;
					sd_get_size -= DOWNLOAD_PACKET_SIZE;
				}
				else
				{
					size = sd_get_size;
					sd_get_size = 0;
				}
				head[12]=size&0xff;
				head[13]=(size>>8)&0xff;
				tlen = size;
				writeHead=1;
				// Wi-Fi driver divide into few packets
				while(tlen)
				{
					if(socket_cmd_wifi_new_client_conn(ctl_blk)==0)
					{
						printf("new connect,cancel download\n");
						goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
					}
					if(!(cmd_service_enable&&drop_client==0))
					{
						printf("service cancel download %d %d\n",cmd_service_enable,drop_client);
						goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
					}
					//available = socket_cmd_wifi_get_client_avail_tx_buf_size(ctl_blk);
					if (1)//(available>=JRX_SOCK_RESP_PAYLOAD_OFFSET)
					{
					#if 0
						if(tlen > available)
							wlen = available;
						else
							wlen = tlen;
						if(writeHead)
							wlen=JRX_SOCK_RESP_PAYLOAD_OFFSET;
					#endif
						writeHead=0;
						wlen=tlen;
						//printf("available=%d,wlen=%d,tlen=%d\n",available,wlen,tlen);
						if(os_time_get()-lastTime>=5000)
						{
							downloadSpeed=((cur_size-lastSize)>>10)*1000/(os_time_get()-lastTime);
							printf("wr=%dKB, %dKB/S\n",cur_size>>10,downloadSpeed);
							lastSize=cur_size;
							lastTime = os_time_get();
						}
						str_t = os_time_get();
						//if(writeHead)
						ret = send(ctl_blk->client, head, JRX_SOCK_RESP_PAYLOAD_OFFSET,0);
						//else
						ret = send(ctl_blk->client, pBuf, wlen,0);
						cur_t = os_time_get();
						netwr_t += (cur_t-str_t);
						if(ret < 0)
						{
							err = NAK_Write_Fail;
							printf("raw data get error: send data fail(wlen=%d)\r\n", wlen);
							goto JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END;
						}
						if(writeHead==0)
						{
							tlen -= wlen;
							pBuf += wlen;
						}
						writeHead=0;
					}
				}
				cur_size += size;
				// printf("wifi send data OK,%d\r\n",retry_cnt);
			}
		}
		else
		{
			if(osal_ftell(fp)==fileSize)
				break;
			printf("file download error!!\r\n");
			err = NAK_Server_is_busy;
			break;
		}
	}
	while(fileSize > cur_size);
JRXSOCK_PLAYBACK_GETRAWDATA_DECODE_END:
	printf("file size:%d,send size:%d\n",fileSize,cur_size);
	printf("download finis,sd:%d,net:%d,ret:%d\r\n",readsd_t,netwr_t,err);
	sendMsgAndWait(OS_TASK_DOWNLOAD_READ, TASK_MSG_UNINIT,0);
	if(fp)
		osal_fclose(fp);
#endif
	return err;
}

static int JRXSOCK_Playback_Stop_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32U i;
	INT32 ret = NAK_OK;
	dropConnAndWaite();
	return ret;
}

static int JRXSOCK_Playback_GetSpecificName_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned short *p_status)
{
	INT16U play_file_order;
	INT16 play_file_index;
	INT32 ret = NAK_OK;
	return ret;
}

static int JRXSOCK_Firmware_Download_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
	INT32U *p;
#if 1
	if(getUsbConnect()==0&&getCurBatteryLevel()<2)
	{
		ret = NAK_Battery_low;
		goto JRXSOCK_Firmware_Download_Decode_END;
	}
	dropConnAndWaite();
	wifi_mode_trans();
	printf("mode free:%dKB\n",modeFreeSpace()>>10);
	// malloc memory
	prx_firmware_buf = ((u32)getCurModeFreeBuffBase()+64)&(~63);
	pfirmware = prx_firmware_buf+SOCKET_CMD_DOWNLOAD_BUF_SIZE+512;//jrx_malloc_align(SOCKET_CMD_FIRMWARE_BUF_SIZE, 64);
	//prx_firmware_buf = jrx_malloc_align(SOCKET_CMD_DOWNLOAD_BUF_SIZE, 64);
	
	pfirmware_cur = pfirmware;

	p = (INT32U*)(rxstring+12);
	firmware_download_size = *p;
	p++;
	firmware_download_checksum = *p;
	firmware_cur_size = 0;
	printf("download_size=%d, download_checksum=0x%x\r\n",firmware_download_size,firmware_download_checksum);
#endif
JRXSOCK_Firmware_Download_Decode_END:
	return ret;
}

static int JRXSOCK_Firmware_SendRawData_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
	INT32U size;
	INT8U *p;
#if 1
	if(get_current_workmode()!=WM_WIFI)
	{
		prx_firmware_buf = NULL;
		return NAK_Mode_error;
	}
	p = (INT8U*)(rxstring+12);
	size = (INT32U)(*p++);
	size += ((*p++)<<8);
	
	//printf("cur_size=%d, size = %d\r\n", firmware_cur_size, size);
	firmware_upgrade_flag=0;
	if (size!=0)
	{	// downloading
		memcpy((INT8*)pfirmware_cur, (INT8*)p, size);
		firmware_cur_size += size;
		pfirmware_cur += (INT32U)size;
	}
	else
	{	// checksum
		INT32U i;
		INT32U chksum = 0;

		printf("download finish\r\n");
		if (firmware_cur_size!=firmware_download_size)
		{
			pfirmware = NULL;
			prx_firmware_buf = NULL;
			printf("download size mismatch !!\r\n");
			ret = NAK_Checksum_error;
			goto JRXSOCK_Firmware_SendRawData_Decode_END;
		}
		for (i=0; i<firmware_download_size;++i)
		{
			chksum += pfirmware[i];
		}
		if (chksum!=firmware_download_checksum)
		{
			pfirmware = NULL;
			prx_firmware_buf = NULL;			
			printf("firmware checksum error !!\r\n");
			ret = NAK_Checksum_error;
			goto JRXSOCK_Firmware_SendRawData_Decode_END;
		}
		firmware_upgrade_flag = 1;		// firmware verify pass
		
	}
#endif
JRXSOCK_Firmware_SendRawData_Decode_END:
	return ret;
}

static int JRXSOCK_Firmware_Upgrade_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK;
#if 1
	if (firmware_upgrade_flag!=1)
	{
		pfirmware = NULL;
		prx_firmware_buf = NULL;
		printf("NAK_Checksum_error\r\n");
		ret = NAK_Checksum_error;
		goto JRXSOCK_Firmware_Upgrade_Decode_END;
	}
	printf("start upgrade\n");
	upgradeWinOpen();
	SPI_NetUpgrade(pfirmware,firmware_download_size);
	resReinit();
	configSaveClear();
	startPoweroff();
#if 0 // josephhsieh_debug
	if ( ap_state_firmware_upgrade()==-1 )
	{
		printf("ap_state_firmware_upgrade malloc buffer error\r\n");
		jrx_free(pfirmware); pfirmware = NULL;
		jrx_free(prx_firmware_buf); prx_firmware_buf = NULL;
		ret = NAK_Mem_malloc_error;
		goto JRXSOCK_Firmware_Upgrade_Decode_END;
	}
#endif

	firmware_upgrade_flag = 2;
#endif
JRXSOCK_Firmware_Upgrade_Decode_END:
	return ret;
}

static int Vendor_Command_Decode(unsigned short jrx_sock_cmd, jrxCmdSvr *ctl_blk, INT8U *rxstring, unsigned char *buf)
{
	return 0;
#if 0
	int i;
	unsigned char *p = (unsigned char *)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET+2]);
	short size = *((short*)(&rxstring[JRX_SOCK_CMD_PAYLOAD_OFFSET]));
	TIME_T	g_time;

	if (size > JRXSOCK_Vendor_Command_Buf_Length)
	{
		size = JRXSOCK_Vendor_Command_Buf_Length;
	}

	/*for(i=0;i<31;i++)
	{
		printf("rxstring[%d]=0x%x\r\n",i,rxstring[i]);
	}*/
	for (i = 0; i < size; ++i)
	{
		buf[i] = p[i];
		//printf("p[%d] = 0x%x\r\n",i,p[i]);
	}
	
	if((*p == 'G')&&(*(p+1) == 'P')&&(*(p+2) == 'V')&&(*(p+3) == 'E')&&(*(p+4) == 'N')&&(*(p+5) == 'D')
	   &&(*(p+6) == 'O')&&(*(p+7) == 'R'))
	{
		g_time.tm_year = p[11];
		g_time.tm_year = g_time.tm_year<<8;
		g_time.tm_year += p[10];
		g_time.tm_mon  = p[12];
		g_time.tm_mday = p[13];
		g_time.tm_hour = p[14];
		g_time.tm_min = p[15];
		g_time.tm_sec = p[16];
		cal_time_set(g_time);
		printf("Phone set time:%d-%02d-%02d %02d:%02d:%02d\r\n",g_time.tm_year,g_time.tm_mon,g_time.tm_mday,
		                                                           g_time.tm_hour,g_time.tm_min,g_time.tm_sec);
#if ENABLE_CHECK_RTC == 1
		ap_state_config_data_time_save_set();
#endif
	}

	return (int)size;
#endif
}

//
// Client -> server command format:
//
// Low byte                                                                  High byte
// +-------------+--------------+-----------------+----------------+-----------------+
// | JRX_SOCK_Tag | JRX_SOCK_Type | JRX_SOCK_Mode_ID | JRX_SOCK_Cmd_ID | JRX_SOCK_Payload |
// +-------------+--------------+-----------------+----------------+-----------------+
//      8byte         2byte           1byte              1byte            Nbyte
//
static int jrx_cmd_process(jrxCmdSvr *ctl_blk, INT8U *rxstring)
{
	INT32 ret = NAK_OK, ret1;
	INT8U *pJRX_SOCK_Tag = (INT8U *)(rxstring + 0);
	INT16 JRX_SOCK_Cmd;
	INT16U len = 0;

	// Check tag
	JRX_SOCK_Cmd = (INT16)(pJRX_SOCK_Tag[JRX_SOCK_CMD_MODE_ID_OFFSET] << 8) | (INT16)pJRX_SOCK_Tag[JRX_SOCK_CMD_CMD_ID_OFFSET];
	if ((pJRX_SOCK_Tag[0] != 'G') || (pJRX_SOCK_Tag[1] != 'P') || (pJRX_SOCK_Tag[2] != 'S') ||
		(pJRX_SOCK_Tag[3] != 'O') || (pJRX_SOCK_Tag[4] != 'C') || (pJRX_SOCK_Tag[5] != 'K') ||
		(pJRX_SOCK_Tag[6] != 'E') || (pJRX_SOCK_Tag[7] != 'T'))
	{
		ret = -1;
		printf("socket_cmd:skip\r\n");
		goto GP_CMD_PROCESS_END;
	}
	else
	{
		// printf("CMD=0x%x\r\n",JRX_SOCK_Cmd);
	}
	if(JRX_SOCK_Cmd!=1)
	{
		printf("recv cmd:0x%x\n", JRX_SOCK_Cmd);
	}

	switch (JRX_SOCK_Cmd)
	{
	case JRXSOCK_General_SetMode:
		JRXSOCK_General_SetMode_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		break;
	case JRXSOCK_General_GetParameterFile:
		General_GetParameterFile_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		break;
	case JRXSOCK_General_GetDeviceStatus:
	{
		unsigned char status[JRXSOCK_General_DeviceStatus_Length];
		General_GetDeviceStatus_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, status);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), JRXSOCK_General_DeviceStatus_Length, status, JRXSOCK_General_DeviceStatus_Length);
	}
	break;
	case JRXSOCK_General_PowerOff:
		JRXSOCK_General_PowerOff_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		break;
	case JRXSOCK_General_RestarStreaming:
		ret = General_RestarStreaming_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_General_AuthDevice:
	{
		unsigned char key[JRXSOCK_General_Key_Length];
		JRXSOCK_General_AuthDevice_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, key);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), JRXSOCK_General_Key_Length, key, JRXSOCK_General_Key_Length);
	}
	break;
	case JRXSOCK_General_RTSP_Status:
	{
		unsigned char key[JRXSOCK_General_Key_RTSP_Length];
		JRXSOCK_General_RTSP_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, key);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), JRXSOCK_General_Key_RTSP_Length, key, JRXSOCK_General_Key_RTSP_Length);
	}
	break;
	case JRXSOCK_Record_Start:
		ret = JRXSOCK_Record_Start_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Record_Audio:
		ret = JRXSOCK_Record_Audio_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_CapturePicture_Capture:
		ret = JRXSOCK_CapturePicture_Capture_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_Start:
		JRXSOCK_Playback_Start_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		break;
	case JRXSOCK_Playback_Pause:
		JRXSOCK_Playback_Pause_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		break;
	case JRXSOCK_Playback_GetFileCount:
		ret = JRXSOCK_Playback_GetFileCount_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret > 0)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0002, (unsigned char *)(&ret), 2);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_GetFileList:
		ret = JRXSOCK_Playback_GetFileList_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret < 0)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		else
			len = ret;
		break;
	case JRXSOCK_Playback_GetThumbnail:
		sdCheckEnSet(0);
		#if 0//RECORD_WITH_THUMBNAL
		ret = JRXSOCK_Playback_GetThumbnailfile(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		#else
		ret = JRXSOCK_Playback_GetThumbnail_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		#endif
		sdCheckEnSet(1);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set(JRX_SOCK_TYPE_ACK << 16 | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_DeleteFile:
#if 0 // SUPPORT_APP_DEL_FILE == CUSTOM_ON
		ret = JRXSOCK_Playback_DelFile_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
#else
		ret = 0xFFFF;
#endif
		if (ret == NAK_OK)
		{
			len = jrx_resp_set(JRX_SOCK_TYPE_ACK << 16 | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_GetRawData:
		sdCheckEnSet(0);
		ret = JRXSOCK_Playback_GetRawData_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		sdCheckEnSet(1);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set(JRX_SOCK_TYPE_ACK << 16 | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			printf("download error:%d\n",ret);
			//lastDownloadCancel=1;
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_Stop:
		ret = JRXSOCK_Playback_Stop_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Playback_GetSpecificName:
	{
		unsigned short status;
		ret = JRXSOCK_Playback_GetSpecificName_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, &status);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0002, (unsigned char *)(&status), 2);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
	}
	break;
	case JRXSOCK_Menu_GetParameter:
	{
		unsigned char value[50];
		unsigned int value_len = 0;
		value_len = Menu_GetParameter_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, (unsigned char *)(&value));
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), value_len, (unsigned char *)(&value), value_len);
	}
	break;
	case JRXSOCK_Menu_SetParameter:
		ret = Menu_SetParameter_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
			len = jrx_resp_set(JRX_SOCK_TYPE_ACK << 16 | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		else
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		break;
	case JRXSOCK_Firmware_Download:
		ret = JRXSOCK_Firmware_Download_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Firmware_SendRawData:
		ret = JRXSOCK_Firmware_SendRawData_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else if (ret == NAK_Checksum_error)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Firmware_Upgrade:
		ret = JRXSOCK_Firmware_Upgrade_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag);
		if (ret == NAK_OK)
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd), 0x0000, 0, 0);
		}
		else
		{
			len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), (short)ret, 0, 0);
		}
		break;
	case JRXSOCK_Vendor_Command:
	{
		unsigned char vendor_buf[JRXSOCK_Vendor_Command_Buf_Length];
		unsigned int vendor_len;
		vendor_len = Vendor_Command_Decode(JRX_SOCK_Cmd, ctl_blk, pJRX_SOCK_Tag, vendor_buf);
		len = jrx_resp_set((JRX_SOCK_TYPE_ACK << 16) | (JRX_SOCK_Cmd & 0x0000FFFF), vendor_len, vendor_buf, vendor_len);
	}
	break;

	default:
		printf("unknown cmd:0x%x\n", JRX_SOCK_Cmd);
		len = jrx_resp_set((JRX_SOCK_TYPE_NAK << 16) | (JRX_SOCK_Cmd), 0xFFFF, 0, 0);
	}
	ret1 = send(ctl_blk->client, (void*)socket_cmd_tx_buf, len, 0);	
	if(ret1 < 0)
	{
		ret = NAK_Write_Fail;
	}
	if (firmware_upgrade_flag == 2)
	{
		printf("upgrade finish reboot\r\n");
		upgrade_exit(0); // only power off can be chosen
	}

GP_CMD_PROCESS_END:
	return ret;
}
static int backup_client = -1;
extern uint32_t rtsp_conn_num;
int wifiOffCheck;
int stopRtpTrans=0;
INT32U wifiOffCheckTime;
int resetTxRetryCnt;
void reset_tx_task_retry_cnt(void)
{
#if 0
	if(resetTxRetryCnt&&wifiGetTxCnt()==0)
	{
		//ssv6xxx_wifi_set_tx_task_retry_cnt(8);
		ssv6xxx_set_tx_retry_cnt(8);
		resetTxRetryCnt=0;
	}
#endif
}
// extern INT8U s_usbd_pin;
static INT32 process_socket_cmd(jrxCmdSvr *ctl_blk)
{
	
	static INT16 JRX_SOCK_MODE = 0;
	static INT16 JRX_SOCK_ID = 0;
	INT32 polling_cnt = 0;
	INT32 ret = 0;
	INT32S rxlen;
	INT8U *rx_buf;
	INT32U rx_buf_size;
	INT16 JRX_SOCK_Cmd;
	int err;
	modeReset();
	wifiOffCheck=0;
	stopRtpTrans=0;
	printf("new client:%d,%d\n",cmd_service_enable,drop_client);
	while (cmd_service_enable&&drop_client==0)
	{
		if (prx_firmware_buf == NULL)
		{
			rx_buf = socket_cmd_rx_buf;
			rx_buf_size = SOCKET_CMD_BUF_SIZE;
		}
		else // firmware upgrade mode
		{
			rx_buf = prx_firmware_buf;
			rx_buf_size = SOCKET_CMD_DOWNLOAD_BUF_SIZE;
		}
		rxlen = recv(ctl_blk->client, (void *)rx_buf, rx_buf_size, 0);
		err=lwip_getSockError(ctl_blk->client);
		//if ( backup_client>=0 )
		{
			//printf("force close %d\r\n", backup_client);
			//rxlen = 0;
		}
		if(rxlen > 0)
		{
			JRX_SOCK_MODE = (INT16)(rx_buf[JRX_SOCK_CMD_MODE_ID_OFFSET]);
			JRX_SOCK_ID = (INT16)(rx_buf[JRX_SOCK_CMD_CMD_ID_OFFSET]);
			JRX_SOCK_Cmd = (JRX_SOCK_MODE << 8) | JRX_SOCK_ID;
			wifiOffCheck=0;
			stopRtpTrans=0;
			wifiOffCheckTime=os_time_get();
			jrx_cmd_process(ctl_blk, rx_buf);
		}
		else if ( (rxlen == -1) && ((err==EAGAIN)||(err==ENOMEM)||(err==ENOBUFS)||(err==EINTR)||(err == 0)) )	//timeout
		{
			//printf("1cmd:err %d, l3_errno=%d %d\r\n", rxlen, err,l3_errno);
			if(rtsp_conn_num&&workmode_is_live())
			{
				if(wifiOffCheck==0)
				{
					if(wifiOffCheckTime>os_time_get())
						wifiOffCheckTime=os_time_get();
					if(os_time_get()-wifiOffCheckTime>=3000)
					{
						//ssv6xxx_wifi_set_tx_task_retry_cnt(1);
						//ssv6xxx_set_tx_retry_cnt(1);
						resetTxRetryCnt=1;
						stopRtpTrans=1;
						wifiOffCheck=1;
					}
				}
				if(wifiOffCheck&&!wifiStaActive())
				{
					printf("tx buff:%d,drop connect!!!\n\n",wifiGetTxCnt());
					break;
				}
			}
			else
				wifiOffCheck=0;
			if(socket_cmd_wifi_new_client_conn(ctl_blk)==0)
				break;
			continue;			
		}
		else  // rxlen = 0 => close
		{
			printf("cmd socke:%d,:err%d,l3_errno=%d\n",ctl_blk->client,rxlen,err);
			break;
		}
	}

PROCESS_SOCKET_CMD_END:
	polling_cnt = 0;
	JRX_SOCK_MODE = 0;
	JRX_SOCK_ID = 0;
	stopRtpTrans=1;
	dropConnAndWaite();
	/* Exit this socket to accept a new connection */
	printf("Exit %s\r\n", __func__);

	return ret;
}

#define SOCKET_CMD_KEEPALIVE_IDLE	    3	/* 10000ms */
#define SOCKET_CMD_KEEPALIVE_INTVL	    1	/* 1000ms */
#define SOCKET_CMD_KEEPALIVE_CNT	    60	/* 1 time */

#define SOCK_TIME_OUT 50
void socket_next_task(void *pvParameters)
{
	jrxCmdSvr *ctl_blk = &socket_socket_ctl_blk;
	struct sockaddr_in cli_addr;
	socklen_t client;

	while(1)
	{
		// detect next connection
		if (backup_client < 0)
		{
			printf("backup_client start\r\n");
			client = sizeof(cli_addr);
			backup_client = accept(ctl_blk->server, (struct sockaddr *)&cli_addr, &client);
			printf("OK = %d!\r\n",backup_client);
			while (backup_client>=0)
			{
				osal_task_sleep(100);
			}
		}
		osal_task_sleep(500);
	}
}
int clientConnect(void)
{
	if(socket_socket_ctl_blk.client>=0)
		return 1;
	return 0;
}
void socket_cmd_task()
{
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	INT32 err;
	socklen_t client;
	struct sockaddr_in server_addr, cli_addr;
	jrxCmdSvr *ctl_blk = &socket_socket_ctl_blk;
	int status;
	int	enable = 0;
	int keepalive = 0;
	int keep_idle = SOCKET_CMD_KEEPALIVE_IDLE;
	int keep_intvl = SOCKET_CMD_KEEPALIVE_INTVL;
	int keep_cnt = SOCKET_CMD_KEEPALIVE_CNT;
	
	ctl_blk->server = socket(AF_INET, SOCK_STREAM, 0);
	if(ctl_blk->server < 0)
	{
		printf("SCKET_CMD failed\r\n");
		goto SOCKET_CMD_TASK_END;
	}

	status = setsockopt(ctl_blk->server, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enable, sizeof(enable));
	if(status < 0)
	{
		printf("SOCKET_CMD SO_KEEPALIVE error\r\n");
		goto SOCKET_CMD_TASK_END;
	}

	/* Set socket address reusable */
	status = setsockopt(ctl_blk->server, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(enable));
	if(status < 0)
	{
		printf("SOCKET_CMD REUSEADDR error %d\r\n", status);
		goto SOCKET_CMD_TASK_END;
	}

	/* Bind the TCP server socket address */
	memset((INT8S *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_len         = sizeof(server_addr);
	server_addr.sin_port        = htons(SOCKET_CMD_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	status = bind(ctl_blk->server, (struct sockaddr *)&server_addr,  sizeof(server_addr));
	if(status < 0)
	{
		printf("SOCKET_CMD bind error\r\n");
		goto SOCKET_CMD_TASK_END;
	}

	/* Listen */
	if(listen(ctl_blk->server , 1) < 0)
	{
		printf("SOCKET_CMD listen error\r\n");
		goto SOCKET_CMD_TASK_END;
	}

	{
		//osal_thread_create(socket_next_task, NULL, 22, 0, 4096, "cmd_client");		
	}
	
	backup_client = -1;
 	while(1)
	{
		ctl_blk->client=-1;
		while(1)
		{
			// detect next connection
			if (backup_client < 0)
			{
				printf("wait client connect\n");
				client = sizeof(cli_addr);
				backup_client = accept(ctl_blk->server, (struct sockaddr *)&cli_addr, &client);
				printf("client OK = %d\r\n",backup_client);
			}
			else
				break;
		}
		ctl_blk->client = backup_client;
		backup_client = -1;

		setsockopt(ctl_blk->client, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
		printf("1client keepalive:%d\r\n",keepalive);
#if 0
		//set socket attribute
		if(setsockopt(ctl_blk->client, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_idle)) < 0 )
		{
			printf("TC set TCP_KEEPIDLE err! \r\n");
			//goto exit_task;
		}

		if(setsockopt(ctl_blk->client, IPPROTO_TCP, TCP_KEEPINTVL, &keep_intvl, sizeof(keep_intvl)) < 0 )
		{
			printf("TC set TCP_KEEPINTVL err! \r\n");
			//goto exit_task;
		}

		if(setsockopt(ctl_blk->client, IPPROTO_TCP, TCP_KEEPCNT, &keep_cnt, sizeof(keep_cnt)) < 0 )
		{
			printf("TC set TCP_KEEPCNT err! ");
			//goto exit_task;
		}
#endif
#if 0
		if(setsockopt(ctl_blk->client, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) < 0 )
		{
			printf("TCP_NODELAY fail\r\n");
		}
#endif

		{
			enable = SO_RCVTIMEO_TIMEOUT;		// timeout ticks
			if(setsockopt(ctl_blk->client, SOL_SOCKET, SO_RCVTIMEO, &enable, sizeof(enable)) < 0 )
			{
				printf("SO_RCVTIMEO error!\r\n");
			}
		}

		if (ctl_blk->client>=0)
		{
			memset(socket_cmd_tx_buf, 0, sizeof(socket_cmd_tx_buf));
			memset(socket_cmd_rx_buf, 0, sizeof(socket_cmd_rx_buf));
			strncpy((char*)socket_cmd_tx_buf, (char*)"GPSOCKET", 8);
			drop_client=0;
			/* A client connected */
			process_socket_cmd(ctl_blk);
				
			///Wifi_Disconnect();		//unlock the AD key 

			/* Close connected client socket then wait for new client connection */
			if(lwip_close(ctl_blk->client) != 0)
			{
				printf("close socket_cmd client error\r\n");
			}
			drop_client=0;
			ctl_blk->client = -1;
			tcp_debug_print_pcbs();
		}
		else
		{
			printf("gp cmd: socket accept fail, netconn 0x%x 0x%x\r\n",ctl_blk->server, ctl_blk->client);
		}
	}
SOCKET_CMD_TASK_END:
		printf("%s error\r\n",__func__);
}
void downloadReadFile(void* d)
{
	u32 msgType,msgData;
	OS_WAIT_DONE_MSG *msg;
	downLoadInfor *infor;
	while(1)
	{
		if(recMsg(OS_TASK_DOWNLOAD_READ,&msgType,&msgData)<=0)
		{
			printf("OS_TASK_DOWNLOAD_READ recv msg error!!!!!!\n\n");
			continue;
		}
		switch(msgType)
		{
		case TASK_MSG_DOWNLOAD_READ:
			infor=(downLoadInfor *)msgData;
			infor->dataLen=osal_fread(infor->buff,1,infor->size,infor->fp);
			infor->ready=1;
			break;
		case TASK_MSG_WAIT_DONE:
			msg=(OS_WAIT_DONE_MSG *)msgData;
			switch(msg->msgID)
			{
				case TASK_MSG_DOWNLOAD_READ_WAIT:
					infor=(downLoadInfor *)msg->msgData;
					infor->dataLen=osal_fread(infor->buff,1,infor->size,infor->fp);
					infor->ready=1;
				break;
				case TASK_MSG_UNINIT:
				break;
			}
			msg->done(msg);
			break;
		default:
			printf("downloadReadFile,unknow msg:%d\n",msgType);
			break;
		}
	}	
}

void socket_cmd_service_init(void)
{
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	INT8U err;

	osal_thread_create(socket_cmd_task, NULL, 19, 0, 1024, "socket_cmd");
	createdTaskByFunc(OS_TASK_DOWNLOAD_READ, downloadReadFile,NULL,19,1024,"download read",10);
	cmdex_service_init();
#if 0
    err = OSTaskCreate(socket_cmd_task, NULL, (void *)&socket_cmd_stack[SOCKET_CMD_TASK_STACKSIZE-1], SOCKET_CMD_PRIORITY);
    if(err != OS_NO_ERR)
    {
        printf("socket cmd_task create failed\r\n");
    }
    else
    {
        printf("socket_cmd_task created ok. pri=%d\r\n", SOCKET_CMD_PRIORITY);
    }
#endif
}

void socket_cmd_start_service(void)
{
#if 1
	cmd_service_enable = 1;
	cmdex_start_service();
#endif
}

void socket_cmd_stop_service(void)
{
	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	int times = 100;
	cmd_service_enable = 0;
	cmdex_stop_service();
	//if (cmd_service_start == 1)
	if(socket_socket_ctl_blk.client>=0)
	{
		cmd_service_enable = 0;
		drop_client=1;
		/* waiting for mpjeg service stop */
		//while (cmd_service_start == 1)
		while(socket_socket_ctl_blk.client>=0)
		{
			drop_client=1;
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
void socket_cmd_drop_client(void)
{
#if (JR_APP_PROTOCOL==0)

	printf("enter [jrx]%s:%d\r\n", __func__, __LINE__);
	int times = 100;
	drop_client=1;
	cmdex_drop_client();
	//if (cmd_service_start == 1)
	if(socket_socket_ctl_blk.client>=0)
	{
		drop_client=1;
		while(socket_socket_ctl_blk.client>=0)
		{
			drop_client=1;
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
