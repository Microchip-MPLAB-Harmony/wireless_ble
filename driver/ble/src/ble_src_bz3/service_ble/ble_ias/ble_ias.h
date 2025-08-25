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
  BLE Immediate Alert Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_ias.h

  Summary:
    Interface for the BLE Immediate Alert Service, providing function prototypes
    and definitions for use by application developers.

  Description:
    This header file includes function prototypes and definitions for the
    implementation of the BLE Immediate Alert Service, facilitating the development
    of applications that require immediate alert notifications.
 *******************************************************************************/

#ifndef BLE_IAS_H
#define BLE_IAS_H

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
 * @addtogroup BLE_IAS BLE Immediate Alert Service
 * @brief Provides an interface for the BLE Immediate Alert Service.
 * @note This module contains the definitions and function prototypes necessary
 *          to interface with the BLE Immediate Alert Service, enabling the
 *          application to handle immediate alert notifications effectively.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_IAS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_IAS_UUID_DEF Immediate Alert Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the Immediate Alert Service characteristics.
 * @{
 */
#define UUID_IMMEDIATE_ALERT_SERVICE                    (0x1802U)                       /**< UUID for Immediate Alert Service. */
#define UUID_ALERT_LEVEL                                (0x2A06U)                       /**< UUID for Alert Level characteristic. */
/** @} */

/**
 * @defgroup BLE_IAS_ASSIGN_HANDLE IAS assigned handles
 * @brief Handles associated with the BLE Immediate Alert Service attributes.
 * @{
 */
#define IAS_START_HDL                                   (0x0FAU)                        /**< Start handle for the BLE Immediate Alert Service. */

/* Enumeration of attribute handles for the BLE Immediate Alert Service. */
typedef enum BLE_IAS_AttributeHandle_T
{
    IAS_HDL_SVC = IAS_START_HDL,                                                        /**< Handle for the BLE Immediate Alert Service primary service. */
    IAS_HDL_CHAR_ALERT_LEVEL,                                                           /**< Handle for the BLE Alert Level characteristic. */
    IAS_HDL_CHARVAL_ALERT_LEVEL,                                                        /**< Handle for the BLE Alert Level characteristic value. */
}BLE_IAS_AttributeHandle_T;

#define IAS_END_HDL                                 (IAS_HDL_CHARVAL_ALERT_LEVEL)       /**< End handle for the BLE Immediate Alert Service. */
/** @} */

/** @} */ //BLE_IAS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_IAS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Immediate Alert Service to the GATT server.
 *
 * This function adds the BLE Immediate Alert Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Immediate Alert service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Immediate Alert service.
 */
uint16_t BLE_IAS_Add(void);

/** @} */ //BLE_IAS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_IAS_H