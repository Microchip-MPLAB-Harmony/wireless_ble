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
  BLE Service Change Manager Middleware Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_scm.h

  Summary:
    This header file provides prototypes and definitions for the BLE Service 
    Change Manager, which handles GATT service changes on a remote GATT server.

  Description:
    The BLE Service Change Manager (SCM) is part of the BLE middleware that 
    facilitates the management of service changes on remote GATT servers. It 
    provides an API for applications to initialize the service change manager 
    and to respond to service changes. The "BLE_SCM_Init" function must be 
    called during system initialization, typically in the "APP_Initialize" 
    function, to prepare the SCM for operation.
 *******************************************************************************/
#ifndef BLE_SCM_H
#define BLE_SCM_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include "stack_mgr.h"
#include "ble_dd.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

/** 
 * @addtogroup BLE_MW BLE Middleware
 * @{ 
 */

/**
 * @defgroup BLE_SCM BLE Service Change Manager (BLE_SCM)
 * 
 * @brief Manages GATT service changes on remote GATT servers.
 * @note This section declares the API for the BLE Service Change Manager. 
 *          It includes initialization functions, event handling, and other 
 *          utilities necessary for the management of service changes. The 
 *          integration of this module ensures that the application can 
 *          appropriately handle scenarios where GATT services are added, 
 *          removed, or modified on a connected remote device.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BLE_SCM_DEFINES Defines
 * @{
 */
 
/**
 * @defgroup BLE_SCM_DISC_NUM Number of discovering characteristics of GATT service
 * @brief Defines the number of characteristics to be discovered in a GATT service.
 * @{
 */
#define BLE_SCM_GATT_DISC_CHAR_NUM          (0x03U)   /**< Number of characteristics to be discovered in the GATT Service. */
/** @} */

/** @} */ //BLE_SCM_DEFINES


/**
 * @addtogroup BLE_SCM_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration for BLE Service Change Module (SCM) event identifiers. */
typedef enum BLE_SCM_EventId_T
{
    BLE_SCM_EVT_SVC_CHANGE,                           /**< Event indicating a service change indication has been received from a remote device. 
                                                            Refer to @ref BLE_SCM_EvtServiceChange_T for the structure detailing event information. */
    BLE_SCM_EVT_BONDED_CHAR_INFO,                     /**< Event indicating characteristic handle information for the GATT Service of a remote bonded device is available.
                                                            The application is responsible for managing this bonded information, such as storing it in persistent storage.
                                                            Refer to @ref BLE_SCM_EvtBondedCharInfo_T for the structure detailing event information. */
    BLE_SCM_EVT_CONFIGURED,                           /**< Event generated after GATT client configuration procedures are completed.
                                                            Refer to @ref BLE_SCM_EvtConfigured_T for the structure detailing event information. */
    BLE_SCM_EVT_DISC_COMPLETE,                        /**< Event generated upon completion of GATT service discovery for a remote connection.
                                                            Refer to @ref BLE_SCM_EvtDiscComplete_T for the structure detailing event information. */
    BLE_SCM_EVT_END                                   /**< Sentinel value indicating the end of the event enumeration. */
}BLE_SCM_EventId_T;

/** @} */ //BLE_SCM_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_SCM_STRUCTS Structures
 * @{
 */

/** @brief Structure for @ref BLE_SCM_EVT_BONDED_CHAR_INFO event. */
typedef struct BLE_SCM_EvtBondedCharInfo_T
{
    uint16_t                    connHandle;                             /**< Connection handle associated with this connection. */
    BLE_DD_CharInfo_T           charInfo[BLE_SCM_GATT_DISC_CHAR_NUM];   /**< Array of characteristic information for the bonded remote peer's GATT service. See @ref BLE_DD_CharInfo_T.*/
} BLE_SCM_EvtBondedCharInfo_T;


/** @brief Structure for @ref BLE_SCM_EVT_SVC_CHANGE event. */
typedef struct BLE_SCM_EvtServiceChange_T
{
    uint16_t                    connHandle;                             /**< Connection handle associated with this connection. */
    uint16_t                    scStartHandle;                          /**< Starting handle of the changed service on the remote device. */
    uint16_t                    scEndHandle;                            /**< Ending handle of the changed service on the remote device. */
} BLE_SCM_EvtServiceChange_T;


/** @brief Structure for @ref BLE_SCM_EVT_CONFIGURED event. */
typedef struct BLE_SCM_EvtConfigured_T
{
    uint16_t                    connHandle;                             /**< Connection handle associated with this connection. */
} BLE_SCM_EvtConfigured_T;


/**@brief Structure for @ref BLE_SCM_EVT_DISC_COMPLETE event. */
typedef struct BLE_SCM_EvtDiscComplete_T
{
    uint16_t                    connHandle;                             /**< Connection handle associated with this connection. */
} BLE_SCM_EvtDiscComplete_T;


/** @brief Union of BLE_SCM callback event data types. */
typedef union
{
    BLE_SCM_EvtServiceChange_T  evtServiceChange;                       /**< Data for @ref BLE_SCM_EVT_SVC_CHANGE event. */
    BLE_SCM_EvtBondedCharInfo_T evtBondedCharInfo;                      /**< Data for @ref BLE_SCM_EVT_BONDED_CHAR_INFO event. */
    BLE_SCM_EvtConfigured_T     evtConfigured;                          /**< Data for @ref BLE_SCM_EVT_CONFIGURED event. */
    BLE_SCM_EvtDiscComplete_T   evtDiscComplete;                        /**< Data for @ref BLE_SCM_EVT_DISC_COMPLETE event. */
} BLE_SCM_EventField_T;


/** @brief BLE_SCM callback event structure. */
typedef struct  BLE_SCM_Event_T
{
    BLE_SCM_EventId_T           eventId;                                /**< Identifier for the event. See @ref BLE_SCM_EventId_T. */
    BLE_SCM_EventField_T        eventField;                             /**< Data associated with the event. See @ref BLE_SCM_EventField_T. */
} BLE_SCM_Event_T;


/**
 * @brief Callback type for BLE_SCM events.
 * 
 * This callback function is used to send BLE_SCM events to the application.
 * 
 * @param[in] p_event Pointer to the event structure containing event details. See @ref BLE_SCM_Event_T.
 */
typedef void (*BLE_SCM_EventCb_T)(BLE_SCM_Event_T *p_event);

/** @} */ //BLE_SCM_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_SCM_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Service Change Module (BLE_SCM).
 *
 * @note This module is necessary when GATT client functionality is supported and it is required to manage service change indications from a remote server.
 *        The module depends on the BLE Device Discovery (BLE_DD) module.
 *
 * @retval MBA_RES_SUCCESS      Initialization of the BLE SCM module was successful.
 * @retval MBA_RES_FAIL         Initialization of the BLE SCM module failed.
*/
uint16_t BLE_SCM_Init(void);


/**
 * @brief Registers a callback function for BLE_SCM events.
 * @note  This function should be invoked during the application's initialization phase to ensure that BLE_SCM events are properly handled.
 *
 * @param[in] eventCb           The callback function that will handle BLE_SCM events. See @ref BLE_SCM_EventCb_T.
 *
*/
void BLE_SCM_EventRegister(BLE_SCM_EventCb_T eventCb);


/**
 * @brief Handles BLE stack events.
 * @note  This function must be called for each BLE event that occurs to ensure proper operation of the BLE_SCM.
 *
 * @param[in] p_stackEvent      Pointer to @ref STACK_Event_T structure buffer.
 *
*/
void BLE_SCM_BleEventHandler(STACK_Event_T *p_stackEvent);


/**
 * @brief Handles BLE Device Discovery (BLE_DD) events.
 * @note  This function must be called for each BLE_DD event that occurs to ensure proper operation of the BLE_SCM.
 *
 * @param[in] p_event           Pointer to the @ref BLE_DD_Event_T structure containing the BLE_DD event information.
 *
*/
void BLE_SCM_BleDdEventHandler(BLE_DD_Event_T *p_event);


/**
 * @brief Sets the characteristic handle information for a bonded connection.
 * @note  The application is responsible for caching the characteristic handle information and providing it to BLE_SCM when a bonded link is re-established.
 *
 * @param[in] connHandle        Connection handle associated with this connection.
 * @param[in] p_charInfo        Pointer to the array of characteristic information structures for the GATT service of the bonded remote peer. 
 *                              The array size should not exceed @ref BLE_SCM_GATT_DISC_CHAR_NUM. See @ref BLE_DD_CharInfo_T.
 *
*/
void BLE_SCM_SetBondedCharInfo(uint16_t connHandle, BLE_DD_CharInfo_T *p_charInfo);

/** @} */ //BLE_SCM_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_SCM_H