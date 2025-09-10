#ifndef    _F_NAME_OPS_H
#define    _F_NAME_OPS_H

#include "typedef.h"

enum
{
	COMB_FILE=0,
	AVI_FILE,
	LOCK_FILE,
	JPG_FILE,
	WAV_FILE,
	FILE_TYPE_MAX
};

#define     CREATE_VID_NEW       "VIDNEW.AVI"
#define     CREATE_PIC_NEW       "PICNEW.JPG"
#define     CREATE_AUD_NEW       "AUDNEW.WAV"

#define     DELETE_OLD_VID       "VIDDEL.AVI"
#define     DELETE_OLD_SOS       "SOSDEL.AVI"
#define     DELETE_OLD_PIC       "PICDEL.JPG"
#define     DELETE_OLD_AUD      "AUDDEL.WAV"

#define     FIND_PRE_VID       "VIDPRE.AVI"
#define     FIND_PRE_SOS       "SOSPRE.AVI"
#define     FIND_PRE_PIC       "PICPRE.JPG"
#define     FIND_PRE_AUD      "AUDPRE.WAV"
#define     FIND_NEXT_VID       "VIDNEXT.AVI"
#define     FIND_NEXT_SOS       "SOSNEXT.AVI"
#define     FIND_NEXT_PIC       "PICNEXT.JPG"
#define     FIND_NEXT_AUD      "AUDNEXT.WAV"

#define     SCAN_VID_FILE       "VIDSCAN.AVI"
#define     SCAN_SOS_FILE       "SOSSCAN.AVI"
#define     SCAN_PIC_FILE       "PICSCAN.JPG"
#define     SCAN_AUD_FILE      "AUDSCAN.WAV"

#define     LOCK_ALL_FILE    "VIDMODY.AVI"
#define     UNLOCK_ALL_FILE    "SOSMODY.AVI"

#define     COMB_DIR        "/DCIM"
#define     VIDEO_DIR        "/VIDEO" //"/VIDEO"
#define     SOS_DIR        "/LOCK"
#define     PHOTO_DIR        "/PHOTO"
#define     AUDIO_DIR        "/AUDIO"




int needDeletFile(u32 fileType,u32 needMB);
char* getCurFileName(char *str,u32 len);
void changeDir(u32 fileType);
u32 getSdFreeSize(void);
void sdFreeSizeAdj(u32 size,int add);
int curFloderHasFile(void);
int curFileIsLock(void);
int delCurFile(void);
void delAllVidJpgFile(void);
void fileScanCurFolder(void);
void saveCurFile(void);
void restoreCurFile(void);
void setCurFileLock(u8 lock);
void lockCurFolderFile(void);
void unlockCurFolderFile(void);






#endif


