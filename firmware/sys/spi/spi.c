#include "debug.h"
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"

#include "spi.h"

extern uint8_t flash_running;

void spi_setCSStatus(int level)
{
	spi0_chip_sel(level);
}

void spi_SendByte(u8 u8Data)
{
	spi0_send_byte(u8Data);
}

u8 u8spi_ReceiveByte(void)
{
	return spi0_receive_byte();
}

void spi_SFSendAddr(DWORD dwFlashAddr)
{
	spi_SendByte((dwFlashAddr >> 16) & 0xff);
	spi_SendByte((dwFlashAddr >> 8) & 0xff);
	spi_SendByte((dwFlashAddr)&0xff);
}

bool spi_SFWaitStatusOK()
{
	int iTOCnt = 0;
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x05);
	while (u8spi_ReceiveByte() & 1)
	{
		if (iTOCnt++ > 8000 * 2000) // 2s
		{
			spi_setCSStatus(1); // CS HIGH
			return FALSE;
		}
	}
	spi_setCSStatus(1); // CS HIGH
	return TRUE;
}

void spi_SFWriteEnable()
{
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x06);
	spi_setCSStatus(1); // CS HIGH
}

void spi_SFWriteDisable()
{
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x04);
	spi_setCSStatus(1); // CS HIGH
}

bool spi_SFEraseSector(DWORD dwSectorAddr)
{
	spi_SFWriteEnable();
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x20);
	spi_SendByte((dwSectorAddr >> 16));
	spi_SendByte((dwSectorAddr >> 8));
	spi_SendByte((dwSectorAddr >> 0));
	spi_setCSStatus(1); // CS HIGH
	if (!spi_SFWaitStatusOK())
	{
		printf("%s fail\r\n", __FUNCTION__);
		return FALSE;
	}
	// printf("%s success\r\n",__FUNCTION__);
	return TRUE;
}

bool spi_SFEraseSector_64K(DWORD dwSectorAddr)
{
	spi_SFWriteEnable();
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0xd8);
	spi_SendByte((dwSectorAddr >> 16));
	spi_SendByte((dwSectorAddr >> 8));
	spi_SendByte((dwSectorAddr >> 0));
	spi_setCSStatus(1); // CS HIGH
	if (!spi_SFWaitStatusOK())
	{
		printf("%s fail\r\n", __FUNCTION__);
		return FALSE;
	}
	// printf("%s success\r\n",__FUNCTION__);
	return TRUE;
}

void spi_SFReadData(void *pDataBuf, DWORD dwFlashAddr, u32 u32DataLen)
{
	u8 *pSFReadBuf = (u8 *)pDataBuf;

	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x03);
	spi_SFSendAddr(dwFlashAddr);
	while (u32DataLen--)
	{
		*pSFReadBuf++ = u8spi_ReceiveByte();
	}
	spi_setCSStatus(1); // CS HIGH
}

static bool spi_SFWriteData(DWORD dwFlashAddr, void *pDataBuf, u32 u32DataLen)
{
	u8 *pSFWriteBuff = (u8 *)pDataBuf;

	spi_SFWriteEnable();
	spi_setCSStatus(0); // CS LOW
	spi_SendByte(0x02);
	spi_SendByte((dwFlashAddr >> 16));
	spi_SendByte((dwFlashAddr >> 8));
	spi_SendByte((dwFlashAddr >> 0));

	while (u32DataLen--)
	{
		spi_SendByte(*pSFWriteBuff++);
	}
	spi_setCSStatus(1); // CS HIGH

	if (!spi_SFWaitStatusOK())
	{
		return FALSE;
	}

	return TRUE;
}
u8 spiWriteNoErase(u32 flash_addr, u8 *data_buf, u32 data_len)
{
	int i, write_num = data_len / 256, remain_data = data_len % 256, erase_num = 0;
	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;
	for (i = 0; i < write_num; i++)
	{
		spi_SFWriteData(flash_addr + i * 256, &data_buf[i * 256], 256);
	}
	if(remain_data)
		spi_SFWriteData(flash_addr + write_num * 256, &data_buf[write_num * 256], remain_data);
	flash_running = 0;
	return 1;
}
u8 spi_write(u32 flash_addr, u8 *data_buf, u32 data_len)
{

	int i, write_num = data_len / 256, remain_data = data_len % 256, erase_num = 0;
	if (data_len % 0x1000 == 0)
	{

		erase_num = data_len / 0x1000;
	}
	else
	{

		erase_num = data_len / 0x1000 + 1;
	}

	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;

	for (i = 0; i < erase_num; i++)
	{
		spi_SFEraseSector(flash_addr + i * 0x1000); // erase 4k
	}
	for (i = 0; i < write_num; i++)
	{

		spi_SFWriteData(flash_addr + i * 256, &data_buf[i * 256], 256);
	}
	if(remain_data)
		spi_SFWriteData(flash_addr + write_num * 256, &data_buf[write_num * 256], remain_data);
	flash_running = 0;

	return 1;
}

u8 spi_write_64K(u32 flash_addr, u8 *data_buf, u32 data_len)
{
	u32 percent,lastPercent=1000;
	int i, write_num = data_len / 256, remain_data = data_len % 256, erase_num = 0;
	if (data_len % 0x10000 == 0)
	{

		erase_num = data_len / 0x10000;
	}
	else
	{

		erase_num = data_len / 0x10000 + 1;
	}

	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;

	for (i = 0; i < erase_num; i++)
	{
		spi_SFEraseSector_64K(flash_addr + i * 0x10000); // erase 4k
	}
	for (i = 0; i < write_num; i++)
	{
		percent=(i+1)*100/write_num;
		if(percent!=lastPercent)
		{
			lastPercent=percent;
			upgradeUpdatePercent(percent);
		}
		spi_SFWriteData(flash_addr + i * 256, &data_buf[i * 256], 256);
	}
	if (remain_data)
	{
		upgradeUpdatePercent(100);
		spi_SFWriteData(flash_addr + write_num * 256, &data_buf[write_num * 256], remain_data);
	}
	flash_running = 0;

	return 1;
}

u8 spi_write_data(u32 flash_addr, u8 *data_buf, struct spidata *data)
{
	u32 data_len = data->datalen;
	u8 *write_buf = data_buf;
	u8 res = 0;
	if (flash_addr < FLASH_START || (flash_addr + data_len + sizeof(struct spidata)) > FLASH_END)
	{
		printf("spi_write over border!\r\n");
		return 0;
	}
	uint8_t *data_len_buf = osal_malloc(sizeof(struct spidata));
	memcpy(data_len_buf, data, sizeof(struct spidata));

	data_len += sizeof(struct spidata); // 总共需要写入的数据量
	int i, write_num = (data_len) / 256, remain_data = (data_len) % 256, erase_num = 0;
	int first_write;

	// 需要擦除的扇区数量
	if (data_len % 0x1000 == 0)
	{

		erase_num = data_len / 0x1000;
	}
	else
	{

		erase_num = data_len / 0x1000 + 1;
	}

	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;

	// 擦除扇区
	for (i = 0; i < erase_num; i++)
	{
		spi_SFEraseSector(flash_addr + i * 0x1000); // erase 4k
	}

	// 第一扇区写入的实际数据数量
	if (data_len > 256)
	{
		first_write = 256 - sizeof(struct spidata);
	}

	spi_SFWriteData(flash_addr, data_len_buf, sizeof(struct spidata));
	osal_free(data_len_buf);
	flash_addr += sizeof(struct spidata);

	spi_SFWriteData(flash_addr, write_buf, first_write);
	write_buf += first_write;
	flash_addr += first_write;

	for (i = 1; i < write_num; i++)
	{
		spi_SFWriteData(flash_addr, write_buf, 256);
		write_buf += 256;
		flash_addr += 256;
	}
	spi_SFWriteData(flash_addr, write_buf, remain_data);
	write_buf += remain_data;
	flash_addr += remain_data;
	res = 1;
WriteFinish:
	flash_running = 0;
	return res;
}

u8 spi_read(u32 flash_addr, u8 *data_buf, u32 data_len)
{
	int i, read_num = data_len / 256, remain_data = data_len % 256;

	//	if((flash_addr < spi_code_limit) || ((flash_addr + data_len) > FLASH_CAP)) {
	//		printf("> spi_read error\n");
	//		return -1;
	//	}

	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;

	for (i = 0; i < read_num; i++)
	{

		spi_SFReadData(&data_buf[i * 256], flash_addr + i * 256, 256);
	}
	if(remain_data)
		spi_SFReadData(&data_buf[read_num * 256], flash_addr + read_num * 256, remain_data);
	flash_running = 0;

	return 1;
}

u8 *spi_read_data(u32 flash_addr, struct spidata *data)
{
	// int i, read_num = data_len / 256, remain_data = data_len % 256;

	//	if((flash_addr < spi_code_limit) || ((flash_addr + data_len) > FLASH_CAP)) {
	//		printf("> spi_read error\n");
	//		return -1;
	//	}

	while (flash_running)
	{
		osal_task_sleep(1);
	}
	flash_running = 1;
	u8 *data_buf = NULL;
	spi_SFReadData(data, flash_addr, sizeof(struct spidata));
	if (data->version != VERSION)
	{
		printf("spi_read_data version isn't match!\r\n");
		printf("data->version:%d\r\n", data->version);
		printf("data->len:%d\r\n", data->datalen);
		printf("data->type:%d\r\n", data->type);
		goto ReadFinish;
	}
	u32 data_len = data->datalen;
	if (flash_addr < FLASH_START || (flash_addr + data_len + sizeof(struct spidata)) > FLASH_END)
	{
		printf("spi_read over border!\r\n");
		goto ReadFinish;
	}
	data_buf = osal_malloc(data_len);
	int i, read_num = data_len / 256, remain_data = data_len % 256;
	for (i = 0; i < read_num; i++)
	{

		spi_SFReadData(&data_buf[i * 256], flash_addr + sizeof(struct spidata) + i * 256, 256);
	}
	spi_SFReadData(&data_buf[read_num * 256], flash_addr + sizeof(struct spidata) + read_num * 256, remain_data);
ReadFinish:
	flash_running = 0;
	return data_buf;
}
static u32 manu_id;
#define SF_READ_ID 0x9f

u32 spi_getFlashReadID(void)
{
	u32 u32Result = 0;
	spi_setCSStatus(0); //cs LOw
	spi_SendByte(SF_READ_ID);

	u32Result = u8spi_ReceiveByte();
	manu_id = u32Result;
	printf("manu_id: 0x%x \n",manu_id);
	u32Result = (u32Result<<8)+u8spi_ReceiveByte();
	u32Result = (u32Result<<8)+u8spi_ReceiveByte();
	printf("u32Result: 0x%x \n",u32Result);
	spi_setCSStatus(1); //cs HIGH
	return u32Result;
}

int spi_get_flash_uuid(char* pbuf,int len)
{
	u32 u32Result=0;
	int i;

	spi_getFlashReadID();
	//hal_spiIOCheck();//复用检查
	spi_setCSStatus(0); //CS LOW
	//len = 0;
	int result = 0;
	//w25q128. 64bit
	//p25q32h 128bit.
	//gd25q32
	//en25q32.
	int readlen;
	spi_SendByte(0x4b);
	spi_SendByte(0);
	spi_SendByte(0);
	spi_SendByte(0);
	spi_SendByte(0);

	printf("spi_get_flash_uuid manu_id: 0x%x \n",manu_id);
	switch(manu_id)
	{
		//winbond.
		case 0xef: 
		case 0x5e: //zb
		case 0x85: //pu ran.
		case 0x0b: //XTX.
		case 0xe0: //paragon.о 
		case 0x68: //BY25Q64ASSIG      
		case 0x20: //xmc XM25QH128C 64bit.  
		case 0x7f: //PMC
		case 0x9d: //issi
		{
			for(i=0;i<len;i++)
			{
				pbuf[i] = u8spi_ReceiveByte();
				printf(" 0x%2x",pbuf[i]);
			}
		}
		break;
		case 0xc2://mxic MX25L12833F
		case 0xc8: //gd
		case 0x1c: //pn25f64
		case 0xf8://fm25q64a dosilicon
		case 0x01: //spansion. S25FL116K
		case 0xa1://foresee: longsys.
				result = -1;
		default:
				for(i=0;i<len;i++)
				{
						pbuf[i] = u8spi_ReceiveByte();
				}
				
				break;        
	}

	spi_setCSStatus(1);//CS HIGH

	if(result == -1)
	{
		printf("warning jecid %d don't support 4b cmd\n",manu_id);        
	}
	return result;        
}


