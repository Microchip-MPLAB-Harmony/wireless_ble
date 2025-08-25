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
  BLE Log Middleware Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_log.h

  Summary:
    This header file provides the prototypes and definitions for the BLE Log 
    middleware used in this project.

  Description:
    The BLE Log middleware facilitates logging of BLE Stack events and 
    data for debugging and analysis purposes. This file defines the API for 
    interacting with the BLE Log functionality.
 *******************************************************************************/
#ifndef BLE_LOG_H
#define BLE_LOG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <string.h>
#include "bt_sys_log.h"

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
 * @defgroup BLE_LOG BLE Log (BLE_LOG)
 * 
 * @brief Provides functions for handling BLE Stack logs.
 * @note This file includes the necessary definitions and prototypes to interface
 *       with the BLE Log functionality provided by the middleware.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BLE_LOG_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_LOG_TYPE BLE log types
 * @brief Defines various BLE log types used for debugging and tracing BLE communication.
 * @{
 */
#define BLE_LOG_TYPE_HCI_COMMAND            0x01                                            /**< HCI Command log type. */
#define BLE_LOG_TYPE_HCI_ACL_TX             0x02                                            /**< HCI ACL Data TX (transmit) packet log type. */
#define BLE_LOG_TYPE_HCI_ACL_RX             0x03                                            /**< HCI ACL Data RX (receive) packet log type. */
#define BLE_LOG_TYPE_HCI_EVENT              0x04                                            /**< HCI Event packet log type. */
/** @} */


/**@} */ //BLE_LOG_DEFINES

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************

/**
 * @addtogroup BLE_LOG_STRUCTS Structures
 * @{
 */

/**
 * @brief Callback type for BLE logging events.
 *
 * This callback function is used to send BLE logging events to the application.
 * It is triggered with the log data when a logging event occurs.
 *
 * @param[in] logType       The type of the log event. See @ref BLE_LOG_TYPE for value definitions.
 * @param[in] logLength     The length of the log data payload in bytes.
 * @param[in] p_logPayload  Pointer to the log data payload. This is the actual data 
 *                          associated with the log event.
 */
typedef void (*BLE_LOG_EventCb_T)(uint8_t logType, uint16_t logLength, uint8_t *p_logPayload);

/** @} */ //BLE_LOG_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BLE_LOG_FUNS Functions
 * @{
 */

/**
 * @brief Registers a callback function for BLE logging events.
 *
 * This function allows a client to register a callback that will be invoked
 * when a BLE logging event occurs.
 * 
 * @param[in] eventCb       The function pointer to the callback that handles BLE logging events. See @ref BLE_LOG_EventCb_T.
*/
void BLE_LOG_EventRegister(BLE_LOG_EventCb_T eventCb);


/**
 * @brief Processes BLE stack logs containing HCI traffic information.
 *
 * This function is responsible for handling the debug logs generated by the BLE stack.
 * It processes the HCI traffic information contained within the logs.
 * 
 * @param[in] p_log        A pointer to the @ref BT_SYS_LogEvent_T structure that contains the log information.
*/
void BLE_LOG_StackLogHandler(BT_SYS_LogEvent_T *p_log);

/** @} */ //BLE_LOG_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_LOG_H