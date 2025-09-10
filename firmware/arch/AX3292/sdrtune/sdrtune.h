#ifndef __SDRTUNE_H__
#define __SDRTUNE_H__

#include "spr_defs.h"
#include "AX329x.h"

#include "typedef.h"

// #define WTD_DIS() wdt_disable()
#define WTD_DIS() WDTCON = 0x5001D;

#define WTD_EN(t)
#define WTD_CLR() WDTCON = 0x5003D

#define clktun_const (1 << 12) | (1 << 6) | (1 << 5)

#ifndef ARRAY_NUM
#define ARRAY_NUM(array) (sizeof(array) / sizeof(array[0]))
#endif

typedef struct tune_s
{
  struct
  {
    u8 index, cnt;
  } value[5]; // 仅保存5组值,依据经验设置,虽然有局限性
  u8 remain;  // value[]中剩余可用的元素数量
  u8 cur;     // 当前可用的value[]的index
} tune_t;

extern tune_t tune_b11_7;    // 仅包含b11_7的tune信息
extern tune_t tune_values[]; // 包含所有的信息

struct boost_t
{
  uint32_t rambase;
  uint32_t entry;
  uint8_t filename[12];
  void (*rambase_adjust)(uint32_t *param);
};

extern void find_load_file(struct boost_t *);

extern uint32_t boost_start[];

#endif /* __SDRTUNE_H__ */
