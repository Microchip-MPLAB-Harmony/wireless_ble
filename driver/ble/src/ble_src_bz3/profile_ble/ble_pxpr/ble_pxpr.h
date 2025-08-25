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
  BLE Proximity Profile Reporter Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_pxpr.h

  Summary:
    Interface for the BLE Proximity Profile Reporter used by application developers.

  Description:
    This header file provides function prototypes and definitions for the
    application layer to interact with the BLE Proximity Profile Reporter.
    It facilitates communication and data exchange with remote proximity devices.
 *******************************************************************************/
#ifndef BLE_PXPR_H
#define BLE_PXPR_H


#define BLE_PXPR_IAS_ENABLE                     /**< Feature of PXP Reporter supporting optional Immediate Alert Service. */
#define BLE_PXPR_TPS_ENABLE                     /**< Feature of PXP Reporter supporting optional Tx Power Service. */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"
#include "ble_lls/ble_lls.h"
#ifdef BLE_PXPR_IAS_ENABLE
#include "ble_ias/ble_ias.h"
#endif
#ifdef BLE_PXPR_TPS_ENABLE
#include "ble_tps/ble_tps.h"
#endif

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
 * @defgroup BLE_PXPR BLE Proximity Profile Reporter
 * 
 * @brief Provides a interface for the BLE Proximity Profile Reporter.
 * @note This section documents the API for the BLE Proximity Profile Reporter.
 *          It includes definitions, function prototypes, and other resources
 *          required to manage the Proximity Profile in a BLE application.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_PXPR_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration for BLE Proximity Profile Reporter (PXP Reporter) alert levels. */
typedef enum BLE_PXPR_AlertLevel_T
{
    BLE_PXPR_ALERT_LEVEL_NO = 0x00U,                 /**< Indicate alert level for No Alert. */
    BLE_PXPR_ALERT_LEVEL_MILD = 0x01U,               /**< Indicate alert level for Mild Alert. */
    BLE_PXPR_ALERT_LEVEL_HIGH = 0x02U,               /**< Indicate alert level for High Alert. */
} BLE_PXPR_AlertLevel_T;


/** @brief Enumeration for BLE PXP Reporter callback event identifiers. */
typedef enum BLE_PXPR_EventId_T
{
    BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND = 0x00U, /**< Indicates that the Alert Level characteristic in the Link Loss Service has been written to. 
                                                          See @ref BLE_PXPR_EvtAlertLevelWriteInd_T for event details.*/
    BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND,         /**< Indicates that the Alert Level characteristic in the Immediate Alert Service has been written to. 
                                                          See @ref BLE_PXPR_EvtAlertLevelWriteInd_T for event details.*/
    BLE_PXPR_EVT_ERR_UNSPECIFIED_IND                /**< Indicates an unspecified internal error within the profile. */
}BLE_PXPR_EventId_T;


/** @} */ //BLE_PXPR_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_PXPR_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND, @ref BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND event. */
typedef struct BLE_PXPR_EvtAlertLevelWriteInd_T
{
    uint16_t                            connHandle;               /**< Connection handle associated with the write request. */
    BLE_PXPR_AlertLevel_T               alertLevel;               /**< Desired alert level to be written. See @ref BLE_PXPR_AlertLevel_T. */
} BLE_PXPR_EvtAlertLevelWriteInd_T;


/** @brief Union of BLE Proximity Profile Reporter callback event data types. */
typedef union
{
    BLE_PXPR_EvtAlertLevelWriteInd_T    evtLlsAlertLevelWriteInd; /**< Data for @ref BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND event. */
    BLE_PXPR_EvtAlertLevelWriteInd_T    evtIasAlertLevelWriteInd; /**< Data for @ref BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND event. */
} BLE_PXPR_EventField_T;


/** @brief BLE Proximity Profile Reporter callback event structure. */
typedef struct  BLE_PXPR_Event_T
{
    BLE_PXPR_EventId_T                  eventId;                  /**< Identifier for the event type. See @ref BLE_PXPR_EventId_T. */
    BLE_PXPR_EventField_T               eventField;               /**< Data associated with the event. See @ref BLE_PXPR_EventField_T. */
} BLE_PXPR_Event_T;


/** 
 * @brief Type definition for the callback function used by the BLE PXP Reporter.
 *
 * @param p_event Pointer to the event structure containing the event ID and data.
 */
typedef void(*BLE_PXPR_EventCb_T)(BLE_PXPR_Event_T *p_event);

/** @} */ //BLE_PXPR_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_PXPR_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Proximity Profile (PXP) Reporter.
 *
 * @retval MBA_RES_SUCCESS          The PXP reporter was successfully initialized.
 * @retval MBA_RES_FAIL             The PXP reporter failed to initialize.
 *
 */
uint16_t BLE_PXPR_Init(void);


/**
 * @brief Registers a callback function for BLE PXP Reporter events.
 *
 * @param[in] routine               The function to be called when a PXP Reporter event occurs.
 *
 */
void BLE_PXPR_EventRegister(BLE_PXPR_EventCb_T routine);


/**
 * @brief Sets the Alert Level for the Link Loss Service (LLS).
 *
 * @param[in] level                 The desired Alert Level to be set. See @ref BLE_PXPR_AlertLevel_T.
 *
 * @retval MBA_RES_SUCCESS          The alert level was successfully set.
 * @retval MBA_RES_INVALID_PARA     The provided parameters are invalid.
 */
uint16_t BLE_PXPR_SetLlsAlertLevel(BLE_PXPR_AlertLevel_T level);


/**
 * @brief Sets the Transmission Power Level for the Tx Power Service.
 *
 * @param[in] level                 The desired Tx Power Level to be set.
 *
 */
void BLE_PXPR_SetTxPowerLevel(int8_t level);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE Stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE Stack event data structure.
 *
*/
void BLE_PXPR_BleEventHandler(STACK_Event_T *p_stackEvent);


/** @} */ //BLE_PXPR_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_PXPR_H