#ifndef  __OS_TASK_INFOR_H
#define  __OS_TASK_INFOR_H
#include "typedef.h"
#include "osal/osal_task.h"
#include "osal/osal_wait.h"
#include "osal/osal_msgq.h"
#include "taskMsg.h"

#ifndef SUPPORT_AUD_REC
#define		SUPPORT_AUD_REC		1
#endif
#ifndef SUPPORT_AUD_ENC
#define		SUPPORT_AUD_ENC		0
#endif
#ifndef SUPPORT_RF_RECV
#define		SUPPORT_RF_RECV		0
#endif


typedef enum
{
	OS_TASK_UI=0,
	OS_TASK_SCAN,
	OS_TASK_MAIN,
	OS_TASK_STORAGE,
	OS_TASK_VID_REC,
	OS_TASK_RTP_JPG,
	OS_TASK_RTP_AUDIO,
	OS_TASK_RTP_SEND,
	OS_TASK_READ_FILE,
	OS_TASK_VIDEOPLAY,
	OS_TASK_USB_DEV,
	OS_TASK_DOWNLOAD_READ,
	OS_TASK_ENC,
	OS_TASK_PRINTER,
	OS_TASK_GAME,
	OS_TASK_ZBAR_IDENTIFY,
	OS_TASK_MAX
}OS_TASK_ID;

void createdTaskByFunc(OS_TASK_ID id,TaskFunction_t  taskFunc,void* parame,u32 priority,u16 stackSize,char* name,u32 msgQsize);

typedef enum
{
	FUNCTION_POWER_ON=0,
	FUNCTION_POWER_OFF,
	FUNCTION_VIDEO_RECORD,
	#if SUPPORT_AUD_REC
	FUNCTION_AUDIO_RECORD,
	#endif
	FUNCTION_PHOTO_ENCODE,
	FUNCTION_WIFI,
	FUNCTION_PLAY_BACK,
	FUNCTION_TAKE_PHOTO,
	FUNCTION_USB_DEVICE,
	FUNCTION_USB_UPDATE,
	FUNCTION_MAINMENU,
	FUNCTION_MAINMENU2,
	FUNCTION_MAINMENU3,
	FUNCTION_AUDIO_PLAYER,
	FUNCTION_GAME_MENU,
	FUNCTION_GAME,
	FUNCTION_ROLE,
	FUNCTION_NETTYPE,
	FUNCTION_APP,
	FUNCTION_MAX
}functionID;

typedef struct _function
{
	char* name;
	uint32 arg;
	void  (*functionOpen)(uint32 arg);
	void  (*functionClose)(uint32 arg);
}sysFunction;


void functionInit(void);
void functionRegister(functionID id,sysFunction* function);
functionID functionCurrent(void);
functionID functionNext(void);
functionID functionPre(void);
void functionStart(functionID id,uint32 arg);
void functionChange(void);


#endif
