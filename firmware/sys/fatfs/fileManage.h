#ifndef  _FILE_MANAGE_H
#define  _FILE_MANAGE_H
#include "fatfs/bitArray.h"
#include "task/app.h"
#if (USB1_CAMERA||USB2_CAMERA)
#define SUBTITLES_SUPPORT	1
#else
#define SUBTITLES_SUPPORT	0
#endif
enum
{
	COMB_FILE=0,
	AVI_FILE,
	LOCK_FILE,
	JPG_FILE,
	WAV_FILE,
	FILE_TYPE_MAX
};

enum
{
	REC_FD=0,
	AUD_FD,
	FD_TYPE_MAX
};

typedef struct
{
	char *dir;
	char *arryFile;
	char fileName[MAX_NAME_LEN];
	char *preFix[4];
	char *sufFix[4];
	int delMask;
	u8 unit;
	int len;
	u32 *buff;
	_data2Name d2name;
	_name2Data name2d;
	_bitArray array;
}_folderManage;

#define     CREATE_VID_NEW       createVidName(NULL)
#define     CREATE_PIC_NEW       createJpgName(NULL)

#define     FIND_PRE_VID       preVidName()
#define     FIND_NEXT_VID      nextVidName()

#define     FIND_PRE_JPG       preJpgName()
#define     FIND_NEXT_JPG      nextJpgName()

#define 	FIND_CUR_FILE		curFileName()
#define 	FIND_LAST_FILE		lastFileName()
#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
#define     CREATE_AUD_NEW       createAudName(NULL)
#define     FIND_PRE_WAV       preWavName()
#define     FIND_NEXT_WAV      nextWavName()
#endif

int curFileIsLock(void);
int hasFile(void);
int curFloderHasFile(void);
void delAllVidJpgFile(void);
int fileIsLock(int data);
void setFileLock(int data,u8 lock);
void saveFileInfor(u32 fileType);
int getFileSum(void);
char *getFileByIdx(int idx,int *type);
char *getFileByData(int data,int *type);
char *getFileByIdxEx(int idx,int *pData,int *type);
int getMaxFileSum(void);
int delDateFile(u32 fileType,u32 deleteData);
int delDateFileByName(u32 fileType,char *name);

int getCurFileIdx(u32 fileType);



#endif




