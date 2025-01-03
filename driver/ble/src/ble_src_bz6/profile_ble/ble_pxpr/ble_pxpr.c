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
  BLE Proximity Profile Reporter Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_pxpr.c

  Summary:
    Implements the server-side functionality of the BLE Proximity Profile.

  Description:
    This source file provides the server-side functions necessary for the
    application to handle the BLE Proximity Profile, enabling the device to
    report proximity information to a paired BLE client.
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
#include "ble_pxpr.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_PXPR_RETRY_TYPE_WRITE_RESP         (0x01U)          // Retry type for write response operations
#define BLE_PXPR_RETRY_TYPE_READ_RESP          (0x02U)          // Retry type for read response operations 
#define BLE_PXPR_RETRY_TYPE_ERR                (0x03U)          // retry type for error response operations 

#define BLE_PXPR_MAX_CONN_NBR                  BLE_GAP_MAX_LINK_NBR    // The maximum number of BLE connections that can be maintained concurrently.

typedef enum BLE_PXPR_State_T
{
    BLE_PXPR_STATE_IDLE = 0x00U,            // The BLE Proxy profile is in an idle state.
    BLE_PXPR_STATE_CONNECTED                // The BLE Proxy profile is in a connected state.
} BLE_PXPR_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef struct BLE_PXPR_ConnList_T
{
    uint16_t            connHandle;         // Connection handle associated with this connection.
    uint8_t             retryType;          // Type of retry mechanism in use, as defined in BLE_PXPR_RETRY_TYPE.
    BLE_PXPR_State_T    state;              // Current state of the connection.
    uint8_t             *p_retryData;       // Pointer to data required for retry operations.
} BLE_PXPR_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

// The alert level for the Proximity Profile's Link Loss Service (LLS).
static BLE_PXPR_AlertLevel_T s_pxprLlsAlertLevel;

// Array to keep track of the connection list 
static BLE_PXPR_ConnList_T   s_pxprConnList[BLE_PXPR_MAX_CONN_NBR];
#ifdef BLE_PXPR_TPS_ENABLE
// The Tx power level for the Proximity Profile's Tx Power Service (TPS).
static int8_t s_pxprTpsTxPowerLevel;
#endif

// Pointer to a function that will be called for Proximity Profile events.
static BLE_PXPR_EventCb_T sp_pxprCbRoutine;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Frees the retry data associated with a BLE connection.
 *
 * @param p_conn Pointer to the BLE connection list structure.
 */
static void ble_pxpr_FreeRetryData(BLE_PXPR_ConnList_T *p_conn) {
    if (p_conn->p_retryData != NULL)
    {
        OSAL_Free(p_conn->p_retryData);
        p_conn->p_retryData = NULL;
        p_conn->retryType = 0;
    }
}


/**
 * @brief Initializes a BLE connection list structure by setting its content to zero.
 *
 * @param p_conn Pointer to the BLE connection list structure to initialize.
 */
static void ble_pxpr_InitConnList(BLE_PXPR_ConnList_T *p_conn)
{
    (void)memset(p_conn, 0, sizeof(BLE_PXPR_ConnList_T));
}


/**
 * @brief Retrieves a pointer to the connection list entry for a given connection handle.
 *
 * @param connHandle The connection handle to search for in the connection list.
 * @retval Pointer to the corresponding connection list entry, or NULL if not found.
 */
static BLE_PXPR_ConnList_T * ble_pxpr_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_PXPR_MAX_CONN_NBR;i++)
    {
        if ((s_pxprConnList[i].state == BLE_PXPR_STATE_CONNECTED) && (s_pxprConnList[i].connHandle == connHandle))
        {
            return &s_pxprConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Retrieves a pointer to the first free (idle) connection list entry.
 *
 * @retval Pointer to the free connection list entry, or NULL if no free entry is available.
 */
static BLE_PXPR_ConnList_T *ble_pxpr_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_PXPR_MAX_CONN_NBR;i++)
    {
        if (s_pxprConnList[i].state == BLE_PXPR_STATE_IDLE)
        {
            s_pxprConnList[i].state = BLE_PXPR_STATE_CONNECTED;
            return &s_pxprConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Conveys a BLE event to the registered callback routine.
 *
 * @param eventId The identifier of the event.
 * @param p_eventField Pointer to the event data.
 * @param eventFieldLen Length of the event data in bytes.
 */
static void ble_pxpr_ConveyEvent(BLE_PXPR_EventId_T eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{
    if (sp_pxprCbRoutine != NULL)
    {
        BLE_PXPR_Event_T evtPara;

        evtPara.eventId = eventId;
        (void)memcpy((uint8_t *)&evtPara.eventField, p_eventField, eventFieldLen);
        sp_pxprCbRoutine(&evtPara);
    }
}


/**
 * @brief Processes a write request event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpr_ProcWrite(GATT_Event_T *p_event)
{
    switch(p_event->eventField.onWrite.attrHandle)
    {
        case (uint16_t)LLS_HDL_CHARVAL_ALERT_LEVEL:
        {
            uint8_t *p_value = p_event->eventField.onWrite.writeValue;
            BLE_PXPR_EvtAlertLevelWriteInd_T evtAlrtLvWriteInd;

            STREAM_TO_U8((uint8_t*)&s_pxprLlsAlertLevel, &p_value);

            evtAlrtLvWriteInd.connHandle = p_event->eventField.onWrite.connHandle;
            evtAlrtLvWriteInd.alertLevel = s_pxprLlsAlertLevel;
            ble_pxpr_ConveyEvent(BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND, (uint8_t *) &evtAlrtLvWriteInd, (uint8_t)sizeof(evtAlrtLvWriteInd));
        }
        break;

        #ifdef BLE_PXPR_IAS_ENABLE
        case (uint16_t)IAS_HDL_CHARVAL_ALERT_LEVEL:
        {
            uint8_t *p_value = p_event->eventField.onWrite.writeValue;
            BLE_PXPR_AlertLevel_T alertLevel;
            BLE_PXPR_EvtAlertLevelWriteInd_T evtAlrtLvWriteInd;
            
            STREAM_TO_U8((uint8_t *)&alertLevel, &p_value);

            evtAlrtLvWriteInd.connHandle = p_event->eventField.onWrite.connHandle;
            evtAlrtLvWriteInd.alertLevel = alertLevel;
            ble_pxpr_ConveyEvent(BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND, (uint8_t *) &evtAlrtLvWriteInd, (uint8_t)sizeof(evtAlrtLvWriteInd));
        }
        break;
        #endif

        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Sends a read response or an error response for a read request.
 *
 * @param p_event Pointer to the GATT event structure.
 * @param p_value Pointer to the value to be sent in the read response.
 * @param len Length of the value to be sent.
 */
static void ble_pxpr_SendReadResponse(GATT_Event_T *p_event, uint8_t *p_value, uint16_t len)
{
    GATTS_SendReadRespParams_T *p_respParams;
    GATTS_SendErrRespParams_T *p_errRespParams;
    BLE_PXPR_ConnList_T *p_conn;
    uint16_t status;

    p_conn = ble_pxpr_GetConnListByHandle(p_event->eventField.onRead.connHandle);

    if (p_conn == NULL)
    {
        ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if (p_event->eventField.onRead.readType == ATT_READ_REQ)
    {
        p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendReadRespParams_T) - (BLE_ATT_MAX_MTU_LEN - ATT_READ_RESP_HEADER_SIZE) + (uint32_t)len);
        if (p_conn->p_retryData == NULL)
        {
            ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
            return;
        }
        p_conn->retryType = BLE_PXPR_RETRY_TYPE_READ_RESP;
        p_respParams = (GATTS_SendReadRespParams_T *)p_conn->p_retryData;
        p_respParams->attrLength = len;
        p_respParams->responseType = ATT_READ_RSP;
        (void)memcpy(p_respParams->attrValue, p_value, len);
        status = GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, p_respParams);
    }
    else
    {
        p_conn->p_retryData = OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
        if (p_conn->p_retryData == NULL)
        {
            ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
            return;
        }
        p_conn->retryType = BLE_PXPR_RETRY_TYPE_ERR;
        p_errRespParams = (GATTS_SendErrRespParams_T *)p_conn->p_retryData;
        p_errRespParams->reqOpcode = p_event->eventField.onRead.readType;
        p_errRespParams->attrHandle = p_event->eventField.onRead.attrHandle;
        p_errRespParams->errorCode = ATT_ERR_REQUEST_NOT_SUPPORT;
        status = GATTS_SendErrorResponse(p_event->eventField.onRead.connHandle, p_errRespParams);
    }

    if (status == MBA_RES_SUCCESS)
    {
        ble_pxpr_FreeRetryData(p_conn);
    }
}


/**
 * @brief Processes a read request event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpr_ProcRead(GATT_Event_T *p_event)
{
    switch(p_event->eventField.onRead.attrHandle)
    {
        case (uint16_t)LLS_HDL_CHARVAL_ALERT_LEVEL:
        {
            ble_pxpr_SendReadResponse(p_event, &s_pxprLlsAlertLevel, 1);
        }
        break;

        #ifdef BLE_PXPR_TPS_ENABLE
        case (uint16_t)TPS_HDL_CHARVAL_TXPOWER_LEVEL:
        {
            ble_pxpr_SendReadResponse(p_event, (uint8_t *)&s_pxprTpsTxPowerLevel, 1);
        }
        break;
        #endif

        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Processes a queued task for a given connection handle.
 *
 * @param connHandle The connection handle for which the queued task is to be processed.
 */
static void ble_pxpr_ProcessQueuedTask(uint16_t connHandle)
{
    uint16_t status;

    BLE_PXPR_ConnList_T *p_conn;

    p_conn = ble_pxpr_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }
    switch (p_conn->retryType)
    {
        case BLE_PXPR_RETRY_TYPE_READ_RESP:
        {
            status = GATTS_SendReadResponse(p_conn->connHandle, 
                (GATTS_SendReadRespParams_T *)p_conn->p_retryData);
        }
        break;
        case BLE_PXPR_RETRY_TYPE_ERR:
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
        ble_pxpr_FreeRetryData(p_conn);
    }
}


/**
 * @brief Process GAP events for the BLE PXPR service.
 *
 * @param p_event Pointer to the BLE_GAP_Event_T structure containing the GAP event data.
 */
static void ble_pxpr_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                BLE_PXPR_ConnList_T *p_conn;
                
                p_conn = ble_pxpr_GetFreeConnList();
                if(p_conn == NULL)
                {
                    ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
                    return;
                }

                p_conn->connHandle=p_event->eventField.evtConnect.connHandle;
            }
        }
        break;
        case BLE_GAP_EVT_DISCONNECTED:
        {
            BLE_PXPR_ConnList_T *p_conn;

            p_conn = ble_pxpr_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);

            if (p_conn == NULL)
            {
                ble_pxpr_ConveyEvent(BLE_PXPR_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
                return;
            }

            ble_pxpr_FreeRetryData(p_conn);
            ble_pxpr_InitConnList(p_conn);
        }
        break;
        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_pxpr_ProcessQueuedTask(p_event->eventField.evtTxBufAvailable.connHandle);
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
 * @brief Process GATT events for the BLE PXPR service.
 *
 * @param p_event Pointer to the GATT_Event_T structure containing the GATT event data.
 */
static void ble_pxpr_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTS_EVT_WRITE:
        {
            ble_pxpr_ProcWrite(p_event);
        }
        break;

        case GATTS_EVT_READ:
        {
            ble_pxpr_ProcRead(p_event);
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
 * @brief Initializes the BLE Proximity Profile (PXP) Reporter.
 *
 * @retval MBA_RES_SUCCESS          The PXP reporter was successfully initialized.
 * @retval MBA_RES_FAIL             The PXP reporter failed to initialize.
 *
 */
uint16_t BLE_PXPR_Init(void)
{
    sp_pxprCbRoutine = NULL;

    if(BLE_LLS_Add() != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    s_pxprLlsAlertLevel = BLE_PXPR_ALERT_LEVEL_NO;

    #ifdef BLE_PXPR_IAS_ENABLE
    if(BLE_IAS_Add() != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    #endif

    #ifdef BLE_PXPR_TPS_ENABLE
    if(BLE_TPS_Add() != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    s_pxprTpsTxPowerLevel = 0;
    #endif

    return MBA_RES_SUCCESS;
}


/**
 * @brief Registers a callback function for BLE PXP Reporter events.
 *
 * @param[in] routine               The function to be called when a PXP Reporter event occurs.
 *
 */
void BLE_PXPR_EventRegister(BLE_PXPR_EventCb_T routine)
{
    sp_pxprCbRoutine = routine;
}


/**
 * @brief Sets the Alert Level for the Link Loss Service (LLS).
 *
 * @param[in] level                 The desired Alert Level to be set.
 *
 * @retval MBA_RES_SUCCESS          The alert level was successfully set.
 * @retval MBA_RES_INVALID_PARA     The provided parameters are invalid.
 */
uint16_t BLE_PXPR_SetLlsAlertLevel(BLE_PXPR_AlertLevel_T level)
{
    if(level > BLE_PXPR_ALERT_LEVEL_HIGH)
    {
        return MBA_RES_INVALID_PARA;
    }

    s_pxprLlsAlertLevel = level;
    return MBA_RES_SUCCESS;
}

#ifdef BLE_PXPR_TPS_ENABLE
/**
 * @brief Sets the Transmission Power Level for the Tx Power Service.
 *
 * @param[in] level                 The desired Tx Power Level to be set.
 *
 */
void BLE_PXPR_SetTxPowerLevel(int8_t level)
{
    s_pxprTpsTxPowerLevel = level;
}
#endif


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE Stack event data structure.
 *
*/
void BLE_PXPR_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_pxpr_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_pxpr_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

