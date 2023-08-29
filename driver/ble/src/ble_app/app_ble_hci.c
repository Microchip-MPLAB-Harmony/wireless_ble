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
  Application BLE HCI Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_hci.c

  Summary:
    This file contains the Application BLE HCI functions for this project.

  Description:
    This file contains the Application BLE HCI functions for this project.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "app_ble_hci.h"
#include "hci.h"

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

bool APP_HciEvtCb(uint16_t length, uint8_t *p_packet)
{
    APP_Msg_T   appMsg;
    uint8_t     *p_data;

    appMsg.msgId = APP_MSG_BLE_STACK_EVT;
    p_data = OSAL_Malloc(length);

    if (p_data)
    {
        memcpy(p_data, p_packet, length);
        ((STACK_HCI_Cb_T *)appMsg.msgData)->hciType = APP_HCI_TYPE_EVENT;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.hciEvent.p_data = p_data;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.hciEvent.evtLength = length;
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
        return true;
    }
    else
    {
        return false;
    }
}

bool APP_HciAclCb(uint16_t connHandle, uint16_t length, uint8_t *p_packet)
{
    APP_Msg_T   appMsg;
    uint8_t     *p_data;

    appMsg.msgId = APP_MSG_BLE_STACK_EVT;
    p_data = OSAL_Malloc(length);

    if (p_data)
    {
        memcpy(p_data, p_packet, length);
        ((STACK_HCI_Cb_T *)appMsg.msgData)->hciType = APP_HCI_TYPE_ACL_DATA;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.aclData.connHandle = connHandle;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.aclData.dataLength = length;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.aclData.p_data = p_data;
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
        return true;
    }
    else
    {
        return false;
    }
}

void App_HciVendorCommandCb(uint8_t cmdId, uint8_t length, uint8_t *p_packet)
{
    APP_Msg_T   appMsg;
    uint8_t     *p_params = NULL;

    if ((length != 0) && ((p_params = OSAL_Malloc(length)) == NULL))
    {
        return;
    }
    else
    {
        appMsg.msgId = APP_MSG_BLE_STACK_EVT;
        if (length)
        {
            memcpy(p_params, p_packet, length);
        }
        ((STACK_HCI_Cb_T *)appMsg.msgData)->hciType = APP_HCI_TYPE_VENDOR_CMD;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.vendorCmd.cmdId = cmdId;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.vendorCmd.cmdLength = length;
        ((STACK_HCI_Cb_T *)appMsg.msgData)->cbParams.vendorCmd.p_cmdParams = p_params;
        OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
    }
}

void APP_BleStackInit()
{
    uint8_t preferAclTxBufNum = 12;

    HCI_EventRegister(APP_HciEvtCb);
    HCI_AclRxRegister(APP_HciAclCb);
    HCI_AppCmdRegister(App_HciVendorCommandCb);

    HCI_Init(preferAclTxBufNum);
}

void APP_BleStackEvtHandler(STACK_HCI_Cb_T *p_stackHciCb)
{
    switch (p_stackHciCb->hciType)
    {
        case APP_HCI_TYPE_EVENT:
        {
            /* TODO: implement your application code.*/

            if (p_stackHciCb->cbParams.hciEvent.p_data)
            {
                OSAL_Free(p_stackHciCb->cbParams.hciEvent.p_data);
            }
        }
        break;

        case APP_HCI_TYPE_ACL_DATA:
        {
            /* TODO: implement your application code.*/

            if (p_stackHciCb->cbParams.aclData.p_data)
            {
                OSAL_Free(p_stackHciCb->cbParams.aclData.p_data);
            }
        }
        break;

        case APP_HCI_TYPE_VENDOR_CMD:
        {
            /* TODO: implement your application code.*/

            if (p_stackHciCb->cbParams.vendorCmd.p_cmdParams)
            {
                OSAL_Free(p_stackHciCb->cbParams.vendorCmd.p_cmdParams);
            }
        }
        break;

        default:
        break;
    }
}

