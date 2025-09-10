#ifndef __HAL_IMGFILE_H__
#define __HAL_IMGFILE_H__

#include <stdint.h>

struct imgfile_device
{
  char *filename;
  void *fp;
  int (*init)(struct imgfile_device *dev);
  void (*seek)(struct imgfile_device *dev, uint32_t pos);
  void (*save_chunk)(struct imgfile_device *dev, uint8_t *buf, uint32_t len);
  void (*read_chunk)(struct imgfile_device *dev, uint8_t *buf, uint32_t len);
  int status;
};

#endif /* __HAL_IMGFILE_H__ */
