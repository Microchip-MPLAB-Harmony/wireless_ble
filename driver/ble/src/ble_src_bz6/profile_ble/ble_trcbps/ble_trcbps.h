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
  BLE Transparent Credit Based Server Profile Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trcbps.h

  Summary:
    Interface for the BLE Transparent Credit Based Server (TRCBPS) used by application developers.

  Description:
    This header defines the interface for the BLE Transparent Credit Based Server 
    (TRCBPS), which enables the application to interact with the BLE stack for 
    managing credit-based data transactions over a BLE connection.
 *******************************************************************************/
#ifndef BLE_TRCBPS_H
#define BLE_TRCBPS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "gatt.h"
#include "stack_mgr.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @addtogroup BLE_PROFILE BLE Profile
 * @{ */

/**
 * @defgroup BLE_TRCBPS BLE Transparent Credit Based Profile Server (TRCBPS)
 * 
 * @brief Provides the server role functionality for the Transparent Credit Based Profile.
 * @note This section defines the API for the BLE Transparent Credit Based Profile Server role.
 *          It includes definitions and prototypes to facilitate credit-based data transfer
 *          between a BLE server and client, ensuring flow control and efficient data streaming.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRCBPS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_TRCBPS_MAX_CONN_NBR TRCBPS maximum connection number
 * @brief Defines the maximum number of connections for the BLE Transparent Credit-Based Profile.
 * @{
 */
#define BLE_TRCBPS_MAX_CONN_NBR                           BLE_GAP_MAX_LINK_NBR    /**< Maximum allowed connection numbers. */
/** @} */


/**
 * @defgroup BLE_TRCBPS_L2CAP_ID TRCBPS unassigned L2CAP ID
 * @brief Defines the value for an unassigned L2CAP ID.
 * @{
 */
#define BLE_TRCBPS_L2CAP_UNASSIGNED_ID                    (0xFFU)                 /**< Unassigned BLE L2CAP ID. */
/** @} */


/**
 * @defgroup BLE_TRCBPS_STATUS TRCPBS status
 * @brief Defines the connection status for the BLE Transparent Credit-Based Profile.
 * @{
 */
#define BLE_TRCBPS_STATUS_STANDBY                         (0x00U)                 /**< Indicating no current L2CAP credit based connection. */
#define BLE_TRCBPS_STATUS_CONNECTING                      (0x01U)                 /**< Indicating an L2CAP credit based connection is in the process of being established. */
#define BLE_TRCBPS_STATUS_CONNECTED                       (0x02U)                 /**< Indicating an L2CAP credit based connection has been successfully established. */
#define BLE_TRCBPS_STATUS_DISCONNECTING                   (0x03U)                 /**< Indicating an L2CAP credit based connection is in the process of being disconnected. */
/** @} */


/**
 * @defgroup BLE_TRCBPS_MTU TRCBPS maximum transmission unit
 * @brief Defines MTU for the BLE Transparent Credit-Based Profile.
 * @{
 */
#define BLE_TRCBPS_SDU_LEN_FIELD_LEN                      (0x02U)                 /**< Length of the SDU length field. */
#define BLE_TRCBPS_CTRL_MTU                               BLE_ATT_MAX_MTU_LEN - BLE_TRCBPS_SDU_LEN_FIELD_LEN    /**< MTU size for the control channel. */
#define BLE_TRCBPS_DATA_MTU                               BLE_ATT_MAX_MTU_LEN - BLE_TRCBPS_SDU_LEN_FIELD_LEN    /**< MTU size for the data channel. */
/** @} */


/**
 * @defgroup BLE_TRCBPS_VENDOR_OPCODE TRCBPS vendor opcodes
 * @brief Defines vendor-specific command opcodes for the control channel of the BLE Transparent Credit-Based Profile.
 * @{
 */
#define BLE_TRCBPS_VENDOR_OPCODE_MIN                      (0x20U)                 /**< Minimum value of vendor opcode. */
#define BLE_TRCBPS_VENDOR_OPCODE_MAX                      (0xFFU)                 /**< Maximum value of vendor opcode. */
/** @} */

/** @} */ //BLE_TRCBPS_DEFINES


/**
 * @addtogroup BLE_TRCBPS_ENUMS Enumerations
 * @{ */

/** @brief Enumeration for BLE Transparent Credit Based Profile callback event identifiers. */
typedef enum BLE_TRCBPS_EventId_T
{
    BLE_TRCBPS_EVT_CONNECTION_STATUS = 0x00U,                                     /**< Event indicating a change in connection status. 
                                                                                        Details are provided in @ref BLE_TRCBPS_EvtConnStatus_T. */
    BLE_TRCBPS_EVT_RECEIVE_DATA,                                                  /**< Event indicating that data has been received on the Data Channel. 
                                                                                        Details are provided in @ref BLE_TRCBPS_EvtReceiveData_T. */
    BLE_TRCBPS_EVT_VENDOR_CMD,                                                    /**< Event indicating that a vendor-specific command has been received. 
                                                                                        Details are provided in @ref BLE_TRCBPS_EvtVendorCmd_T. */
    BLE_TRCBPS_EVT_ERR_NO_MEM                                                     /**< Event indicating an internal error due to insufficient heap memory. */
} BLE_TRCBPS_EventId_T;


/** @brief Enumeration for BLE Transparent Credit Based Profile channel types. */
typedef enum BLE_TRCBPS_ChanType_T
{
    BLE_TRCBPS_UNKNOWN_CHAN = 0x00U,                                              /**< Represents an unknown channel type. */
    BLE_TRCBPS_CTRL_CHAN,                                                         /**< Represents the Control Channel. */
    BLE_TRCBPS_DATA_CHAN                                                          /**< Represents the Data Channel. */
} BLE_TRCBPS_ChanType_T;

/** @} */ //BLE_TRCBPS_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BLE_TRCBPS_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_TRCBPS_EVT_CONNECTION_STATUS event. */
typedef struct
{
    uint16_t                connHandle;                                  /**< Connection handle associated with this connection. */
    BLE_TRCBPS_ChanType_T   chanType;                                    /**< Channel type. Refer to @ref BLE_TRCBPS_ChanType_T for details. */
    uint8_t                 leL2capId;                                   /**< L2CAP session instance. Set to @ref BLE_TRCBPS_L2CAP_UNASSIGNED_ID if unassigned. Applicable only for data channels. */
    uint16_t                peerMtu;                                     /**< Peer device's maximum MTU size. Applicable only for data channels. * */
    uint16_t                peerMps;                                     /**< Peer device's maximum PDU size. Applicable only for data channels. */
    uint8_t                 status;                                      /**< Connection status. Refer to @ref BLE_TRCBPS_STATUS for possible values. */
} BLE_TRCBPS_EvtConnStatus_T;

/** @brief Structure for the @ref BLE_TRCBPS_EVT_RECEIVE_DATA event. */
typedef struct BLE_TRCBPS_EvtReceiveData_T
{
    uint16_t                connHandle;                                  /**< Connection handle associated with this connection. */
    BLE_TRCBPS_ChanType_T   chanType;                                    /**< Channel type. Refer to @ref BLE_TRCBPS_ChanType_T for details. */
    uint8_t                 leL2capId;                                   /**< L2CAP session instance. */
} BLE_TRCBPS_EvtReceiveData_T;

/** @brief Structure for the @ref BLE_TRCBPS_EVT_VENDOR_CMD event. */
typedef struct BLE_TRCBPS_EvtVendorCmd_T
{
    uint16_t                connHandle;                                   /**< Connection handle associated with this connection. */
    uint16_t                length;                                       /**< Length of the vendor command payload. */
    uint8_t                 *p_payLoad;                                   /**< Pointer to the vendor command payload. */
} BLE_TRCBPS_EvtVendorCmd_T;

/** @brief The union of BLE Transparent Credit Based profile event types. */
typedef union BLE_TRCBPS_EventField_T
{
    BLE_TRCBPS_EvtConnStatus_T        connStatus;                          /**< Data for @ref BLE_TRCBPS_EVT_CONNECTION_STATUS event. */
    BLE_TRCBPS_EvtReceiveData_T       onReceiveData;                       /**< Data for @ref BLE_TRCBPS_EVT_RECEIVE_DATA event. */
    BLE_TRCBPS_EvtVendorCmd_T         onVendorCmd;                         /**< Data for @ref BLE_TRCBPS_EVT_VENDOR_CMD event. */
} BLE_TRCBPS_EventField_T;

/**@brief BLE Transparent Credit Based profile callback event. */
typedef struct BLE_TRCBPS_Event_T
{
    BLE_TRCBPS_EventId_T              eventId;                             /**< Identifier of the event. See @ref BLE_TRCBPS_EventId_T. */
    BLE_TRCBPS_EventField_T           eventField;                          /**< Data associated with the event. See @ref BLE_TRCBPS_EventField_T. */
} BLE_TRCBPS_Event_T;

/**
 * @brief Callback type for BLE Transparent Credit Based Profile events.
 *
 * @note This callback function is invoked to handle events from the BLE Transparent Credit Based Profile. 
 *        The application must implement this callback to receive such events.
 *
 * @param p_event Pointer to the event structure containing event-specific data.
 */
typedef void(*BLE_TRCBP_EventCb_T)(BLE_TRCBPS_Event_T *p_event);

/**@} */ //BLE_TRCBPS_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRCBPS_FUNS Functions
 * @{
 */

/**
 *@brief Register a callback for BLE Transparent Credit Based profile events.
 *
 *@param[in] bleTrcbpHandler                 Client callback function to handle BLE TRCBP events.
 *
 */
void BLE_TRCBPS_EventRegister(BLE_TRCBP_EventCb_T bleTrcbpHandler);


/**
 * @brief Initialize the BLE Transparent Credit Based Profile.
 * 
 * @retval MBA_RES_SUCCESS                   Successfully initialized the profile.
 * @retval MBA_RES_INVALID_PARA              Invalid MTU parameter.
 * @retval MBA_RES_FAIL                      Failed to register SPSM or the service.
 *
 */
uint16_t BLE_TRCBPS_Init(void);


/**
 * @brief Query the local L2CAP CoC PSM value for the TRCBP Data Channel.
 * 
 * @param[out] dataPsm                       Pointer to store the PSM of the data channel.
 *
 * @retval MBA_RES_SUCCESS                   Successfully retrieved the PSM value.
 *
 */
uint16_t BLE_TRCBPS_QueryPsm(uint16_t *dataPsm);


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
uint16_t BLE_TRCBPS_ConnReq(uint16_t connHandle);


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
uint16_t BLE_TRCBPS_DisconnectReq(uint16_t connHandle);

/**
 * @brief Send data through the BLE Transparent Credit Based Profile Data pipe.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[in] len                            Length of the data to be sent. Must not exceed @ref BLE_TRCBPS_DATA_MTU.
 * @param[in] p_data                         Pointer to the buffer containing the data to send.
 *
 * @retval MBA_RES_SUCCESS                   Successfully sent data.
 * @retval MBA_RES_OOM                       Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link or data length exceeds max MTU.
 * @retval MBA_RES_NO_RESOURCE               Insufficient credit from the remote device.
 * @retval MBA_RES_BAD_STATE                 Connection not established.
 *
 */
uint16_t BLE_TRCBPS_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data);

/**
 * @brief Send a vendor-specific command through the BLE TRCBP Control pipe.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[in] cmdId                          Vendor command Opcode (see @ref BLE_TRCBPS_VENDOR_OPCODE).
 * @param[in] len                            Length of the vendor command.
 * @param[in] p_payload                      Pointer to the command payload.
 *
 * @retval MBA_RES_SUCCESS                   Successfully sent the vendor command.
 * @retval MBA_RES_OOM                       Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link or data length exceeds max MTU.
 * @retval MBA_RES_BAD_STATE                 Connection not established.
 *
 */
uint16_t BLE_TRCBPS_SendVendorCommand(uint16_t connHandle, uint8_t cmdId, uint16_t len, uint8_t *p_payload);

/**
 * @brief Get the length of queued data in the Data pipe for a given connection handle.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[out] p_dataLength                  Pointer to store the length of the queued data.
 *
 * @retval MBA_RES_SUCCESS                   Successfully retrieved the data length.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link.
 *
 */
uint16_t BLE_TRCBPS_GetDataLength(uint16_t connHandle, uint16_t *p_dataLength);


/**
 * @brief Retrieve queued data from the Data pipe for a given connection handle.
 *
 * @param[in] connHandle                     Connection handle.
 * @param[out] p_data                        Pointer to the data buffer.
 *
 * @retval MBA_RES_SUCCESS                   Get the data successfully.
 * @retval MBA_RES_INVALID_PARA              Invalid L2CAP link.
 * @retval MBA_RES_FAIL                      Queue is empty or other failure.
 *
 */
uint16_t BLE_TRCBPS_GetData(uint16_t connHandle, uint8_t *p_data);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent                  Pointer to the BLE Stack event data structure.
 *
*/
void BLE_TRCBPS_BleEventHandler(STACK_Event_T *p_stackEvent);


/** @} */ //BLE_TRCBPS_FUNS

/** @} */

/** @} */ 

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_TRCBPS_H