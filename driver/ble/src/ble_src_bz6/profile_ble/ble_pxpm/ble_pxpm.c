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
  BLE Proximity Profile Monitor Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_pxpm.c

  Summary:
    Implements the server-side functionality of the BLE Proximity Profile.

  Description:
    This source file provides the server-side functions necessary for the 
    application to interface with the Bluetooth Low Energy (BLE) Proximity 
    Profile. It facilitates communication and data exchange between a BLE 
    peripheral device and a central device within a proximity threshold.
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
#include "ble_pxpm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_PXPM_MAX_CONN_NBR                  BLE_GAP_MAX_LINK_NBR    // Maximum number of connections allowed for Proximity profile monitor.

typedef enum BLE_PXPM_CharAlertLevelIndex_T
{
    PXPM_INDEX_CHARALERTLV = 0x00U,             // Index for Alert Level characteristic.
    PXPM_CHARALERTLV_CHAR_NUM                   // Total number of Alert Level characteristic indexes.
} BLE_PXPM_CharAlertLevelIndex_T;

typedef enum BLE_PXPM_CharTxPowerLevelIndex_T
{
    PXPM_INDEX_CHARTXPWRLV = 0x00U,             // Index for TX Power Level characteristic.
    PXPM_INDEX_CHARTXPWRLVCCCD,                 // Index for TX Power Level characteristic's CCCD.
    PXPM_INDEX_CHARTXPWRLVCPFD,                 // Index for TX Power Level characteristic's CPFD.
    PXPM_CHARTXPWRLV_CHAR_NUM                   // Total number of TX Power Level characteristic indexes.
} BLE_PXPM_CharTxPowerLevelIndex_T;

typedef enum BLE_PXPM_State_T
{
    BLE_PXPM_STATE_IDLE = 0x00U,                // State indicating the Proximity profile is idle.
    BLE_PXPM_STATE_CONNECTED                    // State indicating the Proximity profile is connected.
} BLE_PXPM_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* Structure to keep track of BLE connection list for Proximity Profile Monitor. */
typedef struct BLE_PXPM_ConnList_T
{
    uint8_t                 connIndex;          // Index of the BLE connection.
    BLE_PXPM_State_T        state;              // Current state of the BLE connection.
    uint16_t                connHandle;         // Connection handle for the BLE connection
} BLE_PXPM_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback routine for BLE Proximity Profile Module events.
static BLE_PXPM_EventCb_T       sp_pxpmCbRoutine;

// Connection list for BLE Proximity Profile Module.
static BLE_PXPM_ConnList_T      s_pxpmConnList[BLE_PXPM_MAX_CONN_NBR];

// Discovery information for Link Loss Service (LLS) across connections.
static BLE_DD_DiscInfo_T        s_pxpmLlsDiscInfo[BLE_PXPM_MAX_CONN_NBR];

// Characteristics information list for LLS across connections.
static BLE_DD_CharInfo_T        s_pxpmLlsCharInfoList[BLE_PXPM_MAX_CONN_NBR][PXPM_CHARALERTLV_CHAR_NUM];
#ifdef BLE_PXPM_IAS_ENABLE
// Discovery information for Immediate Alert Service (IAS) across connections.
static BLE_DD_DiscInfo_T        s_pxpmIasDiscInfo[BLE_PXPM_MAX_CONN_NBR];

// Characteristics information list for IAS across connections. 
static BLE_DD_CharInfo_T        s_pxpmIasCharInfoList[BLE_PXPM_MAX_CONN_NBR][PXPM_CHARALERTLV_CHAR_NUM];
#endif
#ifdef BLE_PXPM_TPS_ENABLE
// Discovery information for TX Power Service (TPS) across connections.
static BLE_DD_DiscInfo_T        s_pxpmTpsDiscInfo[BLE_PXPM_MAX_CONN_NBR];

// Characteristics information list for TPS across connections. 
static BLE_DD_CharInfo_T        s_pxpmTpsCharInfoList[BLE_PXPM_MAX_CONN_NBR][PXPM_CHARTXPWRLV_CHAR_NUM];
#endif

// UUID for Link Loss Service discovery.
static const uint8_t            pxpmDiscLlsUuid[] =     { UINT16_TO_BYTES(BLE_PXPM_UUID_LINKLOSS_SVC) };
#ifdef BLE_PXPM_IAS_ENABLE
// UUID for Immediate Alert Service discovery.
static const uint8_t            pxpmDiscIasUuid[] =     { UINT16_TO_BYTES(BLE_PXPM_UUID_IMMEDIATE_ALERT_SVC) };
#endif
#ifdef BLE_PXPM_TPS_ENABLE
// UUID for TX Power Service discovery.
static const uint8_t            pxpmDiscTpsUuid[] =     { UINT16_TO_BYTES(BLE_PXPM_UUID_TXPOWER_SVC) };
#endif

// UUID for TX Power Level characteristic discovery.
static const ATT_Uuid_T         pxpmDiscCharAlertLv =       { { UINT16_TO_BYTES(BLE_PXPM_UUID_ALERT_LEVEL) }, ATT_UUID_LENGTH_2 };
#ifdef BLE_PXPM_TPS_ENABLE
// UUID for TX Power Level Client Characteristic Configuration Descriptor discovery.
static const ATT_Uuid_T         pxpmDiscCharTxPwrLv =       { { UINT16_TO_BYTES(BLE_PXPM_UUID_TXPOWER_LEVEL) }, ATT_UUID_LENGTH_2 };

// UUID for TX Power Level Characteristic Presentation Format Descriptor discovery.
static const ATT_Uuid_T         pxpmDiscCharTxpwrLvCccd =   { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG) }, ATT_UUID_LENGTH_2 };

// Discovery characteristic for Alert Level.
static const ATT_Uuid_T         pxpmDiscCharTxpwrLvCpfd =   { { UINT16_TO_BYTES(UUID_DESCRIPTOR_CHAR_PRE_FORMAT) }, ATT_UUID_LENGTH_2 };
#endif

// Discovery characteristic for Alert Level.
static BLE_DD_DiscChar_T        pxpmAlertLv =       { &pxpmDiscCharAlertLv, 0 };
#ifdef BLE_PXPM_TPS_ENABLE

// Discovery characteristic for TX Power Level. 
static BLE_DD_DiscChar_T        pxpmTxPwrLv =       { &pxpmDiscCharTxPwrLv, 0 };

// Discovery characteristic for TX Power Level Client Characteristic Configuration Descriptor.
static BLE_DD_DiscChar_T        pxpmTxpwrLvCccd =   { &pxpmDiscCharTxpwrLvCccd, CHAR_SET_DESCRIPTOR };

// Discovery characteristic for TX Power Level Characteristic Presentation Format Descriptor.
static BLE_DD_DiscChar_T        pxpmTxpwrLvCpfd =   { &pxpmDiscCharTxpwrLvCpfd, CHAR_SET_DESCRIPTOR };
#endif

static BLE_DD_DiscChar_T        *pxpmDiscAlertLvCharList[] =
{
    &pxpmAlertLv,               // Alert Level Characteristic
};

#ifdef BLE_PXPM_TPS_ENABLE
static BLE_DD_DiscChar_T        *pxpmDiscTxPwrLvCharList[] =
{
    &pxpmTxPwrLv,               // TX Power Level Characteristic.
    &pxpmTxpwrLvCccd,           // TX Power Level Client Characteristic Configuration Descriptor.
    &pxpmTxpwrLvCpfd,           // TX Power Level Characteristic Presentation Format.
};
#endif

// List of characteristics for Link Loss Service across connections.
static BLE_DD_CharList_T        s_pxpmLlsCharList[BLE_PXPM_MAX_CONN_NBR];
#ifdef BLE_PXPM_IAS_ENABLE
// List of characteristics for Immediate Alert Service across connections.
static BLE_DD_CharList_T        s_pxpmIasCharList[BLE_PXPM_MAX_CONN_NBR];
#endif
#ifdef BLE_PXPM_TPS_ENABLE
// List of characteristics for TX Power Service across connections.
static BLE_DD_CharList_T        s_pxpmTpsCharList[BLE_PXPM_MAX_CONN_NBR];
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Initialize a connection list entry to default values.
 *
 * @param connIndex Index of the connection in the connection list.
 */
static void ble_pxpm_InitConnList(uint8_t connIndex)
{
    (void)memset((uint8_t *)&s_pxpmConnList[connIndex], 0, sizeof(BLE_PXPM_ConnList_T));
}


/**
 * @brief Retrieve a connection list entry by its connection handle.
 *
 * @param connHandle The handle of the connection to search for.
 * 
 * @retval Pointer to the connection list entry, or NULL if not found.
 */
static BLE_PXPM_ConnList_T *ble_pxpm_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_PXPM_MAX_CONN_NBR; i++)
    {
        if ((s_pxpmConnList[i].state == BLE_PXPM_STATE_CONNECTED) && (s_pxpmConnList[i].connHandle == connHandle))
        {
            return &s_pxpmConnList[i];
        }
    }
    return NULL;
}


/**
 * @brief Find a free entry in the connection list and mark it as connected.
 *
 * @retval Pointer to the free connection list entry, or NULL if no free entry is available.
 */
static BLE_PXPM_ConnList_T *ble_pxpm_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_PXPM_MAX_CONN_NBR; i++)
    {
        if (s_pxpmConnList[i].state == BLE_PXPM_STATE_IDLE)
        {
            s_pxpmConnList[i].state = BLE_PXPM_STATE_CONNECTED;
            s_pxpmConnList[i].connIndex = i;
            return &s_pxpmConnList[i];
        }
    }
    return NULL;
}


/**
 * @brief Convey an event to the registered callback routine.
 *
 * @param eventId The identifier of the event.
 * @param p_eventField Pointer to the event field data.
 * @param eventFieldLen Length of the event field data.
 */
static void ble_pxpm_ConveyEvent(BLE_PXPM_EventId_T eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{
    if(sp_pxpmCbRoutine != NULL)
    {
        BLE_PXPM_Event_T evtPara;

        evtPara.eventId = eventId;
        (void)memcpy((uint8_t *)&evtPara.eventField, p_eventField, eventFieldLen);
        sp_pxpmCbRoutine(&evtPara);
    }
}


/**
 * @brief Initialize the Link Loss Service (LLS) characteristic list for a specific connection.
 *
 * @param connIndex Index of the connection in the LLS characteristic list.
 */
static void ble_pxpm_InitLlsCharList(uint8_t connIndex)
{
    (void)memset(&s_pxpmLlsDiscInfo[connIndex], 0x0, sizeof(BLE_DD_DiscInfo_T));
    (void)memset((uint8_t *)&s_pxpmLlsCharList[connIndex], 0x0, sizeof(BLE_DD_CharList_T));
    (void)memset((uint8_t *)s_pxpmLlsCharInfoList[connIndex], 0x0, sizeof(BLE_DD_CharInfo_T) * (uint8_t)PXPM_CHARALERTLV_CHAR_NUM);
    s_pxpmLlsCharList[connIndex].p_charInfo = s_pxpmLlsCharInfoList[connIndex];
}

#ifdef BLE_PXPM_IAS_ENABLE
/**
 * @brief Initialize the Immediate Alert Service (IAS) characteristic list for a specific connection.
 *
 * @param connIndex Index of the connection in the IAS characteristic list.
 */
static void ble_pxpm_InitIasCharList(uint8_t connIndex)
{
    (void)memset(&s_pxpmIasDiscInfo[connIndex], 0x0, sizeof(BLE_DD_DiscInfo_T));
    (void)memset((uint8_t *)&s_pxpmIasCharList[connIndex], 0x0, sizeof(BLE_DD_CharList_T));
    (void)memset((uint8_t *)s_pxpmIasCharInfoList[connIndex], 0x0, sizeof(BLE_DD_CharInfo_T) * (uint8_t)PXPM_CHARALERTLV_CHAR_NUM);
    s_pxpmIasCharList[connIndex].p_charInfo = s_pxpmIasCharInfoList[connIndex];
}
#endif

#ifdef BLE_PXPM_TPS_ENABLE
/**
 * @brief Initialize the Tx Power Service (TPS) characteristic list for a specific connection.
 *
 * @param connIndex Index of the connection in the TPS characteristic list.
 */
static void ble_pxpm_InitTpsCharList(uint8_t connIndex)
{
    (void)memset(&s_pxpmTpsDiscInfo[connIndex], 0x0, sizeof(BLE_DD_DiscInfo_T));
    (void)memset((uint8_t *)&s_pxpmTpsCharList[connIndex], 0x0, sizeof(BLE_DD_CharList_T));
    (void)memset(s_pxpmTpsCharInfoList[connIndex], 0x0, sizeof(BLE_DD_CharInfo_T) * (uint8_t)PXPM_CHARTXPWRLV_CHAR_NUM);
    s_pxpmTpsCharList[connIndex].p_charInfo = s_pxpmTpsCharInfoList[connIndex];
}
#endif


/**
 * @brief Register the Alert Level Service discovery.
 *
 * @param p_uuid Pointer to the UUID of the service to discover.
 * @param p_charList Pointer to the characteristic list where the discovered characteristics will be stored.
 * @retval The result of the service discovery registration.
 */
static uint16_t ble_pxpm_InitAlertLvSvcDiscRegister(const uint8_t * p_uuid, BLE_DD_CharList_T * p_charList)
{
    BLE_DD_DiscSvc_T pxpDisc;

    (void)memset(&pxpDisc, 0, sizeof(BLE_DD_DiscSvc_T));
    pxpDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_2;
    (void)memcpy(pxpDisc.svcUuid.uuid, p_uuid, ATT_UUID_LENGTH_2);
    if (*(uint16_t *)pxpDisc.svcUuid.uuid == BLE_PXPM_UUID_LINKLOSS_SVC)
    {
        pxpDisc.p_discInfo = s_pxpmLlsDiscInfo;
    }
#ifdef BLE_PXPM_IAS_ENABLE
    else
    {
        pxpDisc.p_discInfo = s_pxpmIasDiscInfo;
    }
#endif
    pxpDisc.p_discChars = pxpmDiscAlertLvCharList;
    pxpDisc.p_charList = p_charList;
    pxpDisc.discCharsNum = (uint8_t)PXPM_CHARALERTLV_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&pxpDisc);
}

#ifdef BLE_PXPM_TPS_ENABLE
/**
 * @brief Initialize and register the TX Power Level Service for discovery.
 *
 * @param p_uuid Pointer to the UUID of the service to be discovered.
 * @param p_charList Pointer to the characteristic list where discovered characteristics will be stored.
 * @retval uint16_t Returns the status of the service discovery registration.
 */
static uint16_t ble_pxpm_InitTxPwrLvSvcDiscRegister(const uint8_t * p_uuid, BLE_DD_CharList_T * p_charList)
{
    BLE_DD_DiscSvc_T pxpDisc;

    pxpDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_2;
    (void)memcpy(pxpDisc.svcUuid.uuid, p_uuid, ATT_UUID_LENGTH_2);
    pxpDisc.p_discInfo = s_pxpmTpsDiscInfo;
    pxpDisc.p_discChars = pxpmDiscTxPwrLvCharList;
    pxpDisc.p_charList = p_charList;
    pxpDisc.discCharsNum = PXPM_CHARTXPWRLV_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&pxpDisc);
}
#endif


/**
 * @brief Process the completion of service discovery.
 *
 * @param p_event Pointer to the BLE discovery event structure.
 */
static void ble_pxpm_ProcDiscComplete(BLE_DD_Event_T * p_event)
{
    BLE_PXPM_EvtDiscComplete_T evtDiscCmlt;
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);
    if(p_conn == NULL)
    {
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }
    if (s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle == 0U)
    {
        return;
    }
    evtDiscCmlt.connHandle = p_event->eventField.evtDiscResult.connHandle;
    evtDiscCmlt.llsStartHandle = s_pxpmLlsDiscInfo[p_conn->connIndex].svcStartHandle;
    evtDiscCmlt.llsEndHandle   = s_pxpmLlsDiscInfo[p_conn->connIndex].svcEndHandle;

#ifdef BLE_PXPM_IAS_ENABLE
    evtDiscCmlt.iasStartHandle = s_pxpmIasDiscInfo[p_conn->connIndex].svcStartHandle;
    evtDiscCmlt.iasEndHandle   = s_pxpmIasDiscInfo[p_conn->connIndex].svcEndHandle;
#endif
#ifdef BLE_PXPM_TPS_ENABLE
    evtDiscCmlt.tpsStartHandle = s_pxpmTpsDiscInfo[p_conn->connIndex].svcStartHandle;
    evtDiscCmlt.tpsEndHandle   = s_pxpmTpsDiscInfo[p_conn->connIndex].svcEndHandle;
#endif
    ble_pxpm_ConveyEvent(BLE_PXPM_EVT_DISC_COMPLETE_IND, (uint8_t *) &evtDiscCmlt, (uint8_t)sizeof(BLE_PXPM_EvtDiscComplete_T));
}


/**
 * @brief Process the read response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpm_ProcReadResponse(GATT_Event_T *p_event)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(p_event->eventField.onReadResp.connHandle);

    if(p_conn == NULL)
    {
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if(p_event->eventField.onReadResp.charHandle == s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle)
    {
        uint8_t *p_value = p_event->eventField.onReadResp.readValue;
        BLE_PXPM_EvtLlsAlertLvInd_T evt;

        evt.connHandle = p_event->eventField.onReadResp.connHandle;
        
        STREAM_TO_U8((uint8_t*)&evt.alertLevel, &p_value);

        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND, (uint8_t *) &evt, (uint8_t)sizeof(BLE_PXPM_EvtLlsAlertLvInd_T));
    }
    #ifdef BLE_PXPM_TPS_ENABLE
    else if(p_event->eventField.onReadResp.charHandle == s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARTXPWRLV].charHandle)
    {
        uint8_t *p_value = p_event->eventField.onReadResp.readValue;
        BLE_PXPM_EvtTpsTxPwrLvInd_T evt;

        evt.connHandle = p_event->eventField.onReadResp.connHandle;

        STREAM_TO_U8(&evt.txPowerLevel, &p_value);

        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND, (uint8_t *) &evt, (uint8_t)sizeof(BLE_PXPM_EvtTpsTxPwrLvInd_T));
    }
    #endif
    else
    {
        //Shall not enter here
    }
}


/**
 * @brief Process the write response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpm_ProcWriteResponse(GATT_Event_T *p_event)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(p_event->eventField.onWriteResp.connHandle);

    if(p_conn == NULL)
    {
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if(p_event->eventField.onWriteResp.charHandle == s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle)
    {
        BLE_PXPM_EvtLlsAlertLvWriteRspInd_T evt;
        evt.connHandle = p_event->eventField.onWriteResp.connHandle;
        evt.errCode = 0x00;
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND, (uint8_t *) &evt, (uint8_t)sizeof(BLE_PXPM_EvtLlsAlertLvWriteRspInd_T));
    }
}


/**
 * @brief Process the error response event from GATT.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpm_ProcErrorResponse(GATT_Event_T *p_event)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(p_event->eventField.onError.connHandle);

    if(p_conn == NULL)
    {
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    if(p_event->eventField.onError.attrHandle == s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle)
    {
        BLE_PXPM_EvtLlsAlertLvWriteRspInd_T evt;

        evt.connHandle = p_event->eventField.onError.connHandle;
        evt.errCode = p_event->eventField.onError.errCode;
        ble_pxpm_ConveyEvent(BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND, (uint8_t *) &evt, (uint8_t)sizeof(BLE_PXPM_EvtLlsAlertLvWriteRspInd_T));
    }
}


/**
 * @brief Process GATT events and dispatch them to the appropriate handler.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_pxpm_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTC_EVT_READ_RESP:
        {
            ble_pxpm_ProcReadResponse(p_event);
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            ble_pxpm_ProcWriteResponse(p_event);
        }
        break;

        case GATTC_EVT_ERROR_RESP:
        {
            ble_pxpm_ProcErrorResponse(p_event);
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
 * @brief Initialize the characteristic list for a given connection index.
 *
 * @param connIndex Index of the connection whose characteristic list is to be initialized.
 */
static void ble_pxpm_InitConnCharList(uint8_t connIndex)
{
    ble_pxpm_InitConnList(connIndex);

    ble_pxpm_InitLlsCharList(connIndex);
    #ifdef BLE_PXPM_IAS_ENABLE
    ble_pxpm_InitIasCharList(connIndex);
    #endif
    #ifdef BLE_PXPM_TPS_ENABLE
    ble_pxpm_InitTpsCharList(connIndex);
    #endif
}


/**
 * @brief Process GAP events and handle connection and disconnection events.
 *
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_pxpm_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_PXPM_ConnList_T *p_conn = NULL;

    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                p_conn = ble_pxpm_GetFreeConnList();
                if (p_conn != NULL)
                {
                    p_conn->connHandle = p_event->eventField.evtConnect.connHandle;
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            p_conn = ble_pxpm_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);
            if (p_conn != NULL)
            {
                ble_pxpm_InitConnCharList(p_conn->connIndex);
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
 * @brief Initializes the BLE Proximity Profile Monitor.
 *
 * @retval MBA_RES_SUCCESS          Initialization successful.
 * @retval MBA_RES_FAIL             Initialization failed.
 *
 */
uint16_t BLE_PXPM_Init(void)
{
    uint8_t i;
    sp_pxpmCbRoutine = NULL;

    for(i = 0; i < BLE_PXPM_MAX_CONN_NBR; i++)
    {
        ble_pxpm_InitConnCharList(i);
    }

    if(ble_pxpm_InitAlertLvSvcDiscRegister(pxpmDiscLlsUuid, s_pxpmLlsCharList) != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    #ifdef BLE_PXPM_IAS_ENABLE
    if(ble_pxpm_InitAlertLvSvcDiscRegister(pxpmDiscIasUuid, s_pxpmIasCharList) != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    #endif
    #ifdef BLE_PXPM_TPS_ENABLE
    if(ble_pxpm_InitTxPwrLvSvcDiscRegister(pxpmDiscTpsUuid, s_pxpmTpsCharList) != MBA_RES_SUCCESS)
    {
        return MBA_RES_FAIL;
    }
    #endif
    return MBA_RES_SUCCESS;
}


/**
 * @brief Registers a callback function for BLE Proximity Profile Monitor events.
 *
 * @param[in] routine               Callback function to handle BLE PXP Monitor events.
 *
 */
void BLE_PXPM_EventRegister(BLE_PXPM_EventCb_T routine)
{
    sp_pxpmCbRoutine = routine;
}


/**
 * @brief Writes the Alert Level value to the Link Loss Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to set the alert level for.
 * @param[in] level                 Alert level to be set.
 *
 * @retval MBA_RES_SUCCESS          Write operation successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_WriteLlsAlertLevel(uint16_t connHandle, BLE_PXPM_AlertLevel_T level)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);
    uint16_t result;
    GATTC_WriteParams_T *p_writeParams;

    if(p_conn == NULL || (s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle==0x0000U))
    {
        return MBA_RES_INVALID_PARA;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams == NULL)
    {
        return MBA_RES_OOM;
    }

    p_writeParams->charHandle = s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle;
    p_writeParams->charLength = 1;
    p_writeParams->charValue[0] = (uint8_t)level;
    p_writeParams->writeType = ATT_WRITE_REQ;
    p_writeParams->valueOffset = 0x0000;
    result = GATTC_Write(connHandle, p_writeParams);
    OSAL_Free(p_writeParams);

    return result;
}

#ifdef BLE_PXPM_IAS_ENABLE
/**
 * @brief Writes the Alert Level value to the Immediate Alert Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to set the alert level for.
 * @param[in] level                 Alert level to be set.
 *
 * @retval MBA_RES_SUCCESS          Write operation successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_WriteIasAlertLevel(uint16_t connHandle, BLE_PXPM_AlertLevel_T level)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);
    uint16_t result;
    GATTC_WriteParams_T *p_writeParams;

    if(p_conn == NULL || (s_pxpmIasCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle==0x0000))
    {
        return MBA_RES_INVALID_PARA;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams == NULL)
    {
        return MBA_RES_OOM;
    }

    p_writeParams->charHandle = s_pxpmIasCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle;
    p_writeParams->charLength = 1;
    p_writeParams->charValue[0] = level;
    p_writeParams->writeType = ATT_WRITE_CMD;
    p_writeParams->valueOffset = 0x0000;
    result = GATTC_Write(connHandle, p_writeParams);
    OSAL_Free(p_writeParams);

    return result;
}
#endif


/**
 * @brief Reads the Alert Level value from the Link Loss Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to read the alert level from.
 *
 * @retval MBA_RES_SUCCESS          Read operation successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_ReadLlsAlertLevel(uint16_t connHandle)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);

    if(p_conn == NULL || (s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle==0x0000U))
    {
        return MBA_RES_INVALID_PARA;
    }
    
    return GATTC_Read(connHandle, s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARALERTLV].charHandle, 0);
}

#ifdef BLE_PXPM_TPS_ENABLE
/**
 * @brief Reads the Tx Power Level value from the Tx Power Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to read the Tx Power Level from.
 *
 * @retval MBA_RES_SUCCESS          Read operation successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_ReadTpsTxPowerLevel(uint16_t connHandle)
{
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);

    if(p_conn == NULL || (s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARTXPWRLV].charHandle==0x0000))
    {
        return MBA_RES_INVALID_PARA;
    }

    return GATTC_Read(connHandle, s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[PXPM_INDEX_CHARTXPWRLV].charHandle, 0);
}
#endif


/**
 * @brief Retrieves information about the characteristic UUID of the Proximity service that has been discovered.
 * @note This API could be called only after @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event is issued.
 *
 * @param[in]  connHandle           Connection handle.
 * @param[in]  svcUuid              Service UUID (see @ref BLE_PXPM_SVC_UUID).
 * @param[in]  charUuid             Characteristic UUID (see @ref BLE_PXPM_UUID).
 * @param[out] p_charList           Pointer to store the characteristic information of the discovered service.
 *
 * @retval MBA_RES_SUCCESS          Successfully retrieved the characteristic list.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters, such as:\n
 *                                  - Invalid connection handle.
 *                                  - Invalid service UUID.
 *                                  - Invalid characteristic UUID.
 */
uint16_t BLE_PXPM_GetCharList(uint16_t connHandle, uint16_t svcUuid, uint16_t charUuid, BLE_PXPM_CharList_T *p_charList)
{
    uint8_t             idx;
    uint16_t            desUuid;
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (p_charList == NULL) ||
        (svcUuid < BLE_PXPM_UUID_IMMEDIATE_ALERT_SVC) ||
        (svcUuid > BLE_PXPM_UUID_TXPOWER_SVC) ||
        (charUuid < BLE_PXPM_UUID_ALERT_LEVEL) ||
        (charUuid > BLE_PXPM_UUID_TXPOWER_LEVEL)
       )
    {
        return MBA_RES_INVALID_PARA;
    } 
    (void)memset(p_charList, 0, sizeof(BLE_PXPM_CharList_T));
    switch(svcUuid)
    {
#ifdef BLE_PXPM_IAS_ENABLE
        case BLE_PXPM_UUID_IMMEDIATE_ALERT_SVC:
        {
            for (idx = 0; idx < PXPM_CHARALERTLV_CHAR_NUM; idx++)
            {
                BUF_LE_TO_U16(&desUuid, pxpmDiscAlertLvCharList[idx]->p_uuid->uuid);
                if ((desUuid == charUuid) &&
                    ((pxpmDiscAlertLvCharList[idx]->settings & CHAR_SET_DESCRIPTOR) != CHAR_SET_DESCRIPTOR))
                {
                    p_charList->attrHandle = s_pxpmIasCharList[p_conn->connIndex].p_charInfo[idx].attrHandle;
                    p_charList->property   = s_pxpmIasCharList[p_conn->connIndex].p_charInfo[idx].property;
                    p_charList->charHandle = s_pxpmIasCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
                    break;
                }
            }
        }
        break;
#endif
        case BLE_PXPM_UUID_LINKLOSS_SVC:
        {
            for (idx = 0; idx < (uint8_t)PXPM_CHARALERTLV_CHAR_NUM; idx++)
            {
                BUF_LE_TO_U16(&desUuid, pxpmDiscAlertLvCharList[idx]->p_uuid->uuid);
                if ((desUuid == charUuid) &&
                    ((pxpmDiscAlertLvCharList[idx]->settings & CHAR_SET_DESCRIPTOR) != CHAR_SET_DESCRIPTOR))
                {
                    p_charList->attrHandle = s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[idx].attrHandle;
                    p_charList->property   = s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[idx].property;
                    p_charList->charHandle = s_pxpmLlsCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
                    break;
                }
            }
        }
        break;
#ifdef BLE_PXPM_TPS_ENABLE
        case BLE_PXPM_UUID_TXPOWER_SVC:
        {
            for (idx = 0; idx < PXPM_CHARTXPWRLV_CHAR_NUM; idx++)
            {
                BUF_LE_TO_U16(&desUuid, pxpmDiscTxPwrLvCharList[idx]->p_uuid->uuid);
                if ((desUuid == charUuid) &&
                    ((pxpmDiscTxPwrLvCharList[idx]->settings & CHAR_SET_DESCRIPTOR) != CHAR_SET_DESCRIPTOR))
                {
                    p_charList->attrHandle = s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[idx].attrHandle;
                    p_charList->property   = s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[idx].property;
                    p_charList->charHandle = s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
                    break;
                }
            }
        }
        break;
#endif
        default:
        {
            //Do nothing
        }
        break;
    }
    return MBA_RES_SUCCESS;
}


/**
 * @brief Retrieves information about the descriptor list of the Proximity Service that has been discovered.
 * @note This API could be called only after @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event is issued.
 *
 * @param[in]  connHandle           Connection handle.
 * @param[out] p_descList           Pointer to store the descriptor information of the discovered service.
 *
 * @retval MBA_RES_SUCCESS          Successfully retrieved the descriptor list.
 * @retval MBA_RES_INVALID_PARA     The provided connection handle is not valid.
 */
uint16_t BLE_PXPM_GetDescList(uint16_t connHandle, BLE_PXPM_DescList_T *p_descList)
{
#ifdef BLE_PXPM_TPS_ENABLE
    uint8_t             idx;
#endif
    uint8_t             descNum = 0;
    BLE_PXPM_ConnList_T *p_conn = ble_pxpm_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (p_descList == NULL))
    {
        return MBA_RES_INVALID_PARA;
    } 
    (void)memset(p_descList, 0, sizeof(BLE_PXPM_DescList_T));
#ifdef BLE_PXPM_TPS_ENABLE
    for (idx = 0; idx < PXPM_CHARTXPWRLV_CHAR_NUM; idx++)
    {
        if ((pxpmDiscTxPwrLvCharList[idx]->settings & CHAR_SET_DESCRIPTOR) && (s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[idx].charHandle != 0))
        {
            p_descList->descInfo[descNum].attrHandle = s_pxpmTpsCharList[p_conn->connIndex].p_charInfo[idx].charHandle;
            VARIABLE_COPY_TO_BUF(&p_descList->descInfo[descNum].uuid, pxpmDiscTxPwrLvCharList[idx]->p_uuid->uuid, pxpmDiscTxPwrLvCharList[idx]->p_uuid->uuidLength);
            descNum++;
        }
    }
#endif
    p_descList->totalNum = descNum;
    return MBA_RES_SUCCESS;
}


/**
 * @brief Handles BLE_Stack related events.
 * 
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE Stack events buffer.
 *
*/
void BLE_PXPM_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            ble_pxpm_ProcDiscComplete(p_event);
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
 * @brief Handles BLE Database Discovery (BLE DD) events.
 * 
 * @note This function should be called when BLE DD events occur.
 *
 * @param[in] p_event               Pointer to the BLE DD events buffer.
 *
 */
void BLE_PXPM_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_pxpm_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_pxpm_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

