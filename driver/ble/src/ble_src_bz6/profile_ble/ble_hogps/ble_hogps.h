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
  BLE HID over GATT profile server role Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_hogps.h

  Summary:
    Interface for the BLE HID Over GATT Profile Server role functionality.

  Description:
    This header file provides the interface definitions and function prototypes
    for the BLE HID Over GATT Profile server role, enabling the user to implement
    the HID service over a BLE connection.
 *******************************************************************************/
#ifndef BLE_HOGPS_H
#define BLE_HOGPS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stack_mgr.h"
#include "ble_hids/ble_hids.h"

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
 * @defgroup BLE_HOGPS BLE HID Over GATT Profile Server
 * 
 * @brief Provides the interface for the BLE HID over GATT profile server (HOPGS).
 * @note Provides definitions, types, and function prototypes necessary to
 *          implement the server side of the HID over GATT Profile, facilitating
 *          communication between BLE devices in the context of HID services.
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_HOGPS_DEFINES Defines
 * @{
 */

/** @} */ //BLE_HOGPS_DEFINES

/**
 * @addtogroup BLE_HOGPS_ENUMS Enumerations
 * @{
 */

/** @brief Enumeration for BLE HID over GATT Profile (HOGP) service callback events. */
typedef enum BLE_HOGPS_EventId_T
{
    BLE_HOGPS_EVT_BOOT_MODE_ENTER_IND = 0x00U,                  /**< Event indicating the device has entered HID Boot Mode. See @ref BLE_HOGPS_EvtBootModeEnter_T for event details.*/
    BLE_HOGPS_EVT_REPORT_MODE_ENTER_IND,                        /**< Event indicating the device has entered HID Report Mode. See @ref BLE_HOGPS_EvtReportModeEnter_T for event details.*/
    BLE_HOGPS_EVT_HOST_SUSPEND_ENTER_IND,                       /**< Event indicating the HID host is entering suspend mode. See @ref BLE_HOGPS_EvtHostSuspendEnter_T for event details.*/
    BLE_HOGPS_EVT_HOST_SUSPEND_EXIT_IND,                        /**< Event indicating the HID host is exiting suspend mode. See @ref BLE_HOGPS_EvtHostSuspendExit_T for event details.*/
    BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND,                       /**< Event indicating an HID Input Report has been written. See @ref BLE_HOGPS_EvtReportWrite_T for event details.*/
    BLE_HOGPS_EVT_OUTPUT_REPORT_WRITE_IND,                      /**< Event indicating an HID Output Report has been written. See @ref BLE_HOGPS_EvtReportWrite_T for event details.*/
    BLE_HOGPS_EVT_FEATURE_REPORT_WRITE_IND,                     /**< Event indicating an HID Feature Report has been written. See @ref BLE_HOGPS_EvtReportWrite_T for event details.*/
    BLE_HOGPS_EVT_BOOT_KB_INPUT_REPORT_WRITE_IND,               /**< Event indicating a Boot Keyboard Input Report has been written. See @ref BLE_HOGPS_EvtBootReportWrite_T for event details.*/
    BLE_HOGPS_EVT_BOOT_KB_OUTPUT_REPORT_WRITE_IND,              /**< Event indicating a Boot Keyboard Output Report has been written. See @ref BLE_HOGPS_EvtBootReportWrite_T for event details.*/
    BLE_HOGPS_EVT_BOOT_M_INPUT_REPORT_WRITE_IND,                /**< Event indicating a Boot Mouse Input Report has been written. See @ref BLE_HOGPS_EvtBootReportWrite_T for event details.*/
    BLE_HOGPS_EVT_END                                           /**< Sentinel value indicating the end of the event enumeration. */
} BLE_HOGPS_EventId_T;


/** @} */ //BLE_HOGPS_ENUMS

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_HOGPS_STRUCTS Structures
 * @{
 */

/** @brief Structure for the @ref BLE_HOGPS_EVT_BOOT_MODE_ENTER_IND event. */
typedef struct BLE_HOGPS_EvtBootModeEnter_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
} BLE_HOGPS_EvtBootModeEnter_T;


/** @brief Structure for the @ref BLE_HOGPS_EVT_REPORT_MODE_ENTER_IND event. */
typedef struct BLE_HOGPS_EvtReportModeEnter_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
} BLE_HOGPS_EvtReportModeEnter_T;


/** @brief Structure for the @ref BLE_HOGPS_EVT_HOST_SUSPEND_ENTER_IND event. */
typedef struct BLE_HOGPS_EvtHostSuspendEnter_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
} BLE_HOGPS_EvtHostSuspendEnter_T;


/** @brief Structure for the @ref BLE_HOGPS_EVT_HOST_SUSPEND_EXIT_IND event. */
typedef struct BLE_HOGPS_EvtHostSuspendExit_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
} BLE_HOGPS_EvtHostSuspendExit_T;


/** @brief Structure for the @ref BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND, @ref BLE_HOGPS_EVT_OUTPUT_REPORT_WRITE_IND, @ref BLE_HOGPS_EVT_FEATURE_REPORT_WRITE_IND event. */
typedef struct BLE_HOGPS_EvtReportWrite_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
    uint16_t                            length;                 /**< Length of the report data written. */
    uint16_t                            offset;                 /**< Offset where the report data is written. */
    uint8_t                             reportId;               /**< Identifier of the report. */
    uint8_t                             *p_data;                /**< Pointer to the report data. */
} BLE_HOGPS_EvtReportWrite_T;


/** @brief Structure for the @ref BLE_HOGPS_EVT_BOOT_KB_INPUT_REPORT_WRITE_IND, @ref BLE_HOGPS_EVT_BOOT_KB_OUTPUT_REPORT_WRITE_IND, @ref BLE_HOGPS_EVT_BOOT_M_INPUT_REPORT_WRITE_IND event. */
typedef struct BLE_HOGPS_EvtBootReportWrite_T
{
    uint16_t                            connHandle;             /**< The connection handle associated with the connection. */
    uint16_t                            length;                 /**< Length of the report data written. */
    uint16_t                            offset;                 /**< Offset where the report data is written. */
    uint8_t                             *p_data;                /**< Pointer to the report data. */
} BLE_HOGPS_EvtBootReportWrite_T;


/** @brief Union of BLE HID over GATT profile server role callback event data types. */
typedef union
{
    BLE_HOGPS_EvtBootModeEnter_T        evtBootModeEnter;               /**< Data for @ref BLE_HOGPS_EVT_BOOT_MODE_ENTER_IND event. */
    BLE_HOGPS_EvtReportModeEnter_T      evtReportModeEnter;             /**< Data for @ref BLE_HOGPS_EVT_REPORT_MODE_ENTER_IND event. */
    BLE_HOGPS_EvtHostSuspendEnter_T     evtHostSuspendEnter;            /**< Data for @ref BLE_HOGPS_EVT_HOST_SUSPEND_ENTER_IND event. */
    BLE_HOGPS_EvtHostSuspendExit_T      evtHostSuspendExit;             /**< Data for @ref BLE_HOGPS_EVT_HOST_SUSPEND_EXIT_IND event. */
    BLE_HOGPS_EvtReportWrite_T          evtInputReportWrite;            /**< Data for @ref BLE_HOGPS_EVT_INPUT_REPORT_WRITE_IND event.*/
    BLE_HOGPS_EvtReportWrite_T          evtOutputReportWrite;           /**< Data for @ref BLE_HOGPS_EVT_OUTPUT_REPORT_WRITE_IND event. */
    BLE_HOGPS_EvtReportWrite_T          evtFeatureReportWrite;          /**< Data for @ref BLE_HOGPS_EVT_FEATURE_REPORT_WRITE_IND event. */
    BLE_HOGPS_EvtBootReportWrite_T      evtBootKbOutputReportWrite;     /**< Data for @ref BLE_HOGPS_EVT_BOOT_KB_INPUT_REPORT_WRITE_IND event. */
    BLE_HOGPS_EvtBootReportWrite_T      evtBootKbInputReportWrite;      /**< Data for @ref BLE_HOGPS_EVT_BOOT_KB_OUTPUT_REPORT_WRITE_IND event. */
    BLE_HOGPS_EvtBootReportWrite_T      evtBootMouseInputReportWrite;   /**< Data for @ref BLE_HOGPS_EVT_BOOT_M_INPUT_REPORT_WRITE_IND event. */
} BLE_HOGPS_EventField_T;


/** @brief BLE HID over GATT profile server role callback event structure. */
typedef struct  BLE_HOGPS_Event_T
{
    BLE_HOGPS_EventId_T                 eventId;            /**< Identifier for the type of event.  See @ref BLE_HOGPS_EventId_T. */
    
    BLE_HOGPS_EventField_T              eventField;         /**< Data associated with the event. See @ref BLE_HOGPS_EventField_T. */
} BLE_HOGPS_Event_T;



/**
 * @brief Callback type for BLE HID over GATT profile server role events.
 * 
 * This callback function is used to send BLE HID over GATT profile server role events to the application.
 * 
 * @param p_event Pointer to the event structure containing event data.
 */
typedef void(*BLE_HOGPS_EventCb_T)(BLE_HOGPS_Event_T *p_event);

/** @} */ //BLE_HOGPS_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 * @addtogroup BLE_HOGPS_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the BLE HID over GATT profile server role.
 *
 * @retval MBA_RES_SUCCESS          The initialization was successful.
 * @retval MBA_RES_FAIL             The initialization failed.
 *
 */
uint16_t BLE_HOGPS_Init(void);


/**
 * @brief Registers a callback for the BLE HID over GATT profile server role events.
 *
 * @param[in] routine               Callback function to handle BLE HID over GATT profile server role events.
 *
 */
void BLE_HOGPS_EventRegister(BLE_HOGPS_EventCb_T routine);


/**
 * @brief Handles BLE_Stack events.
 * 
 * @note This function should be called when BLE stack events occur.
 *
 * @param[in] p_stackEvent          Pointer to the BLE stack event data.
 *
*/
void BLE_HOGPS_BleEventHandler(STACK_Event_T *p_stackEvent);


#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sets HID keyboard input report.
 * 
 * @note This function prepares the input report for a keyboard device in the Human Interface Device (HID) profile. 
 *          The report includes modifier keys, reserved byte, and up to six keycodes representing the keys that are pressed.
 *
 * @param[in] p_keyCodeArray        An array of 8 bytes representing the keyboard report. The array is structured as follows:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)\n
 *                                  - Byte 1: Reserved (typically 0x00)\n
 *                                  - Bytes 2 to 7: Keycodes 1 to 6, indicating keys that are currently pressed
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetKeyboardInputReport(uint8_t *p_keyCodeArray);
#endif


/**
 * @brief Sets the HID mouse button input report.
 * 
 * @note This function prepares the input report for mouse button states, wheel movement, and AC pan in the HID profile. 
 *          It indicates which mouse buttons are pressed, the vertical scroll wheel movement, and the horizontal scroll (AC Pan).
 *
 * @param[in] buttons               A bitmask representing the state of mouse buttons. Each bit corresponds to a button:\n
 *                                  - Bit 0 to Bit 4: Indicate whether each button is pressed (1) or not (0)
 * @param[in] wheel                 The vertical scroll amount with a valid range from -127 to 127.
 * @param[in] acPan                 The horizontal scroll (AC Pan) amount with a valid range from -127 to 127.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetMouseButtonInputReport(uint8_t buttons, int8_t wheel, int8_t acPan);


/**
 * @brief Sets the HID mouse motion input report.
 *
 * @note This function prepares the input report for mouse motion in the HID profile. 
 *          It specifies the relative movement of the mouse along the X and Y axes.
 * 
 * @param[in] xAxis                 The relative movement on the X-Axis with a valid range from -2047 to 2047.
 * @param[in] yAxis                 The relative movement on the Y-Axis with a valid range from -2047 to 2047.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetMouseMotionInputReport(int16_t xAxis, int16_t yAxis);


#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sets the HID keyboard output report.
 *
 * @note This function updates the LED status indicators for a HID keyboard, such as Num Lock, Caps Lock, 
 *          and Scroll Lock LEDs. The state of each LED is represented by individual bits within the provided 
 *          'ledValue' parameter.
 * 
 * @param[in] ledValue              A 8-bit value where each bit corresponds to an LED on the keyboard:\n
 *                                  - Bit 0: Num Lock LED\n
 *                                  - Bit 1: Caps Lock LED\n
 *                                  - Bit 2: Scroll Lock LED\n
 *                                  - Bit 3: Compose LED\n
 *                                  - Bit 4: Kana LED\n
 *                                  The remaining bits (5-7) are reserved and should be set to 0.
 *
 * @retval MBA_RES_SUCCESS          The output report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetKeyboardOutputReport(uint8_t ledValue);
#endif


#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sets the HID boot keyboard input report.
 *
 * @note This function updates the input report for a boot keyboard with the specified key codes. The input report 
 *          format follows the standard boot protocol layout.
 * 
 * @param[in] p_keyCodeArray        An array representing the keyboard report. The array is expected 
 *                                  to be 8 bytes in length, with the following structure:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)\n
 *                                  - Byte 1: Reserved (typically 0x00)\n
 *                                  - Bytes 2 to 7: Key codes 1 to 6, representing keys pressed
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetBootKeyboardInputReport(uint8_t *p_keyCodeArray);
#endif


#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sets the output report for a HID boot keyboard.
 *
 * @note This function updates the LED status indicators on a HID boot keyboard, such as Num Lock, Caps Lock, 
 *          and Scroll Lock LEDs. The state of each LED is represented by individual bits within the provided 
 *          'ledValue' parameter.
 * 
 * @param[in] ledValue              A bitmask where each bit corresponds to an LED on the keyboard:\n
 *                                  - Bit 0: Num Lock LED\n
 *                                  - Bit 1: Caps Lock LED\n
 *                                  - Bit 2: Scroll Lock LED\n
 *                                  - Bit 3: Compose LED\n
 *                                  - Bit 4: Kana LED\n
 *                                  Bits 5 to 7 are reserved and should be set to 0.
 *
 * @retval MBA_RES_SUCCESS          The output report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The input parameters are invalid.
*/
uint16_t BLE_HOGPS_SetBootKeyboardOutputReport(uint8_t ledValue);
#endif


/**
 * @brief Sets the HID boot mode mouse input report.
 *
 * @note This function prepares the input report for a mouse in HID boot mode with the current state of mouse buttons and position.
 * 
 * @param[in] buttons               Bitmask representing the state of the mouse buttons:\n
 *                                  - Bit 0: Left button (1 if pressed, 0 if not)\n
 *                                  - Bit 1: Right button (1 if pressed, 0 if not)\n
 *                                  - Bit 2: Middle button (1 if pressed, 0 if not)
 * @param[in] xAxis                 The displacement of the mouse along the X-axis since the last report, in units.
 * @param[in] yAxis                 The displacement of the mouse along the Y-axis since the last report, in units.
 *
 * @retval MBA_RES_SUCCESS          The input report was successfully set.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SetBootMouseInputReport(uint8_t buttons, int8_t xAxis, int8_t yAxis);


#ifdef HIDS_KEYBOARD_SUPPORT
/**
 * @brief Sends a HID keyboard input report.
 *
 * @note This function transmits a report containing keyboard keypress information  to a connected BLE host using 
 *          the specified connection handle.
 * 
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] p_keyCodeArray        A pointer to an array of 8 bytes representing the HID report.
*                                   The array structure is as follows:\n
 *                                  - Byte 0: Modifier keys (e.g., Shift, Ctrl)\n
 *                                  - Byte 1: Reserved (must be 0)\n
 *                                  - Bytes 2-7: Key codes (up to 6 keys that are currently pressed)
 *
 * @retval MBA_RES_SUCCESS          The transmission of the input report was successful.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendKeyboardInputReport(uint16_t connHandle, uint8_t *p_keyCodeArray);
#endif


/**
 * @brief Sends a HID mouse button input report.
 *
 * @note This function transmits a report indicating the state of mouse buttons, the scroll wheel, and the AC pan.
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
uint16_t BLE_HOGPS_SendMouseButtonInputReport(uint16_t connHandle, uint8_t buttons, int8_t wheel, int8_t acPan);


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
uint16_t BLE_HOGPS_SendMouseMotionInputReport(uint16_t connHandle, int16_t xAxis, int16_t yAxis);


#if defined(HIDS_KEYBOARD_SUPPORT) && defined (HIDS_BOOT_PROTOCOL_MODE_SUPPORT)
/**
 * @brief Sends a HID boot keyboard input report.
 * 
 * @note This function transmits a keyboard input report in the boot protocol mode. The report consists of 
 *          an array of key codes that represent the keys currently pressed. The first byte of the array 
 *          indicates modifier keys (such as Shift, Ctrl, etc.), followed by a reserved byte, and then up to 
 *          six key codes representing the keys being pressed simultaneously.
 *
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] p_keyCodeArray        A pointer to an 8-byte array containing the key report. The array is
 *                                  structured as follows:\n
 *                                  - Byte 0: Modifier keys bit mask\n
 *                                  - Byte 1: Reserved (typically 0x00)\n
 *                                  - Bytes 2-7: Key codes (up to 6 keys)
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBootKeyboardInputReport(uint16_t connHandle, uint8_t *p_keyCodeArray);
#endif

/**
 * @brief Sends a HID boot mouse input report.
 *
 * @note This function transmits the current state of the mouse buttons along with the X and Y axis movement values.
 * 
 * @param[in] connHandle            The connection handle associated with the connection.
 * @param[in] buttons               Mouse button states encoded as bit fields:\n
 *                                  - Bit 0: Left button\n
 *                                  - Bit 1: Right button\n
 *                                  - Bit 2: Middle button\n
 *                                  A value of 1 indicates the button is pressed; 0 indicates it is not pressed.
 * @param[in] xAxis                 The displacement value along the X-axis since the last report.
 * @param[in] yAxis                 The displacement value along the Y-axis since the last report.
 *
 * @retval MBA_RES_SUCCESS          The input report was sent successfully.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBootMouseInputReport(uint16_t connHandle, uint8_t buttons, int8_t xAxis, int8_t yAxis);


/**
 * @brief Sends the battery level of a HID device using the BLE Battery Service.
 *
 * @param[in] connHandle            The connection handle associated with the connection.
 *
 * @retval MBA_RES_SUCCESS          The battery level was successfully sent.
 * @retval MBA_RES_OOM              Internal memory allocation failure.
 * @retval MBA_RES_INVALID_PARA     The parameters provided are invalid.
*/
uint16_t BLE_HOGPS_SendBatteryLevel(uint16_t connHandle);

/** @} */ //BLE_HOGPS_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BLE_HOGPS_H