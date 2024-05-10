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
    app_ble_hpa_handler.c

  Summary:
    This file contains the Application BLE HPA functions for this project.

  Description:
    This file contains the Application BLE HPA functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include "peripheral/gpio/plib_gpio.h"
#include "bt_sys.h"



// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
static void app_HpaNotifyCpsCb(bool bypass)
{
    if(bypass == true)
    {
        GPIO_PortSet((GPIOA_BASE_ADDRESS + (0x100U * (17U >>4U))), (uint32_t)0x1U << (17U & 0xFU));
    }
    else
    {
        GPIO_PortClear((GPIOA_BASE_ADDRESS + (0x100U * (17U >>4U))), (uint32_t)0x1U << (17U & 0xFU));
    }
}

static void app_HpaNotifyCtxCb(bool active)
{
    if(active == false)
    {
        GPIO_PortClear((GPIOA_BASE_ADDRESS + (0x100U * (10U >>4U))), (uint32_t)0x1U << (10U & 0xFU));
    }
    else
    {
        GPIO_PortSet((GPIOA_BASE_ADDRESS + (0x100U * (10U >>4U))), (uint32_t)0x1U << (10U & 0xFU));
    }
}

void APP_HpaInit(void)
{
    BT_SYS_HpaInit(app_HpaNotifyCpsCb, app_HpaNotifyCtxCb);
}
