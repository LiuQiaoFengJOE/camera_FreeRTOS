#ifndef  _BIT_ARRAY_H
#define  _BIT_ARRAY_H

#include "typedef.h"
#include "fatfs/ff.h"
#define MAX_NAME_LEN  100
typedef char *(*_data2Name)(int data,int type);
typedef int (*_name2Data)(char *name,int* type);

typedef struct
{
	u8 unit;  //bits
	u8 rshfit;
	u8 and;
	u8 mask;
	int allMask;
	int delMask;
	int len;
	u32 *array;
	//int delHead;
	int head;
	int tail;
	int headBak;
	int tailBak;
	int idx;
	int idxData;
	int sum;
	int subtitleSupport;
	char fileName[MAX_NAME_LEN];
	char fileName1[MAX_NAME_LEN];
	char fileName2[MAX_NAME_LEN];
	_data2Name d2name;
	_name2Data name2d;
}_bitArray;

void bitArrayInit(_bitArray *bitArray,u32 *array,int len,u8 unit,int delMask,_data2Name d2name,_name2Data name2d);
int bitArrayLoad(_bitArray *bitArray,char* name);
int bitArraySave(_bitArray *bitArray,char* name);
int bitArrayGetIdxData(_bitArray *bitArray,int idx,int *ptype);
int bitArrayGetDataType(_bitArray *bitArray,int data);
int bitArrayClearData(_bitArray *bitArray,int data);
int bitArrayAddName(_bitArray *bitArray,char *name);
char* bitArrayGetIdxName(_bitArray *bitArray,int idx,int *ptype);
char* bitArrayGetIdxNameEx(_bitArray *bitArray,int idx,int *pdata,int *ptype);
char* bitArrayGetDataName(_bitArray *bitArray,int data,int *ptype);
char* bitArrayGetCurIdxName(_bitArray *bitArray,int *ptype);
int bitArrayGetSum(_bitArray *bitArray);
char* bitArrayGetPreIdxName(_bitArray *bitArray);
char* bitArrayGetNextIdxName(_bitArray *bitArray);
char* bitArrayGetPreIdxNameByType(_bitArray *bitArray, int type);
char* bitArrayGetNextIdxNameByType(_bitArray *bitArray, int type);
int bitArrayDeleteHead(_bitArray *bitArray);
int bitArrayDeleteByData(_bitArray *bitArray,int deleteData);
char* bitArrayCreatName(_bitArray *bitArray,int type,int *nameNum);
int bitArrayDeleteAll(_bitArray *bitArray);
int bitArrayDeleteCur(_bitArray *bitArray);
int bitArrayChangeType(_bitArray *bitArray,int oldType,int newType);
int bitArraySetCurIdxType(_bitArray *bitArray,int type);
int bitArrayChangeDataType(_bitArray *bitArray,int data,int type);
void bitArraySubtitleEn(_bitArray *bitArray,int en);
char* bitArrayGetLastIdxName(_bitArray *bitArray,int *ptype);







#endif
