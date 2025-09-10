#include "typedef.h"
#include "spr_defs.h"
#include "AX329x.h"

////////////////////////////////////////////////////////////////////////////////
// cache
#define CACHE_LINE_SIZE 16

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief dma初始化
 * @details dma在使用前，需要確保MCPCON.1上的pnd為1。
 * 為此，進行一次假DMA，以置上該位
 */
/*
void dma_memcpy_init (void)
{
 uint32_t dst[4];
 uint32_t src[4];

 MCPCON |= BIT(0);     // en
 MCPCON &= ~BIT(2);    // no sir
 MCPSCADDR = (uint32_t) src;
 MCPTGADDR = (uint32_t) dst;
 MCPLENGTH = 15;       // kick
}
*/

////////////////////////////////////////////////////////////////////////////////
static void byte2byte_memcpy(uint8_t *dst, uint8_t *src, uint32_t cnt)
{
  while (cnt--)
  {
    *dst++ = *src++;
  }
}

/* 一次複製4字節，需要保證dst和src按4字節對齊 */
static void word2word_memcpy(uint32_t *dst, uint32_t *src, uint32_t cnt)
{
  while (cnt >= 4)
  {
    cnt -= 4;
    *dst++ = *src++;
  }
}

/* 一次複製16字節，需要保證dst和src按4字節對齊 */
static void _block16_memcpy(uint32_t *dst, uint32_t *src, uint32_t len)
{
  while (len >= 16)
  {
    len -= 16;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    src += 16;
    dst += 16;
  }
}

////////////////////////////////////////////////////////////////////////////////
#define _UNALIGNED(src, dst) ((((uint32_t)src) | ((uint32_t)dst)) & 3)
static void cpu_memcpy(uint8_t *dst, uint8_t *src, uint32_t cnt)
{
  /* 不太小，又雙對齊，則加速 */
  if (cnt >= 16 && !_UNALIGNED(src, dst))
  {
    uint32_t *aligned_dst = (uint32_t *)dst;
    uint32_t *aligned_src = (uint32_t *)src;

    // 先按16字節加速
    while (cnt >= 16)
    {
      cnt -= 16;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
      *aligned_dst++ = *aligned_src++;
      //      aligned_dst[0] = aligned_src[0];
      //      aligned_dst[1] = aligned_src[1];
      //      aligned_dst[2] = aligned_src[2];
      //      aligned_dst[3] = aligned_src[3];
      //      aligned_src += 4;
      //      aligned_dst += 4;
    }
    // 再按4字節加速
    while (cnt >= 4)
    {
      cnt -= 4;
      *aligned_dst++ = *aligned_src++;
    }

    dst = (uint8_t *)aligned_dst;
    src = (uint8_t *)aligned_src;
  }
  // 剩下的，祇能一字節一字節來
  byte2byte_memcpy(dst, src, cnt);
}
/*
static void dma_memcpy (void *dst, void *src, uint32_t cnt)
{
  uint32_t flags;

  dcache_writeback_region ((uint32_t)src, cnt);
  dcache_flush_region     ((uint32_t)dst, cnt);

  while ((MCPCON & BIT(1)) == 0);
  flags = arch_local_irq_save ();

  MCPCON |= BIT(0);     // en
  MCPCON &= ~BIT(2);    // no sir
  MCPSCADDR = (uint32_t) src;
  MCPTGADDR = (uint32_t) dst;
  MCPLENGTH = cnt - 1;  // kick

  arch_local_irq_restore (flags);
  while ((MCPCON & BIT(1)) == 0);
}
*/
////////////////////////////////////////////////////////////////////////////////
void *memcpy(void *dst, const void *src, uint32_t cnt)
{
  /* 如果數據太少，沒有必要使用DMA */ /*
   if ((cnt > CACHE_LINE_SIZE * 3) && (dst >= (void*)0x02000000)) {
     dma_memcpy (dst, src, cnt);
     return dst;
   }
    * */
  cpu_memcpy(dst, src, cnt);
  return dst;
}
