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
  BLE OTA Profile Client Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otapc.h

  Summary:
    Interface for the BLE OTA (Over-The-Air) Profile Client used by applications.

  Description:
    This header file provides the interface definitions and function prototypes
    for the BLE OTA Profile Client, enabling the management of firmware updates
    over a BLE connection.
 *******************************************************************************/
#ifndef BLE_OTAPC_H
#define BLE_OTAPC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
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
 * @addtogroup BLE_OTAP BLE OTA Profile
 * @{
 */

/**
 * @defgroup BLE_OTAPC BLE OTA Profile Client
 * 
 * @brief Provides the interface for the BLE OTA Profile Client.
 * @note This section provides the API for the BLE OTA Profile Client, which
 *          allows applications to handle OTA firmware updates via BLE.
 * @{
 */
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPC_DEFINES Defines
 * @{
 */


/**
 * @defgroup BLE_OTAPC_IMG_TYPE_DEF OTAPC image type definitions
 * @brief Defines different types of OTA (Over the Air) Programming Client images.
 * @{
 */
#define BLE_OTAPC_IMG_TYPE_FW                 (1U << 0U)        /**< Firmaware image type. */
/** @} */


/**
 * @defgroup BLE_OTAPC_FW_FEATURE_DEF OTAPC firmware feature definitions
 * @brief Defines extended features supported by the firmware.
 * @{
 */
#define BLE_OTAPC_FW_FEATURE_MASK1            (1U << 0U)        /**< Bitmask for supported feature set 1. */
/** @} */


/**
 * @defgroup BLE_OTAPC_ENC_TYPE_DEF OTAPC encryption type definitions
 * @brief Defines the encryption methods used for OTA images.
 * @{
 */
#define BLE_OTAPC_ENC_NONE                    (0x00U)           /**< No encryption applied to the firmware image. */
#define BLE_OTAPC_ENC_AES_CBC                 (0x01U)           /**< Firmware image encrypted using AES-CBC encryption. */
/** @} */


/**
 * @defgroup BLE_OTAPC_FILE_TYPE_DEF OTAPC file type definitions
 * @brief Defines the types of files used in OTA updates.
 * @{
 */
#define BLE_OTAPC_IMG_FILE_TYPE_EMB_STACK     (0x01U)           /**< Image file for embedded systems or stack memory.  */
#define BLE_OTAPC_IMG_FILE_TYPE_APP           (0x02U)           /**< Image file intended for application use. */
#define BLE_OTAPC_IMG_FILE_TYPE_EXT           (0x03U)           /**< Image file for external memory devices. */
/** @} */


/**
 * @defgroup BLE_OTAPC_RESULT_DEF OTAPC result code definitions
 * @brief Defines result codes from OTA operations.
 * @{
 */
#define BLE_OTAPC_RESULT_SUCCESS              (0x00U)           /**< Operation completed successfully. */
#define BLE_OTAPC_RESULT_INVALID_STATE        (0x01U)           /**< Operation failed due to an invalid state. */
#define BLE_OTAPC_RESULT_NOT_SUPPORTED        (0x02U)           /**< Operation failed because it is not supported. */
#define BLE_OTAPC_RESULT_OPERATION_FAILED     (0x03U)           /**< Operation failed to complete as requested. */
#define BLE_OTAPC_RESULT_INVALID_PARA         (0x04U)           /**< Operation failed due to invalid parameters. */
#define BLE_OTAPC_RESULT_UNSPECIED_ERROR      (0x05U)           /**< Operation failed due to an unspecified error. */
/** @} */

/** @} */ //BLE_OTAPC_DEFINES


/**
 * @addtogroup BLE_OTAPC_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration for BLE OTA profile callback events. */
typedef enum BLE_OTAPC_EventId_T
{
    BLE_OTAPC_EVT_SEC_REQ = 0x00,                               /**< Request for link encryption to access OTA Service characteristic. Applications should 
                                                                    encrypt the link for OTA operations. Refer to @ref BLE_OTAPC_EvtSecReq_T for details. */
    BLE_OTAPC_EVT_CONNECT_IND,                                  /**< OTA profile ready for firmware update.  Refer to @ref BLE_OTAPC_EvtConnectInd_T for details.*/
    BLE_OTAPC_EVT_FEATURE_IND,                                  /**< Supported OTA server features read. Refer to @ref BLE_OTAPC_EvtFeatureInd_T for details.*/
    BLE_OTAPC_EVT_REQ_RSP_IND,                                  /**< Response for update request received. Refer to @ref BLE_OTAPC_EvtReqRspInd_T for details. */
    BLE_OTAPC_EVT_START_RSP_IND,                                /**< Response for start update received. Refer to @ref BLE_OTAPC_EvtStartRspInd_T for details.  */
    BLE_OTAPC_EVT_FRAGMENT_COMPL_IND,                           /**< Notification that a fragment distribution is complete. Applications should wait for this event 
                                                                    after sending fragments reaching the max size before sending more. For the last fragment, wait 
                                                                    for this event before completing the update. Refer to @ref BLE_OTAPC_EvtFragmentComplInd_T for details.*/
    BLE_OTAPC_EVT_COMPLETE_RSP_IND,                             /**< Response for complete update received. Refer to @ref BLE_OTAPC_EvtCompleteRspInd_T for details. */
    BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND                           /**< Unspecified internal error occurred. */ 
}BLE_OTAPC_EventId_T;

/** @} */ //BLE_OTAPC_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPC_STRUCTS Structures
 * @{
 */

/** @brief Structure containing update request information. */
typedef struct BLE_OTAPC_Req_T
{
    uint32_t        fwImageSize;                        /**< Size of the new firmware image. */
    uint32_t        fwImageId;                          /**< Identifier of the new firmware image. */
    uint32_t        fwImageVer;                         /**< Version of the new firmware image. */
    uint8_t         fwImageEnc;                         /**< Encryption method used for the new firmware image. See @ref BLE_OTAPC_ENC_TYPE_DEF. */
    uint16_t        fwImageChksum;                      /**< Checksum of the new firmware image. */
    uint8_t         fwImageFileType;                    /**< File type of the new firmware image. See @ref BLE_OTAPC_FILE_TYPE_DEF. */
    uint16_t        fwImageCrc16;                       /**< CRC-16 checksum of the new firmware image. */
} BLE_OTAPC_Req_T;
 

/** @brief Structure for the @ref BLE_OTAPC_EVT_SEC_REQ event. */
typedef struct BLE_OTAPC_EvtSecReq_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
} BLE_OTAPC_EvtSecReq_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_CONNECT_IND event. */
typedef struct BLE_OTAPC_EvtConnectInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
} BLE_OTAPC_EvtConnectInd_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_FEATURE_IND event. */
typedef struct BLE_OTAPC_EvtFeatureInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         suppImgType;                        /**< Supported image type for OTA. See @ref BLE_OTAPC_IMG_TYPE_DEF. */
    uint8_t         fwExtFeaure;                        /**< Supported extended firmware features for OTA. See @ref BLE_OTAPC_FW_FEATURE_DEF. */
} BLE_OTAPC_EvtFeatureInd_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_REQ_RSP_IND event. */
typedef struct BLE_OTAPC_EvtReqRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< Result of the firmware update request. See @ref BLE_OTAPC_RESULT_DEF. */
    uint16_t        maxFragImgSize;                     /**< Maximum size for fragmented image packets. */
    uint32_t        imgStartIdx;                        /**< Start index for the image update. */
    uint32_t        fwImageVer;                         /**< Version of the current firmware image. */
} BLE_OTAPC_EvtReqRspInd_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_START_RSP_IND event. */
typedef struct BLE_OTAPC_EvtStartRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< Result of the start OTA process request. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtStartRspInd_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND event. 
 *  @note Application shall wait this event after the total length of sent fragmented images reaches to max fragmented image size before distributing the next fragmented images. */
typedef struct BLE_OTAPC_EvtFragmentComplInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< Result of the fragment transfer. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtFragmentComplInd_T;


/** @brief Structure for the @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND event. */
typedef struct BLE_OTAPC_EvtCompleteRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< Result of the OTA update completion. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtCompleteRspInd_T;


/** @brief Union of BLE OTA profile callback event data types. */
typedef union
{
    BLE_OTAPC_EvtSecReq_T               evtSecReq;                 /**< Data for @ref BLE_OTAPC_EVT_SEC_REQ event. */
    BLE_OTAPC_EvtConnectInd_T           evtConnectInd;             /**< Data for @ref BLE_OTAPC_EVT_CONNECT_IND event. */
    BLE_OTAPC_EvtFeatureInd_T           evtFeatureInd;             /**< Data for @ref BLE_OTAPC_EVT_FEATURE_IND event. */
    BLE_OTAPC_EvtReqRspInd_T            evtReqRspInd;              /**< Data for @ref BLE_OTAPC_EVT_REQ_RSP_IND event. */
    BLE_OTAPC_EvtStartRspInd_T          evtStartRspInd;            /**< Data for @ref BLE_OTAPC_EVT_START_RSP_IND event. */
    BLE_OTAPC_EvtFragmentComplInd_T     evtFragComplInd;           /**< Data for @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND event. */
    BLE_OTAPC_EvtCompleteRspInd_T       evtCompleteRspInd;         /**< Data for @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND event. */
} BLE_OTAPC_EventField_T;


/** @brief BLE OTA profile callback event structure. */
typedef struct  BLE_OTAPC_Event_T
{
    BLE_OTAPC_EventId_T                 eventId;                   /**< Identifier for the event type. See @ref BLE_OTAPC_EventId_T. */
    BLE_OTAPC_EventField_T              eventField;                /**< Data associated with the event. See @ref BLE_OTAPC_EventField_T. */
} BLE_OTAPC_Event_T;


/**
 * @brief Callback type for BLE OTA (Over-The-Air) profile events.
 *
 * @note This callback function is used to forward BLE OTA profile events to the 
 *          application layer. When an event occurs, this function will be invoked with 
 *          a pointer to the event data structure.
 *
 * @param p_event Pointer to the BLE_OTAPC_Event_T structure containing event details.
 */
typedef void(*BLE_OTAPC_EventCb_T)(BLE_OTAPC_Event_T *p_event);

/**@} */ //BLE_OTAPC_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_OTAPC_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE Over-The-Air Programming (OTA) profile client.
 *
 * @retval MBA_RES_SUCCESS           The initialization of the BLE OTA profile client is successful.
 *
 */
uint16_t BLE_OTAPC_Init(void);


/**
 * @brief Registers a callback function for the BLE OTA profile client events.
 *
 * @param[in] bleOtapcRoutine        The callback function to handle BLE OTA profile events.
 *
 */
void BLE_OTAPC_EventRegister(BLE_OTAPC_EventCb_T bleOtapcRoutine);


/**
 * @brief Initiates a feature discovery operation.
 *  
 * @note An event of type @ref BLE_OTAPC_EVT_FEATURE_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform feature discovery.
 *
 * @retval MBA_RES_SUCCESS           The start of the feature discovery procedure is successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_FeatureDisc(uint16_t connHandle);


/**
 * @brief Initiates a firmware update request operation.
 * 
 * @note An event of type @ref BLE_OTAPC_EVT_REQ_RSP_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform the firmware update request operation.
 * @param[in] p_req                  Pointer to the request parameters to send to the server. See @ref BLE_OTAPC_Req_T.
 *
 * @retval MBA_RES_SUCCESS           The start of the firmware update request operation was successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateRequest(uint16_t connHandle, BLE_OTAPC_Req_T * p_req);


/**
 * @brief Initiates a firmware update start operation.
 *
 * @note An event of type @ref BLE_OTAPC_EVT_START_RSP_IND will be generated upon completion of the procedure.
 *
 * @param[in] connHandle             The connection handle for which to perform the firmware update start operation.
 * @param[in] imgType                The type of image to use for the firmware update process, as defined by @ref BLE_OTAPC_IMG_TYPE_DEF.
 *
 * @retval MBA_RES_SUCCESS           The start of the firmware update start operation was successful.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateStart(uint16_t connHandle, uint8_t imgType);


/**
 * @brief Distributes a fragmented firmware image to a server device for OTA DFU.
 * 
 * @note The application should wait for the @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND event after the total length of 
 *          sent fragmented images reaches the maximum size before sending the next set of fragments. If the remaining 
 *          fragmented image is smaller than the maximum size, @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND will be triggered 
 *          once the server receives the complete image.
 *  
 * @param[in] connHandle             Handle of the connection for image distribution.
 * @param[in] length                 Length of the fragment, must not exceed ATT_MTU - 3.
 * @param[in] p_data                 Pointer to the fragmented image data.
 *
 * @retval MBA_RES_SUCCESS           Distribution of the fragmented image started successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      Invalid parameters, such as:\n
 *                                      - Invalid connection handle.
 *                                      - Length exceeds ATT_MTU - 3.
 *                                      - Total length of sent fragments plus new length exceeds the maximum image size.
 *@retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_FragmentDist(uint16_t connHandle, uint16_t length, uint8_t *p_data);


/**
 * @brief Initiates the firmware update completion process.
 *
 * @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND event will be generated upon completion.
 *
 * @param[in] connHandle             Handle of the connection for completing the firmware update.
 *
 * @retval MBA_RES_SUCCESS           Firmware update completion process started successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateComplete(uint16_t connHandle);


/**
 * @brief Requests a device reset on the server device after OTA DFU.
 *
 * @param[in] connHandle             Handle of the connection for device reset request.
 *
 * @retval MBA_RES_SUCCESS           Device reset request initiated successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         The OTA profile is not in the correct state to perform this operation.
 *
 */
uint16_t BLE_OTAPC_DeviceReset(uint16_t connHandle);


/**
 * @brief Handles BLE_Stack events.
 *
 * @note This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack events buffer.
 *
*/
void BLE_OTAPC_BleEventHandler(STACK_Event_T *p_stackEvent);


/**
 * @brief Handless BLE Database Discovery (BLE_DD) middleware events.
 *
 * @note This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event               Pointer to BLE_DD events buffer.
 *
 */
void BLE_OTAPC_BleDdEventHandler(BLE_DD_Event_T *p_event);

/** @} */ //BLE_OTAPC_FUNS

/** @} */

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_OTAPC_H