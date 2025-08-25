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
  BLE TX Power Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_tps.h

  Summary:
    Interface definitions for the BLE TX Power Service (TPS).

  Description:
    Provides function prototypes and constants relevant to the BLE TPS,
    facilitating the management of the transmit power level of a BLE device
    within an application context.
 *******************************************************************************/
#ifndef BLE_TPS_H
#define BLE_TPS_H

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
 * @addtogroup BLE_TPS BLE TX Power Service
 * @brief Provides the interface for the BLE TX Power Service.
 * @note This section defines the API for the BLE TPS which allows
 *          monitoring of the transmission power level in a BLE application.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TPS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_TPS_UUID_DEF TX Power Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the TX Power Service characteristics.
 * @{
 */
#define UUID_TXPOWER_SERVICE                            (0x1804U)                         /**< UUID for TX Power Service. */
#define UUID_TXPOWER_LEVEL                              (0x2A07U)                         /**< UUID for TX Power Level characteritsic. */
/** @} */

/**
 * @defgroup BLE_TPS_ASSIGN_HANDLE TPS assigned handles
 * @brief Handles associated with the BLE TX Power Service attributes.
 * @{
 */
#define TPS_START_HDL                                   (0x139U)                          /**< Start handle for the BLE TX Power Service. */

/* Enumeration of attribute handles for the BLE TX Power Service. */
typedef enum BLE_TPS_AttributeHandle_T
{
    TPS_HDL_SVC = TPS_START_HDL,                                                          /**< Handle for the BLE TX Power Service primary service. */
    TPS_HDL_CHAR_TXPOWER_LEVEL,                                                           /**< Handle for thef TX Power Level characteristic. */
    TPS_HDL_CHARVAL_TXPOWER_LEVEL,                                                        /**< Handle for the TX Power Level characteristic value. */
}BLE_TPS_AttributeHandle_T;

#define TPS_END_HDL                                     (TPS_HDL_CHARVAL_TXPOWER_LEVEL)   /**< End handle for the BLE TX Power Service. */
/** @} */

/** @} */ //BLE_TPS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TPS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE TX Power Service to the GATT server.
 *
 * This function adds the BLE TX Power Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE TX Power service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE TX Power service.
 */
uint16_t BLE_TPS_Add(void);

/** @} */ //BLE_TPS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_TPS_H