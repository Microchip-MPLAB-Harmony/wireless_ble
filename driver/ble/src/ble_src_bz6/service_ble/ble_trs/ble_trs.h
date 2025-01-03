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
  BLE Transparent Service (TRS) Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trs.h

  Summary:
    Interface for the BLE Transparent Service,facilitating the use of TRS in 
    BLE applications.

  Description:
    Provides function prototypes and constants necessary for the integration and
    use of TRS in BLE applications, enabling efficient data communication.
 *******************************************************************************/
#ifndef BLE_TRS_H
#define BLE_TRS_H

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
 * @addtogroup BLE_TRS BLE Transparent Service
 * @{
 * @brief Provides an interface for the BLE Transparent Service.
 * @note This module defines the API for the BLE Transparent Service, facilitating
 * transparent data communication over BLE. It includes function prototypes and
 * structures needed by an application to utilize the service.
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_TRS_UUID_DEF BLE Transparent Service UUID definitions
 * @brief UUIDs for the BLE Transparent Service characteristics.
 * @{
 */
#define UUID_MCHP_PROPRIETARY_SERVICE_16             0x55,0xE4,0x05,0xD2,0xAF,0x9F,0xA9,0x8F,0xE5,0x4A,0x7D,0xFE,0x43,0x53,0x53,0x49         /**< UUID for MCHP BLE Transparent service. (inverse format) */
#define UUID_MCHP_TRANS_TX_16                        0x16,0x96,0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49         /**< UUID for MCHP BLE Transparent TX characteristic. (inverse format) */
#define UUID_MCHP_TRANS_RX_16                        0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49         /**< UUID for MCHP BLE Transparent RX characteristic. (inverse format) */
#define UUID_MCHP_TRANS_CTRL_16                      0x7e,0x3b,0x07,0xff,0x1c,0x51,0x49,0x2f,0xb3,0x39,0x8a,0x4c,0x43,0x53,0x53,0x49         /**< UUID for MCHP BLE Transparent Control characteristic. (inverse format) */
/** @} */

/**
 * @defgroup BLE_TRS_ASSIGN_HANDLE TRS assigned handles
 * @brief Handles associated with the BLE Transparent Service attributes.
 * @{
 */
#define TRS_START_HDL                               (0x00A0U)             /**< Start handle for the BLE Transparent service. */

/* Enumeration of attribute handles for the BLE Transparent Service. */
typedef enum BLE_TRS_AttributeHandle_T
{
    TRS_HDL_SVC = TRS_START_HDL,                                          /**< Handle for the BLE Transparent Service primary service. */
    TRS_HDL_CHAR_TX,                                                      /**< Handle for the Transparent TX characteristic. */
    TRS_HDL_CHARVAL_TX,                                                   /**< Handle for the Transparent TX characteristic value. */
    TRS_HDL_CCCD_TX,                                                      /**< Handle for the Transparent TX characteristic CCCD value.*/
    TRS_HDL_CHAR_RX,                                                      /**< Handle for the Transparent RX characteristic. */
    TRS_HDL_CHARVAL_RX,                                                   /**< Handle for the Transparent RX characteristic value. */
    TRS_HDL_CHAR_CTRL,                                                    /**< Handle for the Transparent Control characteristic. */
    TRS_HDL_CHARVAL_CTRL,                                                 /**< Handle for the Transparent Control characteristic value. */
    TRS_HDL_CCCD_CTRL                                                     /**< Handle for the Transparent Control characteristic CCCD value. */
}BLE_TRS_AttributeHandle_T;

#define TRS_END_HDL                                 TRS_HDL_CCCD_CTRL     /**< End handle for the BLE Transparent Service.  */
/** @} */

/** @} */ //BLE_TRS_DEFINES

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Transparent Service to the GATT server.
 *
 * This function adds the BLE Transparent Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Transparent service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Transparent service.
 */
uint16_t BLE_TRS_Add(void);


/**
 * @brief Configures the permissions for a characteristic within the transparent service.
 *
 * This function sets the access permissions for a given characteristic's attribute handle within the transparent service.
 * It should be called after initializing the transparent service with BLE_TRS_Add().
 *
 * @param[in] attrHdl                         Transparent service attribute handle.
 *                                            Refer to @ref BLE_TRS_AttributeHandle_T for possible values.
 * @param[in] permissions                     Attribute permissions to be set. Refer to @ref GATT_ATTRIBUTE_PERMISSIONS for possible values.
 *
 * @retval MBA_RES_SUCCESS                    The permissions were successfully configured.
 * @retval MBA_RES_INVALID_PARA               The provided attribute handle does not belong to the transparent service.
 *
 */
uint16_t BLE_TRS_PermissionConfig(uint16_t attrHdl, uint8_t permissions);

/** @} */ //BLE_TRS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_TRS_H