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
  BLE Human Interface Device Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_hids.h

  Summary:
    Interface definitions for the BLE Human Interface Device Service.

  Description:
    Provides function prototypes and definitions necessary for the integration
    of the BLE Human Interface Device Service into the application layer.
 *******************************************************************************/

#ifndef BLE_HIDS_H
#define BLE_HIDS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "ble_bas/ble_bas.h"

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
 * @addtogroup BLE_HIDS BLE Human Interface Device Service
 * @brief Provides the interface for the BLE Human Interface Device Service.
 * @note This section documents the API for the BLE HIDS. It includes
 *          definitions, data structures, and function prototypes necessary
 *          to enable HIDS in a BLE application.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

<#if HIDS_BOOTMODE_SUPPORT == true>
#define HIDS_BOOT_PROTOCOL_MODE_SUPPORT     /* Boot Mode Support*/    /**< Define to support Boot Protocol mode. */
<#else>
//#define HIDS_BOOT_PROTOCOL_MODE_SUPPORT     /* Boot Mode Support*/
</#if>

<#if HIDS_KB_SUPPORT == true>
#define HIDS_KEYBOARD_SUPPORT               /* Keyboard Support */     /**< Define to support Keyboard. */
<#else>
//#define HIDS_KEYBOARD_SUPPORT               /* Keyboard Support */
</#if>

<#if HIDS_M_SUPPORT == true>
#define HIDS_MOUSE_SUPPORT                  /* Mouse Support */        /**< Define to support Mouse. */
<#else>
//#define HIDS_MOUSE_SUPPORT                  /* Mouse Support */
</#if>

/**
 * @addtogroup BLE_HIDS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_HIDS_UUID_DEF Human Interface Device Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the Human Interface Device Service characteristics.
 * @{
 */
#define UUID_HUMAN_INTERFACE_DEVICE_SERVICE           (0x1812U)          /**< UUID for Human Interface Device Service. */
#define UUID_HID_BOOT_KB_INPUT_REPORT                 (0x2A22U)          /**< UUID for Boot Keyboard Input Report characteristic. */
#define UUID_HID_BOOT_KB_OUTPUT_REPORT                (0x2A32U)          /**< UUID for Boot Keyboard Output Report characteristic. */
#define UUID_HID_BOOT_MOUSE_INPUT_REPORT              (0x2A33U)          /**< UUID for Boot Mouse Input Report characteristic. */
#define UUID_HID_INFORMATION                          (0x2A4AU)          /**< UUID for HID Information characteristic. */
#define UUID_HID_REPORT_MAP                           (0x2A4BU)          /**< UUID for HID Report Map characteristic. */
#define UUID_HID_CONTROL_POINT                        (0x2A4CU)          /**< UUID for HID Control Point characteristic. */
#define UUID_HID_REPORT                               (0x2A4DU)          /**< UUID for HID Report characteristic. */
#define UUID_HID_PROTOCOL_MODE                        (0x2A4EU)          /**< UUID for HID Protocol Mode characteristic. */
/** @} */

/**
 * @defgroup BLE_HIDS_REP_TYPE Report type definitions
 * @brief Report type definitions of BLE Human Interface Device Service.
 * @{
 */
#define HID_REPORT_TYPE_INPUT                         (0x01U)            /**< Type for input reports. */
#define HID_REPORT_TYPE_OUTPUT                        (0x02U)            /**< Type for output reports. */
#define HID_REPORT_TYPE_FEATURE                       (0x03U)            /**< Type for feature reports. */
/** @} */

/**
 * @defgroup BLE_HIDS_REP_ID Report ID definitions
 * @brief Report ID definitions of BLE Human Interface Device Service.
 * @{
 */
#define HID_REPORT_ID_KB                              (0x00U)            /**< Report ID for Keyboard. */
#define HID_REPORT_ID_MB                              (0x02U)            /**< Report ID for Mouse buttons. */
#define HID_REPORT_ID_MM                              (0x03U)            /**< Report ID for Mouse motion. */
/** @} */

/**
 * @defgroup BLE_HIDS_REP_LEN Report length definitions
 * @brief Report length definitions of BLE Human Interface Device Service.
 * @{
 */
#define HID_REPORT_LENGTH_KB_INPUT                    (0x08U)            /**< Report length of Keyboard Input report. */
#define HID_REPORT_LENGTH_KB_OUTPUT                   (0x01U)            /**< Report length of Keyboard Output report (LED). */
#define HID_REPORT_LENGTH_MB_INPUT                    (0x03U)            /**< Report length of Mouse Button Input report. */
#define HID_REPORT_LENGTH_MM_INPUT                    (0x03U)            /**< Report length of Mouse Motion Input report. */
#define HID_REPORT_LENGTH_BOOT_KB_INPUT               (0x08U)            /**< Report length of Boot Keyboard Input report. */
#define HID_REPORT_LENGTH_BOOT_KB_OUTPUT              (0x01U)            /**< Report length of Boot Keyboard Output report. */
#define HID_REPORT_LENGTH_BOOT_M_INPUT                (0x03U)            /**< Report length of Boot Mouse Input report. */
/** @} */

/**
 * @defgroup BLE_HIDS_MODE Protocol mode definitions
 * @brief Protocol mode definitions for the BLE Human Interface Device Service.
 * @{
 */
#define HID_MODE_BOOT_PROTOCOL                        (0x00U)            /**< Boot Protocol Mode. The HID Service enters this mode after receiving this value. */
#define HID_MODE_REPORT_PROTOCOL                      (0x01U)            /**< Report Protocol Mode. This is the default mode for all HID devices. */
/** @} */

/**
 * @defgroup BLE_HIDS_SUP_STATUS Suspend status definitions
 * @brief HID host suspend status definitions of BLE Human Interface Device Service.
 * @{
 */
#define HID_HOST_SUSPEND_ENTER                        (0x00U)            /**< Indicates the HID host is entering suspend state. */
#define HID_HOST_SUSPEND_EXIT                         (0x01U)            /**< Indicates the HID host is exiting suspend state. */
/** @} */

/**
 * @defgroup BLE_HIDS_ASSIGN_HANDLE BLE HIDS assigned handles
 * @brief Handles associated with the BLE Human Interface Device Service attributes.
 * @{
 */
#define HIDS_START_HDL                                (0x0050U)          /**< Start handle for the BLE Human Interface Device Service. */

/* Enumeration of attribute handles for the BLE Human Interface Device Service. */
typedef enum BLE_HIDS_AttributeHandle_T
{
    HIDS_HDL_SVC = HIDS_START_HDL,                                       /**< Handle for the BLE Human Interface Device Service primary service. */
    HIDS_HDL_INCLUDE,                                                    /**< Handle for the included Service of BLE Human Interface Device service. */

#ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
    HIDS_HDL_CHAR_HID_PROTOCOL_MODE,                                     /**< Handle for the Protocol Mode characteristic. */
    HIDS_HDL_CHARVAL_HID_PROTOCOL_MODE,                                  /**< Handle for the Protocol Mode characteristic Value. */
#endif

#ifdef HIDS_KEYBOARD_SUPPORT
    HIDS_HDL_CHAR_KB_INPUT_REPORT,                                       /**< Handle for the Report characteristic (Keyboard Input Report). */
    HIDS_HDL_CHARVAL_KB_INPUT_REPORT,                                    /**< Handle for the Report characteristic calue (Keyboard Input Report). */
    HIDS_HDL_CCCD_KB_INPUT_REPORT,                                       /**< Handle for the Report CCCD value (Keyboard Input Report).*/
    HIDS_HDL_DESC_REPORT_REF_KB_INPUT_REPORT,                            /**< Handle for the Report Reference Descriptor (Keyboard Input Report). */
#endif

#ifdef HIDS_MOUSE_SUPPORT
    HIDS_HDL_CHAR_MB_INPUT_REPORT,                                       /**< Handle for the Report Characteristic (Mouse Button Input Report). */
    HIDS_HDL_CHARVAL_MB_INPUT_REPORT,                                    /**< Handle for the Report Characteristic Value (Mouse Button Input Report). */
    HIDS_HDL_CCCD_MB_INPUT_REPORT,                                       /**< Handle for the CCCD value (Mouse Button Input Report). */
    HIDS_HDL_DESC_REPORT_REF_MB_INPUT_REPORT,                            /**< Handle for the Report Reference Descriptor (Mouse Button Input Report). */
    HIDS_HDL_CHAR_MM_INPUT_REPORT,                                       /**< Handle for the Report Characteristic (Mouse Motion Input Report). */
    HIDS_HDL_CHARVAL_MM_INPUT_REPORT,                                    /**< Handle for the Report Characteristic Value (Mouse Motion Input Report). */
    HIDS_HDL_CCCD_MM_INPUT_REPORT,                                       /**< Handle for the CCCD value (Mouse Motion Input Report). */
    HIDS_HDL_DESC_REPORT_REF_MM_INPUT_REPORT,                            /**< Handle for the Report Reference Descriptor (Mouse Motion Input Report). */
#endif

#ifdef HIDS_KEYBOARD_SUPPORT
    HIDS_HDL_CHAR_KB_OUTPUT_REPORT,                                      /**< Handle for the Report Characteristic (Keyboard Output Report). */
    HIDS_HDL_CHARVAL_KB_OUTPUT_REPORT,                                   /**< Handle for the Report Characteristic Value (Keyboard Output Report). */
    HIDS_HDL_DESC_REPORT_REF_KB_OUTPUT_REPORT,                           /**< Handle for the Report Reference Descriptor (Keyboard Output Report). */
#endif

    HIDS_HDL_CHAR_REPORT_MAP,                                            /**< Handle for the Report Map Characteristic. */
    HIDS_HDL_CHARVAL_REPORT_MAP,                                         /**< Handle for the Report Map Characteristic Value. */
    HIDS_HDL_DESC_EXT_REPORT_REF,                                        /**< Handle for the External Report Reference Descriptor. */

#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    HIDS_HDL_CHAR_BOOT_KB_INPUT_REPORT,                                  /**< Handle for the Boot Keyboard Input Report Characteristic. */
    HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT,                               /**< Handle for the Boot Keyboard Input Report Characteristic Value. */
    HIDS_HDL_CCCD_BOOT_KB_INPUT_REPORT,                                  /**< Handle for the CCCD value (Boot Keyboard Input Report Characteristic). */
    HIDS_HDL_CHAR_BOOT_KB_OUTPUT_REPORT,                                 /**< Handle for the Boot Keyboard Output Report Characteristic. */
    HIDS_HDL_CHARVAL_BOOT_KB_OUTPUT_REPORT,                              /**< Handle for the Boot Keyboard Output Report Characteristic Value. */
#endif

#if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    HIDS_HDL_CHAR_BOOT_M_INPUT_REPORT,                                   /**< Handle for the Boot Mouse Input Report Characteristic. */
    HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT,                                /**< Handle for the Boot Mouse Input Report Characteristic Value. */
    HIDS_HDL_CCCD_BOOT_M_INPUT_REPORT,                                   /**< Handle for the CCCD value (Boot Mouse Input Report Characteristic). */
#endif

    HIDS_HDL_CHAR_HID_INFO,                                              /**< Handle for the HID Information Characteristic. */
    HIDS_HDL_CHARVAL_HID_INFO,                                           /**< Handle for the HID Information Characteristic Value. */
    HIDS_HDL_CHAR_HID_CTRL,                                              /**< Handle for the HID Control Point Characteristic. */
    HIDS_HDL_CHARVAL_HID_CTRL,                                           /**< Handle for the HID Control Point Characteristic Value. */

    HIDS_HDL_END
}BLE_HIDS_AttributeHandle_T;

#define HIDS_END_HDL                                (HIDS_HDL_END-1)     /**< End handle for the Human Interface Device Service. */
/** @} */

/** @} */ //BLE_HIDS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_HIDS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Human Interface Device Service to the GATT server.
 *
 * This function adds the BLE Human Interface Device Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Human Interface Device service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Human Interface Device service.
 *
 */
uint16_t BLE_HIDS_Add(void);

/** @} */ //BLE_HIDS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_HIDS_H