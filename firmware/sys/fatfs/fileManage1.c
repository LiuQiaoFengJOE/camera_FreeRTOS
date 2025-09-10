#include "typedef.h"
#include "fatfs/ff.h"
#include "fatfs/fileOps.h"
#include "mediaFile/muxer.h"

#define  MAX_FILE_LEN              11
#define  AVI_JPG_COMB    1
typedef int (*nameCmp)(char* n1,char* n2);
typedef int (*_fileOk)(char* n,u32 type);
typedef struct _FILE_INFOR
{
	int firstDeal;
	int opsSuc;
	int endOfLoop;
	char frontFile[MAX_FILE_LEN];
	char rearFile[MAX_FILE_LEN];
	char curFile[MAX_FILE_LEN];
	char bakFile[MAX_FILE_LEN];
	nameCmp cmp;
	_fileOk fileOk;
}FILE_INFOR;
typedef struct _FILE_NAME_INFOR
{
	char *dir;
	char *preFile;
	char *nextFile;
	char *delFile;
	char *scanFile;
	u32 fileType;
}FILE_NAME_INFOR;
FILE_INFOR  fileInfors[FILE_TYPE_MAX];
FILE_NAME_INFOR fileNameInfors[FILE_TYPE_MAX]=
{
{COMB_DIR,FIND_PRE_VID,FIND_NEXT_VID,DELETE_OLD_VID,SCAN_VID_FILE,MEDIA_TYPE_AVI},
{VIDEO_DIR,FIND_PRE_VID,FIND_NEXT_VID,DELETE_OLD_VID,SCAN_VID_FILE,MEDIA_TYPE_AVI},
{SOS_DIR,FIND_PRE_SOS,FIND_NEXT_SOS,DELETE_OLD_SOS,SCAN_SOS_FILE,MEDIA_TYPE_AVI},
{PHOTO_DIR,FIND_PRE_PIC,FIND_NEXT_PIC,DELETE_OLD_PIC,SCAN_PIC_FILE,MEDIA_TYPE_JPG},
{AUDIO_DIR,FIND_PRE_AUD,FIND_NEXT_AUD,DELETE_OLD_AUD,SCAN_AUD_FILE,MEDIA_TYPE_WAV},
};
int dealType;
u32 curDirType;
char vidPrefix[5]="VID";
char sosPrefix[5]="SOS";
int fileIsSoSFile(char* name)
{
	if(name==NULL)
		return 0;
	if(name[0]==sosPrefix[0]&&name[1]==sosPrefix[1]&&name[2]==sosPrefix[2])
		return 1;
	return 0;
}
int fileNameCmp(char* n1,char* n2)
{
	int i;
	if(n1[0]==n2[0]&&n1[1]==n2[1]&&n1[2]==n2[2]&&n1[8]==n2[8]&&n1[9]==n2[9]&&n1[10]==n2[10])
	{
		for(i=3;i<8;i++)
		{
			if(n1[i]<'0'||n1[i]>'9')
				return -1;
		}
		for(i=3;i<8;i++)
		{
			if(n1[i]>n2[i])
				return 2;
			else if(n1[i]<n2[i])
				return 1;
		}
		return 0;
	}
	return -1;
}
int combFileCmp(char* n1,char* n2)
{
	int i;
	uint32 type1=MEDIA_TYPE(n1[8],n1[9],n1[10],0);
	if(type1!=MEDIA_TYPE_AVI&&type1!=MEDIA_TYPE_JPG)
		return -1;
	uint32 type2=MEDIA_TYPE(n2[8],n2[9],n2[10],0);
	if(type2!=MEDIA_TYPE_AVI&&type2!=MEDIA_TYPE_JPG)
		return -1;
	for(i=3;i<8;i++)
	{
		if(n1[i]<'0'||n1[i]>'9')
			return -1;
	}
	for(i=3;i<8;i++)
	{
		if(n1[i]>n2[i])
			return 2;
		else if(n1[i]<n2[i])
			return 1;
	}
	return 0;
}
int isCurFileType(char *n,u32 type)
{
	if(type>=FILE_TYPE_MAX)
		return 0;
	char* cur=fileNameInfors[type].nextFile;
	if(MEDIA_TYPE(n[8],n[9],n[10],0)!=fileNameInfors[type].fileType)
		return 0;
	if(n[0]==cur[0]&&n[1]==cur[1]&&n[2]==cur[2])
		return 1;
	return 0;
}
int isCombFolderFile(char *n,u32 type)
{
	if(type!=COMB_FILE)
		return 0;
	if(isCurFileType(n,AVI_FILE))
		return 1;
	if(isCurFileType(n,JPG_FILE))
		return 1;
	if(isCurFileType(n,LOCK_FILE))
		return 1;
	return 0;
}
u32 getDealType(u32 type)
{
#if AVI_JPG_COMB
	if(type==AVI_FILE||type==JPG_FILE)
		type=COMB_FILE;
#endif
	if(type==COMB_FILE)
	{
		fileInfors[type].cmp=combFileCmp;
		fileInfors[type].fileOk=isCombFolderFile;
	}
	else if(type<FILE_TYPE_MAX)
	{
		fileInfors[type].cmp=fileNameCmp;
		fileInfors[type].fileOk=isCurFileType;
	}
	dealType=type;
	return type;
}
void setDealFileType(u32 fileType)
{
	switch(fileType)
	{
		case MEDIA_TYPE_AVI:
			dealType=AVI_FILE;
			break;
		case MEDIA_TYPE_JPG:
			dealType=JPG_FILE;
			break;
		case MEDIA_TYPE_WAV:
			dealType=WAV_FILE;
			break;
		default:
			printf("unsupport type\n");
	}
	getDealType(dealType);
}
void changeDir(u32 fileType)
{
	fileType=getDealType(fileType);
	//if(curDirType==fileType)
	//	return;
	curDirType=fileType;
	f_chdir(fileNameInfors[fileType].dir);
}
void copyFullFileName(char* dest,char* src)
{
	int i;
	for(i=0;i<11;i++)
		dest[i]=src[i];
}
void copyFileName(char* dest,char* src)
{
	int i;
	for(i=3;i<8;i++)
		dest[i]=src[i];
}
void fileNameInitMin(char* name)
{
	int i;
	for(i=3;i<8;i++)
		name[i]='0';
}
void fileNameInitMax(char* name)
{
	int i;
	for(i=3;i<8;i++)
		name[i]='9';
	//name[6]='2';
}
int creatNextFileName(char* curName)
{
	int i;
	for(i=3;i<8;i++)
		if(curName[i]!='9')
			break;
	if(i==8)
	{
		fileNameInitMin(curName);
		return 0;
	}
	for(i=7;i>=3;i--)
	{
		if(curName[i]<'9')
		{
			curName[i]=curName[i]+1;
			return 0;
		}
		else
		{
			curName[i]='0';
		}
	}
	return -1;
}
int creatPreFileName(char* curName)
{
	int i;
	for(i=3;i<8;i++)
		if(curName[i]!='0')
			break;
	if(i==8)
	{
		fileNameInitMax(curName);
		return 0;
	}
	for(i=7;i>=3;i--)
	{
		if(curName[i]>'0')
		{
			curName[i]=curName[i]-1;
			return 0;
		}
		else
		{
			curName[i]='9';
		}
	}
	return -1;
}

void printFileName(char *name)
{
	int i;
	for(i=0;i<8;i++)
		printf("%c",name[i]);
	printf(".");
	for(i=8;i<11;i++)
		printf("%c",name[i]);
}
int scanFileInit(char *name)
{
	int i;
	dealType=0;
	if(name[3]=='S'&&name[4]=='C'&&name[5]=='A'&&name[6]=='N')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				fileNameInitMin(name);
				copyFullFileName(fileInfors[dealType].curFile,name);
				copyFullFileName(fileInfors[dealType].frontFile,name);
				copyFullFileName(fileInfors[dealType].rearFile,name);
				fileInfors[dealType].firstDeal=1;
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
static char curPrefix[5];
static char newPrefix[5];
int renameFileInit(char *name)
{
	int i;
	dealType=0;
	if(name[3]=='M'&&name[4]=='O'&&name[5]=='D'&&name[6]=='Y')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				curPrefix[0]=name[0];
				curPrefix[1]=name[1];
				curPrefix[2]=name[2];
				if(sosPrefix[0]==name[0]&&sosPrefix[1]==name[1]&&sosPrefix[2]==name[2])
				{
					newPrefix[0]=vidPrefix[0];
					newPrefix[1]=vidPrefix[1];
					newPrefix[2]=vidPrefix[2];
				}
				else
				{
					newPrefix[0]=sosPrefix[0];
					newPrefix[1]=sosPrefix[1];
					newPrefix[2]=sosPrefix[2];
				}
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
int renameOneFile(char *name)
{
	if(curPrefix[0]==name[0]&&curPrefix[1]==name[1]&&curPrefix[2]==name[2])
	{
		if(fileInfors[dealType].fileOk(name,dealType))
		{
			name[0]=newPrefix[0];
			name[1]=newPrefix[1];
			name[2]=newPrefix[2];
			return 1;
		}
	}
	return -1;
}

int scanOneFile(char *name,DWORD time)
{
	int i;
	char* cur=fileInfors[dealType].frontFile;
	//if(name[0]==cur[0]&&name[1]==cur[1]&&name[2]==cur[2]&&name[8]==cur[8]&&name[9]==cur[9]&&name[10]==cur[10])
	if(fileInfors[dealType].fileOk(name,dealType))
	{
		//printf("cur file:");
		//printFileName(name);
		//printf("\n");
		if(fileInfors[dealType].firstDeal)
		{
			for(i=3;i<8;i++)
			{
				if(name[i]<'0'||name[i]>'9')
					return -1;
			}
			fileInfors[dealType].firstDeal=0;
			copyFullFileName(fileInfors[dealType].frontFile,name);
			copyFullFileName(fileInfors[dealType].rearFile,name);
			return 0;
		}
		if(fileInfors[dealType].cmp(name, fileInfors[dealType].frontFile)==1)
		{
			copyFullFileName(fileInfors[dealType].frontFile,name);
		}
		if(fileInfors[dealType].cmp(name, fileInfors[dealType].rearFile)==2)
		{
			copyFullFileName(fileInfors[dealType].rearFile,name);
		}
		return 0;
	}
	return -1;
}
void scanFileEnd(int success)
{
	int i;
	printf("scan file type[%d]: ",dealType);
	if(success)
	{
		printf(" success\n");
		creatNextFileName(fileInfors[dealType].rearFile);
		printf("front file:");
		printFileName(fileInfors[dealType].frontFile);
		printf("\n");
		printf("rear file:");
		printFileName(fileInfors[dealType].rearFile);
		printf("\n");
	}
	else
		printf(" no file found\n");
}

int creatNewFileNameInit(char *name)
{
	//int i;
	dealType=0;
	if(name[3]=='N'&&name[4]=='E'&&name[5]=='W')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				copyFileName(name,fileInfors[dealType].rearFile);
				if(type==MEDIA_TYPE_AVI)
					copyFullFileName(fileInfors[dealType].curFile,fileInfors[dealType].rearFile);
				printf("create new file:");
				printFileName(name);
				printf("\n");
				creatNextFileName(fileInfors[dealType].rearFile);
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
int creatNewFileNameCmp(char* n1,char* n2)
{
	return fileInfors[dealType].cmp(n1, n2);
}
static u32 delFindFileErrCnt;
int deletOldFileInit(char *name)
{
	int i;
	dealType=0;
	if(name[3]=='D'&&name[4]=='E'&&name[5]=='L')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				copyFullFileName(name,fileInfors[dealType].frontFile);
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
int deletOldFileCmp(char* n1,char* n2)
{
	if(fileIsSoSFile(n1))
		return -1;
	return fileInfors[dealType].cmp(n1, n2);
}
void deletOldFileEnd(int success,char* name,DWORD size)
{
	int i;
	if(success)
	{
		if(!fileIsSoSFile(fileInfors[dealType].frontFile))
		{
			copyFullFileName(fileInfors[dealType].frontFile,name);
			creatNextFileName(fileInfors[dealType].frontFile);
		}
		//sdFreeSizeAdj(size,1);
		delFindFileErrCnt=0;
	}
	else
	{
		//for(i=3;i<8;i++)
		//	fileInfors[dealType].frontFile[i]='0';
		delFindFileErrCnt++;
		fileNameInitMin(fileInfors[dealType].frontFile);
	}
	printf("delet file: ");
	for(i=0;i<8;i++)
		printf("%c",name[i]);
	printf(".");
	for(i=8;i<11;i++)
		printf("%c",name[i]);
	if(success)
		printf(" success\n");
	else
		printf(" failed!!!\n");
}

int preFileInit(char *name)
{
	int i;
	dealType=0;
	if(name[3]=='P'&&name[4]=='R'&&name[5]=='E')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				fileInfors[dealType].endOfLoop=0;
				if(fileInfors[dealType].cmp(fileInfors[dealType].curFile,fileInfors[dealType].frontFile)==0)
					copyFullFileName(fileInfors[dealType].curFile,fileInfors[dealType].rearFile);
				else
					creatPreFileName(fileInfors[dealType].curFile);
				copyFullFileName(name,fileInfors[dealType].curFile);
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
int nextFileInit(char *name)
{
	int i;
	dealType=0;
	if(name[3]=='N'&&name[4]=='E'&&name[5]=='X'&&name[6]=='T')
	{
		uint32 type=MEDIA_TYPE(name[8],name[9],name[10],0);
		switch(type)
		{
			case MEDIA_TYPE_AVI:
			case MEDIA_TYPE_JPG:
			case MEDIA_TYPE_WAV:
				setDealFileType(type);
				fileInfors[dealType].endOfLoop=0;
				creatNextFileName(fileInfors[dealType].curFile);
				if(fileInfors[dealType].cmp(fileInfors[dealType].curFile,fileInfors[dealType].rearFile)==0)
				{
					copyFullFileName(fileInfors[dealType].curFile,fileInfors[dealType].frontFile);
				}
				copyFullFileName(name,fileInfors[dealType].curFile);
				return 0;
			default:
				printf("unsupport type\n");
		}
	}
	return -1;
}
int preOrNextFileCmp(char* n1,char* n2)
{
	return fileInfors[dealType].cmp(n1, n2);
}
void preFileFindEnd(int success,char* name)
{
	int i;
	fileInfors[dealType].opsSuc=success;
	if(success)
	{
		copyFullFileName(fileInfors[dealType].curFile,name);
	}
	else
	{
		fileNameInitMax(fileInfors[dealType].curFile);
	}
	printf("find file: ");
	for(i=0;i<8;i++)
		printf("%c",name[i]);
	printf(".");
	for(i=8;i<11;i++)
		printf("%c",name[i]);
	if(success)
		printf(" success\n");
	else
		printf(" failed!!!\n");
}
void nextFileFindEnd(int success,char* name)
{
	int i;
	fileInfors[dealType].opsSuc=success;
	if(success)
	{
		copyFullFileName(fileInfors[dealType].curFile,name);
	}
	else
	{
		fileNameInitMin(fileInfors[dealType].curFile);
	}
	printf("find file: ");
	for(i=0;i<8;i++)
		printf("%c",name[i]);
	printf(".");
	for(i=8;i<11;i++)
		printf("%c",name[i]);
	if(success)
		printf(" success\n");
	else
		printf(" failed!!!\n");
}

void findRealFrontFile(u32 fileType)
{
	void* fp;
	if(fileType>=FILE_TYPE_MAX)
		return 0;
	fileType=getDealType(fileType);
	fileInfors[fileType].opsSuc=0;
	changeDir(fileType);
	copyFullFileName(fileInfors[fileType].curFile,fileInfors[fileType].frontFile);
	creatPreFileName(fileInfors[fileType].curFile);
	fp=osal_fopen(fileNameInfors[fileType].nextFile, "r");
	osal_fclose(fp);
	fp=NULL;
	if(fileInfors[fileType].opsSuc)
	{
		copyFullFileName(fileInfors[fileType].frontFile,fileInfors[fileType].curFile);
	}
	else
	{
		fileNameInitMax(fileInfors[fileType].curFile);
		fp=osal_fopen(fileNameInfors[fileType].nextFile, "r");
		osal_fclose(fp);
		fp=NULL;
		if(fileInfors[fileType].opsSuc)
		{
			copyFullFileName(fileInfors[fileType].frontFile,fileInfors[fileType].curFile);
		}
		else
		{
			fileNameInitMin(fileInfors[fileType].frontFile);
			fileNameInitMin(fileInfors[fileType].rearFile);
		}
	}
	if(fileInfors[fileType].opsSuc)
	{
		printf("front file:");
		printFileName(fileInfors[fileType].frontFile);
		printf("\n");
		printf("rear file:");
		printFileName(fileInfors[fileType].rearFile);
		printf("\n");
	}
}
void findRealRearFile(u32 fileType)
{
	void* fp;
	if(fileType>=FILE_TYPE_MAX)
		return 0;
	fileType=getDealType(fileType);
	fileInfors[fileType].opsSuc=0;
	changeDir(fileType);
	copyFullFileName(fileInfors[fileType].curFile,fileInfors[fileType].rearFile);
	fp=osal_fopen(fileNameInfors[fileType].preFile, "r");
	osal_fclose(fp);
	fp=NULL;
	if(fileInfors[fileType].opsSuc)
	{
		copyFullFileName(fileInfors[fileType].rearFile,fileInfors[fileType].curFile);
		creatNextFileName(fileInfors[fileType].rearFile);
	}
	else
	{
		fileNameInitMin(fileInfors[fileType].rearFile);
		copyFullFileName(fileInfors[fileType].curFile,fileInfors[fileType].rearFile);
		fp=osal_fopen(fileNameInfors[fileType].preFile, "r");
		osal_fclose(fp);
		fp=NULL;
		if(fileInfors[fileType].opsSuc)
		{
			copyFullFileName(fileInfors[fileType].rearFile,fileInfors[fileType].curFile);
			creatNextFileName(fileInfors[fileType].rearFile);
		}
		else
		{
			fileNameInitMin(fileInfors[fileType].frontFile);
			fileNameInitMin(fileInfors[fileType].rearFile);
		}
	}
	if(fileInfors[fileType].opsSuc)
	{
		printf("front file:");
		printFileName(fileInfors[fileType].frontFile);
		printf("\n");
		printf("rear file:");
		printFileName(fileInfors[fileType].rearFile);
		printf("\n");
	}
}

int dirHasFile(u32 fileType)
{
	int ret;
	if(get_sd_status()==0)
		return 0;
	if(fileType>=FILE_TYPE_MAX)
		return 0;
	fileType=getDealType(fileType);
	ret=fileInfors[fileType].cmp(fileInfors[fileType].frontFile,fileInfors[fileType].rearFile);
	if(ret<=0)
		return 0;
	return 1;
}
void fileInforInit(void)
{
#if AVI_JPG_COMB
	findRealFrontFile(COMB_FILE);
	copyFullFileName(fileInfors[COMB_FILE].curFile,fileInfors[COMB_FILE].frontFile);
	printf("current file:");
	printFileName(fileInfors[COMB_FILE].curFile);
	printf("\n");
#else
	findRealFrontFile(AVI_FILE);
	findRealFrontFile(JPG_FILE);
	copyFullFileName(fileInfors[AVI_FILE].curFile,fileInfors[AVI_FILE].frontFile);
	copyFullFileName(fileInfors[JPG_FILE].curFile,fileInfors[JPG_FILE].frontFile);
#endif	
	findRealFrontFile(WAV_FILE);
	copyFullFileName(fileInfors[WAV_FILE].curFile,fileInfors[WAV_FILE].frontFile);
}
int hasFile(void)
{
	if(get_sd_status()==0)
		return 0;
#if AVI_JPG_COMB
	if(dirHasFile(COMB_FILE))
		return 1;
#else
	if(dirHasFile(AVI_FILE))
		return 1;
	if(dirHasFile(JPG_FILE))
		return 1;
#endif
	return 0;
}
char* getCurFileName(char *str,u32 len)
{
	int i;
	char *name=fileInfors[dealType].curFile;
	if(str&&len>12)
	{
		for(i=0;i<8;i++)
			str[i]=name[i];
		str[i]='.';
		for(i=8;i<11;i++)
			str[i+1]=name[i];
		str[i+1]=0;
	}
	return fileInfors[dealType].curFile;
}

void fileOpsReset(void)
{
	//cycleFinish=1;
}

int needDeletFile(u32 fileType,u32 needMB)
{
	int i;
	char nextNewFile[MAX_FILE_LEN];
	fileType=getDealType(fileType);
	delFindFileErrCnt=0;
	if((getSdFreeSize()>>11)<needMB)
	{
		do{
			f_unlink(fileNameInfors[fileType].delFile);
			if(delFindFileErrCnt>=3)
			{
				printf("floder[%s] no file to delete\n",fileNameInfors[fileType].dir);
				break;
			}
		}while((getSdFreeSize()>>11)<needMB);
		printf("sd free size:%dMB\n",getSdFreeSize()>>11);
		return 1;
	}
	if(fileType>=FILE_TYPE_MAX)
		return 0;
	for(i=0;i<MAX_FILE_LEN;i++)
		nextNewFile[i]=fileInfors[fileType].rearFile[i];
	if(fileNameCmp(nextNewFile,fileInfors[fileType].frontFile)==0)
	{
		printf(" file queue full,will delet file\n");
		f_unlink(fileNameInfors[fileType].delFile);
	}
	creatNextFileName(nextNewFile);
	if(fileNameCmp(nextNewFile,fileInfors[fileType].frontFile)==0)
	{
		printf(" file queue full,will delet file\n");
		printf("sd free size:%dMB\n",getSdFreeSize()>>11);
		f_unlink(fileNameInfors[fileType].delFile);
		return 1;
	}
	printf("sd free size:%dMB\n",getSdFreeSize()>>11);
	return 0;
}
int curFloderHasFile(void)
{
	return dirHasFile(curDirType);
}
int curFileIsLock(void)
{
	return fileIsSoSFile(fileInfors[curDirType].curFile);
}
void setCurFileLock(u8 lock)
{
	char* name=fileInfors[curDirType].curFile;
	if(lock)
	{
		name[0]=sosPrefix[0];
		name[1]=sosPrefix[1];
		name[2]=sosPrefix[2];
	}
	else
	{
		name[0]=vidPrefix[0];
		name[1]=vidPrefix[1];
		name[2]=vidPrefix[2];
	}
}
int delCurFile(void)
{
	char fileName[13];
	int ret;
	getCurFileName(fileName,13);
	ret=f_unlink(fileName);
	if(ret != FR_OK)
	{
		printf("delete file[%s] err:%d\n",fileName,ret);
		return -1;
	}
	return 0;
}
void lockCurFolderFile(void)
{
	if(curDirType>=FILE_TYPE_MAX)
		return;
	f_scan(LOCK_ALL_FILE);
}
void unlockCurFolderFile(void)
{
	if(curDirType>=FILE_TYPE_MAX)
		return;
	f_scan(UNLOCK_ALL_FILE);
}
void fileScanCurFolder(void)
{
	if(curDirType>=FILE_TYPE_MAX)
		return;
	f_scan(fileNameInfors[curDirType].scanFile);
	copyFullFileName(fileInfors[COMB_FILE].curFile,fileInfors[COMB_FILE].frontFile);
}
void saveCurFile(void)
{
	if(curDirType>=FILE_TYPE_MAX)
		return;
	copyFullFileName(fileInfors[curDirType].bakFile,fileInfors[curDirType].curFile);
}
void restoreCurFile(void)
{
	if(curDirType>=FILE_TYPE_MAX)
		return;
	copyFullFileName(fileInfors[curDirType].curFile,fileInfors[curDirType].bakFile);
}
void delAllVidJpgFile(void)
{
	delFindFileErrCnt=0;
	#if AVI_JPG_COMB
	while(1)
	{
		f_unlink(fileNameInfors[COMB_FILE].delFile);
		if(delFindFileErrCnt>=3)
		{
			printf("floder[%s] no file to delete\n",fileNameInfors[COMB_FILE].dir);
			break;
		}
	}
	#else
	delFindFileErrCnt=0;
	while(1)
	{
		f_unlink(fileNameInfors[AVI_FILE].delFile);
		if(delFindFileErrCnt>=3)
		{
			printf("floder[%s] no file to delete\n",fileNameInfors[AVI_FILE].dir);
			break;
		}
	}
	delFindFileErrCnt=0;
	while(1)
	{
		f_unlink(fileNameInfors[JPG_FILE].delFile);
		if(delFindFileErrCnt>=3)
		{
			printf("floder[%s] no file to delete\n",fileNameInfors[JPG_FILE].dir);
			break;
		}
	}
	#endif
}
#if 0
void fileScan(void)
{
	curDirType=FILE_TYPE_MAX;
#if AVI_JPG_COMB
	f_mkdir(COMB_DIR);
	changeDir(COMB_FILE);
	f_scan(SCAN_VID_FILE);
	copyFullFileName(fileInfors[COMB_FILE].curFile,fileInfors[COMB_FILE].frontFile);
#else
	f_mkdir(VIDEO_DIR);
	changeDir(AVI_FILE);
	f_scan(SCAN_VID_FILE);
	copyFullFileName(fileInfors[AVI_FILE].curFile,fileInfors[AVI_FILE].frontFile);
	
	f_mkdir(PHOTO_DIR);
	changeDir(JPG_FILE);
	f_scan(SCAN_PIC_FILE);
	copyFullFileName(fileInfors[JPG_FILE].curFile,fileInfors[JPG_FILE].frontFile);
#endif	
	f_mkdir(AUDIO_DIR);
	changeDir(WAV_FILE);
	f_scan(SCAN_AUD_FILE);
	copyFullFileName(fileInfors[WAV_FILE].curFile,fileInfors[WAV_FILE].frontFile);
	calFreeSpace();
	//aviReadHeaderInfor("1:/DCIM/AVI00000.AVI");
	//jpgThumbnailInfor("1:/PHOTO/PIC00000.JPG");
}

#endif











