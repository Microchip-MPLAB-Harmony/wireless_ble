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
  Application BLE Profile Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_anpc_handler.c

  Summary:
    This file contains the Application BLE functions for this project.

  Description:
    This file contains the Application BLE functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include <stdint.h>
#include "ble_anpc/ble_anpc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_AnpcEvtHandler(BLE_ANPC_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_ANPC_EVT_DISC_COMPLETE_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_SUPP_NEW_ALERT_CAT_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_SUPP_UNREAD_ALERT_STAT_CAT_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_WRITE_NEW_ALERT_NTFY_RSP_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_WRITE_UNREAD_ALERT_NTFY_RSP_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_WRITE_CP_RSP_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_NEW_ALERT_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_ERR_UNSPECTIFIED_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_ANPC_EVT_ERR_NO_MEM_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

