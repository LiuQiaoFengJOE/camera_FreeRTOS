#include "task/app.h"
#include "fatfs/ff.h"
#include"fatfs/fileManage.h"
#include "mediaFile/picIdentify.h"


// ��������Ƶ������غ궨��
#define AUDIO_FOLDER "1:/tmp"
//#define RECORD_FOLDER "1:/AUDIO"

typedef struct {
    int isDownloading;      // ����״̬��0=���У�1=������
    int curIdx;       // ��ǰ��������
} AudioDownloadState;

// ȫ������״̬
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
        return -1; // �ļ���ʧ��
    }

    uint8_t header[10]; // ��ȡǰ10�ֽ��㹻�ж�
    size_t bytes_read = osal_fread(header, 1, sizeof(header), fp);
    osal_fclose(fp);

    if (bytes_read < sizeof(header)) {
        return 0; // �ļ�̫С����������MP3
    }

    // ���ID3v2��ǩ (ǰ3�ֽ���"ID3")
    if (header[0] == 'I' && header[1] == 'D' && header[2] == '3') {
        return 1;
    }

    // ���MP3֡ͷͬ���� (0xFFE��0xFFF)
    // ͬ������11λ1����Ӱ汾�Ͳ���Ϣ
    if ((header[0] == 0xFF) && ((header[1] & 0xE0) == 0xE0)) {
        // ���汾�Ͳ� (MPEG-1/2/2.5, Layer III)
        uint8_t version_layer = (header[1] >> 3) & 0x03;
        uint8_t layer = (header[1] >> 1) & 0x03;
        
        // �汾: 11=MPEG-1, 10=MPEG-2, 00=MPEG-2.5
        // ��: 01=Layer III (MP3)
        if (layer == 0x01) { // Layer III
            return 1;
        }
    }

    // ����ļ�ĩβ�Ƿ���ID3v1��ǩ (128�ֽڵ�"TAG")
    // ��Ҫ��ȡ�ļ����128�ֽ�
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

    return 0; // ����MP3�ļ�
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

// ��Ƶ����������ں���
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
           // printf("[downloadAudioBatch] url[%d] ��Ч, ����\n", i);
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
    // ����״̬���˳�����
    osal_task_exit();  // ɾ����������
}


// ֹͣ��Ƶ��������
int picAudioDownloadStop(void)
{
    if (!audioDownloadState.isDownloading) {
        printf("[picAudioDownloadStop] �������������ڽ���\n");
        return 0;
    }

    // ����ֹͣ��־��������Ⲣ�˳�
    audioDownloadState.isDownloading = 0;
   // memset(localAudioPaths, 0, sizeof(localAudioPaths));
    printf("[picAudioDownloadStop]\n");
    return 0;
}

// ������Ƶ��������
int requestAudioDownload(void *param)
{
    if (audioDownloadState.isDownloading) {
        printf("[picAudioDownloadStart] �������أ���������\n");
        return -1;
    }
	picAiRet = param;
    audioDownloadState.isDownloading = 1;
	audioDownloadState.curIdx  = 0;
    // ����������ƽṹ��
    OSAL_TASK task_ctrl;
    task_ctrl.tsk_entry_func = audioDownloadTask;
    task_ctrl.param = param;
    task_ctrl.tsk_prio = 2;  // ���ȼ�������ϵͳ�����������е����ȼ���
    task_ctrl.stk_size = 4096;  // ջ��С���ֵ�λ������FreeRTOS���ã�
    strncpy(task_ctrl.name, "AudioDownloadTask", sizeof(task_ctrl.name) - 1);

    // ��������
    TASK_HDL task_hdl = osal_task_create(&task_ctrl);
    if (task_hdl == NULL) {
        printf("[picAudioDownloadStart] ������Ƶ��������ʧ��\n");
        audioDownloadState.isDownloading = 0;
        return -2;
    }
    printf("[picAudioDownloadStart] ��Ƶ�������񴴽��ɹ�\n");
    return 0;
}

// ������Ƶ����

