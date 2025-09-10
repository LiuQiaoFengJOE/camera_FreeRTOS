#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef BIT
#define BIT(b) (1 << (b))
#endif

#define FILE_NOT_FOUND ~0

#define DBG_EN 1

#ifndef __ASSEMBLER__
struct LOAD_PARAM
{
  uint32_t (*find_load_file)(void);
  uint32_t rambase;
  uint32_t entry;
  uint8_t filename[12];
  void (*rambase_adjust)(uint32_t *param);
  uint32_t filelen;
};
#endif

#endif /*  _COMMON_H_ */
