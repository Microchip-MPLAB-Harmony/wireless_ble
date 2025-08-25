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
  BLE Alert Notification Profile Server Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_anp.h

  Summary:
    Interface for the BLE Alert Notification Profile Server functionality.

  Description:
    This header file exposes the API and definitions for the BLE Alert Notification
    Profile Server, enabling the management and provision of alert notification
    services over BLE.
 *******************************************************************************/
#ifndef BLE_ANPS_H
#define BLE_ANPS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/** 
 * @addtogroup BLE_PROFILE BLE Profile
 * @{
 */

/** 
 * @addtogroup BLE_ANP BLE Alert Notification Profile
 * @{
 */

/**
 * @defgroup BLE_ANPS BLE Alert Notification Profile Server
 * 
 * @brief Provides the interface for the Alert Notification Profile Server.
 * @note This module defines the API for the BLE Alert Notification Profile Server.
 *          It enables the application to handle incoming alerts and notifications
 *          via BLE, ensuring that the device can act as an Alert Notification Server.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_ANPS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_ANPS_CMD   ANPS command definitions
 * @brief TDefines command identifiers for controlling alert notifications.
 * @{
 */
#define BLE_ANPS_CMD_ENABLE_NEW_ALERT               (0x00U)                 /**< Command to enable new alert notifications. */
#define BLE_ANPS_CMD_ENABLE_UNREAD_ALERT_STAT       (0x01U)                 /**< Command to enable unread alert status notifications. */
#define BLE_ANPS_CMD_DISABLE_NEW_ALERT              (0x02U)                 /**< Command to disable new alert notifications. */
#define BLE_ANPS_CMD_DISABLE_UNREAD_ALERT_STAT      (0x03U)                 /**< Command to disable unread alert status notifications. */
#define BLE_ANPS_CMD_NOTI_NEW_ALERT_IMM             (0x04U)                 /**< Command to notify of a new alert immediately. */
#define BLE_ANPS_CMD_NOTI_UNREAD_ALERT_STAT_IMM     (0x05U)                 /**< Command to notify of an unread alert status immediately. */
#define BLE_ANPS_CMD_END                            (BLE_ANPS_CMD_NOTI_UNREAD_ALERT_STAT_IMM) /**< Marker for the last command ID. */
/** @} */


/**
 * @defgroup BLE_ANPS_CAT_ID    ANPS category identifiers
 * @brief The definition of category id. 
 * @{
 */
#define BLE_ANPS_CAT_ID_SIMPLE_ALERT                (0x00U)                 /**< Category ID for simple alerts. */
#define BLE_ANPS_CAT_ID_EMAIL                       (0x01U)                 /**< Category ID for email alerts. */
#define BLE_ANPS_CAT_ID_NEWS                        (0x02U)                 /**< Category ID for news alerts. */
#define BLE_ANPS_CAT_ID_CALL                        (0x03U)                 /**< Category ID for call alerts. */
#define BLE_ANPS_CAT_ID_MISSED_CALL                 (0x04U)                 /**< Category ID for missed call alerts. */
#define BLE_ANPS_CAT_ID_SMS_MMS                     (0x05U)                 /**< Category ID for SMS/MMS alerts. */
#define BLE_ANPS_CAT_ID_VOICE_MAIL                  (0x06U)                 /**< Category ID for voice mail alerts. */
#define BLE_ANPS_CAT_ID_SCHEDULE                    (0x07U)                 /**< Category ID for schedule alerts. */
#define BLE_ANPS_CAT_ID_HIGH_PRIOR_ALERT            (0x08U)                 /**< Category ID for high priority alerts. */
#define BLE_ANPS_CAT_ID_INSTANT_MSG                 (0x09U)                 /**< Category ID for instant messaging alerts. */
#define BLE_ANPS_CAT_ID_ALL_ALERT                   (0xFFU)                 /**< Special category ID representing all alerts. */
/** @} */


/**
 * @defgroup BLE_ANPS_SUPP_CAT_ID_BIT_MASK      ANPS supported category ID bit masks
 * @brief The definition of category id bit mask.
 * @{
 */
#define BLE_ANPS_SUPP_CAT_SIMPLE_ALERT              (1U << 0U)              /**< Bit mask for simple alerts. */
#define BLE_ANPS_SUPP_CAT_EMAIL                     (1U << 1U)              /**< Bit mask for email alerts. */
#define BLE_ANPS_SUPP_CAT_NEWS                      (1U << 2U)              /**< Bit mask for news alerts. */
#define BLE_ANPS_SUPP_CAT_CALL                      (1U << 3U)              /**< Bit mask for call alerts. */
#define BLE_ANPS_SUPP_CAT_MISSED_CALL               (1U << 4U)              /**< Bit mask for missed call alerts. */
#define BLE_ANPS_SUPP_CAT_SMS_MMS                   (1U << 5U)              /**< Bit mask for SMS/MMS alerts. */
#define BLE_ANPS_SUPP_CAT_VOICE_MAIL                (1U << 6U)              /**< Bit mask for voice mail alerts. */
#define BLE_ANPS_SUPP_CAT_SCHEDULE                  (1U << 7U)              /**< Bit mask for schedule alerts. */
#define BLE_ANPS_SUPP_CAT_HIGH_PRIOR_ALERT          (1U << 8U)              /**< Bit mask for high priority alerts. */
#define BLE_ANPS_SUPP_CAT_INSTANT_MSG               (1U << 9U)              /**< Bit mask for instant messaging alerts. */
#define BLE_ANPS_SUPP_CAT_RESERVED                  (0xFC00U)               /**< Reserved bit mask for future use. */
/** @} */

/** @} */ //BLE_ANPS_DEFINES

/**@addtogroup BLE_ANPS_ENUMS Enumerations
 * @{ */

/** @brief Enumeration of BLE Alert Notification Profile Server (ANPS) callback event identifiers. */
typedef enum BLE_ANPS_EventId_T
{
    BLE_ANPS_EVT_ANCP_WRITE_IND = 0x00U,                                    /**< Event indicating the Alert Notification Control Point characteristic 
                                                                                  was written by the client. See @ref BLE_ANPS_EvtAncpWriteInd_T for event details. */
    BLE_ANPS_EVT_ERR_UNSPECIFIED_IND,                                       /**< Unspecified internal error occurred within the profile. */
    BLE_ANPS_EVT_ERR_NO_MEM_IND                                             /**< Internal error due to insufficient heap memory. */
}BLE_ANPS_EventId_T;
/** @} */ //BLE_ANPS_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types 
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BLE_ANPS_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_ANPS_EVT_ANCP_WRITE_IND event. */
typedef struct BLE_ANPS_EvtAncpWriteInd_T
{
    uint16_t                    connHandle;         /**< Connection handle associated with this event. */
    uint8_t                     cmdId;              /**< Command ID indicating the specific action to be taken. 
                                                          Refer to @ref BLE_ANPS_CMD for possible values. */
    uint8_t                     catId;              /**< Category ID of the notification to which the action applies. 
                                                          Refer to @ref BLE_ANPS_CAT_ID for possible values. */
}BLE_ANPS_EvtAncpWriteInd_T;


/** @brief Union containing data types for all possible events in the BLE Alert Notification profile. */
typedef union
{
    BLE_ANPS_EvtAncpWriteInd_T  evtAncpWriteInd;    /**< Data for the @ref BLE_ANPS_EVT_ANCP_WRITE_IND event.*/
}BLE_ANPS_EventField_T;


/** @brief BLE ANPS callback event structure. */
typedef struct  BLE_ANPS_Event_T
{
    BLE_ANPS_EventId_T          eventId;            /**< Identifier for the type of event. */
    BLE_ANPS_EventField_T       eventField;         /**< Data associated with the event. */
}BLE_ANPS_Event_T;


/**
 * @brief Type definition for the callback function used by the BLE Alert Notification profile server.
 * 
 * This callback function is used to send events from the BLE Alert Notification profile server to the application.
 * 
 * @param p_event Pointer to the event structure containing details about the event.
 */
typedef void(*BLE_ANPS_EventCb_T)(BLE_ANPS_Event_T *p_event);

/**@} */ //BLE_ANPS_STRUCTS

// ***************************************************************************** 
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_ANP_FUNS Functions
 * @{
 */


/**
 * @brief Initializes the BLE Alert Notification Profile.
 *
 * @retval MBA_RES_SUCCESS        Successfully initialized the BLE Alert Notification Profile.
 *
 */
uint16_t BLE_ANPS_Init(void);


/**
 * @brief Registers a callback function for the BLE Alert Notification profile events.
 *
 * @param[in] bleAnpRoutine        The callback function to handle BLE Alert Notification profile events.
 *
 */
void BLE_ANPS_EventRegister(BLE_ANPS_EventCb_T bleAnpRoutine);


/**
 * @brief Sets the supported categories for new alerts.
 *
 * @param[in] catMask            Category mask for new alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 *
 * @retval MBA_RES_SUCCESS        Successfully set the supported categories for new alerts.
 * @retval MBA_RES_FAIL           Cannot change supported categories while in a connection.
 * @retval MBA_RES_INVALID_PARA   The parameter is invalid or does not meet the specification.
 */
uint16_t BLE_ANPS_SetSuppNewCat(uint16_t catMask);


/**
 * @brief Sets the supported categories for unread alerts.
 *
 * @param[in] catMask             Category mask for unread alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 *
 * @retval MBA_RES_SUCCESS        Category mask for unread alerts. Refer to @ref BLE_ANPS_SUPP_CAT_ID_BIT_MASK for possible values.
 * @retval MBA_RES_FAIL           Cannot change supported categories while in a connection.
 * @retval MBA_RES_INVALID_PARA   The parameter is invalid or does not meet the specification.
 */
uint16_t BLE_ANPS_SetSuppUnreadCat(uint16_t catMask);


/**
 * @brief Sends a new alert notification to a connected peer device.
 *
 * @param[in] connHandle           The connection handle.
 * @param[in] catId                The category ID of the alert. Refer to @ref BLE_ANPS_CAT_ID for possible values.
 * @param[in] numAlert             The number of new alerts.
 * @param[in] txtStrLen            The length of the text string information. Must be less than or equal to (MTU - 5).
 * @param[in] p_txtStr             Pointer to the text string information.
 *
 * @retval MBA_RES_SUCCESS         Successfully sent the new alert notification.
 * @retval MBA_RES_OOM             Internal memory allocation failure.
 * @retval MBA_RES_NO_RESOURCE     No available buffer to transmit the new alert notification.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters.
 */
uint16_t BLE_ANPS_SendNewAlert(uint16_t connHandle, uint8_t catId, uint8_t numAlert, uint16_t txtStrLen, const uint8_t *p_txtStr);


/**
 * @brief Sends an unread alert status notification to a connected peer device.
 *
 * @param[in] connHandle           The connection handle.
 * @param[in] catId                The category ID of the alert. Refer to @ref BLE_ANPS_CAT_ID for possible values.
 * @param[in] unreadCnt            The count of unread alerts.
 *
 * @retval MBA_RES_SUCCESS         Successfully sent the unread alert status notification.
 * @retval MBA_RES_OOM             Internal memory allocation failure.
 * @retval MBA_RES_NO_RESOURCE     No available buffer to transmit the unread alert status notification.
 * @retval MBA_RES_INVALID_PARA    Invalid parameters.
 */
uint16_t BLE_ANPS_SendUnreadAlertStat(uint16_t connHandle, uint8_t catId, uint8_t unreadCnt);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent        Pointer to the BLE stack events data.
 *
 */
void BLE_ANPS_BleEventHandler(STACK_Event_T *p_stackEvent);

/** @} */ //BLE_ANP_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_ANPS_H