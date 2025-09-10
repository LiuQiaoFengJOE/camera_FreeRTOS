/**從FAT32文件系統中查找並加載特定文件
 * @file fat32.c
 * @author 何凱帆
 */
#include <stdint.h>

#include "bootfat32.h"
#define FATVER 28
#include "bpb.h"

#define LEDERR_NONE 0
#define LEDERR_TIGA 1
#define LEDERR_VISION 2
#define LEDFORERROR2 LEDERR_NONE

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LE32(x) x
#define LE16(x) x
#endif
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#error "LE32 should be defined"
#endif

#ifndef NORETURN
#define NORETURN __attribute__((noreturn))
#endif

#ifndef SECTION
#define SECTION(s) __attribute__((section(s)))
#endif

#ifndef INLINE
#define INLINE inline
#endif

#ifndef POS_BPBHDR
#define POS_BPBHDR SECTION(".bss.hdr")
#endif

#ifndef POS_BPB
#define POS_BPB SECTION(".bss.bpb")
#endif

#ifndef BSS
#define BSS
#endif
#ifndef CODE
#define CODE
#endif

#define bpb32(x) LE32(bpb.x)
#define bpb16(x) LE16(bpb.x)

#define DBG_FAT32 0

// Load entire clusts
// #define load_entire load
// Load entire sectors
#define load_sectors load

int bootbsp_putchar(int);
void bootbsp_readsecs(char BSS *, uint32_t, uint32_t);

/******************************************************************************
 * 提升此模塊的可複用性
 ******************************************************************************/
extern struct LOAD_PARAM CODE load_param;
#define rambase load_param.rambase
#define filename ((char CODE *)load_param.filename)
#define entry (uint32_t) load_param.entry
#define filelen (*(uint32_t BSS *)&load_param.filelen)

/*******************************************************************************
 * Debug
 * 需要bsp提供putchar
 ******************************************************************************/
static const char CODE *puts(const char CODE *str)
{
#if DBG_FAT32
  while (*str)
  {
    bootbsp_putchar(*str);
    ++str;
  }
  return str;
#endif
}

static void putword(uint32_t val)
{
#if DBG_FAT32
  static const char CODE hex[16] = "0123456789ABCDEF";
  int i;
  for (i = 0; i < 8; ++i)
  {
    bootbsp_putchar(hex[val >> 28]);
    val <<= 4;
  }
#endif
}

static void show_log(uint32_t stage)
{
#if DBG_FAT32
  putword(stage);
  puts("STAGE\r\n");
#endif
}

static NORETURN void show_error(uint32_t err)
{
#if DBG_FAT32
  putword(err);
  puts("ERROR\r\n");
  while (1)
    ;
#endif
}

/*******************************************************************************
 * 底層支持函數
 ******************************************************************************/
/* 需由MBR/DBR主程序複製好數據；由BSP分配空間 */
char BSS hdr[8] POS_BPBHDR;
BPB_t BSS bpb POS_BPB;
/* 下列與bpb一起定義成全局變量 */
uint32_t BSS data_sec SECTION(".bss.var");
uint32_t BSS clust_num SECTION(".bss.var");
uint32_t BSS buffered_sec_num SECTION(".bss.var");
uint32_t BSS cur_sec SECTION(".bss.var");

// extern char BSS RAMBASE[];        /* 需由BSP提供 */
// extern const char CODE filename[]; /* 需由BSP提供，以按BSP要求加載固定文件 */
extern uint32_t BSS __sdram[];    /* 需由BSP提供 */
extern uint32_t BSS SDRAM_SIZE[]; /* 需由BSP提供 */

char BSS FAT_SEC_BUFF[512];
#ifndef DIR_SEC_BUFF
#define DIR_SEC_BUFF rambase
#else
char BSS DIR_SEC_BUFF[512];
#endif

static INLINE int clust2sec_check(void)
{
  cur_sec = (clust_num - 2) * bpb.sec_per_clust + data_sec;
  return clust_num < ERR_FAT_NUM;
}

static INLINE void next_clust(void)
{
  uint32_t idx;
  uint32_t sec_num;

  bootbsp_putchar('.');

  idx = clust_num << ITEMSIZE_LOG2;
  sec_num = idx >> bpb.byte_per_sec_shift;
  idx &= (1 << bpb.byte_per_sec_shift) - 1;
  if (sec_num != buffered_sec_num)
  {
    buffered_sec_num = sec_num;
    bootbsp_readsecs(FAT_SEC_BUFF, sec_num + bpb.sec_num_of_FAT, 1);
  }
#if FATVER == 28
  idx = LE32(*(FAT_ITEM_t *)&FAT_SEC_BUFF[idx]);
  clust_num = (idx << 4) >> 4;
#elif FATVER == 16
  clust_num = LE16(*(FAT_ITEM_t *)&FAT_SEC_BUFF[idx]);
#else
  clust_num = LE32(*(FAT_ITEM_t *)&FAT_SEC_BUFF[idx]);
#endif
}

/*******************************************************************************
 * 三個主要流程
 ******************************************************************************/
void bootfat32_init(void)
{
  show_log(5);
  // 計算byte_per_sec_shift，可以用ff1優化
  {
    uint32_t byte_per_sec_shift;
    uint8_t tmp;
    tmp = bpb.byte_per_sec[1];
    byte_per_sec_shift = 7;
    do
    {
      ++byte_per_sec_shift;
      tmp >>= 1;
    } while (tmp);
    bpb.byte_per_sec_shift = byte_per_sec_shift;
  }
  if (DBG_FAT32)
  {
    putword(bpb.byte_per_sec_shift);
    puts("byte_per_sec_shift\r\n");
  }

  // 計算cur_sec
  {
    //    uint32_t sec_per_FAT;
    uint32_t FAT_sec;

    //    sec_per_FAT = bpb32(sec_per_FAT);/* should be bpb16 and no used */
    FAT_sec = bpb32(sec_num_of_partition) + bpb16(sec_num_of_FAT_);

    bpb.sec_num_of_FAT = bpb32(active_FAT) * bpb32(FAT32_sec_per_FAT) + FAT_sec;

    data_sec = bpb.num_of_FAT * bpb32(FAT32_sec_per_FAT) + FAT_sec;
    //  if (DBG_FAT32) {
    //    putword (data_sec); puts ("data_sec\r\n");
    //    putword (bpb.sec_num_of_FAT); puts ("bpb.sec_num_of_FAT\r\n");
    //    putword (sec_per_FAT); puts ("sec_per_FAT\r\n");
    //    putword (FAT_sec); puts ("FAT_sec\r\n");
    //  }
  }

  // 初始化變量
  {
    buffered_sec_num = ~0;
  }
}

static INLINE int find(void)
{
  show_log(2);

  clust_num = bpb32(root_clust);
  while (clust2sec_check())
  {
    uint32_t end_sec = cur_sec + bpb32(sec_per_clust);
    for (; cur_sec < end_sec; ++cur_sec)
    {
      pDIR_t cur_dir, end_dir;
      int i;

      putword(cur_sec);

      bootbsp_readsecs(DIR_SEC_BUFF, cur_sec, 1);
      cur_dir = (pDIR_t)DIR_SEC_BUFF;
      end_dir = (pDIR_t)(DIR_SEC_BUFF + 512);
      for (; cur_dir < end_dir; ++cur_dir)
      {
        if (cur_dir->entry_type == None_DirectoryEntry)
          goto err;
#if 0
        if (! bootfat32_strcmp(filename, (uint8_t*)cur_dir)) {
          clust_num = (LE16(cur_dir->clust_high)<<16) + LE16(cur_dir->clust_low);
          filelen   = cur_dir->file_len;
          if (DBG_FAT32) {putword(clust_num);puts(filename);}
          return 1; // 返回1比返回clust_num經濟
        }
#elif 1
        i = 0;
      match_loop:
        if (filename[i] == 0)
        {
          filelen = cur_dir->file_len;
          clust_num = (LE16(cur_dir->clust_high) << 16) + LE16(cur_dir->clust_low);
          if (DBG_FAT32)
          {
            putword(clust_num);
            puts(filename);
          }
          return 1; // 返回1比返回clust_num經濟
        }
        if (filename[i] != ((uint8_t *)cur_dir)[i])
          continue;
        ++i;
        goto match_loop;
#else
        for (i = 10; i >= 0; --i)
          if (((uint8_t *)cur_dir)[i] != filename[i])
            break;
        if (i >= 0)
          continue;
        filelen = cur_dir->file_len;
        clust_num = LE16(cur_dir->clust_low) + (LE16(cur_dir->clust_high) << 16);
        if (DBG_FAT32)
        {
          putword(clust_num);
          puts(filename);
        }
        return 1; // 返回1比返回clust_num經濟
#endif
      }
    }
    next_clust();
  }
err:
#if LEDFORERROR2 == LEDERR_TIGA
  // PORTA |= (1<<4); while(1) wdt_clr();
  *(unsigned *)0x40 |= (1 << 4);
  while (1)
    *(volatile unsigned *)0x210 |= 0x50000;
#else
  show_error(2);
#endif
  return 0;
}

#if defined(load_entire)
static INLINE void load_entire(void)
{
  char BSS *ptr = rambase;
  // matched
  show_log(1);

  while (clust2sec_check())
  {
    if (DBG_FAT32)
    {
      putword(clust_num);
    }
    bootbsp_readsecs(ptr, cur_sec, bpb.sec_per_clust);
    ptr += bpb.sec_per_clust << 9;
    next_clust();
  }
  show_log(0);
}
#elif defined(load_sectors)
static INLINE void load_sectors(void)
{
  char BSS *ptr = rambase;
  uint32_t fsecs, secs;
  // matched
  show_log(1);

  fsecs = (filelen + 511) >> 9;
  while (clust2sec_check())
  {
    if (DBG_FAT32)
    {
      putword(clust_num);
    }

    secs = bpb.sec_per_clust;
    if (secs > fsecs)
      secs = fsecs;
    fsecs -= secs;

    bootbsp_readsecs(ptr, cur_sec, secs);
    ptr += bpb.sec_per_clust << 9;
    next_clust();
  }
  show_log(0);
}
#elif defined(load_1_clust)
static INLINE void load_1_clust(void)
{
  char BSS *ptr = rambase;
  // matched
  show_log(1);

  if (clust2sec_check())
  {
    bootbsp_readsecs(ptr, cur_sec, bpb.sec_per_clust);
  }
  show_log(0);
}
#elif defined(load_1_sect)
static INLINE void load_1_sect(void)
{
  char BSS *ptr = rambase;
  // matched
  show_log(1);

  if (clust2sec_check())
  {
    bootbsp_readsecs(ptr, cur_sec, 1);
  }
  show_log(0);
}
#elif !defined(load_self)
#error "Should define an load type"
#endif

/*******************************************************************************
 * 主函數
 ******************************************************************************/
/**查找並加載文件
 * 要求：
 * - MBR/DBR主程序原樣複製好BPB
 * - BSP提供文件名filename
 * @retval FILE_NOT_FOUND 沒有找到文件
 */
uint32_t find_load_file(uint32_t dummy, uint8_t *param)
{
  bootfat32_memcpy2(((uint8_t *)&load_param) + 4, param, param + 0x18);
  putword(rambase);
  //  putword((uint32_t)&(filename)[0]);
  puts(filename);
  // puts("find_load_file\r\n");
  if (!find())
    return ~0;
  if (load_param.rambase_adjust)
    load_param.rambase_adjust(&rambase);
  load();
  return (uint32_t)entry;
}
