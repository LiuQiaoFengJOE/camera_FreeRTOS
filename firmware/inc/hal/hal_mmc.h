#ifndef __HAL_MMC_H__
#define __HAL_MMC_H__

#include <stdint.h>

struct mmc_host
{
    uint32_t reg_base;
    uint32_t dat1_port;
    uint32_t dat1_pin;
};
/****************sd card***************************/

typedef enum
{
    NOT_SD,
    SD_10,
    SD_20_NC,
    SD_20_HC,
    MMC
} SD_VER;

typedef enum
{
    SD_OFFLINE = 0,
    SD_READ,
    SD_WRITE,
    SD_FREE,
} CARD_STATE;

typedef struct
{
    unsigned char busy;
    unsigned long dwRCA;
    SD_VER eVer;
    unsigned long dwCap;
    unsigned long dwNextLBA;
    CARD_STATE eCardState;
} SD_INFO;

enum
{
    CARD_OFFLINE = 0,
    CARD_ONLINE,
    CARD_READY,
    CARD_STANDBY,
};
enum
{
    WAITING_OPS = 1,
    DEALING_OPS,
    FINISH_OPS,
};
typedef struct
{
    unsigned long Lba;
    unsigned long LbaLen;
    unsigned char *pbuf;
    unsigned char mode;
    unsigned char status;
} SD_OPS;

/****************************/

#endif /* __HAL_MMC_H__ */
