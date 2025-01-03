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
  BLE HID over GATT profile server role Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_hogps.c

  Summary:
    This source file contains the functions and definitions for the Bluetooth Low
    Energy (BLE) Human Interface Device (HID) over GATT profile server role, which
    can be utilized by the application developer.

  Description:
    The BLE HID over GATT profile server role is designed to enable the exchange
    of HID information over a BLE connection. This file provides a set of APIs
    for the application to interact with the BLE HID service, facilitating the
    implementation of HID devices such as keyboards, mice, and other input devices
    using BLE connectivity.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_hogps.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Structure for BLE HID over GATT Profile (HOGP) connection parameters. */
typedef struct BLE_HOGPS_ConnParams_T
{
    uint16_t                    connHandle;                                                         // Connection handle associated with this connection.
    uint8_t                     protocolMode;                                                       // Current protocol mode (Boot or Report).
    uint8_t                     suspendStatus;                                                      // Current suspend mode status (active or suspended).
} BLE_HOGPS_ConnParams_T;


/* Structure for BLE HID over GATT Profile (HOGP) database. */
typedef struct BLE_HOGPS_Database_T
{
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint16_t                    charLengthKbInputReport;                                            // Length of the Keyboard Input Report.
    #endif
    uint16_t                    charLengthMbInputReport;                                            // Length of the Mouse Button Input Report.
    uint16_t                    charLengthMmInputReport;                                            // Length of the Mouse Motion Input Report.
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint16_t                    charLengthKbOutputReport;                                           // Length of the Keyboard Output Report.
    #endif
    #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    uint16_t                    charLengthBootKbInputReport;                                        // Length of the Boot Keyboard Input Report.
    uint16_t                    charLengthBootKbOutputReport;                                       // Length of the Boot Keyboard Output Report.
    #endif
    uint16_t                    charLengthBootMInputReport;                                         // Length of the Boot Mouse Input Report.
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint8_t                     charValKbInputReport[HID_REPORT_LENGTH_KB_INPUT];                   // Buffer for the Keyboard Input Report data.
    #endif
    uint8_t                     charValMbInputReport[HID_REPORT_LENGTH_MB_INPUT];                   // Buffer for the Mouse Button Input Report data.
    uint8_t                     charValMmInputReport[HID_REPORT_LENGTH_MM_INPUT];                   // Buffer for the Mouse Motion Input Report data.
    #ifdef  HIDS_KEYBOARD_SUPPORT
    uint8_t                     charValKbOutputReport[HID_REPORT_LENGTH_KB_OUTPUT];                 // Buffer for the Keyboard Output Report data.
    #endif
    #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    uint8_t                     charValBootKbInputReport[HID_REPORT_LENGTH_BOOT_KB_INPUT];          // Buffer for the Boot Keyboard Input Report data.
    uint8_t                     charValBootKbOutputReport[HID_REPORT_LENGTH_BOOT_KB_OUTPUT];        // Buffer for the Boot Keyboard Output Report data.
    #endif
    uint8_t                     charValBootMInputReport[HID_REPORT_LENGTH_BOOT_M_INPUT];            // Buffer for the Boot Mouse Input Report data.
} BLE_HOGPS_Database_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static BLE_HOGPS_EventCb_T      sp_hogpsCbRoutine;      // Pointer to the callback function for BLE HID over GATT Profile (HOGP) events.
static BLE_HOGPS_ConnParams_T   s_hogpsConnParams;      // Structure to hold the connection parameters for the BLE HID over GATT Profile.
static BLE_HOGPS_Database_T     s_hogpsDb;              // Instance of the database structure for the BLE HID over GATT Profile services and characteristics.

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Initialize the HID over GATT Profile (HOGP) service database.
 *
 * This function initializes the HOGP service database to default values.
 */
static void ble_hogps_InitDatabase(void)
{
    (void)memset((uint8_t *)&s_hogpsDb, 0x00, sizeof(BLE_HOGPS_Database_T));
    #ifdef HIDS_KEYBOARD_SUPPORT
    s_hogpsDb.charLengthKbInputReport = HID_REPORT_LENGTH_KB_INPUT;
    #endif
    s_hogpsDb.charLengthMbInputReport = HID_REPORT_LENGTH_MB_INPUT;
    s_hogpsDb.charLengthMmInputReport = HID_REPORT_LENGTH_MM_INPUT;
    #ifdef HIDS_KEYBOARD_SUPPORT
    s_hogpsDb.charLengthKbOutputReport = HID_REPORT_LENGTH_KB_OUTPUT;
    #endif
    #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    s_hogpsDb.charLengthBootKbInputReport = HID_REPORT_LENGTH_BOOT_KB_INPUT;
    s_hogpsDb.charLengthBootKbOutputReport = HID_REPORT_LENGTH_BOOT_KB_OUTPUT;
    #endif
    s_hogpsDb.charLengthBootMInputReport = HID_REPORT_LENGTH_BOOT_M_INPUT;
}


/**
 * @brief Convey a HOGP event to the registered callback.
 *
 * @param p_event Pointer to the HOGP event structure that needs to be conveyed.
 */
static void ble_hogps_ConveyEvent(BLE_HOGPS_Event_T *p_event)
{
    if (sp_hogpsCbRoutine != NULL)
    {
        sp_hogpsCbRoutine(p_event);
    }
}


/**
 * @brief Generate and convey a report mode write event.
 *
 * This function creates a HOGP event based on the GATT event received and
 * conveys it to the application via a callback.
 *
 * @param p_event   Pointer to the GATT event structure containing the write information.
 * @param eventId   The ID of the HOGP event to be generated.
 * @param reportId  The ID of the report being written to.
 */
static void ble_hogps_GenerateReportModeWriteEvent(GATT_Event_T *p_event, BLE_HOGPS_EventId_T eventId, uint8_t reportId)
{
    BLE_HOGPS_Event_T hogpsEvent;

    hogpsEvent.eventId = eventId;
    hogpsEvent.eventField.evtInputReportWrite.connHandle = p_event->eventField.onWrite.connHandle;
    hogpsEvent.eventField.evtInputReportWrite.length = p_event->eventField.onWrite.writeDataLength;
    hogpsEvent.eventField.evtInputReportWrite.offset = p_event->eventField.onWrite.valueOffset;
    hogpsEvent.eventField.evtInputReportWrite.reportId = reportId;
    hogpsEvent.eventField.evtInputReportWrite.p_data = p_event->eventField.onWrite.writeValue;
    ble_hogps_ConveyEvent(&hogpsEvent);
}


#ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
/**
 * @brief Generate and convey a boot mode write event.
 *
 * This function creates a HOGP boot mode write event based on the GATT event received and
 * conveys it to the application via a callback.
 *
 * @param p_event Pointer to the GATT event structure containing the write information.
 * @param eventId The ID of the HOGP event to be generated.
 */
static void ble_hogps_GenerateBootModeWriteEvent(GATT_Event_T *p_event, BLE_HOGPS_EventId_T eventId)
{
    BLE_HOGPS_Event_T hogpsEvent;

    hogpsEvent.eventId = eventId;
    hogpsEvent.eventField.evtBootKbInputReportWrite.connHandle = p_event->eventField.onWrite.connHandle;
    hogpsEvent.eventField.evtBootKbInputReportWrite.length = p_event->eventField.onWrite.writeDataLength;
    hogpsEvent.eventField.evtBootKbInputReportWrite.offset = p_event->eventField.onWrite.valueOffset;
    hogpsEvent.eventField.evtBootKbInputReportWrite.p_data = p_event->eventField.onWrite.writeValue;
    ble_hogps_ConveyEvent(&hogpsEvent);
}
#endif


/**
 * @brief Process GAP events specific to the HOGP service.
 *
 * This function processes incoming GAP events and updates the HOGP service
 * connection parameters accordingly.
 *
 * @param p_event Pointer to the GAP event structure.
 */
static void ble_hogps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            s_hogpsConnParams.connHandle = p_event->eventField.evtConnect.connHandle;
            s_hogpsConnParams.protocolMode = HID_MODE_REPORT_PROTOCOL;
            s_hogpsConnParams.suspendStatus = HID_HOST_SUSPEND_EXIT;
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
 * @brief Send a read response to a GATT client.
 *
 * This function prepares and sends a read response for a given attribute to the GATT client.
 *
 * @param p_readEvt     Pointer to the read event structure containing details about the read operation.
 * @param len           The length of the data to be sent in the read response.
 * @param p_value       Pointer to the data to be sent in the read response.
 */
static void ble_hogps_SendReadResponse(GATT_EvtRead_T *p_readEvt, uint16_t len, uint8_t *p_value)
{
    GATTS_SendReadRespParams_T readResponse;
    
    readResponse.attrLength = len - p_readEvt->readOffset;
    (void)memcpy(readResponse.attrValue, p_value + p_readEvt->readOffset, readResponse.attrLength);
    readResponse.responseType = (p_readEvt->readType == ATT_READ_REQ) ? ATT_READ_RSP : ATT_READ_BLOB_RSP;

    (void)GATTS_SendReadResponse(p_readEvt->connHandle, &readResponse);
}


/**
 * @brief Send a read by type response to a GATT client.
 *
 * This function prepares and sends a read by type response for a given attribute to the GATT client.
 *
 * @param p_readEvt     Pointer to the read event structure containing details about the read by type operation.
 * @param len           The length of the data to be sent in the read by type response.
 * @param p_value       Pointer to the data to be sent in the read by type response.
 */
static void ble_hogps_SendReadByTypeResponse(GATT_EvtRead_T *p_readEvt, uint16_t len, uint8_t *p_value)
{
    GATTS_SendReadByTypeRespParams_T readResponse;
    uint8_t *p_buf;
    
    readResponse.allPairsLength = len + (uint16_t)sizeof(p_readEvt->attrHandle);
    readResponse.eachPairLength = (uint8_t)len + (uint8_t)sizeof(p_readEvt->attrHandle);

    p_buf = readResponse.attrDataList;
    U16_TO_STREAM_LE(&p_buf, p_readEvt->attrHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, p_value, len);


    (void)GATTS_SendReadByTypeResponse(p_readEvt->connHandle, &readResponse);
}


/**
 * @brief Process GATT events for the HID over GATT profile service.
 *
 * This function processes various GATT events such as read, write, etc., and takes appropriate actions
 * based on the HID over GATT profile specifications.
 *
 * @param p_event Pointer to the GATT event structure containing details about the event.
 */
static void ble_hogps_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTS_EVT_READ:
        {
            uint16_t valueLen = 0;
            uint8_t *p_value;

            #ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_HID_PROTOCOL_MODE)
            {
                valueLen = 0x01;
                p_value = &s_hogpsConnParams.protocolMode;
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_KB_INPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthKbInputReport;
                p_value = (uint8_t *)&s_hogpsDb.charValKbInputReport;
            }
            #endif
            #ifdef HIDS_MOUSE_SUPPORT
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_MB_INPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthMbInputReport;
                p_value = s_hogpsDb.charValMbInputReport;
            }
            else if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_MM_INPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthMmInputReport;
                p_value = s_hogpsDb.charValMmInputReport;
            }
            else
            {
                //Shall not enter here
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_KB_OUTPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthKbOutputReport;
                p_value = s_hogpsDb.charValKbOutputReport;
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthBootKbInputReport;
                p_value = s_hogpsDb.charValBootKbInputReport;
            }
            else if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_BOOT_KB_OUTPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthBootKbOutputReport;
                p_value = s_hogpsDb.charValBootKbOutputReport;
            }
            else
            {
                //Shall not enter here
            }
            #endif
            #if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            if (p_event->eventField.onRead.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT)
            {
                valueLen = s_hogpsDb.charLengthBootMInputReport;
                p_value = s_hogpsDb.charValBootMInputReport;
            }
            #endif

            if (valueLen > 0U)
            {
                if (p_event->eventField.onRead.readType == ATT_READ_BY_TYPE_REQ)
                {
                    ble_hogps_SendReadByTypeResponse(&p_event->eventField.onRead, valueLen, p_value);
                }
                else
                {
                    ble_hogps_SendReadResponse(&p_event->eventField.onRead, valueLen, p_value);
                }
            }

        }
        break;

        case GATTS_EVT_WRITE:
        {
            /* Only write characteristic value procedure is mandatory in HID */

            if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_HID_CTRL)
            {
                BLE_HOGPS_Event_T hogpsEvent;

                if (p_event->eventField.onWrite.writeDataLength == 0x01U)
                {
                    if (p_event->eventField.onWrite.writeValue[0] == (uint8_t)HID_HOST_SUSPEND_ENTER)
                    {
                        /* Enter suspend mode */
                        s_hogpsConnParams.suspendStatus = HID_HOST_SUSPEND_ENTER;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_HOST_SUSPEND_ENTER_IND;
                        hogpsEvent.eventField.evtHostSuspendEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else if (p_event->eventField.onWrite.writeValue[0] == (uint8_t)HID_HOST_SUSPEND_EXIT)
                    {
                        /* Exit suspend mode */
                        s_hogpsConnParams.suspendStatus = HID_HOST_SUSPEND_EXIT;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_HOST_SUSPEND_EXIT_IND;
                        hogpsEvent.eventField.evtHostSuspendExit.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else
                    {
                        //Shall not enter here
                    }
                }
            }
            #ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_HID_PROTOCOL_MODE)
            {
                BLE_HOGPS_Event_T hogpsEvent;

                if (p_event->eventField.onWrite.writeDataLength == 0x01U)
                {
                    if (p_event->eventField.onWrite.writeValue[0] == (uint8_t)HID_MODE_BOOT_PROTOCOL)
                    {
                        /* Enter boot mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_BOOT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_BOOT_MODE_ENTER_IND;
                        hogpsEvent.eventField.evtBootModeEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else if (p_event->eventField.onWrite.writeValue[0] == (uint8_t)HID_MODE_REPORT_PROTOCOL)
                    {
                        /* Enter report mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_REPORT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_REPORT_MODE_ENTER_IND;
                        hogpsEvent.eventField.evtReportModeEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else
                    {
                        //Shall not enter here
                    }
                }
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_KB_INPUT_REPORT)
            {
                s_hogpsDb.charLengthKbInputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValKbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_KB);
            }
            #endif
            #ifdef HIDS_MOUSE_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_MB_INPUT_REPORT)
            {
                s_hogpsDb.charLengthMbInputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValMbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_MB);
            }
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_MM_INPUT_REPORT)
            {
                s_hogpsDb.charLengthMmInputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValMmInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_MM);
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_KB_OUTPUT_REPORT)
            {
                s_hogpsDb.charLengthKbOutputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValKbOutputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_OUTPUT_REPORT_WRITE_IND, HID_REPORT_ID_KB);
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT)
            {
                s_hogpsDb.charLengthBootKbInputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValBootKbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_KB_INPUT_REPORT_WRITE_IND);
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_BOOT_KB_OUTPUT_REPORT)
            {
                s_hogpsDb.charLengthBootKbOutputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValBootKbOutputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_KB_OUTPUT_REPORT_WRITE_IND);
            }
            #endif
            #if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == (uint16_t)HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT)
            {
                s_hogpsDb.charLengthBootMInputReport = p_event->eventField.onWrite.writeDataLength;
                (void)memcpy(s_hogpsDb.charValBootMInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_M_INPUT_REPORT_WRITE_IND);
            }
            #endif
            else
            {
                //Shall not enter here
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
 * @brief Initializes the BLE HID over GATT profile server role.
 *
 * @retval MBA_RES_SUCCESS          The initialization was successful.
 * @retval MBA_RES_FAIL             The initialization failed.
 *
 */
uint16_t BLE_HOGPS_Init(void)
{
    sp_hogpsCbRoutine = NULL;
    ble_hogps_InitDatabase();
    return BLE_HIDS_Add();
}


/**
 * @brief Registers a callback for the BLE HID over GATT profile server role events.
 *
 * @param[in] routine               Callback function to handle BLE HID over GATT profile server role events.
 *
 */
void BLE_HOGPS_EventRegister(BLE_HOGPS_EventCb_T routine)
{
    sp_hogpsCbRoutine = routine;
}


/**
 * @brief Handles BLE_Stack events.
 * 
 * This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack event data.
 *
*/
void BLE_HOGPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_hogps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_hogps_GattEventProcess((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        {
            //Do nothing
        }
        break;
    }
}

#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sets HID keyboard input report.
 * 
 * This function prepares the input report for a keyboard device in the Human Interface Device (HID) profile.
 * The report includes modifier keys, reserved byte, and up to six keycodes representing the keys that are pressed.
 *
 * @param[in] p_keyCodeArray        An array of 8 bytes representing the keyboard report. The array is structured as follows:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)
 *                                  - Byte 1: Reserved (typically 0x00)
 *                                  - Bytes 2 to 7: Keycodes 1 to 6, indicating keys that are currently pressed
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetKeyboardInputReport(uint8_t *p_keyCodeArray)
{
    (void)memcpy(s_hogpsDb.charValKbInputReport, p_keyCodeArray, HID_REPORT_LENGTH_KB_INPUT);
    s_hogpsDb.charValKbInputReport[1] = 0x00;       // Constant (padding)
    return MBA_RES_SUCCESS;
}
#endif


/**
 * @brief Sets the HID mouse button input report.
 * 
 * This function prepares the input report for mouse button states, wheel movement, and AC pan in the HID profile.
 * It indicates which mouse buttons are pressed, the vertical scroll wheel movement, and the horizontal scroll (AC Pan).
 *
 * @param[in] buttons               A bitmask representing the state of mouse buttons. Each bit corresponds to a button:\n
 *                                  - Bit 0 to Bit 4: Indicate whether each button is pressed (1) or not (0)
 * @param[in] wheel                 The vertical scroll amount with a valid range from -127 to 127.
 * @param[in] acPan                 The horizontal scroll (AC Pan) amount with a valid range from -127 to 127.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetMouseButtonInputReport(uint8_t buttons, int8_t wheel, int8_t acPan)
{
    s_hogpsDb.charValMbInputReport[0] = buttons;
    s_hogpsDb.charValMbInputReport[1] = (uint8_t)wheel;
    s_hogpsDb.charValMbInputReport[2] = (uint8_t)acPan;
    return MBA_RES_SUCCESS;
}


/**
 * @brief Sets the HID mouse motion input report.
 *
 * This function prepares the input report for mouse motion in the HID profile.
 * It specifies the relative movement of the mouse along the X and Y axes.
 * 
 * @param[in] xAxis                 The relative movement on the X-Axis with a valid range from -2047 to 2047.
 * @param[in] yAxis                 The relative movement on the Y-Axis with a valid range from -2047 to 2047.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetMouseMotionInputReport(int16_t xAxis, int16_t yAxis)
{
    s_hogpsDb.charValMmInputReport[0] = (uint8_t)((uint8_t)xAxis>>4U);
    s_hogpsDb.charValMmInputReport[1] = (uint8_t)(((xAxis&0x000F)<<4U)|yAxis>>8U);
    s_hogpsDb.charValMmInputReport[2] = (uint8_t)yAxis;
    return MBA_RES_SUCCESS;
}

#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sets the HID keyboard output report.
 *
 * This function updates the LED status indicators for a HID keyboard, such as Num Lock,
 * Caps Lock, and Scroll Lock LEDs. The state of each LED is represented by individual bits
 * within the provided 'ledValue' parameter.
 * 
 * @param[in] ledValue              A 8-bit value where each bit corresponds to an LED on the keyboard:\n
 *                                  - Bit 0: Num Lock LED
 *                                  - Bit 1: Caps Lock LED
 *                                  - Bit 2: Scroll Lock LED
 *                                  - Bit 3: Compose LED
 *                                  - Bit 4: Kana LED
 *                                  The remaining bits (5-7) are reserved and should be set to 0.
 *
 * @retval MBA_RES_SUCCESS          The output report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetKeyboardOutputReport(uint8_t ledValue)
{
    s_hogpsDb.charValKbOutputReport[0] = ledValue;
    return MBA_RES_SUCCESS;
}
#endif

#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sets the HID boot keyboard input report.
 *
 * This function updates the input report for a boot keyboard with the specified key codes.
 * The input report format follows the standard boot protocol layout.
 * 
 * @param[in] p_keyCodeArray        An array representing the keyboard report. The array is expected 
 *                                  to be 8 bytes in length, with the following structure:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)
 *                                  - Byte 1: Reserved (typically 0x00)
 *                                  - Bytes 2 to 7: Key codes 1 to 6, representing keys pressed
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetBootKeyboardInputReport(uint8_t *p_keyCodeArray)
{
    (void)memcpy(s_hogpsDb.charValBootKbInputReport, p_keyCodeArray, HID_REPORT_LENGTH_BOOT_KB_INPUT);
    return MBA_RES_SUCCESS;
}


/**
 * @brief Sets the output report for a HID boot keyboard.
 *
* This function updates the LED status indicators on a HID boot keyboard, such as Num Lock,
 * Caps Lock, and Scroll Lock LEDs. The state of each LED is represented by individual bits
 * within the provided 'ledValue' parameter.
 * 
 * @param[in] ledValue              A bitmask where each bit corresponds to an LED on the keyboard:\n
 *                                  - Bit 0: Num Lock LED
 *                                  - Bit 1: Caps Lock LED
 *                                  - Bit 2: Scroll Lock LED
 *                                  - Bit 3: Compose LED
 *                                  - Bit 4: Kana LED
 *                                  Bits 5 to 7 are reserved and should be set to 0.
 *
 * @retval MBA_RES_SUCCESS          The output report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetBootKeyboardOutputReport(uint8_t ledValue)
{
    s_hogpsDb.charValBootKbOutputReport[0] = ledValue;
    return MBA_RES_SUCCESS;
}
#endif


/**
 * @brief Sets the HID boot mode mouse input report.
 *
 * This function prepares the input report for a mouse in HID boot mode with the current state of mouse buttons and position.
 * 
 * @param[in] buttons               Bitmask representing the state of the mouse buttons: *
 *                                  - Bit 0: Left button (1 if pressed, 0 if not)
 *                                  - Bit 1: Right button (1 if pressed, 0 if not)
 *                                  - Bit 2: Middle button (1 if pressed, 0 if not)
 * @param[in] xAxis                 The displacement of the mouse along the X-axis since the last report, in units.
 * @param[in] yAxis                 The displacement of the mouse along the Y-axis since the last report, in units.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SetBootMouseInputReport(uint8_t buttons, int8_t xAxis, int8_t yAxis)
{
    s_hogpsDb.charValBootMInputReport[0] = buttons;
    s_hogpsDb.charValBootMInputReport[1] = (uint8_t)xAxis;
    s_hogpsDb.charValBootMInputReport[2] = (uint8_t)yAxis;
    return MBA_RES_SUCCESS;
}

#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sends a HID keyboard input report.
 *
 * This function transmits a report containing keyboard keypress information
 * to a connected BLE host using the specified connection handle.
 * 
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] p_keyCodeArray        A pointer to an array of 8 bytes representing the HID report.
*                                   The array structure is as follows:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)
 *                                  - Byte 1: Reserved (must be 0)
 *                                  - Bytes 2-7: Key codes (up to 6 keys that are currently pressed)
 *
 * @retval MBA_RES_SUCCESS          The transmission of the input report was successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendKeyboardInputReport(uint16_t connHandle, uint8_t *p_keyCodeArray)
{
    GATTS_HandleValueParams_T hvParams;
    uint16_t result;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    if ((result  = BLE_HOGPS_SetKeyboardInputReport(p_keyCodeArray)) != MBA_RES_SUCCESS)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = HIDS_HDL_CHARVAL_KB_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_KB_INPUT;
    (void)memcpy(hvParams.charValue, s_hogpsDb.charValKbInputReport, HID_REPORT_LENGTH_KB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#ifdef HIDS_MOUSE_SUPPORT
/**
 * @brief Sends a HID mouse button input report.
 *
 * This function transmits a report indicating the state of mouse buttons, the scroll wheel, and the AC pan.
 * 
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] buttons               A bitmask representing the state of each button (bit 0 for button 1, bit 1 for button 2, etc.).
 * @param[in] wheel                 The vertical scroll amount, where positive values indicate upward scrolling. Range: -127 to 127.
 * @param[in] acPan                 The horizontal scroll amount, where positive values indicate rightward scrolling. Range: -127 to 127.
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendMouseButtonInputReport(uint16_t connHandle, uint8_t buttons, int8_t wheel, int8_t acPan)
{
    GATTS_HandleValueParams_T hvParams;
    uint16_t result;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    if ((result  = BLE_HOGPS_SetMouseButtonInputReport(buttons, wheel, acPan)) != MBA_RES_SUCCESS)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = (uint16_t)HIDS_HDL_CHARVAL_MB_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_MB_INPUT;
    (void)memcpy(hvParams.charValue, s_hogpsDb.charValMbInputReport, HID_REPORT_LENGTH_MB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}


/**
 * @brief Sends a HID mouse motion input report.
 *
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] xAxis                 The movement along the X-axis. Range: -2047 to 2047.
 * @param[in] yAxis                 The movement along the Y-axis. Range: -2047 to 2047.
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendMouseMotionInputReport(uint16_t connHandle, int16_t xAxis, int16_t yAxis)
{
    GATTS_HandleValueParams_T hvParams;
    uint16_t result;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    if ((result  = BLE_HOGPS_SetMouseMotionInputReport(xAxis, yAxis)) != MBA_RES_SUCCESS)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = (uint16_t)HIDS_HDL_CHARVAL_MM_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_MM_INPUT;
    (void)memcpy(hvParams.charValue, s_hogpsDb.charValMmInputReport, HID_REPORT_LENGTH_MM_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sends a HID boot keyboard input report.
 * 
 * This function transmits a keyboard input report in the boot protocol mode. The report consists of
 * an array of key codes that represent the keys currently pressed. The first byte of the array
 * indicates modifier keys (such as Shift, Ctrl, etc.), followed by a reserved byte, and then up to
 * six key codes representing the keys being pressed simultaneously.
 *
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] p_keyCodeArray        A pointer to an 8-byte array containing the key report. The array is
 *                                  structured as follows:\n
 *                                  - Byte 0: Modifier keys bit mask
 *                                  - Byte 1: Reserved (typically 0x00)
 *                                  - Bytes 2-7: Key codes (up to 6 keys)
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBootKeyboardInputReport(uint16_t connHandle, uint8_t *p_keyCodeArray)
{
    GATTS_HandleValueParams_T hvParams;
    uint16_t result;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    if ((result  = BLE_HOGPS_SetBootKeyboardInputReport(p_keyCodeArray)) != MBA_RES_SUCCESS)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_BOOT_KB_INPUT;
    (void)memcpy(hvParams.charValue, s_hogpsDb.charValBootKbInputReport, HID_REPORT_LENGTH_BOOT_KB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sends a HID boot mouse input report.
 *
 * This function transmits the current state of the mouse buttons along with the X and Y axis movement values.
 * 
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] buttons               Mouse button states encoded as bit fields:\n
 *                                  - Bit 0: Left button
 *                                  - Bit 1: Right button
 *                                  - Bit 2: Middle button
 *                                  A value of 1 indicates the button is pressed; 0 indicates it is not pressed.
 * @param[in] xAxis                 The displacement value along the X-axis since the last report.
 * @param[in] yAxis                 The displacement value along the Y-axis since the last report.
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBootMouseInputReport(uint16_t connHandle, uint8_t buttons, int8_t xAxis, int8_t yAxis)
{
    GATTS_HandleValueParams_T hvParams;
    uint16_t result;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    if ((result  = BLE_HOGPS_SetBootMouseInputReport(buttons, xAxis, yAxis)) != MBA_RES_SUCCESS)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = (uint16_t)HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_BOOT_M_INPUT;
    (void)memcpy(hvParams.charValue, s_hogpsDb.charValBootMInputReport, HID_REPORT_LENGTH_BOOT_M_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif


/**
 * @brief Sends the battery level of a HID device using the BLE Battery Service.
 *
 * @param[in] connHandle            The connection handle associated with the connection.
 *
 * @retval MBA_RES_SUCCESS          The battery level was successfully sent.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBatteryLevel(uint16_t connHandle)
{
    GATTS_HandleValueParams_T hvParams;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = (uint16_t)BAS_HDL_CHARVAL_BAT_LEVEL;
    hvParams.charLength = 0x01;
    (void)BLE_BAS_GetBatteryLevel(hvParams.charValue);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}

