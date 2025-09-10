#include "fatfs/ff.h"
#include "uiSmallModelRes.c"
#include "task/app.h"

#define AUDIO_FOLDER "1:/tmp"
#define TMP_AUDIO "1:/tmp/tmpAudio0.mp3"

#define MAX_RETRY 10
#define RETRY_DELAY_MS 100

extern FRESULT delete_dir(const char* path);

static PicAiRet_T *aiRet = NULL;

static int mp3_play_state = 0; // 0: 未播放, 1: 播放中文, 2: 播放英文

static void idContentPlayMp3(winHandle handle, const char *url, const char *type)
{
    if (!url || strlen(url) == 0) {
        printf("==%s,%d, no %s MP3 to play\n", __FUNCTION__, __LINE__, type);
        return;
    }
    
   // printf("===%s,%d, playing %s MP3: %s\n", __FUNCTION__, __LINE__, type, url);
    if (get_sd_status() != 0) {
        f_mkdir(AUDIO_FOLDER);
        int ret;// = f_unlink(TMP_AUDIO);
        
        int retry_count = 0;
        while (retry_count < MAX_RETRY) {
            // 在每次下载前删除旧文件
            ret = f_unlink(TMP_AUDIO);
            
            ret = picAiDownloadFile(url, TMP_AUDIO);
            if (ret == 0) {
                printf("===%s,%d, %s MP3 downloaded to %s\n", __FUNCTION__, __LINE__, type, TMP_AUDIO);
                break; // 下载成功，退出重试
            }
            retry_count++;
            if (retry_count < MAX_RETRY) {
                osal_task_sleep(RETRY_DELAY_MS); // 等待 500ms 后重试
            }
        }
        
        if (ret == 0) {
            ret = MP3_filelist_reload(AUDIO_FOLDER);
            MP3_stop();
            while (dacIsPlaying()) { osal_task_sleep(10); }
            ret = MP3_play();
            MP3_move_item(OP_NEXT);
        } else {
            printf("===%s,%d, picAiDownloadFile failed for %s after %d retries\n", 
                   __FUNCTION__, __LINE__, type, MAX_RETRY);
        }
    } else {
        printf("===%s,%d, SD card not ready\n", __FUNCTION__, __LINE__);
    }
}

static void idContentPlay(winHandle handle)
{
    if (!aiRet) {
        printf("==%s,%d, aiRet is NULL\n", __FUNCTION__, __LINE__);
        mp3_play_state = 0;
        return;
    }

    if (mp3_play_state == 0) {
        mp3_play_state = 1; // 开始播放中文
        idContentPlayMp3(handle, aiRet->url[1], "Chinese");
    } else if (mp3_play_state == 1 && !MP3_is_playing() && !dacIsPlaying() && MP3_get_totaltime() == 0) {
        mp3_play_state = 2; // 切换到英文
        idContentPlayMp3(handle, aiRet->url[0], "English");
    }
}

static int idContentKeyMsgOk(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        mp3_play_state = 0; // 重置播放状态以从中文开始
        idContentPlay(handle); // 重新播放音频
    }
    return 0;
}

static int idContentKeyMsgUp(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        // winDestroy(&handle);
        MP3_volume_add();
    }
    return 0;
}

static int idContentKeyMsgDown(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        // winDestroy(&handle);
        MP3_volume_dec();
    }
    return 0;
}

static int idContentKeyMsgRight(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        if (get_sd_status() == 0) {
            printf("==%s,%d, SD card not ready\n", __FUNCTION__, __LINE__);
            return 0;
        }
        f_mkdir(AUDIO_FOLDER);
        idContentPlay(handle);
    }
    return 0;
}

static int idContentKeyMsgMenu(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        // winDestroy(&handle);
    }
    return 0;
}

static int idContentKeyMsgMode(winHandle handle, uint32 parameNum, uint32* parame)
{
    uint32 keyState = KEY_STATE_INVALID;
    if (parameNum == 1) keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        // winDestroy(&handle);
    }
    return 0;
}

static int idContentSysMsgSD(winHandle handle, uint32 parameNum, uint32* parame)
{
    if ((SDC_STAT_NULL == SysCtrl.sdcard) || (SDC_STAT_ERROR == SysCtrl.sdcard)) {
        MP3_stop();
    } else if (SDC_STAT_NORMAL == SysCtrl.sdcard) {
        MP3_filelist_reload(AUDIO_FOLDER);
    }
    return 0;
}

static int idContentTouchWin(winHandle handle, uint32 parameNum, uint32* parame)
{
    if (parameNum != 3) {
        printf("==%s,%d, parame num error %d\n", __FUNCTION__, __LINE__, parameNum);
        return 0;
    }
    if (parame[2] == TOUCH_RELEASE) {
        setNotplayKeySoundFlag(1);
        sendMsgNonblock(OS_TASK_UI, KEY_EVENT_OK, KEY_PRESSED);
    }
    return 0;
}

static int idContentOpenWin(winHandle handle, uint32 parameNum, uint32* parame)
{
    if (!parame) {
        printf("==%s,%d, invalid parameters\n", __FUNCTION__, __LINE__);
        return 0;
    }
    MP3_init();
	delete_dir(AUDIO_FOLDER);
	f_mkdir(AUDIO_FOLDER);	
    printf("======uiSmallModelMsg=======\n");
    aiRet = (PicAiRet_T *)parame[0];
    if (aiRet && (aiRet->text[1] || aiRet->text[0] || aiRet->url[1] || aiRet->url[0])) {
        idContentShow(handle, aiRet->text[1], aiRet->text[0]);
        mp3_play_state = 0; // 重置播放状态
        if (get_sd_status() == 0) {
            printf("==%s,%d, SD card not ready\n", __FUNCTION__, __LINE__);
        } else {
            idContentPlay(handle); // 触发首次播放
        }
    } else {
        printf("==%s,%d, invalid aiRet data\n", __FUNCTION__, __LINE__);
        if (configValue2Int(CONFIG_ID_LANGUAGE) == LANUAGE_ENGLISH){
            idContentShow(handle, "photo err, try again", 0);
		}else if (configValue2Int(CONFIG_ID_LANGUAGE) == LANUAGE_SCHINESE){
            idContentShow(handle, "图片异常请重试", 0);
		}
    }
    
    return 0;
}

static int idContentCloseWin(winHandle handle, uint32 parameNum, uint32* parame)
{
    MP3_uninit();
    aiRet = NULL;

    return 0;
}

static int idContentWinChildClose(winHandle handle, uint32 parameNum, uint32* parame)
{
    return 0;
}

static int idContentSysMsg500MS(winHandle handle, uint32 parameNum, uint32* parame)
{
    if (aiRet && mp3_play_state > 0) {
        idContentPlay(handle); // 检查播放状态，自动切换
    }
    return 0;
}

static int idContentKeyMsgPower(winHandle handle, unsigned int parameNum, unsigned int* parame) {
    unsigned int keyState;
    keyState = KEY_STATE_INVALID;
    if (parameNum == 1)
        keyState = parame[0];
    if (keyState == KEY_PRESSED) {
        functionStart(FUNCTION_TAKE_PHOTO,0);
    }
    return 0;
}

msgDealInfor smallModelMsgDeal[] = {
    {SYS_OPEN_WINDOW, idContentOpenWin},
    {SYS_CLOSE_WINDOW, idContentCloseWin},
    {SYS_CHILE_COLSE, idContentWinChildClose},
    {SYS_TOUCH_WINDOW, idContentTouchWin},
    {KEY_EVENT_OK, idContentKeyMsgOk},
    {KEY_EVENT_UP, idContentKeyMsgUp},
    {KEY_EVENT_DOWN, idContentKeyMsgDown},
    {KEY_EVENT_RIGHT, idContentKeyMsgRight},
    {KEY_EVENT_MENU, idContentKeyMsgMenu},
    {SYS_EVENT_SDC, idContentSysMsgSD},
    {SYS_EVENT_500MS, idContentSysMsg500MS},
    {KEY_EVENT_MODE, idContentKeyMsgPower},
    {EVENT_MAX, NULL},
};

WINDOW(SmallModelWindow, smallModelMsgDeal, smallModelWin)
