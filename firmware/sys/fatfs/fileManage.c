#include"fatfs/fileManage.h"

#define RECORD_FILE_SUM   (32*1000)
#define RECORD_FILE_UNIT   2
#define RECORD_FILE_LEN   (RECORD_FILE_SUM*RECORD_FILE_UNIT)/32
char vidPrefix[5]="VID";
char sosPrefix[5]="SOS";

#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
#define AUD_FILE_SUM   (32*1000)
#define AUD_FILE_UNIT   1
#define AUD_FILE_LEN   (AUD_FILE_SUM*AUD_FILE_UNIT)/32
u32 audFolderBuff[AUD_FILE_LEN];
#endif

u32 recordFolderBuff[RECORD_FILE_LEN];
_folderManage fMgs[FD_TYPE_MAX];
u32 curFolder;
#if 1
u32 getFileDir(u32 fileType)
{
	if(fileType==AVI_FILE||fileType==LOCK_FILE||fileType==JPG_FILE)
		return REC_FD;
	#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
	else if(fileType==WAV_FILE)
		return AUD_FD;
	#endif
	return FD_TYPE_MAX;
}
void changeDir(u32 fileType)
{
	curFolder=getFileDir(fileType);
}
void fileInforInit(u32 fileType)
{
	u32 fileDir=getFileDir(fileType);
	if(fileDir>=FD_TYPE_MAX)
		return;
	bitArrayCheck(&fMgs[fileDir].array);
}
void fileScanCurFolder(void)
{
}
void saveCurFile(void)
{
}
void restoreCurFile(void)
{
}

#endif
void dirScan(_bitArray *bitArray,char *path)
{
	DIR dir;
	FRESULT res;
	FILINFO fno;
	res = f_opendir(&dir, path);
    if (res == FR_OK) 
	{
        for (;;) 
		{
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) 
				break;
			bitArrayAddName(bitArray,fno.fname);
        }
		f_closedir(&dir);
		return;
    }
	printf("open dir [%s] err\n",path);
}
void fMgInit(_folderManage *fMg,int subtitleEn)
{
	f_mkdir(fMg->dir);
	bitArrayInit(&fMg->array,fMg->buff,fMg->len,fMg->unit,fMg->delMask,fMg->d2name,fMg->name2d);
	bitArrayLoad(&fMg->array,fMg->arryFile);
	dirScan(&fMg->array,fMg->dir);
	bitArrayCheck(&fMg->array);
	bitArraySubtitleEn(&fMg->array,subtitleEn);
}
void fMgDeleteFile(_folderManage *fMg)
{
	bitArrayDeleteHead(&fMg->array);
}
char *recData2Name(int data,int type)
{
	if(type<1||type>3)
	{
		printf("type [%d] err\n",type);
		return NULL;
	}
	sprintf(fMgs[REC_FD].fileName,"%s/%s%05d.%s",fMgs[REC_FD].dir,fMgs[REC_FD].preFix[type],data,fMgs[REC_FD].sufFix[type]);
	return fMgs[REC_FD].fileName;
}

#define  VID_TY_IDX   1
#define  SOS_TY_IDX   2
#define  JPG_TY_IDX   3
#define VID_DIR_LEN  8

#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
#define  AUD_TY_IDX   1
#define AUD_DIR_LEN  9
#endif
int recName2Data(char *name,int* type)
{
	int i,d;
	if(name[0]==fMgs[REC_FD].preFix[VID_TY_IDX][0]&&name[1]==fMgs[REC_FD].preFix[VID_TY_IDX][1]&&name[2]==fMgs[REC_FD].preFix[VID_TY_IDX][2])
	{
		if(name[9]!=fMgs[REC_FD].sufFix[VID_TY_IDX][0]||name[10]!=fMgs[REC_FD].sufFix[VID_TY_IDX][1]||name[11]!=fMgs[REC_FD].sufFix[VID_TY_IDX][2])
			goto ERR_FILE;
		if(type)
			*type=VID_TY_IDX;
		goto GET_DATA;
	}
	if(name[0]==fMgs[REC_FD].preFix[SOS_TY_IDX][0]&&name[1]==fMgs[REC_FD].preFix[SOS_TY_IDX][1]&&name[2]==fMgs[REC_FD].preFix[SOS_TY_IDX][2])
	{
		if(name[9]!=fMgs[REC_FD].sufFix[SOS_TY_IDX][0]||name[10]!=fMgs[REC_FD].sufFix[SOS_TY_IDX][1]||name[11]!=fMgs[REC_FD].sufFix[SOS_TY_IDX][2])
			goto ERR_FILE;
		if(type)
			*type=SOS_TY_IDX;
		goto GET_DATA;
	}
	if(name[0]==fMgs[REC_FD].preFix[JPG_TY_IDX][0]&&name[1]==fMgs[REC_FD].preFix[JPG_TY_IDX][1]&&name[2]==fMgs[REC_FD].preFix[JPG_TY_IDX][2])
	{
		if(name[9]!=fMgs[REC_FD].sufFix[JPG_TY_IDX][0]||name[10]!=fMgs[REC_FD].sufFix[JPG_TY_IDX][1]||name[11]!=fMgs[REC_FD].sufFix[JPG_TY_IDX][2])
			goto ERR_FILE;
		if(type)
			*type=JPG_TY_IDX;
		goto GET_DATA;
	}
ERR_FILE:
	return -1;
GET_DATA:
	d=0;
	for(i=3;i<8;i++)
	{
		d=d*10+name[i]-'0';
	}
	return d;
}


/*#define  AUD_TY_IDX   1
char *audData2Name(int data,int type)
{
	if(type!=1)
	{
		printf("type [%d] err\n",type);
		return NULL;
	}
	sprintf(fMgs[AUD_FD].fileName,"%s/%s%05d.%s",fMgs[AUD_FD].dir,fMgs[AUD_FD].preFix[type],data,fMgs[AUD_FD].sufFix[type]);
	return fMgs[AUD_FD].fileName;
}
int audName2Data(char *name,int* type)
{
	int i,d;
	if(name[0]==fMgs[AUD_FD].preFix[1][0]&&name[1]==fMgs[AUD_FD].preFix[1][1]&&name[2]==fMgs[AUD_FD].preFix[1][2])
	{
		if(type)
			*type=AUD_TY_IDX;
		goto GET_DATA;
	}
	return -1;
GET_DATA:
	d=0;
	for(i=3;i<8;i++)
	{
		d=d*10+name[i]-'0';
	}
	return d;
}*/
char *createJpgName(int *nameNum)
{
	return bitArrayCreatName(&fMgs[REC_FD].array,3,nameNum);
}
char *createVidName(int *nameNum)
{
	return bitArrayCreatName(&fMgs[REC_FD].array,1,nameNum);
}
char *preVidName(void)
{
	return bitArrayGetPreIdxName(&fMgs[REC_FD].array);
}
char *nextVidName(void)
{
	return bitArrayGetNextIdxName(&fMgs[REC_FD].array);
}

char *preJpgName(void)
{
	return bitArrayGetPreIdxNameByType(&fMgs[REC_FD].array, JPG_TY_IDX);
}
char *nextJpgName(void)
{
	return bitArrayGetNextIdxNameByType(&fMgs[REC_FD].array, JPG_TY_IDX);
}
char *curFileName(void)
{
	return bitArrayGetCurIdxName(&fMgs[curFolder].array,NULL);
}
char *lastFileName(void)
{
	return bitArrayGetLastIdxName(&fMgs[curFolder].array,NULL);
}
int hasFile(void)
{
	if(bitArrayGetSum(&fMgs[REC_FD].array))
		return 1;
	return 0;
}
int curFloderHasFile(void)
{
	if(bitArrayGetSum(&fMgs[curFolder].array))
		return 1;
	return 0;
}
void delAllVidJpgFile(void)
{
	bitArrayDeleteAll(&fMgs[curFolder].array);
}
int delCurFile(void)
{
	return bitArrayDeleteCur(&fMgs[curFolder].array);
}
int delDateFile(u32 fileType,u32 deleteData)
{
	u32 dir=getFileDir(fileType);
	if(dir>=FD_TYPE_MAX)
		return -1;
	return bitArrayDeleteByData(&fMgs[dir].array,deleteData);
}

int delDateFileByName(u32 fileType,char *name)
{
	int deleteData;
	int type;
	u32 dir=getFileDir(fileType);
	if((dir>=FD_TYPE_MAX) || (name == NULL))
		return -1;
	
	deleteData = fMgs[dir].name2d(name, &type);
	return bitArrayDeleteByData(&fMgs[dir].array,deleteData);
}

void lockCurFolderFile(void)
{
	if(curFolder!=REC_FD)
		return;
	bitArrayChangeType(&fMgs[curFolder].array,VID_TY_IDX,SOS_TY_IDX);
}
void unlockCurFolderFile(void)
{
	if(curFolder!=REC_FD)
		return;
	bitArrayChangeType(&fMgs[curFolder].array,SOS_TY_IDX,VID_TY_IDX);
}
int curFileIsLock(void)
{
	int type;
	if(bitArrayGetCurIdxName(&fMgs[REC_FD].array,&type))
	{
		if(type==SOS_TY_IDX)
			return 1;
	}
	return 0;
}
void setCurFileLock(u8 lock)
{
	if(curFolder!=REC_FD)
		return;
	if(lock)
		bitArraySetCurIdxType(&fMgs[curFolder].array,SOS_TY_IDX);
	else
		bitArraySetCurIdxType(&fMgs[curFolder].array,VID_TY_IDX);
}
int fileIsLock(int data)
{
	int type=bitArrayGetDataType(&fMgs[REC_FD].array,data);
	if(type==SOS_TY_IDX)
		return 1;
	return 0;
}
void setFileLock(int data,u8 lock)
{
	if(curFolder!=REC_FD||data<0)
		return;
	if(lock)
		bitArrayChangeDataType(&fMgs[curFolder].array,data,SOS_TY_IDX);
	else
		bitArrayChangeDataType(&fMgs[curFolder].array,data,VID_TY_IDX);
}
void saveFileInfor(u32 fileType)
{
	u32 fileDir=getFileDir(fileType);
	if(fileDir>=FD_TYPE_MAX)
	{
		printf("fileType err:%d\n",fileType);
		return -1;
	}
	bitArraySave(&fMgs[fileDir].array,fMgs[fileDir].arryFile);
}
void deleteHeadFile(void)
{
	bitArrayDeleteHead(&fMgs[REC_FD].array);
}
int needDeletFile(u32 fileType,u32 needMB)
{
	int cnt=0;
	u32 fileDir=getFileDir(fileType);
	if(fileDir>=FD_TYPE_MAX)
	{
		printf("fileType err:%d\n",fileType);
		return -1;
	}
	if((getSdFreeSize()>>11)<needMB)
	{
		do{
			if(bitArrayDeleteHead(&fMgs[fileDir].array)<0)
				cnt++;
			if(cnt>=3)
			{
				printf("floder[%s] delete file err\n",fMgs[fileDir].dir);
				break;
			}
		}while((getSdFreeSize()>>11)<needMB);
		printf("sd free size:%dMB\n",getSdFreeSize()>>11);
		return 1;
	}
	printf("sd free size:%dMB\n",getSdFreeSize()>>11);
	return 0;
}

char* getCurFileName(char *str,int len)
{
	int i;
	char *name=bitArrayGetCurIdxName(&fMgs[curFolder].array,NULL);
	int dirLen;
	if(REC_FD == curFolder)
		dirLen = VID_DIR_LEN;
	#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
	else if(AUD_FD == curFolder)
		dirLen = AUD_DIR_LEN;
	#endif
	if(name&&str&&len>12)
	{
		for(i=0;i<12;i++)
			str[i]=name[i+dirLen];
		str[i]=0;
	}
	if(name==NULL)
	{
		if(len>=5)
			memcpy(str,"none",5);
		return "none";
	}
	return name;
}
char *getFileByIdx(int idx,int *type)
{
	if(curFolder>=FD_TYPE_MAX)
		return NULL;
	return bitArrayGetIdxName(&fMgs[curFolder].array,idx,type);
}
char *getFileByData(int data,int *type)
{
	if(curFolder>=FD_TYPE_MAX)
		return NULL;

	return bitArrayGetDataName(&fMgs[curFolder].array,data,type);
}
char *getFileByIdxEx(int idx,int *pData,int *type)
{
	if(curFolder>=FD_TYPE_MAX)
		return NULL;
	return bitArrayGetIdxNameEx(&fMgs[curFolder].array,idx,pData,type);
}

int getCurFileIdx(u32 fileType)
{
	int Idx;
	int type;
	u32 dir=getFileDir(fileType);
	if(dir>=FD_TYPE_MAX)
		return -1;
	
	Idx = fMgs[dir].array.idx;
	return Idx;
}


#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
char *createAudName(int *nameNum)
{
	return bitArrayCreatName(&fMgs[AUD_FD].array,1,nameNum);
}
char *audData2Name(int data,int type)
{
	if(type!=1)
	{
		printf("type [%d] err\n",type);
		return NULL;
	}
	sprintf(fMgs[AUD_FD].fileName,"%s/%s%05d.%s",fMgs[AUD_FD].dir,fMgs[AUD_FD].preFix[type],data,fMgs[AUD_FD].sufFix[type]);
	return fMgs[AUD_FD].fileName;
}
int audName2Data(char *name,int* type)
{
	int i,d;
	if(name[0]==fMgs[AUD_FD].preFix[1][0]&&name[1]==fMgs[AUD_FD].preFix[1][1]&&name[2]==fMgs[AUD_FD].preFix[1][2])
	{
		if(type)
			*type=AUD_TY_IDX;
		goto GET_DATA;
	}
	return -1;
GET_DATA:
	d=0;
	for(i=3;i<8;i++)
	{
		d=d*10+name[i]-'0';
	}
	return d;
}
char *preWavName(void)
{
	return bitArrayGetPreIdxName(&fMgs[AUD_FD].array);
}
char *nextWavName(void)
{
	return bitArrayGetNextIdxName(&fMgs[AUD_FD].array);
}

#endif
int getFileSum(void)
{
	if(curFolder>=FD_TYPE_MAX)
		return 0;
	return bitArrayGetSum(&fMgs[curFolder].array);
}
int getMaxFileSum(void)
{
	return RECORD_FILE_SUM;
}
void fileScan(void)
{
	curFolder=REC_FD;
	fMgs[REC_FD].dir="1:/DCIM";
	fMgs[REC_FD].arryFile="1:/DCIM/bitarry.dat";
	fMgs[REC_FD].buff=recordFolderBuff;
	memset(recordFolderBuff,0,RECORD_FILE_LEN*4);
	fMgs[REC_FD].len=RECORD_FILE_LEN;
	fMgs[REC_FD].delMask=0xa;
	fMgs[REC_FD].unit=RECORD_FILE_UNIT;
	fMgs[REC_FD].d2name=recData2Name;
	fMgs[REC_FD].name2d=recName2Data;
	fMgs[REC_FD].preFix[VID_TY_IDX]="VID";
	fMgs[REC_FD].preFix[SOS_TY_IDX]="SOS";
	fMgs[REC_FD].preFix[JPG_TY_IDX]="PIC";
	fMgs[REC_FD].sufFix[VID_TY_IDX]="AVI";
	fMgs[REC_FD].sufFix[SOS_TY_IDX]="AVI";
	fMgs[REC_FD].sufFix[JPG_TY_IDX]="JPG";
	fMgInit(&fMgs[REC_FD],SUBTITLES_SUPPORT);
	
	#if SUPPORT_AUD_REC || SUPPORT_AUD_ENC
	fMgs[AUD_FD].dir="1:/AUDIO";
	fMgs[AUD_FD].arryFile="1:/AUDIO/bitarry.dat";
	fMgs[AUD_FD].buff=audFolderBuff;
	memset(audFolderBuff,0,AUD_FILE_LEN*4);
	fMgs[AUD_FD].len=AUD_FILE_LEN;
	fMgs[AUD_FD].delMask=0x2;
	fMgs[AUD_FD].unit=AUD_FILE_UNIT;
	fMgs[AUD_FD].d2name=audData2Name;
	fMgs[AUD_FD].name2d=audName2Data;
	fMgs[AUD_FD].preFix[1]="AUD";
	fMgs[AUD_FD].sufFix[1]="WAV";
	fMgInit(&fMgs[AUD_FD],0);
	#endif
	calFreeSpace();
}















