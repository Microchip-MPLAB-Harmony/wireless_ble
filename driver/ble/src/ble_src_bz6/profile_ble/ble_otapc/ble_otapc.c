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
  BLE OTA Profile Client Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otapc.c

  Summary:
    Implements the client-side functionality of the BLE Over-The-Air (OTA) 
    Profile, enabling OTA firmware updates for BLE devices.

  Description:
    This source file provides the necessary functions to interface with the 
    BLE OTA Profile from the client's perspective. It facilitates the initiation 
    and management of firmware updates over a BLE connection, as per the 
    specifications and requirements of the OTA process.
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
#include "ble_otapc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_OTAPC_OP_CODE_RESPONSE                          (0x01U)                 // Response operation code for BLE OTAPC.
#define BLE_OTAPC_OP_CODE_UPDATE_REQUEST                    (0x02U)                 // Request to initiate an update operation.
#define BLE_OTAPC_OP_CODE_UPDATE_START                      (0x03U)                 // Request to start an update operation.
#define BLE_OTAPC_OP_CODE_UPDATE_COMPLETE                   (0x04U)                 // Indication that update process is complete.
#define BLE_OTAPC_OP_CODE_RESET_REQUEST                     (0x05U)                 // Request to reset the BLE OTAPC.

#define BLE_OTAPC_STATE_INIT                                (0x00U)                 // Initial state of the BLE OTAPC.
#define BLE_OTAPC_STATE_CHAR_DISC_COMP                      (0x01U)                 // State after characteristic discovery is complete.
#define BLE_OTAPC_STATE_RETRY_CTRL_CCCD                     (0x02U)                 // State indicating a retry for writing to the Control Point CCCDs.
#define BLE_OTAPC_STATE_WRITE_CTRL_CCCD                     (0x03U)                 // State indicating a write to the Control Point CCCD is in progresss.
#define BLE_OTAPC_STATE_WAIT_SEC                            (0x04U)                 // State indicating waiting for security procedures to complete.
#define BLE_OTAPC_STATE_CTRL_CCCD_ENABLED                   (0x05U)                 // State indicating the Control Point CCCD is enabled.
#define BLE_OTAPC_STATE_RETRY_DATA_CCCD                     (0x06U)                 // State indicating a retry for writing to the Data CCCD.
#define BLE_OTAPC_STATE_WRITE_DATA_CCCD                     (0x07U)                 // State indicating a write to the Data CCCD is in progress.
#define BLE_OTAPC_STATE_DATA_CCCD_ENABLED                   (0x08U)                 // State indicating the Data CCCD is enabled.
#define BLE_OTAPC_STATE_REQ_ALLOWED                         (0x09U)                 // State indicating that requests are now allowed.

#define UUID_OTA_SERVICE_16         0xD7,0x15,0x82,0x8E,0x1B,0xE6,0x23,0x99,0xB3,0x46,0x3D,0x25,0x50,0x48,0x43,0x4D     // UUID for the OTA service (little endian format).
#define UUID_OTA_CHAR_FEATURE       0x58,0x63,0x90,0x2F,0x4A,0x0C,0x03,0xAF,0x46,0x42,0xE4,0x22,0x50,0x48,0x43,0x4D     // UUID for the OTA feature characteristic (little endian format). 
#define UUID_OTA_CHAR_CTRL_PT       0x76,0x8A,0x02,0x39,0x7F,0xC9,0x82,0x88,0xDE,0x45,0x27,0x93,0x50,0x48,0x43,0x4D     // UUID for the OTA control point characteristic (little endian format).
#define UUID_OTA_CHAR_DATA          0x78,0xD4,0x8C,0x7C,0xF5,0x56,0x7E,0xBA,0xA6,0x40,0xD9,0x34,0x50,0x48,0x43,0x4D     // UUID for the OTA data characteristic (little endian format).

#define BLE_OTAPC_MAX_CONN_NBR                              BLE_GAP_MAX_LINK_NBR    // Maximum number of concurrent OTA profile client connections.

typedef enum BLE_OTAPC_CharIndex_T
{
    OTAPC_INDEX_CHAR_FEATURE=0x01U,                          // Index for OTA Feature Characteristic.
    OTAPC_INDEX_CHAR_CTRL,                                   // Index for OTA Control Point Characteristic.
    OTAPC_INDEX_CHAR_CTRL_CCCD,                              // Index for OTA Control Point CCCD (Client Characteristic Configuration Descriptor).
    OTAPC_INDEX_CHAR_DATA,                                   // Index for OTA Data Characteristic.
    OTAPC_INDEX_CHAR_DATA_CCCD,                              // Index for OTA Data Characteristic CCCD.

    OTAPC_CHAR_NUM                                           // Total number of OTA characteristics.
}BLE_OTAPC_CharIndex_T;

typedef enum BLE_OTAPC_State_T
{
    BLE_OTAPC_STATE_IDLE = 0x00U,                           // State indicating the OTA profile client is idle and not currently connected.
    BLE_OTAPC_STATE_CONNECTED                               // State indicating the OTA profile client is connected.
} BLE_OTAPC_State_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Structure for BLE OTA (Over-The-Air) profile connection parameters. */
typedef struct BLE_OTAPC_ConnList_T
{
    uint8_t                     state;                      // Current state of the BLE OTA profile. See @ref BLE_OTAPC_STATUS for possible states.
    uint8_t                     connIndex;                  // Index of the connection in the connection list.
    uint16_t                    connHandle;                 // Unique handle for the active connection.
    uint16_t                    attMtu;                     // The ATT Maximum Transmission Unit size for this connection.
    uint16_t                    maxFragSize;                // Maximum size of a single OTA fragment.
    uint16_t                    accFragLen;                 // Accumulated length of received fragments.
    BLE_OTAPC_State_T           connState;                  // Detailed connection state. See @ref BLE_OTAPC_STATE for state definitions.
    uint8_t                     fwExtFeature;               // Firmware extension feature flag.
} BLE_OTAPC_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback routine for BLE Over-The-Air Programming Client (OTAPC) events.
static BLE_OTAPC_EventCb_T      sp_otapcCbRoutine;

// OTA Feature Characteristic UUID.
static const ATT_Uuid_T         s_otapcDiscCharFeature =    { {UUID_OTA_CHAR_FEATURE}, ATT_UUID_LENGTH_16 };

// OTA Control Point Characteristic UUID.
static const ATT_Uuid_T         s_otapcDiscCharCtrl =       { {UUID_OTA_CHAR_CTRL_PT}, ATT_UUID_LENGTH_16 };

// Client Characteristic Configuration Descriptor (CCCD) for OTA Control Point.
static const ATT_Uuid_T         s_otapcDiscCharCtrlCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// OTA Data Characteristic UUID.
static const ATT_Uuid_T         s_otapcDiscCharData =       { {UUID_OTA_CHAR_DATA}, ATT_UUID_LENGTH_16 };

// CCCD for OTA Data Characteristic.
static const ATT_Uuid_T         s_otapcDiscCharDataCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// Discovered OTA Feature Characteristic.
static BLE_DD_DiscChar_T        s_otapcFeature =         { &s_otapcDiscCharFeature, 0 };

// Discovered OTA Control Point Characteristic.
static BLE_DD_DiscChar_T        s_otapcCtrl =            { &s_otapcDiscCharCtrl, 0 };

// Discovered CCCD for OTA Control Point.
static BLE_DD_DiscChar_T        s_otapcCtrlCccd =        { &s_otapcDiscCharCtrlCccd, CHAR_SET_DESCRIPTOR };

// Discovered OTA Data Characteristic. */
static BLE_DD_DiscChar_T        s_otapcData =            { &s_otapcDiscCharData, 0 };

// Discovered CCCD for OTA Data Characteristic. */
static BLE_DD_DiscChar_T        s_otapcDataCccd =        { &s_otapcDiscCharDataCccd, CHAR_SET_DESCRIPTOR };

// List of pointers to the discovered characteristics for OTA service.
static BLE_DD_DiscChar_T        *s_otapcDiscCharList[] =
{
    &s_otapcFeature,                 // Pointer to OTA Feature Characteristic
    &s_otapcCtrl,                    // Pointer to OTA Control Point Characteristic
    &s_otapcCtrlCccd,                // Pointer to CCCD for OTA Control Point
    &s_otapcData,                    // Pointer to OTA Data Characteristic
    &s_otapcDataCccd,                // Pointer to CCCD for OTA Data Characteristic
};

// Array of characteristic information structures for each BLE connection.
static BLE_DD_CharInfo_T        s_otapcCharInfoList[BLE_OTAPC_MAX_CONN_NBR][OTAPC_CHAR_NUM];

// Array of characteristic lists for each BLE connection.
static BLE_DD_CharList_T        s_otapcCharList[BLE_OTAPC_MAX_CONN_NBR];

// Array of OTA programming connection structures for each BLE connection.
static BLE_OTAPC_ConnList_T     s_otapcConnList[BLE_OTAPC_MAX_CONN_NBR];

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Initializes the characteristic list for a given connection index.
 *
 * @param connIndex The index of the connection to initialize the characteristic list for.
 */
static void ble_otapc_InitCharList(uint8_t connIndex)
{
    s_otapcCharList[connIndex].connHandle = 0;
    s_otapcCharList[connIndex].p_charInfo = (BLE_DD_CharInfo_T *) &(s_otapcCharInfoList[connIndex]);

    (void)memset(s_otapcCharList[connIndex].p_charInfo, 0x00, sizeof(BLE_DD_CharInfo_T) * (uint8_t)OTAPC_CHAR_NUM);
}


/**
 * @brief Retrieves the connection list entry by connection handle.
 *
 * @param connHandle The handle of the connection to search for.
 * 
 * @retval Pointer to the connection list entry, or NULL if not found.
 */
static BLE_OTAPC_ConnList_T *ble_otapc_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        if ((s_otapcConnList[i].connState == BLE_OTAPC_STATE_CONNECTED) && (s_otapcConnList[i].connHandle == connHandle))
        {
            return &s_otapcConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Gets a free connection list entry and mark it as connected.
 *
 * @retval Pointer to the free connection list entry, or NULL if no free entry is available.
 */
static BLE_OTAPC_ConnList_T *ble_otapc_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        if (s_otapcConnList[i].connState == BLE_OTAPC_STATE_IDLE)
        {
            s_otapcConnList[i].connState = BLE_OTAPC_STATE_CONNECTED;
            s_otapcConnList[i].connIndex = i;
            return &s_otapcConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Enables notifications for the OTAP Control Point characteristic.
 *
 * @param p_conn Pointer to the connection list entry for which to enable notifications.
 */
static void ble_otapc_EnableCtrlCccd(BLE_OTAPC_ConnList_T *p_conn)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL_CCCD].charHandle;
        p_writeParams->charLength = 0x02;
        U16_TO_BUF_LE(p_writeParams->charValue, NOTIFICATION);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->state = BLE_OTAPC_STATE_WRITE_CTRL_CCCD;
        }
        else
        {
            p_conn->state = BLE_OTAPC_STATE_RETRY_CTRL_CCCD;
        }
        OSAL_Free(p_writeParams);
    }
}


/**
 * @brief Enables notifications for the OTAP Data characteristic.
 *
 * @param p_conn Pointer to the connection list entry for which to enable notifications.
 */
static void ble_otapc_EnableDataCccd(BLE_OTAPC_ConnList_T *p_conn)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_DATA_CCCD].charHandle;
        p_writeParams->charLength = 0x02;
        U16_TO_BUF_LE(p_writeParams->charValue, NOTIFICATION);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->state = BLE_OTAPC_STATE_WRITE_DATA_CCCD;
        }
        else
        {
            p_conn->state = BLE_OTAPC_STATE_RETRY_DATA_CCCD;
        }
        OSAL_Free(p_writeParams);
    }
}


/**
 * @brief Processes GATT Read Response for OTAPC (Over The Air Programming Client).
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otapc_ProcGattReadResp(GATT_Event_T *p_event)
{
    BLE_OTAPC_ConnList_T *p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onReadResp.connHandle);
    
    if (p_conn == NULL)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    if (p_event->eventField.onReadResp.charHandle == s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_FEATURE].charHandle)
    {
        if (sp_otapcCbRoutine != NULL)
        {
            BLE_OTAPC_Event_T evt;

            evt.eventId = BLE_OTAPC_EVT_FEATURE_IND;
            evt.eventField.evtFeatureInd.connHandle = p_conn->connHandle;
            evt.eventField.evtFeatureInd.suppImgType = p_event->eventField.onReadResp.readValue[0];
            if (p_event->eventField.onReadResp.attrDataLength > 1U)
			{
                evt.eventField.evtFeatureInd.fwExtFeaure = p_event->eventField.onReadResp.readValue[1];
            }
			else
			{
                evt.eventField.evtFeatureInd.fwExtFeaure = 0x00U;
			}

            p_conn->fwExtFeature = evt.eventField.evtFeatureInd.fwExtFeaure;
            
            sp_otapcCbRoutine(&evt);
        }
    }
}


/**
 * @brief Processes GATT Write Response for OTAPC.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otapc_ProcGattWriteResp(GATT_Event_T *p_event)
{
    BLE_OTAPC_ConnList_T *p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onWriteResp.connHandle);
    
    if (p_conn == NULL)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    if (p_event->eventField.onWriteResp.charHandle < s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle
        || p_event->eventField.onWriteResp.charHandle > s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_DATA_CCCD].charHandle)
    {
        return;
    }

    switch (p_conn->state)
    {
        case BLE_OTAPC_STATE_WRITE_CTRL_CCCD:
        {
            p_conn->state = BLE_OTAPC_STATE_CTRL_CCCD_ENABLED;
            ble_otapc_EnableDataCccd(p_conn);
        }
        break;
        case BLE_OTAPC_STATE_WRITE_DATA_CCCD:
        {
            p_conn->state = BLE_OTAPC_STATE_DATA_CCCD_ENABLED;

            if (sp_otapcCbRoutine != NULL)
            {
                BLE_OTAPC_Event_T evtPara;

                evtPara.eventId = BLE_OTAPC_EVT_CONNECT_IND;
                evtPara.eventField.evtConnectInd.connHandle = p_conn->connHandle;
                sp_otapcCbRoutine(&evtPara);
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
 * @brief Processes GATT Notification for OTAPC.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otapc_ProcGattNotification(GATT_Event_T *p_event)
{
    uint8_t *p_value;
    BLE_OTAPC_ConnList_T *p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onNotification.connHandle);
    
    if (p_conn == NULL || sp_otapcCbRoutine == NULL)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    if (p_event->eventField.onNotification.charHandle == s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle)
    {
        if (p_event->eventField.onNotification.receivedValue[0] != BLE_OTAPC_OP_CODE_RESPONSE)
        {
            BLE_OTAPC_Event_T evt;
            evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
            sp_otapcCbRoutine(&evt);
            return;
        }

        switch (p_event->eventField.onNotification.receivedValue[1])
        {
            case BLE_OTAPC_OP_CODE_UPDATE_REQUEST:
            {
                BLE_OTAPC_Event_T evtPara;

                evtPara.eventId = BLE_OTAPC_EVT_REQ_RSP_IND;
                evtPara.eventField.evtReqRspInd.connHandle = p_conn->connHandle;
                
                p_value = &p_event->eventField.onNotification.receivedValue[2];
                STREAM_TO_U8(&evtPara.eventField.evtReqRspInd.result, &p_value);
                STREAM_LE_TO_U16(&evtPara.eventField.evtReqRspInd.maxFragImgSize, &p_value);
                STREAM_LE_TO_U32(&evtPara.eventField.evtReqRspInd.imgStartIdx, &p_value);
                STREAM_LE_TO_U32(&evtPara.eventField.evtReqRspInd.fwImageVer, &p_value);

                if (evtPara.eventField.evtReqRspInd.result == BLE_OTAPC_RESULT_SUCCESS)
                {
                    p_conn->state = BLE_OTAPC_STATE_REQ_ALLOWED;
                    p_conn->maxFragSize = evtPara.eventField.evtReqRspInd.maxFragImgSize;
                }
            
                sp_otapcCbRoutine(&evtPara);
            }
            break;
            case BLE_OTAPC_OP_CODE_UPDATE_START:
            {
                BLE_OTAPC_Event_T evtPara;

                evtPara.eventId = BLE_OTAPC_EVT_START_RSP_IND;
                evtPara.eventField.evtStartRspInd.connHandle = p_conn->connHandle;
                evtPara.eventField.evtStartRspInd.result = p_event->eventField.onNotification.receivedValue[2];

                sp_otapcCbRoutine(&evtPara);
            }
            break;
            case BLE_OTAPC_OP_CODE_UPDATE_COMPLETE:
            {
                BLE_OTAPC_Event_T evtPara;

                evtPara.eventId = BLE_OTAPC_EVT_COMPLETE_RSP_IND;
                evtPara.eventField.evtCompleteRspInd.connHandle = p_conn->connHandle;
                evtPara.eventField.evtCompleteRspInd.result = p_event->eventField.onNotification.receivedValue[2];

                sp_otapcCbRoutine(&evtPara);
            }
            break;
            default:
            {
                //Do nothing
            }
            break;
        }
    }
    else if (p_event->eventField.onNotification.charHandle == s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_DATA].charHandle)
    {
        BLE_OTAPC_Event_T evtPara;

        evtPara.eventId = BLE_OTAPC_EVT_FRAGMENT_COMPL_IND;
        evtPara.eventField.evtFragComplInd.connHandle = p_conn->connHandle;
        evtPara.eventField.evtFragComplInd.result = p_event->eventField.onNotification.receivedValue[0];

        if (p_conn->accFragLen == p_conn->maxFragSize)
        {
            p_conn->accFragLen = 0;
        }

        sp_otapcCbRoutine(&evtPara);
    }
    else
    {
		//Shall not enter here
    }
}


/**
 * @brief Processes GATT Error Response for OTAPC.
 *
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otapc_ProcGattErrorResp(GATT_Event_T *p_event)
{
    BLE_OTAPC_ConnList_T * p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onError.connHandle);
    if (p_conn == NULL)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    if (s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL_CCCD].charHandle != p_event->eventField.onError.attrHandle)
    {
        return;
    }

    if (p_conn->state != BLE_OTAPC_STATE_WRITE_CTRL_CCCD)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    /* Require security permission to access characteristics. Inform application. */
    if ((p_event->eventField.onError.errCode == ATT_ERR_INSUF_ENC) 
        || (p_event->eventField.onError.errCode == ATT_ERR_INSUF_AUTHN))
    {
        p_conn->state = BLE_OTAPC_STATE_WAIT_SEC;
    
        if (sp_otapcCbRoutine != NULL)
        {
            BLE_OTAPC_Event_T evt;

            evt.eventId = BLE_OTAPC_EVT_SEC_REQ;
            evt.eventField.evtSecReq.connHandle = p_event->eventField.onError.connHandle;
            sp_otapcCbRoutine(&evt);
        }
    }
}


/**
 * @brief Process GAP (Generic Access Profile) events for OTAPC.
 *
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_otapc_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_OTAPC_ConnList_T *p_conn = NULL;

    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                p_conn = ble_otapc_GetFreeConnList();
                if (p_conn != NULL)
                {
                    p_conn->connHandle = p_event->eventField.evtConnect.connHandle;
                }
                else
                {
                    BLE_OTAPC_Event_T evt;
                    evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
                    sp_otapcCbRoutine(&evt);
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);
            if (p_conn != NULL)
            {
                (void)memset(p_conn, 0x00, sizeof(BLE_OTAPC_ConnList_T));
                ble_otapc_InitCharList(p_conn->connIndex);
            }
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            if (p_event->eventField.evtEncryptStatus.status == GAP_STATUS_SUCCESS)
            {
                p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.evtEncryptStatus.connHandle);
                if (p_conn != NULL)
                {
                    if (p_conn->state == BLE_OTAPC_STATE_WAIT_SEC)
                    {
                        ble_otapc_EnableCtrlCccd(p_conn);
                    }
                }
            }
        }
        break;
        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.evtTxBufAvailable.connHandle);
            if (p_conn != NULL)
            {
                if (p_conn->state == BLE_OTAPC_STATE_RETRY_CTRL_CCCD)
                {
                    ble_otapc_EnableCtrlCccd(p_conn);
                }
                else if (p_conn->state == BLE_OTAPC_STATE_RETRY_DATA_CCCD)
                {
                    ble_otapc_EnableDataCccd(p_conn);
                }
                else
                {
					//Shall not enter here
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
 * @brief Processes GATT events for the BLE Over-The-Air Programming Client (OTAPC).
 *
 * This function handles various GATT events and takes appropriate actions
 * based on the event type, such as updating MTU size, handling errors,
 * and processing read, write, and notification responses.
 *
 * @param p_event Pointer to the GATT event structure containing event details.
 */
static void ble_otapc_GattEventProcess(GATT_Event_T *p_event)
{
    BLE_OTAPC_ConnList_T *p_conn = NULL;

    switch (p_event->eventId)
    {
        case ATT_EVT_TIMEOUT:
        {
            /* Reset link information */
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn != NULL)
            {
                p_conn->attMtu = p_event->eventField.onUpdateMTU.exchangedMTU;
            }
            else
            {
                BLE_OTAPC_Event_T evt;
                evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
                sp_otapcCbRoutine(&evt);
            }
        }
        break;
        case GATTC_EVT_ERROR_RESP:
        {
            ble_otapc_ProcGattErrorResp(p_event);
        }
        break;
        case GATTC_EVT_READ_RESP:
        {
            ble_otapc_ProcGattReadResp(p_event);
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            ble_otapc_ProcGattWriteResp(p_event);
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            ble_otapc_ProcGattNotification(p_event);
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.onClientProtocolAvailable.connHandle);
            if (p_conn != NULL)
            {
                if (p_conn->state == BLE_OTAPC_STATE_RETRY_CTRL_CCCD)
                {
                    ble_otapc_EnableCtrlCccd(p_conn);
                }
                else if (p_conn->state == BLE_OTAPC_STATE_RETRY_DATA_CCCD)
                {
                    ble_otapc_EnableDataCccd(p_conn);
                }
                else
                {
					//Shall not enter here
                }
            }
            else
            {
                BLE_OTAPC_Event_T evt;
                evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
                sp_otapcCbRoutine(&evt);
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
 * @brief Registers a callback function for the BLE OTA profile client events.
 *
 * @param[in] bleOtapcRoutine        The callback function to handle BLE OTA profile events.
 *
 */
void BLE_OTAPC_EventRegister(BLE_OTAPC_EventCb_T bleOtapcRoutine)
{
    sp_otapcCbRoutine = bleOtapcRoutine;
}


/**
 * @brief Initializes the BLE Over-The-Air Programming (OTA) profile client.
 *
 * @retval MBA_RES_SUCCESS           The initialization of the BLE OTA profile client is successful
 *
 */
uint16_t BLE_OTAPC_Init(void)
{
    BLE_DD_DiscSvc_T disc;
    uint8_t i;
    uint8_t svc[] = {UUID_OTA_SERVICE_16};
        
    (void)memset(s_otapcConnList, 0x00, sizeof(s_otapcConnList));

    for (i = 0; i < BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        ble_otapc_InitCharList(i);
    }

    disc.svcUuid.uuidLength = ATT_UUID_LENGTH_16;
    (void)memcpy(disc.svcUuid.uuid, svc, ATT_UUID_LENGTH_16);
    disc.p_discInfo = NULL;
    disc.p_discChars = s_otapcDiscCharList;
    disc.p_charList = s_otapcCharList;
    disc.discCharsNum = (uint8_t)OTAPC_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&disc);
}


/**
 * @brief Handles BLE_Stack events.
 *
 * This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack events buffer.
 *
*/
void BLE_OTAPC_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_otapc_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_otapc_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
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
 * @brief The callback function to handle BLE OTA profile events.
 *  
 * An event of type @ref BLE_OTAPC_EVT_FEATURE_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform feature discovery.
 *
 * @retval MBA_RES_SUCCESS           The start of the feature discovery procedure is successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_FeatureDisc(uint16_t connHandle)
{
    BLE_OTAPC_ConnList_T *p_conn;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state < BLE_OTAPC_STATE_DATA_CCCD_ENABLED)
    {
        return MBA_RES_BAD_STATE;
    }

    return GATTC_Read(connHandle, s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_FEATURE].charHandle, 0);
}


/**
 * @brief Initiates a firmware update request operation.
 * 
 * An event of type @ref BLE_OTAPC_EVT_REQ_RSP_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform the firmware update request operation.
 * @param[in] p_req                  Pointer to the request parameters to send to the server.
 *
 * @retval MBA_RES_SUCCESS           The start of the firmware update request operation was successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure..
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateRequest(uint16_t connHandle, BLE_OTAPC_Req_T * p_req)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state < BLE_OTAPC_STATE_DATA_CCCD_ENABLED)
    {
        return MBA_RES_BAD_STATE;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_UPDATE_REQUEST);
        U32_TO_STREAM_LE(&p_buf, p_req->fwImageSize);
        U32_TO_STREAM_LE(&p_buf, p_req->fwImageId);
        U32_TO_STREAM_LE(&p_buf, p_req->fwImageVer);
        U8_TO_STREAM(&p_buf, p_req->fwImageEnc);

        if ((p_conn->fwExtFeature & BLE_OTAPC_FW_FEATURE_MASK1)!=0U)
        {
            U16_TO_STREAM_LE(&p_buf, p_req->fwImageChksum);
            U8_TO_STREAM(&p_buf, p_req->fwImageFileType);
            U16_TO_STREAM_LE(&p_buf, p_req->fwImageCrc16);
        }

        p_writeParams->charLength = (uint16_t)((uint32_t)p_buf - (uint32_t)p_writeParams->charValue);


        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
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
 * @brief Initiates a firmware update start operation.
 *
 * An event of type @ref BLE_OTAPC_EVT_START_RSP_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform the firmware update start operation.
 * @param[in] imgType                The type of image to use for the firmware update process, as defined by @ref BLE_OTAPC_IMG_TYPE_DEF.
 *
 * @retval MBA_RES_SUCCESS           The start of the firmware update start operation was successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateStart(uint16_t connHandle, uint8_t imgType)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
    {
        return MBA_RES_BAD_STATE;
    }


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_UPDATE_START);
        U8_TO_STREAM(&p_buf, imgType);
        p_writeParams->charLength = (uint16_t)((uint32_t)p_buf - (uint32_t)p_writeParams->charValue);


        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);

        OSAL_Free(p_writeParams);

        p_conn->accFragLen = 0;
    }
    else
    {
        result = MBA_RES_OOM;
    }

    return result;

}


/**
 * @brief Distributes a fragmented firmware image to a server device for OTA DFU.
 * 
 * The application should wait for the @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND event after the total length of 
 * sent fragmented images reaches the maximum size before sending the next set of fragments. If the remaining 
 * fragmented image is smaller than the maximum size, @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND will be triggered
 * once the server receives the complete image.
 *  
 * @param[in] connHandle             Handle of the connection for image distribution.
 * @param[in] length                 Length of the fragment, must not exceed ATT_MTU - 3.
 * @param[in] p_data                 Pointer to the fragmented image data.
 *
 * @retval MBA_RES_SUCCESS           Distribution of the fragmented image started successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      Invalid parameters, such as:\n
 *                                      - Invalid connection handle.
 *                                      - Length exceeds ATT_MTU - 3.
 *                                      - Total length of sent fragments plus new length exceeds the maximum image size.
 *@retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_FragmentDist(uint16_t connHandle, uint16_t length, uint8_t *p_data)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (length > (p_conn->attMtu - ATT_WRITE_HEADER_SIZE))
        || (p_conn->accFragLen + length > p_conn->maxFragSize))
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
    {
        return MBA_RES_BAD_STATE;
    }


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_DATA].charHandle;
        
        (void)memcpy(p_writeParams->charValue, p_data, length);
        p_writeParams->charLength = length;
        p_writeParams->writeType = ATT_WRITE_CMD;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);

        OSAL_Free(p_writeParams);

        p_conn->accFragLen += length;
    }
    else
    {
        result = MBA_RES_OOM;
    }

    return result;

}


/**
 * @brief Initiates the firmware update completion process.
 *
 * @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND event will be generated upon completion.
 *
 * @param[in] connHandle             Handle of the connection for completing the firmware update.
 *
 * @retval MBA_RES_SUCCESS           Firmware update completion process started successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateComplete(uint16_t connHandle)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
    {
        return MBA_RES_BAD_STATE;
    }


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_UPDATE_COMPLETE);
        p_writeParams->charLength = (uint16_t)((uint32_t)p_buf - (uint32_t)p_writeParams->charValue);


        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
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
 * @brief Requests a device reset on the server device after OTA DFU.
 *
 * @param[in] connHandle             Handle of the connection for device reset request.
 *
 * @retval MBA_RES_SUCCESS           Device reset request initiated successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_DeviceReset(uint16_t connHandle)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
    {
        return MBA_RES_BAD_STATE;
    }


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_RESET_REQUEST);
        p_writeParams->charLength = (uint16_t)((uint32_t)p_buf - (uint32_t)p_writeParams->charValue);


        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
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
 * @brief Handless BLE Database Discovery (BLE_DD) middleware events.
 *
 * This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event               Pointer to BLE_DD events buffer.
 *
 */
void BLE_OTAPC_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            BLE_OTAPC_ConnList_T *p_conn;

            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);

            if (p_conn != NULL)
            {
                /* By checking the discovered handles exist or not. */
                if (s_otapcCharList[p_conn->connIndex].p_charInfo[OTAPC_INDEX_CHAR_FEATURE].charHandle != 0U)
                {
                    p_conn->state = BLE_OTAPC_STATE_CHAR_DISC_COMP;
                    ble_otapc_EnableCtrlCccd(p_conn);
                }
            }
            else
            {
                BLE_OTAPC_Event_T evt;
                evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
                sp_otapcCbRoutine(&evt);
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

