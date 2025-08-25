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
  BLE Alert Notification Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_ans.h

  Summary:
    Interface for the BLE Alert Notification Service, providing function prototypes and data structures.

  Description:
    This header defines the interface for the BLE Alert Notification Service within the application. It 
    includes function prototypes and data structures required to manage the ANS, facilitating communication 
    between BLE devices for alert notifications.
 *******************************************************************************/

#ifndef BLE_ANS_H
#define BLE_ANS_H

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
 * @addtogroup BLE_ANS BLE Alert Notification Service
 * @brief Provides the interface for the Alert Notification Service over BLE.
 * @note This section documents the API for the BLE Alert Notification Service. It includes definitions, 
 *      function prototypes, and data structures necessary for handling alert notifications between BLE devices.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_ANS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_ANS_ASSIGN_HANDLE BLE ANS assigned handles
 * @brief Handles for attributes within the BLE Alert Notification Service (ANS).
 * @{
 */
#define BLE_ANS_START_HDL                  (0x0150U)      /**< Start handle for the Alert Notification Service. */

/** 
 * Enumeration of attribute handles for the BLE Alert Notification Service.
 */
typedef enum BLE_ANS_AttributeHandle_T
{
    ANS_HDL_SVC = BLE_ANS_START_HDL,                      /**< Handle for the Alert Notificationprimary service. */

    ANS_HDL_CHAR_SUPP_NEW_ALERT_CAT,                      /**< Handle for the Supported New Alert Category characteristic.  */
    ANS_HDL_CHARVAL_SUPP_NEW_ALERT_CAT,                   /**< Handle for the Supported New Alert Category value. */

    ANS_HDL_CHAR_NEW_ALERT,                               /**< Handle for the New Alert characteristic. */
    ANS_HDL_CHARVAL_NEW_ALERT,                            /**< handle for the New Alert value. */
    ANS_HDL_CHARVAL_NEW_ALERT_CCC,                        /**< handle for the New Alert CCCD value. */

    ANS_HDL_CHAR_SUPP_UNREAD_ALERT_CAT,                   /**< Handle for the Supported Unread Alert Category characteristic. */
    ANS_HDL_CHARVAL_SUPP_UNREAD_ALERT_CAT,                /**< Handle for the Supported Unread Alert Category characteristic value. */

    ANS_HDL_CHAR_UNREAD_ALERT_STAT,                       /**< Handle for the Unread Alert Status characteristic. */
    ANS_HDL_CHARVAL_UNREAD_ALERT_STAT,                    /**< Handle for the Unread Alert Status characteristic value. */
    ANS_HDL_CHARVAL_UNREAD_ALERT_STAT_CCC,                /**< Handle for the Unread Alert Status CCCD value. */

    ANS_HDL_CHAR_ANCP,                                    /**< Handle for the Alert Notification Control Point characteristic. */
    ANS_HDL_CHARVAL_ANCP                                  /**< Handle for the Alert Notification Control Point characteristic value. */

}BLE_ANS_AttributeHandle_T;

#define BLE_ANS_END_HDL   ANS_HDL_CHARVAL_ANCP            /**< End handle for the Alert Notification Service. */
/** @} */

/** @} */ //BLE_ANS_DEFINES
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
 * @brief Adds the BLE Alert Notificaiton Service to the GATT server.
 *
 * This function adds the BLE Alert Notificaiton Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 *
 * @retval MBA_RES_SUCCESS                    The BLE Alert Notificaiton service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Alert Notificaiton service.
 *
 */
uint16_t BLE_ANS_Add(void);

/** @} */ //BLE_ANS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_ANS_H
