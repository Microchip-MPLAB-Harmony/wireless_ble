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
    This file contains the BLE OTA Profile Client functions for application user.

  Description:
    This file contains the BLE OTA Profile Client functions for application user.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_otapc/ble_otapc.h"


// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define BLE_OTAPC_OP_CODE_RESPONSE            0x01
#define BLE_OTAPC_OP_CODE_UPDATE_REQUEST      0x02
#define BLE_OTAPC_OP_CODE_UPDATE_START        0x03
#define BLE_OTAPC_OP_CODE_UPDATE_COMPLETE     0x04
#define BLE_OTAPC_OP_CODE_RESET_REQUEST       0x05



#define BLE_OTAPC_STATE_INIT                                    0x00
#define BLE_OTAPC_STATE_CHAR_DISC_COMP                          0x01
#define BLE_OTAPC_STATE_RETRY_CTRL_CCCD                         0x02
#define BLE_OTAPC_STATE_WRITE_CTRL_CCCD                         0x03
#define BLE_OTAPC_STATE_WAIT_SEC                                0x04
#define BLE_OTAPC_STATE_CTRL_CCCD_ENABLED                       0x05
#define BLE_OTAPC_STATE_RETRY_DATA_CCCD                         0x06
#define BLE_OTAPC_STATE_WRITE_DATA_CCCD                         0x07
#define BLE_OTAPC_STATE_DATA_CCCD_ENABLED                       0x08
#define BLE_OTAPC_STATE_REQ_ALLOWED                             0x09


/**@defgroup UUID UUID
 * @brief The definition of UUID
 * @{ */
#define UUID_OTA_SERVICE_16                                  0xD7,0x15,0x82,0x8E,0x1B,0xE6,0x23,0x99,0xB3,0x46,0x3D,0x25,0x50,0x48,0x43,0x4D     /**< Service UUID in little endian format. */
#define UUID_OTA_CHAR_FEATURE                                0x58,0x63,0x90,0x2F,0x4A,0x0C,0x03,0xAF,0x46,0x42,0xE4,0x22,0x50,0x48,0x43,0x4D     /**< Characteristic UUID in little endian format. */
#define UUID_OTA_CHAR_CTRL_PT                                0x76,0x8A,0x02,0x39,0x7F,0xC9,0x82,0x88,0xDE,0x45,0x27,0x93,0x50,0x48,0x43,0x4D     /**< Characteristic UUID in little endian format. */
#define UUID_OTA_CHAR_DATA                                   0x78,0xD4,0x8C,0x7C,0xF5,0x56,0x7E,0xBA,0xA6,0x40,0xD9,0x34,0x50,0x48,0x43,0x4D     /**< Characteristic UUID in little endian format. */
/** @} */

/**@defgroup BLE_OTAPC_MAX_CONN_NBR Maximum connection number
 * @brief The definition of maximum allowed link number of OTA profile client connections.
 * @{ */
#define BLE_OTAPC_MAX_CONN_NBR                BLE_GAP_MAX_LINK_NBR                     /**< Maximum allowed OTA profile client connections. */
/** @} */

/**@brief Enumeration type of BLE OTA profile characteristics. */
typedef enum BLE_OTAPC_CharIndex_T
{
    OTAPC_INDEX_CHAR_FEATURE,                                /**< Index of OTA Feature Characteristic. */
    OTAPC_INDEX_CHAR_CTRL,                                   /**< Index of OTA Control Point characteristic. */
    OTAPC_INDEX_CHAR_CTRL_CCCD,                              /**< Index of OTA Control Point CCCD. */
    OTAPC_INDEX_CHAR_DATA,                                   /**< Index of OTA Data Characteristic. */
    OTAPC_INDEX_CHAR_DATA_CCCD,                              /**< Index of OTA Data Characteristic CCCD. */

    OTAPC_CHAR_NUM                                           /**< Total number of OTA characteristics. */
}BLE_OTAPC_CharIndex_T;

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/**@brief The structure contains information about BLE OTA profile connection parameters for recording connection information. */
typedef struct BLE_OTAPC_ConnList_T
{
    uint8_t                     state;                  /**< BLE OTA profile current state. @ref BLE_OTAPC_STATUS.*/
    uint8_t                     connIndex;
    uint16_t                    connHandle;             /**< Connection handle associated with this connection. */
    uint16_t                    attMtu;
    uint16_t                    maxFragSize;
    uint16_t                    accFragLen;
} BLE_OTAPC_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static BLE_OTAPC_EventCb_T      sp_otapcCbRoutine;

static const ATT_Uuid_T         s_otapcDiscCharFeature =    { {UUID_OTA_CHAR_FEATURE}, ATT_UUID_LENGTH_16 };
static const ATT_Uuid_T         s_otapcDiscCharCtrl =       { {UUID_OTA_CHAR_CTRL_PT}, ATT_UUID_LENGTH_16 };
static const ATT_Uuid_T         s_otapcDiscCharCtrlCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };
static const ATT_Uuid_T         s_otapcDiscCharData =       { {UUID_OTA_CHAR_DATA}, ATT_UUID_LENGTH_16 };
static const ATT_Uuid_T         s_otapcDiscCharDataCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };


static BLE_DD_DiscChar_T        s_otapcFeature =         { &s_otapcDiscCharFeature, 0 };
static BLE_DD_DiscChar_T        s_otapcCtrl =            { &s_otapcDiscCharCtrl, 0 };
static BLE_DD_DiscChar_T        s_otapcCtrlCccd =        { &s_otapcDiscCharCtrlCccd, CHAR_SET_DESCRIPTOR };
static BLE_DD_DiscChar_T        s_otapcData =            { &s_otapcDiscCharData, 0 };
static BLE_DD_DiscChar_T        s_otapcDataCccd =        { &s_otapcDiscCharDataCccd, CHAR_SET_DESCRIPTOR };

static BLE_DD_DiscChar_T        *s_otapcDiscCharList[] =
{
    &s_otapcFeature,                 /* OTA Feature Characteristic */
    &s_otapcCtrl,                    /* OTA Control Point Characteristic */
    &s_otapcCtrlCccd,                /* OTA Control Point Characteristic CCCD */
    &s_otapcData,                    /* OTA Data Characteristic */
    &s_otapcDataCccd,                /* OTA Data Characteristic CCCD */
};

static BLE_DD_CharInfo_T        s_otapcCharInfoList[BLE_OTAPC_MAX_CONN_NBR][OTAPC_CHAR_NUM];
static BLE_DD_CharList_T        s_otapcCharList[BLE_OTAPC_MAX_CONN_NBR];
static BLE_OTAPC_ConnList_T     s_otapcConnList[BLE_OTAPC_MAX_CONN_NBR];

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void ble_otapc_InitCharList(uint8_t connIndex)
{
    s_otapcCharList[connIndex].connHandle = 0;
    s_otapcCharList[connIndex].p_charInfo = (BLE_DD_CharInfo_T *) &(s_otapcCharInfoList[connIndex]);

    memset(s_otapcCharList[connIndex].p_charInfo, 0x00, sizeof(BLE_DD_CharInfo_T) * OTAPC_CHAR_NUM);
}

static BLE_OTAPC_ConnList_T *ble_otapc_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        if (s_otapcConnList[i].connHandle == connHandle)
        {
            return &s_otapcConnList[i];
        }
    }

    return NULL;
}

static BLE_OTAPC_ConnList_T *ble_otapc_GetFreeConnList()
{
    uint8_t i;

    for(i=0; i<BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        if (s_otapcConnList[i].connHandle == 0)
        {
            s_otapcConnList[i].connIndex = i;
            return &s_otapcConnList[i];
        }
    }

    return NULL;
}


static uint16_t ble_otapc_EnableCtrlCccd(BLE_OTAPC_ConnList_T *p_conn)
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
    else
    {
        result = MBA_RES_OOM;
    }

    return result;
}

static uint16_t ble_otapc_EnableDataCccd(BLE_OTAPC_ConnList_T *p_conn)
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
    else
    {
        result = MBA_RES_OOM;
    }

    return result;
}

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
        if (sp_otapcCbRoutine)
        {
            BLE_OTAPC_Event_T evt;

            evt.eventId = BLE_OTAPC_EVT_FEATURE_IND;
            evt.eventField.evtFeatureInd.connHandle = p_conn->connHandle;
            evt.eventField.evtFeatureInd.suppImgType = p_event->eventField.onReadResp.readValue[0];
            
            sp_otapcCbRoutine(&evt);
        }
    }
}

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
        return;

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

            if (sp_otapcCbRoutine)
            {
                BLE_OTAPC_Event_T evtPara;

                evtPara.eventId = BLE_OTAPC_EVT_CONNECT_IND;
                evtPara.eventField.evtConnectInd.connHandle = p_conn->connHandle;
                sp_otapcCbRoutine(&evtPara);
            }
        }
        break;
        default:
        break;
    }
}

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
            p_conn->accFragLen = 0;

        sp_otapcCbRoutine(&evtPara);
    }
    


}


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
        return;

    if (p_conn->state != BLE_OTAPC_STATE_WRITE_CTRL_CCCD)
    {
        BLE_OTAPC_Event_T evt;
        evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
        sp_otapcCbRoutine(&evt);
        return;
    }

    /* Require security permission to access characteristics. Inform application. */
    if ((p_event->eventField.onError.errCode == ATT_ERRCODE_INSUFFICIENT_ENCRYPTION) 
        || (p_event->eventField.onError.errCode == ATT_ERRCODE_INSUFFICIENT_AUTHENTICATION))
    {
        p_conn->state = BLE_OTAPC_STATE_WAIT_SEC;
    
        if (sp_otapcCbRoutine)
        {
            BLE_OTAPC_Event_T evt;

            evt.eventId = BLE_OTAPC_EVT_SEC_REQ;
            evt.eventField.evtSecReq.connHandle = p_event->eventField.onError.connHandle;
            sp_otapcCbRoutine(&evt);
        }
    }
}

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
                memset(p_conn, 0x00, sizeof(BLE_OTAPC_ConnList_T));
                ble_otapc_InitCharList(p_conn->connIndex);
            }
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            if (p_event->eventField.evtEncryptStatus.status == BLE_GAP_ENCRYPT_SUCCESS)
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
                    ble_otapc_EnableCtrlCccd(p_conn);
                else if (p_conn->state == BLE_OTAPC_STATE_RETRY_DATA_CCCD)
                    ble_otapc_EnableDataCccd(p_conn);
            }
        }
        break;

        default:
        break;
    }
}

void ble_otapc_GattEventProcess(GATT_Event_T *p_event)
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
                    ble_otapc_EnableCtrlCccd(p_conn);
                else if (p_conn->state == BLE_OTAPC_STATE_RETRY_DATA_CCCD)
                    ble_otapc_EnableDataCccd(p_conn);
            }
            else
            {
                BLE_OTAPC_Event_T evt;
                evt.eventId = BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND;
                sp_otapcCbRoutine(&evt);
            }
        break;

        default:
            break;
    }
}

void BLE_OTAPC_EventRegister(BLE_OTAPC_EventCb_T bleOtapcRoutine)
{
    sp_otapcCbRoutine = bleOtapcRoutine;
}

uint16_t BLE_OTAPC_Init()
{
    BLE_DD_DiscSvc_T disc;
    uint8_t i;
    uint8_t svc[] = {UUID_OTA_SERVICE_16};
        
    memset(s_otapcConnList, 0x00, sizeof(s_otapcConnList));

    for (i = 0; i < BLE_OTAPC_MAX_CONN_NBR; i++)
    {
        ble_otapc_InitCharList(i);
    }

    disc.svcUuid.uuidLength = ATT_UUID_LENGTH_16;
    memcpy(disc.svcUuid.uuid, svc, ATT_UUID_LENGTH_16);
    disc.p_discChars = s_otapcDiscCharList;
    disc.p_charList = s_otapcCharList;
    disc.discCharsNum = OTAPC_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&disc);
}

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
        break;
    }
}

uint16_t BLE_OTAPC_FeatureDisc(uint16_t connHandle)
{
    BLE_OTAPC_ConnList_T *p_conn;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
        return MBA_RES_INVALID_PARA;

    if (p_conn->state < BLE_OTAPC_STATE_DATA_CCCD_ENABLED)
        return MBA_RES_BAD_STATE;

    return GATTC_Read(connHandle, s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_FEATURE].charHandle, 0);
}

uint16_t BLE_OTAPC_UpdateRequest(uint16_t connHandle, BLE_OTAPC_Req_T * p_req)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
        return MBA_RES_INVALID_PARA;

    if (p_conn->state < BLE_OTAPC_STATE_DATA_CCCD_ENABLED)
        return MBA_RES_BAD_STATE;


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
        p_writeParams->charLength = p_buf - p_writeParams->charValue;


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

uint16_t BLE_OTAPC_UpdateStart(uint16_t connHandle, uint8_t imgType)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
        return MBA_RES_INVALID_PARA;

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
        return MBA_RES_BAD_STATE;


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_UPDATE_START);
        U8_TO_STREAM(&p_buf, imgType);
        p_writeParams->charLength = p_buf - p_writeParams->charValue;


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

uint16_t BLE_OTAPC_FragmentDist(uint16_t connHandle, uint16_t length, uint8_t *p_data)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if ((p_conn == NULL) || (length > (p_conn->attMtu - ATT_WRITE_HEADER_SIZE))
        || (p_conn->accFragLen + length > p_conn->maxFragSize))
        return MBA_RES_INVALID_PARA;

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
        return MBA_RES_BAD_STATE;


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_DATA].charHandle;
        
        memcpy(p_writeParams->charValue, p_data, length);
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

uint16_t BLE_OTAPC_UpdateComplete(uint16_t connHandle)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
        return MBA_RES_INVALID_PARA;

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
        return MBA_RES_BAD_STATE;


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_UPDATE_COMPLETE);
        p_writeParams->charLength = p_buf - p_writeParams->charValue;


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

uint16_t BLE_OTAPC_DeviceReset(uint16_t connHandle)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;
    BLE_OTAPC_ConnList_T *p_conn;
    uint8_t *p_buf;

    p_conn = ble_otapc_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
        return MBA_RES_INVALID_PARA;

    if (p_conn->state != BLE_OTAPC_STATE_REQ_ALLOWED)
        return MBA_RES_BAD_STATE;


    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_otapcCharInfoList[p_conn->connIndex][OTAPC_INDEX_CHAR_CTRL].charHandle;
        
        p_buf = p_writeParams->charValue;
        U8_TO_STREAM(&p_buf, BLE_OTAPC_OP_CODE_RESET_REQUEST);
        p_writeParams->charLength = p_buf - p_writeParams->charValue;


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


void BLE_OTAPC_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            BLE_OTAPC_ConnList_T *p_conn;

            p_conn = ble_otapc_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);

            if (p_conn)
            {
                /* By checking the discovered handles exist or not. */
                if (s_otapcCharList[p_conn->connIndex].p_charInfo[OTAPC_INDEX_CHAR_FEATURE].charHandle != 0)
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
        break;
    }
}

