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
  BLE OTA Profile Server Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otaps.h

  Summary:
    Interface for the BLE OTA (Over-The-Air) Profile Server used by applications.

  Description:
    The BLE OTA Profile Server module provides the necessary functions and
    definitions for an application to implement the OTA firmware update
    capability. It facilitates the transfer of firmware images from an OTA
    client to the server hosting the BLE device.
 *******************************************************************************/
#ifndef BLE_OTAPS_H
#define BLE_OTAPS_H

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
 * @addtogroup BLE_OTAP BLE OTA Profile
 * @{
 */

/**
 * @defgroup BLE_OTAPS BLE OTA Profile Server
 * 
 * @brief Interface for the BLE OTA (Over-The-Air) Profile Server used by applications.
 * @note This section contains definitions and prototypes that form the API for
 *          the BLE OTA profile server, facilitating the integration of OTA
 *          firmware update capabilities into BLE-enabled applications.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPS_DEFINES Defines
 * @{
 */

/**
 * @defgroup BLE_OTAPS_IMG_TYPE_DEF OTAPS image type definitions
 * @brief Defines OTAPS image types.
 * @{
 */
#define BLE_OTAPS_IMG_TYPE_FW                    (1U << 0U)                              /**< Firmaware image type. */
/** @} */


/**
 * @defgroup BLE_OTAPS_FW_FEATURE_DEF OTAPS firmware extended feature definitions
 * @brief Defines extended firmware features in OTAPS.
 * @{
 */
#define BLE_OTAPS_FW_FEATURE_MASK1               (1U << 0U)                              /**< Bitmask for supported feature set 1. */
/** @} */


/**
 * @defgroup BLE_OTAPS_FEATURE_DEF OTAPS supported features
 * @brief Defines features supported by the BLE OTA Profile.
 * @{
 */
#define BLE_OTAPS_FEATURE_SUPP_IMG_TYPE          (BLE_OTAPS_IMG_TYPE_FW)                 /**< Supported image type for BLE OTA Profile. */
#define BLE_OTAPS_FEATURE_FW_EXT_FEATURE         (BLE_OTAPS_FW_FEATURE_MASK1)            /**< Supported extended firmware feature for BLE OTA Profile. */
/** @} */


/**
 * @defgroup BLE_OTAPS_IMG_FILE_TYPE_DEF OTAPS image file type definitions
 * @brief Defines OTAPS image file types.
 * @{
 */
#define BLE_OTAPS_IMG_FILE_TYPE_INT              (0x01U)                                 /**< Image designated for internal flash memory. */
#define BLE_OTAPS_IMG_FILE_TYPE_APP              (0x02U)                                 /**< Image to be passed to the application layer. */
#define BLE_OTAPS_IMG_FILE_TYPE_EXT              (0x03U)                                 /**< Image designated for external flash memory. */
#define BLE_OTAPS_IMG_FILE_TYPE_UNKNOWN          (0xFFU)                                 /**< Unrecognized image file type. */
/** @} */


/** @} */ //BLE_OTAPS_DEFINES


/**
 * @addtogroup BLE_OTAPS_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration for BLE OTA (Over-The-Air) profile callback events. */
typedef enum BLE_OTAPS_EventId_T
{
    BLE_OTAPS_EVT_UPDATE_REQ=0x0U,                          /**< Event indicating a request to initiate the update process. See @ref BLE_OTAPS_EvtUpdateReq_T for event details. */
    BLE_OTAPS_EVT_START_IND,                                /**< Event indicating the start of a firmware image update. See @ref BLE_OTAPS_EvtStartInd_T for event details. */
    BLE_OTAPS_EVT_UPDATING_IND,                             /**< Event indicating ongoing transmission of firmware image fragments. See @ref BLE_OTAPS_EvtUpdatingInd_T for event details. */
    BLE_OTAPS_EVT_UPDATING_REQ,                             /**< Event requesting continuation of the firmware image update. The application should respond with @ref BLE_OTAPS_UpdatingResponse. 
                                                                  See @ref BLE_OTAPS_EvtUpdatingInd_T for event details. */
    BLE_OTAPS_EVT_COMPLETE_IND,                             /**< Event indicating the completion of the firmware update. See @ref BLE_OTAPS_EvtCompleteInd_T for event details. */
    BLE_OTAPS_EVT_RESET_IND,                                /**< Event indicating a request for the device to reset, typically after a firmware update. */
    BLE_OTAPS_EVT_ERR_UNSPECIFIED_IND                       /**< Event indicating an unspecified internal error within the profile. */
}BLE_OTAPS_EventId_T;

/** @} */ //BLE_OTAPS_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPS_STRUCTS Structures
 * @{
 */

/** @brief Structure containing the current device's firmware information. */
typedef struct BLE_OTAPS_DevInfo_T
{
    uint32_t      fwImageVer;                                    /**< Firmware image version of the current device.  */
} BLE_OTAPS_DevInfo_T;


/** @brief Structure for the @ref BLE_OTAPS_EVT_UPDATE_REQ event. */
typedef struct BLE_OTAPS_EvtUpdateReq_T
{
    uint16_t      connHandle;                                 /**< Connection handle for the firmware update request. */
    uint32_t      fwImageSize;                                /**< Size of the new firmware image in bytes. */
    uint32_t      fwImageId;                                  /**< Identifier of the new firmware image. */
    uint32_t      fwImageVer;                                 /**< Version of the new firmware image. */
    uint16_t      fwImageChksum;                              /**< Checksum of the new firmware image for validation. */
    uint16_t      fwImageCrc16;                               /**< CRC-16 checksum of the new firmware image for validation. */
    uint8_t       fwImageFileType;                            /**< File type of the new firmware image. Refer to @ref BLE_OTAPS_IMG_FILE_TYPE for possible values. */
} BLE_OTAPS_EvtUpdateReq_T;


/** @brief Structure for the @ref BLE_OTAPS_EVT_START_IND event. */
typedef struct BLE_OTAPS_EvtStartInd_T
{
    uint8_t       imageType;                                  /**< Type of the image being updated. Refer to @ref BLE_OTAPS_IMG_TYPE_DEF for possible values. */
} BLE_OTAPS_EvtStartInd_T;


/** @brief Structure for the @ref BLE_OTAPS_EVT_UPDATING_IND event. */
typedef struct BLE_OTAPS_EvtUpdatingInd_T
{
    uint16_t      length;                                     /**< Length of the current image fragment being updated. */
    uint8_t       *p_fragment;                                /**< Pointer to the image fragment data. If not NULL, the application must handle the fragment update. */
} BLE_OTAPS_EvtUpdatingInd_T;


/** @brief Structure for the @ref BLE_OTAPS_EVT_COMPLETE_IND event. */
typedef struct BLE_OTAPS_EvtCompleteInd_T
{
    bool          errStatus;                                  /**< Error status of the update process. If false, the application must 
                                                                  call @ref BLE_OTAPS_CompleteResponse to send the result to the peer device. */
} BLE_OTAPS_EvtCompleteInd_T;


/** @brief Union of BLE OTA profile callback event data types. */
typedef union
{
    BLE_OTAPS_EvtUpdateReq_T            evtUpdateReq;         /**< Data for @ref BLE_OTAPS_EVT_UPDATE_REQ event. */
    BLE_OTAPS_EvtStartInd_T             evtStartInd;          /**< Data for @ref BLE_OTAPS_EVT_START_IND event. */    
    BLE_OTAPS_EvtUpdatingInd_T          evtUpdatingInd;       /**< Data for @ref BLE_OTAPS_EVT_UPDATING_IND event. */        
    BLE_OTAPS_EvtCompleteInd_T          evtCompleteInd;       /**< Data for @ref BLE_OTAPS_EVT_COMPLETE_IND event. */        
} BLE_OTAPS_EventField_T;


/** @brief BLE OTA profile callback event structure. */
typedef struct  BLE_OTAPS_Event_T
{
    BLE_OTAPS_EventId_T                 eventId;              /**< Identifier for the event. See @ref BLE_OTAPS_EventId_T. */
    BLE_OTAPS_EventField_T              eventField;           /**< Data associated with the event. See @ref BLE_OTAPS_EventField_T.*/
} BLE_OTAPS_Event_T;


/** @brief Type definition for the BLE OTA profile event callback function.
 * 
 * @note This callback function is used to send BLE OTA profile events to the application.
 * 
 * @param p_event Pointer to the event structure containing event data.
 */
typedef void(*BLE_OTAPS_EventCb_T)(BLE_OTAPS_Event_T *p_event);

/** @} */ //BLE_OTAPS_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPS_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Over-The-Air Programming Service (OTAPS) profile and service.
 *
 * @retval MBA_RES_SUCCESS            Initialization successful.
 *
 */
uint16_t BLE_OTAPS_Init(void);


/**
 * @brief Registers a callback function for BLE OTA profile events.
 *
 * @param[in] bleOtapsRoutine         Callback function to handle BLE OTA profile events.
 *
 */
void BLE_OTAPS_EventRegister(BLE_OTAPS_EventCb_T bleOtapsRoutine);


/**
 * @brief Stops the ongoing BLE OTA firmware update procedure.
 *
 * @retval MBA_RES_SUCCESS            OTA procedure successfully stopped.
 */
uint16_t BLE_OTAPS_UpdateStop(void);


/**
 * @brief Sets the Initialization Vector (IV) and encryption key for AES-CBC decryption of the new OTA DFU image.
 *
 * @note This function must be called before starting the BLE OTA procedure if the image is encrypted.
 *
 * @param[in] p_iv                    Pointer to the 16-byte IV.
 * @param[in] p_key                   Pointer to the 16-byte encryption key.
 *
 * @retval MBA_RES_SUCCESS            IV and key set successfully.
 * @retval MBA_RES_BAD_STATE          Operation failed because OTA is in progress.
 */
uint16_t BLE_OTAPS_SetEncrytionInfo(uint8_t *p_iv, uint8_t *p_key);


/**
 * @brief Responds to the peer device after receiving a @ref BLE_OTAPS_EVT_UPDATE_REQ event.
 *
 * @note The application must decide whether to allow the start of the BLE OTA update procedure.
 * 
 * @param[in] connHandle             Handle of the connection to send the response.
 * @param[in] isAllow                True to allow the firmware update, false to deny.
 * @param[in] p_devInfo              Pointer to the device information to be sent if isAllow is true; ignored otherwise. 
 *                                    See @ref BLE_OTAPS_DevInfo_T.
 *
 * @retval MBA_RES_SUCCESS           Response sent successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         Inappropriate OTA state to call this function.
 *
 */
uint16_t BLE_OTAPS_UpdateResponse(uint16_t connHandle, bool isAllow, BLE_OTAPS_DevInfo_T *p_devInfo);


/**
 * @brief Responds to the peer device after receiving a @ref BLE_OTAPS_EVT_UPDATING_REQ event.
 * 
 * @note The application must handle the update of the fragmented firmware image.
 * 
 * @param[in] success                True if the update is successful, false otherwise.
 *
 * @retval MBA_RES_SUCCESS           Response sent successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_BAD_STATE         Inappropriate OTA state to call this function.
 *
 */
uint16_t BLE_OTAPS_UpdatingResponse(bool success);


/**
 * @brief Responds to the peer device after receiving a @ref BLE_OTAPS_EVT_COMPLETE_IND event if errStatus is false.
 * 
 * @note The application must verify the success or failure of the device firmware update.
 *
 * @param[in] success                True if the firmware update is successful, false otherwise.
 *
 * @retval MBA_RES_SUCCESS           Response sent successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_BAD_STATE         Inappropriate OTA state to call this function.
 *
 */
uint16_t BLE_OTAPS_CompleteResponse(bool success);


/**
 * @brief Handles BLE_Stack events.
 *
 * @note This function should be called by the application when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack event data.
 *
*/
void BLE_OTAPS_BleEventHandler(STACK_Event_T *p_stackEvent);


/** @} */ //BLE_OTAPS_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_OTAPS_H