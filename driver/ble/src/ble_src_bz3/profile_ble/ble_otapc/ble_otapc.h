/*******************************************************************************
  BLE OTA Profile Client Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otapc.h

  Summary:
    This file contains the BLE OTA Profile Client functions for application user.

  Description:
    This file contains the BLE OTA Profile Client functions for application user.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

/**
 * @addtogroup BLE_OTAPC
 * @{
 * @brief Header file for the BLE OTA Profile Client library.
 * @note Definitions and prototypes for the BLE OTA profile client stack layer application programming interface.
 */
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


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPC_DEFINES Defines
 * @{ */


/**@defgroup BLE_OTAPC_IMG_TYPE_DEF OTAPC image type
 * @brief The definition of image type.
 * @{ */
#define BLE_OTAPC_IMG_TYPE_FW                 (1 << 0)                                 /**< Firmaware image type. */
/** @} */

/**@defgroup BLE_OTAPC_ENC_TYPE_DEF Encryption method
 * @brief The definition of encryption method.
 * @{ */
#define BLE_OTAPC_ENC_NONE                    0x00                                     /**< New firmware image is not encrypted. */
#define BLE_OTAPC_ENC_AES_CBC                 0x01                                     /**< New firmware image is encrypted by AES-CBC. */
/** @} */

/**@defgroup BLE_OTAPC_RESULT_DEF OTAPC result code
 * @brief The definition of operation result.
 * @{ */
#define BLE_OTAPC_RESULT_SUCCESS              0x00                                     /**< Response for successful operation. */
#define BLE_OTAPC_RESULT_INVALID_STATE        0x01                                     /**< Response if the requested operation is not valid in the current state of the firmware update process. */
#define BLE_OTAPC_RESULT_NOT_SUPPORTED        0x02                                     /**< Response if unsupported Opcode is received. */
#define BLE_OTAPC_RESULT_OPERATION_FAILED     0x03                                     /**< Response if the requested operation failed. */
#define BLE_OTAPC_RESULT_INVALID_PARA         0x04                                     /**< Response if Parameter received does not meet the requirements of the service. */
#define BLE_OTAPC_RESULT_UNSPECIED_ERROR      0x05                                     /**< The server has encountered an unspecified error. */
/** @} */



/**@} */ //BLE_OTAPC_DEFINES


/**@addtogroup BLE_OTAPC_ENUMS Enumerations
 * @{ */

/**@brief Enumeration type of BLE OTA profile callback events. */
typedef enum BLE_OTAPC_EventId_T
{
    BLE_OTAPC_EVT_SEC_REQ = 0x00,                           /**< Encryption is required accessing OTA Service characteristic at remote. Application should encrypt the link to perform further OTA operations. See @ref BLE_OTAPC_EvtSecReq_T for event details. */
    BLE_OTAPC_EVT_CONNECT_IND,                              /**< OTA profile is ready to perform firmware update procedure. See @ref BLE_OTAPC_EvtConnectInd_T for event details. */
    BLE_OTAPC_EVT_FEATURE_IND,                              /**< The supported feature of the OTA server read by @ref BLE_OTAPC_FeatureDisc. See @ref BLE_OTAPC_EvtFeatureInd_T for event details. */
    BLE_OTAPC_EVT_REQ_RSP_IND,                              /**< The response of @ref BLE_OTAPC_UpdateRequest operation. See @ref BLE_OTAPC_EvtReqRspInd_T for event details. */
    BLE_OTAPC_EVT_START_RSP_IND,                            /**< The response of @ref BLE_OTAPC_UpdateStart operation. See @ref BLE_OTAPC_EvtStartRspInd_T for event details. */
    BLE_OTAPC_EVT_FRAGMENT_COMPL_IND,                       /**< The notification of @ref BLE_OTAPC_FragmentDist operation. See @ref BLE_OTAPC_EvtFragmentComplInd_T for event details.
                                                             Application shall wait this event after the total length of sent fragmented images reaches to max fragmented image size before distributing the next fragmented images.
                                                             If the length of remaining fragmented image is less than max fragmented image size, application shall wait this event before calling @ref BLE_OTAPC_UpdateComplete. */
    BLE_OTAPC_EVT_COMPLETE_RSP_IND,                         /**< The response of @ref BLE_OTAPC_UpdateComplete operation. See @ref BLE_OTAPC_EvtCompleteRspInd_T for event details. */
    BLE_OTAPC_EVT_ERR_UNSPECIFIED_IND                       /**< Profile internal unspecified error occurs. */ 
}BLE_OTAPC_EventId_T;

/**@} */ //BLE_OTAPC_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPC_STRUCTS Structures
 * @{ */

/**@brief Data structure for @ref BLE_OTAPC_EVT_SEC_REQ event. */
typedef struct BLE_OTAPC_EvtSecReq_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
} BLE_OTAPC_EvtSecReq_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_CONNECT_IND event. */
typedef struct BLE_OTAPC_EvtConnectInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
} BLE_OTAPC_EvtConnectInd_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_FEATURE_IND event. */
typedef struct BLE_OTAPC_EvtFeatureInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         suppImgType;                        /**< The supported image type. See @ref BLE_OTAPC_IMG_TYPE_DEF */
} BLE_OTAPC_EvtFeatureInd_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_REQ_RSP_IND event. */
typedef struct BLE_OTAPC_EvtReqRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< The request result. See @ref BLE_OTAPC_RESULT_DEF. */
    uint16_t        maxFragImgSize;                     /**< Maximum fragmented image size. */
    uint32_t        imgStartIdx;                        /**< The start index of image. */
    uint32_t        fwImageVer;                         /**< The version of current firmware image. */
} BLE_OTAPC_EvtReqRspInd_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_START_RSP_IND event. */
typedef struct BLE_OTAPC_EvtStartRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< The start result. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtStartRspInd_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND event. 
 * Application shall wait this event after the total length of sent fragmented images reaches to max fragmented image size before distributing the next fragmented images. */
typedef struct BLE_OTAPC_EvtFragmentComplInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< The start result. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtFragmentComplInd_T;

/**@brief Data structure for @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND event. */
typedef struct BLE_OTAPC_EvtCompleteRspInd_T
{
    uint16_t        connHandle;                         /**< Connection handle associated with this connection. */
    uint8_t         result;                             /**< The start result. See @ref BLE_OTAPC_RESULT_DEF. */
} BLE_OTAPC_EvtCompleteRspInd_T;


/**@brief Union of BLE OTA profile callback event data types. */
typedef union
{
    BLE_OTAPC_EvtSecReq_T               evtSecReq;                 /**< Handle @ref BLE_OTAPC_EVT_SEC_REQ. */
    BLE_OTAPC_EvtConnectInd_T           evtConnectInd;             /**< Handle @ref BLE_OTAPC_EVT_CONNECT_IND. */
    BLE_OTAPC_EvtFeatureInd_T           evtFeatureInd;             /**< Handle @ref BLE_OTAPC_EVT_FEATURE_IND. */
    BLE_OTAPC_EvtReqRspInd_T            evtReqRspInd;              /**< Handle @ref BLE_OTAPC_EVT_REQ_RSP_IND. */
    BLE_OTAPC_EvtStartRspInd_T          evtStartRspInd;            /**< Handle @ref BLE_OTAPC_EVT_START_RSP_IND. */
    BLE_OTAPC_EvtFragmentComplInd_T     evtFragComplInd;           /**< Handle @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND. */
    BLE_OTAPC_EvtCompleteRspInd_T       evtCompleteRspInd;         /**< Handle @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND. */
} BLE_OTAPC_EventField_T;

/**@brief BLE OTA profile callback event.*/
typedef struct  BLE_OTAPC_Event_T
{
    BLE_OTAPC_EventId_T         eventId;                /**< Event ID.*/
    BLE_OTAPC_EventField_T      eventField;             /**< Event field. */
} BLE_OTAPC_Event_T;

/**@brief The structure contains udpate request information. \n
* It should be used to request the OTA procedure in @ref BLE_OTAPC_UpdateRequest. */
typedef struct BLE_OTAPC_Req_T
{
    uint32_t                    fwImageSize;            /**< The size of new firmware image. */
    uint32_t                    fwImageId;              /**< The identity of new firmware image. */
    uint32_t                    fwImageVer;             /**< The version of new firmware image */
    uint8_t                     fwImageEnc;             /**< The encrypt method of new firmware image. See @ref BLE_OTAPC_ENC_TYPE_DEF. */
} BLE_OTAPC_Req_T;

/**@brief BLE OTA profile callback type. This callback function sends BLE OTA profile events to the application. */
typedef void(*BLE_OTAPC_EventCb_T)(BLE_OTAPC_Event_T *p_event);

/**@} */ //BLE_OTAPC_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**@addtogroup BLE_OTAPC_FUNS Functions
 * @{ */

/**
 *@brief Initialize BLE OTA Profile.
 *
 * @retval MBA_RES_SUCCESS          Success to initialize BLE OTA Profile. 
 *
 */
uint16_t BLE_OTAPC_Init();

/**
 *@brief Register BLE OTA profile callback.\n
 *       Application must call this API before starting BLE OTA procedure. 
 *
 *
 *@param[in] bleOtapcRoutine        Client callback function.
 *
 */
void BLE_OTAPC_EventRegister(BLE_OTAPC_EventCb_T bleOtapcRoutine);

/**
 *@brief Perform OTA features discovery operation for Server role device.\n
 *       @ref BLE_OTAPC_EVT_FEATURE_IND will be generated if the procedure is complete.
 *
 *@param[in] connHandle             Handle of the connection to do feature discovery. 
 *
 *@retval MBA_RES_SUCCESS           Successfully starts the feature discovery procedure.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *@retval MBA_RES_BAD_STATE         OTA profile is not ready to perform this operation.
 *
 */
uint16_t BLE_OTAPC_FeatureDisc(uint16_t connHandle);

/**
 *@brief Perform firmware update request operation.\n
 *       @ref BLE_OTAPC_EVT_REQ_RSP_IND will be generated if the procedure is complete.
 *
 *@param[in] connHandle             Handle of the connection to do firmware update request operation. 
 *@param[in] p_req                  The request parameters send to server.
 *
 *@retval MBA_RES_SUCCESS           Successfully starts firmware update request operation.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *@retval MBA_RES_BAD_STATE         OTA profile is not ready to perform this operation.
 *
 */
uint16_t BLE_OTAPC_UpdateRequest(uint16_t connHandle, BLE_OTAPC_Req_T * p_req);

/**
 *@brief Perform firmware update start operation.\n
 *       @ref BLE_OTAPC_EVT_START_RSP_IND will be generated if the procedure is complete.
 *
 *@param[in] connHandle             Handle of the connection to do firmware update start operation. 
 *@param[in] imgType                The image used for firmware update process. See @ref BLE_OTAPC_IMG_TYPE_DEF.
 *
 *@retval MBA_RES_SUCCESS           Successfully starts firmware update start operation.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *@retval MBA_RES_BAD_STATE         OTA profile server has not allowed to perform firmware update proceudre yet.
 *
 */
uint16_t BLE_OTAPC_UpdateStart(uint16_t connHandle, uint8_t imgType);

/**
 *@brief Distribute fragmented firmware image to Server device for OTA DFU operation.\n
 *       Application shall wait the @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND after the total length of sent fragmented images reaches to max fragmented image size before distributing the next fragmented images.
 *       If the length of remaining fragmented image is less than max fragmented image size, @ref BLE_OTAPC_EVT_FRAGMENT_COMPL_IND will be generated once the complete image is received by server.
 * 
 *@param[in] connHandle             Handle of the connection to do fragmented image distribution. 
 *@param[in] length                 The length of fragment. It should not exceed ATT_MTU - 3.
 *@param[in] p_data                 The fragmented image data.
 *
 *@retval MBA_RES_SUCCESS           Successfully starts fragmented image distribution.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters.
 *                                  - Connection handle is not valid.
 *                                  - The length is exceed ATT_MTU - 3.
 *                                  - The total length of sent fragmented images + length exceeds max fragmented image size.
 *@retval MBA_RES_BAD_STATE         OTA profile server has not allowed to perform firmware update proceudre yet.
 *
 */
uint16_t BLE_OTAPC_FragmentDist(uint16_t connHandle, uint16_t length, uint8_t *p_data);

/**
 *@brief Perform firmware update complete operation.\n
 *       @ref BLE_OTAPC_EVT_COMPLETE_RSP_IND will be generated if the procedure is complete.
 *
 *@param[in] connHandle             Handle of the connection to do firmware update complete operation. 
 *
 *@retval MBA_RES_SUCCESS           Successfully starts firmware update complete operation.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *@retval MBA_RES_BAD_STATE         OTA profile server has not allowed to perform firmware update proceudre yet.
 *
 */
uint16_t BLE_OTAPC_UpdateComplete(uint16_t connHandle);

/**
 *@brief Perform device reset request operation to Server role device after OTA DFU procedure.\n
 *
 *@param[in] connHandle             Handle of the connection to do device reset request operation. 
 *
 *@retval MBA_RES_SUCCESS           Successfully starts device reset request operation.
 *@retval MBA_RES_OOM               No available memory.
 *@retval MBA_RES_INVALID_PARA      Invalid parameters. Connection handle is not valid
 *@retval MBA_RES_BAD_STATE         OTA profile server has not allowed to perform firmware update proceudre yet.
 *
 */
uint16_t BLE_OTAPC_DeviceReset(uint16_t connHandle);

/**@brief Handle BLE_Stack events.
 *       This API should be called in the application while caching BLE_Stack events
 *
 * @param[in] p_stackEvent          Pointer to BLE_Stack events buffer.
 *
*/
void BLE_OTAPC_BleEventHandler(STACK_Event_T *p_stackEvent);

/**@brief Handle BLE_DD (Database Discovery middleware) events.
 *        This API should be called in the application while caching BLE_DD events
 *
 * @param[in] p_event               Pointer to BLE_DD events buffer.
 *
 */
void BLE_OTAPC_BleDdEventHandler(BLE_DD_Event_T *p_event);

/**@} */ //BLE_OTAPC_FUNS


#endif

/**
  @}
*/



