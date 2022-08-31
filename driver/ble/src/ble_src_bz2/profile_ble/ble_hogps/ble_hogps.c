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
    This file contains the BLE HID over GATT profile server role functions for application user.

  Description:
    This file contains the BLE HID over GATT profile server role functions for application user.
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
#include "ble_hogps/ble_hogps.h"

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

/**@brief The structure contains information about BLE HOGPS connection parameters. */
typedef struct BLE_HOGPS_ConnParams_T
{
    uint16_t                    connHandle;                                                         /**< Connection handle associated with this connection. */
    uint8_t                     protocolMode;                                                       /**< Record current protocol mode. */
    uint8_t                     suspendStatus;                                                      /**< Record current suspend status. */
} BLE_HOGPS_ConnParams_T;

/**@brief The structure contains information about BLE HOGPS database. */
typedef struct BLE_HOGPS_Database_T
{
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint16_t                    charLengthKbInputReport;                                            /**< Length of Keyboard Input Report value. */
    #endif
    uint16_t                    charLengthMbInputReport;                                            /**< Length of Mouse Buttons Input Report value. */
    uint16_t                    charLengthMmInputReport;                                            /**< Length of Mouse Motion Input Report value. */
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint16_t                    charLengthKbOutputReport;                                           /**< Length of Keyboard Output Report value. */
    #endif
    #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    uint16_t                    charLengthBootKbInputReport;                                        /**< Length of Boot Keyboard Input Report value. */
    uint16_t                    charLengthBootKbOutputReport;                                       /**< Length of Boot Keyboard Output Report value. */
    #endif
    uint16_t                    charLengthBootMInputReport;                                         /**< Length of Boot Mouse Input Report value. */
    #ifdef HIDS_KEYBOARD_SUPPORT
    uint8_t                     charValKbInputReport[HID_REPORT_LENGTH_KB_INPUT];                   /**< Keyboard Input Report value. */
    #endif
    uint8_t                     charValMbInputReport[HID_REPORT_LENGTH_MB_INPUT];                   /**< Mouse Buttons Input Report value. */
    uint8_t                     charValMmInputReport[HID_REPORT_LENGTH_MM_INPUT];                   /**< Mouse Motion Input Report value. */
    #ifdef  HIDS_KEYBOARD_SUPPORT
    uint8_t                     charValKbOutputReport[HID_REPORT_LENGTH_KB_OUTPUT];                 /**< Keyboard Output Report value. */
    #endif
    #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
    uint8_t                     charValBootKbInputReport[HID_REPORT_LENGTH_BOOT_KB_INPUT];          /**< Boot Keyboard Input Report value. */
    uint8_t                     charValBootKbOutputReport[HID_REPORT_LENGTH_BOOT_KB_OUTPUT];        /**< Boot Keyboard Output Report value. */
    #endif
    uint8_t                     charValBootMInputReport[HID_REPORT_LENGTH_BOOT_M_INPUT];            /**< Boot Mouse Input Report value. */
} BLE_HOGPS_Database_T;

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static BLE_HOGPS_EventCb_T      sp_hogpsCbRoutine;      /* Variables for callback routines. */
static BLE_HOGPS_ConnParams_T   s_hogpsConnParams;      /* Connection parameters. */
static BLE_HOGPS_Database_T     s_hogpsDb;              /* Database. */

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static void ble_hogps_init_db()
{
    memset((uint8_t *)&s_hogpsDb, 0x00, sizeof(BLE_HOGPS_Database_T));
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

static void ble_hogps_ConveyEvent(BLE_HOGPS_Event_T *p_event)
{
    if (sp_hogpsCbRoutine)
    {
        sp_hogpsCbRoutine(p_event);
    }
}

static void ble_hogps_GenerateReportModeWriteEvent(GATT_Event_T *p_event, uint8_t eventId, uint8_t reportId)
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
static void ble_hogps_GenerateBootModeWriteEvent(GATT_Event_T *p_event, uint8_t eventId)
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
        break;
    }
}

static void ble_hogps_GattEventProcess(GATT_Event_T *p_event)
{
    switch (p_event->eventId)
    {
        case GATTS_EVT_READ:
        {
            GATTS_SendReadRespParams_T readResponse;

            if (p_event->eventField.onRead.readType == ATT_READ_REQ)
            {
                readResponse.responseType = ATT_READ_RSP;
            }
            else
            {
                readResponse.responseType = ATT_READ_BLOB_RSP;
            }

            #ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_HID_PROTOCOL_MODE)
            {
                readResponse.attrLength = 0x01;
                readResponse.attrValue[0] = s_hogpsConnParams.protocolMode;
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_KB_INPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthKbInputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValKbInputReport, s_hogpsDb.charLengthKbInputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
            #ifdef HIDS_MOUSE_SUPPORT
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_MB_INPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthMbInputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValMbInputReport, s_hogpsDb.charLengthMbInputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            else if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_MM_INPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthMmInputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValMmInputReport, s_hogpsDb.charLengthMmInputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_KB_OUTPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthKbOutputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValKbOutputReport, s_hogpsDb.charLengthKbOutputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthBootKbInputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValBootKbInputReport, s_hogpsDb.charLengthBootKbInputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            else if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_BOOT_KB_OUTPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthBootKbOutputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValBootKbOutputReport, s_hogpsDb.charLengthBootKbOutputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
            #if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            if (p_event->eventField.onRead.attrHandle == HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT)
            {
                readResponse.attrLength = s_hogpsDb.charLengthBootMInputReport;
                memcpy(readResponse.attrValue, s_hogpsDb.charValBootMInputReport, s_hogpsDb.charLengthBootMInputReport);
                GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, &readResponse);
            }
            #endif
        }
        break;

        case GATTS_EVT_WRITE:
        {
            GATTS_SendWriteRespParams_T writeResp;
            GATTS_SendErrRespParams_T   errorResp;

            /* Only write characteristic value procedure is mandatory in HID */
            writeResp.responseType = ATT_WRITE_RSP;
            errorResp.reqOpcode = ATT_WRITE_REQ;
            errorResp.attrHandle = p_event->eventField.onWrite.attrHandle;

            if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_HID_CTRL)
            {
                BLE_HOGPS_Event_T hogpsEvent;

                if (p_event->eventField.onWrite.writeDataLength == 0x01)
                {
                    if (p_event->eventField.onWrite.writeValue[0] == HID_HOST_SUSPEND_ENTER)
                    {
                        /* Enter boot mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_BOOT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_HOST_SUSPEND_ENTER_IND;
                        hogpsEvent.eventField.evtHostSuspendEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else if (p_event->eventField.onWrite.writeValue[0] == HID_HOST_SUSPEND_EXIT)
                    {
                        /* Enter report mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_REPORT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_HOST_SUSPEND_EXIT_IND;
                        hogpsEvent.eventField.evtHostSuspendExit.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                }
            }
            #ifdef HIDS_BOOT_PROTOCOL_MODE_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_HID_PROTOCOL_MODE)
            {
                BLE_HOGPS_Event_T hogpsEvent;

                if (p_event->eventField.onWrite.writeDataLength == 0x01)
                {
                    if (p_event->eventField.onWrite.writeValue[0] == HID_MODE_BOOT_PROTOCOL)
                    {
                        /* Enter boot mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_BOOT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_BOOT_MODE_ENTER_IND;
                        hogpsEvent.eventField.evtBootModeEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                    else if (p_event->eventField.onWrite.writeValue[0] == HID_MODE_REPORT_PROTOCOL)
                    {
                        /* Enter report mode */
                        s_hogpsConnParams.protocolMode = HID_MODE_REPORT_PROTOCOL;
                        hogpsEvent.eventId = BLE_HOGPS_EVT_REPORT_MODE_ENTER_IND;
                        hogpsEvent.eventField.evtReportModeEnter.connHandle = p_event->eventField.onWrite.connHandle;
                        ble_hogps_ConveyEvent(&hogpsEvent);
                    }
                }
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_KB_INPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_KB_INPUT)
                {
                    s_hogpsDb.charLengthKbInputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValKbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                }
                else
                {
                    errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                }
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_KB);
            }
            #endif
            #ifdef HIDS_MOUSE_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_MB_INPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_MB_INPUT)
                {
                    s_hogpsDb.charLengthMbInputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValMbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                }
                else
                {
                    errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                }
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_MB);
            }
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_MM_INPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_MM_INPUT)
                {
                    s_hogpsDb.charLengthMmInputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValMmInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                }
                else
                {
                    errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                }
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, HID_REPORT_ID_MM);
            }
            #endif
            #ifdef HIDS_KEYBOARD_SUPPORT
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_KB_OUTPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_KB_OUTPUT)
                {
                    s_hogpsDb.charLengthKbOutputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValKbOutputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    if (p_event->eventField.onWrite.writeType != ATT_WRITE_CMD)
                    {
                        GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                    }
                }
                else
                {
                    if (p_event->eventField.onWrite.writeType != ATT_WRITE_CMD)
                    {
                        errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                        GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                    }
                }
                ble_hogps_GenerateReportModeWriteEvent(p_event, BLE_HOGPS_EVT_OUTPUT_REPORT_WRITE_IND, HID_REPORT_ID_KB);
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_BOOT_KB_INPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_BOOT_KB_INPUT)
                {
                    s_hogpsDb.charLengthBootKbInputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValBootKbInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                }
                else
                {
                    errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                }
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_KB_INPUT_REPORT_WRITE_IND);
            }
            #endif
            #if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_BOOT_KB_OUTPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_BOOT_KB_OUTPUT)
                {
                    s_hogpsDb.charLengthBootKbOutputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValBootKbOutputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    if (p_event->eventField.onWrite.writeType != ATT_WRITE_CMD)
                    {
                        GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                    }
                }
                else
                {
                    if (p_event->eventField.onWrite.writeType != ATT_WRITE_CMD)
                    {
                        errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                        GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                    }
                }
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_KB_OUTPUT_REPORT_WRITE_IND);
            }
            #endif
            #if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
            else if (p_event->eventField.onWrite.attrHandle == HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT)
            {
                if (p_event->eventField.onWrite.writeDataLength <= HID_REPORT_LENGTH_BOOT_M_INPUT)
                {
                    s_hogpsDb.charLengthBootMInputReport = p_event->eventField.onWrite.writeDataLength;
                    memcpy(s_hogpsDb.charValBootMInputReport, p_event->eventField.onWrite.writeValue, p_event->eventField.onWrite.writeDataLength);
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, &writeResp);
                }
                else
                {
                    errorResp.errorCode = ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, &errorResp);
                }
                ble_hogps_GenerateBootModeWriteEvent(p_event, BLE_HOGPS_EVT_BOOT_M_INPUT_REPORT_WRITE_IND);
            }
            #endif
        }
        break;

        default:
        break;
    }
}

uint16_t BLE_HOGPS_Init(void)
{
    sp_hogpsCbRoutine = NULL;
    ble_hogps_init_db();
    return BLE_HIDS_Add();
}

void BLE_HOGPS_EventRegister(BLE_HOGPS_EventCb_T routine)
{
    sp_hogpsCbRoutine = routine;
}

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
        break;
    }
}

#ifdef HIDS_KEYBOARD_SUPPORT
uint16_t BLE_HOGPS_SetKeyboardInputReport(uint8_t *p_keyCodeArray)
{
    memcpy(s_hogpsDb.charValKbInputReport, p_keyCodeArray, HID_REPORT_LENGTH_KB_INPUT);
    s_hogpsDb.charValKbInputReport[1] = 0x00;       // Constant (padding)
    return MBA_RES_SUCCESS;
}
#endif

uint16_t BLE_HOGPS_SetMouseButtonInputReport(uint8_t buttons, int8_t wheel, int8_t acPan)
{
    s_hogpsDb.charValMbInputReport[0] = buttons;
    s_hogpsDb.charValMbInputReport[1] = wheel;
    s_hogpsDb.charValMbInputReport[2] = acPan;
    return MBA_RES_SUCCESS;
}

uint16_t BLE_HOGPS_SetMouseMotionInputReport(int16_t xAxis, int16_t yAxis)
{
    s_hogpsDb.charValMmInputReport[0] = (uint8_t)(xAxis>>4);
    s_hogpsDb.charValMmInputReport[1] = (((xAxis&0x000F)<<4)|yAxis>>8);
    s_hogpsDb.charValMmInputReport[2] = (uint8_t)yAxis;
    return MBA_RES_SUCCESS;
}

#ifdef HIDS_KEYBOARD_SUPPORT
uint16_t BLE_HOGPS_SetKeyboardOutputReport(uint8_t ledValue)
{
    s_hogpsDb.charValKbOutputReport[0] = ledValue;
    return MBA_RES_SUCCESS;
}
#endif

#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
uint16_t BLE_HOGPS_SetBootKeyboardInputReport(uint8_t *p_keyCodeArray)
{
    memcpy(s_hogpsDb.charValBootKbInputReport, p_keyCodeArray, HID_REPORT_LENGTH_BOOT_KB_INPUT);
    return MBA_RES_SUCCESS;
}

uint16_t BLE_HOGPS_SetBootKeyboardOutputReport(uint8_t ledValue)
{
    s_hogpsDb.charValBootKbOutputReport[0] = ledValue;
    return MBA_RES_SUCCESS;
}
#endif

uint16_t BLE_HOGPS_SetBootMouseInputReport(uint8_t buttons, int8_t xAxis, int8_t yAxis)
{
    s_hogpsDb.charValBootMInputReport[0] = buttons;
    s_hogpsDb.charValBootMInputReport[1] = xAxis;
    s_hogpsDb.charValBootMInputReport[2] = yAxis;
    return MBA_RES_SUCCESS;
}

#ifdef HIDS_KEYBOARD_SUPPORT
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
    memcpy(hvParams.charValue, s_hogpsDb.charValKbInputReport, HID_REPORT_LENGTH_KB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#ifdef HIDS_MOUSE_SUPPORT
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

    hvParams.charHandle = HIDS_HDL_CHARVAL_MB_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_MB_INPUT;
    memcpy(hvParams.charValue, s_hogpsDb.charValMbInputReport, HID_REPORT_LENGTH_MB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}

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

    hvParams.charHandle = HIDS_HDL_CHARVAL_MM_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_MM_INPUT;
    memcpy(hvParams.charValue, s_hogpsDb.charValMmInputReport, HID_REPORT_LENGTH_MM_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
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
    memcpy(hvParams.charValue, s_hogpsDb.charValBootKbInputReport, HID_REPORT_LENGTH_BOOT_KB_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

#if defined(HIDS_MOUSE_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
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

    hvParams.charHandle = HIDS_HDL_CHARVAL_BOOT_M_INPUT_REPORT;
    hvParams.charLength = HID_REPORT_LENGTH_BOOT_M_INPUT;
    memcpy(hvParams.charValue, s_hogpsDb.charValBootMInputReport, HID_REPORT_LENGTH_BOOT_M_INPUT);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}
#endif

uint16_t BLE_HOGPS_SendBatteryLevel(uint16_t connHandle)
{
    GATTS_HandleValueParams_T hvParams;

    if (s_hogpsConnParams.connHandle != connHandle)
    {
        return MBA_RES_INVALID_PARA;
    }

    hvParams.charHandle = BAS_HDL_CHARVAL_BAT_LEVEL;
    hvParams.charLength = 0x01;
    BLE_BAS_GetBatteryLevel(hvParams.charValue);
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;
    return GATTS_SendHandleValue(connHandle, &hvParams);
}

