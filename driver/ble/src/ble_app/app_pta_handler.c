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
#include "bt_sys_pta.h"
#include "pta.h"

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static volatile bool s_blePtaReqSet;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void APP_PtaWlanActiveIrqHandler(void)
{
    if((PTA_WlanStatus() == true) && (s_blePtaReqSet == true))
    {
        BT_SYS_PtaWlanActiveIrqHandler();
    }
}

static void APP_PtaNotifyBtActiveCb(bool active)
{
    if((active == true) && (PTA_GetReq() == false))
    {
        PTA_ReqSet(APP_PtaWlanActiveIrqHandler);
        s_blePtaReqSet = true;
    }
    if(active == false)
    {
        PTA_ClearIrqHandler();
        PTA_ReqClear();
        s_blePtaReqSet = false;
    }
}

static void APP_PtaNotifyBtPriorityCb(bool priority)
{
    if((priority == true) && (PTA_GetReq() == false))
    {
        PTA_PrioSet();
    }

    if(priority == false)
    {
        PTA_PrioClear();
    }
}

static bool APP_PtaGetWlanActiveStatusCb(void)
{
    return PTA_WlanStatus();
}

void APP_PtaInit(void)
{
    s_blePtaReqSet = false;
    BT_SYS_PtaInit(APP_PtaNotifyBtActiveCb, APP_PtaNotifyBtPriorityCb, APP_PtaGetWlanActiveStatusCb);
}
