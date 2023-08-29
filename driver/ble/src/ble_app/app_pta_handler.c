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
  Application BLE PTA Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_pta_handler.c

  Summary:
    This file contains the Application BLE PTA functions for this project.

  Description:
    This file contains the Application BLE PTA functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include "peripheral/gpio/plib_gpio.h"
#include "bt_sys_pta.h"



// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void APP_PtaNotifyBtActiveCb(bool active)
{
    if(active == true)
    {
        BT_ACTIVE_Set();
    }
    else
    {
        BT_ACTIVE_Clear();
    }
}
static void APP_PtaNotifyBtPriorityCb(bool priority)
{
    if(priority == true)//high priority
    {
        BT_PRIORITY_Set();
    }
    else
    {
        BT_PRIORITY_Clear();
    }
}

static bool APP_PtaGetWlanActiveStatus(void)
{
    if(0x1U == WLAN_ACTIVE_Get())
    {
        return true;
    }

    return false;
}
static bool APP_PtaGetWlanActiveStatusCb(void)
{
    return APP_PtaGetWlanActiveStatus();
}

static void APP_PtaWlanActiveIrqHandler(GPIO_PIN pin, uintptr_t context)
{
    if(true == APP_PtaGetWlanActiveStatus())
    {
        BT_SYS_PtaWlanActiveIrqHandler();
    }
}

void APP_PtaInit()
{
    BT_SYS_PtaInit(APP_PtaNotifyBtActiveCb, APP_PtaNotifyBtPriorityCb, APP_PtaGetWlanActiveStatusCb);
    /* GPIO Initialization */
    GPIO_PinInterruptCallbackRegister(WLAN_ACTIVE_PIN, APP_PtaWlanActiveIrqHandler, 0);
    WLAN_ACTIVE_InterruptEnable();
}
