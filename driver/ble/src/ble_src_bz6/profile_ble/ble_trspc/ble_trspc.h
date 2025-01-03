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
    ble_trspc.h

  Summary:
    Interface for the BLE Transparent Client Profile, providing function prototypes
    and definitions for use by application developers.

  Description:
    This header file exposes the API and definitions for the BLE Transparent Client
    Profile. It enables the application to interact with the BLE Transparent Profile,
    facilitating communication with remote transparent servers.
 *******************************************************************************/
#ifndef BLE_TRSPC_H
#define BLE_TRSPC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stack_mgr.h"
#include "ble_gcm/ble_dd.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @addtogroup BLE_PROFILE BLE Profile
 * @{
 */

/**
 * @addtogroup BLE_TRSP BLE Transparent Profile
 * @{
 */

/**
 * @defgroup BLE_TRSPC Transparent Profile Client Role (TRSPC)
 * @brief Provides a interface for the BLE Transparent Profile Client.
 * @note This module provides the necessary interfaces and definitions for the
 *          application to use the Transparent Profile Client functionalities. It
 *          includes function prototypes and constants relevant to the client role.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRSPC_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_TRSPC_MAX_CONN_NBR TRSPC maximum connection number
 * @brief Defines the maximum number of concurrent connections.
 * @{
 */
#define BLE_TRSPC_MAX_CONN_NBR                  BLE_GAP_MAX_LINK_NBR    /**< Maximum number of concurrent connections supported by the device. */
/** @} */


/**
 * @defgroup BLE_TRSPC_UL_STATUS TRSPC uplink status definition
 * @brief Defines the status of the BLE transparent service uplink.
 * @{
 */
#define BLE_TRSPC_UL_STATUS_DISABLED            (0x00U)                 /**< Uplink functionality is disabled. */
#define BLE_TRSPC_UL_STATUS_CBFCENABLED         (0x01U)                 /**< Uplink functionality is enabled with credit-based flow control. */
/** @} */

/**
 * @defgroup BLE_TRSPC_DL_STATUS TRSPC downlink status definition
 * @brief Defines the status of the BLE transparent service downlink.
 * @{
 */
#define BLE_TRSPC_DL_STATUS_DISABLED            (0x00U)                 /**< Downlink functionality is disabled. */
#define BLE_TRSPC_DL_STATUS_NONCBFCENABLED      (0x10U)                 /**< Downlink functionality is enabled without credit-based flow control. */
#define BLE_TRSPC_DL_STATUS_CBFCENABLED         (0x20U)                 /**< Downlink functionality is enabled with credit-based flow control. */
/** @} */


/** @} */ //BLE_TRSPC_DEFINES

/**
 * @addtogroup BLE_TRSPC_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration of BLE Transparent Profile callback event types. */
typedef enum BLE_TRSPC_EventId_T
{
    BLE_TRSPC_EVT_UL_STATUS = 0x01,                                   /**< Event indicating an uplink status update. 
                                                                            Refer to @ref BLE_TRSPC_EvtUplinkStatus_T for details. */
    BLE_TRSPC_EVT_DL_STATUS,                                          /**< Event indicating a downlink status update. 
                                                                            Refer to @ref BLE_TRSPC_EvtDownlinkStatus_T for details. */
    BLE_TRSPC_EVT_RECEIVE_DATA,                                       /**< Event indicating data has been received on the data channel. 
                                                                            Refer to @ref BLE_TRSPC_EvtReceiveData_T for details. */
    BLE_TRSPC_EVT_VENDOR_CMD,                                         /**< Event indicating a vendor-specific command has been received. 
                                                                            Refer to @ref BLE_TRSPC_EvtVendorCmd_T for details. */
    BLE_TRSPC_EVT_VENDOR_CMD_RSP,                                     /**< Event indicating a response to a vendor-specific command has been received. 
                                                                            Refer to @ref BLE_TRSPC_EvtVendorCmdRsp_T for details. */
    BLE_TRSPC_EVT_DISC_COMPLETE,                                      /**< Event indicating the discovery process has completed. 
                                                                            Refer to @ref BLE_TRSPC_EvtDiscComplete_T for details. */
    BLE_TRSPC_EVT_ERR_NO_MEM,                                         /**< Event indicating an internal error due to insufficient heap memory. */
    BLE_TRSPC_EVT_END                                                 /**< Sentinel value indicating the end of the enumeration. */
}BLE_TRSPC_EventId_T;

/** @} */ //BLE_TRSPC_ENUMS


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRSPC_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_TRSPC_EVT_UL_STATUS event. */
typedef struct BLE_TRSPC_EvtUplinkStatus_T
{
    uint16_t         connHandle;                        /**< Connection handle associated with this connection. */
    uint8_t          status;                            /**< Uplink connection status. Refer to @ref BLE_TRSPC_UL_STATUS for possible values. */
}   BLE_TRSPC_EvtUplinkStatus_T;

/** @brief Structure for the @ref BLE_TRSPC_EVT_DL_STATUS event. */
typedef struct BLE_TRSPC_EvtDownlinkStatus_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         status;                             /**< Downlink connection status. Refer to @ref BLE_TRSPC_DL_STATUS for possible values. */
    uint8_t         currentCreditNumber;                /**< Current number of available credits for downlink communication. */
}   BLE_TRSPC_EvtDownlinkStatus_T;

/** @brief Structure for the @ref BLE_TRSPC_EVT_RECEIVE_DATA event. */
typedef struct BLE_TRSPC_EvtReceiveData_T
{
    uint16_t         connHandle;                        /**< Connection handle associated with this connection. */
}   BLE_TRSPC_EvtReceiveData_T;

/**@brief Structure for the @ref BLE_TRSPC_EVT_VENDOR_CMD event. */
typedef struct BLE_TRSPC_EvtVendorCmd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         payloadLength;                      /**< Length of the vendor command payload. */
    uint8_t         *p_payLoad;                         /**< Pointer to the vendor command payload. */
}   BLE_TRSPC_EvtVendorCmd_T;

/** @brief Structure for the @ref BLE_TRSPC_EVT_VENDOR_CMD_RSP event. */
typedef struct BLE_TRSPC_EvtVendorCmdRsp_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
}   BLE_TRSPC_EvtVendorCmdRsp_T;

/**@brief Structure for the @ref BLE_TRSPC_EVT_DISC_COMPLETE event. */
typedef struct BLE_TRSPC_EvtDiscComplete_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
}   BLE_TRSPC_EvtDiscComplete_T;

/** @brief The union of BLE Transparent profile client event types. */
typedef union
{
    BLE_TRSPC_EvtUplinkStatus_T     onUplinkStatus;     /**< Data for @ref BLE_TRSPC_EVT_UL_STATUS event. */
    BLE_TRSPC_EvtDownlinkStatus_T   onDownlinkStatus;   /**< Data for @ref BLE_TRSPC_EVT_DL_STATUS event. */
    BLE_TRSPC_EvtReceiveData_T      onReceiveData;      /**< Data for @ref BLE_TRSPC_EVT_RECEIVE_DATA event. */
    BLE_TRSPC_EvtVendorCmd_T        onVendorCmd;        /**< Data for @ref BLE_TRSPC_EVT_VENDOR_CMD event. */
    BLE_TRSPC_EvtVendorCmdRsp_T     onVendorCmdRsp;     /**< Data for @ref BLE_TRSPC_EVT_VENDOR_CMD_RSP event. */
    BLE_TRSPC_EvtDiscComplete_T     onDiscComplete;     /**< Data for @ref BLE_TRSPC_EVT_DISC_COMPLETE event. */
} BLE_TRSPC_EventField_T;


/** @brief BLE Transparent profile client callback event. */
typedef struct  BLE_TRSPC_Event_T
{
    BLE_TRSPC_EventId_T             eventId;            /**< Identifier of the event. See @ref BLE_TRSPC_EventId_T. */
    BLE_TRSPC_EventField_T          eventField;         /**< Data associated with the event. See @ref BLE_TRSPC_EventField_T. */
} BLE_TRSPC_Event_T;

/** 
 * @brief Callback type for BLE Transparent profile client events.
 * 
 * @note This callback function is used to send events from the BLE Transparent profile client to the application.
 * 
 * @param p_event Pointer to the event structure containing details about the event.
 */
typedef void(*BLE_TRSPC_EventCb_T)(BLE_TRSPC_Event_T *p_event);

/** @} */ //BLE_TRSPC_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRSPC_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Transparent Profile Client.
 *
 * @retval MBA_RES_SUCCESS                  BLE Transparent Profile Client was successfully initialized.
 * @retval MBA_RES_FAIL                     BLE Transparent Profile Client failed to initialize.
 *
 */
uint16_t BLE_TRSPC_Init(void);

/**
 * @brief Registers a callback function for BLE Transparent profile client events.
 *
 * @param[in] bleTranCliHandler             The client callback function to handle BLE Transparent Profile client events.
 *
 */
void BLE_TRSPC_EventRegister(BLE_TRSPC_EventCb_T bleTranCliHandler);

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
uint16_t BLE_TRSPC_SendVendorCommand(uint16_t connHandle, uint8_t commandID, uint8_t commandLength, uint8_t *p_commandPayload);

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
uint16_t BLE_TRSPC_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data);

/**
 * @brief Retrieves the length of data queued.
 *
 * @param[in]  connHandle                   Connection handle associated with the queued data.
 * @param[out] p_dataLength                 Pointer to where the data length will be stored.
 *
 */
void BLE_TRSPC_GetDataLength(uint16_t connHandle, uint16_t *p_dataLength);

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
uint16_t BLE_TRSPC_GetData(uint16_t connHandle, uint8_t *p_data);

/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called by the application when BLE stack events occur.
 *
 * @param[in] p_stackEvent        					Pointer to the BLE stack event data.
 *
 */
void BLE_TRSPC_BleEventHandler(STACK_Event_T *p_stackEvent);

/**
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *
 * @note This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event                       Pointer to the BLE_DD event data.
 *
 */
void BLE_TRSPC_BleDdEventHandler(BLE_DD_Event_T *p_event);

/** @} */ //BLE_TRSPC_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_TRSPC_H