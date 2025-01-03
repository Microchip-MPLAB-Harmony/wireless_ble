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
  BLE Link Loss Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_lls.h

  Summary:
    Interface for the BLE Link Loss Service, providing function prototypes
    and definitions for use by application developers.

  Description:
    The BLE Link Loss Service is part of the BLE service layer. This file
    includes the necessary interfaces for the application user to interact with
    the Link Loss Service. It is designed to alert the user when the BLE link
    is lost with a connected peer device.
 *******************************************************************************/
#ifndef BLE_LLS_H
#define BLE_LLS_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @addtogroup BLE_SERVICE BLE Service
 * @{
 */

/**
 * @addtogroup BLE_LLS BLE Link Loss Service
 * @brief Provides the interface for the BLE Link Loss Service.
 * @note This module contains the definitions and prototypes necessary to
 *          integrate the BLE Link Loss Service into BLE applications. It
 *          defines the behavior required to manage alerts when a BLE link is
 *          lost.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_LLS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_LLS_UUID_DEF Link Loss Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the Link Loss Service characteristics.
 * @{ */
#define UUID_LINKLOSS_SERVICE                           (0x1803U)                     /**< UUID for Link Loss Service.*/
#define UUID_ALERT_LEVEL                                (0x2A06U)                     /**< UUID for Alert Level characteristic.  */
/** @} */


/**
 * @defgroup BLE_LLS_ASSIGN_HANDLE LLS assigned handles
 * @brief Handles associated with the BLE Link Loss Service attributes.
 * @{ */
#define LLS_START_HDL                                   (0x00F0U)                     /**< Start handle for the BLE Lonk Loss Service. */

/* Enumeration of attribute handles for the BLE Link Loss Service. */
typedef enum BLE_LLS_AttributeHandle_T
{
    LLS_HDL_SVC = LLS_START_HDL,                                                      /**< Handle for the BLE Link Loss Service primary service. */
    LLS_HDL_CHAR_ALERT_LEVEL,                                                         /**< Handle for the BLE Alert Level characteristic. */
    LLS_HDL_CHARVAL_ALERT_LEVEL,                                                      /**< Handle for the BLE Alert Level characteristic value. */
}BLE_LLS_AttributeHandle_T;

#define LLS_END_HDL                                 (LLS_HDL_CHARVAL_ALERT_LEVEL)     /**< End handle for the BLE Link Loss Service. */
/** @} */

/** @} */ //BLE_LLS_DEFINES

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_LLS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Link Loss Service to the GATT server.
 *
 * This function adds the BLE Link Loss Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Link Loss service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Link Loss service.
 */
uint16_t BLE_LLS_Add(void);

/** @} */ //BLE_LLS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_LLS_H