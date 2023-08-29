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
  Application BLE HCI Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_hci_handler.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef APP_BLE_HCI_H
#define APP_BLE_HCI_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>
#include <stdbool.h>


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define APP_HCI_TYPE_COMMAND            0x01    /* HCI type: HCI command */
#define APP_HCI_TYPE_ACL_DATA           0x02    /* HCI type: ACL data */
#define APP_HCI_TYPE_EVENT              0x03    /* HCI type: HCI event */
#define APP_HCI_TYPE_VENDOR_CMD         0x04    /* HCI type: Vendor command*/

typedef struct APP_HCI_HciEvent_T
{
    uint8_t                             *p_data;
    uint16_t                            evtLength;
} APP_HCI_HciEvent_T;

typedef struct APP_HCI_AclData_T
{
    uint16_t                            connHandle;
    uint16_t                            dataLength;
    uint8_t                             *p_data;
} APP_HCI_AclData_T;

typedef struct APP_HCI_VendorCommand_T
{
    uint8_t                             cmdId;
    uint8_t                             cmdLength;
    uint8_t                             *p_cmdParams;
} APP_HCI_VendorCommand_T;

typedef union
{
    APP_HCI_HciEvent_T                  hciEvent;
    APP_HCI_AclData_T                   aclData;
    APP_HCI_VendorCommand_T             vendorCmd;
} APP_HCI_CbParams_T;

/* Event structure for receiving HCI packets from BLE Stack */
typedef struct STACK_HCI_Cb_T
{
    uint8_t                 hciType;
    APP_HCI_CbParams_T      cbParams;
} STACK_HCI_Cb_T;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_BleStackInit()

  Summary:
     HCI function initialization.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.
*/
void APP_BleStackInit();

/*******************************************************************************
  Function:
    void APP_BleStackEvtHandler( STACK_HCI_Cb_T *p_stackHciCb )

  Summary:
     Function for handling APP_MSG_BLE_STACK_EVT message.

  Description:

  Precondition:

  Parameters:
    None.

  Returns:
    None.

*/
void APP_BleStackEvtHandler(STACK_HCI_Cb_T *p_stackHciCb);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* APP_BLE_HCI_H */


/*******************************************************************************
 End of File
 */
