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
  BLE Device Information Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_dis.h

  Summary:
    Interface for the BLE Device Information Service, providing access to device-related data.

  Description:
    This header defines the interface for the BLE Device Information Service, which allows
    an application to expose manufacturer and device information over BLE.
 *******************************************************************************/

#ifndef BLE_DIS_H
#define BLE_DIS_H

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
 * @addtogroup BLE_DIS BLE Device Information Service
 * @brief Provides access to the BLE Device Information Service.
 * @note This section defines the API for the BLE Device Information Service, which
 *          enables the retrieval of device information by a peer device.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
// Enable flags for Device Information Service (DIS) characteristics
#define DIS_MANU_NAME_ENABLE                                        // Enable Manufacturer Name String characteristic
#define DIS_MODEL_NUM_ENABLE                                        // Enable Model Number String characteristic
#define DIS_SERIAL_NUM_ENABLE                                       // Enable Serial Number String characteristic
#define DIS_HW_REV_ENABLE                                           // Enable Hardware Revision String characteristic
#define DIS_FW_REV_ENABLE                                           // Enable Firmware Revision String characteristic
#define DIS_SW_REV_ENABLE                                           // Enable Software Revision String characteristic
#define DIS_SYSTEM_ID_ENABLE                                        // Enable System ID characteristic
#define DIS_IEEE_ENABLE                                             // Enable IEEE 11073-20601 Regulatory Certification Data List characteristic
#define DIS_PNP_ID_ENABLE                                           // Enable PnP ID characteristic

// Predefined values for Device Information Service (DIS) characteristics
#define DIS_MANU_NAME                           "Microchip"         // Manufacturer Name
#define DIS_MODEL_NUM                           "PIC32CX"           // Model Number
#define DIS_SERIAL_NUM                          "0001"              // Serial Number
#define DIS_HW_REVISION                         "0002"              // Hardware Revision
#define DIS_FW_REVISION                         "0003"              // Firmware Revision
#define DIS_SW_REVISION                         "0004"              // Software Revision
#define DIS_SYSTEM_ID                           0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00          // System ID value
#define DIS_IEEE_11073_20601                    0x00, 0x00, 0x00, 0x05, 0x06, 0x40, 0x49, 0x00, 0x00    // IEEE 11073-20601 Regulatory Certification Data List value
#define DIS_PNP_ID                              0x01, 0x00, 0xcd, 0x00, 0x01, 0x00, 0x02                // PnP ID value

/**
 * @addtogroup BLE_DIS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_DIS_UUID_DEF Device Information Service UUID definitions
 * @brief Bluetooth SIG-defined UUIDs for the Device Information Service characteristics.
 * @{
 */
#define UUID_DEVICE_INFO_SERVICE            (0x180AU)       /**< UUID for Device Information Service. */
/** @} */

/**
 * @defgroup BLE_DIS_ASSIGN_HANDLE BLE DIS assigned handles
 * @brief Handles associated with the BLE Device Information Service attributes.
 * @{ */
#define DIS_START_HDL                       (0x0080U)       /**< Start handle for the BLE Device Information Service. */

/* Enumeration of attribute handles for the BLE Device Information Service. */
typedef enum BLE_DIS_AttributeHandle_T
{
    DIS_HDL_SVC = DIS_START_HDL,                            /**< Handle for the Device Information Service primary service. */
#ifdef DIS_MANU_NAME_ENABLE
    DIS_HDL_CHAR_MANU_NAME,                                 /**< Handle for the Manufacture Name String characteristic. */
    DIS_HDL_CHARVAL_MANU_NAME,                              /**< Handle for the Manufacture Name String characteristic value. */
#endif
#ifdef DIS_MODEL_NUM_ENABLE
    DIS_HDL_CHAR_MODEL_NUM,                                 /**< Handle for the Model Number String characteristic. */
    DIS_HDL_CHARVAL_MODEL_NUM,                              /**< Handle for the Model Number String characteristic value. */
#endif
#ifdef DIS_SERIAL_NUM_ENABLE
    DIS_HDL_CHAR_SERIAL_NUM,                                /**< Handle for the Serial Number String characteristic. */
    DIS_HDL_CHARVAL_SERIAL_NUM,                             /**< Handle for the Serial Number String characteristic value. */
#endif
#ifdef DIS_HW_REV_ENABLE
    DIS_HDL_CHAR_HW_REV,                                    /**< Handle for the Hardware Revision String characteristic. */
    DIS_HDL_CHARVAL_HW_REV,                                 /**< Handle for the Hardware Revision String characteristic value. */
#endif
#ifdef DIS_FW_REV_ENABLE
    DIS_HDL_CHAR_FW_REV,                                    /**< Handle for the Firmware Revision String characteristic. */
    DIS_HDL_CHARVAL_FW_REV,                                 /**< Handle for the Firmware Revision String characteristic value. */
#endif
#ifdef DIS_SW_REV_ENABLE
    DIS_HDL_CHAR_SW_REV,                                    /**< Handle for the Software Revision String characteristic. */
    DIS_HDL_CHARVAL_SW_REV,                                 /**< Handle for the Software Revision String characteristic value. */
#endif
#ifdef DIS_SYSTEM_ID_ENABLE
    DIS_HDL_CHAR_SYSTEM_ID,                                 /**< Handle for the System ID characteristic. */
    DIS_HDL_CHARVAL_SYSTEM_ID,                              /**< Handle for the System ID characteristic value. */
#endif
#ifdef DIS_IEEE_ENABLE
    DIS_HDL_CHAR_IEEE_RCDL,                                 /**< Handle for the IEEE 11073-20601 Regulatory Certification Data List characteristic. */
    DIS_HDL_CHARVAL_IEEE_RCDL,                              /**< Handle for the IEEE 11073-20601 Regulatory Certification Data List characteristic value. */
#endif
#ifdef DIS_PNP_ID_ENABLE
    DIS_HDL_CHAR_PNP_ID,                                    /**< Handle for the PnP ID characteristic. */
    DIS_HDL_CHARVAL_PNP_ID,                                 /**< Handle for the PnP ID characteristic value. */
#endif
    DIS_HDL_END
}BLE_DIS_AttributeHandle_T;

#define DIS_END_HDL                         (DIS_HDL_END-1) /**< End handle for the Device Information Service. */
/** @} */

/** @} */ //BLE_DIS_DEFINES
// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_DIS_FUNS Functions
 * @{
 */

/**
 * @brief Adds the BLE Device Information Service to the GATT server.
 *
 * This function adds the BLE Device Information Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Device Information service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Device Information service.
 *
 */
uint16_t BLE_DIS_Add(void);

/** @} */ //BLE_DIS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif //BLE_DIS_H