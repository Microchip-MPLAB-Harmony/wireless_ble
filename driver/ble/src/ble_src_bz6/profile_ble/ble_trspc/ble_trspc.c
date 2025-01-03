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
  BLE Transparent Client Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trspc.c

  Summary:
    Implements the client-side functionality of the BLE Transparent Profile.

  Description:
    This source file provides the implementation for the client-side operations
    of the BLE Transparent Profile, enabling the application to interact with
    the corresponding server-side profile on a remote BLE device.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/mw_assert.h"
#include "ble_util/byte_stream.h"
#include "ble_trspc.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_TRSPC_CCCD_DISABLE                  (0x0000U)       // Client Characteristic Configuration Descriptor (CCCD) disabled state.
#define BLE_TRSPC_CCCD_NOTIFY                   NOTIFICATION    // CCCD notification property enabled state.

#define BLE_TRSPC_CBFC_OPCODE_SUCCESS           (0x00U)         // Response code for successful operation.
#define BLE_TRSPC_CBFC_OPCODE_DL_ENABLED        (0x14U)         // Opcode to enable Credit Based Flow Control (CBFC) for downlink.
#define BLE_TRSPC_CBFC_OPCODE_UL_ENABLED        (0x15U)         // Opcode to enable Credit Based Flow Control (CBFC) for uplink.

#define BLE_TRSPC_VENDOR_OPCODE_MIN             (0x20U)         // Minimum opcode value for TRS vendor-specific commands.
#define BLE_TRSPC_VENDOR_OPCODE_MAX             (0xFFU)         // Maximum opcode value for TRS vendor-specific commands.

#define BLE_TRSPC_INIT_CREDIT                   (0x10U)         // Initial credit value for CBFC.

// Maximum number of incoming buffers, based on initial credit and max connections.
#define BLE_TRSPC_MAX_BUF_IN                    (BLE_TRSPC_INIT_CREDIT*BLE_TRSPC_MAX_CONN_NBR)

#define BLE_TRSPC_MAX_RETURN_CREDIT             (13U)           // Maximum number of credits that can be returned.

#define CBFC_PROC_IDLE                          (0x00U)         // CBFC procdure: Idle.
#define CBFC_PROC_ENABLE_SESSION                (0x01U)         // CBFC procdure: Enable Control Point CCCD.
#define CBFC_PROC_ENABLE_TCP_CCCD               (0x02U)         // CBFC procdure: Enable TCP CCCD.
#define CBFC_PROC_ENABLE_TDD_CBFC               (0x03U)         // CBFC procdure: Enable TDD CBFC.
#define CBFC_PROC_ENABLE_TUD_CCCD               (0x04U)         // CBFC procdure: Enable TUD CCCD.
#define CBFC_PROC_DISABLE_TUD_CCCD              (0x05U)         // CBFC procdure: Disable TUD CCCD.

#define VENCOM_PROC_IDLE                        (0x00U)         // Vendor command response procdure: Idle.
#define VENCOM_PROC_ENABLE                      (0x01U)         // Vendor command response procdure: Enable.

#define BLE_TRSPC_CBFC_DISABLED                 (0x00U)         // CBFC downlink/uplink disabled state for BLE transparent service.
#define BLE_TRSPC_CBFC_DL_ENABLED               (0x01U)         // CBFC downlink enabled state for BLE transparent service.
#define BLE_TRSPC_CBFC_UL_ENABLED               (0x02U)         // CBFC uplink enabled state for BLE transparent service.

#define BLE_TRSPC_CP_STATUS_DISABLED            (0x00U)         // Transparent control point CCCD disabled state.
#define BLE_TRSPC_CP_STATUS_ENABLED             (0x80U)         // Transparent control point CCCD enabled state (Notify).

// MCHP proprietary service UUID (little endian).
#define UUID_MCHP_TRANS_SVC                     0x55,0xE4,0x05,0xD2,0xAF,0x9F,0xA9,0x8F,0xE5,0x4A,0x7D,0xFE,0x43,0x53,0x53,0x49

// MCHP Transparent TUD characteristic UUID (little endian).
#define UUID_MCHP_CHAR_TUD                      0x16,0x96,0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49

// MCHP Transparent TDD characteristic UUID (little endian).
#define UUID_MCHP_CHAR_TDD                      0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49

// MCHP Transparent TCP characteristic UUID (little endian).
#define UUID_MCHP_CHAR_TCP                      0x7e,0x3b,0x07,0xff,0x1c,0x51,0x49,0x2f,0xb3,0x39,0x8a,0x4c,0x43,0x53,0x53,0x49

typedef enum BLE_TRSPC_CharIndex_T
{
    TRSPC_INDEX_CHARTUD = 0x00U,                // Index for Transparent Uplink Data Characteristic.
    TRSPC_INDEX_CHARTUDCCCD,                    // Index for Transparent Uplink Data CCCD.
    TRSPC_INDEX_CHARTDD,                        // Index for Transparent Downlink Data Characteristic.
    TRSPC_INDEX_CHARTCP,                        // Index for Transparent Control Point Characteristic.
    TRSPC_INDEX_CHARTCPCCCD,                    // Index for Transparent Control Point CCCD.
    TRSPC_CHAR_NUM                              // Total number of TRS characteristics.
}BLE_TRSPC_CharIndex_T;

typedef enum BLE_TRSPC_State_T
{
    BLE_TRSPC_STATE_IDLE = 0x00,                // The BLE Transparent profile is in an idle state.
    BLE_TRSPC_STATE_CONNECTED                   // The BLE Transparent profile is in a connected state.
} BLE_TRSPC_State_T;


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Structure for BLE transparent profile packet information. */
typedef struct BLE_TRSPC_PacketList_T
{
    uint16_t                   length;                  // Length of the data in bytes.
    uint8_t                    *p_packet;               // Pointer to the data buffer for transmission or reception.
} BLE_TRSPC_PacketList_T;

/* Structure for managing the input queue of packets in the BLE transparent profile. */
typedef struct BLE_TRSPC_QueueIn_T
{
    uint8_t                    usedNum;                 // Number of packets currently in the input buffer.
    uint8_t                    writeIndex;              // Index at which a new packet will be written to the buffer.
    uint8_t                    readIndex;               // Index from which the next packet will be read from the buffer.
    BLE_TRSPC_PacketList_T     packetList[BLE_TRSPC_INIT_CREDIT];  // Array of packet buffers.
} BLE_TRSPC_QueueIn_T;

/* Structure for storing connection-specific information in the BLE transparent profile. */
typedef struct BLE_TRSPC_ConnList_T
{
    uint8_t                     trsState;               // Current state of the BLE transparent service.
    uint8_t                     connIndex;              // Index identifying the connection.
    uint16_t                    connHandle;             // Handle for the BLE connection.
    uint16_t                    attMtu;                 // MTU size for the current connection.
    uint8_t                     cbfcConfig;             // Configuration for credit-based flow control.
    uint8_t                     cbfcProcedure;          // Ongoing procedure for configuring credit-based flow control.
    uint8_t                     localCredit;            // Credits available for downlink (Client to Server).
    uint8_t                     peerCredit;             // Credits available for uplink (Server to Client).
    uint8_t                     vendorCmdProc;          // Procedure for handling vendor-specific commands.
    BLE_TRSPC_QueueIn_T         inputQueue;             // Queue for incoming packets.
    uint8_t                     cbfcRetryProcedure;     // Procedure for retrying credit-based flow control configuration.
    uint8_t                     sessionReqAuth;         // Indicates if data session requires authentication.
    BLE_TRSPC_State_T           state;                  // State of the BLE connection.
} BLE_TRSPC_ConnList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback function type for BLE Transparent profile events processing.
static BLE_TRSPC_EventCb_T      bleTrspcProcess;

// Connection list for BLE Transparent profile.
static BLE_TRSPC_ConnList_T     s_trspcConnList[BLE_TRSPC_MAX_CONN_NBR];

// Characteristic information list for BLE Transparent profile
static BLE_DD_CharInfo_T        s_trsCharInfoList[BLE_TRSPC_MAX_CONN_NBR][TRSPC_CHAR_NUM];

// UUID for the Microchip Transparent Service.
static const uint8_t            discSvcUuid[] =     {UUID_MCHP_TRANS_SVC};

// UUID for the Transparent Uplink Data Characteristic. 
static const ATT_Uuid_T         discCharTud =       { {UUID_MCHP_CHAR_TUD}, ATT_UUID_LENGTH_16 };

// UUID for the Transparent Uplink Data Characteristic's Client Characteristic Configuration Descriptor (CCCD).
static const ATT_Uuid_T         discCharTudCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// UUID for the Transparent Downlink Data Characteristic.
static const ATT_Uuid_T         discCharTdd =       { {UUID_MCHP_CHAR_TDD}, ATT_UUID_LENGTH_16 };

// UUID for the Transparent Control Point Characteristic.
static const ATT_Uuid_T         discCharTcp =       { {UUID_MCHP_CHAR_TCP}, ATT_UUID_LENGTH_16 };

// UUID for the Transparent Control Point Characteristic's Client Characteristic Configuration Descriptor (CCCD). 
static const ATT_Uuid_T         discCharTcpCccd =   { {UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// Discovery structure for the Transparent Uplink Data Characteristic. 
static BLE_DD_DiscChar_T        trsTud =            { &discCharTud, 0 };

// Discovery structure for the Transparent Uplink Data Characteristic CCCD.
static BLE_DD_DiscChar_T        trsTudCccd =        { &discCharTudCccd, CHAR_SET_DESCRIPTOR };

// Discovery structure for the Transparent Downlink Data Characteristic.
static BLE_DD_DiscChar_T        trsTdd =            { &discCharTdd, 0 };

// Discovery structure for the Transparent Control Point Characteristic.
static BLE_DD_DiscChar_T        trsTcp =            { &discCharTcp, 0 };

// Discovery structure for the Transparent Control Point Characteristic CCCD.
static BLE_DD_DiscChar_T        trsTcpCccd =        { &discCharTcpCccd, CHAR_SET_DESCRIPTOR };

static BLE_DD_DiscChar_T        *trsDiscCharList[] =
{
    &trsTud,                    // Transparent Uplink Data Characteristic.
    &trsTudCccd,                // Transparent Uplink Data Characteristic CCCD.
    &trsTdd,                    // Transparent Downlink Data Characteristic.
    &trsTcp,                    // Transparent Control Point Characteristic.
    &trsTcpCccd                 // Transparent Control Point Characteristic CCCD.
};

// Characteristic list for the BLE Transparent Service.
static BLE_DD_CharList_T        s_trsCharList[BLE_TRSPC_MAX_CONN_NBR];

// Assertion to ensure the buffer initialization matches the connection number and initial credits.
MW_ASSERT((BLE_TRSPC_MAX_CONN_NBR*BLE_TRSPC_INIT_CREDIT)==BLE_TRSPC_MAX_BUF_IN);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static void ble_trspc_ProcessQueuedTask(void);
static void ble_trspc_OnLinkEncrypted(uint16_t connHandle);

/** 
 * @brief Initialize a BLE Transparent Service Connection List.
 *
 * @param p_conn Pointer to the BLE Transparent Service Connection List to initialize.
 */
static void ble_trspc_InitConnList(BLE_TRSPC_ConnList_T *p_conn)
{
    (void)memset((uint8_t *)p_conn, 0, sizeof(BLE_TRSPC_ConnList_T));
    p_conn->attMtu= BLE_ATT_DEFAULT_MTU_LEN;
}


/**
 * @brief Retrieve a connection list entry by its connection handle.
 *
 * @param connHandle The connection handle to search for.
 * 
 * @retval Pointer to the corresponding connection list entry, or NULL if not found.
 */
static BLE_TRSPC_ConnList_T *ble_trspc_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        if ((s_trspcConnList[i].state == BLE_TRSPC_STATE_CONNECTED) && (s_trspcConnList[i].connHandle == connHandle))
        {
            return &s_trspcConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Get a free connection list entry.
 *
 * @retval Pointer to a free connection list entry, or NULL if all entries are in use.
 */
static BLE_TRSPC_ConnList_T *ble_trspc_GetFreeConnList(void)
{
    uint8_t i;

    for(i=0; i<BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        if (s_trspcConnList[i].state == BLE_TRSPC_STATE_IDLE)
        {
            s_trspcConnList[i].state = BLE_TRSPC_STATE_CONNECTED;
            s_trspcConnList[i].connIndex = i;
            return &s_trspcConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Initialize a list of BLE Device Discovery Characteristics.
 *
 * @param p_charList    Pointer to the characteristic list to initialize.
 * @param connIndex     Index of the connection for which the characteristic list is associated.
 */
static void ble_trspc_InitCharList(BLE_DD_CharList_T *p_charList, uint8_t connIndex)
{
    uint8_t i;

    p_charList->connHandle = 0;
    p_charList->p_charInfo = (BLE_DD_CharInfo_T *) &(s_trsCharInfoList[connIndex]);

    for(i=0U; i<(uint8_t)TRSPC_CHAR_NUM; i++)
    {
        s_trsCharInfoList[connIndex][i].charHandle = 0;
        s_trsCharInfoList[connIndex][i].property = 0;
    }
}

/**
 * @brief Enable notifications for the Transparent Service Control Point characteristic.
 *
 * @param p_conn Pointer to the connection list entry for which to enable notifications.
 */
static void ble_trspc_EnableControlPointCccd(BLE_TRSPC_ConnList_T *p_conn)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCPCCCD].charHandle;
        p_writeParams->charLength = 0x02;
        U16_TO_BUF_LE(p_writeParams->charValue, BLE_TRSPC_CCCD_NOTIFY);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->cbfcProcedure = CBFC_PROC_ENABLE_TCP_CCCD;
            p_conn->cbfcRetryProcedure = CBFC_PROC_IDLE;
        }
        else
        {
            p_conn->cbfcRetryProcedure = CBFC_PROC_ENABLE_SESSION;
        }
        OSAL_Free(p_writeParams);
    }
}

/**
 * @brief Enable downlink credit-based flow control for the Transparent Service.
 *
 * @param p_conn Pointer to the connection list entry for which to enable flow control.
 */
static void ble_trspc_EnableDownlinkCreditBaseFlowControl(BLE_TRSPC_ConnList_T *p_conn)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCP].charHandle;
        p_writeParams->charLength = 0x01;
        p_writeParams->charValue[0] = BLE_TRSPC_CBFC_OPCODE_DL_ENABLED;
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->cbfcProcedure = CBFC_PROC_ENABLE_TDD_CBFC;
            p_conn->cbfcRetryProcedure = CBFC_PROC_IDLE;
        }
        else
        {
            p_conn->cbfcRetryProcedure = CBFC_PROC_ENABLE_TCP_CCCD;
        }
        OSAL_Free(p_writeParams);
    }
}

/**
 * @brief Return uplink credits to the peer device for the Transparent Service.
 *
 * @param p_conn Pointer to the connection list entry for which to return credits.
 */
static void ble_trspc_ClientReturnCredit(BLE_TRSPC_ConnList_T *p_conn)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCP].charHandle;
        p_writeParams->charLength = 0x02;
        p_writeParams->charValue[0] = BLE_TRSPC_CBFC_OPCODE_UL_ENABLED;
        p_writeParams->charValue[1] = p_conn->peerCredit;
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->peerCredit = 0;
        }
        OSAL_Free(p_writeParams);
    }
}


/**
 * @brief Configure the Client Characteristic Configuration Descriptor (CCCD) for uplink data.
 *
 * @param p_conn Pointer to the connection list entry for which to configure the CCCD.
 * @param cccdValue The value to write to the CCCD.
 */
static void ble_trspc_ConfigureUplinkDataCccd(BLE_TRSPC_ConnList_T *p_conn, uint16_t cccdValue)
{
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTUDCCCD].charHandle;
        p_writeParams->charLength = (uint16_t)sizeof(cccdValue);
        U16_TO_BUF_LE(p_writeParams->charValue, cccdValue);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;

        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            if (cccdValue == BLE_TRSPC_CCCD_NOTIFY)
            {
                p_conn->cbfcProcedure = CBFC_PROC_ENABLE_TUD_CCCD;
            }
            else
            {
                p_conn->cbfcProcedure = CBFC_PROC_DISABLE_TUD_CCCD;
            }
            p_conn->cbfcRetryProcedure = CBFC_PROC_IDLE;
        }
        else
        {
            p_conn->cbfcRetryProcedure = CBFC_PROC_ENABLE_TDD_CBFC;
        }
        OSAL_Free(p_writeParams);
    }
}

/**
 * @brief Enable data session for the given connection handle.
 *
 * @param connHandle The connection handle for which to enable the data session.
 * @param cbfcConfig The credit-based flow control configuration to be enabled.
 */
static void ble_trspc_EnableDataSession(uint16_t connHandle, uint8_t cbfcConfig)
{
    BLE_TRSPC_ConnList_T *p_conn;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);

    if (p_conn != NULL)
    {
        p_conn->cbfcConfig |= cbfcConfig;

        if ((p_conn->cbfcConfig&BLE_TRSPC_CBFC_DL_ENABLED)!= 0U)
        {
            ble_trspc_EnableControlPointCccd(p_conn);
        }
        else
        {
            p_conn->trsState |= BLE_TRSPC_DL_STATUS_NONCBFCENABLED;
            if ((p_conn->cbfcConfig&BLE_TRSPC_CBFC_UL_ENABLED)!= 0U)
            {
                ble_trspc_ConfigureUplinkDataCccd(p_conn, BLE_TRSPC_CCCD_NOTIFY);
            }
        }
    }
}


/**
 * @brief Process received data for a given connection.
 *
 * @param p_conn            Pointer to the connection list entry.
 * @param receivedLen       Length of the received data.
 * @param p_receivedValue   Pointer to the buffer containing the received data.
 */
static void ble_trspc_RcvData(BLE_TRSPC_ConnList_T *p_conn, uint16_t receivedLen, uint8_t *p_receivedValue)
{
    if (p_conn->inputQueue.usedNum < (uint8_t)BLE_TRSPC_INIT_CREDIT)
    {
        BLE_TRSPC_Event_T evtPara;
        uint8_t *p_buffer = NULL;

        (void)memset((uint8_t *) &evtPara, 0, sizeof(evtPara));
        p_buffer = OSAL_Malloc(receivedLen);

        if (p_buffer == NULL)
        {
            evtPara.eventId = BLE_TRSPC_EVT_ERR_NO_MEM;
            if (bleTrspcProcess != NULL)
            {
                bleTrspcProcess(&evtPara);
            }
            return;
        }

        (void)memcpy(p_buffer, p_receivedValue, receivedLen);
        p_conn->inputQueue.packetList[p_conn->inputQueue.writeIndex].length = receivedLen;
        p_conn->inputQueue.packetList[p_conn->inputQueue.writeIndex].p_packet = p_buffer;
        p_conn->inputQueue.writeIndex++;
        if (p_conn->inputQueue.writeIndex >= BLE_TRSPC_INIT_CREDIT)
        {
            p_conn->inputQueue.writeIndex = 0;
        }

        p_conn->inputQueue.usedNum++;

        evtPara.eventId = BLE_TRSPC_EVT_RECEIVE_DATA;
        evtPara.eventField.onReceiveData.connHandle = p_conn->connHandle;
        if (bleTrspcProcess != NULL)
        {
            bleTrspcProcess(&evtPara);
        }
    }
}


/**
 * @brief Process GATT write response for a given connection.
 *
 * @param p_conn Pointer to the connection list entry.
 */
static void ble_trspc_ProcGattWriteResp(BLE_TRSPC_ConnList_T *p_conn)
{
    BLE_TRSPC_Event_T evtPara;

    (void)memset((uint8_t *) &evtPara, 0, sizeof(evtPara));

    switch (p_conn->cbfcProcedure)
    {
        case CBFC_PROC_ENABLE_SESSION:
        {
            ble_trspc_EnableControlPointCccd(p_conn);
        }
        break;
        case CBFC_PROC_ENABLE_TCP_CCCD:
        {
            p_conn->cbfcProcedure = CBFC_PROC_IDLE;
            p_conn->trsState |= BLE_TRSPC_CP_STATUS_ENABLED;
            ble_trspc_EnableDownlinkCreditBaseFlowControl(p_conn);
        }
        break;

        case CBFC_PROC_ENABLE_TDD_CBFC:
        {
            p_conn->cbfcProcedure = CBFC_PROC_IDLE;
            if ((p_conn->cbfcConfig&BLE_TRSPC_CBFC_UL_ENABLED) != 0U)
            {
                ble_trspc_ConfigureUplinkDataCccd(p_conn, BLE_TRSPC_CCCD_NOTIFY);
            }
        }
        break;

        case CBFC_PROC_ENABLE_TUD_CCCD:
        {
            p_conn->cbfcProcedure = CBFC_PROC_IDLE;
            p_conn->trsState |= BLE_TRSPC_UL_STATUS_CBFCENABLED;
            /* Initialize and give credits after connected */
            p_conn->peerCredit = BLE_TRSPC_INIT_CREDIT;
            ble_trspc_ClientReturnCredit(p_conn);

            if (bleTrspcProcess != NULL)
            {
                evtPara.eventId = BLE_TRSPC_EVT_UL_STATUS;
                evtPara.eventField.onUplinkStatus.connHandle = p_conn->connHandle;
                evtPara.eventField.onUplinkStatus.status = BLE_TRSPC_UL_STATUS_CBFCENABLED;
                bleTrspcProcess(&evtPara);
            }
        }
        break;

        case CBFC_PROC_DISABLE_TUD_CCCD:
        {
            p_conn->cbfcProcedure = CBFC_PROC_IDLE;
            p_conn->trsState &= (uint8_t)(~BLE_TRSPC_UL_STATUS_CBFCENABLED);
            if (bleTrspcProcess != NULL)
            {
                evtPara.eventId = BLE_TRSPC_EVT_UL_STATUS;
                evtPara.eventField.onUplinkStatus.connHandle = p_conn->connHandle;
                evtPara.eventField.onUplinkStatus.status = BLE_TRSPC_UL_STATUS_DISABLED;
                bleTrspcProcess(&evtPara);
            }

            /* Reset credit */
            p_conn->peerCredit = 0;
        }
        break;

        default:
        {
            if (p_conn->vendorCmdProc != VENCOM_PROC_IDLE)
            {
                p_conn->vendorCmdProc = VENCOM_PROC_IDLE;
                if (bleTrspcProcess != NULL)
                {
                    evtPara.eventId = BLE_TRSPC_EVT_VENDOR_CMD_RSP;
                    evtPara.eventField.onVendorCmdRsp.connHandle = p_conn->connHandle;
                    bleTrspcProcess(&evtPara);
                }
            }
        }
        break;
    }
}


/**
 * @brief Process GATT notifications for a given connection.
 *
 * @param p_conn        Pointer to the connection list entry.
 * @param p_event       Pointer to the GATT event containing the notification data.
 * @param charHandle    The characteristic handle for which the notification was received.
 */
static void ble_trspc_ProcGattNotification(BLE_TRSPC_ConnList_T *p_conn, GATT_EvtReceiveHandleValue_T *p_event , uint16_t charHandle)
{
    BLE_TRSPC_Event_T evtPara;

    (void)memset((uint8_t *) &evtPara, 0, sizeof(evtPara));

    if (charHandle == s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTUD].charHandle)
    {
        ble_trspc_RcvData(p_conn, p_event->receivedLength, p_event->receivedValue);
    }
    else if (charHandle == s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCP].charHandle)
    {
        /* Opcode: response */
        if (p_event->receivedValue[0] == BLE_TRSPC_CBFC_OPCODE_SUCCESS)
        {
            /* Request opcode of response */
            if (p_event->receivedValue[1] == BLE_TRSPC_CBFC_OPCODE_DL_ENABLED)
            {
                p_conn->trsState |= BLE_TRSPC_DL_STATUS_CBFCENABLED;
                BUF_BE_TO_U16(&p_conn->attMtu, &p_event->receivedValue[2]);
                p_conn->localCredit += p_event->receivedValue[4];

                if (bleTrspcProcess != NULL)
                {
                    evtPara.eventId = BLE_TRSPC_EVT_DL_STATUS;
                    evtPara.eventField.onDownlinkStatus.connHandle = p_conn->connHandle;
                    evtPara.eventField.onDownlinkStatus.status = BLE_TRSPC_DL_STATUS_CBFCENABLED;
                    evtPara.eventField.onDownlinkStatus.currentCreditNumber = p_conn->localCredit;
                    bleTrspcProcess(&evtPara);
                }
            }
        }
        else if (p_event->receivedValue[0]>=BLE_TRSPC_VENDOR_OPCODE_MIN)
        {
            if (bleTrspcProcess != NULL)
            {
                evtPara.eventId = BLE_TRSPC_EVT_VENDOR_CMD;
                evtPara.eventField.onVendorCmd.connHandle = p_conn->connHandle;
                evtPara.eventField.onVendorCmd.payloadLength = (uint8_t)p_event->receivedLength;
                evtPara.eventField.onVendorCmd.p_payLoad = p_event->receivedValue;

                bleTrspcProcess(&evtPara);
            }
        }
        else
        {
			//Shall not enter here
        }
    }
    else
    {
		//Shall not enter here
    }
}

/**
 * @brief Process GATT events.
 *
 * @param p_event Pointer to the GATT event to be processed.
 */
static void ble_trspc_GattEventProcess(GATT_Event_T *p_event)
{
    BLE_TRSPC_ConnList_T *p_conn = NULL;

    switch (p_event->eventId)
    {
        case ATT_EVT_TIMEOUT:
        {
            /* Reset link information */
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn != NULL)
            {
                p_conn->attMtu = p_event->eventField.onUpdateMTU.exchangedMTU;
            }
        }
        break;

        case GATTC_EVT_ERROR_RESP:
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.onError.connHandle);
            if (p_conn != NULL)
            {
                if ((p_event->eventField.onError.reqOpcode == ATT_WRITE_REQ) &&
                    (p_event->eventField.onError.errCode == ATT_ERR_INSUF_AUTHN || 
                    p_event->eventField.onError.errCode == ATT_ERR_INSUF_ENC) &&
                    (p_event->eventField.onError.attrHandle == s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCPCCCD].charHandle))
                {
                    p_conn->sessionReqAuth = 1;
                }
            }
            break;

        case GATTC_EVT_WRITE_RESP:
        {
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.onWriteResp.connHandle);

            if (p_conn != NULL)
            {
                if (p_event->eventField.onWriteResp.charHandle != s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTUD].charHandle &&
                    p_event->eventField.onWriteResp.charHandle != s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTUDCCCD].charHandle &&
                    p_event->eventField.onWriteResp.charHandle != s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTDD].charHandle &&
                    p_event->eventField.onWriteResp.charHandle != s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCP].charHandle &&
                    p_event->eventField.onWriteResp.charHandle != s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCPCCCD].charHandle)
                {
                    break;
                }

                ble_trspc_ProcGattWriteResp(p_conn);
            }
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.onNotification.connHandle);

            if (p_conn != NULL)
            {
                ble_trspc_ProcGattNotification(p_conn, &p_event->eventField.onNotification, p_event->eventField.onNotification.charHandle);
            }
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.onClientProtocolAvailable.connHandle);
            if ((p_conn != NULL) && (p_conn->cbfcRetryProcedure!=0U))
            {
                p_conn->cbfcProcedure = p_conn->cbfcRetryProcedure;
                p_conn->cbfcRetryProcedure = CBFC_PROC_IDLE;
                ble_trspc_ProcGattWriteResp(p_conn);
            }
            else if (p_conn != NULL)
            {
                ble_trspc_ProcessQueuedTask();
            }
            else
            {
				//Shall not enter here
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
 * @brief Check if there are any queued tasks that need processing.
 *
 * @retval true if there are queued tasks, false otherwise.
 */
static bool ble_trspc_CheckQueuedTask(void)
{
    uint8_t i;

    for (i = 0; i < BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        if ((s_trspcConnList[i].state == BLE_TRSPC_STATE_CONNECTED) && (s_trspcConnList[i].peerCredit >= BLE_TRSPC_MAX_RETURN_CREDIT))
        {
            return true;
        }
    }

    return false;
}


/**
 * @brief Process queued tasks.
 */
static void ble_trspc_ProcessQueuedTask(void)
{
    uint8_t i;

    for(i = 0; i < BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        if (s_trspcConnList[i].state == BLE_TRSPC_STATE_CONNECTED)
        {
            if (s_trspcConnList[i].peerCredit >= BLE_TRSPC_MAX_RETURN_CREDIT)
            {
                ble_trspc_ClientReturnCredit(&s_trspcConnList[i]);
            }
        }
    }
}

/**
 * @brief Process GAP events.
 *
 * @param p_event Pointer to the GAP event to be processed.
 */
static void ble_trspc_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_TRSPC_ConnList_T *p_conn = NULL;

    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                p_conn = ble_trspc_GetFreeConnList();
                if (p_conn != NULL)
                {
                    p_conn->connHandle = p_event->eventField.evtConnect.connHandle;
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);
            if (p_conn != NULL)
            {
                // Flush all queued data.
                while (p_conn->inputQueue.usedNum > 0U)
                {
                    if (p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet != NULL)
                    {
                        OSAL_Free(p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet);
                        p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet = NULL;
                    }

                    p_conn->inputQueue.readIndex++;
                    if (p_conn->inputQueue.readIndex >= BLE_TRSPC_INIT_CREDIT)
                    {
                        p_conn->inputQueue.readIndex = 0;
                    }

                    p_conn->inputQueue.usedNum--;
                }
                ble_trspc_InitConnList(p_conn);
            }
        }
        break;

        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_trspc_ProcessQueuedTask();
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            if (p_event->eventField.evtEncryptStatus.status == GAP_STATUS_SUCCESS)
            {
                ble_trspc_OnLinkEncrypted(p_event->eventField.evtEncryptStatus.connHandle);
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
 * @brief Handle link encryption event.
 *
 * @param connHandle The connection handle for which the link encryption event occurred.
 */
static void ble_trspc_OnLinkEncrypted(uint16_t connHandle)
{
    uint8_t i;
    BLE_TRSPC_ConnList_T *p_conn;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);
    
    /* Check if TRS characteristics are found or not. */
    for (i=0; i<BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        if (s_trsCharList[i].connHandle == connHandle)
        {
            /* By checking the discovered handles exist or not. */
            if (s_trsCharList[i].p_charInfo[TRSPC_INDEX_CHARTCP].charHandle != 0U)
            {
                if (p_conn != NULL && p_conn->sessionReqAuth == 1U)
                {
                    ble_trspc_EnableDataSession(connHandle, (BLE_TRSPC_CBFC_DL_ENABLED|BLE_TRSPC_CBFC_UL_ENABLED));
                }
            }
            break;
        }
    }
}

/**
 * @brief Registers a callback function for BLE Transparent profile client events.
 *
 * @param[in] bleTranCliHandler             The client callback function to handle BLE Transparent Profile client events.
 *
 */
void BLE_TRSPC_EventRegister(BLE_TRSPC_EventCb_T bleTranCliHandler)
{
    bleTrspcProcess = bleTranCliHandler;
}


/**
 * @brief Initializes the BLE Transparent Profile Client.
 *
 * @retval MBA_RES_SUCCESS                  BLE Transparent Profile Client was successfully initialized.
 * @retval MBA_RES_FAIL                     BLE Transparent Profile Client failed to initialize.
 *
 */
uint16_t BLE_TRSPC_Init(void)
{
    BLE_DD_DiscSvc_T trsDisc;
    uint8_t i;

    /* Reset connection information */
    for (i = 0; i < BLE_TRSPC_MAX_CONN_NBR; i++)
    {
        ble_trspc_InitConnList(&s_trspcConnList[i]);
        ble_trspc_InitCharList(&s_trsCharList[i], i);
    }

    trsDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_16;
    (void)memcpy(trsDisc.svcUuid.uuid, discSvcUuid, ATT_UUID_LENGTH_16);
    trsDisc.p_discInfo = NULL;
    trsDisc.p_discChars = trsDiscCharList;
    trsDisc.p_charList = s_trsCharList;
    trsDisc.discCharsNum = (uint8_t)TRSPC_CHAR_NUM;
    return BLE_DD_ServiceDiscoveryRegister(&trsDisc);
}


/**
 * @brief Sends a vendor-specific command over the BLE connection.
 *
 * @param[in] connHandle                    Connection handle associated with this connection.
 * @param[in] commandID                     The identifier for the vendor-specific command.
 * @param[in] commandLength                 The length of the command payload.
 * @param[in] p_commandPayload              Pointer to the command payload data.
 *
 * @retval MBA_RES_SUCCESS                  The command was successfully sent.
 * @retval MBA_RES_FAIL                     The connection is invalid or the CCCD of TCP is not enabled.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA             The commandID or commandLength is invalid.
 *
 */
uint16_t BLE_TRSPC_SendVendorCommand(uint16_t connHandle, uint8_t commandID, uint8_t commandLength, uint8_t *p_commandPayload)
{
    BLE_TRSPC_ConnList_T *p_conn;
    GATTC_WriteParams_T *p_writeParams;
    uint16_t result;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return MBA_RES_FAIL;
    }

    if ((p_conn->trsState&BLE_TRSPC_CP_STATUS_ENABLED)==0U)
    {
        return MBA_RES_FAIL;
    }

    if (commandID < BLE_TRSPC_VENDOR_OPCODE_MIN)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (commandLength > (p_conn->attMtu-ATT_WRITE_HEADER_SIZE-1U))
    {
        return MBA_RES_INVALID_PARA;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
    if (p_writeParams != NULL)
    {
        p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTCP].charHandle;
        p_writeParams->charLength = ((uint16_t)commandLength+1U);
        p_writeParams->charValue[0] = commandID;
        (void)memcpy(&p_writeParams->charValue[1], p_commandPayload, commandLength);
        p_writeParams->writeType = ATT_WRITE_REQ;
        p_writeParams->valueOffset = 0x0000;
        p_writeParams->flags = 0;
        result = GATTC_Write(p_conn->connHandle, p_writeParams);
        if (result == MBA_RES_SUCCESS)
        {
            p_conn->vendorCmdProc = VENCOM_PROC_ENABLE;
        }
        OSAL_Free(p_writeParams);
        return result;
    }
    else
    {
        return MBA_RES_OOM;
    }
}


/**
 * @brief Sends transparent data to the connected BLE device.
 *
 * @param[in] connHandle                    Connection handle associated with this connection.
 * @param[in] len                           The length of the data to be sent.
 * @param[in] p_data                        Pointer to the data to be sent.
 *
 * @retval MBA_RES_SUCCESS                  The data was successfully sent.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA             The parameters do not meet the specifications.
 *
 */
uint16_t BLE_TRSPC_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data)
{
    GATTC_WriteParams_T *p_writeParams;
    BLE_TRSPC_ConnList_T *p_conn;
    uint16_t result;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return MBA_RES_FAIL;
    }

    if (((p_conn->trsState&BLE_TRSPC_DL_STATUS_CBFCENABLED) != 0U) && (p_conn->localCredit == 0U))
    {
        return MBA_RES_NO_RESOURCE;
    }

    if (ble_trspc_CheckQueuedTask())
    {
        return MBA_RES_NO_RESOURCE;
    }

    if (len > (p_conn->attMtu - ATT_WRITE_HEADER_SIZE))
    {
        return MBA_RES_FAIL;
    }

    p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));

    if (p_writeParams == NULL)
    {
        return MBA_RES_OOM;
    }

    if ((p_conn->trsState & BLE_TRSPC_DL_STATUS_CBFCENABLED) != 0U)
    {
        p_writeParams->writeType = ATT_WRITE_CMD;
    }
    else if ((p_conn->trsState & BLE_TRSPC_DL_STATUS_NONCBFCENABLED) != 0U)
    {
        p_writeParams->writeType = ATT_WRITE_REQ;
    }
    else
    {
        OSAL_Free(p_writeParams);
        return MBA_RES_BAD_STATE;
    }

    p_writeParams->charHandle = s_trsCharInfoList[p_conn->connIndex][TRSPC_INDEX_CHARTDD].charHandle;
    p_writeParams->charLength = len;
    (void)memcpy(p_writeParams->charValue, p_data, len);
    p_writeParams->valueOffset = 0;
    p_writeParams->flags = 0;
    result = GATTC_Write(connHandle, p_writeParams);
    OSAL_Free(p_writeParams);

    if (result == MBA_RES_SUCCESS)
    {
        if ((p_conn->trsState & BLE_TRSPC_DL_STATUS_CBFCENABLED) != 0U)
        {
            p_conn->localCredit--;
        }
    }

    return result;
}


/**
 * @brief Retrieves the length of data queued.
 *
 * @param[in]  connHandle                   Connection handle associated with the queued data.
 * @param[out] p_dataLength                 Pointer to where the data length will be stored.
 *
 */
void BLE_TRSPC_GetDataLength(uint16_t connHandle, uint16_t *p_dataLength)
{
    BLE_TRSPC_ConnList_T *p_conn = NULL;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);
    if (p_conn != NULL)
    {
        if ((p_conn->inputQueue.usedNum) > 0U)
        {
            *p_dataLength = p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].length;
        }
        else
        {
            *p_dataLength = 0;
        }
    }
    else
    {
        *p_dataLength = 0;
    }
}


/**
 * @brief Retrieves queued data from the buffer.
 *
 * @param[in]  connHandle                   Connection handle associated with the queued data.
 * @param[out] p_data                       Pointer to the buffer where the data should be copied.
 *
 * @retval MBA_RES_SUCCESS                  The data was successfully retrieved.
 * @retval MBA_RES_FAIL                     No data in the input queue.
 *
 */
uint16_t BLE_TRSPC_GetData(uint16_t connHandle, uint8_t *p_data)
{
    BLE_TRSPC_ConnList_T *p_conn = NULL;

    p_conn = ble_trspc_GetConnListByHandle(connHandle);
    if (p_conn != NULL)
    {
        if ((p_conn->inputQueue.usedNum) > 0U)
        {
            if (p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet != NULL)
            {
                (void)memcpy(p_data, p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet, 
                    p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].length);
                OSAL_Free(p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet);
                p_conn->inputQueue.packetList[p_conn->inputQueue.readIndex].p_packet = NULL;
            }

            p_conn->inputQueue.readIndex++;
            if (p_conn->inputQueue.readIndex >= BLE_TRSPC_INIT_CREDIT)
            {
                p_conn->inputQueue.readIndex = 0;
            }

            p_conn->inputQueue.usedNum --;

            if ((p_conn->trsState & BLE_TRSPC_UL_STATUS_CBFCENABLED) != 0U)
            {
                p_conn->peerCredit++;
                if (p_conn->peerCredit >= BLE_TRSPC_MAX_RETURN_CREDIT)
                {
                    ble_trspc_ClientReturnCredit(p_conn);
                }
            }

            return MBA_RES_SUCCESS;
        }
        else
        {
            return MBA_RES_FAIL;
        }
    }
    else
    {
        return MBA_RES_FAIL;
    }
}


/**
 * @brief Handles BLE_Stack events.
 * This function should be called by the application when BLE stack events occur.
 *
 * @param[in] p_stackEvent        					Pointer to the BLE stack event data.
 *
 */
void BLE_TRSPC_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_trspc_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_trspc_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
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
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *
 * @note This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event                       Pointer to the BLE_DD event data.
 *
 */
void BLE_TRSPC_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            uint8_t i;
            BLE_TRSPC_ConnList_T *p_conn;

            /* Check if TRS characteristics are found or not. */
            for (i=0; i<BLE_TRSPC_MAX_CONN_NBR; i++)
            {
                if (s_trsCharList[i].connHandle == p_event->eventField.evtDiscResult.connHandle)
                {
                    /* By checking the discovered handles exist or not. */
                    if (s_trsCharList[i].p_charInfo[TRSPC_INDEX_CHARTCP].charHandle != 0U)
                    {
                        if (bleTrspcProcess != NULL)
                        {
                            BLE_TRSPC_Event_T evtPara;

                            evtPara.eventId = BLE_TRSPC_EVT_DISC_COMPLETE;
                            evtPara.eventField.onUplinkStatus.connHandle = p_event->eventField.evtDiscResult.connHandle;
                            bleTrspcProcess(&evtPara);
                        }

                        p_conn = ble_trspc_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);
                        if (p_conn != NULL && p_conn->sessionReqAuth == 0U)
                        {
                            ble_trspc_EnableDataSession(p_event->eventField.evtDiscResult.connHandle, (BLE_TRSPC_CBFC_DL_ENABLED|BLE_TRSPC_CBFC_UL_ENABLED));
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