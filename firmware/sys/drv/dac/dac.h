#ifndef __AUADC_H__
#define __AUADC_H__
#include "osal/osal_task.h"
#include "osal/osal_alloc.h"
#include "osal/osal_wait.h"
#include "stream/stream.h"
#include "ax32xx_auadc.h"
#include "debug.h"
#include "stream/stream.h"

typedef void *(*_getPlayBuff)(u8 **buff, u32 *size, void *argv);
typedef void (*_playDone)(void *argv);
s32 dacPlay(u32 sampleRate, u8 volume, _getPlayBuff getPlayBuff, _playDone playDone, void *argv);
s32 dacPlayBuff(u32 sampleRate, u8 volume, u8 *curPlayBuff, u32 curPlayBuffSize);

#endif
