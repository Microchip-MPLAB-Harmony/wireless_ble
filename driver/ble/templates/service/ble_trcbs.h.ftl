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
  BLE Transparent Credit Based Service (TRCBS) Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trcbs.h

  Summary:
    Interface for the BLE Transparent Credit Based Service,facilitating the 
    use of TRCBS in BLE applications.

  Description:
    Provides function prototypes and constants necessary for the integration and
    use of TRCBS in BLE applications, enabling efficient data communication.
 *******************************************************************************/

#ifndef BLE_TRCBS_H
#define BLE_TRCBS_H

<#if TRCBS_ENABLE_CONFIG == true>
#include "configuration.h"
</#if>

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
 * @addtogroup BLE_TRCBS BLE Transparent Credit Based Service (TRCBS)
 * @brief Provides the interface for BLE Transparent Credit Based Service.
 * @note This section documents the API for the BLE TRCBS, including
 *          definitions, functions, and data structures necessary for application
 *          developers to utilize the service in BLE communication
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRCBS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_TRCBS_UUID_DEF BLE Transparent Credit Based Service UUID definitions
 * @brief UUIDs for the BLE Transparent Credit Based Service characteristics.
 * @{
 */
<#if TRCBS_ENABLE_CONFIG == true>
#define UUID_MCHP_PROPRIETARY_SERVICE_TRCB_16                      CONFIG_BLE_SVC_TRCBS_UUID_MCHP_PROPRIETARY_SERVICE_TRCB_16
#define UUID_MCHP_TRCB_L2CAP_PSM_16                                CONFIG_BLE_SVC_TRCBS_UUID_MCHP_TRCB_L2CAP_PSM_16
#define UUID_MCHP_TRCB_CTRL_16                                     CONFIG_BLE_SVC_TRCBS_UUID_MCHP_TRCB_CTRL_16
<#else>
#define UUID_MCHP_PROPRIETARY_SERVICE_TRCB_16                      ${CHECKED_TRCBS_SERVICE_UUID}    /* Service UUID */
#define UUID_MCHP_TRCB_L2CAP_PSM_16                                ${CHECKED_TRCBS_PSM_UUID}    /* PSM UUID */
#define UUID_MCHP_TRCB_CTRL_16                                     ${CHECKED_TRCBS_CTRL_UUID}    /* CTRL UUID */
</#if>
/** @} */

/**
 * @defgroup BLE_TRCBS_PSM TRCPB PSM definition
 * @brief The definition of BLE Transparent Credit Based PSM.
 * @{
 */
#define BLE_TRCB_DATA_PSM                                 (0x0081U)                         /**< PSM for the BLE Transparent Credit Based data channel. */
/** @} */

/**
 * @defgroup BLE_TRCBS_ASSIGN_HANDLE TRCBS assigned handles
 * @brief Handles associated with the BLE Transparent Credit Based Service attributes.
 * @{
 */
#define BLE_TRCB_START_HDL                                (0x00C0U)                         /**< Start handle for the BLE Transparent Credit Based service. */

/* Enumeration of attribute handles for the BLE Transparent Credit Based Service. */
typedef enum BLE_TRCB_AttributeHandle_T
{
    BLE_TRCB_HDL_SRV = BLE_TRCB_START_HDL,                                                  /**< Handle for the BLE Transparent Credit Based Service primary service. */
    BLE_TRCB_HDL_CHAR_CTRL,                                                                 /**< Handle for the BLE Transparent Credit Based Control characteristic. */
    BLE_TRCB_HDL_CHARVAL_CTRL,                                                              /**< Handle for the BLE Transparent Credit Based Control characteristic value. */
    BLE_TRCB_HDL_CCCD_CTRL,                                                                 /**< Handle for the BLE Transparent Credit Based Control CCCD value. */
    BLE_TRCB_HDL_CHAR_L2CAP_PSM,                                                            /**< Handle for the BLE Transparent Credit Based L2CAP PSM characteristic. */
    BLE_TRCB_HDL_CHARVAL_L2CAP_PSM                                                          /**< Handle for the BLE Transparent Credit Based L2CAP PSM characteristic value. */
}BLE_TRCB_AttributeHandle_T;

#define BLE_TRCB_END_HDL                                  BLE_TRCB_HDL_CHARVAL_L2CAP_PSM    /**< End handle for the BLE Transparent Credit Based Service. */
/** @} */

/** @} */ //BLE_TRCBS_DEFINES

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_TRCBS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Transparent Credit Based Service to the GATT server.
 *
 * This function adds the BLE Transparent Credit Based Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Transparent Credit Based service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Transparent Credit Based service.
 */
uint16_t BLE_TRCBS_Add(void);

/** @} */ //BLE_TRCBS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_TRCBS_H