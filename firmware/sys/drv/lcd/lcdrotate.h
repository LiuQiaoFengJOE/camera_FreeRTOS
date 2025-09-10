#include "typedef.h"
#include "lcdframe.h"
#ifndef __HAL_ROTATE_H
#define __HAL_ROTATE_H

enum rotate_channel_e {
    ROTATE_CH0, //video
    ROTATE_CH1, //osd
    ROTATE_CH_MAX,
};
void lcdRotateInit(void);
void lcdRotateAdd(u8 ch,disp_frame_t * src,disp_frame_t * dst);

#endif
