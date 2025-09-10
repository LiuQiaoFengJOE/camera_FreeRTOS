/**@File osal_msgq.h
 * @brief 简单的message queue實現的64-bit message傳遞
 * @details
 * 可以傳遞消息類型和消息值共64位信息
 */

#ifndef __ASM_MSGQ_H__
#define __ASM_MSGQ_H__

#include "FreeRTOS.h"
#include "queue.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define OSAL_MSGQ_ERRID ((void *)0)

#define osal_msgq_create(qname, maxmsg) xQueueCreate(maxmsg, 2 * sizeof(uint32_t))
#define osal_msgq_delete(mqid) vQueueDelete(mqid)

#define osal_msgsnd(mqid, msg_t, msg_v) ({\
  uint32_t msg[2] = {(uint32_t)(msg_t), (uint32_t)(msg_v)};\
  xQueueSend (mqid, msg, ~0);\
})

#define osal_msgrcv(mqid, msg_t, msg_v) ({\
  uint32_t msg[2];\
  int ret = xQueueReceive (mqid, msg, ~0);\
  if(msg_t)\
        *msg_t = msg[0];\
  if(msg_v)\
        *msg_v = msg[1];\
  ret;\
})

#ifdef __cplusplus
}
#endif

#endif /* __ASM_MSGQ_H__ */
