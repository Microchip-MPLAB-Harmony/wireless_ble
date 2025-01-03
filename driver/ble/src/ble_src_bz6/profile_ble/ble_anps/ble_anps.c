/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  BLE Alert Notification Profile Server Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_anps.c

  Summary:
    Implements the server-side functionality of the BLE Alert Notification Profile.

  Description:
    This source file provides the implementation for the server-side functions
    of the Bluetooth Low Energy (BLE) Alert Notification Profile (ANP), enabling
    the device to serve alert notifications to a connected BLE client.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_ans/ble_ans.h"
#include "ble_anps.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_ANPS_ERRCODE_COMMAND_NOT_SUPPORTTED     0xA0U           // Error code indicating the command is not supported.

#define BLE_ANPS_RETRY_TYPE_WRITE_RESP              (0x01U)         // Retry type for write response.
#define BLE_ANPS_RETRY_TYPE_READ_RESP               (0x02U)         // Retry type for read response.
#define BLE_ANPS_RETRY_TYPE_ERR                     (0x03U)         // Retry type for error response.

#define BLE_ANPS_MAX_CONN_NBR                       BLE_GAP_MAX_LINK_NBR    //< Maximum number of allowed connections.

 typedef enum BLE_ANPS_State_T
{
    BLE_ANPS_STATE_IDLE = 0x00U,                    // State indicating the service is idle.
    BLE_ANPS_STATE_CONNECTED                        // State indicating the service is connected.
} BLE_ANPS_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* Structure for BLE Alert Notification Profile (ANP) supported categories. */
typedef struct BLE_ANPS_Params_T
{
    uint16_t            suppNewCat;                         // Supported New Alert Categories.
    uint16_t            suppUnreadCat;                      // Supported Unread Alert Categories.
} BLE_ANPS_Params_T;

/* Structure for BLE ANP connection-specific information. */
typedef struct BLE_ANPS_ConnList_T
{
    uint16_t            connHandle;                         // Connection handle associated with this connection.
    BLE_ANPS_State_T    state;                              // Current state of the ANP connection.
    uint8_t             retryType;                          // Type of retry mechanism in use.
    uint8_t             *p_retryData;                       // Pointer to the data required for retry operations.
} BLE_ANPS_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static BLE_ANPS_EventCb_T     sp_anpsCbRoutine;             // Callback function for ANPS (Alert Notification Profile Server) events.
static BLE_ANPS_ConnList_T    s_anpsConnList[BLE_ANPS_MAX_CONN_NBR]; // Array to keep track of connection-specific information for each active connection.
static BLE_ANPS_Params_T      s_anpsParams;                 // Structure to hold the parameters for the ANPS (Alert Notification Profile Server).

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Frees the retry data associated with a BLE connection.
 *
 * @param p_conn Pointer to the BLE_ANPS_ConnList_T structure that contains the retry data to be freed.
 */
static void ble_anps_FreeRetryData(BLE_ANPS_ConnList_T *p_conn) {
    if (p_conn->p_retryData != NULL)
    {
        OSAL_Free(p_conn->p_retryData);
        p_conn->p_retryData = NULL;
        p_conn->retryType = 0;
    }
}


/**
 * @brief Initializes a BLE connection list entry to default values.
 *
 * @param p_conn Pointer to the BLE_ANPS_ConnList_T structure to be initialized.
 */
static void ble_anps_InitConnList(BLE_ANPS_ConnList_T *p_conn)
{
    (void)memset(p_conn, 0, sizeof(BLE_ANPS_ConnList_T));
}


/**
 * @brief Retrieves a connection list entry by its connection handle.
 *
 * @param connHandle The connection handle used to find the corresponding connection list entry.
 * 
 * @retval Pointer to the BLE_ANPS_ConnList_T structure if found, otherwise NULL.
 */
static BLE_ANPS_ConnList_T * ble_anps_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_ANPS_MAX_CONN_NBR;i++)
    {
        if ((s_anpsConnList[i].state == BLE_ANPS_STATE_CONNECTED) && (s_anpsConnList[i].connHandle == connHandle))
        {
            return &s_anpsConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Gets a free connection list entry.
 *
 * @retval Pointer to the BLE_ANPS_ConnList_T structure if a free entry is available, otherwise NULL.
 */
static BLE_ANPS_ConnList_T *ble_anps_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_ANPS_MAX_CONN_NBR;i++)
    {
        if (s_anpsConnList[i].state == BLE_ANPS_STATE_IDLE)
        {
            s_anpsConnList[i].state = BLE_ANPS_STATE_CONNECTED;
            return &s_anpsConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Conveys a BLE ANPS event to the registered callback.
 *
 * @param eventId       The event ID to be conveyed.
 * @param p_eventField  Pointer to the event field data.
 * @param eventFieldLen Length of the event field data.
 */
static void ble_anps_ConveyEvent(BLE_ANPS_EventId_T eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{ 
    if (sp_anpsCbRoutine != NULL)
    {
        BLE_ANPS_Event_T evtPara;

        evtPara.eventId = eventId;
        (void)memcpy((uint8_t *)&evtPara.eventField, (uint8_t *)p_eventField, eventFieldLen);
        sp_anpsCbRoutine(&evtPara);
    }
}


/**
 * @brief Processes a write request or command received from the GATT client.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anps_ProcWrite(GATT_Event_T *p_event)
{
    uint8_t errCode = 0;
    uint16_t status;
    BLE_ANPS_ConnList_T *p_conn;

    p_conn = ble_anps_GetConnListByHandle(p_event->eventField.onWrite.connHandle);

    if (p_conn == NULL)
    {
        ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if (p_event->eventField.onWrite.attrHandle == (uint16_t)ANS_HDL_CHARVAL_ANCP)
    {
        if (p_event->eventField.onWrite.writeValue[0] > BLE_ANPS_CMD_END)
        {
            errCode = BLE_ANPS_ERRCODE_COMMAND_NOT_SUPPORTTED;
        }
        else if (p_event->eventField.onWrite.writeType != ATT_WRITE_REQ)
        {
            errCode = ATT_ERR_REQUEST_NOT_SUPPORT;
        }
        else
        {
            //Do nothing
        }


        if (errCode==0U)
        {/* Write Response.*/
            BLE_ANPS_EvtAncpWriteInd_T evtWriteInd;
            GATTS_SendWriteRespParams_T *p_writeRespParams;

            evtWriteInd.connHandle = p_event->eventField.onWrite.connHandle;
            evtWriteInd.cmdId = p_event->eventField.onWrite.writeValue[0];
            evtWriteInd.catId = p_event->eventField.onWrite.writeValue[1];
            ble_anps_ConveyEvent(BLE_ANPS_EVT_ANCP_WRITE_IND, (uint8_t *)&evtWriteInd, (uint8_t)sizeof(evtWriteInd));
            p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
            if (p_conn->p_retryData == NULL)
            {
                ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_NO_MEM_IND, NULL, 0);
                return;
            }

            p_conn->retryType = BLE_ANPS_RETRY_TYPE_WRITE_RESP;
            p_writeRespParams = (GATTS_SendWriteRespParams_T *)p_conn->p_retryData;
            p_writeRespParams->responseType = ATT_WRITE_RSP;
            status = GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, p_writeRespParams);
        }
        else
        {/* Error Response.*/
            GATTS_SendErrRespParams_T *p_errRespParams;

            p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
            if (p_conn->p_retryData == NULL)
            {
                ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_NO_MEM_IND, NULL, 0);
                return;
            }
            p_conn->retryType = BLE_ANPS_RETRY_TYPE_ERR;
            p_errRespParams = (GATTS_SendErrRespParams_T *)p_conn->p_retryData;
            p_errRespParams->reqOpcode = p_event->eventField.onWrite.writeType;
            p_errRespParams->attrHandle = p_event->eventField.onWrite.attrHandle;
            p_errRespParams->errorCode = errCode;
            status = GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, p_errRespParams);
        }
        if (status == MBA_RES_SUCCESS)
        {
            ble_anps_FreeRetryData(p_conn);
        }
    }
} 


/**
 * @brief Processes a read request received from the GATT client.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anps_ProcRead(GATT_Event_T *p_event)
{
    uint8_t value[BLE_ATT_MAX_MTU_LEN - ATT_READ_RESP_HEADER_SIZE];
    uint8_t valueLen = 2;
    bool proceedFlag=false;
    uint16_t status;
    GATTS_SendReadRespParams_T *p_respParams;
    GATTS_SendErrRespParams_T *p_errRespParams;
    BLE_ANPS_ConnList_T *p_conn;


    switch (p_event->eventField.onRead.attrHandle)
    {
        case (uint16_t)ANS_HDL_CHARVAL_SUPP_NEW_ALERT_CAT:
        {
            (void)memcpy((uint8_t *)value, (uint8_t *)&s_anpsParams.suppNewCat, valueLen);
            proceedFlag = true;
        }
        break;
        case (uint16_t)ANS_HDL_CHARVAL_SUPP_UNREAD_ALERT_CAT:
        {
            (void)memcpy((uint8_t *)value, (uint8_t *)&s_anpsParams.suppUnreadCat, valueLen);
            proceedFlag = true;
        }
        break;
        default:
        {
            ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        }
        break;
    }

    if(proceedFlag==false)
    {
        return;
    }

    p_conn = ble_anps_GetConnListByHandle(p_event->eventField.onRead.connHandle);

    if (p_conn == NULL)
    {
        ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if (p_event->eventField.onRead.readType == ATT_READ_REQ)
    {
        p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendReadRespParams_T) - (BLE_ATT_MAX_MTU_LEN - ATT_READ_RESP_HEADER_SIZE) + (uint32_t)valueLen);
        if (p_conn->p_retryData == NULL)
        {
            ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_NO_MEM_IND, NULL, 0);
            return;
        }

        p_conn->retryType = BLE_ANPS_RETRY_TYPE_READ_RESP;
        p_respParams = (GATTS_SendReadRespParams_T *)p_conn->p_retryData;
        p_respParams->attrLength = valueLen;
        p_respParams->responseType = ATT_READ_RSP;
        (void)memcpy(p_respParams->attrValue, value, valueLen);
        status = GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, p_respParams);
    }
    else
    {
        p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
        if (p_conn->p_retryData == NULL)
        {
            ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_NO_MEM_IND, NULL, 0);
            return;
        }
        p_conn->retryType = BLE_ANPS_RETRY_TYPE_ERR;
        p_errRespParams = (GATTS_SendErrRespParams_T *)p_conn->p_retryData;
        p_errRespParams->reqOpcode = p_event->eventField.onRead.readType;
        p_errRespParams->attrHandle = p_event->eventField.onRead.attrHandle;
        p_errRespParams->errorCode = ATT_ERR_REQUEST_NOT_SUPPORT;
        status = GATTS_SendErrorResponse(p_event->eventField.onRead.connHandle, p_errRespParams);
    }

    
    if (status == MBA_RES_SUCCESS)
    {
        ble_anps_FreeRetryData(p_conn);
    }
}


/**
 * @brief Process a queued task for a BLE Alert Notification Profile Server.
 *
 * This function processes a queued task based on the connection handle provided.
 * It will handle write, read, and error responses accordingly.
 *
 * @param connHandle The connection handle associated with the BLE device.
 */
static void ble_anps_ProcessQueuedTask(uint16_t connHandle)
{
    uint16_t status;

    BLE_ANPS_ConnList_T *p_conn;

    p_conn = ble_anps_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }
    switch (p_conn->retryType)
    {
        case BLE_ANPS_RETRY_TYPE_WRITE_RESP:
        {
            status = GATTS_SendWriteResponse(p_conn->connHandle, 
                (GATTS_SendWriteRespParams_T *)p_conn->p_retryData);
        }
        break;
        case BLE_ANPS_RETRY_TYPE_READ_RESP:
        {
            status = GATTS_SendReadResponse(p_conn->connHandle, 
                (GATTS_SendReadRespParams_T *)p_conn->p_retryData);
        }
        break;
        case BLE_ANPS_RETRY_TYPE_ERR:
        {
            status = GATTS_SendErrorResponse(p_conn->connHandle, 
                (GATTS_SendErrRespParams_T *)p_conn->p_retryData);
        }
        break;
        default:
        {
            status = MBA_RES_SUCCESS;
        }
        break;
    }

    if (status == MBA_RES_SUCCESS)
    {
        ble_anps_FreeRetryData(p_conn);
    }
}


/**
 * @brief Process GATT events for the BLE Alert Notification Profile Server.
 *
 * This function handles incoming GATT events such as write and read requests
 * and processes them using the appropriate handlers.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anps_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTS_EVT_WRITE:
        {
            ble_anps_ProcWrite(p_event);
        }
        break;
        case GATTS_EVT_READ:
        {
            ble_anps_ProcRead(p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    } 
}


/**
 * @brief Initializes the BLE Alert Notification Profile.
 *
 * @retval MBA_RES_SUCCESS        Successfully initialized the BLE Alert Notification Profile.
 *
 */
uint16_t BLE_ANPS_Init(void)
{
    uint8_t i;

    sp_anpsCbRoutine = NULL;
    (void)memset(&s_anpsParams, 0, sizeof(BLE_ANPS_Params_T));
    for (i = 0; i < BLE_ANPS_MAX_CONN_NBR; i++)
    {
        ble_anps_InitConnList(&s_anpsConnList[i]);
    }
    return BLE_ANS_Add();
}


/**
 * @brief Registers a callback function for the BLE Alert Notification profile events.
 *
 * @param[in] bleAnpRoutine        The callback function to handle BLE Alert Notification profile events.
 *
 */
void BLE_ANPS_EventRegister(BLE_ANPS_EventCb_T bleAnpRoutine)
{
    sp_anpsCbRoutine = bleAnpRoutine;
}


/**
 * @brief Sets the supported categories for new alerts.
 *
 * @param[in] catMask            Category mask for new alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 *
 * @retval MBA_RES_SUCCESS        Successfully set the supported categories for new alerts.
 * @retval MBA_RES_FAIL           Cannot change supported categories while in a connection.
 * @retval MBA_RES_INVALID_PARA   The parameter is invalid or does not meet the specification.
 */
uint16_t BLE_ANPS_SetSuppNewCat(uint16_t catMask)
{
    uint8_t i;

    for(i=0; i<BLE_ANPS_MAX_CONN_NBR;i++)
    {
        if (s_anpsConnList[i].state == BLE_ANPS_STATE_CONNECTED)
        {
            return MBA_RES_FAIL;
        }
    }
    if((catMask & BLE_ANPS_SUPP_CAT_RESERVED)!= 0U)
    {
        return MBA_RES_INVALID_PARA;
    }
    s_anpsParams.suppNewCat = catMask;
    return MBA_RES_SUCCESS;
}


/**
 * @brief Sets the supported categories for unread alerts.
 *
 * @param[in] catMask             Category mask for unread alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 *
 * @retval MBA_RES_SUCCESS        Category mask for unread alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 * @retval MBA_RES_FAIL           Cannot change supported categories while in a connection.
 * @retval MBA_RES_INVALID_PARA   The parameter is invalid or does not meet the specification.
 */
uint16_t BLE_ANPS_SetSuppUnreadCat(uint16_t catMask)
{
    uint8_t i;

    for(i=0; i<BLE_ANPS_MAX_CONN_NBR;i++)
    {
        if (s_anpsConnList[i].state == BLE_ANPS_STATE_CONNECTED)
        {
            return MBA_RES_FAIL;
        }
    }
    if((catMask & BLE_ANPS_SUPP_CAT_RESERVED)!= 0U)
    {
        return MBA_RES_INVALID_PARA;
    }
    s_anpsParams.suppUnreadCat = catMask;
    return MBA_RES_SUCCESS;
}


/**
 * @brief Sends a new alert notification to a connected peer device.
 *
 * @param[in] connHandle           The connection handle.
 * @param[in] catId                The category ID of the alert. Refer to @ref BLE_ANPS_CAT_ID for possible values.
 * @param[in] numAlert             The number of new alerts.
 * @param[in] txtStrLen            The length of the text string information. Must be less than or equal to (MTU - 5).
 * @param[in] p_txtStr             Pointer to the text string information.
 *
 * @retval MBA_RES_SUCCESS         Successfully sent the new alert notification.
 * @retval MBA_RES_OOM             Internal memory allocation failure.
 * @retval MBA_RES_NO_RESOURCE     No available buffer to transmit the new alert notification.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters.
 */
uint16_t BLE_ANPS_SendNewAlert(uint16_t connHandle, uint8_t catId, uint8_t numAlert, uint16_t txtStrLen, const uint8_t *p_txtStr)
{
    uint16_t result = MBA_RES_OOM;
    GATTS_HandleValueParams_T *p_notiParams;

    if (p_txtStr == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    p_notiParams = (GATTS_HandleValueParams_T *)OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_notiParams != NULL)
    {
        p_notiParams->charHandle = (uint16_t)ANS_HDL_CHARVAL_NEW_ALERT;
        p_notiParams->charLength = txtStrLen + 2U;
        p_notiParams->charValue[0] = catId;
        p_notiParams->charValue[1] = numAlert; 
        (void)memcpy(&p_notiParams->charValue[2], p_txtStr, txtStrLen);
        p_notiParams->sendType = ATT_HANDLE_VALUE_NTF;

        result = GATTS_SendHandleValue(connHandle, p_notiParams);
        OSAL_Free((uint8_t *)p_notiParams);
    }
    return result;
}


/**
 * @brief Sends an unread alert status notification to a connected peer device.
 *
 *@param[in] connHandle           The connection handle.
 *@param[in] catId                The category ID of the alert. Refer to @ref BLE_ANPS_CAT_ID for possible values.
 *@param[in] unreadCnt            The count of unread alerts.
 *
 *@retval MBA_RES_SUCCESS         Successfully sent the unread alert status notification.
 *@retval MBA_RES_OOM             Internal memory allocation failure.
 *@retval MBA_RES_NO_RESOURCE     No available buffer to transmit the unread alert status notification.
 *@retval MBA_RES_INVALID_PARA    Invalid parameters.
 */
uint16_t BLE_ANPS_SendUnreadAlertStat(uint16_t connHandle, uint8_t catId, uint8_t unreadCnt)
{
    uint16_t result = MBA_RES_OOM;
    GATTS_HandleValueParams_T *p_notiParams;

    p_notiParams = (GATTS_HandleValueParams_T *)OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_notiParams != NULL)
    {
        p_notiParams->charHandle = (uint16_t)ANS_HDL_CHARVAL_UNREAD_ALERT_STAT;
        p_notiParams->charLength = 2;
        p_notiParams->charValue[0] = catId;
        p_notiParams->charValue[1] = unreadCnt; 
        p_notiParams->sendType = ATT_HANDLE_VALUE_NTF;

        result = GATTS_SendHandleValue(connHandle, p_notiParams);
        OSAL_Free((uint8_t *)p_notiParams);
    }
    return result;
}


/**
 * @brief Handles BLE_Stack events.
 * 
 * This function should be called when BLE stack events occur.
 * 
 * @param[in] p_stackEvent          Pointer to the stack event structure.
 *
*/
static void ble_anps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                BLE_ANPS_ConnList_T *p_conn;
                
                p_conn = ble_anps_GetFreeConnList();
                if(p_conn == NULL)
                {
                    ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
                    return;
                }

                p_conn->connHandle=p_event->eventField.evtConnect.connHandle;
            }
        }
        break;
        case BLE_GAP_EVT_DISCONNECTED:
        {
            BLE_ANPS_ConnList_T *p_conn;

            p_conn = ble_anps_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);

            if (p_conn == NULL)
            {
                ble_anps_ConveyEvent(BLE_ANPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
                return;
            }

            ble_anps_FreeRetryData(p_conn);
            ble_anps_InitConnList(p_conn);
        }
        break;
        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_anps_ProcessQueuedTask(p_event->eventField.evtTxBufAvailable.connHandle);
        }
        break;
        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Handle BLE_Stack events.
 *       This function should be called when BLE stack events occur in the application.
 *
 * @param[in] p_stackEvent        Pointer to the BLE stack events data.
 *
 */
void BLE_ANPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_anps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_anps_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

