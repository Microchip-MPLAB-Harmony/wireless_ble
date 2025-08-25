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
    ble_ancs.c

  Summary:
    Implementation of the BLE ANCS profile functions for application developers.

  Description:
    This source file provides the implementation of the Bluetooth Low Energy
    (BLE) Apple Notification Center Service (ANCS) profile. It is designed to
    be used by application developers to facilitate communication with ANCS
    compliant devices, enabling notification-related interactions.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/mw_assert.h"
#include "ble_util/byte_stream.h"
#include "ble_ancs.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_ANCS_NTFY_DATA_LENGTH               (8U)                        // Length of the notification data.
#define BLE_ANCS_NTFY_EVT_ID_INDEX              (0U)                        // Index for Event ID in notification data.
#define BLE_ANCS_NTFY_FLAGS_INDEX               (1U)                        // Index for Flags in notification data.
#define BLE_ANCS_NTFY_CATEGORY_ID_INDEX         (2U)                        // Index for Category ID in notification data.
#define BLE_ANCS_NTFY_CATEGORY_CNT_INDEX        (3U)                        // Index for Category Count in notification data.
#define BLE_ANCS_NTFY_UID_INDEX                 (4U)                        // Index for Notification UID in notification data.

/* Apple Notification Center Service (ANCS) UUIDs in little endian format. */
#define UUID_ANCS_SVC                           0xD0,0x00,0x2D,0x12,0x1E,0x4B,0x0F,0xA4,0x99,0x4E,0xCE,0xB5,0x31,0xF4,0x05,0x79         // ANCS service UUID.
#define UUID_ANCS_CHAR_NOTIFICATION_SOURCE      0xBD,0x1D,0xA2,0x99,0xE6,0x25,0x58,0x8C,0xD9,0x42,0x01,0x63,0x0D,0x12,0xBF,0x9F         // Notification Source characteristic UUID.
#define UUID_ANCS_CHAR_CONTROL_POINT            0xD9,0xD9,0xAA,0xFD,0xBD,0x9B,0x21,0x98,0xA8,0x49,0xE1,0x45,0xF3,0xD8,0xD1,0x69         // Control Point characteristic UUID.
#define UUID_ANCS_CHAR_DATA_SOURCE              0xFB,0x7B,0x7C,0xCE,0x6A,0xB3,0x44,0xBE,0xB5,0x4B,0xD6,0x24,0xE9,0xC6,0xEA,0x22         // Data Source characteristic UUID.

#define BLE_ANCS_MAX_CONN_NBR                   BLE_GAP_MAX_LINK_NBR        // Aligns with the GAP's max link number.

#define BLE_ANCS_EVENT_FLAG_SILENT              0U                          // Silent: Bit 0 (LSB).
#define BLE_ANCS_EVENT_FLAG_IMPORTANT           1U                          // Important: Bit 1.
#define BLE_ANCS_EVENT_FLAG_PREEXISTING         2U                          // Pre-existing: Bit 2.
#define BLE_ANCS_EVENT_FLAG_POSITIVE_ACTION     3U                          // Positive action: Bit 3.
#define BLE_ANCS_EVENT_FLAG_NEGATIVE_ACTION     4U                          // Negative action: Bit 4.

#define BLE_ANCS_RETRY_TYPE_ENABLE_NTFY         0x01U                       // Enable notification retry type.

/* Enumeration for BLE Apple Notification Center Service (ANCS) characteristic indexes. */
typedef enum BLE_ANCS_CharIndex_T
{
    ANCS_INDEX_CHAR_CONTROL_POINT = 0U,                                     // Index for the Control Point characteristic.
    ANCS_INDEX_CHAR_NTFY,                                                   // Index for the Notification Source characteristic.
    ANCS_INDEX_CHAR_NTFY_CCCD,                                              // Index for the Notification Source characteristic's CCCD (Client Characteristic Configuration Descriptor).
    ANCS_INDEX_CHAR_DATA,                                                   // Index for the Data Source characteristic.
    ANCS_INDEX_CHAR_DATA_CCCD,                                              // Index for the Data Source characteristic's CCCD.
    ANCS_INDEX_CHAR_MAX_NUM                                                 // Total number of ANCS characteristics.
}BLE_ANCS_CharIndex_T;
 
/* Enumeration for iOS notification events. */
typedef enum BLE_ANCS_NtfyEvt_T
{
    BLE_ANCS_NTFY_EVT_ADDED = 0x0U,                                         // Notification was added.
    BLE_ANCS_NTFY_EVT_MODIFIED,                                             // Notification was modified.
    BLE_ANCS_NTFY_EVT_REMOVED,                                              // Notification was removed.
    BLE_ANCS_NTFY_EVT_MAX                                                   // Placeholder for the maximum event value.
} BLE_ANCS_NtfyEvt_T;

/* Enumeration for App Attribute IDs in the context of the Get App Attributes command. */
typedef enum BLE_ANCS_AppAttrId_T
{
    BLE_ANCS_APP_ATTR_ID_DISPLAY_NAME = 0x0U,                               // Attribute ID for the app's display name.
    BLE_ANCS_APP_ATTR_ID_MAX                                                // Undefined application attribute ID, used as a placeholder for the maximum value.
} BLE_ANCS_AppAttrId_T;

/* Enumeration for packet order in a sequence of ANCS data packets. */
typedef enum BLE_ANCS_PacketOrder_T
{
    BLE_ANCS_FIRST_PACKET = 0,                                              // Indicates the first packet in a sequence.
    BLE_ANCS_MID_PACKET,                                                    // Indicates a packet in the middle of a sequence.
    BLE_ANCS_LAST_PACKET                                                    // Indicates the last packet in a sequence.
} BLE_ANCS_PacketOrder_T;

/* Enumeration for command IDs used in the ANCS protocol. */
typedef enum BLE_ANCS_CmdId_T
{
    BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR = 0x00U,                              // Command to request notification attributes.
    BLE_ANCS_COMMAND_ID_GET_APP_ATTR,                                       // Command to request notification attributes.
    BLE_ANCS_COMMAND_ID_SET_PERFORM_NTFY_ACTION,                            // Command to perform an action on a notification.
} BLE_ANCS_CmdId_T;

/* Enumeration for the states of the ANCS service. */
typedef enum BLE_ANCS_State_T
{
    BLE_ANCS_STATE_IDLE = 0x00,                                             // ANCS service is idle.
    BLE_ANCS_STATE_CONNECTED                                                // ANCS service is connected to a peer device.
} BLE_ANCS_State_T;
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* Structure representing a list of attributes for a given notification or app. */
typedef struct BLE_ANCS_AttrList_T
{
    uint8_t                     attrId;                                     // Attribute ID (e.g., AppIdentifier, Title, Subtitle, etc.).
    uint16_t                    attrLen;                                    // Length of the attribute data; excess data beyond this length is ignored.
    uint8_t                     *p_attrData;                                // Pointer to the allocated memory for the attribute data.
} BLE_ANCS_AttrList_T;

/* Structure holding decoded notification and application attribute data for BLE Apple Notification Center Service (ANCS). */
typedef struct BLE_ANCS_DecodeAttrs_T
{
    BLE_ANCS_DecodeNtfyAttrs_T  attrNtfyData;                               // Notification attribute data.
    BLE_ANCS_DecodeAppAttrs_T   attrAppData;                                // Application attribute data.
} BLE_ANCS_DecodeAttrs_T;

/* Structure representing a connection list for BLE ANCS. */
typedef struct BLE_ANCS_ConnList_T
{
    uint8_t                     connIndex;                                  // Index of the connection.
    BLE_ANCS_State_T            state;                                      // Current state of the ANCS connection.
    uint16_t                    connHandle;                                 // Connection handle.
    uint16_t                    attMtu;                                     // Attribute MTU size.

    BLE_ANCS_CharIndex_T        enableCccd;                                 // Characteristic index for enabling CCCD.

    uint8_t                     *p_packet;                                  // Pointer to the packet data.
    BLE_ANCS_PacketOrder_T      completePkt;                                // Status of the complete packet.
    uint16_t                    pktLen;                                     // Length of the packet.

    BLE_ANCS_DecodeAttrs_T      attrData;                                   // Decoded attribute data.

    BLE_ANCS_AttrList_T         ntfyAttrList[BLE_ANCS_NTFY_ATTR_ID_MAX];    // List of notification attributes.
    BLE_ANCS_AttrList_T         appAttrList[BLE_ANCS_APP_ATTR_ID_MAX];      // List of application attributes.
    BLE_ANCS_AttrList_T         *p_attrList;                                // Pointer to the current attribute list being processed.
    uint8_t                     retryType;                                  // Type of retry operation.
} BLE_ANCS_ConnList_T;

/* Structure service database and discovery list for BLE ANCS. */
typedef struct BLE_ANCS_ServiceDb_T
{
    BLE_DD_CharList_T ancsCharList[BLE_ANCS_MAX_CONN_NBR];
    BLE_DD_CharInfo_T ancsCharInfoList[BLE_ANCS_MAX_CONN_NBR][ANCS_INDEX_CHAR_MAX_NUM];
    BLE_DD_DiscChar_T ancsDiscCharList[ANCS_INDEX_CHAR_MAX_NUM];
    BLE_DD_DiscChar_T *p_ancsDiscCharList[ANCS_INDEX_CHAR_MAX_NUM];
} BLE_ANCS_ServiceDb_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
// Callback for processing BLE ANCS events.
static BLE_ANCS_EventCb_T       s_bleAncsProcess;
// An array to keep track of the connection list for ANCS.
static BLE_ANCS_ConnList_T      *sp_ancsConnList[BLE_ANCS_MAX_CONN_NBR];
// List of pointers to the discovery information for ANCS characteristics and descriptors.
static BLE_ANCS_ServiceDb_T     *sp_ancsServiceDb;

// UUID for the ANCS service discovery.
static const uint8_t            s_discSvcUuid[] =     {UUID_ANCS_SVC};    

// UUID for the ANCS control point characteristic discovery.
static const ATT_Uuid_T         s_discCharCtrl =      {{UUID_ANCS_CHAR_CONTROL_POINT}, ATT_UUID_LENGTH_16 };

// UUID for the ANCS notification source characteristic discovery.
static const ATT_Uuid_T         s_discCharNtfy =      {{UUID_ANCS_CHAR_NOTIFICATION_SOURCE}, ATT_UUID_LENGTH_16 };

// UUID for the ANCS notification source characteristic's CCCD (Client Characteristic Configuration Descriptor) discovery.
static const ATT_Uuid_T         s_discCharNtfyCccd =  {{UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };

// UUID for the ANCS data source characteristic discovery.
static const ATT_Uuid_T         s_discCharData =      {{UUID_ANCS_CHAR_DATA_SOURCE}, ATT_UUID_LENGTH_16 };

// UUID for the ANCS data source characteristic's CCCD discovery.
static const ATT_Uuid_T         s_discCharDataCccd =  {{UINT16_TO_BYTES(UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)}, ATT_UUID_LENGTH_2 };



// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Conveys an ANCS event to the registered process handler.
 *
 * @param eventId       The identifier of the ANCS event.
 * @param p_eventField  Pointer to the event data.
 * @param eventFieldLen Length of the event data.
 */
static void ble_ancs_ConveyEvent(BLE_ANCS_EventId_T eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{
    if(s_bleAncsProcess != NULL)
    {
        BLE_ANCS_Event_T evtPara;

        evtPara.eventId = eventId;
        (void)memcpy((uint8_t *)&evtPara.eventField, p_eventField, eventFieldLen);
        s_bleAncsProcess(&evtPara);
    }
}


/**
 * @brief Writes data to a GATT characteristic.
 *
 * @param p_conn        Pointer to the connection list structure.
 * @param p_writeParams Pointer to the GATT write parameters structure.
 * @param charHandle    Handle of the characteristic to write to.
 * @param len           Length of the data to write.
 * 
 * @retval uint16_t     Result of the GATT write operation.
 */
static uint16_t ble_ancs_GattWrite(BLE_ANCS_ConnList_T *p_conn, GATTC_WriteParams_T *p_writeParams, 
                            uint16_t charHandle, uint16_t len)
{
    uint16_t ret;

    p_writeParams->charHandle = charHandle;
    p_writeParams->charLength = len;
    p_writeParams->writeType  = ATT_WRITE_REQ;
    p_writeParams->valueOffset  = 0;
    p_writeParams->flags = 0;

    ret= GATTC_Write(p_conn->connHandle, p_writeParams);

    return ret;
}


/**
 * @brief Initializes the resources for a BLE ANCS connection.
 *
 * @param p_conn    Pointer to the ANCS connection list structure.
 */
static void ble_ancs_InitResource(BLE_ANCS_ConnList_T *p_conn)
{
    BLE_ANCS_DecodeNtfyAttrs_T  *p_attrNtfyData= &p_conn->attrData.attrNtfyData;
    
    if (p_conn->p_packet!= NULL )
    {
        OSAL_Free(p_conn->p_packet);
        p_conn->p_packet    = NULL;
    }

    p_conn->completePkt = BLE_ANCS_FIRST_PACKET;
    p_conn->pktLen      = 0;
    
    if (p_attrNtfyData->p_title != NULL)
    {
        OSAL_Free(p_attrNtfyData->p_title);
        p_attrNtfyData->p_title = NULL;
    }
    
    if (p_attrNtfyData->p_subtitle != NULL)
    {
        OSAL_Free(p_attrNtfyData->p_subtitle);
        p_attrNtfyData->p_subtitle = NULL;
    }
    
    if (p_attrNtfyData->p_msg != NULL)
    {
        OSAL_Free(p_attrNtfyData->p_msg);
        p_attrNtfyData->p_msg = NULL;
    }
}


/**
 * @brief Verify the validity of the notification event.
 *
 * This function checks if the notification event ID and category ID are within their valid ranges.
 *
 * @param p_ntfy    Pointer to the notification structure.
 * @param ntfyEvtId Notification event ID to verify.
 * 
 * @retval true if the notification event ID or category ID is out of range, false otherwise.
 */
static bool ble_ancs_VerifyNotification(BLE_ANCS_EvtNtfyInd_T *p_ntfy, BLE_ANCS_NtfyEvt_T ntfyEvtId)
{
    if (  (ntfyEvtId >= BLE_ANCS_NTFY_EVT_MAX)
       || (p_ntfy->categoryId >= BLE_ANCS_CATEGORY_ID_MAX))
    {
        return true;
    }

    return false;
}


/**
 * @brief Handle the notification response.
 *
 * This function processes the received notification attributes and conveys the appropriate event.
 *
 * @param p_conn    Pointer to the connection list structure.
 * @param p_event Pointer to the received handle value event structure.
 */
static void ble_ancs_NotificationRsp(BLE_ANCS_ConnList_T *p_conn, GATT_EvtReceiveHandleValue_T *p_event)
{
    BLE_ANCS_EvtNtfyInd_T *p_ntfy;
    BLE_ANCS_Event_T evtPara;
    uint8_t *p_data  = p_event->receivedValue, ntfyEvtId;
    BLE_ANCS_EventId_T eventId[] = {BLE_ANCS_EVT_NTFY_ADDED_IND, BLE_ANCS_EVT_NTFY_MODIFIED_IND, BLE_ANCS_EVT_NTFY_REMOVED_IND};
    uint16_t  hvx_data_len= p_event->receivedLength;

    if (hvx_data_len != BLE_ANCS_NTFY_DATA_LENGTH)
    {
        ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    p_ntfy = &evtPara.eventField.evtNtfyInd;

    ntfyEvtId  = p_data[BLE_ANCS_NTFY_EVT_ID_INDEX];

    p_ntfy->ntfyEvtFlagMask.silent          =
            (p_data[BLE_ANCS_NTFY_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_SILENT) & 0x01U;

    p_ntfy->ntfyEvtFlagMask.important       =
            (p_data[BLE_ANCS_NTFY_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_IMPORTANT) & 0x01U;

    p_ntfy->ntfyEvtFlagMask.preExisting    =
            (p_data[BLE_ANCS_NTFY_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_PREEXISTING) & 0x01U;

    p_ntfy->ntfyEvtFlagMask.positiveAction =
            (p_data[BLE_ANCS_NTFY_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_POSITIVE_ACTION) & 0x01U;

    p_ntfy->ntfyEvtFlagMask.negativeAction =
            (p_data[BLE_ANCS_NTFY_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_NEGATIVE_ACTION) & 0x01U;

    p_ntfy->categoryId               =
        (BLE_ANCS_CategoryId_T) p_data[BLE_ANCS_NTFY_CATEGORY_ID_INDEX];

    p_ntfy->categoryCount            = p_data[BLE_ANCS_NTFY_CATEGORY_CNT_INDEX];
    (void)memcpy((uint8_t*)&p_ntfy->ntfyId, (uint8_t*)&p_data[BLE_ANCS_NTFY_UID_INDEX], 4);

    if (ble_ancs_VerifyNotification(p_ntfy, (BLE_ANCS_NtfyEvt_T)ntfyEvtId)==true)
    {
        ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_UNSPECIFIED_IND, NULL, 0);
        return;
    }

    evtPara.eventField.evtNtfyInd.connHandle  = p_conn->connHandle;
    ble_ancs_ConveyEvent(eventId[ntfyEvtId], (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtNtfyInd_T));
}


/**
 * @brief Get the connection list entry by connection handle.
 *
 * This function searches the connection list for an entry with the given connection handle.
 *
 * @param connHandle The connection handle to search for.
 * 
 * @retval Pointer to the connection list entry, or NULL if not found.
 */
static BLE_ANCS_ConnList_T *ble_ancs_GetConnListByHandle(uint16_t connHandle)
{
    uint8_t i;
    BLE_ANCS_ConnList_T *p_conn = NULL;

    for(i=0; i<BLE_ANCS_MAX_CONN_NBR; i++)
    {
        if ((sp_ancsConnList[i] != NULL) && (sp_ancsConnList[i]->state == BLE_ANCS_STATE_CONNECTED) && (sp_ancsConnList[i]->connHandle == connHandle))
        {
            p_conn = sp_ancsConnList[i];
            break;
        }
    }

    return p_conn;
}


/**
 * @brief Check if the ANCS packet is complete.
 *
 * This function checks the packet data to determine if the ANCS packet is complete or if more packets are expected.
 *
 * @param p_conn    Pointer to the connection list structure containing the packet data.
 * 
 * @retval The status of the packet (last packet, mid packet, or error).
 */
static BLE_ANCS_PacketOrder_T  ble_ancs_ChkAncsComplete(BLE_ANCS_ConnList_T *p_conn)
{
    BLE_ANCS_PacketOrder_T ret;
    uint8_t *p_data = p_conn->p_packet;
    uint16_t len = p_conn->pktLen, index =0;
    BLE_ANCS_CmdId_T  commandId = (BLE_ANCS_CmdId_T)p_data[0];

    index++;        //CommandID

    if (commandId == BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR)
    {
        index += 4U; //NotificationUID
    }
    else
    {
        index += (uint16_t)strlen((char *)(p_data +1U));
        index ++;   //'\0'
    }


    while(true)
    {
        uint16_t attrLen;

        index++;    //AttributeID
        attrLen = p_data[index++];
        attrLen |= (uint16_t)p_data[index++] << 8U;

        if (index + attrLen == len)
        {
            ret = BLE_ANCS_LAST_PACKET;
            break;
        }
        else
        {
            if (index + attrLen < len)
            {
                index += attrLen;  
            }
            else   
            {      
                ret = BLE_ANCS_MID_PACKET; 
                break; 
            }
        }
    }

    p_conn->completePkt = ret;

    return ret;
}


/**
 * @brief Handles the response for attribute data received from ANCS.
 *
 * @param p_conn Pointer to the connection list structure.
 * 
 */
static void ble_ancs_AttrsRsp(BLE_ANCS_ConnList_T *p_conn)
{
    uint8_t * p_data        = p_conn->p_packet;
    BLE_ANCS_DecodeNtfyAttrs_T  *p_attrNtfyData= &p_conn->attrData.attrNtfyData;
    BLE_ANCS_DecodeAppAttrs_T   *p_attrAppData = &p_conn->attrData.attrAppData;

    uint16_t  hvx_data_len  = p_conn->pktLen, index =0;
    BLE_ANCS_CmdId_T  commandId  = (BLE_ANCS_CmdId_T) p_data[index++];
    uint16_t maxLen[BLE_ANCS_COMMAND_ID_GET_APP_ATTR +1][BLE_ANCS_NTFY_ATTR_ID_MAX] ={
        { BLE_ANCS_MAX_APPID_LEN,   BLE_ANCS_MAX_TITLE_LEN, BLE_ANCS_MAX_SUB_TITLE_LEN,
          BLE_ANCS_MAX_MESSAGE_LEN, BLE_ANCS_MAX_MESSAGE_SIZE_LEN, BLE_ANCS_MAX_DATE_LEN,
          BLE_ANCS_MAX_POS_ACTION_LABEL_LEN, BLE_ANCS_MAX_NEG_ACTION_LABEL_LEN
        },
        { BLE_ANCS_MAX_APP_NAME_LEN,0,0,0,0,0,0,0}
    };

    if (commandId == BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR)
    {
        p_conn->p_attrList    = p_conn->ntfyAttrList;
        (void)memcpy((uint8_t*)&p_attrNtfyData->ntfyId , (uint8_t*)&p_data[index], 4);
        index += 4U;
    }
    else
    {
        p_conn->p_attrList    = p_conn->appAttrList;
        (void)strcpy((char *)p_attrAppData->appId , (char *)(p_data +index));
        index += (uint16_t)(strlen((char *)p_attrAppData->appId) +1U);
    }

    do
    {
        uint16_t len, bufferFail, normalAttrID;
        uint8_t attrId;

        attrId  = p_data[index++];
        //the len does not include '\0' by ANCS spec
        len     = p_data[index++];
        len     |= (uint16_t)p_data[index++] << 8U;

        bufferFail      = 0;
        normalAttrID    = 1;

        if (commandId == BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR)
        {
            if ((attrId == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_TITLE)    ||
                (attrId == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_SUBTITLE) ||
                (attrId == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_MESSAGE))
            {
                uint8_t *p_tmp;

                p_tmp = OSAL_Malloc((uint32_t)maxLen[commandId][attrId] +1U);  //reserved '\0'
                if (p_tmp == NULL)
                {
                   bufferFail = 1;
                }
                else
                {
                    if (attrId == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_TITLE)
                    {
                        p_attrNtfyData->p_title      = p_tmp;
                    }
                    else if (attrId == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_SUBTITLE)
                    {
                        p_attrNtfyData->p_subtitle   = p_tmp;
                    }
                    else 
                    {
                        p_attrNtfyData->p_msg        = p_tmp;
                    }
                      
                    p_conn->p_attrList[attrId].p_attrData   = p_tmp; 
                }

                normalAttrID = 0;
            }
        }
        
        if (normalAttrID != 0U)
        {            
            if (len >= maxLen[commandId][attrId])
            {
                bufferFail = 1;
            }
        }
         
        if (bufferFail == 0U)
        {
            if (attrId != (uint8_t)BLE_ANCS_NTFY_ATTR_ID_MESSAGE_SIZE)
            {
                (void)memcpy(p_conn->p_attrList[attrId].p_attrData, p_data +index, len);
                p_conn->p_attrList[attrId].p_attrData[len] = 0;     //'\0'
            }
            else
            {
                uint8_t str[8];

                (void)memcpy(str, p_data +index, len);
                str[len] = 0;                                       //'\0'

                p_attrNtfyData->msgSize = (uint16_t)(atoi((char *)str));
            }

            if (commandId == BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR)
            {
                uint8_t *p_bitId = (uint8_t *)&p_attrNtfyData->bitmask;

                *p_bitId |= (1U << attrId);
            }
        }
        else
        {
            BLE_ANCS_Event_T evtPara;

            evtPara.eventField.evtErrAttrBufInd.connHandle  = p_conn->connHandle;
            evtPara.eventField.evtErrAttrBufInd.attrId      = (BLE_ANCS_NtfyAttrId_T)attrId;
            evtPara.eventField.evtErrAttrBufInd.len         = len;
            ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_ATTR_BUF_IND, (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtErrAttrBufInd_T));
        }

        index += len;

    } while (index <hvx_data_len);
}


/**
 * @brief Queries the notification attributes and prepares the data to be sent over GATT.
 *
 * @param p_conn        Pointer to the connection list structure.
 * @param p_gattValue   Pointer to the buffer where the GATT value will be stored.
 * @param uid           Unique identifier for the notification.
 * @param enabeMask     Bitmask to specify which attributes to query.
 * 
 * @retval uint16_t The length of the encoded data.
 */
static uint16_t ble_ancs_QueryNtfyAttr(BLE_ANCS_ConnList_T *p_conn, uint8_t *p_gattValue, uint32_t uid, BLE_ANCS_NtfyAttrsMask_T enabeMask)
{
    uint16_t index  = 0;
    uint8_t bitmask = *(uint8_t *)&enabeMask;
    uint16_t maxEncodeLen[] = {0, BLE_ANCS_MAX_TITLE_LEN, BLE_ANCS_MAX_SUB_TITLE_LEN, BLE_ANCS_MAX_MESSAGE_LEN};

    if (bitmask != 0U)
    {
        uint8_t   attr;
        //Encode Command ID.
        p_gattValue[index++] = (uint8_t)BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR;

        //Encode Notification UID.
        (void)memcpy((uint8_t*)&p_gattValue[index], (uint8_t*)&uid, 4);
        index += 4U;

        //Encode Attribute ID.
        for (attr = (uint8_t)BLE_ANCS_NTFY_ATTR_ID_APP_IDENTIFIER; attr < (uint8_t)BLE_ANCS_NTFY_ATTR_ID_MAX; attr++)
        {
            if ((bitmask & (1U <<attr))!= 0U)
            {
                p_gattValue[index++] = (uint8_t)attr;

                if ((attr == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_TITLE) ||
                    (attr == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_SUBTITLE) ||
                    (attr == (uint8_t)BLE_ANCS_NTFY_ATTR_ID_MESSAGE))
                {
                    //Encode Length field. Only applicable for Title, Subtitle, and Message.
                    (void)memcpy((uint8_t*)&p_gattValue[index], (uint8_t*)&maxEncodeLen[attr], 2);

                    index += 2U;
                }
            }
        }
    }
    return index ;
}


/**
 * @brief Encodes the notification action into the provided buffer.
 *
 * @param p_conn        Pointer to the connection list structure.
 * @param p_encodedData Pointer to the buffer where the encoded data will be stored.
 * @param ntfyId        Notification identifier to which the action applies.
 * @param actId         Action identifier that specifies the action to be performed.
 * 
 * @retval uint16_t The length of the encoded data.
 */
static uint16_t ble_ancs_EncodeNotifAction(BLE_ANCS_ConnList_T *p_conn, uint8_t * p_encodedData, uint32_t ntfyId, BLE_ANCS_ActionId_T actId)
{
    uint8_t index = 0;

    p_encodedData[index++] = (uint8_t)BLE_ANCS_COMMAND_ID_SET_PERFORM_NTFY_ACTION;
    (void)memcpy((uint8_t*)&p_encodedData[index], (uint8_t*)&ntfyId, 4);
    index += 4U;
    p_encodedData[index++] = (uint8_t)actId;

    return index;
}


/**
 * @brief Requests attributes for a specific iOS notification.
 * This function triggers the ANCS client role to retrieve notification attributes from an iOS device.
 *
 * @param[in] connHandle                    The connection handle associated with the iOS device.
 * @param[in] ntfyId                        The unique identifier for the iOS notification. The ntfyId could be retrieved when receiving @ref BLE_ANCS_EVT_NTFY_ADDED_IND or @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND or @ref BLE_ANCS_EVT_NTFY_REMOVED_IND events.
 * @param[in] bitmask                       A bitmask specifying which notification attributes to retrieve.
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
uint16_t BLE_ANCS_GetNtfyAttr(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_NtfyAttrsMask_T bitmask)
{
    uint16_t ret = MBA_RES_INVALID_PARA, len;
    BLE_ANCS_ConnList_T *p_conn;

    p_conn = ble_ancs_GetConnListByHandle(connHandle);
    if (p_conn != NULL)
    {       
        GATTC_WriteParams_T *p_writeParams;
        uint16_t charHandle = sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][ANCS_INDEX_CHAR_CONTROL_POINT].charHandle;

        p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
        if (p_writeParams == NULL)
        {
            return MBA_RES_OOM;
        }
      
        len = ble_ancs_QueryNtfyAttr(p_conn, p_writeParams->charValue, ntfyId, bitmask);
             
        if (len!= 0U)
        {
            ret = ble_ancs_GattWrite(p_conn, p_writeParams, charHandle, len); 
        }
    
        OSAL_Free(p_writeParams);                                
    }
 
    return ret;
}


/**
 * @brief Queries the app attributes and prepares the data to be sent over GATT.
 *
 * @param p_conn    Pointer to the connection list structure.
 * @param p_appId   Pointer to the app identifier string.
 * @param p_value   Pointer to the buffer where the GATT value will be stored.
 * 
 * @retval uint16_t The length of the encoded data.
 */
static uint16_t ble_ancs_QueryAppAttr(BLE_ANCS_ConnList_T *p_conn, uint8_t *p_appId, uint8_t *p_value)
{
    uint16_t  index = 0;

    // Encode Command ID.
    p_value[index++] = (uint8_t)BLE_ANCS_COMMAND_ID_GET_APP_ATTR;

    //Encode app identifier.
    (void)strcpy((char *)(p_value +index) , (char *)p_appId);
    index += (uint16_t)(strlen((char *)p_appId) +1U);

    //Encode Attribute ID.
    p_value[index++] = (uint8_t)BLE_ANCS_APP_ATTR_ID_DISPLAY_NAME;

    return index;
}


/**
 * @brief Retrieves attributes for a specific app on an iOS device.
 *
 * @param[in] connHandle                    The handle for the current connection with the iOS device.
 * @param[in] p_appId                       Pointer to a null-terminated string representing the app's identifier. App Identifier can be retrieved when receiving @ref BLE_ANCS_EVT_NTFY_ATTR_IND event.
 * @param[in] bitmask                       Bitmask specifying which app attributes to retrieve. Refer to @ref BLE_ANCS_AppAttrsMask_T for possible values.
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
uint16_t BLE_ANCS_GetAppAttr(uint16_t connHandle, uint8_t *p_appId, BLE_ANCS_AppAttrsMask_T bitmask)
{
    uint16_t ret = MBA_RES_INVALID_PARA, index;
    BLE_ANCS_ConnList_T *p_conn;

    if (bitmask.displayName==0U)
    {
        return MBA_RES_INVALID_PARA;
    }
            
    p_conn = ble_ancs_GetConnListByHandle(connHandle);
    if (p_conn != NULL)
    { 
        GATTC_WriteParams_T *p_writeParams;
        uint16_t charHandle = sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][ANCS_INDEX_CHAR_CONTROL_POINT].charHandle;

        p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
        if (p_writeParams == NULL)
        {
            return MBA_RES_OOM;   
        }
        
        index = ble_ancs_QueryAppAttr(p_conn, p_appId, p_writeParams->charValue);
        
        if (index != 0U)
        {
            ret = ble_ancs_GattWrite(p_conn, p_writeParams, charHandle, index);
        }
         
        OSAL_Free(p_writeParams);
    }

    return ret;
}


/**
 * @brief Performs an action on a specific iOS notification.
 * 
 * This function sends a command to the iOS device to perform a specified action on a notification identified 
 * by its unique identifier (UID). The action to be performed is determined by the action ID provided.
 *
 * @param[in] connHandle                    The handle for the current connection with the iOS device.
 * @param[in] ntfyId                        The 32-bit UID of the iOS notification on which to perform the action.
 *                                          The ntfyId can be retrieved when receiving @ref BLE_ANCS_EVT_NTFY_ADDED_IND or @ref BLE_ANCS_EVT_NTFY_MODIFIED_IND or @ref BLE_ANCS_EVT_NTFY_REMOVED_IND events.
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
uint16_t BLE_ANCS_PerformNtfyAction(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_ActionId_T actId)
{
    uint16_t len, ret= MBA_RES_INVALID_PARA;
    BLE_ANCS_ConnList_T *p_conn;

    if (actId < BLE_ANCS_ACTION_ID_MAX)
    {
        p_conn = ble_ancs_GetConnListByHandle(connHandle);
        if (p_conn != NULL)
        {
            GATTC_WriteParams_T *p_writeParams;
            uint16_t charHandle = sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][ANCS_INDEX_CHAR_CONTROL_POINT].charHandle;

            p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
            if (p_writeParams == NULL)
            {
                return MBA_RES_OOM;
            }
            
            len = ble_ancs_EncodeNotifAction(p_conn, p_writeParams->charValue, ntfyId, actId);

            ret = ble_ancs_GattWrite(p_conn, p_writeParams, charHandle, len);

            OSAL_Free(p_writeParams);
        }
    }

    return ret;
}


/**
 * @brief Process error event from GATT.
 *
 * @param p_event   Pointer to the GATT event structure.
 */
static void ble_ancs_ProcErr(GATT_Event_T *p_event)
{
    BLE_ANCS_Event_T evtPara;
    evtPara.eventField.evtErrInd.connHandle  = p_event->eventField.onError.connHandle;
    evtPara.eventField.evtErrInd.errCode     = p_event->eventField.onError.errCode;
    ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_IND, (uint8_t *)&evtPara.eventField,(uint8_t)sizeof(BLE_ANCS_EvtErrInd_T));
}


/**
 * @brief Process GATT notifications for the ANCS service.
 *
 * @param p_conn    Pointer to the ANCS connection list structure.
 * @param p_event   Pointer to the GATT event structure.
 */
static void ble_ancs_ProcGattNotification(BLE_ANCS_ConnList_T *p_conn, GATT_Event_T *p_event)
{
    uint16_t charHandle = p_event->eventField.onNotification.charHandle;
    BLE_ANCS_Event_T evtPara;

    if (charHandle == sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][ANCS_INDEX_CHAR_NTFY].charHandle)
    {
        ble_ancs_NotificationRsp(p_conn, &p_event->eventField.onNotification);
    }
    else if (charHandle == sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][ANCS_INDEX_CHAR_DATA].charHandle)
    {
        BLE_ANCS_DecodeNtfyAttrs_T  *p_attrNtfyData= &p_conn->attrData.attrNtfyData;
        uint8_t * p_data   = p_event->eventField.onNotification.receivedValue;
        uint16_t  dataLen  = p_event->eventField.onNotification.receivedLength;

        if (p_conn->completePkt == BLE_ANCS_FIRST_PACKET)
        {
            *(uint8_t *)&p_attrNtfyData->bitmask = 0;

            p_conn->p_packet = OSAL_Malloc(BLE_ANCS_MAX_PACKET_BUFFER_SIZE);
            if (p_conn->p_packet == NULL)
            {
               return;
            }
        }
        else
        {
            if (p_conn->pktLen +dataLen > BLE_ANCS_MAX_PACKET_BUFFER_SIZE)
            {
            /*
                Check if BLE_ANCS_MAX_PACKET_BUFFER_SIZE is suitable
            */
                ble_ancs_InitResource(p_conn);
                evtPara.eventField.evtErrRecomposeBufInd.connHandle  = p_conn->connHandle;
                ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND, (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtErrRecomposeBufInd_T));

                return;
            }
        }
        (void)memcpy(p_conn->p_packet +p_conn->pktLen , p_data, dataLen);
        p_conn->pktLen  += dataLen;

        /*
            Recompose packet if more notification
            Check if miss any notification, so it can not be recomposed successfully.
        */
        if (ble_ancs_ChkAncsComplete(p_conn) == BLE_ANCS_LAST_PACKET)
        {
            BLE_ANCS_CmdId_T  commandId = (BLE_ANCS_CmdId_T) p_conn->p_packet[0];

            ble_ancs_AttrsRsp(p_conn);

            if (commandId == BLE_ANCS_COMMAND_ID_GET_NTFY_ATTR)
            {
                evtPara.eventField.evtNtfyAttrInd.connHandle  = p_conn->connHandle;
                evtPara.eventField.evtNtfyAttrInd.p_data = &p_conn->attrData.attrNtfyData;
                ble_ancs_ConveyEvent(BLE_ANCS_EVT_NTFY_ATTR_IND, (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtNtfyAttrInd_T));
            }
            else
            {
                evtPara.eventField.evtAppAttrInd.connHandle     = p_conn->connHandle;
                evtPara.eventField.evtAppAttrInd.p_data = &p_conn->attrData.attrAppData;
                ble_ancs_ConveyEvent(BLE_ANCS_EVT_APP_ATTR_IND, (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtAppAttrInd_T));
            }
            ble_ancs_InitResource(p_conn);
        }
    }
    else
    {
        //Shall not enter here
    }
}


/**
 * @brief Enable the CCCD (Client Characteristic Configuration Descriptor) for the ANCS characteristics.
 *
 * @param connHandle    Connection handle to identify the GATT connection.
 */
static void ble_ancs_enableCccd(uint16_t connHandle)
{
    BLE_ANCS_ConnList_T *p_conn = ble_ancs_GetConnListByHandle(connHandle);
    uint16_t status;

    if (p_conn == NULL)
    {
        return;
    }

    switch (p_conn->enableCccd)
    {
        case ANCS_INDEX_CHAR_NTFY_CCCD:
        case ANCS_INDEX_CHAR_DATA_CCCD:
        {
            GATTC_WriteParams_T *p_writeParams = OSAL_Malloc(sizeof(GATTC_WriteParams_T));
            if (p_writeParams == NULL)
            {
                ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_NO_MEM_IND, NULL, 0);
                return;
            }
            p_writeParams->charHandle   = sp_ancsServiceDb->ancsCharInfoList[p_conn->connIndex][p_conn->enableCccd].charHandle;
            U16_TO_BUF_LE(p_writeParams->charValue, NOTIFICATION);
            p_writeParams->charLength   = 2;
            p_writeParams->writeType    = ATT_WRITE_REQ;
            p_writeParams->valueOffset  = 0x0000;
            p_writeParams->flags        = 0;

            status = GATTC_Write(connHandle, p_writeParams);
            if (status == MBA_RES_SUCCESS)
            {
                if (p_conn->enableCccd == ANCS_INDEX_CHAR_DATA_CCCD)
                {
                    p_conn->enableCccd = ANCS_INDEX_CHAR_NTFY_CCCD;
                }
                else
                {
                    p_conn->enableCccd = ANCS_INDEX_CHAR_CONTROL_POINT;
                }
                p_conn->retryType = 0U;
            }
            else
            {
                p_conn->retryType = BLE_ANCS_RETRY_TYPE_ENABLE_NTFY;
            }
            OSAL_Free(p_writeParams);
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
 * @brief Process queued tasks for the ANCS service based on the connection handle.
 *
 * @param connHandle    Connection handle to identify the GATT connection.
 */
static void ble_ancs_ProcessQueuedTask(uint16_t connHandle)
{
    BLE_ANCS_ConnList_T *p_conn;

    p_conn = ble_ancs_GetConnListByHandle(connHandle);
    if (p_conn == NULL)
    {
        return;
    }
    switch (p_conn->retryType)
    {
        case BLE_ANCS_RETRY_TYPE_ENABLE_NTFY:
        {
            ble_ancs_enableCccd(connHandle);
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
 * @brief Process GATT events for the ANCS service.
 *
 * @param p_event   Pointer to the GATT event structure.
 */
static void ble_ancs_GattEventProcess(GATT_Event_T *p_event)
{
    BLE_ANCS_ConnList_T *p_conn;
    
    switch (p_event->eventId)
    {
        case ATT_EVT_UPDATE_MTU:
        {
            p_conn = ble_ancs_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn != NULL)
            {
                p_conn->attMtu =  p_event->eventField.onUpdateMTU.exchangedMTU;
            }
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            p_conn = ble_ancs_GetConnListByHandle(p_event->eventField.onNotification.connHandle);
            if (p_conn != NULL)
            {
                ble_ancs_ProcGattNotification(p_conn, p_event);
            }
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
            p_conn = ble_ancs_GetConnListByHandle(p_event->eventField.onUpdateMTU.connHandle);
            if (p_conn == NULL)
            {
                return;
            }

            if ((p_conn->enableCccd == ANCS_INDEX_CHAR_NTFY_CCCD) || (p_conn->enableCccd == ANCS_INDEX_CHAR_DATA_CCCD))
            {
                ble_ancs_enableCccd(p_event->eventField.onClientProtocolAvailable.connHandle);
            }
        }
        break;
        case GATTC_EVT_ERROR_RESP:
        {
            p_conn = ble_ancs_GetConnListByHandle(p_event->eventField.onError.connHandle);
            if (p_conn != NULL)
            {
                ble_ancs_ProcErr(p_event);
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
 * @brief Get a free connection list entry for the ANCS service.
 *
 * @retval Pointer to the ANCS connection list structure, or NULL if no free entry is available.
 */
static BLE_ANCS_ConnList_T *ble_ancs_GetFreeConnList(void)
{
    uint8_t i;
    BLE_ANCS_ConnList_T *p_conn = NULL;

    for(i = 0; i < BLE_ANCS_MAX_CONN_NBR; i++)
    {
        if (sp_ancsConnList[i] == NULL)
        {
            sp_ancsConnList[i] = OSAL_Malloc(sizeof(BLE_ANCS_ConnList_T));
            p_conn = sp_ancsConnList[i];
            if (p_conn != NULL)
            {
                BLE_ANCS_DecodeNtfyAttrs_T *p_attrNtfyData = &sp_ancsConnList[i]->attrData.attrNtfyData;
                BLE_ANCS_DecodeAppAttrs_T  *p_attrAppData  = &sp_ancsConnList[i]->attrData.attrAppData;

                (void)memset(p_conn, 0, sizeof(BLE_ANCS_ConnList_T));
                p_conn->state     = BLE_ANCS_STATE_CONNECTED;
                p_conn->connIndex = i;
                p_conn->attMtu    = BLE_ATT_DEFAULT_MTU_LEN;
                p_conn->ntfyAttrList[BLE_ANCS_NTFY_ATTR_ID_APP_IDENTIFIER].p_attrData 
                                  = p_attrNtfyData->appId;
                p_conn->ntfyAttrList[BLE_ANCS_NTFY_ATTR_ID_DATE].p_attrData 
                                  = p_attrNtfyData->date;
                p_conn->ntfyAttrList[BLE_ANCS_NTFY_ATTR_ID_POSITIVE_ACTION_LABEL].p_attrData 
                                  = p_attrNtfyData->positiveAction;
                p_conn->ntfyAttrList[BLE_ANCS_NTFY_ATTR_ID_NEGATIVE_ACTION_LABEL].p_attrData 
                                  = p_attrNtfyData->negativeAction;
                p_conn->appAttrList[BLE_ANCS_APP_ATTR_ID_DISPLAY_NAME].p_attrData 
                                  = p_attrAppData->displayName;
            }
            break;
        }
    }

    return p_conn;
}


/**
 * @brief Free the connection list for the ANCS service.
 *
 * @param p_conn        Pointer to the ANCS connection list structure to initialize.
 * @param disconnect    Flag indicating whether to disconnect.
 */
static void ble_ancs_FreeConnList(BLE_ANCS_ConnList_T *p_conn)
{
    uint8_t i;
    ble_ancs_InitResource(p_conn);
    for (i = 0; i < BLE_ANCS_MAX_CONN_NBR; i++)
    {
        if (sp_ancsConnList[i] == p_conn)
        {
            OSAL_Free(sp_ancsConnList[i]);
            sp_ancsConnList[i] = NULL;
            break;
        }
    }
}


/**
 * @brief Process GAP events for the ANCS service.
 *
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_ancs_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    BLE_ANCS_ConnList_T *p_conn = NULL;
    
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            if (p_event->eventField.evtConnect.status == GAP_STATUS_SUCCESS)
            {
                p_conn = ble_ancs_GetFreeConnList();

                if (p_conn == NULL)
                {
                    ble_ancs_ConveyEvent(BLE_ANCS_EVT_ERR_NO_MEM_IND, NULL, 0);
                }
                else
                {
                    p_conn->connHandle = p_event->eventField.evtConnect.connHandle;
                }
            }
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            uint16_t connHandle = p_event->eventField.evtDisconnect.connHandle;
            p_conn = ble_ancs_GetConnListByHandle(connHandle);
            if (p_conn != NULL)
            {
                ble_ancs_FreeConnList(p_conn);
            }
        }
        break;
        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            uint16_t connHandle = p_event->eventField.evtEncryptStatus.connHandle;
            if (p_event->eventField.evtEncryptStatus.status == GAP_STATUS_SUCCESS)
            {
                p_conn = ble_ancs_GetConnListByHandle(connHandle);
                if (p_conn == NULL)
                {
                    return;
                }
                p_conn->enableCccd = ANCS_INDEX_CHAR_DATA_CCCD;
                ble_ancs_enableCccd(connHandle);
            }
        }
        break;
        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            ble_ancs_ProcessQueuedTask(p_event->eventField.evtTxBufAvailable.connHandle);
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
 * @brief Registers a callback function for BLE ANCS events.
 *
 * @param[in] bleAncsHandler                The function to be called when an ANCS event occurs.
 *
 */
void BLE_ANCS_EventRegister(BLE_ANCS_EventCb_T bleAncsHandler)
{
    s_bleAncsProcess = bleAncsHandler;
}


/**
 * @brief Initialize the characteristic list for the Apple Notification Center Service (ANCS).
 *
 * This function initializes the characteristic list for a given connection index by setting the
 * connection handle to 0 and assigning the pointer to the characteristic information structure.
 * It also initializes the characteristic handle and property for each characteristic in the list.
 *
 * @param[in,out] p_charList    Pointer to the characteristic list to initialize.
 * @param[in] connIndex         The index of the connection for which the characteristic list is being initialized.
 * 
 */
static void ble_ancs_InitCharList(BLE_DD_CharList_T *p_charList, uint8_t connIndex)
{
    uint8_t i;

    p_charList->connHandle = 0;
    p_charList->p_charInfo = (BLE_DD_CharInfo_T *) (sp_ancsServiceDb->ancsCharInfoList[connIndex]);

    for(i=0; i<(uint8_t)ANCS_INDEX_CHAR_MAX_NUM; i++)
    {
        sp_ancsServiceDb->ancsCharInfoList[connIndex][i].charHandle = 0;
        sp_ancsServiceDb->ancsCharInfoList[connIndex][i].property = 0;
    }
}

/**
 * @brief Initializes the Apple Notification Center Service (ANCS) profile.
 *
 * @retval MBA_RES_SUCCESS                  BLE ANCS profile was initialized successfully.
 * @retval MBA_RES_FAIL                     Fail to initialize the BLE ANCS profile.
 * @retval MBA_RES_OOM                      Internal memory allocation failure.
 *
 */
uint16_t BLE_ANCS_Init(void)
{
    uint8_t i;
    uint16_t ret;
    BLE_DD_DiscSvc_T  ancsDisc;

    if (sp_ancsServiceDb != NULL)
    {
        return MBA_RES_FAIL;
    }
    sp_ancsServiceDb = (BLE_ANCS_ServiceDb_T*)OSAL_Malloc(sizeof(BLE_ANCS_ServiceDb_T));
    if (sp_ancsServiceDb == NULL)
    {
        return MBA_RES_OOM;
    }

    (void)memset(sp_ancsServiceDb->ancsDiscCharList, 0x00, sizeof(BLE_DD_DiscChar_T)*ANCS_INDEX_CHAR_MAX_NUM);
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_CONTROL_POINT].p_uuid = &s_discCharCtrl;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_NTFY].p_uuid          = &s_discCharNtfy;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_NTFY_CCCD].p_uuid     = &s_discCharNtfyCccd;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_NTFY_CCCD].settings   = CHAR_SET_DESCRIPTOR;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_DATA].p_uuid          = &s_discCharData;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_DATA_CCCD].p_uuid     = &s_discCharDataCccd;
    sp_ancsServiceDb->ancsDiscCharList[ANCS_INDEX_CHAR_DATA_CCCD].settings   = CHAR_SET_DESCRIPTOR;

    for(i = 0; i < ANCS_INDEX_CHAR_MAX_NUM; i++)
    {
        sp_ancsServiceDb->p_ancsDiscCharList[i] = &sp_ancsServiceDb->ancsDiscCharList[i];
    }

    for(i = 0; i < BLE_ANCS_MAX_CONN_NBR; i++)
    {
        ble_ancs_InitCharList(&sp_ancsServiceDb->ancsCharList[i], i);
    }
    ancsDisc.svcUuid.uuidLength = ATT_UUID_LENGTH_16;
    (void)memcpy(ancsDisc.svcUuid.uuid, s_discSvcUuid, ATT_UUID_LENGTH_16);
    ancsDisc.p_discInfo   = NULL;
    ancsDisc.p_discChars  = sp_ancsServiceDb->p_ancsDiscCharList;
    ancsDisc.p_charList   = sp_ancsServiceDb->ancsCharList;
    ancsDisc.discCharsNum = (uint8_t)ANCS_INDEX_CHAR_MAX_NUM;

    ret = BLE_DD_ServiceDiscoveryRegister(&ancsDisc);

    if (ret != MBA_RES_SUCCESS)
    {
        OSAL_Free(sp_ancsServiceDb);
        sp_ancsServiceDb = NULL;
    }
    return ret;

}


/**
 * @brief Handles BLE_Stack events.
 *        This function should be called when BLE stack events occur..
 *
 * @param[in] p_stackEvent                  Pointer to the BLE stack event data.
 *
 */
void BLE_ANCS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
            ble_ancs_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        break;

        case STACK_GRP_GATT:
            ble_ancs_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}


/**
 * @brief Handles BLE Database Discovery (BLE_DD) events.
 *        This function should be called when BLE_DD events occur.
 *
 * @param[in] p_event                       Pointer to the BLE_DD event data.
 *
 */
void BLE_ANCS_BleDdEventHandler(BLE_DD_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_DD_EVT_DISC_COMPLETE:
        {
            BLE_ANCS_ConnList_T *p_conn;

            p_conn = ble_ancs_GetConnListByHandle(p_event->eventField.evtDiscResult.connHandle);
            if (p_conn != NULL)
            {
                BLE_ANCS_Event_T evtPara;

                p_conn->enableCccd = ANCS_INDEX_CHAR_DATA_CCCD;
                ble_ancs_enableCccd(p_event->eventField.evtDiscResult.connHandle);

                evtPara.eventField.evtDiscComplete.connHandle = p_conn->connHandle;
                ble_ancs_ConveyEvent(BLE_ANCS_EVT_DISC_COMPLETE_IND, (uint8_t *)&evtPara.eventField, (uint8_t)sizeof(BLE_ANCS_EvtDiscComplete_T));
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

