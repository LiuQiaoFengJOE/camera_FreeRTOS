#ifndef __SPI_H
#define __SPI_H
#include "debug.h"
#define JSON_TYPE 0x01
#define VERSION 1141859

#define FLASH_START 0x7F0000
#define FLASH_END 0x800000

struct spidata
{
	u32 datalen;
	u32 version;
	u8 type;
};
#endif
