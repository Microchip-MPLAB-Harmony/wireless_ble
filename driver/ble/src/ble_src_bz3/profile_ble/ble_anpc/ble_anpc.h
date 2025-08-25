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
  BLE Alert Notification Profile Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_anpc.h

  Summary:
    Interface for the BLE Alert Notification Profile Client used in applications.

  Description:
    This header defines the interface for the BLE Alert Notification Profile Client,
    which enables the application to interact with the Alert Notification Service of
    a remote BLE device.
 *******************************************************************************/
#ifndef BLE_ANPC_H
#define BLE_ANPC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"
#include "ble_gcm/ble_dd.h"
#include "gatt.h"

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
 * @defgroup BLE_ANPC BLE Alert Notification Profile Client
 * 
 * @brief Provides the interface for the BLE Alert Notification Profile Client (ANPC).
 * @note This section provides the API for the BLE ANPC, facilitating communication
 *          with the Alert Notification Service on a BLE server. It includes definitions,
 *          data structures, and function prototypes necessary to use the ANPC.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_ANPC_DEFINES Defines
 * @{
 */

/** 
 * @defgroup BLE_ANPC_ERRCODE    ANPC error codes
 * @brief Defines error code for ANPC.
 * @{
 */
#define BLE_ANPC_ERRCODE_COMMAND_NOT_SUPPORTTED     (0xA0U)     /**< Error code for unsupported command.*/
/** @} */


/** 
 * @defgroup BLE_ANPC_CMD   ANPC command definitions
 * @brief Defines the ANPC commands.
 * @{ 
 */
#define BLE_ANPC_CMD_ENABLE_NEW_ALERT               (0x00U)     /**< Command to enable new alert notifications. */
#define BLE_ANPC_CMD_ENABLE_UNREAD_ALERT_STAT       (0x01U)     /**< Command to enable unread alert status notifications. */
#define BLE_ANPC_CMD_DISABLE_NEW_ALERT              (0x02U)     /**< Command to disable new alert notifications. */
#define BLE_ANPC_CMD_DISABLE_UNREAD_ALERT_STAT      (0x03U)     /**< Command to disable unread alert status notifications. */
#define BLE_ANPC_CMD_NOTI_NEW_ALERT_IMM             (0x04U)     /**< Command to immediately notify of a new alert. */
#define BLE_ANPC_CMD_NOTI_UNREAD_ALERT_STAT_IMM     (0x05U)     /**< Command to immediately notify of an unread alert status. */
#define BLE_ANPC_CMD_END                            (BLE_ANPC_CMD_NOTI_UNREAD_ALERT_STAT_IMM) /**< Marker for the end of command IDs. */
/** @} */


/** 
 * @defgroup BLE_ANPC_DESC_MAX_NUM   ANPC maximum descriptor count
 * @brief Defines the maximum number of descriptors.
 * @{ 
 */
#define BLE_ANPC_DESC_MAX_NUM 						(2U)	    /**< Maximum number of descriptors allowed. */
/** @} */


/** 
 * @defgroup BLE_ANPC_CAT_ID    ANPC category identifiers
 * @brief Defines the category identifiers.
 * @{
 */
#define BLE_ANPC_CAT_ID_SIMPLE_ALERT                (0x00U)     /**< Category ID for simple alerts. */
#define BLE_ANPC_CAT_ID_EMAIL                       (0x01U)     /**< Category ID for email alerts. */
#define BLE_ANPC_CAT_ID_NEWS                        (0x02U)     /**< Category ID for news alerts. */
#define BLE_ANPC_CAT_ID_CALL                        (0x03U)     /**< Category ID for call alerts. */
#define BLE_ANPC_CAT_ID_MISSED_CALL                 (0x04U)     /**< Category ID for missed call alerts. */
#define BLE_ANPC_CAT_ID_SMS_MMS                     (0x05U)     /**< Category ID for SMS/MMS alerts. */
#define BLE_ANPC_CAT_ID_VOICE_MAIL                  (0x06U)     /**< Category ID for voicemail alerts. */
#define BLE_ANPC_CAT_ID_SCHEDULE                    (0x07U)     /**< Category ID for schedule alerts. */
#define BLE_ANPC_CAT_ID_HIGH_PRIOR_ALERT            (0x08U)     /**< Category ID for high-priority alerts. */
#define BLE_ANPC_CAT_ID_INSTANT_MSG                 (0x09U)     /**< Category ID for instant messaging alerts. */
#define BLE_ANPC_CAT_ID_ALL_ALERT                   (0xFFU)     /**< Category ID for all alerts. */
/** @} */


/** 
 * @defgroup BLE_ANPC_SUPP_CAT_ID_BIT_MASK  ANPC supported category ID bit masks
 * @brief Bit masks for supported category IDs.
 * @{
 */
#define BLE_ANPC_SUPP_CAT_SIMPLE_ALERT              (1U << 0U)  /**< Bit mask for simple alert category. */
#define BLE_ANPC_SUPP_CAT_EMAIL                     (1U << 1U)  /**< Bit mask for email alert category. */
#define BLE_ANPC_SUPP_CAT_NEWS                      (1U << 2U)  /**< Bit mask for news alert category. */
#define BLE_ANPC_SUPP_CAT_CALL                      (1U << 3U)  /**< Bit mask for call alert category. */
#define BLE_ANPC_SUPP_CAT_MISSED_CALL               (1U << 4U)  /**< Bit mask for missed call alert category. */
#define BLE_ANPC_SUPP_CAT_SMS_MMS                   (1U << 5U)  /**< Bit mask for SMS/MMS alert category. */
#define BLE_ANPC_SUPP_CAT_VOICE_MAIL                (1U << 6U)  /**< Bit mask for voicemail alert category. */
#define BLE_ANPC_SUPP_CAT_SCHEDULE                  (1U << 7U)  /**< Bit mask for schedule alert category. */
#define BLE_ANPC_SUPP_CAT_HIGH_PRIOR_ALERT          (1U << 8U)  /**< Bit mask for high-priority alert category. */
#define BLE_ANPC_SUPP_CAT_INSTANT_MSG               (1U << 9U)  /**< Bit mask for instant messaging alert category. */
/** @} */


/** 
 * @defgroup BLE_ANPC_UUID    UUIDs of characteristics in Alert Notification profile
 * @brief UUIDs for characteristics used within the Alert Notification Service.
 * @{
 */
#define BLE_ANPC_UUID_ANCP                          (0x2A44U)   /**< UUID for Alert Notification Control Point characteristic. */
#define BLE_ANPC_UUID_UNREAD_ALERT_STAT             (0x2A45U)   /**< UUID for Unread Alert Status characteristic. */
#define BLE_ANPC_UUID_NEW_ALERT                     (0x2A46U)   /**< UUID for New Alert characteristic. */
#define BLE_ANPC_UUID_SUPP_NEW_ALERT_CAT            (0x2A47U)   /**< UUID for Supported New Alert Category characteristic. */
#define BLE_ANPC_UUID_SUPP_UNREAD_ALERT_CAT         (0x2A48U)   /**< UUID for Supported Unread Alert Category characteristic. */
/** @} */
/** @} */ //BLE_ANPC_DEFINES

/** 
 * @addtogroup BLE_ANPC_ENUMS Enumerations
 * @{ 
 */

/** @brief Enumeration for BLE Alert Notification Profile callback events. */
typedef enum BLE_ANPC_EventId_T
{
    BLE_ANPC_EVT_DISC_COMPLETE_IND = 0x00U,                     /**< Event indicating the completion of service discovery. 
                                                                        Details in @ref BLE_ANPC_EvtDiscComplete_T. */
    BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND,                        /**< Event indicating the supported categories for new alerts. 
                                                                        Details in @ref BLE_ANPC_EvtSuppNewAlertCatInd_T. */
    BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND,                /**< Event indicating the supported categories for unread alert status. 
                                                                        Details in @ref BLE_ANPC_EvtSuppUnreadAlertCatInd_T. */
    BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND,                  /**< Event indicating a response to a write request for new alert notifications. 
                                                                        Details in @ref BLE_ANPC_EvtWriteNewAlertRspInd_T. */
    BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND,               /**< Event indicating a response to a write request for unread alert notifications. 
                                                                        Details in @ref BLE_ANPC_EvtWriteUnreadAlertRspInd_T. */
    BLE_ANPC_EVT_WRITE_CP_RSP_IND,                              /**< Event indicating a response to a write request to the Control Point. 
                                                                        Details in @ref BLE_ANPC_EvtWriteCpRspInd_T. */
    BLE_ANPC_EVT_NEW_ALERT_IND,                                 /**< Event indicating the reception of a new alert notification. 
                                                                        Details in @ref BLE_ANPC_EvtNewAlertInd_T. */
    BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND,                         /**< Event indicating the reception of an unread alert status notification. 
                                                                        Details in @ref BLE_ANPC_EvtUnreadAlertStatInd_T. */
    BLE_ANPC_EVT_ERR_UNSPECIFIED_IND,                           /**< Event indicating an unspecified internal error. */
    BLE_ANPC_EVT_ERR_NO_MEM_IND,                                /**< Event indicating an error due to insufficient memory. */
}BLE_ANPC_EventId_T;

/**@} */ //BLE_ANPC_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/** 
 * @addtogroup BLE_ANPC_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_ANPC_EVT_DISC_COMPLETE_IND event. */
typedef struct BLE_ANPC_EvtDiscComplete_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the discovery completion. */
    uint16_t            ansStartHandle;         /**< Start handle of the discovered ANS. */
    uint16_t            ansEndHandle;           /**< End handle of the discovered ANS. */
}BLE_ANPC_EvtDiscComplete_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND event. */
typedef struct BLE_ANPC_EvtSuppNewAlertCatInd_T
{
    uint16_t            connHandle;             /**< Connection handle for which the supported categories are indicated. */
    uint16_t            category;               /**< Bit mask of supported new alert categories. Refer to 
                                                @ref BLE_ANPC_SUPP_CAT_ID_BIT_MASK for possible values. */
}BLE_ANPC_EvtSuppNewAlertCatInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND event. */
typedef struct BLE_ANPC_EvtSuppUnreadAlertCatInd_T
{
    uint16_t            connHandle;             /**< Connection handle for which the supported categories are indicated. */
    uint16_t            category;               /**< Bit mask of supported unread alert status categories. Refer to 
                                                @ref BLE_ANPC_SUPP_CAT_ID_BIT_MASK for possible values. */
}BLE_ANPC_EvtSuppUnreadAlertCatInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteNewAlertRspInd_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the write request. */
    uint16_t            errCode;                /**< Error code indicating the result of the write operation. Refer to 
                                                @ref ATT_ERROR_CODES for possible values. */
}BLE_ANPC_EvtWriteNewAlertRspInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteUnreadAlertRspInd_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the write request. */
    uint16_t            errCode;                /**< Error code indicating the result of the write operation. Refer to 
                                                @ref ATT_ERROR_CODES for possible values. */
}BLE_ANPC_EvtWriteUnreadAlertRspInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_WRITE_CP_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteCpRspInd_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the write request. */
    uint16_t            errCode;                /**< Error code indicating the result of the write operation. Refer to 
                                                @ref BLE_ANPC_ERRCODE for possible values. */
}BLE_ANPC_EvtWriteCpRspInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_NEW_ALERT_IND event. */
typedef struct BLE_ANPC_EvtNewAlertInd_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the event. */
    uint8_t             categoryId;             /**< Category ID of the new alert. Refer to @ref BLE_ANPC_CAT_ID for possible values. */
    uint8_t             numOfNewAlert;          /**< Number of new alerts for the given category. */
    uint16_t            receivedLength;         /**< Length of the received alert text string. */
    uint8_t             *p_receivedValue;       /**< Pointer to the received alert text string data. */
}BLE_ANPC_EvtNewAlertInd_T;


/** @brief Structure for the @ref BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND event. */
typedef struct BLE_ANPC_EvtUnreadAlertStatInd_T
{
    uint16_t            connHandle;             /**< Connection handle associated with the event. */
    uint8_t             categoryId;             /**< Category ID of the unread alert. Refer to @ref BLE_ANPC_CAT_ID for possible values. */
    uint8_t             unreadCnt;              /**< Count of unread alerts for the given category. */
}BLE_ANPC_EvtUnreadAlertStatInd_T;


/** @brief Structure representing the characteristic list. */
typedef struct BLE_ANPC_CharList_T
{
    uint16_t            attrHandle;             /**< Attribute handle of the characteristic. */
    uint8_t             property;               /**< Properties of the characteristic. */
    uint16_t            charHandle;             /**< Handle of the characteristic. */
}BLE_ANPC_CharList_T;


/** @brief Structure representing descriptor information. */
typedef struct BLE_ANPC_DescInfo_T
{
    uint16_t            attrHandle;             /**< Attribute handle of the descriptor. */
    uint16_t            uuid;                   /**< UUID of the descriptor. */
}BLE_ANPC_DescInfo_T;


/** @brief Structure representing the descriptor list. */
typedef struct BLE_ANPC_DescList_T
{
    uint8_t 		    totalNum;  		        /**< Total number of descriptors discovered. */
    BLE_ANPC_DescInfo_T descInfo[BLE_ANPC_DESC_MAX_NUM]; /**< Array of discovered descriptor information.*/
}BLE_ANPC_DescList_T;


/** @brief Union of BLE Alert Notification Client callback event data types. */
typedef union
{
    BLE_ANPC_EvtDiscComplete_T           evtDiscComplete;          /**< Data for the @ref BLE_ANPC_EVT_DISC_COMPLETE_IND event. */
    BLE_ANPC_EvtSuppNewAlertCatInd_T     evtSuppNewAlertCatInd;    /**< Data for the @ref BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND event. */
    BLE_ANPC_EvtSuppUnreadAlertCatInd_T  evtSuppUnreadAlertCatInd; /**< Data for the @ref BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND event. */
    BLE_ANPC_EvtWriteNewAlertRspInd_T    evtWriteNewAlertRspInd;   /**< Data for the @ref BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND event. */
    BLE_ANPC_EvtWriteUnreadAlertRspInd_T evtWriteUnreadAlertRspInd;/**< Data for the @ref BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND event. */
    BLE_ANPC_EvtWriteCpRspInd_T          evtWriteCpRspInd;         /**< Data for the @ref BLE_ANPC_EVT_WRITE_CP_RSP_IND event. */
    BLE_ANPC_EvtNewAlertInd_T            evtNewAlertInd;           /**< Data for the @ref BLE_ANPC_EVT_NEW_ALERT_IND event. */
    BLE_ANPC_EvtUnreadAlertStatInd_T     evtUnreadAlertStatInd;    /**< Data for the @ref BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND event. */
}BLE_ANPC_EventField_T;


/** @brief BLE ANPC callback event structure. */
typedef struct  BLE_ANPC_Event_T
{
    BLE_ANPC_EventId_T                   eventId;                  /**< Identifier for the type of event.  */
    BLE_ANPC_EventField_T                eventField;               /**< Data associated with the event. */
}BLE_ANPC_Event_T;


/**
 * @brief Type definition for the BLE Alert Notification profile Client callback function.
 * 
 * This callback function is used to send BLE Alert Notification profile client events to the application.
 * 
 * @param p_event Pointer to the event structure containing event ID and data.
 */
typedef void(*BLE_ANPC_EventCb_T)(BLE_ANPC_Event_T *p_event);

/**@} */ //BLE_ANPC_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/** 
 * @addtogroup BLE_ANPC_FUNS Functions
 * @{
 */

/**
 * @brief Initializes BLE Alert Notification Client.
 *
 * @retval MBA_RES_SUCCESS          The client was successfully initialized.
 * @retval MBA_RES_FAIL             The client failed to initialize.
 */
uint16_t BLE_ANPC_Init(void);


/**
 * @brief Registers a callback function for the BLE Alert Notification Client events.
 *
 * @param[in] routine               The callback function to handle client events.
 */
void BLE_ANPC_EventRegister(BLE_ANPC_EventCb_T routine);


/**
 * @brief Requests the value of the Supported New Alert Category from the server.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 *
 * @retval MBA_RES_SUCCESS          The read request was successfully issued.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_FAIL             The read request failed to issue.
 */
uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle);


/**
 * @brief Requests the value of the Supported Unread Alert Category from the server.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 *
 * @retval MBA_RES_SUCCESS          The read request was successfully issued.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_FAIL             The read request failed to issue.
 */
uint16_t BLE_ANPC_ReadSuppUnreadAlertCat(uint16_t connHandle);


/**
 * @brief Enables or disables notifications for new alerts.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] enable                Set to true to enable notifications; false to disable.
 *
 * @retval MBA_RES_SUCCESS          The operation was successful.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableNewAlertNtfy(uint16_t connHandle, bool enable);


/**
 * @brief Enables or disables notifications for unread alerts.\n
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] enable                Set to true to enable notifications; false to disable.
 *
 * @retval MBA_RES_SUCCESS          The operation was successful.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable);


/**
 * @brief Sends a command to the Alert Notification Control Point.
 *
 * @param[in] connHandle            The connection handle to identify the BLE connection.
 * @param[in] cmdId                 The command ID.
 * @param[in] catId                 The category ID.
 *
 * @retval MBA_RES_SUCCESS          The command was successfully sent.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_WriteAncp(uint16_t connHandle, uint8_t cmdId, uint8_t catId);


/**
 * @brief Retrieves a list of discovered Alert Notification service characteristics.
 * 
 * This function should be called after the BLE_ANPC_EVT_DISC_COMPLETE_IND event.
 *
 * @param[in]  connHandle           The connection handle to identify the BLE connection.
 * @param[in]  charUuid             The UUID of the characteristic to look for.
 * @param[out] p_charList           On success, filled with the discovered characteristics info.
 *
 * @retval MBA_RES_SUCCESS          The characteristic list was successfully retrieved.
 * @retval MBA_RES_INVALID_PARA     The parameters are invalid (e.g., invalid connection handle or UUID).
 */
uint16_t BLE_ANPC_GetCharList(uint16_t connHandle, uint16_t charUuid, BLE_ANPC_CharList_T *p_charList);


/**
 * @brief Retrieves a list of descriptors within the Alert Notification Service.
 * 
 * @note This function should be called after the BLE_ANPC_EVT_DISC_COMPLETE_IND event.
 *
 * @param[in]  connHandle           The connection handle to identify the BLE connection.
 * @param[out] p_descList           On success, filled with the discovered descriptors info.
 *
 * @retval MBA_RES_SUCCESS          The descriptor list was successfully retrieved.
 * @retval MBA_RES_INVALID_PARA     The connection handle is invalid.
 */
uint16_t BLE_ANPC_GetDescList(uint16_t connHandle, BLE_ANPC_DescList_T *p_descList);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE stack events occur.
 * 
 * @param[in] p_stackEvent          Pointer to the stack event structure.
 *
*/
void BLE_ANPC_BleEventHandler(STACK_Event_T *p_stackEvent);


/**
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *
 * @note This function should be called when BLE_DD events occur.
 * 
 * @param[in] p_event               Pointer to the BLE DD event structure.
 *
*/
void BLE_ANPC_BleDdEventHandler(BLE_DD_Event_T *p_event);

/**@} */ //BLE_ANPC_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_ANPC_H