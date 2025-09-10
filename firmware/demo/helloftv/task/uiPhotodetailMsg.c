#include "fatfs/ff.h"
#include "uiPhotodetailRes.c"
#include "task/app.h"
#include "MP3_filelist.h"
#include "pcm_play_fifo.h"
#include "mediaFile/picIdentify.h"

#define MAX_PATH_LEN 30
#define AUDIO_FOLDER "1:/tmp"

extern PicAiRet_T *getpidAiRet(void);
extern int requestAudioDownload(void *param);
extern int picAudioDownloadStop(void);

static int curTextItem;
static int lastIndex = -1;
static int isUpdate = 0;
static int isOKUpdate = 0;
static int isfirstUpdate = 0;
static int currentFileIndex = 0;
//static PicAiRet_T *aiRet=NULL;
static int isPaused = 0;

static void idContentPlay(winHandle handle,char* file){
    int playRetry = 0;
	PicAiRet_T *aiRet = getpidAiRet();
PLAY_RETRY:
	MP3_filelist_reload(AUDIO_FOLDER);
    int curfile = MP3_set_cur_file(file);
    curTextItem = MP3_get_item_idx();
    if (curTextItem == currentFileIndex){
        lastIndex ++;
        MP3_play();
        idContentShow(handle, aiRet->text[curTextItem], 0);
    }else{
        currentFileIndex = lastIndex;
    }
    isPaused = 0;	
}

static int idContentKeyMsgOk(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        if (isOKUpdate){
            if (!isPaused && (MP3_is_playing() || dacIsPlaying())) {
                MP3_stop();
                isPaused = 1;
                printf("[OK] Paused at curTextItem=%d\n", curTextItem);
            } else if (isPaused == 1) {  // ����ͣ����������
                MP3_play();
                isPaused = 0;
                printf("[OK] Resumed at curTextItem=%d\n", curTextItem);
            } else if (isPaused == 2) {  // ��ϣ���ͷ��ʼ
                MP3_stop();  // ȷ��ֹͣ�κ�Ǳ�ڲ���
                currentFileIndex = 0;
                curTextItem = 0;
                isUpdate = 1;
                lastIndex = 0;
                isPaused = 0;
                // idContentPlay(handle, localAudioPaths[0]);
                printf("[OK] Restart from beginning\n");
            }
        }
    }
    return 0;
}

static int idContentKeyMsgUp(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState;
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
    }
    return 0;
}

static int idContentKeyMsgDown(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState;
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
    }
    return 0;
}

static int idContentKeyMsgMenu(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState;
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        if (curTextItem > 0) {
            curTextItem--;
            currentFileIndex = curTextItem;
			char paths[MAX_PATH_LEN];
			sprintf(paths, "tmpAudio%d.mp3", currentFileIndex);
            idContentPlay(handle, paths);
            printf("[Mode] Previous segment: curTextItem=%d\n", curTextItem);
        } else {
            printf("[Mode] Already at first segment\n");
        }
    }
    return 0;
}

static int idContentKeyMsgMode(winHandle handle, uint32 parameNum, uint32* parame) {
    uint32 keyState;
	PicAiRet_T *aiRet = getpidAiRet();
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        if (aiRet != NULL && curTextItem < aiRet->textNum - 1) {
            curTextItem++;
            currentFileIndex = curTextItem;

			char paths[MAX_PATH_LEN];
			sprintf(paths, "tmpAudio%d.mp3", currentFileIndex);
            idContentPlay(handle, paths);
            printf("[Menu] Next segment: curTextItem=%d\n", curTextItem);
        } else {
            printf("[Menu] Already at last segment\n");
        }
    }
    return 0;
}

static int idContentSysMsgSD(winHandle handle, uint32 parameNum, uint32* parame) {
    if ((SDC_STAT_NULL == SysCtrl.sdcard) || (SDC_STAT_ERROR == SysCtrl.sdcard)) {
        MP3_stop();
        printf("==%s,%d, SD card removed or error\n", __FUNCTION__, __LINE__);
        idContentShow(handle, "SD Card Error", 0);
    } else if (SDC_STAT_NORMAL == SysCtrl.sdcard) {
        MP3_filelist_reload(AUDIO_FOLDER);
        printf("==%s,%d, SD card inserted\n", __FUNCTION__, __LINE__);
    }
    return 0;
}


static int idContentKeyMsgPower(winHandle handle, uint32 parameNum, uint32* parame) {
    unsigned int keyState;
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        isOKUpdate = 0;
       // functionStart(FUNCTION_TAKE_PHOTO, 0);
       winDestroy(&handle);
    }
    return 0;
}

static int idContentOpenWin(winHandle handle, uint32 parameNum, uint32* parame) {
    MP3_init();
	delete_dir(AUDIO_FOLDER);
	f_mkdir(AUDIO_FOLDER);
    DialogBoxShow(handle, SysCtrl.aiuserRole);
    if (SysCtrl.ailangid == PICID_LANGID_SCH) {
        idContentShow(handle, "思考中，请稍等...", 0);
    }
	else
    {
        winSetResid(winItem(handle, ID_CONTENT_0_ID), strToResId("Thinking, please wait..."));
    }
    curTextItem = 0;
    lastIndex = -1;
    isUpdate = 0;
    isfirstUpdate = 0;
    isPaused = 0;
    currentFileIndex = 0;
    printf("==%s,%d, Window opened, isFirstUpdate=%d\n", __FUNCTION__, __LINE__, isUpdate);
    return 0;
}

static int idContentSysMsgIdentifyUpdate(winHandle handle, uint32 parameNum, uint32* parame) {
 	PicAiRet_T *aiRet = getpidAiRet();
    if (!isfirstUpdate){
        MP3_filelist_reload(AUDIO_FOLDER);
        isfirstUpdate = 1;
        isUpdate = 1;
        isOKUpdate = 1;
    }
    return 0;
}

static int idContentSysMsg500MS(winHandle handle, uint32 parameNum, uint32* parame) {
    if (isUpdate && !isPaused){
        if (MP3_is_playing() == 0 && dacIsPlaying() == 0){
            MP3_filelist_reload(AUDIO_FOLDER);
			PicAiRet_T *aiRet = getpidAiRet();
            int num = aiRet ? aiRet->textNum : MP3_get_total_files();
            if (currentFileIndex < num) {
				//printf("===idContentSysMsg500MS, currentFileIndex:%d, num:%d\n", currentFileIndex, num);
				sendMsgNonblock(OS_TASK_UI, VID_EVENT_MP3_PLAY, 0);
            } else {
                isUpdate = 0;
               // printf("[500ms] All segments finished, auto-play stopped\n");
            }
        }
    }
    return 0;
}

static int idContentSysMsgPlay(winHandle handle, uint32 parameNum, uint32* parame)
{
#define MAX_PATH_LEN 30
	char paths[MAX_PATH_LEN];

	sprintf(paths, "tmpAudio%d.mp3", currentFileIndex);
	idContentPlay(handle,paths);
	currentFileIndex ++;

    return 0;
}


static int idContentCloseWin(winHandle handle, uint32 parameNum, uint32* parame) {

    printf("==%s,%d, idContent Close Win\n", __FUNCTION__, __LINE__);
    MP3_stop();
    printf("==%s,%d, Uninitializing MP3 player\n", __FUNCTION__, __LINE__);
    MP3_uninit();
	//delete_dir(AUDIO_FOLDER);
    picAudioDownloadStop();
    curTextItem = 0;
    lastIndex = -1;
    isUpdate = 0;
    currentFileIndex = 0;
    isOKUpdate = 0;
	picAiReleaseAllRetult();
  //  functionStart(FUNCTION_TAKE_PHOTO, 0);
    return 0;
}

static int idContentWinChildClose(winHandle handle, uint32 parameNum, uint32* parame) {
    printf("==%s,%d, idContent WinChild Close\n", __FUNCTION__, __LINE__);
    return 0;
}

msgDealInfor idContentMsgDeal[] = {
    {SYS_OPEN_WINDOW, idContentOpenWin},
    {SYS_CLOSE_WINDOW, idContentCloseWin},
    {SYS_CHILE_COLSE, idContentWinChildClose},
    {KEY_EVENT_OK, idContentKeyMsgOk},
    {KEY_EVENT_UP, idContentKeyMsgUp},
    {KEY_EVENT_DOWN, idContentKeyMsgDown},
    {KEY_EVENT_RIGHT, idContentKeyMsgMode},
    {KEY_EVENT_LEFT, idContentKeyMsgMenu},
    {VID_EVENT_IDENTIFY_UPDATE, idContentSysMsgIdentifyUpdate},
    {SYS_EVENT_SDC, idContentSysMsgSD},
    {SYS_EVENT_500MS, idContentSysMsg500MS},
    {VID_EVENT_MP3_PLAY, idContentSysMsgPlay},
    {KEY_EVENT_MODE, idContentKeyMsgPower},
    {EVENT_MAX, NULL},
};

WINDOW(idContentWindow, idContentMsgDeal, idContentWin)
WINDOW(idContentUniWindow, idContentMsgDeal, idContentUniWin)
