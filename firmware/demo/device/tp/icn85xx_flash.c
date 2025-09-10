#include "icn85xx.h"
#include "icn85xx_flash.h"
#include "tpiic.h"
//#include "../../ax32_platform_demo/application.h"


#ifdef ICN87XX
static DT_UCHAR_P fw_buf;
static DT_UCHAR fw_mode = 0;
static DT_UINT fw_size = 0;
DT_USHORT const Crc16Table[]= {
    0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022, 
    0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041, 
    0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1, 
    0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082, 
    0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1, 
    0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2, 
    0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162, 
    0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101, 
    0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321, 
    0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342, 
    0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2, 
    0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381, 
    0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2, 
    0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1, 
    0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261, 
    0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202, 
};
#endif

extern void delay_2ms(int cnt);

static u8 icn85xx_prog_write(u32 addr, u8 *buf, u32 len)
{
    #if 0
    int i;
    for (i = 0;i < len; i++){
        if (!_touch_prog_write(ICN85XX_PROG_WADDR, addr+i, buf+i, 1)){
            return FALSE;
        }
    }
    return TRUE;
    #else
    return _touch_prog_write(ICN85XX_PROG_WADDR, addr, buf, len);
    #endif
}

static u8 icn85xx_prog_read(u32 addr, u8 *buf, u32 len)
{
    #if 1
    int i;
    for (i = 0;i < len; i++){
        if (!_touch_prog_read(ICN85XX_PROG_WADDR, ICN85XX_PROG_RADDR, addr+i, buf+i, 1)){
            return FALSE;
        }
    }
    return TRUE;
    #else
    return _touch_prog_read(ICN85XX_PROG_WADDR, ICN85XX_PROG_RADDR, addr, buf, len);
    #endif
}
static u8 icn85xx_prog_write_16bit(u32 addr, u8 *buf, u32 len)
{
    #if 0
    int i;
    for (i = 0;i < len; i++){
        if (!_touch_prog_write(ICN85XX_PROG_WADDR, addr+i, buf+i, 1)){
            return FALSE;
        }
    }
    return TRUE;
    #else
    return _touch_panel_write(ICN85XX_PROG_WADDR, addr, buf, len);
    #endif
}
static u8 icn85xx_prog_read_16bit(u32 addr, u8 *buf, u32 len)
{
    #if 1
    int i;
    for (i = 0;i < len; i++){
        if (!_touch_panel_read(ICN85XX_PROG_WADDR, ICN85XX_PROG_RADDR, addr+i, buf+i, 1)){
            return FALSE;
        }
    }
    return TRUE;
    #else
    return _touch_prog_read(ICN85XX_PROG_WADDR, ICN85XX_PROG_RADDR, addr, buf, len);
    #endif
}


#if ICN85XX_FW_CRC_ON
static unsigned int icn85xx_crc_calc(unsigned crc_in, char *buf, int len)
{
    int i;
    unsigned int crc = crc_in;
    for(i = 0; i < len; i++)
        crc = (crc << 8) ^ crc32table[((crc >> 24) ^ *buf++) & 0xFF];
    return crc;
}
#endif

#if ICN85XX_FW_CRC_ON
static int  icn85xx_crc_enable(unsigned char enable)
{
    unsigned char ucTemp;
    int ret = 0;
    if(enable==1)
    {
        ucTemp = 1;
        ret = icn85xx_prog_write(0x40028, &ucTemp, 1 );
    }
    else if(enable==0)
    {
        ucTemp = 0;
        ret = icn85xx_prog_write(0x40028, &ucTemp, 1 );
    }
    printf("enable = %d,ret = %d\n",enable,ret);
    return ret;
}
static int  icn85xx_crc_check(unsigned int crc, unsigned int len)
{
//    int ret;
    unsigned int crc_len;
    unsigned int crc_result;
    unsigned char ucTemp[4] = {0,0,0,0};

    /*ret=*/ icn85xx_prog_read(0x4002c, ucTemp, 4 );
    crc_result = ucTemp[3]<<24 | ucTemp[2]<<16 | ucTemp[1] << 8 | ucTemp[0];

    /*ret =*/ icn85xx_prog_read(0x40034, ucTemp, 2);
    crc_len = ucTemp[1] << 8 | ucTemp[0];

    if((crc_result == crc) && (crc_len == len)){
        printf("crc_fw is: 0x%x\n", crc);
        printf("ok crc_result: 0x%x\n", crc_result);
        printf("ok crc_len: %d\n", crc_len);
        return 1;
    }else{
        printf("crc_fw is: 0x%x\n", crc);
        printf("err crc_result: 0x%x\n", crc_result);
        printf("err crc_len: %d\n", crc_len);
        return 0;
    }

}
#endif

static int  icn85xx_bootfrom_sram(void)
{
    int ret = -1;
    unsigned char ucTemp = 0x03;
    unsigned long addr = 0x40400;

    //change bootmode from sram
    ret = icn85xx_prog_write(addr, &ucTemp, 1 );
    if (ret){
        printf("icn85xx_bootfrom_sram ok\n");
    }else{
        printf("icn85xx_bootfrom_sram err\n");
    }
    return ret;
}

static u8 icn85xx_goto_progmode(void)
{
    u8 retry = 0;

    while (retry++ < 3) {
        u8 ucTemp = 0x5a;
        if(icn85xx_prog_write(0xcc3355, &ucTemp, 1)){
            //printf("prog write ok\n");
            break;
        }
        printf("prog write err retry = %d\n",retry);
    }
    printf("goto progmode ok\n");
    return 1;
}

static int  icn85xx_read_flashid(void)
{
    unsigned char ucTemp[4] = {0,0,0,0};
    int flashid=0;

    ucTemp[2]=0x08;
    ucTemp[1]=0x10;
    ucTemp[0]=0x00;
    icn85xx_prog_write(0x4062c,ucTemp,3);

    ucTemp[0]=0x9f;

    icn85xx_prog_write(0x40630,ucTemp,1);

    ucTemp[1] = 0x00;
    ucTemp[0] = 0x03;

    icn85xx_prog_write(0x40640,ucTemp,2);

    ucTemp[0]=1;
    icn85xx_prog_write(0x40644,ucTemp,1);
    while(ucTemp[0])
    {
        icn85xx_prog_read(0x40644,ucTemp,1);
    }

    icn85xx_prog_read(0x40648,(u8 *)&flashid,4);
    flashid=flashid&0x00ffffff;
    if((MD25D40_ID1 == flashid) || (MD25D40_ID2 == flashid)
        ||(MD25D20_ID1 == flashid) || (MD25D20_ID2 == flashid)
        ||(GD25Q10_ID == flashid) || (MX25L512E_ID == flashid)
        || (MD25D05_ID == flashid)|| (MD25D10_ID == flashid))
    {
        icn85xx_prog_read(0x040000, ucTemp, 3);
        if((ucTemp[2] == 0x85) && (ucTemp[1] == 0x05)){
            printf("icntype = ICN85XX_WITH_FLASH_85\n");
            icntype = ICN85XX_WITH_FLASH_85;
        }
        else if((ucTemp[2] == 0x85) && (ucTemp[1] == 0x0e)){
            printf("icntype = ICN85XX_WITH_FLASH_86\n");
            icntype = ICN85XX_WITH_FLASH_86;
        }
		else if((ucTemp[2] == 0x87) && (ucTemp[1] == 0x0e)){
            printf("icntype = ICN85XX_WITH_FLASH_86\n");
            icntype = ICN85XX_WITHOUT_FLASH_87;
        }
		printf("===========%d,%d \n",ucTemp[2],ucTemp[1]);
    }else{
        printf("icntype = ICN85XX_WITHOUT_FLASH\n");
        printf("\r\nflashid = %x - %x\n",ucTemp[2],ucTemp[1]);
		
        icntype = ICN85XX_WITHOUT_FLASH;
    }

    //printf("flashid: 0x%x\n", flashid);
    return flashid;
}

int  icn85xx_fw_update(void)
{
    int file_size/*, last_length*/;
//    int j, num;
//    unsigned int crc_fw = 0;
    
    printf("icn85xx_fw_update start\n");
    
    file_size = sizeof(icn85xx_fw);  
    if (!file_size) return 0;
    
    if (!icn85xx_goto_progmode()){
        printf("goto progmode failed\n");
        return 0;
    }

    delay_2ms(1);
    #if ICN85XX_FW_CRC_ON
    icn85xx_crc_enable(1);
    crc_fw = icn85xx_crc_calc(crc_fw, icn85xx_fw, file_size);
    #endif
    
    if(!icn85xx_prog_write(0,(u8 *)icn85xx_fw,file_size)){
        printf("write icn85xx fw failed\n");
        return 0;
    }

    #if ICN85XX_FW_CRC_ON
    icn85xx_crc_enable(0);
    if(!icn85xx_crc_check(crc_fw, file_size)){
        printf("icn85xx fw crc check err\n");
        return 0;
    }
    #endif
    
    if(!icn85xx_bootfrom_sram()){
        printf("boot from sram failed\n");
        return 0;
    }
    delay_2ms(50);
    printf("icn85xx_fw_update successed\n");
    return 1;
}
#ifdef ICN87XX
DT_USHORT icn85xx_readVersion(void)
{
    DT_SSHORT err = 0;
    DT_UCHAR tmp[2];    
    DT_USHORT CurVersion;

    err = icn85xx_prog_read(0x000c, tmp, 2);
    if (err < 0) {
        printf("icn85xx_readVersion failed: %d\r\n", err); 
        return 0;
    }       
    CurVersion = (tmp[0]<<8) | tmp[1];

    return CurVersion;  
}

static DT_SSHORT icn87xx_read_flashid(void){

	u16 ret = 0;
    int flashid;
	u8 ucTemp[4];
	u8 count=0;

    ucTemp[0] = FLASH_CMD_READ_IDENTIFICATION;
    ret = icn85xx_prog_write(CMD_SEL_87, ucTemp,1);
    if(ret <= 0)
    {
        return 0;
    }
	ucTemp[1] = (u8)(SRAM_EXCHANGE_ADDR1 >> 8);
    ucTemp[0] = (u8)SRAM_EXCHANGE_ADDR1;
    ret = icn85xx_prog_write(SRAM_ADDR_87, ucTemp,2);
    if(ret <= 0)
    {
        return ret; 
    }
	ucTemp[0] = 1;
    ret = icn85xx_prog_write(START_DEXC_87, ucTemp,1);
	if(ret <= 0)
    {
        return ret; 
    }

	while(ucTemp[0])
	{
		ret = icn85xx_prog_read(SF_BUSY_87,&ucTemp[0],1);
		if(ret <= 0)
	    {
	        return ret; 
	    }
		printf("%s,%d,ucTemp[0] = %d\n",__func__,__LINE__,ucTemp[0]);
		if(count++>3)
			break;
	}
	ret = icn85xx_prog_read(SRAM_EXCHANGE_ADDR1, ucTemp,4);
	if(ret <= 0)
    {
        return ret; 
    }
	printf("%s,%d,ucTemp0,1,2,3 = %d,%d,%d,%d,\n",__func__,__LINE__,ucTemp[0],ucTemp[1],ucTemp[2],ucTemp[3]);
	flashid = (((ucTemp[0])<<16) + ((ucTemp[1])<<8) + ucTemp[2]);//correct
	printf("%s,%d,flash_id = %d\n",__func__,__LINE__,flashid);
	if((MD25D40_ID1 == flashid) || (MD25D40_ID2 == flashid)
	||(MD25D20_ID1 == flashid) || (MD25D20_ID2 == flashid)
	||(GD25Q10_ID == flashid) || (MX25L512E_ID == flashid)
	|| (MD25D05_ID == flashid)|| (MD25D10_ID == flashid))
	{
		printf("icntype = ICN85XX_WITH_FLASH_87\n");
        icntype = ICN85XX_WITH_FLASH_87;
	}
	else
	{
		printf("icntype = ICN85XX_WITHOUT_FLASH_87\n");
        icntype = ICN85XX_WITHOUT_FLASH_87;
	}
	
    return 1;
}
DT_SSHORT icn87xx_boot_sram(void)
{
    DT_SSHORT ret = -1;

    DT_UCHAR ucTemp = 0x03;
    ret = icn85xx_prog_write_16bit(0xf400,&ucTemp,1);
    return ret;
}
DT_USHORT icn87xx_calculate_crc(DT_USHORT len)
{
    DT_UCHAR ucTemp[4];
    DT_SSHORT ret = -1;
    DT_USHORT crc = 0;

	// 2.1
    ucTemp[0] = 0x00;
    ucTemp[1] = 0x00;
    ret = icn85xx_prog_write_16bit(SRAM_ADDR_87, ucTemp,2);
    if(ret <= 0)
    {
        return ret;
    }
    // 2.2
    ucTemp[0] = len & 0xff;
    ucTemp[1] = (len >> 8 )& 0xff;
    ret = icn85xx_prog_write_16bit(DATA_LENGTH_87, ucTemp,2);
    if(ret <= 0)
    {
        return ret;
    }
    // 2.3
    ucTemp[0] = 0x01;
    ret = icn85xx_prog_write_16bit(SW_CRC_START_87, ucTemp,1);
    if(ret <= 0)
    {
        return ret;
    }

    // 2.4
    while(ucTemp[0])
    {
        ret = icn85xx_prog_read_16bit(SF_BUSY_87, ucTemp,1);
        if(ret <= 0)
        {
            return ret;
        }
    }
    ret = icn85xx_prog_read_16bit(CRC_RESULT_87, ucTemp,2);
    if(ret <= 0)
    {
        return ret;
    }
    crc = (((ucTemp[1])<<8) + (ucTemp[0]))&0x0000ffff;

    return crc;
}


DT_SSHORT  icn87xx_fw_download(DT_ULONG offset, DT_UCHAR_P  buffer, DT_UINT size)
{     
	DT_UINT i = 0;   
	DT_UINT j = 0;   
	DT_UCHAR testb[B_SIZE];

//	TP_Debug("<8752.%d",offset);
	//printf("icn87xx_fw_download offset=%d,size=%d  \n",offset,size);
	icn85xx_prog_write_16bit(offset,buffer,size);   
    icn85xx_prog_read_16bit(offset,testb,5); 
	//printf("icn87xx_fw_download over");

	//hal_wdtClear();

    for(i = 0; i < 5; i++)
    {
    	
        if(buffer[i] != testb[i])
        {
			printf("buffer:  ");
			for(j = 0; j < 5; j++)
				printf("%x ,",buffer[j]);
			printf("\ntestb:  ");
			for(j = 0; j < 5; j++)
				printf("%x ,",testb[j]);
			printf("\n");
            return -1;
        }  
    }

    return 0;   
}
DT_USHORT icn87xx_fw_Crc(DT_USHORT crc,DT_UCHAR_P buf ,DT_USHORT size)
{
    DT_USHORT u16CrcValue = crc;
    DT_USHORT u16Length = size;
    DT_UCHAR  u8CheckData = 0;

    while(u16Length)
    {
        u8CheckData = *buf++;
        u16CrcValue = (u16CrcValue << 8)^Crc16Table[(u16CrcValue >> 8)^((u8CheckData)&0xff)];
        u16Length--;
    }
	
    return u16CrcValue;
}
DT_SSHORT  icn85xx_read_fw(DT_USHORT offset, DT_UINT length, DT_UCHAR_P buf)
{

//    DT_USHORT   pos = offset; 
	
    if(fw_mode == 1)
    {
        memcpy(buf, fw_buf+offset, length);
    }
    else
    {                   
        //vfs_read(fp, buf, length, &pos); 
    }

    return 0;       
}

DT_SSHORT icn87xx_read_fw_info(DT_UCHAR_P fw, DT_UCHAR_P buffer, DT_USHORT u16Addr, DT_UINT u8Length)
{
//    DT_SSHORT file_size;
	printf("icn87xx_read_fw_info\n");

	fw_mode = 1; 
	fw_size = sizeof(icn87xx_fw);
	fw_buf = (DT_UCHAR_P)&icn87xx_fw[0];
//	file_size = fw_size;  
	if(fw_mode == 1)
    {
        memcpy(buffer, fw_buf+u16Addr, u8Length);
    }
    return 1; 
}
int icn87xx_fw_write(void)
{
	DT_UINT file_size, last_length;
//    DT_SSHORT ret = -1;
    DT_UINT num;
	DT_ULONG j;
    DT_UCHAR temp_buf[B_SIZE];
    DT_USHORT crc_fw = 0;
    DT_USHORT sram_len = 0; 
//    DT_USHORT sram_crc = 0;
//    DT_USHORT temp_crc = 0;
//    DT_USHORT fw_version = 0;
    DT_UCHAR retry;
	DT_SCHAR firmware[128] =  {"icn85xx_firmware"};

	/*ret =*/ icn87xx_read_fw_info((DT_UCHAR_P)firmware, (DT_UCHAR_P)temp_buf, FIRMWARA_INFO_AT_BIN_ADDR,16); //contain sram lenth ,fwversion
    sram_len = (((temp_buf[9])<<8) + temp_buf[8]) + (((temp_buf[6])<<16) + ((temp_buf[5])<<8) + temp_buf[4]) - (((temp_buf[2])<<16) + ((temp_buf[1])<<8) + temp_buf[0]);
//    fw_version = (((temp_buf[13])<<8)+ temp_buf[12]);
    file_size = fw_size;
	printf("sram_len:%d  ,file_size:%d \r\n",sram_len,file_size);
	
    if (!icn85xx_goto_progmode()){
        printf("goto progmode failed\n");
        return 0;
    }
	if(sram_len == file_size)
	{
		num = file_size/B_SIZE;
		crc_fw = 0;
		for(j=0; j < num; j++)
		{
            icn85xx_read_fw(j*B_SIZE, B_SIZE, temp_buf);        
            crc_fw = icn87xx_fw_Crc(crc_fw, temp_buf, B_SIZE);
			for(retry=0;retry<3;retry++)
			{
		        if(icn87xx_fw_download(j*B_SIZE, temp_buf, B_SIZE) != 0)
		        {
		            printf("87download error j:%d\r\n",j);
		        }   
				else
				{
					break;
				}
			}
			if(retry>=3)
			{
				     printf("87 download fail retry:%d\r\n",retry);
			         return 0;
			}
			delay_2ms(1);
        }
		last_length = file_size - B_SIZE*num;
		if(last_length > 0)
		{
			printf("start write rest icn85xx_fw array!!!\n");
            icn85xx_read_fw(j*B_SIZE, last_length, temp_buf);     
            crc_fw = icn87xx_fw_Crc(crc_fw, temp_buf, last_length);
			for(retry=0;retry<3;retry++)
			{
		        if(icn87xx_fw_download(j*B_SIZE, temp_buf, last_length) != 0)
		        {
		            printf("87 download error j:%d\r\n",j);
		        }   
				else
				{
					break;
				}
			}
			if(retry>=3)
			{
				printf("download fail retry:%d\r\n",retry);
			    return 0;
			}
			else
				printf("write icn85xx_fw array successed\n");
			delay_2ms(1);
        }
		for(retry=0;retry<3;retry++)
		{
			if( icn87xx_calculate_crc(sram_len) != crc_fw)
	        {
	            printf("icn87xx_crc_check error retry=%d\r\n",retry);
	        }   
			else
			{
//				sram_crc =  crc_fw;
				break;
			}
		}
		if(retry>=3)
		{
            printf("read flash data error, crc error\r\n");
            return 0;
		}
	}
	if(icn87xx_boot_sram() <= 0)
    {
        printf("icn85xx_bootfrom_sram error\r\n");
        return 0;
    } 
	delay_2ms(10);
    printf("icn87xx_fw_update successed\n");
	
	return 1;
}
int  icn87xx_fw_update(void)
{
//    DT_SSHORT ret = -1;
    DT_UCHAR retry;
	
	for(retry=0; retry<3; retry++){
		if(icn87xx_fw_write()){
			printf("icn87xx_fw_update success.\n");
			break;
		}
	}

	return 1;
}
#endif
int icn85xx_flash_check(void){

    if(!icn85xx_goto_progmode()){
        return 0;
    }
    {
        u8 buf[3];
        buf[0] = buf[1] = buf[2] = 0x0;
        if(!icn85xx_prog_write(0x040000, buf, 3)){
            printf("0x040000 write err\n");
            return 0;
        }

        if(!icn85xx_prog_read(0x040000, buf, 3)){
            printf("0x040000 read err\n");
            return 0;
        }

        printf("read 0x040000 ,buf[2] = 0x%x,buf[1] =0x%x\n" ,buf[2],buf[1]);
        if((buf[2] == 0x85) && ((buf[1] == 0x05) || (buf[1] == 0x0e))){
            icn85xx_read_flashid();
        }
		else{//for icn87
			//#ifdef ICN87XX
			if(!icn85xx_prog_read_16bit(0xf001, buf, 2)){
				return 0;
			}
			printf("read 0xf001, icnt87 flashid: (0x%x, 0x%x) 0x%x\r\n",buf[0],buf[1],buf[2]);
			if(buf[1] == 0x87){
				icn87xx_read_flashid();
				return 1;
			}
			//#endif
//			DT_USHORT curVersion = 0;
//			curVersion = icn85xx_readVersion();
//			printf("read 0x000c ,current version: 0x%x\r\n", curVersion);
            return 0;
        }
        
    }
    return 1;
}
