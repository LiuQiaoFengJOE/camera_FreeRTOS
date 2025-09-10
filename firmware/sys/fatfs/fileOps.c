#include "fatfs/fileOps.h"






static u32 clusterSize=64; //[unit: sectors]
static u32 sdFreeSize; // uint: KB
u32 calFreeSpace(void)
{
	FATFS *fs;
	FRESULT res;
	DWORD fre_clust, fre_sect, tot_sect;
	if(get_sd_status()==0)
	{
		sdFreeSize=0;
		return;
	}
	res = f_getfree("1:", &fre_clust, &fs);
	if (res)
	{
		printf("f_getfree res:%d\n",res);
		return;
	}
	clusterSize=fs->csize;
	fre_sect = fre_clust * fs->csize;
	sdFreeSize=fre_sect;
	printf("sd free size:%dMB,cluster:%d\n",fre_sect>>11,clusterSize);
	return fre_sect;
}
u32 getSdFreeSize(void)
{
	if(get_sd_status()==0)
		return 0;
	return sdFreeSize;
}
void sdFreeSizeAdj(u32 size,int add)
{
	u32 clusters;
	clusters=size/(clusterSize<<9);
	if(size%(clusterSize<<9))
		clusters++;
	size=clusters*clusterSize;
	if(add)
	{
		sdFreeSize+=size;
		printf("sd increace:%dMB,free:%dMB\n",size>>11,sdFreeSize>>11);
	}
	else
	{
		if(sdFreeSize>size)
			sdFreeSize-=size;
		else
			sdFreeSize=0;
		printf("sd decreace:%dMB,free:%dMB\n",size>>11,sdFreeSize>>11);
	}
}

























