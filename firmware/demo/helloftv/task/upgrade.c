#include "task/uiWin.h"
#include "fatfs/ff.h"
#include "systemSetting/sysCommon.h"

#define UPGRADE_VERIFY          1
#define UPGRADE_DELETE          0   // delete update file
#define UPGRADE_RESOURCE       1   // update resource when upgrade end

#define UPGRADE_BUFF_SIZE   (64*1024)
winHandle progressBar;
winHandle strHandle;
extern u8 spiWriteNoErase(u32 flash_addr, u8 *data_buf, u32 data_len);
void upgrade_uiProgress(uint32 progress)
{
    ax32xx_wdtClear();
	winSetPorgressRate(progressBar,progress);
	drawUIService(false);
}
void resReinit(void)
{
	resfs_fini();
	resfs_init(_SYSDSK_ "res.bin");
    fontInit(); //reinitial resource
    iconInit(); //reinitial resource 
	configLanguage();
}
int sdUpgrade(void)
{
	void* upgradeFp;
	int ret,progress,i,res;
	u32 upgradeFileSize,addr,readSize,realSize;
	INT8U *buffer=NULL;
	INT8U *buffSave;
	strHandle=INVALID_HANDLE;
	progressBar=INVALID_HANDLE;

	upgradeFp=osal_fopen(UPDATE_BIN, "rb");
	if(upgradeFp==NULL)
	{
		return -1;
	}
    printf("start upgrade\n");

	upgradeFileSize = osal_fsize(upgradeFp);
//-------get cache memory
	ret = -1;
	buffer = (INT8U *)osal_malloc(UPGRADE_BUFF_SIZE+64);//shareMemMalloc(4096);
    if(buffer == NULL)
    {
    	printf("malloc err\n");
		return -1;
	}
	buffSave=buffer;
	buffer=((u32)buffer+63)&(~63);
	lcd_bk_ctrl(1);
    printf("upgrade : start.>>Please do not power off.\n");
	winSetfgColor(rectCreate(R1x(0),R1y(0),R1w(320),R1h(240),INVALID_HANDLE,WIN_ABS_POS,0), R_COLOR_GRAY);
	progressBar=progressBarCreate(R1x(70),R1y(110),R1w(180),R1h(6),INVALID_HANDLE,WIN_ABS_POS,0);
	winSetfgColor(progressBar, R_COLOR_BLUE);
	winSetbgColor(progressBar, R_COLOR_GRAY3);
	strHandle=stringIconCreate(R1x(0),R1y(125),R1w(320),R1h(35),INVALID_HANDLE,WIN_ABS_POS,0);
	winSetStrInfor(strHandle,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_BLUE);
	winSetResid(strHandle,R_ID_STR_UPDATE_START);
	drawUIService(false);
	ax32xx_wdtEnable(0);
	resfs_fini();
    printf("upgrade : write\n"); 
	addr = 0;
	i = addr;
	ret = 0;
	progress = 0;
	osal_fseek(upgradeFp,addr);
    while(1)
    {
    	if(upgradeFileSize-addr>UPGRADE_BUFF_SIZE)
			readSize=UPGRADE_BUFF_SIZE;
		else
			readSize=upgradeFileSize-addr;
		if(readSize==0)
			break;
		realSize = osal_fread(buffer,readSize,1,upgradeFp);
		if(realSize!=readSize)
		{
			if(addr==0)
			{
				osal_free(buffSave);
				printf("first read err\n");
				return -1;
			}
			ret = -2;
			goto UPGRADE_END;
		}
		if(addr==0)
		{
			memset(buffer,0,512);
		}
		if(spi_write(addr,buffer,realSize)<0)
		{
			ret = -2;
			goto UPGRADE_END;
		}

		i+=realSize;
		addr+=realSize;
		if(((i*100)/upgradeFileSize)!=progress)
		{
			progress = (i*100)/upgradeFileSize;
			printf("\rupgrade : %d%",progress);
		#if UPGRADE_VERIFY	
			upgrade_uiProgress(progress>>1);
		#else
		    upgrade_uiProgress(progress);
		#endif
		}	
    }
	osal_fseek(upgradeFp,0);
	osal_fread(buffer,4096,1,upgradeFp);
	spi_write(0,buffer,4096);
	//spiWriteNoErase(0,upgradeHeader,512);
	printf("\n");
//-----verify
#if UPGRADE_VERIFY > 0
    int j;
	printf("upgrade : verify.\n");
    i = 0;
	addr = 0;
	progress = 0;
	osal_fseek(upgradeFp,0);
	while(1)
	{
		if(upgradeFileSize-addr>UPGRADE_BUFF_SIZE/2)
			readSize=UPGRADE_BUFF_SIZE/2;
		else
			readSize=upgradeFileSize-addr;
		if(readSize==0)
			break;
		
		realSize = osal_fread(buffer,readSize,1,upgradeFp);
		if(realSize!=readSize)
		{
			ret = -3;
			printf("read err\n");
			goto UPGRADE_END;
		}
		if(spi_read(addr,(buffer+realSize),realSize)<0)
		{
			ret = -3;
			goto UPGRADE_END;
		}
		for(j=0;j<realSize;j++)
		{
			if(buffer[j] != buffer[realSize+j])
			{
				ret = -3;
				printf("verify err\n");
				goto UPGRADE_END;
			}
		}
		addr+=realSize;
		i+=realSize;

		if(((i*100)/upgradeFileSize)!=progress)
		{
			progress = (i*100)/upgradeFileSize;
			printf("\rupgrade : %d%",progress);
		#if UPGRADE_VERIFY	
			upgrade_uiProgress(50+(progress>>1));
		#endif
		}
	}
#endif
    ret = 0;
UPGRADE_END:
	if(ret<0)
	{
		memset(buffer,0,512);
		spi_write(0,buffer,512);
	}
	close(upgradeFp);
	if(buffSave)
	    osal_free(buffSave);
	
#if UPGRADE_DELETE
		f_unlink(UPDATE_BIN);
		printf("upgrade : delete file ->%s!\n",UPDATE_BIN);
#endif

#if UPGRADE_RESOURCE
	if (ret = f_mount(getFatFs(0), _SYSDSK_, 1))
	{
		p_err("mount err %d\r\n", ret);
		return 1;
	}
    resfs_init(_SYSDSK_ "res.bin");
    fontInit(); //reinitial resource
    iconInit(); //reinitial resource 
	configLanguage();
#endif	
	if(ret<0)
	{
		printf("upgrade : fail!\n");
		winSetStrInfor(strHandle,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_RED);
		winSetResid(strHandle,(u32)"upgrade failed!!!");
	}
    else
    {
		printf("upgrade : success!\n");
		winSetResid(strHandle,R_ID_STR_UPDATE_END);
		//taskStart(TASK_POWER_OFF,0);
    }
	drawUIService(false);
    hal_rtcUninit();
	printf("upgrade : power off!\n");
	osal_task_sleep(1000);
	lcd_bk_ctrl(0);
	power_off2();
	return 0;
}





