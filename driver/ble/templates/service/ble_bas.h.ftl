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
  BLE Battery Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_bas.h

  Summary:
    Interface for the BLE Battery Service used in applications.

  Description:
    Provides the API and necessary definitions for the BLE Battery Service,
    enabling the management of the battery state within a BLE application.
 *******************************************************************************/

#ifndef BLE_BAS_H
#define BLE_BAS_H

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
 * @addtogroup BLE_BAS BLE Battery Service
 * @brief Provides the interface for the BLE Battery Service.
 * @note This section documents the API and necessary details for integrating
 * the BLE Battery Service into a BLE application, facilitating battery level
 * reporting over BLE connections.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
<#if BAS_NOTIFY_ENABLE == true>
#define BAS_BAT_LEVEL_NOTIFY_ENABLE        /* Enable Notify Property */
<#else>
//#define BAS_BAT_LEVEL_NOTIFY_ENABLE        /* Enable Notify Property */
</#if>

/**
 * @addtogroup BLE_BAS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_BAS_UUID_DEF Battery Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the Battery Service characteristics.
 * @{
 */
#define UUID_BATTERY_SERVICE                (0x180FU)       /**< UUID for Battery Service. */
#define UUID_BATTERY_LEVEL                  (0x2A19U)       /**< UUID for Battery Level characteristic. */
/** @} */

/**
 * @defgroup BLE_BAS_ASSIGN_HANDLE BLE BAS assigned handles
 * @brief  Handles associated with the BLE Battery Service attributes.
 * @{
 */
#define BAS_START_HDL                       (0x00E0U)       /**< Start handle for the Battery Service. */

/* Enumeration of attribute handles for the BLE Battery Service. */
typedef enum BLE_BAS_AttributeHandle_T
{
    BAS_HDL_SVC = BAS_START_HDL,                            /**< Handle for the Battery Service primary service. */
    BAS_HDL_CHAR_BAT_LEVEL,                                 /**< Handle for the Battery Level characteristic. */
    BAS_HDL_CHARVAL_BAT_LEVEL,                              /**< Handle for the Battery Level characteristic value. */
    #ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
    BAS_HDL_CCCD_BAT_LEVEL,                                 /**< Handle for the Battery Level CCCD value. */
    #endif
    BAS_HDL_END                                             /**< Sentinel value indicating the end of the Battery Service handles. */
}BLE_BAS_AttributeHandle_T;

#define BAS_END_HDL                       ((uint16_t)BAS_HDL_END-1U)    /**< End handle for the Battery Service. */
/** @} */

/** @} */ //BLE_BAS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_ANS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Battery Service to the GATT server.
 *
 * This function adds the BLE Battery Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Battery service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Battery service.
 *
 */
uint16_t BLE_BAS_Add(void);

/**
 * @brief Sets the battery level for the BLE Battery Service.
 *
 * @param[in] batteryLevel                   The new battery level to set (0-100%).
 *
 * @retval MBA_RES_SUCCESS                   The battery level was successfully set.
 * @retval MBA_RES_INVALID_PARA              The provided batteryLevel is out of the expected range.
 *
 */
uint16_t BLE_BAS_SetBatteryLevel(uint8_t batteryLevel);

/**
 * @brief Retrieves the current battery level from the BLE Battery Service.
 *
 * @param[in] p_batteryLevel                 Pointer to a variable where the battery level will be stored.
 *
 * @retval MBA_RES_SUCCESS                   The battery level was successfully retrieved.
 *
 */
uint16_t BLE_BAS_GetBatteryLevel(uint8_t *p_batteryLevel);

/** @} */ //BLE_ANS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_BAS_H