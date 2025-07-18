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
  Application BLE Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ble_handler.c

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
<#if APP_BLE_DEVICE == "pic32cx_bz6_family">
#include "configuration.h"
</#if>
#include "osal/osal_freertos_extend.h"
#include "app_ble_handler.h"
<#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false && GAP_DSADV_EN == true>
#include "app_ble_dsadv.h"
</#if>

<#if APP_TRSP_CLIENT == true>
#include "app_trspc_handler.h"
</#if>

<#if APP_PXP_CLIENT == true>
#include "app_pxpm_handler.h"
</#if>

<#if APP_OTAP_CLIENT == true>
#include "app_otapc_handler.h"
</#if>

<#if APP_ANP_CLIENT == true>
#include "app_anpc_handler.h"
</#if>

<#if APP_ANCS_CLIENT == true>
#include "app_ancs_handler.h"
</#if>
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
void APP_BleGapEvtHandler(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            /* TODO: implement your application code.*/

            <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false && GAP_DSADV_EN == true>
            //Restart DSADV when the BLE link is terminated
            APP_BleDsadvRestart();
            </#if>
        }
        break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ENCRYPT_STATUS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ENC_INFO_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_EXT_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_TX_BUF_AVAILABLE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DEVICE_NAME_CHANGED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PHY_UPDATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_SCAN_REQ_RECEIVED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_DIRECT_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_SYNC_EST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_REPORT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_PERI_ADV_SYNC_LOST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_SET_TERMINATED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_SCAN_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_TRANSMIT_POWER_REPORTING:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_ADV_COMPL:
        {
            /* TODO: implement your application code.*/

            <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false && GAP_DSADV_EN == true>
            //Handle DSADV complete event
            APP_BleDsadvCompl();
            </#if>
        }
        break;

        case BLE_GAP_EVT_PATH_LOSS_THRESHOLD:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        <#if APP_BLE_DEVICE == "pic32cx_bz6_family">
        case BLE_GAP_EVT_SUBRATE_CHANGE:
        {
            /* TODO: implement your application code.*/
        }
        break;
        </#if>

        default:
        break;
    }
}

<#if GAP_CENTRAL == true || GAP_PERIPHERAL == true>
void APP_BleL2capEvtHandler(BLE_L2CAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_L2CAP_EVT_CONN_PARA_UPD_REQ:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CONN_PARA_UPD_RSP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_CONN_FAIL_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_SDU_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_ADD_CREDITS_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_L2CAP_EVT_CB_DISC_IND:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

void APP_GattEvtHandler(GATT_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case GATTC_EVT_ERROR_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_CHAR_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_DESC_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_READ_USING_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_READ_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_WRITE_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_HV_NOTIFY:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_HV_INDICATE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_READ:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_WRITE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_HV_CONFIRM:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case ATT_EVT_TIMEOUT:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case ATT_EVT_UPDATE_MTU:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTC_EVT_DISC_CHAR_BY_UUID_RESP:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_SERVICE_CHANGE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_CLIENT_FEATURE_CHANGE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_CLIENT_CCCDLIST_CHANGE:
        {
            /* TODO: implement your application code.*/
            OSAL_Free(p_event->eventField.onClientCccdListChange.p_cccdList);
        }
        break;

        case GATTC_EVT_PROTOCOL_AVAILABLE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case GATTS_EVT_PROTOCOL_AVAILABLE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        <#if APP_BLE_DEVICE == "pic32cx_bz6_family">
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true>
        case GATTS_EVT_ENC_DATA_KEY_MATL_REQ:
        {
            /* TODO: implement your application code.*/
            
            uint8_t initVector[] = CONFIG_BLE_GAP_SVC_EDKM_IV_C;
            uint8_t key[] = CONFIG_BLE_GAP_SVC_EDKM_KEY_C;

            GATTS_EncDataKeyMatlRsp(p_event->eventField.onEncDataKeyMatlReq.connHandle, key, initVector);
        }
        break;
            </#if>
        </#if>

        default:
        break;
    }
}

void APP_BleSmpEvtHandler(BLE_SMP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_SMP_EVT_PAIRING_COMPLETE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_SECURITY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_PASSKEY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_NOTIFY_KEYS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_PAIRING_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_OOB_DATA_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_INPUT_SC_OOB_DATA_REQUEST:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_KEYPRESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_SMP_EVT_GEN_SC_OOB_DATA_DONE:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

void APP_DmEvtHandler(BLE_DM_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_DM_EVT_DISCONNECTED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONNECTED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_START:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_SUCCESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_SECURITY_FAIL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_PAIRED_DEVICE_FULL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_PAIRED_DEVICE_UPDATED:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONN_UPDATE_SUCCESS:
        {
            /* TODO: implement your application code.*/
        }
        break;

        case BLE_DM_EVT_CONN_UPDATE_FAIL:
        {
            /* TODO: implement your application code.*/
        }
        break;

        default:
        break;
    }
}

    <#if BLE_BOOL_GATT_CLIENT == true>
void APP_DdEvtHandler(BLE_DD_Event_T *p_event)
{
        <#if BOOL_GCM_SCM == true>
    BLE_SCM_BleDdEventHandler(p_event);
        </#if>

        <#if APP_TRSP_CLIENT == true>
    BLE_TRSPC_BleDdEventHandler(p_event);
        </#if>

        <#if APP_OTAP_CLIENT == true>
    BLE_OTAPC_BleDdEventHandler(p_event);
        </#if>

        <#if APP_PXP_CLIENT == true>
    BLE_PXPM_BleDdEventHandler(p_event);
        </#if>

        <#if APP_ANP_CLIENT == true>
    BLE_ANPC_BleDdEventHandler(p_event);
        </#if>

        <#if APP_ANCS_CLIENT == true>
    BLE_ANCS_BleDdEventHandler(p_event);
        </#if>
}

        <#if BOOL_GCM_SCM == true>
void APP_ScmEvtHandler(BLE_SCM_Event_T *p_event)
{
    if (p_event->eventId == BLE_SCM_EVT_SVC_CHANGE)
    {
        BLE_DD_RestartServicesDiscovery(p_event->eventField.evtServiceChange.connHandle);
    }

    /* TODO: implement your application state machine.*/
}
        </#if>
    </#if>
</#if>
