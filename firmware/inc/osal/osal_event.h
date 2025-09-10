/********************************************************************************
*
*AppoTech Corporation, All Rights Reserved. 2016 Copyright (C)
*
* File name: osal_event.h
*
* Description:
* Event relative functions definition for Operating System Abstract Layer
*
* Author: CHEN<chen@buildwin.com.cn>
*
* Create date: June. 15, 2016
*
*********************************************************************************
*
* Revision history:
---------------------------------------------------------------------------------
Date                    Modification                 Revision            Name
---------------------------------------------------------------------------------
June. 15, 2016      Created                       V0.1                 CHEN

********************************************************************************/
/**
 * @File osal_event.h
 * @brief Event  relative functions definition for Operating System Abstract Layer
 */

#ifndef __OSAL_EVENT_H__
#define __OSAL_EVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_defs.h>

    typedef enum
    {
        /* Unblock task waiting until all specified bits are set */
        OSAL_EVENT_BITS_AND = 0x1,

        /* Unblock task waiting if one of specified bits are set */
        OSAL_EVENT_BITS_OR = 0x2,

        /* Clear flag automaticaly after the first Unblock task waiting  */
        OSAL_EVENT_BITS_CLR = 0x4,
    } OSAL_EVENT_MODE;

    /************************************************************************************
     * @name osal_event_create
     * ----------------------------------------------------------------------------------
     * @brief Create the event
     *
     * Arguments:
     * @param[in] void
     * ----------------------------------------------------------------------------------
     * @return OSAL_HDL
     * @retval NULL - Fail to create the event
     * @retval Not NULL - Succeed to create and return the event handle
     ***********************************************************************************/
    OSAL_HDL osal_event_create(void);

    /************************************************************************************
     * @name osal_event_wait
     * ----------------------------------------------------------------------------------
     * @brief Wait the event bits
     *
     * Arguments:
     * @param[in] event_hdl - Handle of the event
     * @param[in] wait_bits - The event bits  to wait
     * @param[in] wait_mode - The mode of waiting event bits
     * @param[in] time_out - Time for wait time out
     * ----------------------------------------------------------------------------------
     * @return RET_CODE
     * @retval The value of the event group at the time either the bits being waited for became set,
     *    or the block time expired
     ***********************************************************************************/
    RET_CODE osal_event_wait(OSAL_HDL event_hdl, UINT32 wait_bits, UINT32 wait_mode, UINT32 time_out);

    /************************************************************************************
     * @name osal_event_set
     * ----------------------------------------------------------------------------------
     * @brief Set the event bits
     *
     * Arguments:
     * @param[in] event_hdl - Handle of the event
     * @param[in] set_bits - The event bits  to set
     * ----------------------------------------------------------------------------------
     * @return RET_CODE
     * The value of the event group at the time the call to xEventGroupSetBits()
     ***********************************************************************************/
    RET_CODE osal_event_set(OSAL_HDL event_hdl, UINT32 set_bits);

    /************************************************************************************
     * @name osal_event_clear
     * ----------------------------------------------------------------------------------
     * @brief Clear the event bits
     *
     * Arguments:
     * @param[in] event_hdl - Handle of the event
     * @param[in] clear_bits - The event bits	to clear
     * ----------------------------------------------------------------------------------
     * @return RET_CODE
     * The value of the event group at the time the call to xEventGroupClearBits()
     ***********************************************************************************/
    RET_CODE osal_event_clear(OSAL_HDL event_hdl, UINT32 clear_bits);

    /************************************************************************************
     * @name osal_event_delete
     * ----------------------------------------------------------------------------------
     * @brief Delete the event
     *
     * Arguments:
     * @param[in] event_hdl - Handle of the event
     * ----------------------------------------------------------------------------------
     * @return void
     ***********************************************************************************/
    void osal_event_delete(OSAL_HDL event_hdl);

#ifdef __cplusplus
}
#endif

#endif /* __OSAL_EVENT_H__ */
