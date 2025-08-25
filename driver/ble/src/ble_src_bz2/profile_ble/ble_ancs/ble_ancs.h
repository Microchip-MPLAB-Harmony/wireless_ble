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
  BLE ANCS Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_ancs.h

  Summary:
    Interface for the BLE Apple Notification Center Service (ANCS) profile.

  Description:
    Provides function prototypes and constants for the ANCS profile, enabling
    the application to interact with the Apple Notification Center Service.
 *******************************************************************************/
#ifndef BLE_ANCS_H
#define BLE_ANCS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
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
 * @defgroup BLE_ANCS BLE Apple Notification Center Service (ANCS) profile
 * 
 * @brief Provides the interface for the BLE Apple Notification Center Service (ANCS) Profile.
 * @note This module contains the definitions and prototypes necessary to
 *          interface with the ANCS provided by BLE devices.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/** 
 * @addtogroup BLE_ANCS_DEFINES Defines
 * @{
 */
/** 
 * @defgroup BLE_ANCS_DECODE_LEN  ANCS attribute lengths
 * @brief Defines maximum lengths of various decoded attributes.
 * @{
 */
#define BLE_ANCS_MAX_PACKET_BUFFER_SIZE     (1024U)                 /**< Maximum buffer size for reassembled packets. */
#define BLE_ANCS_MAX_APPID_LEN              (32U)                   /**< Maximum length of App Identifier. */      
#define BLE_ANCS_MAX_TITLE_LEN              (32U)                   /**< Maximum length of Title. */
#define BLE_ANCS_MAX_SUB_TITLE_LEN          (32U)                   /**< Maximum length of Subtitle. */
#define BLE_ANCS_MAX_MESSAGE_LEN            (512U)                  /**< Maximum length of Message. */
#define BLE_ANCS_MAX_MESSAGE_SIZE_LEN       (5U)                    /**< Maximum length of Message Size. */      
#define BLE_ANCS_MAX_DATE_LEN               (18U)                   /**< Maximum length of Date. */   
#define BLE_ANCS_MAX_POS_ACTION_LABEL_LEN   (8U)                    /**< Maximum length of Positive Action Label. */      
#define BLE_ANCS_MAX_NEG_ACTION_LABEL_LEN   (8U)                    /**< Maximum length of Negative Action Label. */  
#define BLE_ANCS_MAX_APP_NAME_LEN           (16U)                   /**< Maximum length of Application Name. */  
/** @} */


/** 
 * @defgroup ANCS_ERROR_CODES  ANCS error codes
 * @brief Defines error code for ANCS.
 * @{
 */
#define ANCS_ERRCODE_UNKNOWN_COMMAND        (0xA0U)                 /**< Unrecognized command by the NP. */
#define ANCS_ERRCODE_INVALID_COMMAND        (0xA1U)                 /**< Command improperly formatted. */
#define ANCS_ERRCODE_INVALID_PARAMETER      (0xA2U)                 /**< Parameter refers to a non-existent object on NP. */
#define ANCS_ERRCODE_ACTION_FAILED          (0xA3U)                 /**< Action could not be performed. */
/** @} */
       
/** @} */ //BLE_ANCS_DEFINES     


/** 
 * @addtogroup BLE_ANCS_ENUMS Enumerations
 * @{ 
 */

/** @brief Identifiers for possible actions on iOS notifications. */
typedef enum BLE_ANCS_ActionId_T
{
    BLE_ANCS_ACTION_ID_POSITIVE = 0U,                               /**< Identifier for a positive action on a notification. */
    BLE_ANCS_ACTION_ID_NEGATIVE,                                    /**< Identifier for a negative action on a notification. */
    BLE_ANCS_ACTION_ID_MAX                                          /**< Maximum value for action identifiers, used as a boundary. */
} BLE_ANCS_ActionId_T;


/** @brief Identifiers for categories of iOS notifications. */
typedef enum BLE_ANCS_CategoryId_T
{
    BLE_ANCS_CATEGORY_ID_OTHER = 0x00U,                             /**< Category for miscellaneous notifications.  */
    BLE_ANCS_CATEGORY_ID_INCOMING_CALL,                             /**< Category for incoming call notifications. */
    BLE_ANCS_CATEGORY_ID_MISSED_CALL,                               /**< Category for missed call notifications. */
    BLE_ANCS_CATEGORY_ID_VOICE_MAIL,                                /**< Category for voice mail notifications. */
    BLE_ANCS_CATEGORY_ID_SOCIAL,                                    /**< Category for social media notifications. */
    BLE_ANCS_CATEGORY_ID_SCHEDULE,                                  /**< Category for calendar and scheduling notifications. */
    BLE_ANCS_CATEGORY_ID_EMAIL,                                     /**< Category for email notifications. */
    BLE_ANCS_CATEGORY_ID_NEWS,                                      /**< Category for news notifications. */
    BLE_ANCS_CATEGORY_ID_HEALTH_AND_FITNESS,                        /**< Category for health and fitness notifications. */
    BLE_ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE,                      /**< Category for business and finance notifications. */
    BLE_ANCS_CATEGORY_ID_LOCATION,                                  /**< Category for location-based notifications. */
    BLE_ANCS_CATEGORY_ID_ENTERTAINMENT,                             /**< Category for entertainment notifications. */
    BLE_ANCS_CATEGORY_ID_MAX                          	            /**< Maximum value for category identifiers. */
} BLE_ANCS_CategoryId_T;


/** @brief Event identifiers for BLE ANCS events delivered to the application. */
typedef enum BLE_ANCS_EventId_T
{
    BLE_ANCS_EVT_DISC_COMPLETE_IND = 0x00U,                         /**< Event indicating the completion of service discovery. 
                                                                            See @ref BLE_ANCS_EvtDiscComplete_T for event details.*/
    BLE_ANCS_EVT_ERR_ATTR_BUF_IND,                                  /**< Event indicating an attribute buffer overflow error. 
                                                                            See @ref BLE_ANCS_EvtErrAttrBufInd_T for event details.*/
    BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND,                             /**< Event indicating a recomposed buffer overflow error. 
                                                                            See @ref BLE_ANCS_EvtErrRecomposeBufInd_T for event details.*/
    BLE_ANCS_EVT_NTFY_ADDED_IND,                                    /**< Event indicating a new notification was added. 
                                                                            See @ref BLE_ANCS_EvtNtfyInd_T for event details.*/
    BLE_ANCS_EVT_NTFY_MODIFIED_IND,                                 /**< Event indicating an existing notification was modified. 
                                                                            See @ref BLE_ANCS_EvtNtfyInd_T for event details.*/
    BLE_ANCS_EVT_NTFY_REMOVED_IND,                                  /**< Event indicating a notification was removed. 
                                                                            See @ref BLE_ANCS_EvtNtfyInd_T for event details.*/
    BLE_ANCS_EVT_NTFY_ATTR_IND,                                     /**< Event indicating a notification attribute was received. 
                                                                            See @ref BLE_ANCS_EvtNtfyAttrInd_T for event details.*/
    BLE_ANCS_EVT_APP_ATTR_IND,                                      /**< Event indicating an app attribute was received. 
                                                                            See @ref BLE_ANCS_EvtAppAttrInd_T for event details.*/
    BLE_ANCS_EVT_ERR_UNSPECIFIED_IND,                               /**< Event indicating an unspecified internal error. */
    BLE_ANCS_EVT_ERR_NO_MEM_IND,                                    /**< Event indicating a memory allocation error. */
    BLE_ANCS_EVT_ERR_IND,                                           /**< Event indicating an error related to Control Point characteristic 
                                                                            operations. See @ref BLE_ANCS_EvtErrInd_T for event details. */
} BLE_ANCS_EventId_T;


/** @brief Identifiers for attributes of iOS notifications. */
typedef enum BLE_ANCS_NtfyAttrId_T
{
    BLE_ANCS_NTFY_ATTR_ID_APP_IDENTIFIER = 0x00U,                   /**< Attribute identifier for the notification's App Identifier. */
    BLE_ANCS_NTFY_ATTR_ID_TITLE,                                    /**< Attribute identifier for the notification's Title. */
    BLE_ANCS_NTFY_ATTR_ID_SUBTITLE,                                 /**< Attribute identifier for the notification's Subtitle. */
    BLE_ANCS_NTFY_ATTR_ID_MESSAGE,                                  /**< Attribute identifier for the notification's Message. */
    BLE_ANCS_NTFY_ATTR_ID_MESSAGE_SIZE,                             /**< Attribute identifier for the notification's Message Size. */
    BLE_ANCS_NTFY_ATTR_ID_DATE,                                     /**< Attribute identifier for the notification's Date. */
    BLE_ANCS_NTFY_ATTR_ID_POSITIVE_ACTION_LABEL,                    /**< Attribute identifier for the notification's Positive Action. */
    BLE_ANCS_NTFY_ATTR_ID_NEGATIVE_ACTION_LABEL,                    /**< Attribute identifier for the notification's Negative Action. */
    BLE_ANCS_NTFY_ATTR_ID_MAX                                       /**< Maximum value for attribute identifiers. */
} BLE_ANCS_NtfyAttrId_T;

/** @} */ //BLE_ANPC_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/** 
 * @addtogroup BLE_ANCS_STRUCTS Structures
 * @{
 */

/** @brief Structure representing the bitmask for iOS notification attribute flags. */
typedef struct BLE_ANCS_NtfyAttrsMask_T
{
    unsigned int                appId           : 1;                /**< Flag indicating the presence of the App Identifier attribute. */
    unsigned int                title           : 1;                /**< Flag indicating the presence of the Title attribute. */
    unsigned int                subtitle        : 1;                /**< Flag indicating the presence of the Subtitle attribute. */
    unsigned int                msg             : 1;                /**< Flag indicating the presence of the Message attribute. */
    unsigned int                msgSize         : 1;                /**< Flag indicating the presence of the Message Size attribute. */
    unsigned int                date            : 1;                /**< Flag indicating the presence of the Date attribute. */
    unsigned int                positiveAction  : 1;                /**< Flag indicating the presence of a Positive Action attribute. */
    unsigned int                negativeAction  : 1;                /**< Flag indicating the presence of a Negative Action attribute. */
} BLE_ANCS_NtfyAttrsMask_T;


/** @brief Structure representing the bitmask for iOS app attribute flags. */
typedef struct BLE_ANCS_AppAttrsMask_T
{
    unsigned int                displayName     : 1;                /**< Flag indicating the presence of the Display Name attribute. */
    unsigned int                reserved        : 7;                /**< Reserved bits for future use. */
} BLE_ANCS_AppAttrsMask_T;


/** @brief Structure representing the bitmask for iOS notification event flags. */
typedef struct BLE_ANCS_NtfyEvtFlagMask_T
{
    unsigned int                silent          : 1;                /**< Flag indicating the notification should be silent. */
    unsigned int                important       : 1;                /**< Flag indicating the notification is of high priority. */
    unsigned int                preExisting     : 1;                /**< Flag indicating the notification is pre-existing. */
    unsigned int                positiveAction  : 1;                /**< Flag indicating the availability of a positive action. */
    unsigned int                negativeAction  : 1;                /**< Flag indicating the availability of a negative action. */
} BLE_ANCS_NtfyEvtFlagMask_T; 


/** @brief Structure containing decoded information for iOS notification attributes. */
typedef struct BLE_ANCS_DecodeNtfyAttrs_T
{
    BLE_ANCS_NtfyAttrsMask_T    bitmask;                            /**< Bitmask indicating which attributes are present. See @ref BLE_ANCS_NtfyAttrsMask_T. */
    uint32_t                    ntfyId;                             /**< Unique identifier for the notification. */
    uint8_t                     appId[BLE_ANCS_MAX_APPID_LEN];      /**< App Identifier, valid if corresponding flag in bitmask is set. */
    uint8_t                     *p_title;                           /**< Pointer to the Title, valid if corresponding flag in bitmask is set. */
    uint8_t                     *p_subtitle;                        /**< Pointer to the Subtitle, valid if corresponding flag in bitmask is set. */
    uint8_t                     *p_msg;                             /**< Pointer to the Message, valid if corresponding flag in bitmask is set. */
    uint16_t                    msgSize;                            /**< Message Size, valid if corresponding flag in bitmask is set. */
    uint8_t                     date[BLE_ANCS_MAX_DATE_LEN];        /**< Date, valid if corresponding flag in bitmask is set. */
    uint8_t                     positiveAction[BLE_ANCS_MAX_POS_ACTION_LABEL_LEN]; /**< Positive Action label, valid if corresponding flag in bitmask is set. */
    uint8_t                     negativeAction[BLE_ANCS_MAX_NEG_ACTION_LABEL_LEN]; /**< Negative Action label, valid if corresponding flag in bitmask is set. */

} BLE_ANCS_DecodeNtfyAttrs_T;


/** @brief Structure containing information about iOS app attributes. */
typedef struct BLE_ANCS_DecodeAppAttrs_T
{
    uint8_t                     appId[BLE_ANCS_MAX_APPID_LEN];          /**< App identifier. */
    uint8_t                     displayName[BLE_ANCS_MAX_APP_NAME_LEN]; /**< Display name. */
} BLE_ANCS_DecodeAppAttrs_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_NTFY_ADDED_IND, @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND, @ref BLE_ANCS_EVT_NTFY_REMOVED_IND event. */
typedef struct BLE_ANCS_EvtNtfyInd_T
{
    uint16_t                    connHandle;                         /**< Connection handle associated with this connection. */
    uint32_t                    ntfyId;                             /**< Unique identifier for the iOS notification. */
    BLE_ANCS_NtfyEvtFlagMask_T  ntfyEvtFlagMask;                    /**< Flags indicating special conditions of the notification (e.g., Silent, Important). 
                                                                            See @ref BLE_ANCS_NtfyEvtFlagMask_T. */
    BLE_ANCS_CategoryId_T       categoryId;                         /**< Category of the notification (e.g., Email, Location). See @ref BLE_ANCS_CategoryId_T. */
    uint8_t                     categoryCount;                      /**< Number of active notifications in this category. */
} BLE_ANCS_EvtNtfyInd_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_NTFY_ATTR_IND event. */
typedef struct BLE_ANCS_EvtNtfyAttrInd_T
{
    uint16_t                    connHandle;                         /**< Connection handle associated with this connection. */
    BLE_ANCS_DecodeNtfyAttrs_T  *p_data;                            /**< Pointer to the decoded notification attributes. See @ref BLE_ANCS_DecodeNtfyAttrs_T. */
} BLE_ANCS_EvtNtfyAttrInd_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_APP_ATTR_IND event. */
typedef struct BLE_ANCS_EvtAppAttrInd_T
{
    uint16_t                    connHandle;                         /**< Connection handle associated with this connection. */
    BLE_ANCS_DecodeAppAttrs_T   *p_data;                            /**< Pointer to the decoded app attributes. See @ref BLE_ANCS_DecodeAppAttrs_T. */
} BLE_ANCS_EvtAppAttrInd_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_DISC_COMPLETE_IND event. */
typedef struct BLE_ANCS_EvtDiscComplete_T
{
    uint16_t                    connHandle;                         /**< Handle of the BLE connection where discovery is completed. */
}BLE_ANCS_EvtDiscComplete_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_ERR_ATTR_BUF_IND event. */
typedef struct BLE_ANCS_EvtErrAttrBufInd_T
{
    uint16_t                    connHandle;                         /**< Connection handle associated with this connection. */
    BLE_ANCS_NtfyAttrId_T       attrId;                             /**< Identifier of the attribute that caused the error. See @ref BLE_ANCS_NtfyAttrId_T.  */
    uint16_t                    len;                                /**< Length of the attribute data. */
} BLE_ANCS_EvtErrAttrBufInd_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND event. */
typedef struct BLE_ANCS_EvtErrRecomposeBufInd_T
{
    uint16_t                    connHandle;                         /**< Connection handle associated with this connection. */
} BLE_ANCS_EvtErrRecomposeBufInd_T;


/** @brief Structure for the @ref BLE_ANCS_EVT_ERR_IND event. */
typedef struct BLE_ANCS_EvtErrInd_T
{
    uint16_t    				connHandle;        		            /**< Connection handle associated with this connection. */
    uint8_t     				errCode;           		            /**< Error code specifying the reason for the error. Refer to @ref ANCS_ERROR_CODES for details. */
} BLE_ANCS_EvtErrInd_T;


/** @brief Union of BLE ANCS callback event data types. */
typedef union
{
    BLE_ANCS_EvtDiscComplete_T          evtDiscComplete;            /**< Data for the @ref BLE_ANCS_EVT_DISC_COMPLETE_IND event.*/
    BLE_ANCS_EvtErrAttrBufInd_T         evtErrAttrBufInd;           /**< Data for the @ref BLE_ANCS_EVT_ERR_ATTR_BUF_IND event. */
    BLE_ANCS_EvtErrRecomposeBufInd_T    evtErrRecomposeBufInd;      /**< Data for the @ref BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND event. */
    BLE_ANCS_EvtNtfyInd_T               evtNtfyInd;                 /**< Data for the @ref BLE_ANCS_EVT_NTFY_ADDED_IND, @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND, 
                                                                        and @ref BLE_ANCS_EVT_NTFY_REMOVED_IND events. */
    BLE_ANCS_EvtNtfyAttrInd_T           evtNtfyAttrInd;             /**< Data for the @ref BLE_ANCS_EVT_NTFY_ATTR_IND event. */
    BLE_ANCS_EvtAppAttrInd_T            evtAppAttrInd;              /**< Data for the @ref BLE_ANCS_EVT_APP_ATTR_IND event. */
    BLE_ANCS_EvtErrInd_T                evtErrInd;               	/**< Data for the @ref BLE_ANCS_EVT_ERR_IND event. */
} BLE_ANCS_EventField_T;

/** @brief BLE ANCS callback event structure. */
typedef struct
{
    BLE_ANCS_EventId_T          eventId;                            /**< Identifier for the type of event. See @ref BLE_ANCS_EventId_T. */
    BLE_ANCS_EventField_T       eventField;                         /**< Data associated with the event. See @ref BLE_ANCS_EventField_T. */
} BLE_ANCS_Event_T;


/**
 * @brief BLE ANCS callback function type. 
 * Applications implement this callback to receive BLE ANCS events from the stack.
 * 
 * @param p_event Pointer to the event structure containing event ID and data.
*/
typedef void(*BLE_ANCS_EventCb_T)(BLE_ANCS_Event_T *p_event);

/** @} */ //BLE_ANCS_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/** 
 * @addtogroup BLE_ANCS_FUNS Functions
 * @{
 */ 

/**
 * @brief Initializes the Apple Notification Center Service (ANCS) profile.
 *
 * @retval MBA_RES_SUCCESS                  BLE ANCS profile was initialized successfully.
 * @retval MBA_RES_FAIL                     Fail to initialize the BLE ANCS profile.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 *
 */
uint16_t BLE_ANCS_Init(void);


/**
 * @brief Registers a callback function for BLE ANCS events.
 *
 * @param[in] bleAncsHandler                The function to be called when an ANCS event occurs.
 *
 */
void BLE_ANCS_EventRegister(BLE_ANCS_EventCb_T bleAncsHandler);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent                  Pointer to the BLE stack event data. See @ref STACK_Event_T.
 *
 */
void BLE_ANCS_BleEventHandler(STACK_Event_T *p_stackEvent);


/**
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *
 * @note This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event                       Pointer to the BLE_DD event data. See @ref BLE_DD_Event_T.
 *
 */
void BLE_ANCS_BleDdEventHandler(BLE_DD_Event_T *p_event);


/**
 * @brief Requests attributes for a specific iOS notification.
 * This function triggers the ANCS client role to retrieve notification attributes from an iOS device.
 *
 * @param[in] connHandle                    The connection handle associated with the iOS device.
 * @param[in] ntfyId                        The unique identifier for the iOS notification. The ntfyId could be retrieved 
 *                                          when receiving @ref BLE_ANCS_EVT_NTFY_ADDED_IND or @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND 
 *                                          or @ref BLE_ANCS_EVT_NTFY_REMOVED_IND events.
 * @param[in] bitmask                       A bitmask specifying which notification attributes to retrieve. Refer to @ref BLE_ANCS_NtfyAttrsMask_T
 *                                          for possible values.
 *
 * @retval MBA_RES_SUCCESS                  Get notification attributes was issued successfully.
 * @retval MBA_RES_FAIL                     The operation is not permitted.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA             Invalid parameters were provided. Possible reasons include:\n
 *                                          - Invalid connection handle.\n
 *                                          - Invalid write parameters. See @ref GATTC_WriteParams_T for valid values.\n
 * @retval MBA_RES_NO_RESOURCE              Insufficient resources to perform the operation.
 * @retval MBA_RES_BUSY                     The GATT Client is currently busy with another operation.
 */
uint16_t BLE_ANCS_GetNtfyAttr(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_NtfyAttrsMask_T bitmask);


/**
 * @brief Retrieves attributes for a specific app on an iOS device.
 *
 * @param[in] connHandle                    The handle for the current connection with the iOS device.
 * @param[in] p_appId                       Pointer to a null-terminated string representing the app's identifier. App Identifier 
 *                                          can be retrieved when receiving @ref BLE_ANCS_EVT_NTFY_ATTR_IND event.
 * @param[in] bitmask                       Bitmask specifying which app attributes to retrieve. Refer to @ref BLE_ANCS_AppAttrsMask_T 
 *                                          for possible values.
 *
 * @retval MBA_RES_SUCCESS                  Command successfully sent to retrieve app attributes.
 * @retval MBA_RES_FAIL                     The operation is not permitted.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA             Invalid parameters were provided. Possible reasons include:\n
 *                                          - Invalid connection handle.\n
 *                                          - Invalid write parameters. See @ref GATTC_WriteParams_T for valid values.\n
 * @retval MBA_RES_NO_RESOURCE              Insufficient resources to perform the operation.
 * @retval MBA_RES_BUSY                     The GATT Client is currently busy with another operation.
 */
uint16_t BLE_ANCS_GetAppAttr(uint16_t connHandle, uint8_t *p_appId, BLE_ANCS_AppAttrsMask_T bitmask);


/**
 * @brief Performs an action on a specific iOS notification.
 * 
 * This function sends a command to the iOS device to perform a specified action on a notification identified 
 * by its unique identifier (UID). The action to be performed is determined by the action ID provided.
 *
 * @param[in] connHandle                    The handle for the current connection with the iOS device.
 * @param[in] ntfyId                        The 32-bit UID of the iOS notification on which to perform the action.
 *                                          The ntfyId can be retrieved when receiving @ref BLE_ANCS_EVT_NTFY_ADDED_IND or 
 *                                          @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND or @ref BLE_ANCS_EVT_NTFY_REMOVED_IND events.
 * @param[in] actId                         The action to perform on the notification. Refer to @ref BLE_ANCS_ActionId_T for possible values.
 *
 * @retval MBA_RES_SUCCESS                  Command successfully sent to perform the action on the notification.
 * @retval MBA_RES_FAIL                     The operation is not permitted.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA             Invalid parameters were provided. Possible reasons include:\n
 *                                          - Invalid connection handle.\n
 *                                          - Invalid write parameters. See @ref GATTC_WriteParams_T for valid values.\n
 * @retval MBA_RES_NO_RESOURCE              No available resource to send write operation.
 * @retval MBA_RES_BUSY                     The GATT Client is currently busy with another operation.
 */
uint16_t BLE_ANCS_PerformNtfyAction(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_ActionId_T actId);
/** @} */ //BLE_ANCS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_ANCS_H