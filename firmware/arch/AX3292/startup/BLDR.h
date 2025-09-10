#ifndef __BLDR_H__
#define __BLDR_H__

#include <stdint.h>

typedef void (*bldr_entry_func)(void);

typedef struct BLDR_t
{
  uint32_t ver;
  uint8_t magic[4];
  uint8_t chksum;
  uint8_t off;
  uint8_t flags;
  uint8_t dummy[5];

  uint8_t reserved[16];

  uint32_t addr;
  uint32_t secno;
  uint32_t secs;
  bldr_entry_func entry;

  uint32_t chksum_app;
  uint32_t magickey;
  uint32_t spi_param;
  uint32_t spinand_cmd;

  uint32_t baud;
} BLDR_t;

typedef struct
{
  uint8_t dummy[0x58];
} BPB_FAT32_t;

#endif /* __BLDR_H__ */
