/*******************************************************************************
  BLE Alert Notification Monitor Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_anpc.c

  Summary:
    This file contains the BLE Alert Notification Server functions for application user.

  Description:
    This file contains the BLE Alert Notification Server functions for application user.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

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
#include "ble_anpc/ble_anpc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

#define BLE_ANPC_UUID_ANS_SVC                0x1811
#define BLE_ANPC_UUID_ANCP                   0x2A44    /**< Alert Notification Control Point.*/
#define BLE_ANPC_UUID_UNREAD_ALERT_STAT      0x2A45    /**< Unread Alert Status Declaration.*/
#define BLE_ANPC_UUID_NEW_ALERT              0x2A46    /**< New Alert Declaration.*/
#define BLE_ANPC_UUID_SUPP_NEW_ALERT_CAT     0x2A47    /**< Supported New Alert Category Declaration.*/
#define BLE_ANPC_UUID_SUPP_UNREAD_ALERT_CAT  0x2A48    /**< Support Unread Alert Category Declaration.*/



/**@defgroup BLE_ANPC_CBFC_PROC BLE_ANPC_CBFC_PROC
 * @brief The definition of ANPC procedure in connect/disconnect process.
 * @{ */
#define BLE_ANPC_ANPC_PROC_IDLE                0x00    /**< CBFC procdure: Idle. */
#define BLE_ANPC_ANPC_PROC_ENABLE_NEW_CCCD     0x01    /**< CBFC procdure: Enable NEW CCCD. */
#define BLE_ANPC_ANPC_PROC_ENABLE_UNREAD_CCCD  0x02    /**< CBFC procdure: Enable UNREAD CCCD. */
#define BLE_ANPC_ANPC_PROC_ENABLE_SESSION      0x03    /**< CBFC procdure: Enable Control Point CCCD. */
/** @} */

/**@defgroup BLE_ANPC_MAX_CONN_NBR BLE_ANPC_MAX_CONN_NBR
 * @brief The definition of Memory size.
 * @{ */
#define BLE_ANPC_MAX_CONN_NBR       BLE_GAP_MAX_LINK_NBR    /**< Maximum allowing Conncetion Numbers for MBADK. */
/** @} */

typedef enum BLE_ANPC_CharAlertNotiIndex_T
{
    ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT,
    ANPC_INDEX_CHARAN_NEW_ALERT,
    ANPC_INDEX_CHARAN_NEW_ALERT_CCC,
    ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT,
    ANPC_INDEX_CHARAN_UNREAD_ALERT_STAT,
    ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC,
    ANPC_INDEX_CHARAN_ANCP,
    ANPC_CHARAN_CHAR_NUM
} BLE_ANPC_CharAlertNotiIndex_T;

typedef struct BLE_ANPC_ConnList_T
{
    int8_t      connIndex;
    uint16_t    connHandle;
} BLE_ANPC_ConnList_T;

static BLE_ANPC_EventCb_T       sp_anpcCbRoutine;
static BLE_ANPC_ConnList_T      s_anpcConnList[BLE_ANPC_MAX_CONN_NBR];
static BLE_DD_CharInfo_T        s_anpcAnsCharInfoList[BLE_ANPC_MAX_CONN_NBR][ANPC_CHARAN_CHAR_NUM];

static const ATT_Uuid_T anpcDiscCharSuppNewAlertCat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_SUPP_NEW_ALERT_CAT) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharNewAlert = { { UINT16_TO_BYTES(BLE_ANPC_UUID_NEW_ALERT) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharNewAlertCcc = { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharSuppUnreadAlertCat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_SUPP_UNREAD_ALERT_CAT) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharUnreadAlertStat = { { UINT16_TO_BYTES(BLE_ANPC_UUID_UNREAD_ALERT_STAT) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharUnreadAlertCcc = { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) }, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T anpcDiscCharAncp = { { UINT16_TO_BYTES(BLE_ANPC_UUID_ANCP) }, ATT_UUID_LENGTH_2 };

static BLE_DD_DiscChar_T   anpcSuppNewAlertCat = { &anpcDiscCharSuppNewAlertCat, 0 };
static BLE_DD_DiscChar_T   anpcNewAlert = { &anpcDiscCharNewAlert, 0 };
static BLE_DD_DiscChar_T   anpcNewAlertCcc = { &anpcDiscCharNewAlertCcc, CHAR_SET_DESCRIPTOR };
static BLE_DD_DiscChar_T   anpcSuppUnreadAlertCat = { &anpcDiscCharSuppUnreadAlertCat, 0 };
static BLE_DD_DiscChar_T   anpcUnreadAlertStat = { &anpcDiscCharUnreadAlertStat, 0 };
static BLE_DD_DiscChar_T   anpcUnreadAlertCcc = { &anpcDiscCharUnreadAlertCcc, CHAR_SET_DESCRIPTOR };
static BLE_DD_DiscChar_T   anpcAncp = { &anpcDiscCharAncp, 0 };

static BLE_DD_DiscChar_T   *anpcDiscAlertNotiCharList[] =
{
    &anpcSuppNewAlertCat,     /* Supported New Alert Category. */
    &anpcNewAlert,            /* New Alert. */
    &anpcNewAlertCcc,         /* New Alert Client Characteristic Configuration. */
    &anpcSuppUnreadAlertCat,  /* Supported Unread Alert Category. */
    &anpcUnreadAlertStat,     /* Unread Alert Status. */
    &anpcUnreadAlertCcc,      /* Unread Alert Client Characteristic Configuration. */
    &anpcAncp                 /* Alert Notification Control Point. */
}; 
static const uint8_t anpcDiscAnsUuid[] = { UINT16_TO_BYTES(BLE_ANPC_UUID_ANS_SVC) };
static BLE_DD_CharList_T s_anpcAnsCharList[BLE_ANPC_MAX_CONN_NBR];
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// ***************************************************************************** 
// *****************************************************************************
static void ble_anpc_InitConnList(uint8_t connIndex)
{
    memset((uint8_t *)&s_anpcConnList[connIndex], 0, sizeof(BLE_ANPC_ConnList_T)); }

static BLE_ANPC_ConnList_T *ble_anpc_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_ANPC_MAX_CONN_NBR; i++)
    {
        if (s_anpcConnList[i].connHandle == connHandle)
        {
            return &s_anpcConnList[i];
        }
    }
    return NULL;
}

static BLE_ANPC_ConnList_T *ble_anpc_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_ANPC_MAX_CONN_NBR; i++)
    {
        if (s_anpcConnList[i].connHandle == 0)
        {
            s_anpcConnList[i].connIndex = i;
            return &s_anpcConnList[i];
        }
    }
    return NULL;
}

static void ble_anpc_ConveyEvent(BLE_ANPC_Event_T *p_event)
{
    if(sp_anpcCbRoutine)
    {
        sp_anpcCbRoutine(p_event);
    }
}

static void ble_anpc_InitAnsCharList(uint8_t connIndex)
{
    memset(&s_anpcAnsCharList[connIndex], 0x0, sizeof(BLE_DD_CharList_T));
    memset(s_anpcAnsCharInfoList[connIndex], 0x0, sizeof(BLE_DD_CharInfo_T) * ANPC_CHARAN_CHAR_NUM);
    s_anpcAnsCharList[connIndex].p_charInfo = s_anpcAnsCharInfoList[connIndex];
}

static uint16_t ble_anpc_InitAlertNotiSvcDiscRegister(const uint8_t * p_uuid, BLE_DD_CharList_T * p_charList)
{
    BLE_DD_DiscSvc_T anpDisc;

    anpDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_2;
    memcpy(anpDisc.svcUuid.uuid, p_uuid, ATT_UUID_LENGTH_2);
    anpDisc.p_discChars = anpcDiscAlertNotiCharList;
    anpDisc.p_charList = p_charList;
    anpDisc.discCharsNum = ANPC_CHARAN_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&anpDisc);
}

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
    charHandle = s_anpcAnsCharList[p_conn->connIndex].p_charInfo[index].charHandle;
    if (charHandle == 0)
    {
        return MBA_RES_FAIL;
    }
    return GATTC_Read(connHandle, charHandle, 0);
}
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

    charHandle = s_anpcAnsCharInfoList[p_conn->connIndex][index].charHandle;
    if (charHandle == 0)
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

static void ble_anpc_ProcNotificationInd(GATT_Event_T *p_event)
{
    uint8_t *p_value = p_event->eventField.onNotification.receivedValue;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onNotification.connHandle);
    BLE_ANPC_Event_T evt;

    if(p_conn == NULL)
    {
        evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
        ble_anpc_ConveyEvent(&evt);
        return;
    } 
    
    if (p_event->eventField.onNotification.charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT].charHandle)
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
        evt.eventField.evtNewAlertInd.receivedLength = (rcvLength - 2);

        STREAM_TO_U8(&evt.eventField.evtNewAlertInd.categoryId, &p_value);
        STREAM_TO_U8(&evt.eventField.evtNewAlertInd.numOfNewAlert, &p_value);
        STREAM_COPY_TO_VARIABLE(evt.eventField.evtNewAlertInd.p_receivedValue, &p_value , evt.eventField.evtNewAlertInd.receivedLength);

        ble_anpc_ConveyEvent(&evt);

        OSAL_Free(evt.eventField.evtNewAlertInd.p_receivedValue);
    }
    else if (p_event->eventField.onNotification.charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_STAT].charHandle)
    {

        evt.eventId = BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND;

        evt.eventField.evtUnreadAlertStatInd.connHandle = p_event->eventField.onNotification.connHandle;
        evt.eventField.evtUnreadAlertStatInd.categoryId  = p_value[0];
        evt.eventField.evtUnreadAlertStatInd.unreadCnt   = p_value[1];

        ble_anpc_ConveyEvent(&evt);
    }
}

static void ble_anpc_ProcReadResponse(GATT_Event_T *p_event)
{
    uint8_t *p_value = p_event->eventField.onReadResp.readValue;
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onReadResp.connHandle);
    BLE_ANPC_Event_T evt;

    if(p_conn == NULL)
    {
        evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
        ble_anpc_ConveyEvent(&evt);
        return;
    } 
    if (p_event->eventField.onReadResp.charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND;
        evt.eventField.evtSuppNewAlertCatInd.connHandle = p_event->eventField.onReadResp.connHandle;
        STREAM_LE_TO_U16(&evt.eventField.evtSuppNewAlertCatInd.category, &p_value);

        ble_anpc_ConveyEvent(&evt);
    }
    else if (p_event->eventField.onReadResp.charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND;
        evt.eventField.evtSuppUnreadAlertCatInd.connHandle = p_event->eventField.onReadResp.connHandle;
        STREAM_LE_TO_U16(&evt.eventField.evtSuppUnreadAlertCatInd.category, &p_value);

        ble_anpc_ConveyEvent(&evt);
    }

}

static void ble_anpc_ProcWriteResponse(GATT_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onWriteResp.connHandle);
    BLE_ANPC_Event_T evt;
    uint16_t charHandle = p_event->eventField.onWriteResp.charHandle;

    if(p_conn == NULL)
    {
        evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
        ble_anpc_ConveyEvent(&evt);
        return;
    }

    if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteNewAlertRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteNewAlertRspInd.errCode = 0x00;
    }
    else if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteUnreadAlertRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteUnreadAlertRspInd.errCode = 0x00;
    }
    else if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_CP_RSP_IND;
        evt.eventField.evtWriteCpRspInd.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.eventField.evtWriteCpRspInd.errCode = 0x00;
    }
    else
    {
        evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
        ble_anpc_ConveyEvent(&evt);
        return;
    }
    ble_anpc_ConveyEvent(&evt);
}

static void ble_anpc_ProcErrorResponse(GATT_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = ble_anpc_GetConnListByHandle(p_event->eventField.onError.connHandle);
    BLE_ANPC_Event_T evt;
    uint16_t charHandle = p_event->eventField.onError.attrHandle;

    if (p_conn == NULL)
    {
        evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
        ble_anpc_ConveyEvent(&evt);
        return;
    }

    if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_NEW_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteNewAlertRspInd.connHandle = p_event->eventField.onError.connHandle;
        evt.eventField.evtWriteNewAlertRspInd.errCode = p_event->eventField.onError.errCode;
    }
    else if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC].charHandle)
    {
        evt.eventId = BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND;
        evt.eventField.evtWriteUnreadAlertRspInd.connHandle = p_event->eventField.onError.connHandle;
        evt.eventField.evtWriteUnreadAlertRspInd.errCode = p_event->eventField.onError.errCode;
    }
    else if (charHandle == s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle)
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

static void ble_anpc_InitConnCharList(uint8_t connIndex)
{
    ble_anpc_InitConnList(connIndex);
    ble_anpc_InitAnsCharList(connIndex);
}
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
        break;
    }
}

static void ble_anpc_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_ANPC_ConnList_T *p_conn = NULL;
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                p_conn = ble_anpc_GetFreeConnList();
                if (p_conn != NULL)
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
                ble_anpc_InitConnCharList(p_conn->connIndex);
            }
        }
        break;

        default:
        break;
    }
}

uint16_t BLE_ANPC_Init(void)
{
    uint8_t i;
    sp_anpcCbRoutine = NULL;

    for(i = 0; i < BLE_ANPC_MAX_CONN_NBR; i++)
    {
        ble_anpc_InitConnCharList(i);
    }
    if(ble_anpc_InitAlertNotiSvcDiscRegister(anpcDiscAnsUuid, s_anpcAnsCharList) != MBA_RES_SUCCESS)
        return MBA_RES_FAIL;

    return MBA_RES_SUCCESS;
}

void BLE_ANPC_EventRegister(BLE_ANPC_EventCb_T routine)
{
    sp_anpcCbRoutine = routine;
}

uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle)
{
    return ble_anpc_ReadAlertCat(connHandle, ANPC_INDEX_CHARAN_SUPP_NEW_ALERT_CAT);
}

uint16_t BLE_ANPC_ReadSuppUnreadAlertCat(uint16_t connHandle)
{
    return ble_anpc_ReadAlertCat(connHandle, ANPC_INDEX_CHARAN_SUPP_UNREAD_ALERT_CAT);
}

uint16_t BLE_ANPC_EnableNewAlertNtfy(uint16_t connHandle, bool enable)
{
    return ble_anpc_WriteAlertDataCccd(connHandle, enable, ANPC_INDEX_CHARAN_NEW_ALERT_CCC);
}

uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable)
{
    return ble_anpc_WriteAlertDataCccd(connHandle, enable, ANPC_INDEX_CHARAN_UNREAD_ALERT_CCC);
}

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
        result = MBA_RES_OOM;
    }
    charHandle = s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle;
    if (charHandle == 0)
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
                evt.eventId = BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND;
                ble_anpc_ConveyEvent(&evt);
                return;
            } 
            if (s_anpcAnsCharList[p_conn->connIndex].p_charInfo[ANPC_INDEX_CHARAN_ANCP].charHandle)
            {
                evt.eventId = BLE_ANPC_EVT_DISC_COMPLETE_IND;
                evt.eventField.evtDiscComplete.connHandle = p_event->eventField.evtDiscResult.connHandle;
                ble_anpc_ConveyEvent(&evt);
            }
        } 
        break;

        default:
        break;
    }
}

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
        break;
    }
}

