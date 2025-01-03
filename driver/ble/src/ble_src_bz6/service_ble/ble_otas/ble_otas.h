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
  BLE OTA (Over-The-Air) Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otas.h

  Summary:
    Interface for the BLE OTA Service, providing function prototypes
    and definitions for use by application developers.

  Description:
    The BLE OTA Service allows for firmware updates of BLE devices.
    This file defines the service's interface and should be included by
    applications that implement the OTA functionality.
 *******************************************************************************/

#ifndef BLE_OTAS_H
#define BLE_OTAS_H

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
 * @addtogroup BLE_OTAS BLE OTA Service
 * @brief Provides the interface for the BLE Over-The-Air (OTA) Service.
 * @note This module defines the API for enabling OTA firmware updates
 *          for BLE devices. It includes function prototypes, data structures,
 *          and constants relevant to the OTA process.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_OTAS_ASSIGN_HANDLE OTAS assigned handles
 * @brief Handles associated with the BLE OTA Service attributes.
 * @{
 */
#define BLE_OTAS_START_HDL                                    (0x0100U)                 /**< Start handle for the BLE OTA Service.*/


/* Enumeration of attribute handles for the BLE OTA Service. */
typedef enum BLE_OTAS_AttributeHandle_T
{
    BLE_OTAS_HDL_SRV = BLE_OTAS_START_HDL,                                              /**< Handle for the BLE OTA Service primary service. */
    BLE_OTAS_HDL_FEATURE,                                                               /**< Handle for the OTA Feature characteristic. */
    BLE_OTAS_HDL_FEATURE_VAL,                                                           /**< Handle for the OTA Feature characteristic value. */
    BLE_OTAS_HDL_CTRL,                                                                  /**< Handle for the OTA Control Point characteristic. */
    BLE_OTAS_HDL_CTRL_VAL,                                                              /**< Handle for the OTA Control Point characteristic value. */
    BLE_OTAS_HDL_CTRL_CCCD,                                                             /**< Handle for the OTA Control Point CCCD value. */
    BLE_OTAS_HDL_DATA,                                                                  /**< Handle for the OTA Data characteristic. */
    BLE_OTAS_HDL_DATA_VAL,                                                              /**< Handle for the OTA Data characteristic value. */
    BLE_OTAS_HDL_DATA_CCCD                                                              /**< Handle for the OTA Data CCCD value. */
}BLE_OTAS_AttributeHandle_T;

#define BLE_OTAS_END_HDL                                      BLE_OTAS_HDL_DATA_CCCD    /**< End handle for the BLE OTA Service. */
/** @} */

/** @} */ //BLE_OTAS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE OTA Service to the GATT server.
 *
 * This function adds the BLE OTA Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE OTA service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE OTA service.
 */
uint16_t BLE_OTAS_Add(void);

/** @} */ //BLE_OTAS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_OTAS_h