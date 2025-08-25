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
  BLE Service Change Manager Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_scm.c

  Summary:
    Implements the BLE Service Change Manager, providing functions and events
    for application-level integration.

  Description:
    This source file includes the implementation of the BLE Service Change Manager,
    which facilitates the management of service changes in BLE applications. It is
    essential to invoke the "BLE_SCM_Init" function within the "APP_Initialize"
    function to ensure proper initialization of this module.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "osal/osal_freertos_extend.h"
#include "mba_error_defs.h"
#include "gatt.h"
#include "ble_smp.h"
#include "ble_util/byte_stream.h"
#include "ble_scm.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define UUID_SVC_GATT                               0x1801      // UUID for the Generic Attribute (GATT) Service from the Bluetooth SIG specification.
#define UUID_CHAR_SERVICE_CHANGE                    0x2A05      // UUID for the Service Change Characteristic from the Bluetooth SIG specification.
#define UUID_CHAR_CLIENT_SUPPORTED_FEATURES         0x2B29      // UUID for the Client Supported Features Characteristic from the Bluetooth SIG specification.

/* Enumeration for discovered characteristics indices of GATT service. */
typedef enum DISC_INDEX_GattSvc_T
{
    DISC_INDEX_GATT_SC_CHAR,                                    // Index for the Service Change Characteristic.
    DISC_INDEX_GATT_SC_CCCD,                                    // Index for the Service Change CCCD (Client Characteristic Configuration Descriptor).
    DISC_INDEX_GATT_CSF_CHAR                                    // Index for the Client Supported Features Characteristic.
}DISC_INDEX_GattSvc_T;

/* Enumeration for the configuration procedures of BLE service change manager. */
typedef enum BLE_SCM_ConfigProcedure_T
{
    BLE_SCM_CONFIG_NONE=0x00U,                                  // No ongoing configuration procedure.
    BLE_SCM_CONFIG_GATT_ENABLE_INDICATION,                      // Procedure to enable indications for the Service Change characteristic.
    BLE_SCM_CONFIG_GATT_WRITE_FEATURES,                         // Procedure to write to the Client Supported Features characteristic.
    BLE_SCM_CONFIG_END                                          // Sentinel value indicating the end of the config procedures.
}BLE_SCM_ConfigProcedure_T;

/* Enumeration for the status of configuration procedures. */
typedef enum BLE_SCM_ConfigStatus_T
{
    BLE_SCM_CONFIG_STATUS_SUCCESS=0x00U,                        // Configuration procedure completed successfully.
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Configuration procedure failed. A retry may be attempted with a different configuration.
    BLE_SCM_CONFIG_STATUS_ATT_BUSY,                             // ATT protocol layer is currently busy.
    BLE_SCM_CONFIG_STATUS_END                                   // Sentinel value indicating the end of the config status values.
}BLE_SCM_ConfigStatus_T;

/* Enumeration for the state of the BLE_SCM module. */
typedef enum BLE_SCM_State_T
{
    BLE_SCM_STATE_IDLE = 0x00U,                                 // Device is in the default state, not currently connected to a peer.
    BLE_SCM_STATE_CONNECTED                                     // Device is currently connected to a peer.
} BLE_SCM_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Structure for a connection instance in the service change manager module. */
typedef struct BLE_SCM_Conn_T
{
    uint16_t                    connHandle;                     // Handle identifying the connection.
    uint8_t                     connIndex;                      // Index of the connection in the connection database array.
    BLE_SCM_ConfigProcedure_T   configProcedure;                // The current configuration procedure to be executed after service discovery is complete.
    bool                        isGattSvcFound;                 // Flag indicating whether the GATT service has been found for this connection.
    bool                        isPairingComplete;              // Flag indicating whether pairing has been completed for this connection.
    BLE_SCM_ConfigProcedure_T   queueProcedure;                 // Procedure queued due to ATT protocol being busy.
    BLE_SCM_State_T             state;                          // Current state of the connection.
} BLE_SCM_Conn_T;


/* Structure for the service change manager module. */
typedef struct BLE_SCM_Ctrl_T
{
    BLE_SCM_Conn_T              conn[BLE_GAP_MAX_LINK_NBR];     //Central connection database for the service change manager, supporting up to BLE_GAP_MAX_LINK_NBR connections.
} BLE_SCM_Ctrl_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static BLE_SCM_EventCb_T        s_scmEventCb;                   // Callback function for service change manager.
static BLE_SCM_Ctrl_T *         sp_scmCtrl;                     // Pointer to the service change manager module's structure.
static const uint8_t            s_svcUuidGatt[ATT_UUID_LENGTH_2] = {UINT16_TO_BYTES(UUID_SVC_GATT)}; // UUID for GATT service.
static const ATT_Uuid_T         s_discCharSc =          { {UINT16_TO_BYTES(UUID_CHAR_SERVICE_CHANGE)}, ATT_UUID_LENGTH_2 }; // UUID for Service Change characteristic.

// UUID for Client Characteristic Configuration Descriptor (CCCD) of Service Change characteristic.
static const ATT_Uuid_T         s_discCharScCccd =      { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// UUID for Client Supported Features characteristic.
static const ATT_Uuid_T         s_discCharCsf    =      { {UINT16_TO_BYTES(UUID_CHAR_CLIENT_SUPPORTED_FEATURES)}, ATT_UUID_LENGTH_2 };

// Descriptor for GATT Service Change characteristic.
static BLE_DD_DiscChar_T        s_gattSvcChange =       { &s_discCharSc, 0 };

// Descriptor for CCCD of GATT Service Change characteristic.
static BLE_DD_DiscChar_T        s_gattSvcChangeCccd =   { &s_discCharScCccd, CHAR_SET_DESCRIPTOR };

// Descriptor for GATT Client Supported Features characteristic.
static BLE_DD_DiscChar_T        s_gattCliSupFeature =   { &s_discCharCsf, 0 };
static BLE_DD_DiscChar_T        *s_gattDiscCharList[] =
{
    &s_gattSvcChange,                                           // GATT Service Change Characteristic.
    &s_gattSvcChangeCccd,                                       // Client Characteristic Configuration Descriptor (CCCD) for Service Change.
    &s_gattCliSupFeature                                        // Client Supported Features Characteristic.
};
static BLE_DD_CharInfo_T ** sp_gattCharInfoList;                // Characteristic information list. */
static BLE_DD_CharList_T *  sp_gattCharList;                    // Characteristic list. */
static const BLE_SCM_ConfigStatus_T       s_scmMbaRetCodeMap[]=
{
    BLE_SCM_CONFIG_STATUS_SUCCESS,                              // Corresponds to MBA_RES_SUCCESS
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_FAIL
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_OOM (Out of Memory)
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_INVALID_PARA (Invalid Parameter)
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_NO_RESOURCE
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_BAD_STATE
    BLE_SCM_CONFIG_STATUS_FAIL,                                 // Corresponds to MBA_RES_PENDING_DUE_TO_SECURITY
    BLE_SCM_CONFIG_STATUS_ATT_BUSY                              // Corresponds to MBA_RES_BUSY (ATT Protocol Busy)
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Initialize a BLE connection object.
 * 
 * @param[in] p_conn Pointer to the BLE connection object to initialize.
 */
static void ble_scm_InitConn(BLE_SCM_Conn_T *p_conn)
{
    (void)memset((uint8_t *)p_conn, 0, sizeof(BLE_SCM_Conn_T));
}


/**
 * @brief Retrieve a free BLE connection object.
 * 
 * @retval Pointer to the free BLE connection object, or NULL if none are available.
 */
static BLE_SCM_Conn_T *ble_scm_GetFreeConn(void)
{
    uint8_t i;

    for (i=0; i<BLE_GAP_MAX_LINK_NBR; i++)
    {
        if (sp_scmCtrl->conn[i].state == BLE_SCM_STATE_IDLE)
        {
            sp_scmCtrl->conn[i].state = BLE_SCM_STATE_CONNECTED;
            sp_scmCtrl->conn[i].connIndex = i;
            return &sp_scmCtrl->conn[i];
        }
    }
    return NULL;
}


/**
 * @brief Find a BLE connection object by its handle.
 * 
 * @param[in] connHandle The handle of the connection to find.
 * 
 * @retval Pointer to the BLE connection object, or NULL if not found.
 */
static BLE_SCM_Conn_T *ble_scm_FindConnByHandle(uint16_t connHandle)
{
    uint8_t i;

    for (i=0; i<BLE_GAP_MAX_LINK_NBR; i++)
    {
        if ((sp_scmCtrl->conn[i].state == BLE_SCM_STATE_CONNECTED) && (sp_scmCtrl->conn[i].connHandle == connHandle))
        {
            sp_scmCtrl->conn[i].connIndex = i;
            return &sp_scmCtrl->conn[i];
        }
    }
    return NULL;
}


/**
 * @brief Create a BLE connection object for a specific connection handle.
 * 
 * @param[in] connHandle The handle of the connection for which to create the object.
 * 
 * @retval Pointer to the newly created BLE connection object, or NULL if unable to get a free connection.
 */
static BLE_SCM_Conn_T *ble_scm_CreateConn(uint16_t connHandle)
{
    BLE_SCM_Conn_T *p_conn;

    p_conn = ble_scm_GetFreeConn();

    if (p_conn != NULL)
    {
        p_conn->connHandle = connHandle;
        p_conn->queueProcedure = BLE_SCM_CONFIG_NONE;
    }

    return p_conn;
}


/** @brief Initialize the list of BLE characteristics. */
static void ble_scm_InitCharList(void)
{
    uint8_t i, j;

    for(i=0; i<BLE_GAP_MAX_LINK_NBR; i++)
    {
        sp_gattCharList[i].connHandle = 0;
        sp_gattCharList[i].p_charInfo = sp_gattCharInfoList[i];

        for(j=0; j<BLE_SCM_GATT_DISC_CHAR_NUM; j++)
        {
            sp_gattCharInfoList[i][j].charHandle = 0;
            sp_gattCharInfoList[i][j].property = 0;
        }
    }
}


/**
 * @brief Get the attribute handle for the Service Changed characteristic.
 * 
 * @param[in] connHandle    The handle of the connection to search within.
 * @param[in] index         The index of the characteristic in the list.
 * 
 * @retval The attribute handle, or 0 if not found.
 */
static uint16_t ble_scm_GetScAttrHandle(uint16_t connHandle, uint8_t index)
{
    uint8_t i;

    for (i=0; i<BLE_GAP_MAX_LINK_NBR; i++)
    {
        if (sp_gattCharList[i].connHandle == connHandle)
        {
            if (sp_gattCharList[i].p_charInfo[index].charHandle == 0U)
            {
                break;
            }
            else
            {
                return sp_gattCharList[i].p_charInfo[index].charHandle;
            }
        }
    }
    return 0;
}


/**
 * @brief Process GAP events.
 * 
 * @param[in] p_event         Pointer to the GAP event.
 */
static void ble_scm_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                /* Create connection instance if it is not created in BLE_SCM_SetBondedCharInfo(). */
                if (ble_scm_FindConnByHandle(p_event->eventField.evtConnect.connHandle) == NULL)
                {
                    (void)ble_scm_CreateConn(p_event->eventField.evtConnect.connHandle);
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            BLE_SCM_Conn_T *p_conn;

            /* Find connection instance */
            p_conn = ble_scm_FindConnByHandle(p_event->eventField.evtDisconnect.connHandle);

            if (p_conn != NULL)
            {
                (void)memset(sp_gattCharInfoList[p_conn->connIndex], 0, (sizeof(BLE_DD_CharInfo_T)*BLE_SCM_GATT_DISC_CHAR_NUM));
                ble_scm_InitConn(p_conn);
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
 * @brief Process Service Change events.
 * 
 * @param[in] connHandle            The handle of the connection where the service change occurred.
 * @param[in] affectedStartHandle   The starting handle of the affected service.
 * @param[in] affectedEndHandle     The ending handle of the affected service.
 */
static void ble_scm_ProcServiceChange(uint16_t connHandle, uint16_t affectedStartHandle, uint16_t affectedEndHandle)
{
    if (s_scmEventCb != NULL)
    {
        BLE_SCM_Event_T scmEvent;

        scmEvent.eventId = BLE_SCM_EVT_SVC_CHANGE;
        scmEvent.eventField.evtServiceChange.connHandle = connHandle;
        scmEvent.eventField.evtServiceChange.scStartHandle = affectedStartHandle;
        scmEvent.eventField.evtServiceChange.scEndHandle = affectedEndHandle;
        s_scmEventCb(&scmEvent);
    }
}


/**
 * @brief Send a GATT write request.
 * 
 * @param[in] p_conn        Pointer to the BLE connection object.
 * @param[in] attrHandle    The attribute handle to write to.
 * @param[in] attrLength    The length of the attribute data.
 * @param[in] p_attrValue   Pointer to the attribute data.
 * 
 * @retval The result of the write operation.
 */
static uint16_t ble_scm_SendGattWriteRequest(BLE_SCM_Conn_T *p_conn, uint16_t attrHandle, uint16_t attrLength, uint8_t *p_attrValue)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t            result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = attrHandle;
        p_writeParams->charLength = attrLength;
        (void)memcpy(p_writeParams->charValue, p_attrValue, attrLength);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0;
        p_writeParams->flags = 0;
        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        OSAL_Free(p_writeParams);
    }
    else
    {
        result = MBA_RES_OOM;
    }
    return result;
}


/**
 * @brief Enable Service Change indication for a given connection.
 * 
 * @param[in] p_conn Pointer to the BLE connection object.
 * 
 * @retval The status of the configuration operation.
 */
static BLE_SCM_ConfigStatus_T ble_scm_EnableServiceChangeIndication(BLE_SCM_Conn_T *p_conn)
{
    BLE_SCM_ConfigStatus_T  result;
    uint16_t    scCccdHandle, apiStatus;
    uint8_t     cccdValue[] = {UINT16_TO_BYTES(INDICATION)};

    scCccdHandle = ble_scm_GetScAttrHandle(p_conn->connHandle, (uint8_t)DISC_INDEX_GATT_SC_CCCD);

    if (scCccdHandle != 0U)
    {
        apiStatus = ble_scm_SendGattWriteRequest(p_conn, scCccdHandle, (uint16_t)sizeof(cccdValue), cccdValue);
        result = s_scmMbaRetCodeMap[apiStatus];
    }
    else
    {
        result = BLE_SCM_CONFIG_STATUS_FAIL;
    }
    return result;
}


/**
 * @brief Write Client Supported Features.
 * 
 * @param[in] p_conn Pointer to the BLE connection object.
 * 
 * @retval The status of the write operation.
 */
static BLE_SCM_ConfigStatus_T ble_scm_WriteClientSupportFeatures(BLE_SCM_Conn_T *p_conn)
{
    BLE_SCM_ConfigStatus_T  result;
    uint16_t    csfHandle, apiStatus;
    uint8_t     feature[] = {GATT_ROBUST_CACHING};

    csfHandle = ble_scm_GetScAttrHandle(p_conn->connHandle, (uint8_t)DISC_INDEX_GATT_CSF_CHAR);

    if (csfHandle != 0U)
    {
        apiStatus = ble_scm_SendGattWriteRequest(p_conn, csfHandle, (uint16_t)sizeof(feature), feature);
        result = s_scmMbaRetCodeMap[apiStatus];
    }
    else
    {
        result = BLE_SCM_CONFIG_STATUS_FAIL;
    }
    return result;
}


/**
 * @brief State machine for BLE SCM configuration procedures.
 * 
 * @param[in] p_conn    Pointer to the BLE connection object.
 * @param[in] procedure The configuration procedure to execute.
 */
static void ble_scm_ConfigProcedureStateMachine(BLE_SCM_Conn_T *p_conn, BLE_SCM_ConfigProcedure_T procedure)
{
    if (procedure == BLE_SCM_CONFIG_GATT_ENABLE_INDICATION)
    {
        BLE_SCM_ConfigStatus_T result;

        result = ble_scm_EnableServiceChangeIndication(p_conn);

        if (result == BLE_SCM_CONFIG_STATUS_SUCCESS)
        {
            p_conn->configProcedure = procedure;
        }
        else if (result == BLE_SCM_CONFIG_STATUS_ATT_BUSY)
        {
            p_conn->queueProcedure = procedure;
        }
        else
        {
            procedure += 1U;
        }
    }

    if (procedure == BLE_SCM_CONFIG_GATT_WRITE_FEATURES)
    {
        BLE_SCM_ConfigStatus_T result;

        result = ble_scm_WriteClientSupportFeatures(p_conn);

        if (result == BLE_SCM_CONFIG_STATUS_SUCCESS)
        {
            p_conn->configProcedure = procedure;
        }
        else if (result == BLE_SCM_CONFIG_STATUS_ATT_BUSY)
        {
            p_conn->queueProcedure = procedure;
        }
        else
        {
            procedure += 1U;
        }
    }

    if (procedure == BLE_SCM_CONFIG_END)
    {
        p_conn->configProcedure = BLE_SCM_CONFIG_NONE;
        if (s_scmEventCb != NULL)
        {
            BLE_SCM_Event_T scmEvent;

            scmEvent.eventId = BLE_SCM_EVT_CONFIGURED;
            scmEvent.eventField.evtConfigured.connHandle = p_conn->connHandle;
            s_scmEventCb(&scmEvent);
        }
    }
}


/**
 * @brief Process GATT events.
 * 
 * @param[in] p_event Pointer to the GATT event.
 */
static void ble_scm_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTC_EVT_ERROR_RESP:
        {
            BLE_SCM_Conn_T *p_conn;

            p_conn = ble_scm_FindConnByHandle(p_event->eventField.onError.connHandle);
            if (p_conn != NULL)
            {
                if (p_event->eventField.onError.errCode == ATT_ERR_DATABASE_OUT_OF_SYNC)
                {
                    p_conn->configProcedure = BLE_SCM_CONFIG_NONE;
                    p_conn->isGattSvcFound = false;
                    ble_scm_ProcServiceChange(p_conn->connHandle, 0x0001, 0xFFFF);
                }
                else
                {
                    if (p_conn->configProcedure != BLE_SCM_CONFIG_NONE)
                    {
                        ble_scm_ConfigProcedureStateMachine(p_conn, (p_conn->configProcedure+1U));
                    }
                }
            }
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            BLE_SCM_Conn_T *p_conn;

            p_conn = ble_scm_FindConnByHandle(p_event->eventField.onWriteResp.connHandle);

            if ((p_conn != NULL) && (p_conn->configProcedure))
            {
                ble_scm_ConfigProcedureStateMachine(p_conn, (p_conn->configProcedure+1U));
            }
        }
        break;

        case GATTC_EVT_HV_INDICATE:
        {
            if (p_event->eventField.onIndication.charHandle == ble_scm_GetScAttrHandle(p_event->eventField.onIndication.connHandle, (uint8_t)DISC_INDEX_GATT_SC_CHAR))
            {
                uint16_t        affectedStartHandle, affectedEndHandle;
                BLE_SCM_Conn_T  *p_conn;

                p_conn = ble_scm_FindConnByHandle(p_event->eventField.onIndication.connHandle);
                if (p_conn != NULL)
                {
                    p_conn->isGattSvcFound = false;
                }

                BUF_LE_TO_U16(&affectedStartHandle, &p_event->eventField.onIndication.receivedValue[0]);
                BUF_LE_TO_U16(&affectedEndHandle, &p_event->eventField.onIndication.receivedValue[2]);
                ble_scm_ProcServiceChange(p_event->eventField.onIndication.connHandle, affectedStartHandle, affectedEndHandle);
            }
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
            BLE_SCM_Conn_T *p_conn;

            p_conn = ble_scm_FindConnByHandle(p_event->eventField.onClientProtocolAvailable.connHandle);

            if ((p_conn != NULL) && (p_conn->queueProcedure!=0U))
            {
                ble_scm_ConfigProcedureStateMachine(p_conn, p_conn->queueProcedure);
                p_conn->queueProcedure = BLE_SCM_CONFIG_NONE;
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
 * @brief Process SMP events.
 * 
 * @param[in] p_event Pointer to the SMP event.
 */
static void ble_scm_SmpEventProcess(BLE_SMP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_SMP_EVT_PAIRING_COMPLETE:
        {
            BLE_SCM_Conn_T *p_conn;

            p_conn = ble_scm_FindConnByHandle(p_event->eventField.evtPairingComplete.connHandle);

            if (p_conn != NULL)
            {
                if (p_event->eventField.evtPairingComplete.status == BLE_SMP_PAIRING_SUCCESS)
                {
                    p_conn->isPairingComplete = true;
                    if (p_conn->isGattSvcFound)
                    {
                        /* Send handle information to application when connection paired and characteristics discovered. */
                        if (s_scmEventCb != NULL)
                        {
                            BLE_SCM_Event_T scmEvent;

                            scmEvent.eventId = BLE_SCM_EVT_BONDED_CHAR_INFO;
                            scmEvent.eventField.evtBondedCharInfo.connHandle = p_event->eventField.evtPairingComplete.connHandle;
                            (void)memcpy(scmEvent.eventField.evtBondedCharInfo.charInfo, sp_gattCharInfoList[p_conn->connIndex], sizeof(BLE_DD_CharInfo_T)*BLE_SCM_GATT_DISC_CHAR_NUM);
                            s_scmEventCb(&scmEvent);
                        }
                    }
                }
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
 * @brief Initialize the BLE Service Change Manager.
 * 
 * @retval The result of the initialization process.
 */
uint16_t BLE_SCM_Init(void)
{
    BLE_DD_DiscSvc_T gattDisc;

    if (sp_scmCtrl == NULL)
    {
        sp_scmCtrl = OSAL_Malloc(sizeof(BLE_SCM_Ctrl_T));

        if (sp_scmCtrl == NULL)
        {
            return MBA_RES_OOM;
        }
    }

    if (sp_gattCharList == NULL)
    {
        sp_gattCharList = OSAL_Malloc(sizeof(BLE_DD_CharList_T) *BLE_GAP_MAX_LINK_NBR);

        if (sp_gattCharList == NULL)
        {
            return MBA_RES_OOM;
        }
    }

    if (sp_gattCharInfoList == NULL)
    {
        sp_gattCharInfoList = OSAL_Malloc(sizeof(BLE_DD_CharInfo_T *) * BLE_GAP_MAX_LINK_NBR);
        
        if (sp_gattCharInfoList == NULL)
        {
            return MBA_RES_OOM;
        }        

        (void)memset(sp_gattCharInfoList, 0x00, sizeof(BLE_DD_CharInfo_T *) * BLE_GAP_MAX_LINK_NBR);
    }

    for (uint8_t i = 0; i < BLE_GAP_MAX_LINK_NBR; i++)
    {
        if (sp_gattCharInfoList[i] == NULL)
        {
            sp_gattCharInfoList[i] = OSAL_Malloc(sizeof(BLE_DD_CharInfo_T) * BLE_SCM_GATT_DISC_CHAR_NUM);

            if (sp_gattCharInfoList[i] == NULL)
            {
                return MBA_RES_OOM;
            }
        }
    }


    (void)memset((uint8_t *)sp_scmCtrl, 0, sizeof(BLE_SCM_Ctrl_T));

    ble_scm_InitCharList();

    /* Register GATT service discovery */
    gattDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_2;
    (void)memcpy(gattDisc.svcUuid.uuid, s_svcUuidGatt, ATT_UUID_LENGTH_2);
    gattDisc.p_discInfo = NULL;
    gattDisc.p_discChars = s_gattDiscCharList;
    gattDisc.p_charList = sp_gattCharList;
    gattDisc.discCharsNum = BLE_SCM_GATT_DISC_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&gattDisc);
}


/**
 * @brief Register an event callback for BLE Service Change Manager events.
 * 
 * @param[in] eventCb The callback function to register.
 */
void BLE_SCM_EventRegister(BLE_SCM_EventCb_T eventCb)
{
    s_scmEventCb = eventCb;
}


/**
 * @brief BLE event handler for the Service Change Manager.
 * 
 * @param[in] p_stackEvent Pointer to the stack event.
 */
void BLE_SCM_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_scm_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_scm_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_BLE_SMP:
        {
            ble_scm_SmpEventProcess((BLE_SMP_Event_T *)p_stackEvent->p_event);
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
 * @brief BLE Device Discovery event handler for the Service Change Manager.
 * 
 * @param[in] p_event Pointer to the device discovery event.
 */
void BLE_SCM_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            BLE_SCM_Conn_T *p_conn;

            p_conn = ble_scm_FindConnByHandle(p_event->eventField.evtDiscResult.connHandle);

            /* Check if GATT service is found of the connection */
            if (p_conn != NULL)
            {
                /* By checking the discovered handles exist or not. */
                if (ble_scm_GetScAttrHandle(p_event->eventField.evtDiscResult.connHandle, (uint8_t)DISC_INDEX_GATT_SC_CHAR) != 0U)
                {
                    p_conn->isGattSvcFound = true;

                    if (s_scmEventCb != NULL)
                    {
                        BLE_SCM_Event_T scmEvent;

                        scmEvent.eventId = BLE_SCM_EVT_DISC_COMPLETE;
                        scmEvent.eventField.evtDiscComplete.connHandle = p_event->eventField.evtDiscResult.connHandle;
                        s_scmEventCb(&scmEvent);

                        if (p_conn->isPairingComplete)
                        {
                            scmEvent.eventId = BLE_SCM_EVT_BONDED_CHAR_INFO;
                            scmEvent.eventField.evtBondedCharInfo.connHandle = p_event->eventField.evtDiscResult.connHandle;
                            (void)memcpy(scmEvent.eventField.evtBondedCharInfo.charInfo, sp_gattCharInfoList[p_conn->connIndex], sizeof(BLE_DD_CharInfo_T)*BLE_SCM_GATT_DISC_CHAR_NUM);
                            s_scmEventCb(&scmEvent);
                        }
                    }

                    /* Start configuration procedure when discovery complete and GATT svc is found. */
                    ble_scm_ConfigProcedureStateMachine(p_conn, BLE_SCM_CONFIG_GATT_ENABLE_INDICATION);
                }
                else
                {
                    p_conn->isGattSvcFound = false;
                }
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
 * @brief Set the bonded characteristics information for a given connection.
 * 
 * @param[in] connHandle    The handle of the connection.
 * @param[in] p_charInfo    Pointer to the characteristics information.
 */
void BLE_SCM_SetBondedCharInfo(uint16_t connHandle, BLE_DD_CharInfo_T *p_charInfo)
{
    BLE_SCM_Conn_T *p_conn;

    p_conn = ble_scm_FindConnByHandle(connHandle);

    if (p_conn == NULL)
    {
        p_conn = ble_scm_CreateConn(connHandle);
    }

    if (p_conn != NULL)
    {
        (void)memcpy(sp_gattCharInfoList[p_conn->connIndex], p_charInfo, (sizeof(BLE_DD_CharInfo_T)*BLE_SCM_GATT_DISC_CHAR_NUM));
        p_conn->isGattSvcFound = true;
    }
}

