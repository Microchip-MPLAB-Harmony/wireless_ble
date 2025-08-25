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
  BLE Alert Notification Monitor Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_anpc.c

  Summary:
    Implements the client-side logic for the Alert Notification Service (ANS) over BLE.

  Description:
    This source file provides the necessary functions to interface with the Alert
    Notification Service as a client in a BLE application. It facilitates the
    initiation, handling, and termination of alert notifications from a BLE server.
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
#include "ble_util/byte_stream.h"
#include "ble_anpc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_ANPC_UUID_ANS_SVC                   (0x1811U)     // UUID for the Alert Notification Service. 

#define BLE_ANPC_PROC_IDLE                      (0x00U)       // ANPC procedure is idle.
#define BLE_ANPC_PROC_ENABLE_NEW_CCCD           (0x01U)       // Enable notification for new incoming alerts.
#define BLE_ANPC_PROC_ENABLE_UNREAD_CCCD        (0x02U)       // Enable notification for unread alert status.
#define BLE_ANPC_PROC_ENABLE_SESSION            (0x03U)       // Enable notification for control point changes.

#define BLE_ANPC_MAX_CONN_NBR                   BLE_GAP_MAX_LINK_NBR    // Maximum number of concurrent connections supported.

typedef enum BLE_ANPC_CharAlertNotiIndex_T
{
    ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT = 0U,                // Index for the Supported New Alert Category characteristic.
    ANPC_INDEX_CHARAN_NEW_ALERT,                              // Index for the New Alert characteristic.
    ANPC_INDEX_CHARAN_NEW_ALERT_CCC,                          // Index for the New Alert CCCD (Client Characteristic Configuration Descriptor).
    ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT,                  // Index for the Supported Unread Alert Category characteristic.                 
    ANPC_INDEX_CHARAN_UNREAD_ALERT_STAT,                      // Index for the Unread Alert Status characteristic.
    ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC,                       // Index for the Unread Alert Status CCCD.
    ANPC_INDEX_CHARAN_ANCP,                                   // Index for the Alert Notification Control Point characteristic.
    ANPC_INDEX_CHARAN_MAX_NUM                                 // Total number of characteristics in the ANPC .
} BLE_ANPC_CharAlertNotiIndex_T;


typedef enum BLE_ANPC_State_T
{
    BLE_ANPC_STATE_IDLE = 0x00U,                              // State indicating the service is idle.
    BLE_ANPC_STATE_CONNECTED                                  // State indicating the service is connected.
} BLE_ANPC_State_T;
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* The structure contains information about BLE ANPC profile connection parameters for recording connection information. */
typedef struct BLE_ANPC_ConnList_T
{
    uint8_t           connIndex;  // Connection index associated with this connection.
    BLE_ANPC_State_T  state;      // State associated with this connection.
    uint16_t          connHandle; // Connection handle associated with this connection.
} BLE_ANPC_ConnList_T;

/* The Structure service database and discovery list for BLE ANPC. */
typedef struct BLE_ANPC_AnsServiceDb_T
{
    BLE_DD_CharList_T anpcAnsCharList[BLE_ANPC_MAX_CONN_NBR];
    BLE_DD_DiscInfo_T anpcAnsDiscInfo[BLE_ANPC_MAX_CONN_NBR];
    BLE_DD_CharInfo_T anpcAnsCharInfoList[BLE_ANPC_MAX_CONN_NBR][ANPC_INDEX_CHARAN_MAX_NUM];
    BLE_DD_DiscChar_T anpcAnsDiscCharList[ANPC_INDEX_CHARAN_MAX_NUM];
    BLE_DD_DiscChar_T *p_anpcAnsDiscCharList[ANPC_INDEX_CHARAN_MAX_NUM];
} BLE_ANPC_AnsServiceDb_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback routine for ANPC (Alert Notification Profile Client) events.
static BLE_ANPC_EventCb_T   s_anpcCbRoutine;

// An array to keep track of the connection list for ANPC.
static BLE_ANPC_ConnList_T  *sp_anpcConnList[BLE_ANPC_MAX_CONN_NBR];

// List of pointers to the discovery information for ANS characteristics and descriptors.
static BLE_ANPC_AnsServiceDb_T *sp_anpcAnsServiceDb;

// UUID for the Supported New Alert Category characteristic in the Alert Notification Profile (ANP).
static const ATT_Uuid_T s_anpcDiscCharSuppNewAlertCat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_SUPP_NEW_ALERT_CAT) }, ATT_UUID_LENGTH_2 };

// UUID for the New Alert characteristic in the ANP.
static const ATT_Uuid_T s_anpcDiscCharNewAlert = { { UINT16_TO_BYTES(BLE_ANPC_UUID_NEW_ALERT) }, ATT_UUID_LENGTH_2 };

// UUID for the Client Characteristic Configuration Descriptor (CCCD) of the New Alert characteristic.
static const ATT_Uuid_T s_anpcDiscCharNewAlertCcc = { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) }, ATT_UUID_LENGTH_2 };

// UUID for the Supported Unread Alert Category characteristic in the ANP.
static const ATT_Uuid_T s_anpcDiscCharSuppUnreadAlertCat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_SUPP_UNREAD_ALERT_CAT) }, ATT_UUID_LENGTH_2 };

// UUID for the Unread Alert Status characteristic in the ANP.
static const ATT_Uuid_T s_anpcDiscCharUnreadAlertStat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_UNREAD_ALERT_STAT) }, ATT_UUID_LENGTH_2 };

// UUID for the CCCD of the Unread Alert Status characteristic.
static const ATT_Uuid_T s_anpcDiscCharUnreadAlertCcc = { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) }, ATT_UUID_LENGTH_2 };

// UUID for the Alert Notification Control Point (ANCP) characteristic in the ANP.
static const ATT_Uuid_T s_anpcDiscCharAncp = { { UINT16_TO_BYTES(BLE_ANPC_UUID_ANCP) }, ATT_UUID_LENGTH_2 };

// UUID for the Alert Notification Service (ANS).
static const uint8_t s_anpcDiscAnsUuid[] = { UINT16_TO_BYTES(BLE_ANPC_UUID_ANS_SVC) };
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// ***************************************************************************** 
// *****************************************************************************

/**
 * @brief Retrieves a pointer to the connection list entry by connection handle.
 * 
 * @param connHandle            The handle of the connection.
 * 
 * @retval BLE_ANPC_ConnList_T* Pointer to the connection list entry or NULL if not found.
 */
static BLE_ANPC_ConnList_T *ble_anpc_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_ANPC_MAX_CONN_NBR; i++)
    {
        if ((sp_anpcConnList[i] != NULL) && (sp_anpcConnList[i]->state == BLE_ANPC_STATE_CONNECTED) && (sp_anpcConnList[i]->connHandle == connHandle))
        {
            return sp_anpcConnList[i];
        }
    }
    return NULL;
}


/**
 * @brief Get a free connection list entry for the ANPC.
 *
 * @retval Pointer to the ANPC connection list structure, or NULL if no free entry is available.
 */
static BLE_ANPC_ConnList_T *ble_anpc_GetFreeConnList(void)
{
    uint8_t i;
    BLE_ANPC_ConnList_T *p_conn = NULL;

    for(i = 0; i < BLE_ANPC_MAX_CONN_NBR; i++)
    {
        if (sp_anpcConnList[i] == NULL)
        {
            sp_anpcConnList[i] = OSAL_Malloc(sizeof(BLE_ANPC_ConnList_T));
            p_conn = sp_anpcConnList[i];
            if (p_conn != NULL)
            {
                (void)memset(p_conn, 0, sizeof(BLE_ANPC_ConnList_T));
                p_conn->state     = BLE_ANPC_STATE_CONNECTED;
                p_conn->connIndex = i;
            }
            break;
        }
    }
    return p_conn;
}

/**
 * @brief Conveys an event to the registered callback routine.
 * 
 * @param p_event       Pointer to the event to be conveyed.
 */
static void ble_anpc_ConveyEvent(BLE_ANPC_Event_T *p_event)
{
    if(s_anpcCbRoutine != NULL)
    {
        s_anpcCbRoutine(p_event);
    }
}

/**
 * @brief Initializes the Alert Notification Service (ANS) characteristic list to zero.
 * 
 * @param connIndex     Index of the connection in the ANS characteristic list.
 */
static void ble_anpc_InitAnsCharList(uint8_t connIndex)
{
    (void)memset(&sp_anpcAnsServiceDb->anpcAnsDiscInfo[connIndex], 0x0, sizeof(BLE_DD_DiscInfo_T));
    (void)memset(&sp_anpcAnsServiceDb->anpcAnsCharList[connIndex], 0x0, sizeof(BLE_DD_CharList_T));
    (void)memset(sp_anpcAnsServiceDb->anpcAnsCharInfoList[connIndex], 0x0, sizeof(BLE_DD_CharInfo_T)*ANPC_INDEX_CHARAN_MAX_NUM);
    sp_anpcAnsServiceDb->anpcAnsCharList[connIndex].p_charInfo = sp_anpcAnsServiceDb->anpcAnsCharInfoList[connIndex];
}


/**
 * @brief Reads the Alert Category characteristic value from the ANS.
 * 
 * @param connHandle    The handle of the connection.
 * @param index         Index of the Alert Notification characteristic.
 * 
 * @retval uint16_t     Result of the read operation.
 */
static uint16_t ble_anpc_ReadAlertCat(uint16_t connHandle, BLE_ANPC_CharAlertNotiIndex_T index)
{
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(connHandle);
    uint16_t charHandle;

    if(p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    } 
    if ((index != ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT) && (index != ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT))
    {
        return MBA_RES_INVALID_PARA;
    }
    charHandle = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[index].charHandle;
    if (charHandle == 0U)
    {
        return MBA_RES_FAIL;
    }
    return GATTC_Read(connHandle, charHandle, 0);
}


/**
 * @brief Writes to the Alert Data Client Characteristic Configuration Descriptor (CCCD) to enable or disable notifications.
 * 
 * @param connHandle    The handle of the connection.
 * @param enable        Boolean value to enable or disable notifications.
 * @param index         Index of the Alert Notification characteristic.
 * 
 * @retval uint16_t Result of the write operation.
 */
static uint16_t ble_anpc_WriteAlertDataCccd(uint16_t connHandle, bool enable, BLE_ANPC_CharAlertNotiIndex_T index)
{
    GATTC_WriteParams_T *p_writeParams;
    BLE_ANPC_ConnList_T *p_conn;
    uint16_t charHandle, result;
    uint16_t cccValue = 0;

    p_conn = ble_anpc_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    charHandle = sp_anpcAnsServiceDb->anpcAnsCharInfoList[p_conn->connIndex][index].charHandle;
    if (charHandle == 0U)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (enable)
    {
        cccValue = NOTIFICATION;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams == NULL)
    {
        return MBA_RES_OOM;
    }

    p_writeParams->charHandle = charHandle;
    p_writeParams->charLength = 0x02;
    U16_TO_BUF_LE(p_writeParams->charValue, cccValue);
    p_writeParams->writeType = ATT_WRITE_REQ;
    p_writeParams->valueOffset = 0x0000;
    p_writeParams->flags = 0;

    result = GATTC_Write(connHandle, p_writeParams);
    OSAL_Free(p_writeParams);
    return result;
}


/**
 * @brief Processes notification or indication received from the GATT server.
 * 
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anpc_ProcNotificationInd(GATT_Event_T *p_event)
{
    uint8_t *p_value = p_event->eventField.onNotification.receivedValue;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onNotification.connHandle);
    BLE_ANPC_Event_T evt;

    if(p_conn == NULL)
    {
        return;
    }

    if (p_event->eventField.onNotification.charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT].charHandle)
    {
        uint16_t rcvLength = p_event->eventField.onNotification.receivedLength;
        evt.eventId = BLE_ANPC_EVT_NEW_ALERT_IND;
        evt.eventField.evtNewAlertInd.p_receivedValue = OSAL_Malloc(rcvLength);
        if (evt.eventField.evtNewAlertInd.p_receivedValue == NULL) 
        {
            evt.eventId = BLE_ANPC_EVT_ERR_NO_MEM_IND;
            ble_anpc_ConveyEvent(&evt);
            return;
        }
        evt.eventField.evtNewAlertInd.connHandle = p_event->eventField.onNotification.connHandle;
        evt.eventField.evtNewAlertInd.receivedLength = (rcvLength - 2U);

        STREAM_TO_U8(&evt.eventField.evtNewAlertInd.categoryId, &p_value);
        STREAM_TO_U8(&evt.eventField.evtNewAlertInd.numOfNewAlert, &p_value);
        STREAM_COPY_TO_VARIABLE(evt.eventField.evtNewAlertInd.p_receivedValue, &p_value , evt.eventField.evtNewAlertInd.receivedLength);

        ble_anpc_ConveyEvent(&evt);

        OSAL_Free(evt.eventField.evtNewAlertInd.p_receivedValue);
    }
    else if (p_event->eventField.onNotification.charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_STAT].charHandle)
    {

        evt.eventId = BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND;

        evt.eventField.evtUnreadAlertStatInd.connHandle = p_event->eventField.onNotification.connHandle;
        evt.eventField.evtUnreadAlertStatInd.categoryId  = p_value[0];
        evt.eventField.evtUnreadAlertStatInd.unreadCnt   = p_value[1];

        ble_anpc_ConveyEvent(&evt);
    }
    else
    {
		//Shall not enter here
    }
}


/**
 * @brief Processes the read response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anpc_ProcReadResponse(GATT_Event_T *p_event)
{
    uint8_t *p_value = p_event->eventField.onReadResp.readValue;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onReadResp.connHandle);
    BLE_ANPC_Event_T evt;

    if(p_conn == NULL)
    {
        return;
    }
    if (p_event->eventField.onReadResp.charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND;
        evt.eventField.evtSuppNewAlertCatInd.connHandle = p_event->eventField.onReadResp.connHandle;
        STREAM_LE_TO_U16(&evt.eventField.evtSuppNewAlertCatInd.category, &p_value);

        ble_anpc_ConveyEvent(&evt);
    }
    else if (p_event->eventField.onReadResp.charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND;
        evt.eventField.evtSuppUnreadAlertCatInd.connHandle = p_event->eventField.onReadResp.connHandle;
        STREAM_LE_TO_U16(&evt.eventField.evtSuppUnreadAlertCatInd.category, &p_value);

        ble_anpc_ConveyEvent(&evt);
    }
    else
    {
		//Shall not enter here
    }

}


/**
 * @brief Processes the write response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anpc_ProcWriteResponse(GATT_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onWriteResp.connHandle);
    BLE_ANPC_Event_T evt;
    uint16_t charHandle = p_event->eventField.onWriteResp.charHandle;

    if(p_conn == NULL)
    {
        return;
    }

    if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteNewAlertRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteNewAlertRspInd.errCode = 0x00;
    }
    else if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteUnreadAlertRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteUnreadAlertRspInd.errCode = 0x00;
    }
    else if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_CP_RSP_IND;
        evt.eventField.evtWriteCpRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteCpRspInd.errCode = 0x00;
    }
    else
    {
        return;
    }
    ble_anpc_ConveyEvent(&evt);
}


/**
 * @brief Processes the error response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anpc_ProcErrorResponse(GATT_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onError.connHandle);
    BLE_ANPC_Event_T evt;
    uint16_t charHandle = p_event->eventField.onError.attrHandle;

    if (p_conn == NULL)
    {
        return;
    }

    if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteNewAlertRspInd.connHandle = p_event->eventField.onError.connHandle;
        evt.eventField.evtWriteNewAlertRspInd.errCode = p_event->eventField.onError.errCode;
    }
    else if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteUnreadAlertRspInd.connHandle = p_event->eventField.onError.connHandle;
        evt.eventField.evtWriteUnreadAlertRspInd.errCode = p_event->eventField.onError.errCode;
    }
    else if (charHandle == sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_CP_RSP_IND;
        evt.eventField.evtWriteCpRspInd.connHandle = p_event->eventField.onError.connHandle;
        evt.eventField.evtWriteCpRspInd.errCode = p_event->eventField.onError.errCode;
    }
    else
    {
        return;
    }
    ble_anpc_ConveyEvent(&evt);
}

/**
 * @brief Free the connection list for the ANPC.
 *
 * @param p_conn        Pointer to the ANPC connection list structure to initialize.
 * @param disconnect    Flag indicating whether to disconnect.
 */
static void ble_anpc_FreeConnList(BLE_ANPC_ConnList_T *p_conn)
{
    uint8_t i;
    for (i = 0; i < BLE_ANPC_MAX_CONN_NBR; i++)
    {
        if (sp_anpcConnList[i] == p_conn)
        {
            OSAL_Free(sp_anpcConnList[i]);
            sp_anpcConnList[i] = NULL;
            break;
        }
    }
}

/**
 * @brief Processes GATT events and dispatch them to the appropriate handler.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_anpc_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTC_EVT_HV_NOTIFY:
        {
            ble_anpc_ProcNotificationInd(p_event);
        }
        break;

        case GATTC_EVT_READ_RESP:
        {
            ble_anpc_ProcReadResponse(p_event);
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            ble_anpc_ProcWriteResponse(p_event);
        }
        break;

        case GATTC_EVT_ERROR_RESP:
        {
            ble_anpc_ProcErrorResponse(p_event);
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
 * @brief Processes GAP events and handle connection-related events.
 *
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_anpc_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = NULL;
    BLE_ANPC_Event_T evt;
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if (p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS)
            {
                p_conn = ble_anpc_GetFreeConnList();
                if (p_conn == NULL)
				{
                    evt.eventId = BLE_ANPC_EVT_ERR_NO_MEM_IND;
                    ble_anpc_ConveyEvent(&evt);
                }
                else
                {
                    p_conn->connHandle = p_event->eventField.evtConnect.connHandle;
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);
            if (p_conn != NULL)
            {
                ble_anpc_FreeConnList(p_conn);
            }
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
 * @brief Initializes BLE Alert Notification Client.
 *
 * @retval MBA_RES_SUCCESS          The client was successfully initialized.
 * @retval MBA_RES_FAIL             The client failed to initialize.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */

uint16_t BLE_ANPC_Init(void)
{
    uint8_t i;
    uint16_t ret;
    BLE_DD_DiscSvc_T anpDisc;

    if (sp_anpcAnsServiceDb)
    {
        return MBA_RES_FAIL;
    }
    sp_anpcAnsServiceDb = (BLE_ANPC_AnsServiceDb_T*)OSAL_Malloc(sizeof(BLE_ANPC_AnsServiceDb_T));
    if (sp_anpcAnsServiceDb == NULL)
    {
        return MBA_RES_OOM;
    }

    (void)memset(sp_anpcAnsServiceDb->anpcAnsDiscCharList, 0x00, sizeof(BLE_DD_DiscChar_T)*ANPC_INDEX_CHARAN_MAX_NUM);
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT].p_uuid    = &s_anpcDiscCharSuppNewAlertCat;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_NEW_ALERT].p_uuid             = &s_anpcDiscCharNewAlert;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].p_uuid         = &s_anpcDiscCharNewAlertCcc;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].settings       = CHAR_SET_DESCRIPTOR;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT].p_uuid = &s_anpcDiscCharSuppUnreadAlertCat;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_UNREAD_ALERT_STAT].p_uuid     = &s_anpcDiscCharUnreadAlertStat;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].p_uuid      = &s_anpcDiscCharUnreadAlertCcc;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].settings    = CHAR_SET_DESCRIPTOR;
    sp_anpcAnsServiceDb->anpcAnsDiscCharList[ANPC_INDEX_CHARAN_ANCP].p_uuid                  = &s_anpcDiscCharAncp;

    for(i = 0; i < ANPC_INDEX_CHARAN_MAX_NUM; i++)
    {
        sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[i] = &sp_anpcAnsServiceDb->anpcAnsDiscCharList[i];
    }

    for(i = 0; i < BLE_ANPC_MAX_CONN_NBR; i++)
    {
        ble_anpc_InitAnsCharList(i);
    }
    anpDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_2;
    (void)memcpy(anpDisc.svcUuid.uuid, s_anpcDiscAnsUuid, ATT_UUID_LENGTH_2);
    anpDisc.p_discInfo   = sp_anpcAnsServiceDb->anpcAnsDiscInfo;
    anpDisc.p_discChars  = sp_anpcAnsServiceDb->p_anpcAnsDiscCharList;
    anpDisc.p_charList   = sp_anpcAnsServiceDb->anpcAnsCharList;
    anpDisc.discCharsNum = (uint8_t)ANPC_INDEX_CHARAN_MAX_NUM;

    ret = BLE_DD_ServiceDiscoveryRegister(&anpDisc);

    if (ret != MBA_RES_SUCCESS)
    {
        OSAL_Free(sp_anpcAnsServiceDb);
        sp_anpcAnsServiceDb = NULL;
    }
    return ret;
}


/**
 * @brief Registers a callback function for the BLE Alert Notification Client events.
 *
 * @param[in] routine               The callback function to handle client events.
 */
void BLE_ANPC_EventRegister(BLE_ANPC_EventCb_T routine)
{
    s_anpcCbRoutine = routine;
}


/**
 * @brief Requests the value of the Supported New Alert Category from the server.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 *
 * @retval MBA_RES_SUCCESS          The read request was successfully issued.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_FAIL             The read request failed to issue.
 */
uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle)
{
    return ble_anpc_ReadAlertCat(connHandle, ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT);
}


/**
 * @brief Requests the value of the Supported Unread Alert Category from the server.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 *
 * @retval MBA_RES_SUCCESS          The read request was successfully issued.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_FAIL             The read request failed to issue.
 */
uint16_t BLE_ANPC_ReadSuppUnreadAlertCat(uint16_t connHandle)
{
    return ble_anpc_ReadAlertCat(connHandle, ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT);
}


/**
 * @brief Enables or disables notifications for new alerts.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] enable                Set to true to enable notifications; false to disable.
 *
 * @retval MBA_RES_SUCCESS          The operation was successful.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableNewAlertNtfy(uint16_t connHandle, bool enable)
{
    return ble_anpc_WriteAlertDataCccd(connHandle, enable, ANPC_INDEX_CHARAN_NEW_ALERT_CCC);
}


/**
 * @brief Enables or disables notifications for unread alerts.\n
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] enable                Set to true to enable notifications; false to disable.
 *
 * @retval MBA_RES_SUCCESS          The operation was successful.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable)
{
    return ble_anpc_WriteAlertDataCccd(connHandle, enable, ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC);
}


/**
 * @brief Sends a command to the Alert Notification Control Point.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] cmdId                 The command ID.
 * @param[in] catId                 The category ID.
 *
 * @retval MBA_RES_SUCCESS          The command was successfully sent.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_WriteAncp(uint16_t connHandle, uint8_t cmdId, uint8_t catId)
{
    GATTC_WriteParams_T *p_writeParams;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(connHandle);
    uint16_t charHandle, result;

    if(p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams == NULL)
    {
        return MBA_RES_OOM;
    }
    charHandle = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle;
    if (charHandle == 0U)
    {
        return MBA_RES_INVALID_PARA;
    }
    p_writeParams->charHandle = charHandle;
    p_writeParams->charLength = 0x02;
    p_writeParams->charValue[0] = cmdId;
    p_writeParams->charValue[1] = catId;
    p_writeParams->writeType = ATT_WRITE_REQ;
    p_writeParams->valueOffset = 0x0000;
    p_writeParams->flags = 0;

    result = GATTC_Write(p_conn->connHandle, p_writeParams);
    OSAL_Free(p_writeParams);

    return result;
}


/**
 * @brief Retrieves a list of discovered Alert Notification service characteristics.
 * 
 * This function should be called after the BLE_ANPC_EVT_DISC_COMPLETE_IND event.
 *
 * @param[in]  connHandle           The connection handle to identify the BLE connection.
 * @param[in]  charUuid             The UUID of the characteristic to look for.
 * @param[out] p_charList           On success, filled with the discovered characteristics info.
 *
 * @retval MBA_RES_SUCCESS          The characteristic list was successfully retrieved.
 * @retval MBA_RES_INVALID_PARA     The parameters are invalid (e.g., invalid connection handle or UUID).
 */
uint16_t BLE_ANPC_GetCharList(uint16_t connHandle, uint16_t charUuid, BLE_ANPC_CharList_T *p_charList)
{
    uint8_t             idx;
    uint16_t            desUuid;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (p_charList == NULL) ||
        (charUuid < BLE_ANPC_UUID_ANCP) ||
        (charUuid > BLE_ANPC_UUID_SUPP_UNREAD_ALERT_CAT)
       )
    {
        return MBA_RES_INVALID_PARA;
    } 
    (void)memset(p_charList, 0, sizeof(BLE_ANPC_CharList_T)); 
    for (idx = 0; idx < (uint8_t)ANPC_INDEX_CHARAN_MAX_NUM; idx++)
    {
        BUF_LE_TO_U16(&desUuid, sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[idx]->p_uuid->uuid);
        if ((desUuid == charUuid) &&
            ((sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[idx]->settings & CHAR_SET_DESCRIPTOR) != CHAR_SET_DESCRIPTOR))
        {
            p_charList->attrHandle = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[idx].attrHandle; 
            p_charList->property   = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[idx].property;
            p_charList->charHandle = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
            break;
        }
    }
    return MBA_RES_SUCCESS;
}


/**
 * @brief Retrieves a list of descriptors within the Alert Notification Service.
 * 
 * This function should be called after the BLE_ANPC_EVT_DISC_COMPLETE_IND event.
 *
 * @param[in]  connHandle           The connection handle to identify the BLE connection.
 * @param[out] p_descList           On success, filled with the discovered descriptors info.
 *
 * @retval MBA_RES_SUCCESS          The descriptor list was successfully retrieved.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 */
uint16_t BLE_ANPC_GetDescList(uint16_t connHandle, BLE_ANPC_DescList_T *p_descList)
{
    uint8_t             idx, descNum = 0;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (p_descList == NULL))
    {
        return MBA_RES_INVALID_PARA;
    } 
    (void)memset(p_descList, 0, sizeof(BLE_ANPC_DescList_T)); 
    for (idx = 0; idx < (uint8_t)ANPC_INDEX_CHARAN_MAX_NUM; idx++)
    {
        if (((sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[idx]->settings & CHAR_SET_DESCRIPTOR) != 0U) && (sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[idx].charHandle != 0U))
        {
            p_descList->descInfo[descNum].attrHandle = sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
            VARIABLE_COPY_TO_BUF(&p_descList->descInfo[descNum].uuid, sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[idx]->p_uuid->uuid, sp_anpcAnsServiceDb->p_anpcAnsDiscCharList[idx]->p_uuid->uuidLength);
            descNum++;
        }
    }
    p_descList->totalNum = descNum; 
    return MBA_RES_SUCCESS;
}


/**
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *
 * @param[in] p_event               Pointer to the BLE DD event structure.
 *
*/
void BLE_ANPC_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            BLE_ANPC_Event_T evt;
            BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);
            if(p_conn == NULL)
            {
                return;
            } 
            if (sp_anpcAnsServiceDb->anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle != 0U)
            {
                evt.eventId = BLE_ANPC_EVT_DISC_COMPLETE_IND;
                evt.eventField.evtDiscComplete.connHandle  = p_event->eventField.evtDiscResult.connHandle;
                evt.eventField.evtDiscComplete.ansStartHandle = sp_anpcAnsServiceDb->anpcAnsDiscInfo[p_conn->connIndex].svcStartHandle;
                evt.eventField.evtDiscComplete.ansEndHandle   = sp_anpcAnsServiceDb->anpcAnsDiscInfo[p_conn->connIndex].svcEndHandle;
                ble_anpc_ConveyEvent(&evt);
            }
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
 * @brief Handles BLE_Stack events.
 * 
 * This function should be called when BLE stack events occur.
 * 
 * @param[in] p_stackEvent          Pointer to the stack event structure.
 *
*/
void BLE_ANPC_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_anpc_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_anpc_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

