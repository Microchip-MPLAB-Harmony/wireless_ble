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
  BLE Transparent Credit Based Server Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trcbps.c

  Summary:
    Implements the server-side functionality of the BLE Transparent Credit-Based 
    Profile, enabling the handling of credit-based data flow control.

  Description:
    This source file provides the necessary functions to manage server operations
    within the BLE Transparent Credit-Based Profile. It includes initializations,
    event handling, and data transfer management to facilitate communication
    between BLE devices using a credit-based flow control mechanism.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "ble_l2cap.h"
#include "ble_util/mw_assert.h"
#include "ble_util/byte_stream.h"
#include "ble_trcbs/ble_trcbs.h"
#include "ble_trcbps.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_TRCBPS_MAX_CHAN_NBR                 (0x01U)                         // Maximum number of channels per L2CAP CoC (Connection-oriented Channel) link.

// Maximum number of connections in the connection list, derived from the product of max connections and max channels.
#define BLE_TRCBPS_MAX_CONNLIST_NBR             (BLE_TRCBPS_MAX_CONN_NBR * BLE_TRCBPS_MAX_CHAN_NBR)

#define BLE_TRCB_CTRL_PSM                       (0x0080U)                       // PSM value for the control channel.

#define BLE_TRCBPS_SET_FLAG(x, y)               ((x) |= (1U << (y)))            // Macro to set a flag 'y' in variable 'x'. 
#define BLE_TRCBPS_CLR_FLAG(x, y)               ((x) &= ~(1U << (y)))           // Macro to clear a flag 'y' in variable 'x'.
#define BLE_TRCBPS_CHK_FLAG(x, y)               ((x) & (1U << (y)))             // Macro to check the status of flag 'y' in variable 'x'.

#define BLE_TRCBPS_CCCD_DISABLE                 (0x0000U)                       // CCCD value to disable notifications and indications.
#define BLE_TRCBPS_CCCD_NOTIFY                  NOTIFICATION                    // CCCD value to enable NOTIFY property for notifications.

#define BLE_TRCBPS_CTRL_MPS                     BLE_L2CAP_MAX_PDU_SIZE          // MPS size for the control channel.
#define BLE_TRCBPS_DATA_MPS                     BLE_L2CAP_MAX_PDU_SIZE          // MPS size for the data channel.

#define BLE_TRCBPS_CTRL_MAX_CREDITS             (0x0002U)                       // Maximum number of credits for flow control on the control channel.
#define BLE_TRCBPS_CTRL_MAX_ACCU_CREDITS        (0x0001U)                       // Maximum accumulated credits that can be sent to the peer on the control channel.
#define BLE_TRCBPS_DATA_MAX_CREDITS             (0x0008U)                       // Maximum number of credits for flow control on the data channel.
#define BLE_TRCBPS_DATA_MAX_ACCU_CREDITS        (0x0005U)                       // Maximum accumulated credits that can be sent to the peer on the data channel. 
#define BLE_TRCBPS_PERMISSION                   (0x00U)                         // Permission setting for the profile.

#define BLE_TRCBPS_CTRL_MAX_BUF_IN              BLE_TRCBPS_CTRL_MAX_CREDITS     // Maximum number of incoming buffers for the control channel, based on max credits. 
#define BLE_TRCBPS_DATA_MAX_BUF_IN              BLE_TRCBPS_DATA_MAX_CREDITS     // Maximum number of incoming buffers for the data channel, based on max credits.

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* Structure for BLE Transparent Credit Based profile packet information. */
typedef struct BLE_TRCBPS_PacketList_T
{
    uint8_t                             leL2capId;                  // Identifier for the L2CAP session instance.
    uint8_t                             frameNum;                   // Number of frames reassembled for this SDU.
    uint16_t                            length;                     // Length of the data in bytes.
    uint8_t                             *p_packet;                  // Pointer to the received data buffer.
} BLE_TRCBPS_PacketList_T;

/* Structure for the input queue format of BLE Transparent Credit Based profile packets. */
typedef struct BLE_TRCBPS_QueueIn_T
{
    uint8_t                             usedNum;                    // Number of packets currently in the input buffer.
    uint8_t                             writeIndex;                 // Index at which new data will be written to the packet buffer.
    uint8_t                             readIndex;                  // Index at which data will be read from the packet buffer.
    BLE_TRCBPS_PacketList_T             packetList[BLE_TRCBPS_DATA_MAX_BUF_IN];  // Array of packet buffers.
} BLE_TRCBPS_QueueIn_T;

/* Structure for BLE Transparent Credit Based profile connection parameters. */
typedef struct BLE_TRCBPS_ConnList_T
{
    uint16_t                            connHandle;                 // Handle for the current connection.
    uint16_t                            spsm;                       // Service Protocol/Service Multiplexer for the L2CAP session.
    uint8_t                             leL2capId;                  // Identifier for the L2CAP session instance.
    uint8_t                             state;                      // Current state of the L2CAP CoC.
    uint16_t                            localMtu;                   // Maximum Transmission Unit size for the local device.
    uint16_t                            peerMtu;                    // Maximum Transmission Unit size for the peer device.
    uint16_t                            localMps;                   // Maximum PDU payload size for the local device.
    uint16_t                            peerMps;                    // Maximum PDU payload size for the peer device.
    uint16_t                            localCredits;               // Available credits for the local device.
    uint16_t                            localAccuCredits;           // Accumulated credits to be sent to the peer device.
    uint16_t                            peerCredits;                // Available credits for the peer device.
    uint16_t                            attMtu;                     // Current ATT MTU size for the connection. 
    BLE_TRCBPS_QueueIn_T                queueIn;                    // Input queue for data channel packets
    bool                                encStatus;                  // Flag indicating the encryption status of the connection.
} BLE_TRCBPS_ConnList_T;


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback function for BLE TRCBP (Thermometer Characteristic-Based Protocol) events.
static BLE_TRCBP_EventCb_T             s_bleTrcbpProcess;

// Array to keep track of connection information for each TRCBP connection.
static BLE_TRCBPS_ConnList_T           s_trcbpConnList[BLE_TRCBPS_MAX_CONNLIST_NBR];

// Flag variable used to indicate the status of TRCBP-related operations or conditions.
static uint8_t                         s_trcbpFlag;

// Pointer to the structure containing parameters for sending a write response in GATT.
static GATTS_SendWriteRespParams_T     *sp_trcbpsRespParams;

// Pointer to the structure containing parameters for sending an error response in GATT.
static GATTS_SendErrRespParams_T       *sp_trcbpsErrParams;

// Connection handle used for sending responses or errors in the TRCBP service.
static uint16_t                        s_trcbpsRespErrConnHandle;


// Assert to ensure that the product of the maximum number of connections and channels equals the defined maximum connection list number.
// This is a sanity check to ensure that the connection list array is properly sized.
MW_ASSERT((BLE_TRCBPS_MAX_CONN_NBR * BLE_TRCBPS_MAX_CHAN_NBR) == BLE_TRCBPS_MAX_CONNLIST_NBR);

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Initialize a connection list entry and optionally clear its queue.
 *
 * @param p_conn        Pointer to the connection list entry to initialize.
 * @param clearQueue    Boolean flag indicating whether to clear the queue.
 */
static void ble_trcbps_InitConnList(BLE_TRCBPS_ConnList_T *p_conn, bool clearQueue)
{
    if ((clearQueue) && (p_conn != NULL))
    {
        while (p_conn->queueIn.usedNum != 0U)
        {
            if (p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet != NULL)
            {
                OSAL_Free(p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet);
                p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet = NULL;
            }

            p_conn->queueIn.readIndex++;

            if (p_conn->queueIn.readIndex >= BLE_TRCBPS_DATA_MAX_BUF_IN)
            {
                p_conn->queueIn.readIndex = 0;
            }

            p_conn->queueIn.usedNum --;
        }
    }

    (void)memset((uint8_t *)p_conn, 0, sizeof(BLE_TRCBPS_ConnList_T));

    BLE_TRCBPS_CLR_FLAG(s_trcbpFlag, p_conn->leL2capId);

    p_conn->leL2capId = BLE_TRCBPS_L2CAP_UNASSIGNED_ID;
    p_conn->localMtu = BLE_ATT_MAX_MTU_LEN;
    p_conn->localMps = BLE_L2CAP_MAX_PDU_SIZE;
    p_conn->localCredits = BLE_TRCBPS_DATA_MAX_CREDITS;
    p_conn->attMtu = BLE_ATT_DEFAULT_MTU_LEN;
}


/**
 * @brief Get the Service Protocol/Service Multiplexer (SPSM) value based on the channel type.
 *
 * @param type The channel type (control or data).
 * 
 * @retval uint16_t The SPSM value for the specified channel type.
 */
static uint16_t ble_trcbps_GetSpsm(BLE_TRCBPS_ChanType_T type)
{
    if (type == BLE_TRCBPS_CTRL_CHAN)
    {
        return BLE_TRCB_CTRL_PSM;
    }
    else
    {
        return BLE_TRCB_DATA_PSM;
    }
}


/**
 * @brief Convert a Service Protocol/Service Multiplexer (SPSM) value to the corresponding channel type.
 *
 * @param spsm The SPSM value.
 * 
 * @retval BLE_TRCBPS_ChanType_T The channel type corresponding to the given SPSM value.
 */
static BLE_TRCBPS_ChanType_T ble_trcbps_CovertSpsmToType(uint16_t spsm)
{
    if (spsm == BLE_TRCB_CTRL_PSM)
    {
        return BLE_TRCBPS_CTRL_CHAN;
    }
    else if (spsm == (uint16_t)BLE_TRCB_DATA_PSM)
    {
        return BLE_TRCBPS_DATA_CHAN;
    }
    else
    {
        return BLE_TRCBPS_UNKNOWN_CHAN;
    }
}


/**
 * @brief Retrieve a connection list entry by its connection handle.
 *
 * @param connHandle The connection handle.
 * 
 * @retval BLE_TRCBPS_ConnList_T* Pointer to the connection list entry, or NULL if not found.
 */
static BLE_TRCBPS_ConnList_T *ble_trcbps_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for (i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
    {
        if (s_trcbpConnList[i].connHandle == connHandle)
        {
            return &s_trcbpConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Retrieve a connection list entry by its L2CAP ID.
 *
 * @param l2capId The L2CAP ID.
 * 
 * @retval BLE_TRCBPS_ConnList_T* Pointer to the connection list entry, or NULL if not found.
 */
static BLE_TRCBPS_ConnList_T *ble_trcbps_GetConnListByL2capId(uint8_t l2capId)
{
    uint8_t i;
    
    for(i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
    {
        if (s_trcbpConnList[i].leL2capId == l2capId)
        {
            return &s_trcbpConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Retrieve a connection list entry by its connection handle and channel type.
 *
 * @param connHandle    The connection handle.
 * @param type          The channel type.
 * 
 * @retval BLE_TRCBPS_ConnList_T* Pointer to the connection list entry, or NULL if not found.
 */
static BLE_TRCBPS_ConnList_T *ble_trcbps_GetConnListByChanType(uint16_t connHandle, BLE_TRCBPS_ChanType_T type)
{
    uint8_t i;

    for (i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
    {
        if ((s_trcbpConnList[i].connHandle == connHandle) && (s_trcbpConnList[i].spsm == ble_trcbps_GetSpsm(type)))
        {
            return &s_trcbpConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Retrieve a free connection list entry.
 *
 * @retval BLE_TRCBPS_ConnList_T* Pointer to the free connection list entry, or NULL if none are available.
 */
static BLE_TRCBPS_ConnList_T *ble_trcbps_GetFreeConnList(void)
{
    uint8_t i;

    for(i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
    {
        if (s_trcbpConnList[i].leL2capId == BLE_TRCBPS_L2CAP_UNASSIGNED_ID)
        {
            return &s_trcbpConnList[i];
        }
    }

    return NULL;
}


/**
 * @brief Convey a connection status event to the registered callback.
 *
 * @param connStatusPara Pointer to the connection status event parameters.
 */
static void ble_trcbps_ConveyConnStatusEvt(BLE_TRCBPS_EvtConnStatus_T *connStatusPara)
{
    BLE_TRCBPS_Event_T evtPara;

    evtPara.eventId = BLE_TRCBPS_EVT_CONNECTION_STATUS;

    (void)memcpy((uint8_t *)&evtPara.eventField.connStatus, (uint8_t *)connStatusPara, sizeof(BLE_TRCBPS_EvtConnStatus_T));

    /*
    if ((status == BLE_TRCBPS_STATUS_CONNECTING) && (leL2capId != BLE_TRCBPS_L2CAP_UNASSIGNED_ID))
    {
        leL2capId = BLE_TRCBPS_L2CAP_UNASSIGNED_ID;
    }
    */

    if (s_bleTrcbpProcess != NULL)
    {
        s_bleTrcbpProcess(&evtPara);
    }
}


/**
 * @brief Convey a received data event to the registered callback.
 *
 * @param p_conn Pointer to the connection list entry that received data.
 */
static void ble_trcbp_ConveyRcvDataEvt(BLE_TRCBPS_ConnList_T *p_conn)
{
    BLE_TRCBPS_Event_T evtPara;

    evtPara.eventId = BLE_TRCBPS_EVT_RECEIVE_DATA;
    evtPara.eventField.onReceiveData.connHandle = p_conn->connHandle;
    evtPara.eventField.onReceiveData.chanType = ble_trcbps_CovertSpsmToType(p_conn->spsm);
    evtPara.eventField.onReceiveData.leL2capId = p_conn->leL2capId;

    if (s_bleTrcbpProcess != NULL)
    {
        s_bleTrcbpProcess(&evtPara);
    }
}


/**
 * @brief Handle received data, store it in the connection list entry's queue, and convey the event.
 *
 * @param p_conn Pointer to the connection list entry that received data.
 * @param p_event Pointer to the L2CAP event containing the received data.
 */
static void ble_trcbps_RcvData(BLE_TRCBPS_ConnList_T *p_conn, BLE_L2CAP_Event_T *p_event)
{
    uint8_t maxBufNum;

    if (p_conn->spsm == BLE_TRCB_CTRL_PSM)
    {
        maxBufNum = BLE_TRCBPS_CTRL_MAX_BUF_IN;
    }
    else
    {
        maxBufNum = BLE_TRCBPS_DATA_MAX_BUF_IN;
    }

    if (p_conn->queueIn.usedNum < maxBufNum)
    {
        uint8_t *p_buffer;

        p_buffer = OSAL_Malloc(p_event->eventField.evtCbSduInd.length);

        if (p_buffer == NULL)
        {
            BLE_TRCBPS_Event_T evtPara;
            evtPara.eventId = BLE_TRCBPS_EVT_ERR_NO_MEM;
            if (s_bleTrcbpProcess != NULL)
            {
                s_bleTrcbpProcess(&evtPara);
            }
            return;
        }

        (void)memcpy(p_buffer, p_event->eventField.evtCbSduInd.payload, p_event->eventField.evtCbSduInd.length);

        p_conn->queueIn.packetList[p_conn->queueIn.writeIndex].leL2capId= p_conn->leL2capId;
        p_conn->queueIn.packetList[p_conn->queueIn.writeIndex].length = p_event->eventField.evtCbSduInd.length;
        p_conn->queueIn.packetList[p_conn->queueIn.writeIndex].p_packet = p_buffer;
        p_conn->queueIn.packetList[p_conn->queueIn.writeIndex].frameNum = p_event->eventField.evtCbSduInd.frames;

        p_conn->queueIn.writeIndex++;

        if (p_conn->queueIn.writeIndex >= maxBufNum)
        {
            p_conn->queueIn.writeIndex = 0;
        }

        p_conn->queueIn.usedNum++;
        p_conn->localCredits -= p_event->eventField.evtCbSduInd.frames;

        ble_trcbp_ConveyRcvDataEvt(p_conn);
    }

}


/**
 * @brief Check if there is a queued task.
 *
 * @retval True if there is a queued task, false otherwise.
 */
static bool ble_trcbps_CheckQueuedTask(void)
{
    if (s_trcbpFlag != 0U)
    {
        return true;
    }

    if (sp_trcbpsRespParams != NULL)
    {
        return true;
    }

    if (sp_trcbpsErrParams != NULL)
    {
        return true;
    }

    return false;
}


/**
 * @brief Process the queued task.
 */
static void ble_trcbps_ProcessQueuedTask(void)
{
    uint8_t i;
    uint16_t ret;

    if (s_trcbpFlag != 0U)
    {
        for (i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
        {
            if (s_trcbpConnList[i].leL2capId != BLE_TRCBPS_L2CAP_UNASSIGNED_ID)
            {
                uint16_t maxAccuCredit;

                if (s_trcbpConnList[i].spsm == BLE_TRCB_CTRL_PSM)
                {
                    maxAccuCredit = BLE_TRCBPS_CTRL_MAX_ACCU_CREDITS;
                }
                else
                {
                    maxAccuCredit = BLE_TRCBPS_DATA_MAX_ACCU_CREDITS;
                }

                if (s_trcbpConnList[i].localAccuCredits >= maxAccuCredit)
                {
                    ret = BLE_L2CAP_CbAddCredits(s_trcbpConnList[i].leL2capId, s_trcbpConnList[i].localAccuCredits);

                    if (ret == MBA_RES_SUCCESS)
                    {
                        BLE_TRCBPS_CLR_FLAG(s_trcbpFlag, i);

                        s_trcbpConnList[i].localCredits += s_trcbpConnList[i].localAccuCredits;
                        s_trcbpConnList[i].localAccuCredits = 0;
                    }
                }
            }
        }
    }

    if (sp_trcbpsRespParams != NULL)
    {
        ret = GATTS_SendWriteResponse(s_trcbpsRespErrConnHandle, sp_trcbpsRespParams);
        if (ret == MBA_RES_SUCCESS)
        {
            OSAL_Free(sp_trcbpsRespParams);
            sp_trcbpsRespParams = NULL;
            s_trcbpsRespErrConnHandle = 0;
        }
    }

    if (sp_trcbpsErrParams != NULL)
    {
        ret = GATTS_SendErrorResponse(s_trcbpsRespErrConnHandle, sp_trcbpsErrParams);
        if (ret == MBA_RES_SUCCESS)
        {
            OSAL_Free(sp_trcbpsErrParams);
            sp_trcbpsErrParams = NULL;
            s_trcbpsRespErrConnHandle = 0;
        }
    }
}


/**
 *@brief Register a callback for BLE Transparent Credit Based profile events.
 *
 *@param[in] bleTrcbpHandler                 Client callback function to handle BLE TRCBP events.
 *
 */
void BLE_TRCBPS_EventRegister(BLE_TRCBP_EventCb_T bleTrcbpHandler)
{
    s_bleTrcbpProcess = bleTrcbpHandler;
}


/**
 * @brief Initialize the BLE Transparent Credit Based Profile.
 * 
 * @retval MBA_RES_SUCCESS                   Successfully initialized the profile.
 * @retval MBA_RES_INVALID_PARA              Invalid MTU parameter.
 * @retval MBA_RES_FAIL                      Failed to register SPSM or the service.
 *
 */
uint16_t BLE_TRCBPS_Init(void)
{
    uint8_t i;
    uint16_t ret;


    //Regist PSM for data channel
    ret = BLE_L2CAP_CbRegisterSpsm(BLE_TRCB_DATA_PSM, BLE_TRCBPS_DATA_MTU, BLE_TRCBPS_DATA_MPS, BLE_TRCBPS_DATA_MAX_CREDITS, BLE_TRCBPS_PERMISSION);
    
    if (ret != MBA_RES_SUCCESS)
    {
        return ret;
    }

    sp_trcbpsRespParams = NULL;
    sp_trcbpsErrParams = NULL;
    s_trcbpsRespErrConnHandle = 0;

    for (i = 0; i < BLE_TRCBPS_MAX_CONNLIST_NBR; i++)
    {
        ble_trcbps_InitConnList(&s_trcbpConnList[i], false);
    }

    return BLE_TRCBS_Add();
}


/**
 * @brief Query the local L2CAP CoC PSM value for the TRCBP Data Channel.
 * 
 * @param[out] dataPsm                       Pointer to store the PSM of the data channel.
 *
 * @retval MBA_RES_SUCCESS                   Successfully retrieved the PSM value.
 *
 */
uint16_t BLE_TRCBPS_QueryPsm(uint16_t *dataPsm)
{
    *dataPsm = BLE_TRCB_DATA_PSM;

    return MBA_RES_SUCCESS;
}


/**
 * @brief Issue an L2CAP CoC connection request to establish a TRCBP Data pipe.
 * 
 * @param[in] connHandle                     Connection handle.
 *
 * @retval MBA_RES_SUCCESS                   Successfully issued a connection request.
 * @retval MBA_RES_OOM                       Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA              Invalid connection handle or SPSM.
 * @retval MBA_RES_NO_RESOURCE               No transmit buffers available.
 *
 */
uint16_t BLE_TRCBPS_ConnReq(uint16_t connHandle)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;
    uint16_t ret;

    p_conn = ble_trcbps_GetFreeConnList();

    if (p_conn == NULL)
    {
        return MBA_RES_NO_RESOURCE;
    }

    ret = BLE_L2CAP_CbConnReq(connHandle, ble_trcbps_GetSpsm(BLE_TRCBPS_DATA_CHAN));

    if (ret == MBA_RES_SUCCESS)
    {
        BLE_TRCBPS_EvtConnStatus_T connStatusPara;

        (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

        p_conn->connHandle = connHandle;
        p_conn->state = BLE_TRCBPS_STATUS_CONNECTING;
        p_conn->spsm = ble_trcbps_GetSpsm(BLE_TRCBPS_DATA_CHAN);

        connStatusPara.connHandle = p_conn->connHandle;
        connStatusPara.chanType = BLE_TRCBPS_DATA_CHAN;
        connStatusPara.leL2capId = BLE_TRCBPS_L2CAP_UNASSIGNED_ID;
        connStatusPara.status = p_conn->state;

        ble_trcbps_ConveyConnStatusEvt(&connStatusPara);
    }
    /*
    else
    {
        p_conn->state = BLE_TRCBPS_STATUS_STANDBY;
    }
    */

    return ret;
}


/**
 * @brief Issue an L2CAP CoC disconnect request for a TRCBP Data pipe.
 * 
 * @param[in] connHandle                     Connection handle.
 *
 * @retval MBA_RES_SUCCESS                   Successfully sent a disconnect request.
 * @retval MBA_RES_OOM                       Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link.
 * @retval MBA_RES_NO_RESOURCE               No transmit buffers available.
 * @retval MBA_RES_FAIL                      Failed due to incorrect handle or SPSM.
 *
 */
uint16_t BLE_TRCBPS_DisconnectReq(uint16_t connHandle)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;
    uint16_t ret;

    p_conn = ble_trcbps_GetConnListByChanType(connHandle, BLE_TRCBPS_DATA_CHAN);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    ret = BLE_L2CAP_CbDiscReq(p_conn->leL2capId);

    if (ret == MBA_RES_SUCCESS)
    {
        BLE_TRCBPS_EvtConnStatus_T connStatusPara;

        (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

        p_conn->state = BLE_TRCBPS_STATUS_DISCONNECTING;

        connStatusPara.connHandle = p_conn->connHandle;
        connStatusPara.chanType = BLE_TRCBPS_DATA_CHAN;
        connStatusPara.leL2capId = p_conn->leL2capId;
        connStatusPara.status = p_conn->state;

        ble_trcbps_ConveyConnStatusEvt(&connStatusPara);
    }

    return ret;
}


/**
 * @brief Send data through the BLE Transparent Credit Based Profile Data pipe.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[in] len                            Length of the data to be sent. Must not exceed BLE_TRCBPS_DATA_MTU.
 * @param[in] p_data                         Pointer to the buffer containing the data to send.
 *
 * @retval MBA_RES_SUCCESS                   Successfully sent data.
 * @retval MBA_RES_OOM                       Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link or data length exceeds max MTU.
 * @retval MBA_RES_NO_RESOURCE               Insufficient credit from the remote device.
 * @retval MBA_RES_BAD_STATE                 Connection not established.
 *
 */
uint16_t BLE_TRCBPS_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;

    p_conn = ble_trcbps_GetConnListByChanType(connHandle, BLE_TRCBPS_DATA_CHAN);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (ble_trcbps_CheckQueuedTask())
    {
        return MBA_RES_NO_RESOURCE;
    }
    if (p_conn->state == BLE_TRCBPS_STATUS_CONNECTED)
    {
        if (p_conn->peerCredits != 0U)
        {
            uint16_t ret;

            ret = BLE_L2CAP_CbSendSdu(p_conn->leL2capId, len, p_data);

            if (ret == MBA_RES_SUCCESS)
            {
                p_conn->peerCredits--;
            }

            return ret;
        }
        else
        {
            return MBA_RES_NO_RESOURCE;
        }
    }
    else
    {
        return MBA_RES_BAD_STATE;
    }
}


uint16_t BLE_TRCBPS_SendVendorCommand(uint16_t connHandle, uint8_t cmdId, uint16_t len, uint8_t *p_payload)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;
    GATTS_HandleValueParams_T *p_hvParams;

    p_conn = ble_trcbps_GetConnListByHandle(connHandle);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (cmdId < BLE_TRCBPS_VENDOR_OPCODE_MIN)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (len > (p_conn->attMtu - ATT_NOTI_INDI_HEADER_SIZE - 1U))
    {
        return MBA_RES_INVALID_PARA;
    }

    p_hvParams = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_hvParams != NULL)
    {
        uint16_t ret;

        p_hvParams->charHandle = (uint16_t)BLE_TRCB_HDL_CHARVAL_CTRL;
        p_hvParams->charLength = (len + 1U);
        p_hvParams->charValue[0] = cmdId;
        (void)memcpy(&p_hvParams->charValue[1], p_payload, len);
        p_hvParams->sendType = ATT_HANDLE_VALUE_NTF;
        ret = GATTS_SendHandleValue(p_conn->connHandle, p_hvParams);

        OSAL_Free(p_hvParams);

        return ret;
    }
    else
    {
        return MBA_RES_OOM;
    }
}


/**
 * @brief Get the length of queued data in the Data pipe for a given connection handle.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[out] p_dataLength                  Pointer to store the length of the queued data.
 *
 * @retval MBA_RES_SUCCESS                   Successfully retrieved the data length.
 * @retval MBA_RES_INVALID_PARA              Connection not established.
 *
 */
uint16_t BLE_TRCBPS_GetDataLength(uint16_t connHandle, uint16_t *p_dataLength)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;

    *p_dataLength = 0;
    p_conn = ble_trcbps_GetConnListByChanType(connHandle, BLE_TRCBPS_DATA_CHAN);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->queueIn.usedNum > 0U)
    {
        *p_dataLength = p_conn->queueIn.packetList[p_conn->queueIn.readIndex].length;
    }

    return MBA_RES_SUCCESS;
}


/**
 * @brief Retrieve queued data from the Data pipe for a given connection handle.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[out] p_data                        Pointer to the data buffer
 *
 * @retval MBA_RES_SUCCESS                   Get the data successfully.
 * @retval MBA_RES_INVALID_PARA              Connection not established.
 * @retval MBA_RES_FAIL                      Queue is empty or other failure.
 *
 */
uint16_t BLE_TRCBPS_GetData(uint16_t connHandle, uint8_t *p_data)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;

    p_conn = ble_trcbps_GetConnListByChanType(connHandle, BLE_TRCBPS_DATA_CHAN);

    if (p_conn == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }

    if (p_conn->queueIn.usedNum > 0U)
    {
        uint8_t maxBufNum;
        uint16_t maxAccuCredit;

        if (p_conn->spsm == BLE_TRCB_CTRL_PSM)
        {
            maxBufNum = BLE_TRCBPS_CTRL_MAX_BUF_IN;
            maxAccuCredit = BLE_TRCBPS_CTRL_MAX_ACCU_CREDITS;
        }
        else
        {
            maxBufNum = BLE_TRCBPS_DATA_MAX_BUF_IN;
            maxAccuCredit = BLE_TRCBPS_DATA_MAX_ACCU_CREDITS;
        }

        if (p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet != NULL)
        {
            (void)memcpy(p_data, p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet, p_conn->queueIn.packetList[p_conn->queueIn.readIndex].length);

            OSAL_Free(p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet);
            p_conn->queueIn.packetList[p_conn->queueIn.readIndex].p_packet = NULL;
        }

        p_conn->localAccuCredits += p_conn->queueIn.packetList[p_conn->queueIn.readIndex].frameNum;
        p_conn->queueIn.readIndex++;

        if (p_conn->queueIn.readIndex >= maxBufNum)
        {
            p_conn->queueIn.readIndex = 0;
        }

        p_conn->queueIn.usedNum --;

        if (p_conn->localAccuCredits >= maxAccuCredit)
        {
            uint16_t ret;

            ret = BLE_L2CAP_CbAddCredits(p_conn->leL2capId, p_conn->localAccuCredits);

            if (ret != MBA_RES_SUCCESS)
            {
                BLE_TRCBPS_SET_FLAG(s_trcbpFlag, p_conn->leL2capId);
            }
            else
            {
                p_conn->localCredits += p_conn->localAccuCredits;
                //p_conn->queueIn.usedNum -= p_conn->localAccuCredits;

                p_conn->localAccuCredits = 0;
            }
        }

        return MBA_RES_SUCCESS;
    }
    else
    {
        return MBA_RES_FAIL;
    }
}


/**
 * @brief Process control pipe for a BLE connection.
 * 
 * @param p_conn    Pointer to the BLE connection list.
 * @param length    Length of the value to be processed.
 * @param p_value   Pointer to the value to be processed.
 */
static void ble_trcbps_CtrlValue(BLE_TRCBPS_ConnList_T *p_conn, uint16_t length, uint8_t *p_value)
{
    BLE_TRCBPS_Event_T evtPara;

    (void)memset((uint8_t *) &evtPara, 0, sizeof(evtPara));

    if ((p_value[0] >= BLE_TRCBPS_VENDOR_OPCODE_MIN)
        && (s_bleTrcbpProcess!= NULL))
    {
        evtPara.eventId = BLE_TRCBPS_EVT_VENDOR_CMD;
        evtPara.eventField.onVendorCmd.connHandle = p_conn->connHandle;
        evtPara.eventField.onVendorCmd.length = length;
        evtPara.eventField.onVendorCmd.p_payLoad = p_value;

        s_bleTrcbpProcess(&evtPara);
    }
}


/**
 * @brief Process the CCCD control for a BLE connection.
 * 
 * @param p_conn    Pointer to the BLE connection list.
 * @param p_value   Pointer to the value to be processed.
 * @retval uint8_t  Returns 0 on success, ATT_ERR_APPLICATION_ERROR on failure.
 */
static uint8_t ble_trcbps_CtrlCccd(BLE_TRCBPS_ConnList_T *p_conn, uint8_t *p_value)
{
    uint16_t cccd;
    BLE_TRCBPS_EvtConnStatus_T connStatusPara;

    BUF_LE_TO_U16(&cccd, p_value);
    
    if ((cccd != 0U) && (cccd != BLE_TRCBPS_CCCD_NOTIFY))
    {
        return ATT_ERR_APPLICATION_ERROR;
    }

    (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

    connStatusPara.connHandle = p_conn->connHandle;
    connStatusPara.chanType = BLE_TRCBPS_CTRL_CHAN;

    if (cccd == BLE_TRCBPS_CCCD_NOTIFY)
    {
        connStatusPara.status = BLE_TRCBPS_STATUS_CONNECTED;
    }
    else
    {
        connStatusPara.status = BLE_TRCBPS_STATUS_STANDBY;
    }

    ble_trcbps_ConveyConnStatusEvt(&connStatusPara);

    return 0;
}


/**
 * @brief Process GATT write events.
 * 
 * @param p_event   Pointer to the GATT event structure.
 */
static void ble_trcbps_GattsWriteProcess(GATT_Event_T *p_event)
{
    uint8_t error = 0;
    BLE_TRCBPS_ConnList_T *p_conn = NULL;
    uint16_t status;

    if ((p_event->eventField.onWrite.attrHandle <= (uint16_t)BLE_TRCB_START_HDL) ||
        (p_event->eventField.onWrite.attrHandle > (uint16_t)BLE_TRCB_END_HDL))
    {
        /* Not BLE TRCBS characteristic. */
        return;
    }

    p_conn = ble_trcbps_GetConnListByHandle(p_event->eventField.onWrite.connHandle);
    if (p_conn == NULL)
    {
        error = ATT_ERR_APPLICATION_ERROR;
    }

    if (p_event->eventField.onWrite.writeType == ATT_PREPARE_WRITE_REQ)
    {
        error = ATT_ERR_REQUEST_NOT_SUPPORT;
    }

    if (error==0U)
    {
        switch(p_event->eventField.onWrite.attrHandle)
        {
            case (uint16_t)BLE_TRCB_HDL_CHARVAL_CTRL:
            {
                ble_trcbps_CtrlValue(p_conn, p_event->eventField.onWrite.writeDataLength, 
                    p_event->eventField.onWrite.writeValue);
            }
            break;

            case (uint16_t)BLE_TRCB_HDL_CCCD_CTRL:
            {
                error = ble_trcbps_CtrlCccd(p_conn, p_event->eventField.onWrite.writeValue);
            }
            break;

            default:
            {
                //Do nothing
            }
            break;
        }
    }

    if ((p_event->eventField.onWrite.writeType == ATT_WRITE_REQ)
    || (p_event->eventField.onWrite.writeType == ATT_PREPARE_WRITE_REQ))
    {
        if (error==0U)
        {
            sp_trcbpsRespParams = (GATTS_SendWriteRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
            if (sp_trcbpsRespParams == NULL)
            {
                BLE_TRCBPS_Event_T evtPara;
                evtPara.eventId = BLE_TRCBPS_EVT_ERR_NO_MEM;
                if (s_bleTrcbpProcess != NULL)
                {
                    s_bleTrcbpProcess(&evtPara);
                }
                return;
            }
            s_trcbpsRespErrConnHandle = p_event->eventField.onWrite.connHandle;
            sp_trcbpsRespParams->responseType = ATT_WRITE_RSP;
            status = GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, sp_trcbpsRespParams);
            if (status == MBA_RES_SUCCESS)
            {
                OSAL_Free(sp_trcbpsRespParams);
                sp_trcbpsRespParams = NULL;
            }
        }
        else
        {
            sp_trcbpsErrParams = (GATTS_SendErrRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
            if (sp_trcbpsErrParams == NULL)
            {
                BLE_TRCBPS_Event_T evtPara;
                evtPara.eventId = BLE_TRCBPS_EVT_ERR_NO_MEM;
                if (s_bleTrcbpProcess != NULL)
                {
                    s_bleTrcbpProcess(&evtPara);
                }
                return;
            }
            s_trcbpsRespErrConnHandle = p_event->eventField.onWrite.connHandle;
            sp_trcbpsErrParams->reqOpcode = p_event->eventField.onWrite.writeType;
            sp_trcbpsErrParams->attrHandle = p_event->eventField.onWrite.attrHandle;
            sp_trcbpsErrParams->errorCode = error;
            status = GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, sp_trcbpsErrParams);
            if (status == MBA_RES_SUCCESS)
            {
                OSAL_Free(sp_trcbpsErrParams);
                sp_trcbpsErrParams = NULL;
            }
        }
    }
}


/**
 * @brief Process GATT events.
 * 
 * @param p_event   Pointer to the GATT event structure.
 */
static void ble_trcbps_GattEventProcess(GATT_Event_T *p_event)
{
    BLE_TRCBPS_ConnList_T *p_conn = NULL;

    switch (p_event->eventId)
    {
        case ATT_EVT_TIMEOUT:
        {
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            p_conn = ble_trcbps_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn == NULL)
            {
                break;
            }

            p_conn->attMtu = p_event->eventField.onUpdateMTU.exchangedMTU;
        }
        break;

        case GATTS_EVT_WRITE:
        {
            ble_trcbps_GattsWriteProcess(p_event);
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
 * @brief Process GAP events.
 * 
 * @param p_event   Pointer to the GAP event structure.
 */
static void ble_trcbps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
            break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;

            p_conn = ble_trcbps_GetConnListByHandle(p_event->eventField.evtDisconnect.connHandle);

            if (p_conn != NULL)
            {
                ble_trcbps_InitConnList(p_conn, true);
            }
        }
        break;

        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_trcbps_ProcessQueuedTask();
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;

            p_conn = ble_trcbps_GetConnListByHandle(p_event->eventField.evtEncryptStatus.connHandle);

            if (p_conn != NULL)
            {
                if (p_event->eventField.evtEncryptStatus.status == GAP_STATUS_SUCCESS)
                {
                    p_conn->encStatus = true;
                }
                else
                {
                    p_conn->encStatus = false;
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
 * @brief Process L2CAP events.
 * 
 * @param p_event Pointer to the L2CAP event structure.
 */
static void ble_trcbps_L2capEventProcess(BLE_L2CAP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_L2CAP_EVT_CB_CONN_IND:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;
            p_conn = ble_trcbps_GetFreeConnList();

            if (p_conn != NULL)
            {
                BLE_TRCBPS_EvtConnStatus_T connStatusPara;

                (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

                p_conn->connHandle = p_event->eventField.evtCbConnInd.connHandle;
                p_conn->leL2capId =p_event->eventField.evtCbConnInd.leL2capId;
                p_conn->state = BLE_TRCBPS_STATUS_CONNECTED;
                p_conn->peerMtu = p_event->eventField.evtCbConnInd.remoteMtu;
                p_conn->peerMps = p_event->eventField.evtCbConnInd.remoteMps;
                p_conn->peerCredits = p_event->eventField.evtCbConnInd.initialCredits;
                p_conn->spsm = p_event->eventField.evtCbConnInd.spsm;

                connStatusPara.connHandle = p_conn->connHandle;
                connStatusPara.chanType = ble_trcbps_CovertSpsmToType(p_conn->spsm);
                connStatusPara.leL2capId = p_conn->leL2capId;
                connStatusPara.status = p_conn->state;
                connStatusPara.peerMps = p_conn->peerMps;
                connStatusPara.peerMtu = p_conn->peerMtu;

                ble_trcbps_ConveyConnStatusEvt(&connStatusPara);
            }
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_FAIL_IND:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;

            p_conn = ble_trcbps_GetConnListByHandle(p_event->eventField.evtCbConnFailInd.connHandle);

            if (p_conn != NULL)
            {
                BLE_TRCBPS_EvtConnStatus_T connStatusPara;

                (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

                connStatusPara.connHandle = p_conn->connHandle;
                connStatusPara.chanType= ble_trcbps_CovertSpsmToType(p_conn->spsm);
                connStatusPara.leL2capId = BLE_TRCBPS_L2CAP_UNASSIGNED_ID;
                connStatusPara.status = BLE_TRCBPS_STATUS_STANDBY;

                ble_trcbps_ConveyConnStatusEvt(&connStatusPara);

                ble_trcbps_InitConnList(p_conn, false);
            }
        }
        break;

        case BLE_L2CAP_EVT_CB_SDU_IND:
        {
             BLE_TRCBPS_ConnList_T *p_conn = NULL;

             p_conn = ble_trcbps_GetConnListByL2capId(p_event->eventField.evtCbSduInd.leL2capId);

             if (p_conn != NULL)
             {
                ble_trcbps_RcvData(p_conn, p_event);
             }
        }
        break;

        case BLE_L2CAP_EVT_CB_ADD_CREDITS_IND:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;

            p_conn = ble_trcbps_GetConnListByL2capId(p_event->eventField.evtCbAddCreditsInd.leL2capId);

            if (p_conn != NULL)
            {
                p_conn->peerCredits += p_event->eventField.evtCbAddCreditsInd.credits;
            }
        }
        break;

        case BLE_L2CAP_EVT_CB_DISC_IND:
        {
            BLE_TRCBPS_ConnList_T *p_conn = NULL;
            uint8_t l2capId;

            p_conn = ble_trcbps_GetConnListByL2capId(p_event->eventField.evtCbDiscInd.leL2capId);
            l2capId = p_event->eventField.evtCbDiscInd.leL2capId;

            if (p_conn != NULL)
            {
                BLE_TRCBPS_EvtConnStatus_T connStatusPara;

                (void)memset((uint8_t *)&connStatusPara, 0, sizeof(BLE_TRCBPS_EvtConnStatus_T));

                connStatusPara.connHandle = p_conn->connHandle;
                connStatusPara.chanType = ble_trcbps_CovertSpsmToType(p_conn->spsm);
                connStatusPara.leL2capId = l2capId;
                connStatusPara.status = BLE_TRCBPS_STATUS_STANDBY;
                
                ble_trcbps_ConveyConnStatusEvt(&connStatusPara);

                //Check if there is still data stored in the RX queue
                ble_trcbps_InitConnList(p_conn, true);
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
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent                  Pointer to the BLE Stack event data structure.
 *
*/
void BLE_TRCBPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_trcbps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_BLE_L2CAP:
        {
            ble_trcbps_L2capEventProcess((BLE_L2CAP_Event_T *)p_stackEvent->p_event);
         }
        break;

        case STACK_GRP_GATT:
        {
            ble_trcbps_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;
        
        default:
        {
            //Do nothing
        }
        break;
    }    
}
