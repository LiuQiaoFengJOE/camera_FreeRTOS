#include "task/app.h"
#include "fatfs/ff.h"
#include"fatfs/fileManage.h"
#include "mediaFile/picIdentify.h"


// 新增：音频下载相关宏定义
#define AUDIO_FOLDER "1:/tmp"
//#define RECORD_FOLDER "1:/AUDIO"

typedef struct {
    int isDownloading;      // 下载状态：0=空闲，1=进行中
    int curIdx;       // 当前下载索引
} AudioDownloadState;

// 全局下载状态
static AudioDownloadState audioDownloadState = {0};
static PicAiRet_T *picAiRet = NULL;

PicAiRet_T *getpidAiRet(void)
{
	return picAiRet;
}

static int idContentFileIsMp3(const char *filename)
{
    FILE *fp = osal_fopen(filename, "rb");
    if (!fp) {
        return -1; // 文件打开失败
    }

    uint8_t header[10]; // 读取前10字节足够判断
    size_t bytes_read = osal_fread(header, 1, sizeof(header), fp);
    osal_fclose(fp);

    if (bytes_read < sizeof(header)) {
        return 0; // 文件太小，不可能是MP3
    }

    // 检查ID3v2标签 (前3字节是"ID3")
    if (header[0] == 'I' && header[1] == 'D' && header[2] == '3') {
        return 1;
    }

    // 检查MP3帧头同步字 (0xFFE或0xFFF)
    // 同步字是11位1，后接版本和层信息
    if ((header[0] == 0xFF) && ((header[1] & 0xE0) == 0xE0)) {
        // 检查版本和层 (MPEG-1/2/2.5, Layer III)
        uint8_t version_layer = (header[1] >> 3) & 0x03;
        uint8_t layer = (header[1] >> 1) & 0x03;
        
        // 版本: 11=MPEG-1, 10=MPEG-2, 00=MPEG-2.5
        // 层: 01=Layer III (MP3)
        if (layer == 0x01) { // Layer III
            return 1;
        }
    }

    // 检查文件末尾是否有ID3v1标签 (128字节的"TAG")
    // 需要读取文件最后128字节
    fp = osal_fopen(filename, "rb");
    if (!fp) {
        return 0;
    }
    
    osal_fseek(fp, -128, SEEK_END);
    uint8_t id3v1_tag[3];
    osal_fread(id3v1_tag, 1, 3, fp);
    osal_fclose(fp);
    
    if (id3v1_tag[0] == 'T' && id3v1_tag[1] == 'A' && id3v1_tag[2] == 'G') {
        return 1;
    }

    return 0; // 不是MP3文件
}
FRESULT delete_dir(const char* path)
{
    FRESULT res;
    DIR dir;
    FILINFO fno;
    char full_path[256];
    
    res = f_opendir(&dir, path);
    if (res != FR_OK) {
        printf("==%s,%d, f_opendir %s failed: %d\n", __FUNCTION__, __LINE__, path, res);
        return res;
    }
    
    for (;;) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (fno.fname[0] == '.') continue;
        
        sprintf(full_path, "%s/%s", path, fno.fname);
        
        if (fno.fattrib & AM_DIR) {
            res = delete_dir(full_path); // Recursively delete subdirectory contents
            if (res != FR_OK) break;
        } else {
            res = f_unlink(full_path); // Delete file
            if (res != FR_OK) {
                printf("==%s,%d, f_unlink %s failed: %d\n", __FUNCTION__, __LINE__, full_path, res);
                break;
            }
        }
    }
    
    f_closedir(&dir);
    return res;
}

// 音频下载任务入口函数
static void audioDownloadTask(void *param) {
    uint32_t flags;
	PicAiRet_T *aiRet = (PicAiRet_T *)param;
    int i, ret = 0;
	int dcnt;
    char audio_filename[64];

	osal_task_sleep(500);
audioDownload:
	printf("====%s,%d, curIdx:%d, total:%d\n", __FUNCTION__, __LINE__, audioDownloadState.curIdx, aiRet->urlNum);
    for (i = audioDownloadState.curIdx; i < aiRet->urlNum; i++) {

		if (audioDownloadState.isDownloading == 0)
		{
			break;
		}
		
        if (!aiRet->url[i] || strlen(aiRet->url[i]) == 0) {
           // printf("[downloadAudioBatch] url[%d] 无效, 跳过\n", i);
            ret = 1;
            continue;
        }
		
		dcnt = 0;
        sprintf(audio_filename, "%s/tmpAudio%d.mp3", AUDIO_FOLDER, i);
downloadOne:
		ret = picAiDownloadFile(aiRet->url[i], audio_filename);
		if (ret != 0)
		{
			printf("====%s,%d, err!\n", __FUNCTION__, __LINE__);
            ret = 1;
        } else {
        	if ((idContentFileIsMp3(audio_filename) != 1) && (dcnt++<3))
			{
				printf("====redownload file:%s\n", audio_filename);
				osal_task_sleep(500);
				f_unlink(audio_filename);
				goto downloadOne;
			}
			
        	audioDownloadState.curIdx = i;
            if ((SysCtrl.photoMode == PHOTO_IDCONTENT) || (SysCtrl.photoMode == PHOTO_AI_CHAT)){
                sendMsgNonblock(OS_TASK_UI, VID_EVENT_IDENTIFY_UPDATE, 0);
            }
        }
		osal_task_sleep(50);
    }

	if ((aiRet->reCode != 4) && (audioDownloadState.isDownloading != 0))
	{
		aiRet = picIdtfyContentGet();
		printf("====re audio download, curidx:%d, urlNum:%d\n", audioDownloadState.curIdx, aiRet->urlNum);
		if (aiRet && aiRet->urlNum > audioDownloadState.curIdx+1)
		{
			picAiRet = aiRet;
			goto audioDownload;
		}
	}

	printf("exit audioDownloadTask\n");
    // 清理状态并退出任务
    osal_task_exit();  // 删除自身任务
}


// 停止音频下载任务
int picAudioDownloadStop(void)
{
    if (!audioDownloadState.isDownloading) {
        printf("[picAudioDownloadStop] 无下载任务正在进行\n");
        return 0;
    }

    // 设置停止标志，任务会检测并退出
    audioDownloadState.isDownloading = 0;
   // memset(localAudioPaths, 0, sizeof(localAudioPaths));
    printf("[picAudioDownloadStop]\n");
    return 0;
}

// 启动音频下载任务
int requestAudioDownload(void *param)
{
    if (audioDownloadState.isDownloading) {
        printf("[picAudioDownloadStart] 正在下载，忽略请求\n");
        return -1;
    }
	picAiRet = param;
    audioDownloadState.isDownloading = 1;
	audioDownloadState.curIdx  = 0;
    // 定义任务控制结构体
    OSAL_TASK task_ctrl;
    task_ctrl.tsk_entry_func = audioDownloadTask;
    task_ctrl.param = param;
    task_ctrl.tsk_prio = 2;  // 优先级（根据系统调整，假设中等优先级）
    task_ctrl.stk_size = 4096;  // 栈大小（字单位，根据FreeRTOS配置）
    strncpy(task_ctrl.name, "AudioDownloadTask", sizeof(task_ctrl.name) - 1);

    // 创建任务
    TASK_HDL task_hdl = osal_task_create(&task_ctrl);
    if (task_hdl == NULL) {
        printf("[picAudioDownloadStart] 创建音频下载任务失败\n");
        audioDownloadState.isDownloading = 0;
        return -2;
    }
    printf("[picAudioDownloadStart] 音频下载任务创建成功\n");
    return 0;
}

// 请求音频下载

