/**@File osal_msgq.h
 * @brief message queue which can transfer 64-bit messages
 * @details
 * 除了最弱的uCosII祇能傳遞32位消息指針之外，其他多數系統都有能力傳遞長消息。
 * 傳遞太長的消息也沒有必要，像Windows一樣傳遞消息類型和消息參數還是很有必要的。
 * 因此定義一個傳遞64-bit的消息機制還是比較實用的。
 */

#ifndef __OSAL_MSGQ_H__
#define __OSAL_MSGQ_H__

#include "asm/osal_msgq.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_MSGQ_H__ */
