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
  BLE OTA Profile Server Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otaps.c

  Summary:
    Implements the server-side functionality of the BLE Over-The-Air (OTA)
    Profile, enabling firmware updates for BLE devices.

  Description:
    Provides an interface for the application layer to handle incoming OTA
    firmware update requests and manage the OTA process.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "bt_sys.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_util/mw_dfu.h"
#include "ble_util/mw_aes.h"
#include "ble_otas/ble_otas.h"
#include "ble_otaps.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/* Operation Codes for BLE OTAPS. */
#define BLE_OTAPS_OP_CODE_RESPONSE            (0x01U)           // Operation code for OTAPS response.
#define BLE_OTAPS_OP_CODE_UPDATE_REQUEST      (0x02U)           // Operation code for OTAPS update request.
#define BLE_OTAPS_OP_CODE_UPDATE_START        (0x03U)           // Operation code for OTAPS update start.
#define BLE_OTAPS_OP_CODE_UPDATE_COMPLETE     (0x04U)           // Operation code for OTAPS update complete.
#define BLE_OTAPS_OP_CODE_RESET_REQUEST       (0x05U)           // Operation code for OTAPS reset request.

/* Various size definitions. */
#define BLE_OTAPS_UPDATE_REQ_BASIC_SIZE       (0x0DU)           // Basic size for OTAPS update request.
#define BLE_OTAPS_UPDATE_REQ_FW_MASK1_SIZE    (0x05U)           // Size of firmware mask 1 for OTAPS update request.
#define BLE_OTAPS_UPDATE_RSP_SIZE             (0x0AU)           // Size of OTAPS update response.
#define BLE_OTAPS_UPDATE_START_SIZE           (0x01U)           // Size of OTAPS update start.

/* States for BLE OTAPS. */
#define BLE_OTAPS_STATE_CCCD_DISABLED         (0x00U)           // State indicating that CCCD is disabled in OTAPS.  
#define BLE_OTAPS_STATE_CCCD_ENABLED          (0x01U)           // State indicating that CCCD is enabled in OTAPS.
#define BLE_OTAPS_STATE_WAIT_REQ_RSP          (0x02U)           // State indicating OTAPS is waiting for a request response.
#define BLE_OTAPS_STATE_UPDATE_REQ            (0x03U)           // State indicating an update request in OTAPS.
#define BLE_OTAPS_STATE_UPDATE_START          (0x04U)           // State indicating the start of an update in OTAPS.
#define BLE_OTAPS_STATE_UPDATE_ERROR          (0x05U)           // State indicating an error during update in OTAPS.
#define BLE_OTAPS_STATE_WAIT_UPDATE_RSP       (0x06U)           // State indicating OTAPS is waiting for an update response.
#define BLE_OTAPS_STATE_WAIT_COMP_RSP         (0x07U)           // State indicating OTAPS is waiting for a complete response.
#define BLE_OTAPS_STATE_COMPLETE              (0x08U)           // State indicating the completion of an operation in OTAPS.

/* Result code for BLE OTAPC. */
#define BLE_OTAPS_RESULT_SUCCESS              (0x00U)           // Result code for successful operation in OTAPS.
#define BLE_OTAPS_RESULT_INVALID_STATE        (0x01U)           // Result code for invalid state error in OTAPS. */
#define BLE_OTAPS_RESULT_NOT_SUPPORTED        (0x02U)           // Result code for not supported error in OTAPS.
#define BLE_OTAPS_RESULT_OPERATION_FAILED     (0x03U)           // Result code for operation failed error in OTAPS.
#define BLE_OTAPS_RESULT_INVALID_PARA         (0x04U)           // Result code for invalid parameter error in OTAPS.

/* Encryption type for BLE OTAPC. */
#define BLE_OTAPS_ENC_NONE                    (0x00U)           // Encryption type indicating no encryption for OTAPS.
#define BLE_OTAPS_ENC_AES_CBC                 (0x01U)           // Encryption type indicating AES CBC encryption for OTAPS.

#define BLE_OTAPS_START_IDX_LEN               (0x04U)           // Length of the start index in OTAPS.
#define BLE_OTAPS_IMG_BUF_SIZE                (0x200U)          // Size of the image buffer in OTAPS.
#define BLE_OTAPS_MTU_SIZE                    (0x400U)          // MTU size for OTAPS.


#define BLE_OTAPS_CCCD_CTRL_ON                (1U << 0U)        // Bitmask to turn on control point CCCD in OTAPS.
#define BLE_OTAPS_CCCD_DATA_ON                (1U << 1U)        // Bitmask to turn on data CCCD in OTAPS.

/* Retry type for OTAPS. */
#define BLE_OTAPS_RETRY_TYPE_NONE             (0x00U)           // Retry type indicating no retry for OTAPS.
#define BLE_OTAPS_RETRY_TYPE_ERROR            (0x01U)           // Retry type indicating a retry on error for OTAPS.
#define BLE_OTAPS_RETRY_TYPE_READ             (0x02U)           // Retry type indicating a retry on read operation for OTAPS.
#define BLE_OTAPS_RETRY_TYPE_WRITE            (0x03U)           // Retry type indicating a retry on write operation for OTAPS.
#define BLE_OTAPS_RETRY_TYPE_NTF              (0x04U)           // Retry type indicating a retry on notification for OTAPS.


#define BLE_OTAPS_INVALID_CONN_HANDLE         (0xFFFFU)         // Constant representing an invalid connection handle in OTAPS.

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* Structure to keep track of BLE OTAPS retry list items. */
typedef struct BLE_OTAPS_RetryList_T
{
    uint16_t        connHandle;         // Connection handle associated with the retry item. */
    uint8_t         *p_pkt;             // Pointer to the packet data that needs to be retried. */
    uint8_t         type;               // Type of the retry item, defining the operation to be retried. */
    GATT_Event_T    *p_gattEvt;         // Pointer to the GATT event associated with the retry item. */
}BLE_OTAPS_RetryList_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static BLE_OTAPS_EventCb_T sp_otapsCbRoutine;               // Callback routine for BLE OTAPS (Over The Air Programming Service) events.
static uint8_t      s_otapsState;                           // Current state of the OTAPS.
static uint8_t      *sp_otapsPktBuf;                        // Pointer to the OTAPS packet buffer.
static uint8_t      s_otapsAesIv[16];                       // Initialization vector for AES encryption used in OTAPS.
static uint8_t      s_otapsAesKey[16];                      // AES encryption key used in OTAPS.
static uint8_t      *sp_otapsPlainBuf;                      // Pointer to the buffer for plain data before encryption in OTAPS.
static uint8_t      s_otapsCccdStatus;                      // Status of the Client Characteristic Configuration Descriptor (CCCD) for OTAPS.
static uint8_t      s_otapsImgType;                         // Image type being updated via OTAPS.
static uint32_t     s_otapsUpdateOffset;                    // Current offset in the update process for OTAPS.
static uint16_t     s_otapsPktBufIdx;                       // Index to the current position in the OTAPS packet buffer.
static uint32_t     s_otapsImgReceieved;                    // Amount of image data received so far via OTAPS.
static uint32_t     s_otapsPktTotal;                        // Total number of packets expected for the current OTAPS operation.
static struct 
{
    uint32_t        fwSize;                                 // Size of the firmware image 
} s_otapsImgSize;
static uint16_t     s_otapsConnHandle;                      // Connection handle for the current OTAPS session.
static MW_AES_Ctx_T *sp_otapsAes;                           // Pointer to the AES context used for encryption in OTAPS.
static uint8_t      s_otapsEnc;                             // Flag indicating if encryption is enabled for OTAPS.
static uint8_t      s_otapsFileType;                        // Type of file being transferred via OTAPS.
static BLE_OTAPS_RetryList_T s_otapsRetry[BLE_GAP_MAX_LINK_NBR]; // Retry list for OTAPS, one entry per possible BLE connection.
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Gets the size of the update request based on supported image types.
 *
 * @retval Size of the update request in bytes.
 */
static uint16_t ble_otaps_GetUpdateReqSize(void)
{
    uint16_t sz = 0;
    uint8_t suppType = BLE_OTAPS_FEATURE_SUPP_IMG_TYPE;

    if((suppType & BLE_OTAPS_IMG_TYPE_FW) != 0U)
    {
        uint8_t suppFeat = BLE_OTAPS_FEATURE_FW_EXT_FEATURE;

        sz = BLE_OTAPS_UPDATE_REQ_BASIC_SIZE;
        if ( (suppFeat & BLE_OTAPS_FW_FEATURE_MASK1) != 0U)
        {
            sz += BLE_OTAPS_UPDATE_REQ_FW_MASK1_SIZE;
        }
    }

    return sz;
}


/**
 * @brief Retrieves the retry list entry for a given connection handle.
 *
 * @param connHandle The connection handle to search for in the retry list.
 * 
 * @retval Pointer to the retry list entry, or NULL if not found.
 */
static BLE_OTAPS_RetryList_T * ble_otaps_GetRetryListByHandle(uint16_t connHandle)
{
    uint8_t i;

    for(i=0; i<BLE_GAP_MAX_LINK_NBR;i++)
    {
        if (s_otapsRetry[i].connHandle == connHandle)
        {
            return &s_otapsRetry[i];
        }
    }

    return NULL;
}


/**
 * @brief Check if an OTAPS procedure is currently in progress.
 *
 * @retval True if a procedure is in progress, false otherwise.
 */
static bool ble_otaps_IsProcInProgress(void)
{
    return (s_otapsState > BLE_OTAPS_STATE_CCCD_ENABLED
        && s_otapsState < BLE_OTAPS_STATE_COMPLETE);
}


/**
 * @brief Convert MBA error codes to OTAPS result codes.
 *
 * @param mbaErr The MBA error code to convert.
 * 
 * @retval The corresponding OTAPS result code.
 */
static uint8_t ble_otaps_MbaErrToResult(uint16_t mbaErr)
{
    uint8_t result;
    switch (mbaErr)
    {
        case MBA_RES_SUCCESS:
            result = BLE_OTAPS_RESULT_SUCCESS;
        break;
        case MBA_RES_FAIL:
        case MBA_RES_OOM:
            result =  BLE_OTAPS_RESULT_OPERATION_FAILED;
        break;
        case MBA_RES_INVALID_PARA:
            result = BLE_OTAPS_RESULT_INVALID_PARA;
        break;
        case MBA_RES_BAD_STATE:
            result = BLE_OTAPS_RESULT_INVALID_STATE;
        break;
        default:
            result = BLE_OTAPS_RESULT_OPERATION_FAILED;
        break;
    }
    return result;
}


/**
 * @brief Frees all allocated buffers used in the OTAPS process. */
static void ble_otaps_FreeBuf(void)
{
    if (sp_otapsPktBuf != NULL)
    {
        OSAL_Free(sp_otapsPktBuf); 
        sp_otapsPktBuf = NULL;
    }

    if (sp_otapsPlainBuf != NULL)
    {
        OSAL_Free(sp_otapsPlainBuf);
        sp_otapsPlainBuf = NULL;
    }

    if (sp_otapsAes != NULL)
    {
        OSAL_Free(sp_otapsAes);
        sp_otapsAes = NULL;
    }
}


/**
 * @brief Stops the current OTAPS procedure and reset related state variables. */
static void ble_otaps_StopProc(void)
{
    s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;
    
    s_otapsCccdStatus = 0x00;
    
    s_otapsConnHandle = BLE_OTAPS_INVALID_CONN_HANDLE;
}


/**
 * @brief Convey an OTAPS event to the registered callback function.
 *
 * @param eventId The identifier of the event to convey.
 * @param p_eventField Pointer to the event data.
 * @param eventFieldLen Length of the event data in bytes.
 */
static void ble_otaps_FreeRetry(BLE_OTAPS_RetryList_T *p_retry)
{
    if (p_retry->p_pkt != NULL)
    {
        OSAL_Free(p_retry->p_pkt);
        p_retry->p_pkt = NULL;
    }

    if (p_retry->p_gattEvt != NULL)
    {
        OSAL_Free(p_retry->p_gattEvt);
        p_retry->p_gattEvt = NULL;
    }

    p_retry->type = BLE_OTAPS_RETRY_TYPE_NONE;
}


/**
 * @brief Send a response to the peer device for a given request.
 *
 * @param p_retry Pointer to the retry list entry where the response packet is stored.
 * @param reqOpcode The request operation code to which this response corresponds.
 * @param result The result code to be sent in the response.
 * 
 * @retval MBA result code indicating the status of the response transmission.
 */
static void ble_otaps_ConveyEvent(BLE_OTAPS_EventId_T eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{
    if (sp_otapsCbRoutine != NULL)
    {
        BLE_OTAPS_Event_T evtPara;

        evtPara.eventId = eventId;
        (void)memcpy((uint8_t *)&evtPara.eventField, p_eventField, eventFieldLen);
        sp_otapsCbRoutine(&evtPara);
    }
}


/**
 * @brief Send a data response to the peer device.
 *
 * @param p_retry Pointer to the retry list entry where the data response packet is stored.
 * @param result The result code to be sent in the data response.
 * 
 * @retval MBA result code indicating the status of the data response transmission.
 */
static uint16_t ble_otaps_SendResponse(BLE_OTAPS_RetryList_T *p_retry, uint8_t reqOpcode, uint8_t result)
{
    uint16_t status = MBA_RES_OOM;
    
    p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_retry->p_pkt != NULL)
    {
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charHandle = (uint16_t)BLE_OTAS_HDL_CTRL_VAL;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charLength = 0x03;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[0] = BLE_OTAPS_OP_CODE_RESPONSE;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[1] = reqOpcode;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[2] = result;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->sendType = ATT_HANDLE_VALUE_NTF;

        if (result != BLE_OTAPS_RESULT_SUCCESS)
        {
            ble_otaps_FreeBuf();
        }

        status = GATTS_SendHandleValue(p_retry->connHandle, (GATTS_HandleValueParams_T *)p_retry->p_pkt);

        if (status == MBA_RES_SUCCESS)
        {
            ble_otaps_FreeRetry(p_retry);
        }
        else
        {
            p_retry->type = BLE_OTAPS_RETRY_TYPE_NTF;
        }
    }

    return status;
}


/**
 * @brief Send a data response for the BLE Over-The-Air Programming Service (OTAPS).
 *
 * @param p_retry Pointer to the retry list structure.
 * @param result Result to be sent as part of the response.
 * 
 * @retval MBA_RES_OOM if memory allocation failed, otherwise the result of the GATTS_SendHandleValue function.
 */
static uint16_t ble_otaps_SendDataResponse(BLE_OTAPS_RetryList_T *p_retry, uint8_t result)
{
    uint16_t status = MBA_RES_OOM;

    p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_retry->p_pkt != NULL)
    {

        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charHandle = (uint16_t)BLE_OTAS_HDL_DATA_VAL;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charLength = 0x01;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[0] = result;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->sendType = ATT_HANDLE_VALUE_NTF;

        if (result != BLE_OTAPS_RESULT_SUCCESS)
        {
            ble_otaps_FreeBuf();
        }
        
        status = GATTS_SendHandleValue(s_otapsConnHandle, (GATTS_HandleValueParams_T *)p_retry->p_pkt);
   
        if (status == MBA_RES_SUCCESS)
        {
            ble_otaps_FreeRetry(p_retry);
        }
        else
        {
           p_retry->type = BLE_OTAPS_RETRY_TYPE_NTF;
        }
   }
   
   return status;

}


/**
 * @brief Send an update request response for the BLE OTAPS.
 *
 * @param p_retry Pointer to the retry list structure.
 * @param result Result to be sent as part of the response.
 * @param p_devInfo Pointer to the device information structure.
 * 
 * @retval MBA_RES_OOM if memory allocation failed, otherwise the result of the GATTS_SendHandleValue function.
 */
static uint16_t ble_otaps_SendUpdateReqResponse(BLE_OTAPS_RetryList_T *p_retry, uint8_t result, BLE_OTAPS_DevInfo_T * p_devInfo)
{
    uint16_t status = MBA_RES_OOM;
        
    p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_retry->p_pkt != NULL)
    {
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charHandle = (uint16_t)BLE_OTAS_HDL_CTRL_VAL;

        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charLength = 0x03U + BLE_OTAPS_UPDATE_RSP_SIZE;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[0] = BLE_OTAPS_OP_CODE_RESPONSE;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[1] = BLE_OTAPS_OP_CODE_UPDATE_REQUEST;
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[2] = result;

        if (result == BLE_OTAPS_RESULT_SUCCESS)
        {
            U16_TO_BUF_LE(&((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[3], BLE_OTAPS_MTU_SIZE);
            (void)memset(&((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[5], 0x00, BLE_OTAPS_START_IDX_LEN);
            U32_TO_BUF_LE(&((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[5U + BLE_OTAPS_START_IDX_LEN], p_devInfo->fwImageVer);
        }
        else
        {
            (void)memset(&((GATTS_HandleValueParams_T *)p_retry->p_pkt)->charValue[3], 0x00, BLE_OTAPS_UPDATE_RSP_SIZE);
        }
        
        ((GATTS_HandleValueParams_T *)p_retry->p_pkt)->sendType = ATT_HANDLE_VALUE_NTF;

        status = GATTS_SendHandleValue(p_retry->connHandle, (GATTS_HandleValueParams_T *)p_retry->p_pkt);

        if (status == MBA_RES_SUCCESS)
        {
            ble_otaps_FreeRetry(p_retry);
        }
        else
        {
            p_retry->type = BLE_OTAPS_RETRY_TYPE_NTF;
        }
    }
    
    return status;

}


/**
 * @brief Update the image for the BLE OTAPS.
 *
 * @param len Length of the data to be updated.
 * 
 * @retval MBA_RES_SUCCESS if the update is successful, otherwise an error code.
 */
static uint16_t ble_otaps_UpdateImage(uint16_t len)
{
    uint16_t err;
    uint8_t *p_buf;
    uint16_t updateLen, pktOffset;

    if (s_otapsFileType == BLE_OTAPS_IMG_FILE_TYPE_APP)
	{
        return MBA_RES_SUCCESS;
	}

    err = MBA_RES_SUCCESS;
    pktOffset = 0;
    while (pktOffset < len)
    {
        updateLen = (len - pktOffset > BLE_OTAPS_IMG_BUF_SIZE) ? BLE_OTAPS_IMG_BUF_SIZE : len - pktOffset;

        if (s_otapsEnc == BLE_OTAPS_ENC_NONE)
        {
            (void)memcpy(sp_otapsPlainBuf, sp_otapsPktBuf + pktOffset, updateLen);
        }
        else
        {
            err = MW_AES_AesCbcDecrypt(sp_otapsAes, updateLen, sp_otapsPlainBuf, sp_otapsPktBuf + pktOffset);
        }

        if (err == MBA_RES_SUCCESS)
        {
            p_buf = sp_otapsPlainBuf;

            switch (s_otapsImgType)
            {
                case BLE_OTAPS_IMG_TYPE_FW:
                    err = MW_DFU_FwImageUpdate(updateLen, p_buf);
                break;
                default:
                {
                    //Do nothing
                }
                break;
            }

        } 

        if (err == MBA_RES_SUCCESS)
        {
            pktOffset += updateLen;
            s_otapsUpdateOffset += updateLen;
        }
        else
        {
            break;
        }
    }
    
    return err;
}


/**
 * @brief Process an update request for the BLE OTAPS.
 *
 * @param p_retry Pointer to the retry list structure.
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otaps_ProcUpdateRequest(BLE_OTAPS_RetryList_T *p_retry, GATT_Event_T *p_event)
{
    bool isReqFileType;

    if (ble_otaps_IsProcInProgress())
    {
        (void)ble_otaps_SendUpdateReqResponse(p_retry, BLE_OTAPS_RESULT_INVALID_STATE, NULL);
        return;
    }

    if (p_event->eventField.onWrite.writeDataLength - 1U == ble_otaps_GetUpdateReqSize())
    {
        isReqFileType = true;
    }
    else if (p_event->eventField.onWrite.writeDataLength - 1U == BLE_OTAPS_UPDATE_REQ_BASIC_SIZE)
    {
        isReqFileType = false;
    }
    else
    {
        (void)ble_otaps_SendUpdateReqResponse(p_retry, BLE_OTAPS_RESULT_NOT_SUPPORTED, NULL);
        return;
    }
        
    {
        uint16_t err;
        uint8_t result;
        uint8_t * p_value = p_event->eventField.onWrite.writeValue + 1;
        MW_DFU_Info_T dfu;
        BLE_OTAPS_EvtUpdateReq_T evtUpReq;

        
        STREAM_LE_TO_U32(&dfu.fwImageSize, &p_value);
        STREAM_LE_TO_U32(&evtUpReq.fwImageId, &p_value);
        STREAM_LE_TO_U32(&evtUpReq.fwImageVer, &p_value);
        STREAM_TO_U8(&s_otapsEnc, &p_value);
        if (isReqFileType)
        {
            STREAM_LE_TO_U16(&evtUpReq.fwImageChksum, &p_value);
            STREAM_TO_U8(&s_otapsFileType, &p_value);
            STREAM_LE_TO_U16(&evtUpReq.fwImageCrc16, &p_value);
        }
        else
        {
            evtUpReq.fwImageChksum = 0;
            s_otapsFileType = BLE_OTAPS_IMG_FILE_TYPE_INT;
            evtUpReq.fwImageCrc16 = 0;
        }

        if (s_otapsEnc > BLE_OTAPS_ENC_AES_CBC || s_otapsFileType > BLE_OTAPS_IMG_FILE_TYPE_EXT)
        {
            (void)ble_otaps_SendUpdateReqResponse(p_retry, BLE_OTAPS_RESULT_NOT_SUPPORTED, NULL);
            return;
        }

        if (s_otapsFileType != BLE_OTAPS_IMG_FILE_TYPE_APP)
        {
            dfu.fwFlashType = s_otapsFileType;
            err = MW_DFU_Config(&dfu);
            result = ble_otaps_MbaErrToResult(err);
        }
        else
        {
           result = BLE_OTAPS_RESULT_SUCCESS;
        }

        if (result == BLE_OTAPS_RESULT_SUCCESS)
        {
            evtUpReq.connHandle = p_event->eventField.onWrite.connHandle;

            evtUpReq.fwImageSize = dfu.fwImageSize;
            if (isReqFileType)
            {
                evtUpReq.fwImageFileType = s_otapsFileType;
            }
            else
            {
                evtUpReq.fwImageFileType = BLE_OTAPS_IMG_FILE_TYPE_UNKNOWN;
            }
            
            s_otapsImgSize.fwSize = dfu.fwImageSize;

            if (s_otapsFileType == BLE_OTAPS_IMG_FILE_TYPE_APP)
			{
                s_otapsEnc = BLE_OTAPS_ENC_NONE;
			}

            s_otapsState = BLE_OTAPS_STATE_WAIT_REQ_RSP;
            
            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATE_REQ, (uint8_t *)&evtUpReq, (uint8_t)sizeof(evtUpReq));
        }
        else
        {
            err = ble_otaps_SendUpdateReqResponse(p_retry, result, NULL);
        }
    }
}


/**
 * @brief Process an update start for the BLE OTAPS.
 *
 * @param p_retry Pointer to the retry list structure.
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otaps_ProcUpdateStart(BLE_OTAPS_RetryList_T *p_retry, GATT_Event_T *p_event)
{
    uint16_t err;
    uint8_t result = BLE_OTAPS_RESULT_NOT_SUPPORTED;

    if (s_otapsState !=  BLE_OTAPS_STATE_UPDATE_REQ)
    {
        (void)ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_START, BLE_OTAPS_RESULT_INVALID_STATE);
        return;
    }
        
    if (p_event->eventField.onWrite.writeDataLength - 1U != BLE_OTAPS_UPDATE_START_SIZE)
    {
        (void)ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_START, BLE_OTAPS_RESULT_NOT_SUPPORTED);
        return;
    }

    s_otapsImgType = p_event->eventField.onWrite.writeValue[1];

    switch (s_otapsImgType)
    {
        case BLE_OTAPS_IMG_TYPE_FW:
        {
            if (s_otapsFileType == BLE_OTAPS_IMG_FILE_TYPE_APP)
			{
                result = BLE_OTAPS_RESULT_SUCCESS;
			}
            else
            {
                err = MW_DFU_FwImageStart();
                result = ble_otaps_MbaErrToResult(err);
            }

            s_otapsPktTotal = s_otapsImgSize.fwSize;
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }

    if (result == BLE_OTAPS_RESULT_SUCCESS)
    {
        sp_otapsPktBuf = OSAL_Malloc(BLE_OTAPS_MTU_SIZE);
        if (s_otapsFileType != BLE_OTAPS_IMG_FILE_TYPE_APP)
		{
            sp_otapsPlainBuf = OSAL_Malloc(BLE_OTAPS_IMG_BUF_SIZE);
		}

        if (s_otapsEnc == BLE_OTAPS_ENC_AES_CBC)
		{
            sp_otapsAes = OSAL_Malloc(sizeof(MW_AES_Ctx_T));
		}
        if (sp_otapsPktBuf == NULL || (s_otapsFileType != BLE_OTAPS_IMG_FILE_TYPE_APP && sp_otapsPlainBuf == NULL)
            || (s_otapsEnc == BLE_OTAPS_ENC_AES_CBC && sp_otapsAes == NULL)) 
        {
            result = BLE_OTAPS_RESULT_OPERATION_FAILED;
        }
        else
        {
            BLE_OTAPS_EvtStartInd_T evtInit;

            evtInit.imageType = s_otapsImgType;

            s_otapsState = BLE_OTAPS_STATE_UPDATE_START;
            s_otapsUpdateOffset = 0;
            s_otapsPktBufIdx = 0;
            s_otapsImgReceieved = 0;

            if (s_otapsEnc == BLE_OTAPS_ENC_AES_CBC)
            {
                err = MW_AES_CbcDecryptInit(sp_otapsAes, s_otapsAesKey, s_otapsAesIv);
                result = ble_otaps_MbaErrToResult(err);
            }

            if (result == BLE_OTAPS_RESULT_SUCCESS)
			{
                ble_otaps_ConveyEvent(BLE_OTAPS_EVT_START_IND, (uint8_t *)&evtInit , (uint8_t)sizeof(evtInit));
			}
        }
    }

    err = ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_START, result);
}


/**
 * @brief Callback function for handling control point write operations.
 * 
 * This function processes the write operations on the OTA control point characteristic.
 * It handles different op codes and invokes the corresponding procedures.
 * 
 * @param p_retry Pointer to the retry list structure.
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otaps_OnCtrlPtWrite(BLE_OTAPS_RetryList_T *p_retry, GATT_Event_T *p_event)
{
    switch (p_event->eventField.onWrite.writeValue[0])
    {
        case BLE_OTAPS_OP_CODE_UPDATE_REQUEST:
            ble_otaps_ProcUpdateRequest(p_retry, p_event);
            break;
        case BLE_OTAPS_OP_CODE_UPDATE_START:
            ble_otaps_ProcUpdateStart(p_retry, p_event);
            break;
        case BLE_OTAPS_OP_CODE_UPDATE_COMPLETE:
        {
            BLE_OTAPS_EvtCompleteInd_T evtComplete;

            if (s_otapsState == BLE_OTAPS_STATE_COMPLETE)
            {
                (void)ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, BLE_OTAPS_RESULT_INVALID_STATE);
                return;
            }

            if (s_otapsState >= BLE_OTAPS_STATE_UPDATE_START 
                && s_otapsImgReceieved >= s_otapsPktTotal)
            {
                evtComplete.errStatus = false;

                s_otapsState = BLE_OTAPS_STATE_WAIT_COMP_RSP;
            }
            else
            {
                ble_otaps_FreeBuf();
            
                evtComplete.errStatus = true;
                
                (void)ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, BLE_OTAPS_RESULT_SUCCESS);

                s_otapsState = BLE_OTAPS_STATE_COMPLETE;
            }

            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_COMPLETE_IND, (uint8_t *)&evtComplete , (uint8_t)sizeof(evtComplete));
        }
        break;
        case BLE_OTAPS_OP_CODE_RESET_REQUEST:
            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_RESET_IND, NULL, 0);
            break;
        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Handles incoming packets during the OTA process.
 * 
 * This function manages the reception of data packets during the OTA update process.
 * It appends received data to a buffer and triggers image update when necessary.
 * 
 * @param p_retry Pointer to the retry list structure.
 * @param p_event Pointer to the GATT event structure.
 */
static void ble_otaps_PacketHandler(BLE_OTAPS_RetryList_T *p_retry, GATT_Event_T *p_event)
{
    uint16_t err;
    uint8_t result;
    BLE_OTAPS_EvtUpdatingInd_T evtUpdate;

    if (s_otapsState != BLE_OTAPS_STATE_UPDATE_START)
    {
        (void)ble_otaps_SendDataResponse(p_retry, BLE_OTAPS_RESULT_INVALID_STATE);
        return;
    }

    if (p_event->eventField.onWrite.writeDataLength + s_otapsPktBufIdx > BLE_OTAPS_MTU_SIZE)
    {
        (void)ble_otaps_SendDataResponse(p_retry, BLE_OTAPS_RESULT_OPERATION_FAILED);
        return;
    }


    evtUpdate.length = 0;
    (void)memcpy(sp_otapsPktBuf + s_otapsPktBufIdx, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
    s_otapsPktBufIdx += p_event->eventField.onWrite.writeDataLength;

    if (p_event->eventField.onWrite.writeDataLength + s_otapsImgReceieved >= s_otapsPktTotal 
        || s_otapsPktBufIdx == BLE_OTAPS_MTU_SIZE)
    {
        err = ble_otaps_UpdateImage(s_otapsPktBufIdx);

        result = ble_otaps_MbaErrToResult(err);

        if (result != BLE_OTAPS_RESULT_SUCCESS)
        {
            s_otapsState = BLE_OTAPS_STATE_UPDATE_ERROR;
            err = ble_otaps_SendDataResponse(p_retry, result);
            return;
        }

        evtUpdate.length += s_otapsPktBufIdx;
        s_otapsPktBufIdx = 0;
    }

    if (evtUpdate.length != 0U)
    {
        if (s_otapsFileType == BLE_OTAPS_IMG_FILE_TYPE_APP)
        {
            evtUpdate.p_fragment = sp_otapsPktBuf;

            s_otapsState = BLE_OTAPS_STATE_WAIT_UPDATE_RSP;
            
            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATING_REQ, (uint8_t *)&evtUpdate , (uint8_t)sizeof(evtUpdate));
        }
        else
        {
            evtUpdate.p_fragment = NULL;

            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATING_IND, (uint8_t *)&evtUpdate , (uint8_t)sizeof(evtUpdate));

            err = ble_otaps_SendDataResponse(p_retry, result);
        }
    }

    s_otapsImgReceieved += p_event->eventField.onWrite.writeDataLength;
}


/**
 * @brief Disconnects the OTA session.
 * 
 * This function stops the OTA process and frees any allocated buffers.
 */
static void ble_otaps_Disconnect(void)
{
    ble_otaps_StopProc();
        
    ble_otaps_FreeBuf();
}


/**
 * @brief Processes GAP events related to the OTA service.
 * 
 * This function handles GAP events such as connection, disconnection, and transmission buffer availability.
 * It manages the retry list based on the events and performs necessary actions.
 * 
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_otaps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_OTAPS_RetryList_T    *p_retry;

    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if ((p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS))
            {
                p_retry=ble_otaps_GetRetryListByHandle(0);
                if(p_retry==NULL)
                {
                    ble_otaps_ConveyEvent(BLE_OTAPS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
                    return;
                }

                p_retry->connHandle=p_event->eventField.evtConnect.connHandle;
            }
        }
        break;
        case BLE_GAP_EVT_DISCONNECTED:
        {
            if (s_otapsConnHandle == p_event->eventField.evtDisconnect.connHandle)
            {
                ble_otaps_Disconnect();
            }

            p_retry=ble_otaps_GetRetryListByHandle(p_event->eventField.evtDisconnect.connHandle);
            if(p_retry!=NULL)
            {
                ble_otaps_FreeRetry(p_retry);
                p_retry->connHandle = 0;
            }
        }
        break;
        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            p_retry=ble_otaps_GetRetryListByHandle(p_event->eventField.evtTxBufAvailable.connHandle);
        
            if ((p_retry!= NULL) && (p_retry->p_pkt != NULL ))
            {
                switch (p_retry->type)
                {
                    case BLE_OTAPS_RETRY_TYPE_ERROR:
                    {
                        if (MBA_RES_SUCCESS == GATTS_SendErrorResponse(p_retry->connHandle, (GATTS_SendErrRespParams_T *)p_retry->p_pkt))
                        {
                            ble_otaps_FreeRetry(p_retry);
                        }
                    }
                    break;
                    case BLE_OTAPS_RETRY_TYPE_READ:
                    {
                        if (MBA_RES_SUCCESS == GATTS_SendReadResponse(p_retry->connHandle, (GATTS_SendReadRespParams_T *)p_retry->p_pkt))
                        {
                            ble_otaps_FreeRetry(p_retry);
                        }
                    }
                    break;
                    case BLE_OTAPS_RETRY_TYPE_WRITE:
                    {
                        if (MBA_RES_SUCCESS == GATTS_SendWriteResponse(p_retry->connHandle, (GATTS_SendWriteRespParams_T *)p_retry->p_pkt))
                        {
                            OSAL_Free(p_retry->p_pkt);
                            p_retry->p_pkt = NULL;
                            p_retry->type = BLE_OTAPS_RETRY_TYPE_NONE;

                            if (p_retry->p_gattEvt != NULL)
                            {
                                ble_otaps_OnCtrlPtWrite(p_retry, p_retry->p_gattEvt);

                                OSAL_Free(p_retry->p_gattEvt);
                                p_retry->p_gattEvt = NULL;
                            }
                        }
                    }
                    break;
                    case BLE_OTAPS_RETRY_TYPE_NTF:
                    {
                        if (MBA_RES_SUCCESS == GATTS_SendHandleValue(p_retry->connHandle, (GATTS_HandleValueParams_T *)p_retry->p_pkt))
                        {
                            ble_otaps_FreeRetry(p_retry);
                        }
                    }
                    break;

                    default:
                    {
                        //Do nothing
                    }
                    break;

                }
            }
        }
        break;
        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief GATT event handler for the BLE Over-The-Air Programming Service (OTAPS).
 *
 * This function handles various GATT events such as write and read requests
 * related to the OTAPS and performs appropriate actions based on the event type.
 *
 * @param[in] p_event Pointer to the GATT event structure containing details about the event.
 */
static void ble_otaps_GattEventHandler(GATT_Event_T *p_event)
{
    BLE_OTAPS_RetryList_T    *p_retry;

    switch (p_event->eventId)
    {
        case GATTS_EVT_WRITE:
        {
            uint8_t errCode = 0;

            p_retry=ble_otaps_GetRetryListByHandle(p_event->eventField.onWrite.connHandle);

            if (p_retry == NULL || p_retry->p_pkt != NULL)
            {
                break;
            }

            if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_DATA_VAL)
            {
                /* No need to send response. */
                if (p_event->eventField.onWrite.connHandle == s_otapsConnHandle)
                {
                    ble_otaps_PacketHandler(p_retry, p_event);
                }
                return;
            }

            if ((p_event->eventField.onWrite.attrHandle != (uint16_t)BLE_OTAS_HDL_CTRL_CCCD) &&
                (p_event->eventField.onWrite.attrHandle != (uint16_t)BLE_OTAS_HDL_CTRL_VAL) &&
                (p_event->eventField.onWrite.attrHandle != (uint16_t)BLE_OTAS_HDL_DATA_CCCD))
            {
                /* Only process write operations on SETTING_MANUAL_WRITE_RSP in OTA. */
                return;
            }


            if (sp_otapsCbRoutine == NULL)
            {
                errCode = ATT_ERR_APPLICATION_ERROR;
            }
            else if (p_event->eventField.onWrite.writeType != ATT_WRITE_REQ)
            {
                errCode = ATT_ERR_REQUEST_NOT_SUPPORT;
            }
            else
            {
				//Shall not enter here
            }

            if (errCode==0U)
            {
                if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_CCCD ||
                    p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_DATA_CCCD)
                {
                    uint16_t cccd;

                    BUF_LE_TO_U16(&cccd, p_event->eventField.onWrite.writeValue);

                    if (cccd == NOTIFICATION)
                    {
                        if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_CCCD)
                        {
                            s_otapsCccdStatus |= (BLE_OTAPS_CCCD_CTRL_ON);
                        }
                        else
                        {
                            s_otapsCccdStatus |= (BLE_OTAPS_CCCD_DATA_ON);
                        }

                        if ((s_otapsCccdStatus & (BLE_OTAPS_CCCD_CTRL_ON|BLE_OTAPS_CCCD_CTRL_ON))
                            == (BLE_OTAPS_CCCD_CTRL_ON|BLE_OTAPS_CCCD_CTRL_ON))
                        {
                            s_otapsState = BLE_OTAPS_STATE_CCCD_ENABLED;
                        }

                        errCode = 0x00;
                    }
                    else if (cccd == 0x0000U)
                    {
                        if (s_otapsConnHandle == BLE_OTAPS_INVALID_CONN_HANDLE || p_event->eventField.onWrite.connHandle == s_otapsConnHandle)
                        {
                            if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_CCCD)
                            {
                                s_otapsCccdStatus &= ~((uint8_t)BLE_OTAPS_CCCD_CTRL_ON);
                            }
                            else
                            {
                                s_otapsCccdStatus &= ~((uint8_t)BLE_OTAPS_CCCD_DATA_ON);
                            }
                        
                            s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;
                            errCode = 0x00;
                        }
                        else
                        {
                            errCode = ATT_ERR_WRITE_NOT_PERMITTED;
                        }
                    }
                    else
                    {
                        errCode = ATT_ERR_APPLICATION_ERROR;
                    }
                }
                else if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_VAL)
                {
                    if (s_otapsState == BLE_OTAPS_STATE_CCCD_DISABLED
                        || (s_otapsConnHandle != BLE_OTAPS_INVALID_CONN_HANDLE && p_event->eventField.onWrite.connHandle != s_otapsConnHandle)
                        || (s_otapsConnHandle == BLE_OTAPS_INVALID_CONN_HANDLE && p_event->eventField.onWrite.writeValue[0] != BLE_OTAPS_OP_CODE_UPDATE_REQUEST))
					{
                        errCode = ATT_ERR_WRITE_NOT_PERMITTED;
					}
                }
				else
				{
					//Shall not enter here
				}
            }

            if (errCode==0U)
            {
                p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
                if (p_retry->p_pkt != NULL)
                {
                    ((GATTS_SendWriteRespParams_T *)p_retry->p_pkt)->responseType = ATT_WRITE_RSP;
                    if (GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, (GATTS_SendWriteRespParams_T *)p_retry->p_pkt) == MBA_RES_SUCCESS)
                    {
                        ble_otaps_FreeRetry(p_retry);

                        /* It will take long time to validate so we must send write response before calling BLE_OTAPS_OnCtrlPtWrite */
                        if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_VAL)
                        {
                            ble_otaps_OnCtrlPtWrite(p_retry, p_event);
                        }
                    }
                    else if (p_event->eventField.onWrite.attrHandle == (uint16_t)BLE_OTAS_HDL_CTRL_VAL)
                    {
                        uint16_t validEvtLen;

                        validEvtLen = (uint32_t)p_event->eventField.onWrite.writeValue - (uint32_t)p_event + p_event->eventField.onWrite.writeDataLength;
                    
                        p_retry->p_gattEvt = OSAL_Malloc(validEvtLen);
                        if (p_retry->p_gattEvt != NULL)
                        {
                            (void)memcpy(p_retry->p_gattEvt, p_event, validEvtLen);
                        }

                        p_retry->type = BLE_OTAPS_RETRY_TYPE_WRITE;
                    }
                    else
                    {
						//Shall not enter here
                    }
                }


            }
            else
            {
                p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
                if (p_retry->p_pkt != NULL)
                {
                    ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->reqOpcode = p_event->eventField.onWrite.writeType;
                    ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->attrHandle = p_event->eventField.onWrite.attrHandle;
                    ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->errorCode = errCode;
                    if (GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, (GATTS_SendErrRespParams_T *)p_retry->p_pkt) == MBA_RES_SUCCESS)
                    {
                        ble_otaps_FreeRetry(p_retry);
                    }
                    else
                    {
                        p_retry->type = BLE_OTAPS_RETRY_TYPE_ERROR;
                    }
                }
            }
        }
        break;

        case GATTS_EVT_READ:
        {
            if (p_event->eventField.onRead.attrHandle == (uint16_t)BLE_OTAS_HDL_FEATURE_VAL)
            {
                uint8_t errCode = 0;

                p_retry=ble_otaps_GetRetryListByHandle(p_event->eventField.onRead.connHandle);
                
                if (p_retry == NULL || p_retry->p_pkt != NULL)
                {
                    break;
                }

                if (sp_otapsCbRoutine == NULL)
                {
                    errCode = ATT_ERR_APPLICATION_ERROR;
                }
                else if (s_otapsState == BLE_OTAPS_STATE_CCCD_DISABLED)
                {
                    errCode = ATT_ERR_READ_NOT_PERMITTED;
                }
                else if (p_event->eventField.onRead.readType != ATT_READ_REQ)
                {
                    errCode = ATT_ERR_REQUEST_NOT_SUPPORT;
                }
                else
                {
					//Shall not enter here
                }


                if (errCode==0U)
                {
                    p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_SendReadRespParams_T));
                    if (p_retry->p_pkt != NULL)
                    {
                        ((GATTS_SendReadRespParams_T *)p_retry->p_pkt)->attrLength = 0x02;
                        ((GATTS_SendReadRespParams_T *)p_retry->p_pkt)->responseType = ATT_READ_RSP;
                        ((GATTS_SendReadRespParams_T *)p_retry->p_pkt)->attrValue[0] = BLE_OTAPS_FEATURE_SUPP_IMG_TYPE;
                        ((GATTS_SendReadRespParams_T *)p_retry->p_pkt)->attrValue[1] = BLE_OTAPS_FEATURE_FW_EXT_FEATURE;
                        if (GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, (GATTS_SendReadRespParams_T *)p_retry->p_pkt) == MBA_RES_SUCCESS)
                        {
                            ble_otaps_FreeRetry(p_retry);
                        }
                        else
                        {
                            p_retry->type = BLE_OTAPS_RETRY_TYPE_READ;
                        }
                    }
                }
                else
                {
                    p_retry->p_pkt = OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
                    if (p_retry->p_pkt != NULL)
                    {
                        ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->reqOpcode = p_event->eventField.onRead.readType;
                        ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->attrHandle = p_event->eventField.onRead.attrHandle;
                        ((GATTS_SendErrRespParams_T *)p_retry->p_pkt)->errorCode = errCode;
                        if (GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, (GATTS_SendErrRespParams_T *)p_retry->p_pkt) == MBA_RES_SUCCESS)
                        {
                            ble_otaps_FreeRetry(p_retry);
                        }
                        else
                        {
                            p_retry->type = BLE_OTAPS_RETRY_TYPE_ERROR;
                        }
                    }
                }
            }
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Initializes the BLE Over-The-Air Programming Service (OTAPS) profile and service.
 *
 * @retval MBA_RES_SUCCESS            Initialization successful.
 *
 */
uint16_t BLE_OTAPS_Init(void)
{
	sp_otapsCbRoutine = NULL;
    sp_otapsPktBuf = NULL;
    sp_otapsPlainBuf = NULL;
    sp_otapsAes = NULL;

    s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;

    s_otapsConnHandle = BLE_OTAPS_INVALID_CONN_HANDLE;

    s_otapsCccdStatus = 0x00;

    (void)memset(s_otapsRetry, 0x00, sizeof(s_otapsRetry));

    return BLE_OTAS_Add();
}


/**
 * @brief Registers a callback function for BLE OTA profile events.
 *
 * @param[in] bleOtapsRoutine         Callback function to handle BLE OTA profile events.
 *
 */
void BLE_OTAPS_EventRegister(BLE_OTAPS_EventCb_T bleOtapsRoutine)
{
    sp_otapsCbRoutine = bleOtapsRoutine;
}


/**
 * @brief Stops the ongoing BLE OTA firmware update procedure.
 *
 * @retval MBA_RES_SUCCESS            OTA procedure successfully stopped.
 */
uint16_t BLE_OTAPS_UpdateStop(void)
{
    if (ble_otaps_IsProcInProgress())
    {
        s_otapsState = BLE_OTAPS_STATE_CCCD_ENABLED;

        ble_otaps_FreeBuf();
    }

    return MBA_RES_SUCCESS;
}


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
uint16_t BLE_OTAPS_SetEncrytionInfo(uint8_t * p_iv, uint8_t * p_key)
{
    if (ble_otaps_IsProcInProgress()==true)
    {
        return MBA_RES_BAD_STATE;
    }

    (void)memcpy(s_otapsAesIv, p_iv, 16);
    (void)memcpy(s_otapsAesKey, p_key, 16);

    return MBA_RES_SUCCESS;
}


/**
 * @brief Responds to the peer device after receiving a @ref BLE_OTAPS_EVT_UPDATE_REQ event.
 *
 * @note The application must decide whether to allow the start of the BLE OTA update procedure.
 * 
 * @param[in] connHandle             Handle of the connection to send the response.
 * @param[in] isAllow                True to allow the firmware update, false to deny.
 * @param[in] p_devInfo              Pointer to the device information to be sent if isAllow is true; ignored otherwise.
 *
 * @retval MBA_RES_SUCCESS           Response sent successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA      The provided connection handle is not valid.
 * @retval MBA_RES_BAD_STATE         Inappropriate OTA state to call this function.
 *
 */
uint16_t BLE_OTAPS_UpdateResponse(uint16_t connHandle, bool isAllow, BLE_OTAPS_DevInfo_T * p_devInfo)
{
    uint16_t status;
    BLE_OTAPS_RetryList_T *p_retry;

    p_retry = ble_otaps_GetRetryListByHandle(connHandle);

    if (p_retry == NULL)
    {
        return MBA_RES_INVALID_PARA;
    }
    
    if ((s_otapsState != BLE_OTAPS_STATE_WAIT_REQ_RSP) || (p_retry->p_pkt!= NULL))
    {
        return MBA_RES_BAD_STATE;
    }

    if (isAllow)
    {
        s_otapsState = BLE_OTAPS_STATE_UPDATE_REQ;

        s_otapsConnHandle = connHandle;
    }

    status = ble_otaps_SendUpdateReqResponse(p_retry, (isAllow) ? BLE_OTAPS_RESULT_SUCCESS : BLE_OTAPS_RESULT_NOT_SUPPORTED, p_devInfo);

    ble_otaps_FreeRetry(p_retry);

    return status;
}


/**
 * @brief Responds to the peer device after receiving a @ref BLE_OTAPS_EVT_UPDATING_REQ event.
 * 
 * @note The application must handle the update of the fragmented firmware image.
 * 
 * @param[in] success                True if the update is successful, false otherwise.
 *
 * @retval MBA_RES_SUCCESS           Response sent successfully.
 * @retval MBA_RES_OOM               Internal memory allocation failure.
 * @BLE_OTAPS_Init MBA_RES_BAD_STATE         Inappropriate OTA state to call this function.
 *
 */
uint16_t BLE_OTAPS_UpdatingResponse(bool success)
{
    uint16_t status;
    BLE_OTAPS_RetryList_T *p_retry;

    p_retry = ble_otaps_GetRetryListByHandle(s_otapsConnHandle);

    if ((s_otapsState != BLE_OTAPS_STATE_WAIT_UPDATE_RSP)
        || (p_retry == NULL) || (p_retry->p_pkt != NULL))
    {
        return MBA_RES_BAD_STATE;
    }

    s_otapsState = BLE_OTAPS_STATE_UPDATE_START;

    status = ble_otaps_SendDataResponse(p_retry, (success) ? BLE_OTAPS_RESULT_SUCCESS : BLE_OTAPS_RESULT_OPERATION_FAILED);

    ble_otaps_FreeRetry(p_retry);

    return status;
}


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
uint16_t BLE_OTAPS_CompleteResponse(bool success)
{
    uint16_t status;
    BLE_OTAPS_RetryList_T *p_retry;

    p_retry = ble_otaps_GetRetryListByHandle(s_otapsConnHandle);

    if ((s_otapsState != BLE_OTAPS_STATE_WAIT_COMP_RSP)
        || (p_retry == NULL) || (p_retry->p_pkt != NULL) )
    {
        return MBA_RES_BAD_STATE;
    }

    s_otapsState = BLE_OTAPS_STATE_COMPLETE;

    status = ble_otaps_SendResponse(p_retry, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, (success) ? BLE_OTAPS_RESULT_SUCCESS : BLE_OTAPS_RESULT_OPERATION_FAILED);

    ble_otaps_FreeRetry(p_retry);

    return status;
}


/**
 * @brief Handles BLE_Stack events.
 *
 * @note This function should be called by the application when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack event data.
 *
*/
void BLE_OTAPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_otaps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_otaps_GattEventHandler((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

