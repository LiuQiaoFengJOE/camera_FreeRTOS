#include "systemSetting.h"
#include "task/app.h"

uint8_t set_rec(int gear)
{
	int rtp_waitting;
	if(!workmode_is_live())
		return DEVICE_BUSY;
	if(get_sd_status()==0)
		return DEVICE_NO_SDCARD;
	if (gear == RECORD_VIDEO_TOGGLE)
	{
		if(videoIsRecording())
		{
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
			return DEVICE_NORMAL;
		}
		else
		{
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_START,0);
			return DEVICE_NORMAL;
		}
	}
	else if (gear == RECORD_VIDEO_STOP)
	{
		if(videoIsRecording())
			sendMsgAndWait(OS_TASK_VID_REC, TASK_MSG_VID_REC_STOP,0);
		return DEVICE_NORMAL;
	}
	else
	{
		printf("set_rec gear:%d\r\n", gear);
		return DEVICE_NO_REPLY; // 不返回任何信息,这里可能是app端切换到拍照时发送强制停止录像的命令
	}
	return DEVICE_NO_REPLY; // 似乎不应该出现在这个地方
}




