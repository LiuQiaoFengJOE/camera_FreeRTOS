#ifndef __AUADC_H__
#define __AUADC_H__
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "osal/osal_wait.h"
#include "stream/stream.h"
#include "ax32xx_auadc.h"
#include "debug.h"
#include "stream/stream.h"

typedef struct auadc_drv
{
  void *controller;
  int running; // adc正在工作中
  u8 simGain;
  u8 digGain;
  //  struct frame   *yuv_frame;
  //  struct frame   *jpg_frame;
  uint32_t pcm_rate;
  //  uint32_t        yuv_w, yuv_h, y_size;
  //  uint32_t        jpg_w, jpg_h;
} auadc_drv;

#endif
