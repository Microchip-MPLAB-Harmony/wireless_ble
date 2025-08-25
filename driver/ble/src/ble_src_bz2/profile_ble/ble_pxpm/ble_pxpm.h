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
  BLE Proximity Profile Monitor Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_pxpm.h

  Summary:
    Interface for the BLE Proximity Profile Monitor used by application developers.

  Description:
    Provides function prototypes and definitions necessary for the integration of
    the BLE Proximity Profile Monitor into user applications.
 *******************************************************************************/

#ifndef BLE_PXPM_H
#define BLE_PXPM_H
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"
#include "ble_gcm/ble_dd.h"

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
 * @addtogroup BLE_PXP BLE Proximity Profile
 * @{
 */

/**
 * @defgroup BLE_PXPM BLE Proximity Profile Monitor
 * 
 * @brief Provides a interface for the BLE Proximity Profile Monitor.
 * @note This module defines the API for the BLE Proximity Profile Monitor, enabling
 *          the management of Proximity reporting and events in a BLE client role.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_PXPM_IAS_ENABLE                                 /**< Feature of PXP Monitor supporting optional Immediate Alert Service. */
#define BLE_PXPM_TPS_ENABLE                                 /**< Feature of PXP Monitor supporting optional Tx Power Service. */

/**
 * @addtogroup BLE_PXPM_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_PXPM_DESC_MAX_NUM PXPM Maximum descriptor number
 * @brief Defines maximum number of descriptors allowed.
 * @{
 */
#define BLE_PXPM_DESC_MAX_NUM               (2U)            /**< Maximum number of descriptors.*/
/** @} */

/**
 * @defgroup BLE_PXPM_SVC_UUID UUIDs for services in the Proximity profile
 * @brief Defines UUIDs for services in the Proximity profile.
 * @{
 */
#define BLE_PXPM_UUID_IMMEDIATE_ALERT_SVC   (0x1802U)       /**< UUID for Immediate Alert Service. */
#define BLE_PXPM_UUID_LINKLOSS_SVC          (0x1803U)       /**< UUID for Link Loss Service. */
#define BLE_PXPM_UUID_TXPOWER_SVC           (0x1804U)       /**< UUID for TX Power Service. */
/** @} */

/**
 * @defgroup BLE_PXPM_UUID UUIDs for characteristics in the Proximity profile
 * @brief Defines UUIDs for characteristics in the Proximity profile.
 * @{
 */
#define BLE_PXPM_UUID_ALERT_LEVEL           (0x2A06U)       /**< UUID for Alert Level characteristic. */
#define BLE_PXPM_UUID_TXPOWER_LEVEL         (0x2A07U)       /**< UUID for Tx Power Level characteristic. */
/** @} */
/** @} */ //BLE_PXPM_DEFINES

/**
 * @addtogroup BLE_PXPM_ENUMS Enumerations
 * @{
 */

/** @brief Alert levels for BLE Proximity Profile Monitor. */
typedef enum BLE_PXPM_AlertLevel_T
{
    BLE_PXPM_ALERT_LEVEL_NO = 0x00U,                        /**< No Alert. */
    BLE_PXPM_ALERT_LEVEL_MILD = 0x01U,                      /**< Mild Alert. */
    BLE_PXPM_ALERT_LEVEL_HIGH = 0x02U,                      /**< High Alert. */
} BLE_PXPM_AlertLevel_T;

/** @brief Enumeration for BLE PXP Monitor profile callback events. */
typedef enum BLE_PXPM_EventId_T
{
    BLE_PXPM_EVT_DISC_COMPLETE_IND = 0x00U,                 /**< Discovery complete; ready for PXP procedures. See @ref BLE_PXPM_EvtDiscComplete_T for event details.*/
    BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND,             /**< LLS Alert Level write response received. See @ref BLE_PXPM_EvtLlsAlertLvWriteRspInd_T for event details.*/
    BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND,                       /**< LLS Alert Level read response received. See @ref BLE_PXPM_EvtLlsAlertLvInd_T for event details.*/
    BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND,                    /**< TPS TX Power Level read response received. See @ref BLE_PXPM_EvtTpsTxPwrLvInd_T for event details.*/
    BLE_PXPM_EVT_ERR_UNSPECIFIED_IND                        /**< Unspecified internal error occurred. */
}BLE_PXPM_EventId_T;

/** @} */ //BLE_PXPM_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_PXPM_STRUCTS Structures
 * @{
*/

/** @brief Structure for the @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event. */
typedef struct BLE_PXPM_EvtDiscComplete_T
{
    uint16_t                connHandle;                     /**< Connection handle where the discovery process completed. */
    uint16_t                llsStartHandle;                 /**< Start handle of the Link Loss Service (LLS). */
    uint16_t                llsEndHandle;                   /**< End handle of the Link Loss Service (LLS). */
#ifdef BLE_PXPM_IAS_ENABLE
    uint16_t                iasStartHandle;                 /**< Start handle of the Immediate Alert Service (IAS), if enabled. */
    uint16_t                iasEndHandle;                   /**< End handle of the Immediate Alert Service (IAS), if enabled. */
#endif
#ifdef BLE_PXPM_TPS_ENABLE
    uint16_t                tpsStartHandle;                 /**< Start handle of the Tx Power Service (TPS), if enabled. */
    uint16_t                tpsEndHandle;                   /**< End handle of the Tx Power Service (TPS), if enabled. */
#endif
} BLE_PXPM_EvtDiscComplete_T;


/** @brief Structure for the @ref BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND event. */
typedef struct BLE_PXPM_EvtLlsAlertLvWriteRspInd_T
{
    uint16_t                connHandle;                     /**< Connection handle associated with the write response. */
    uint16_t                errCode;                        /**< Error code for the write operation, as defined in @ref ATT_ERROR_CODES. */
} BLE_PXPM_EvtLlsAlertLvWriteRspInd_T;


/** @brief Structure for the @ref BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND event. */
typedef struct BLE_PXPM_EvtTpsTxPwrLvInd_T
{
    uint16_t                connHandle;                     /**< Connection handle associated with the Tx power level indication. */
    int8_t                  txPowerLevel;                   /**< Current Tx power level in dBm. */
} BLE_PXPM_EvtTpsTxPwrLvInd_T;


/** @brief Structure for the @ref BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND event. */
typedef struct BLE_PXPM_EvtLlsAlertLvInd_T
{
    uint16_t                connHandle;                     /**< Connection handle associated with the alert level indication. */
    BLE_PXPM_AlertLevel_T   alertLevel;                     /**< Alert level value. */
} BLE_PXPM_EvtLlsAlertLvInd_T;


/** @brief Structure representing a characteristic list. */
typedef struct BLE_PXPM_CharList_T
{
    uint16_t                attrHandle;                     /**< Handle of the attribute. */
    uint8_t                 property;                       /**< Properties of the characteristic. */
    uint16_t                charHandle;                     /**< Handle of the characteristic. */
} BLE_PXPM_CharList_T;


/** @brief Structure representing descriptor information. */
typedef struct BLE_PXPM_DescInfo_T
{
    uint16_t                attrHandle;                     /**< Handle of the descriptor's attribute. */
    uint16_t                uuid;                           /**< UUID of the descriptor. */
} BLE_PXPM_DescInfo_T;


/** @brief Structure representing a descriptor list. */
typedef struct BLE_PXPM_DescList_T
{
    uint8_t                 totalNum;                       /**< Total number of descriptors in the list. */
    BLE_PXPM_DescInfo_T     descInfo[BLE_PXPM_DESC_MAX_NUM]; /**< Array of descriptor information. See @ref BLE_PXPM_DescInfo_T. */
} BLE_PXPM_DescList_T;


/** @brief Union of BLE Proximity Profile Monitor callback event data types. */
typedef union
{
    BLE_PXPM_EvtDiscComplete_T          evtDiscComplete;            /**< Data for @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event. */
    BLE_PXPM_EvtLlsAlertLvWriteRspInd_T evtLlsAlertLvWriteRspInd;   /**< Data for @ref BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND event. */
    BLE_PXPM_EvtLlsAlertLvInd_T         evtLlsAlertLvInd;           /**< Data for @ref BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND event. */
    BLE_PXPM_EvtTpsTxPwrLvInd_T         evtTpsTxPwrLvInd;           /**< Data for @ref BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND event. */
} BLE_PXPM_EventField_T;


/** @brief BLE Proximity Profile Monitor callback event structure. */
typedef struct  BLE_PXPM_Event_T
{
    BLE_PXPM_EventId_T                  eventId;                    /**< Identifier of the event. See @ref BLE_PXPM_EventId_T. */
    BLE_PXPM_EventField_T               eventField;                 /**< Data associated with the event. See @ref BLE_PXPM_EventField_T.  */
} BLE_PXPM_Event_T;


/** 
 * @brief BLE Proximity Profile Monitor callback function type.
 * @note Applications must implement this callback to receive BLE PXP Monitor events.
 * 
 * @param p_event Pointer to the event structure containing event data.
 */
typedef void(*BLE_PXPM_EventCb_T)(BLE_PXPM_Event_T *p_event);

/** @} */ //BLE_PXPM_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_PXPM_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Proximity Profile Monitor.
 *
 * @retval MBA_RES_SUCCESS          Initialization successful.
 * @retval MBA_RES_FAIL             Initialization failed.
 *
 */
uint16_t BLE_PXPM_Init(void);


/**
 * @brief Registers a callback function for BLE Proximity Profile Monitor events.
 *
 * @param[in] routine               Callback function to handle BLE PXP Monitor events.
 *
 */
void BLE_PXPM_EventRegister(BLE_PXPM_EventCb_T routine);


/**
 * @brief Writes the Alert Level value to the Link Loss Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to set the alert level for.
 * @param[in] level                 Alert level to be set. See @ref BLE_PXPM_AlertLevel_T for the valid values.
 *
 * @retval MBA_RES_SUCCESS          Write operation successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_WriteLlsAlertLevel(uint16_t connHandle, BLE_PXPM_AlertLevel_T level);


#ifdef BLE_PXPM_IAS_ENABLE
/**
 * @brief Writes the Alert Level value to the Immediate Alert Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to set the alert level for.
 * @param[in] level                 Alert level to be set. See @ref BLE_PXPM_AlertLevel_T for the valid values.
 *
 * @retval MBA_RES_SUCCESS          Write operation successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_WriteIasAlertLevel(uint16_t connHandle, BLE_PXPM_AlertLevel_T level);
#endif


/**
 * @brief Reads the Alert Level value from the Link Loss Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to read the alert level from.
 *
 * @retval MBA_RES_SUCCESS          Read operation successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_ReadLlsAlertLevel(uint16_t connHandle);


#ifdef BLE_PXPM_TPS_ENABLE
/**
 * @brief Reads the Tx Power Level value from the Tx Power Service of a connected peer PXP Reporter device.
 *
 * @param[in] connHandle            Connection handle to read the Tx Power Level from.
 *
 * @retval MBA_RES_SUCCESS          Read operation successful.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters.
 */
uint16_t BLE_PXPM_ReadTpsTxPowerLevel(uint16_t connHandle);
#endif


/**
 * @brief Retrieves information about the characteristic UUID of the Proximity service that has been discovered.
 * @note This API could be called only after @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event is issued.
 *
 * @param[in]  connHandle           Connection handle.
 * @param[in]  svcUuid              Service UUID (see @ref BLE_PXPM_SVC_UUID).
 * @param[in]  charUuid             Characteristic UUID (see @ref BLE_PXPM_UUID).
 * @param[out] p_charList           Pointer to store the characteristic information of the discovered service. See @ref BLE_PXPM_CharList_T.
 *
 * @retval MBA_RES_SUCCESS          Successfully retrieved the characteristic list.
 * @retval MBA_RES_INVALID_PARA     Invalid parameters, such as:\n
 *                                  - Invalid connection handle.
 *                                  - Invalid service UUID.
 *                                  - Invalid characteristic UUID.
 */
uint16_t BLE_PXPM_GetCharList(uint16_t connHandle, uint16_t svcUuid, uint16_t charUuid, BLE_PXPM_CharList_T *p_charList);


/**
 * @brief Retrieves information about the descriptor list of the Proximity Service that has been discovered.
 * @note This API could be called only after @ref BLE_PXPM_EVT_DISC_COMPLETE_IND event is issued.
 *
 * @param[in]  connHandle           Connection handle.
 * @param[out] p_descList           Pointer to store the descriptor information of the discovered service. See @ref BLE_PXPM_DescList_T.
 *
 * @retval MBA_RES_SUCCESS          Successfully retrieved the descriptor list.
 * @retval MBA_RES_INVALID_PARA     The provided connection handle is not valid.
 */
uint16_t BLE_PXPM_GetDescList(uint16_t connHandle, BLE_PXPM_DescList_T *p_descList);


/**
 * @brief Handles BLE_Stack related events.
 * 
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE Stack events buffer.
 *
 */
void BLE_PXPM_BleEventHandler(STACK_Event_T *p_stackEvent);


/**
 * @brief Handles BLE Database Discovery (BLE DD) events.
 * 
 * @note This function should be called when BLE DD events occur.
 *
 * @param[in] p_event               Pointer to the BLE DD events buffer.
 *
 */
void BLE_PXPM_BleDdEventHandler(BLE_DD_Event_T *p_event);


/** @} */ //BLE_PXPM_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_PXPM_H