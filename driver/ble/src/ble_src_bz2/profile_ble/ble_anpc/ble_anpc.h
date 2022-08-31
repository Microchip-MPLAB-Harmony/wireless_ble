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
    This file contains the BLE Alert Notification Profile Client functions for application user.

  Description:
    This file contains the BLE Alert Notification Profile Client functions for application user.
 *******************************************************************************/


/**
 * @addtogroup BLE_ANPC
 * @{
 * @brief Header file for the BLE Alert Notification profile Client library.
 * @note Definitions and prototypes for the BLE Alert Notification profile client stack layer application programming interface.
 */
#ifndef BLE_ANPC_H
#define BLE_ANPC_H

// *****************************************************************************
// *****************************************************************************
// Section: Macros for Included Files
// *****************************************************************************
// *****************************************************************************

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

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_ANPC_DEFINES Defines
 * @{ */

/**@defgroup BLE_ANPC_ERRCODE    Error Code definition 
 * @brief The definition of command id.
 * @{ */
#define BLE_ANPC_ERRCODE_COMMAND_NOT_SUPPORTTED     (0xA0) /**< Command not supportted.*/
/** @} */

/**@defgroup BLE_ANPC_CMD   Command definition
 * @brief The definition of command id.
 * @{ */
#define BLE_ANPC_CMD_ENABLE_NEW_ALERT               (0x00) /**< Enable new alert notification.*/ 
#define BLE_ANPC_CMD_ENABLE_UNREAD_ALERT_STAT       (0x01) /**< Enable unread alert status notification.*/
#define BLE_ANPC_CMD_DISABLE_NEW_ALERT              (0x02) /**< Disable new alert notification.*/
#define BLE_ANPC_CMD_DISABLE_UNREAD_ALERT_STAT      (0x03) /**< Disable unread alert status notification.*/
#define BLE_ANPC_CMD_NOTI_NEW_ALERT_IMM             (0x04) /**< Notify new alert immediately.*/
#define BLE_ANPC_CMD_NOTI_UNREAD_ALERT_STAT_IMM     (0x05) /**< Notify unread alert status immediately.*/
#define BLE_ANPC_CMD_END                            (BLE_ANPC_CMD_NOTI_UNREAD_ALERT_STAT_IMM) /**< End of command ID.*/
/** @} */

/**@defgroup BLE_ANPC_CAT_ID    Category Id
 * @brief The definition of category id. 
 * @{ */
#define BLE_ANPC_CAT_ID_SIMPLE_ALERT                (0x00) /**< Simple Alert. The title of the alert.*/  
#define BLE_ANPC_CAT_ID_EMAIL                       (0x01) /**< Email. Sender name.*/
#define BLE_ANPC_CAT_ID_NEWS                        (0x02) /**< News. Title of the news feed.*/
#define BLE_ANPC_CAT_ID_CALL                        (0x03) /**< Call. Caller name or caller ID.*/
#define BLE_ANPC_CAT_ID_MISSED_CALL                 (0x04) /**< Missed Call. Caller name or caller ID*/
#define BLE_ANPC_CAT_ID_SMS_MMS                     (0x05) /**< SMS. Sender name or caller ID.*/
#define BLE_ANPC_CAT_ID_VOICE_MAIL                  (0x06) /**< Voice Mail. Sender name or caller ID.*/
#define BLE_ANPC_CAT_ID_SCHEDULE                    (0x07) /**< Sechedule. Title of the schedule.*/
#define BLE_ANPC_CAT_ID_HIGH_PRIOR_ALERT            (0x08) /**< High Prioritized Alert. Title of alert.*/
#define BLE_ANPC_CAT_ID_INSTANT_MSG                 (0x09) /**< Instant Messaging. Sender name.*/
#define BLE_ANPC_CAT_ID_ALL_ALERT                   (0xFF) /**< All alert.*/
/** @} */

/**@defgroup BLE_ANPC_SUPP_CAT_ID_BIT_MASK          Supported Category ID. If a bit is set to 0, the associated feature is not supported. 1 is supported.
 * @brief The definition of category id bit mask.
 * @{ */
#define BLE_ANPC_SUPP_CAT_SIMPLE_ALERT              (1<<0) /**< Simple Alert.*/  
#define BLE_ANPC_SUPP_CAT_EMAIL                     (1<<1) /**< Email.*/
#define BLE_ANPC_SUPP_CAT_NEWS                      (1<<2) /**< News.*/
#define BLE_ANPC_SUPP_CAT_CALL                      (1<<3) /**< Call.*/
#define BLE_ANPC_SUPP_CAT_MISSED_CALL               (1<<4) /**< Missed Call.*/
#define BLE_ANPC_SUPP_CAT_SMS_MMS                   (1<<5) /**< SMS, MMS.*/
#define BLE_ANPC_SUPP_CAT_VOICE_MAIL                (1<<6) /**< Voice Mail.*/
#define BLE_ANPC_SUPP_CAT_SCHEDULE                  (1<<7) /**< Schedule.*/
#define BLE_ANPC_SUPP_CAT_HIGH_PRIOR_ALERT          (1<<8) /**< High Prioritized Alert.*/
#define BLE_ANPC_SUPP_CAT_INSTANT_MSG               (1<<9) /**< Instant Message.*/
/** @} */

/**@} */ //BLE_ANPC_DEFINES

/**@addtogroup BLE_ANPC_ENUMS Enumerations
 * @{ */

/**@brief Enumeration type of BLE ANPC callback events. */

typedef enum BLE_ANPC_EventId_T
{
    BLE_ANPC_EVT_DISC_COMPLETE_IND = 0x00,         /**< Event for discovery completion. See @ref BLE_ANPC_EvtDiscComplete_T for event details.*/
    BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND,           /**< Event for receiving read supported new alert category response. See @ref BLE_ANPC_EvtSuppNewAlertCatInd_T for event details.*/
    BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND,   /**< Event for receiving read supported unread alert status category response. See @ref BLE_ANPC_EvtSuppUnreadAlertCatInd_T for event details.*/
    BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND,     /**< Event for receiving write new alert response. See @ref BLE_ANPC_EvtWriteNewAlertRspInd_T for event details.*/
    BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND,  /**< Event for receiving write unread alert response. See @ref BLE_ANPC_EvtWriteUnreadAlertRspInd_T for event details.*/
    BLE_ANPC_EVT_WRITE_CP_RSP_IND,                 /**< Event for receiving write cp response. See @ref BLE_ANPC_EvtWriteCpRspInd_T for event details.*/
    BLE_ANPC_EVT_NEW_ALERT_IND,                    /**< Event for receiving new alert notification. See @ref BLE_ANPC_EvtNewAlertInd_T for event details.*/
    BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND,            /**< Event for receiving unread alert status notification. See @ref BLE_ANPC_EvtUnreadAlertStatInd_T for event details.*/
    BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND,             /**< Profile internal unspecified error occurs.*/
    BLE_ANPC_EVT_ERR_NO_MEM_IND,                   /**< Profile internal error occurs due to insufficient heap memory.*/
}BLE_ANPC_EventId_T;

/**@} */ //BLE_ANPC_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_ANPC_STRUCTS Structures
 * @{ */

/**@brief Data structure for @ref BLE_ANPC_EVT_DISC_COMPLETE_IND event. */
typedef struct BLE_ANPC_EvtDiscComplete_T
{
    uint16_t        connHandle;         /**< The connection handle of discovery completion. */
}BLE_ANPC_EvtDiscComplete_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND event. */
typedef struct BLE_ANPC_EvtSuppNewAlertCatInd_T
{
    uint16_t        connHandle;         /**< The connection handle. */
    uint16_t        category;           /**< Supported New Alert Category. See @ref BLE_ANPC_SUPP_CAT_ID_BIT_MASK.*/ 
}BLE_ANPC_EvtSuppNewAlertCatInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND event. */
typedef struct BLE_ANPC_EvtSuppUnreadAlertCatInd_T
{
    uint16_t        connHandle;         /**< The connection handle. */
    uint16_t        category;           /**< Supported Unread Alert Category. See @ref BLE_ANPC_SUPP_CAT_ID_BIT_MASK.*/
}BLE_ANPC_EvtSuppUnreadAlertCatInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteNewAlertRspInd_T
{
    uint16_t        connHandle;         /**< The connection handle.*/
    uint16_t        errCode;            /**< Error code. See @ref ATT_ERROR_CODES.*/
}BLE_ANPC_EvtWriteNewAlertRspInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteUnreadAlertRspInd_T
{
    uint16_t        connHandle;         /**< The connection handle.*/
    uint16_t        errCode;            /**< Error code. See @ref ATT_ERROR_CODES.*/
}BLE_ANPC_EvtWriteUnreadAlertRspInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_WRITE_CP_RSP_IND event. */
typedef struct BLE_ANPC_EvtWriteCpRspInd_T
{
    uint16_t        connHandle;         /**< The connection handle.*/
    uint16_t        errCode;            /**< The error code of config control point response. See @ref BLE_ANPC_ERRCODE. */
}BLE_ANPC_EvtWriteCpRspInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_NEW_ALERT_IND event. */
typedef struct BLE_ANPC_EvtNewAlertInd_T
{
    uint16_t        connHandle;         /**< The connection handle.*/
    uint8_t         categoryId;         /**< The category Id. See @ref BLE_ANPC_CAT_ID.*/
    uint8_t         numOfNewAlert;      /**< Number of new alert.*/
    uint16_t        receivedLength;     /**< Receive txt string length.*/
    uint8_t         *p_receivedValue;   /**< Point of txt string data.*/
}BLE_ANPC_EvtNewAlertInd_T;

/**@brief Data structure for @ref BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND event. */
typedef struct BLE_ANPC_EvtUnreadAlertStatInd_T
{
    uint16_t        connHandle;         /**< The connection handle.*/
    uint8_t         categoryId;         /**< The category Id. See @ref BLE_ANPC_CAT_ID.*/
    uint8_t         unreadCnt;          /**< Unread Count.*/
}BLE_ANPC_EvtUnreadAlertStatInd_T;


/**@brief Union of BLE Alert Notification Client callback event data types. */
typedef union
{
    BLE_ANPC_EvtDiscComplete_T           evtDiscComplete;          /**< Handle @ref BLE_ANPC_EVT_DISC_COMPLETE_IND */
    BLE_ANPC_EvtSuppNewAlertCatInd_T     evtSuppNewAlertCatInd;    /**< Handle @ref BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND */
    BLE_ANPC_EvtSuppUnreadAlertCatInd_T  evtSuppUnreadAlertCatInd; /**< Handle @ref BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND */
    BLE_ANPC_EvtWriteNewAlertRspInd_T    evtWriteNewAlertRspInd;   /**< Handle @ref BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND */
    BLE_ANPC_EvtWriteUnreadAlertRspInd_T evtWriteUnreadAlertRspInd;/**< Handle @ref BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND */
    BLE_ANPC_EvtWriteCpRspInd_T          evtWriteCpRspInd;         /**< Handle @ref BLE_ANPC_EVT_WRITE_CP_RSP_IND */
    BLE_ANPC_EvtNewAlertInd_T            evtNewAlertInd;           /**< Handle @ref BLE_ANPC_EVT_NEW_ALERT_IND */
    BLE_ANPC_EvtUnreadAlertStatInd_T     evtUnreadAlertStatInd;    /**< Handle @ref BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND */
}BLE_ANPC_EventField_T;

/**@brief BLE Alert Notification Client callback event. */
typedef struct  BLE_ANPC_Event_T
{
    BLE_ANPC_EventId_T                  eventId;            /**< Event ID. See @ref BLE_ANPC_EventId_T.  */
    BLE_ANPC_EventField_T               eventField;         /**< Event field */
}BLE_ANPC_Event_T;

/**@brief BLE Alert Notification profile Client callback type. This callback function sends BLE Alert Notification profile client events to the application.*/
typedef void(*BLE_ANPC_EventCb_T)(BLE_ANPC_Event_T *p_event);

/**@} */ //BLE_ANPC_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_ANPC_FUNS Functions
 * @{ */

/**
 * @brief Initialize BLE Alert Notification Client.
 *
 * @retval MBA_RES_SUCCESS          Success to initalize BLE Alert Notification Client.
 * @retval MBA_RES_FAIL             Fail to initalize BLE Alert Notification Client.
 *
 */
uint16_t BLE_ANPC_Init(void);

/**
 * @brief Register BLE Alert Notification Client callback.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 * @param[in] routine        Client callback function.
 *
 */
void BLE_ANPC_EventRegister(BLE_ANPC_EventCb_T routine);

/**
 * @brief Read the value of Supported New Alert Category.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 *@param[in] connHandle             Handle of the connection.
 *
 * @retval MBA_RES_SUCCESS          Send command successful.
 * @retval MBA_RES_FAIL             Fail.
 */
uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle);

/**
 * @brief Read the value of Supported Unread Alert Category.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 * @param[in] connHandle            Handle of the connection.
 * @retval MBA_RES_SUCCESS          Success.
 */
uint16_t BLE_ANPC_ReadSuppUnreadAlertCat(uint16_t connHandle);

/**
 * @brief Enable New Alert Notification.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 * @param[in] connHandle            Handle of the connection.
 * @param[in] enable                Set true to enable notification of New Alert. Otherwise set false.
 *
 * @retval MBA_RES_SUCCESS          Send command successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableNewAlertNtfy(uint16_t connHandle, bool enable);

/**
 * @brief Enable Unread Alert Notificaiton.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 * @param[in] connHandle            Handle of the connection.
 * @param[in] enable                Set true to enable notification of Unread Alert. Otherwise set false.
 *
 * @retval MBA_RES_SUCCESS          Send command successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable);

/**
 * @brief Write Alert Notification Control Point.\n
 *       Application must call this API after starting BLE Alert Notification Client procedure.
 *
 * @param[in] connHandle            Handle of the connection.
 * @param[in] cmdId                 The command Id. See @ref BLE_ANPC_CMD.
 * @param[in] catId                 The category Id. See @ref BLE_ANPC_CAT_ID.
 *
 * @retval MBA_RES_SUCCESS          Send command successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 */
uint16_t BLE_ANPC_WriteAncp(uint16_t connHandle, uint8_t cmdId, uint8_t catId);

/**
 * @brief Handle BLE_Stack related events.
 *       This API should be called in the application while caching BLE_Stack events
 *
*/
void BLE_ANPC_BleEventHandler(STACK_Event_T *p_stackEvent);

/**
 * @brief Handle BLE DD (Database Discovery middleware) events.
 *       This API should be called in the application while caching BLE DD events
 *
 * @param[in] p_event          Pointer to BLE DD events buffer.
 *
*/
void BLE_ANPC_BleDdEventHandler(BLE_DD_Event_T *p_event);

/**@} */ //BLE_ANPC_FUNS


#endif

/**
  @}
*/


