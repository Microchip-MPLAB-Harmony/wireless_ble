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
    app_ble.c

  Summary:
    This file contains the Application BLE implementation for this project.

  Description:
    This file contains the Application BLE implementation for this project.
 *******************************************************************************/


#include "app.h"
#include "osal/osal_freertos_extend.h"
#include "app_ble.h"
#include "app_ble_handler.h"
<#if BLE_SYS_LOG_EN == true>
#include "app_ble_log_handler.h"
</#if>

<#if APP_TRCBP_SERVER == true>
#include "app_trcbps_handler.h"
</#if>

<#if APP_TRSP_SERVER == true>
#include "app_trsps_handler.h"
</#if>

<#if APP_TRSP_CLIENT == true>
#include "app_trspc_handler.h"
</#if>

<#if APP_PXP_SERVER == true>
#include "app_pxpr_handler.h"
</#if>

<#if APP_PXP_CLIENT == true>
#include "app_pxpm_handler.h"
</#if>

<#if APP_OTAP_SERVER == true>
#include "app_otaps_handler.h"
</#if>

<#if APP_OTAP_CLIENT == true>
#include "app_otapc_handler.h"
</#if>

<#if APP_ANP_SERVER == true>
#include "app_anps_handler.h"
</#if>

<#if APP_ANP_CLIENT == true>
#include "app_anpc_handler.h"
</#if>

<#if APP_HOGP_SERVER == true>
#include "app_hogps_handler.h"
</#if>

<#if APP_ANCS_CLIENT == true>
#include "app_ancs_handler.h"
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

<#if GAP_DEV_NAME_CHECK == true>
#define GAP_DEV_NAME_VALUE          "${GAP_DEV_NAME}"
<#else>
#define GAP_DEV_NAME_VALUE          !!! Length of GAP Device Name is invalid !!!
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************
<#if BLE_BOOL_GATT_CLIENT == true>
BLE_DD_Config_T         ddConfig;
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

void APP_BleStackCb(STACK_Event_T *p_stack)
{
    STACK_Event_T stackEvent;
    APP_Msg_T   appMsg;
    APP_Msg_T   *p_appMsg;

    memcpy((uint8_t *)&stackEvent, (uint8_t *)p_stack, sizeof(STACK_Event_T));
    stackEvent.p_event=OSAL_Malloc(p_stack->evtLen);
    if(stackEvent.p_event==NULL)
    {
        return;
    }
    memcpy(stackEvent.p_event, p_stack->p_event, p_stack->evtLen);
    stackEvent.p_event=stackEvent.p_event;

    if (p_stack->groupId==STACK_GRP_GATT)
    {
        GATT_Event_T *p_evtGatt = (GATT_Event_T *)stackEvent.p_event;

        if (p_evtGatt->eventId == GATTS_EVT_CLIENT_CCCDLIST_CHANGE)
        {
            uint8_t *p_payload;

            p_payload = (uint8_t *)OSAL_Malloc((p_evtGatt->eventField.onClientCccdListChange.numOfCccd*4));
            if (p_payload != NULL)
            {
                memcpy(p_payload, (uint8_t *)p_evtGatt->eventField.onClientCccdListChange.p_cccdList, (p_evtGatt->eventField.onClientCccdListChange.numOfCccd*4));
                p_evtGatt->eventField.onClientCccdListChange.p_cccdList = (GATTS_CccdList_T *)p_payload;
            }
        }
    }

    appMsg.msgId=APP_MSG_BLE_STACK_EVT;

    ((STACK_Event_T *)appMsg.msgData)->groupId=p_stack->groupId;
    ((STACK_Event_T *)appMsg.msgData)->evtLen=p_stack->evtLen;
    ((STACK_Event_T *)appMsg.msgData)->p_event=stackEvent.p_event;

    p_appMsg = &appMsg;
    OSAL_QUEUE_Send(&appData.appQueue, p_appMsg, 0);
}

void APP_BleStackEvtHandler(STACK_Event_T *p_stackEvt)
{
    switch(p_stackEvt->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            APP_BleGapEvtHandler((BLE_GAP_Event_T *)p_stackEvt->p_event);
        }
        break;
        
        case STACK_GRP_BLE_L2CAP:
        {
            APP_BleL2capEvtHandler((BLE_L2CAP_Event_T *)p_stackEvt->p_event);
         }
        break;
        case STACK_GRP_BLE_SMP:
        {
            APP_BleSmpEvtHandler((BLE_SMP_Event_T *)p_stackEvt->p_event);
         }
        break;

        case STACK_GRP_GATT:
        {
            APP_GattEvtHandler((GATT_Event_T *)p_stackEvt->p_event);
        }
        break;
        
        default:
        break;

    }

    //Direct event to BLE middleware
    BLE_DM_BleEventHandler(p_stackEvt);

    <#if BLE_BOOL_GATT_CLIENT == true>
    BLE_DD_BleEventHandler(&ddConfig, p_stackEvt);
        <#if BOOL_GCM_SCM == true>
    BLE_SCM_BleEventHandler(p_stackEvt);
        </#if>
    </#if>

    //Direct event to BLE profiles
    <#if APP_TRSP_SERVER == true>
    /* Transparent Profile */
    BLE_TRSPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_TRSP_CLIENT == true>
    /* Transparent Profile */
    BLE_TRSPC_BleEventHandler(p_stackEvt);
    </#if>
    
    <#if APP_TRCBP_SERVER == true>
    /* Transparent Credit Based Profile */
    BLE_TRCBPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_PXP_SERVER == true>
    /* Proximity Profile */
    BLE_PXPR_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_PXP_CLIENT == true>
    /* Proximity Profile */
    BLE_PXPM_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_OTAP_SERVER == true>
    /* Over-The-Air Profile */
    BLE_OTAPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_OTAP_CLIENT == true>
    BLE_OTAPC_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_ANP_SERVER == true>
    /* Alert Notification Profile */
    BLE_ANPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_ANP_CLIENT == true>
    BLE_ANPC_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_HOGP_SERVER == true>
    BLE_HOGPS_BleEventHandler(p_stackEvt);
    </#if>

    <#if APP_ANCS_CLIENT == true>
    BLE_ANCS_BleEventHandler(p_stackEvt);
    </#if>

    OSAL_Free(p_stackEvt->p_event);
}

<#if BLE_SYS_LOG_EN == true>
void APP_BleStackLogCb(BT_SYS_LogEvent_T *p_event)
{
    BT_SYS_LogEvent_T  	logEvent;
    APP_Msg_T       	appMsg;
    
    logEvent.p_logPayload = NULL;
    logEvent.p_returnParams = NULL;

    if (p_event->payloadLength)
    {
        logEvent.p_logPayload = OSAL_Malloc(p_event->payloadLength);
        if (logEvent.p_logPayload == NULL)
        {
            return;
        }
        memcpy(logEvent.p_logPayload, p_event->p_logPayload, p_event->payloadLength);
    }

    /* Allocate memory for returned parameters if exists. */
    if (p_event->paramsLength)
    {
        logEvent.p_returnParams = OSAL_Malloc(p_event->paramsLength);
        if (logEvent.p_returnParams == NULL)
        {
            if (p_event->payloadLength)
            {
                OSAL_Free(logEvent.p_logPayload);
            }
            return;
        }
        memcpy(logEvent.p_returnParams, p_event->p_returnParams, p_event->paramsLength);
    }

    logEvent.logType = p_event->logType;
    logEvent.logId = p_event->logId;
    logEvent.payloadLength = p_event->payloadLength;
    logEvent.paramsLength = p_event->paramsLength;

    appMsg.msgId = APP_MSG_BLE_STACK_LOG;

    memcpy((uint8_t *)appMsg.msgData, (uint8_t *)&logEvent, sizeof(BT_SYS_LogEvent_T));

    OSAL_QUEUE_Send(&appData.appQueue, &appMsg, 0);
}

</#if>

void APP_BleStackLogHandler(BT_SYS_LogEvent_T *p_logEvt)
{
<#if BLE_SYS_LOG_EN == true>
    BLE_LOG_StackLogHandler(p_logEvt);
</#if>
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


void APP_BleConfigBasic()
{
    <#if GAP_PERIPHERAL == true>
    int8_t                          connTxPower;
    </#if>
    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false>
    int8_t                          advTxPower;
    BLE_GAP_AdvParams_T             advParam;
        <#if GAP_ADV_DATA_LEN != 0>
    uint8_t advData[]={${GAP_ADV_DATA}};
    BLE_GAP_AdvDataParams_T         appAdvData;
        </#if>
        <#if GAP_SCAN_RSP_DATA_LEN != 0>
    uint8_t scanRspData[]={${GAP_SCAN_RSP_DATA}};
    BLE_GAP_AdvDataParams_T         appScanRspData;
        </#if>    
    </#if>

    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false>
    // Configure advertising parameters
    BLE_GAP_SetAdvTxPowerLevel(${GAP_ADV_TX_PWR},&advTxPower);      /* Advertising TX Power */
    
    memset(&advParam, 0, sizeof(BLE_GAP_AdvParams_T));
    advParam.intervalMin = ${GAP_ADV_INTERVAL_MIN};     /* Advertising Interval Min */
    advParam.intervalMax = ${GAP_ADV_INTERVAL_MAX};     /* Advertising Interval Max */
    advParam.type = ${GAP_ADV_TYPE};        /* Advertising Type */
    advParam.advChannelMap = ${GAP_CHANNEL_MAP};        /* Advertising Channel Map */
    advParam.filterPolicy = ${GAP_ADV_FILT_POLICY};     /* Advertising Filter Policy */
    BLE_GAP_SetAdvParams(&advParam);
    
        <#if GAP_ADV_DATA_LEN != 0>
    // Configure advertising data
    appAdvData.advLen=sizeof(advData);
    memcpy(appAdvData.advData, advData, appAdvData.advLen);     /* Advertising Data */
    BLE_GAP_SetAdvData(&appAdvData);
        </#if>

        <#if GAP_SCAN_RSP_DATA_LEN != 0>
    //Configure advertising scan response data
    appScanRspData.advLen=sizeof(scanRspData);
    memcpy(appScanRspData.advData, scanRspData, appScanRspData.advLen);     /* Scan Response Data */
    BLE_GAP_SetScanRspData(&appScanRspData);
        </#if>
    </#if>

    <#if GAP_PERIPHERAL == true>
    BLE_GAP_SetConnTxPowerLevel(${GAP_CONN_TX_PWR}, &connTxPower);      /* Connection TX Power */
    </#if>
}
void APP_BleConfigAdvance()
{
    uint8_t devName[]={GAP_DEV_NAME_VALUE};
    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == true>
    int8_t selectedTxPower;
    BLE_GAP_ExtAdvParams_T advParams;
        <#if GAP_EXT_ADV_DATA_LEN != 0>
    uint8_t advData[]={${GAP_EXT_ADV_DATA}};
    BLE_GAP_ExtAdvDataParams_T appAdvData;
        </#if>
        <#if GAP_EXT_SCAN_RSP_DATA_LEN != 0>
    uint8_t scanRspData[]={${GAP_EXT_SCAN_RSP_DATA}};
    BLE_GAP_ExtAdvDataParams_T appScanRspData;
        </#if>
        <#if GAP_EXT_ADV_ADV_SET_2 == true>
    int8_t selectedTxPower2;
            <#if GAP_EXT_ADV_DATA_LEN_2 != 0>
    uint8_t advData2[]={${GAP_EXT_ADV_DATA_2}};
            </#if>
            <#if GAP_EXT_SCAN_RSP_DATA_LEN_2 != 0>
    uint8_t scanRspData2[]={${GAP_EXT_SCAN_RSP_DATA_2}};
            </#if>
        </#if>
    </#if>    

    BLE_SMP_Config_T                smpParam;

    <#if BOOL_GAP_PRIVACY == true>
    BLE_GAP_LocalPrivacyParams_T    localPriParam;
    </#if>
   
    <#if GAP_SCAN == true>
        <#if BOOL_GAP_EXT_SCAN == false>
    BLE_GAP_ScanningParams_T        scanParam;
        <#else>
    BLE_GAP_ExtScanningPhy_T        scanParams;
        </#if>
    </#if>
    BLE_DM_Config_T                 dmConfig;
    BLE_GAP_ServiceOption_T         gapServiceOptions;
    <#if BLE_BOOL_GATT_SVC_CHANGE == true>
    GATTS_GattServiceOptions_T      gattServiceOptions;
    </#if>
    
    <#if GAP_CENTRAL == true && GAP_PERIPHERAL == false>
    int8_t                          connTxPower;
    </#if>

    // Configure Device Name
    BLE_GAP_SetDeviceName(sizeof(devName), devName);    /* Device Name */

    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == true>
    //Configure advertising Set 1
    advParams.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};        /* Advertising Handle */
        <#if GAP_EXT_ADV_EVT_PROPERTIES == '0'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;   /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '1'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;    /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '2'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;   /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '3'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV;  /* Advertising Event Properties */
            <#if GAP_EXT_ADV_EVT_PROP_ANON == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
            </#if>
            <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
            </#if>
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '4'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV;    /* Advertising Event Properties */
            <#if GAP_EXT_ADV_EVT_PROP_ANON == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
            </#if>
            <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
            </#if>
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '5'>
    advParams.evtProperies = 0; /* Advertising Event Properties */
            <#if GAP_EXT_ADV_EVT_PROP_ANON == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
            </#if>
            <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
            </#if>
        </#if>
    advParams.priIntervalMin = ${GAP_EXT_ADV_PRI_INTERVAL_MIN};     /* Primary Advertising Interval Min */
    advParams.priIntervalMax = ${GAP_EXT_ADV_PRI_INTERVAL_MAX};     /* Primary Advertising Interval Max */
    advParams.priChannelMap = ${GAP_PRI_CHANNEL_MAP};       /* Primary Advertising Channel Map */
    memset(&advParams.peerAddr, 0x00, sizeof(advParams.peerAddr));
    advParams.filterPolicy = ${GAP_EXT_ADV_FILT_POLICY};    /* Advertising Filter Policy */
    advParams.txPower = ${GAP_EXT_ADV_TX_POWER};    /* Advertising TX Power */
    advParams.priPhy = ${GAP_PRI_ADV_PHY};      /* Primary Advertising PHY */
    advParams.secMaxSkip = ${GAP_EXT_ADV_MAX_SKIP};     /* Secondary Advertising Max Skip */
    advParams.secPhy = ${GAP_SEC_ADV_PHY};      /* Secondary Advertising PHY */
    advParams.sid = ${GAP_EXT_ADV_SID};     /* Advertising SID */
    advParams.scanReqNotifiEnable = ${GAP_EXT_ADV_SCAN_ENABLE?c};   /* Scan Request Notification Enable */
    BLE_GAP_SetExtAdvParams(&advParams, &selectedTxPower);
    
        <#if GAP_EXT_ADV_DATA_LEN != 0>
    appAdvData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};
    appAdvData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appAdvData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appAdvData.advLen = sizeof(advData);
    appAdvData.p_advData=OSAL_Malloc(appAdvData.advLen);
    if(appAdvData.p_advData)
    {
        memcpy(appAdvData.p_advData, advData, appAdvData.advLen);       /* Advertising Data */
    }
    BLE_GAP_SetExtAdvData(&appAdvData);
    OSAL_Free(appAdvData.p_advData);
        </#if>
    
        <#if GAP_EXT_SCAN_RSP_DATA_LEN != 0>
    appScanRspData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE};
    appScanRspData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appScanRspData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appScanRspData.advLen = sizeof(scanRspData);
    appScanRspData.p_advData=OSAL_Malloc(appScanRspData.advLen);
    if(appScanRspData.p_advData)
    {
        memcpy(appScanRspData.p_advData, scanRspData, appScanRspData.advLen);       /* Scan Response Data */
    }
    BLE_GAP_SetExtScanRspData(&appScanRspData);
    OSAL_Free(appScanRspData.p_advData);
        </#if>

        <#if GAP_EXT_ADV_ADV_SET_2 == true>
    //Configure advertising Set 2
    advParams.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};      /* Advertising Handle */
            <#if GAP_EXT_ADV_EVT_PROPERTIES_2 == '0'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;   /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '1'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;    /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '2'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV;   /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '3'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV;  /* Advertising Event Properties */
                <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
                </#if>
                <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
                </#if>
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '4'>
    advParams.evtProperies = BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV;    /* Advertising Event Properties */
                <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
                </#if>
                <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
                </#if>
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '5'>
    advParams.evtProperies = 0; /* Advertising Event Properties */
                <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS;    /* Advertising Event Properties */
                </#if>
                <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>
    advParams.evtProperies |= BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER;    /* Advertising Event Properties */
                </#if>
            </#if>
    advParams.priIntervalMin = ${GAP_EXT_ADV_PRI_INTERVAL_MIN_2};   /* Primary Advertising Interval Min */
    advParams.priIntervalMax = ${GAP_EXT_ADV_PRI_INTERVAL_MAX_2};   /* Primary Advertising Interval Max */
    advParams.priChannelMap = ${GAP_PRI_CHANNEL_MAP_2};     /* Primary Advertising Channel Map */
    memset(&advParams.peerAddr, 0x00, sizeof(advParams.peerAddr));
    advParams.filterPolicy = ${GAP_EXT_ADV_FILT_POLICY_2};      /* Advertising Filter Policy */
    advParams.txPower = ${GAP_EXT_ADV_TX_POWER_2};      /* Advertising TX Power */
    advParams.priPhy = ${GAP_PRI_ADV_PHY_2};        /* Primary Advertising PHY */
    advParams.secMaxSkip = ${GAP_EXT_ADV_MAX_SKIP_2};       /* Secondary Advertising Max Skip */
    advParams.secPhy = ${GAP_SEC_ADV_PHY_2};        /* Secondary Advertising PHY */
    advParams.sid = ${GAP_EXT_ADV_SID_2};       /* Advertising SID */
    advParams.scanReqNotifiEnable = ${GAP_EXT_ADV_SCAN_ENABLE_2?c};     /* Scan Request Notification Enable */
    BLE_GAP_SetExtAdvParams(&advParams, &selectedTxPower2);
    
            <#if GAP_EXT_ADV_DATA_LEN_2 != 0>
    appAdvData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};
    appAdvData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appAdvData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appAdvData.advLen = sizeof(advData2);
    appAdvData.p_advData=OSAL_Malloc(appAdvData.advLen);
    if(appAdvData.p_advData)
    {
        memcpy(appAdvData.p_advData, advData2, appAdvData.advLen);      /* Advertising Data */
    }
    BLE_GAP_SetExtAdvData(&appAdvData);
    OSAL_Free(appAdvData.p_advData);
            </#if>
    
            <#if GAP_EXT_SCAN_RSP_DATA_LEN_2 != 0>
    appScanRspData.advHandle = ${GAP_EXT_ADV_ADV_SET_HANDLE_2};
    appScanRspData.operation = BLE_GAP_EXT_ADV_DATA_OP_COMPLETE;
    appScanRspData.fragPreference = BLE_GAP_EXT_ADV_DATA_FRAG_PREF_ALL;
    appScanRspData.advLen = sizeof(scanRspData2);
    appScanRspData.p_advData=OSAL_Malloc(appScanRspData.advLen);
    if(appScanRspData.p_advData)
    {
        memcpy(appScanRspData.p_advData, scanRspData2, appScanRspData.advLen);      /* Scan Response Data */
    }
    BLE_GAP_SetExtScanRspData(&appScanRspData);
    OSAL_Free(appScanRspData.p_advData);
            </#if>
        </#if>
    </#if>    
    
    // GAP Service option
    gapServiceOptions.charDeviceName.enableWriteProperty = ${GAP_SVC_DEV_NAME_WRITE?c};             /* Enable Device Name Write Property */
    gapServiceOptions.charAppearance.appearance = 0x${GAP_SVC_APPEARANCE};                          /* Appearance */
    gapServiceOptions.charPeriPreferConnParam.enable = ${GAP_SVC_PERI_PRE_CP?c};                    /* Enable Peripheral Preffered Connection Parameters */
    <#if GAP_SVC_PERI_PRE_CP == true>
    gapServiceOptions.charPeriPreferConnParam.minConnInterval = ${GAP_SVC_CP_MIN_CONN_INTERVAL};    /* Minimum Connection Interval */
    gapServiceOptions.charPeriPreferConnParam.maxConnInterval = ${GAP_SVC_CP_MAX_CONN_INTERVAL};    /* Maximum Connection Interval */
    gapServiceOptions.charPeriPreferConnParam.peripheralLatency = ${GAP_SVC_CP_PERI_LATENCY};       /* Peripheral Latency */
    gapServiceOptions.charPeriPreferConnParam.connSupervisionTimeoutMulti = ${GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT}; /* Connection Sup ervision Timeout Multiplier */
    </#if>
    
    BLE_GAP_ConfigureBuildInService(&gapServiceOptions);
    
    <#if BOOL_GAP_PRIVACY == true> 
    // Configure privacy
    localPriParam.privacyAddrType = ${GAP_PRIV_ADDR_TYPE};  /* Address Type */
    localPriParam.addrTimeout = ${GAP_PRIV_ADDR_TO};        /* Address Timeout */
    memset(localPriParam.localIrk, 0, 16);
    BLE_GAP_SetLocalPrivacy(true, &localPriParam);
    </#if>
    
    <#if GAP_SCAN == true>
    // Configure scan parameters
        <#if BOOL_GAP_EXT_SCAN == false>
    scanParam.type = ${GAP_SCAN_TYPE};      /* Scan Type */
    scanParam.interval = ${GAP_SCAN_INTERVAL};      /* Scan Interval */
    scanParam.window = ${GAP_SCAN_WINDOW};      /* Scan Window */
    scanParam.filterPolicy = ${GAP_SCAN_FILT_POLICY};       /* Scan Filter Policy */
    scanParam.disChannel = ${GAP_SCAN_DIS_CHANNEL_MAP};      /* Disable specific channel during scanning */
    BLE_GAP_SetScanningParam(&scanParam);
        <#else>
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
    scanParams.le1mPhy.enable = true;       /* Scan PHY */
    scanParams.le1mPhy.type = ${GAP_EXT_SCAN_TYPE_1M};      /* Scan Type */
    scanParams.le1mPhy.interval = ${GAP_EXT_SCAN_INTERVAL_1M};      /* Scan Interval */
    scanParams.le1mPhy.window = ${GAP_EXT_SCAN_WINDOW_1M};      /* Scan Window */
    scanParams.le1mPhy.disChannel = ${GAP_EXT_SCAN_DIS_CHANNEL_MAP_1M};      /* Disable specific channel during scanning */
            <#else>
    scanParams.le1mPhy.enable = false;      /* Scan PHY */
            </#if>
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_CODED') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
    scanParams.leCodedPhy.enable = true;        /* Scan PHY */
    scanParams.leCodedPhy.type = ${GAP_EXT_SCAN_TYPE_CODED};        /* Scan Type */
    scanParams.leCodedPhy.interval = ${GAP_EXT_SCAN_INTERVAL_CODED};        /* Scan Interval */
    scanParams.leCodedPhy.window = ${GAP_EXT_SCAN_WINDOW_CODED};        /* Scan Interval */
    scanParams.leCodedPhy.disChannel = ${GAP_EXT_SCAN_DIS_CHANNEL_MAP_CODED};      /* Disable specific channel during scanning */
            <#else>
    scanParams.leCodedPhy.enable = false;       //Scan PHY
            </#if>
    BLE_GAP_SetExtScanningParams(${GAP_EXT_SCAN_FILT_POLICY}, &scanParams);
        </#if>
    </#if>

    <#if GAP_CENTRAL == true && GAP_PERIPHERAL == false>
    BLE_GAP_SetConnTxPowerLevel(${GAP_CONN_TX_PWR}, &connTxPower);      /* Connection TX Power */
    </#if>

    // Configure SMP parameters
    memset(&smpParam, 0, sizeof(BLE_SMP_Config_T));
    smpParam.ioCapability = ${SMP_IOCAP_TYPE};                  /* IO Capability */
    <#if SMP_AUTH_REQ_BOND == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_BONDING;             /* Authentication Setting: Bonding */
    </#if>
    <#if SMP_AUTH_REQ_MITM == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_MITM;                /* Authentication Setting: MITM */
    </#if>
    <#if SMP_AUTH_REQ_SC == true>
    smpParam.authReqFlag |= BLE_SMP_OPTION_SECURE_CONNECTION;   /* Authentication Setting: Secure Connections */
    </#if>
    <#if SMP_OOB == true>
    smpParam.oobDataFlag = true;                                /* OOB Authentication Data Present */
    </#if>
    <#if SMP_SC_ONLY == true>
    smpParam.scOnly = true;                      				/* Secure Connections Only */
    </#if>
    <#if SMP_AUTH_PAIR == true>
    smpParam.authPairingRequired = true;                        /* Authenticated pairing method is required */
    </#if>
    BLE_SMP_Config(&smpParam);

    // Configure BLE_DM middleware parameters
    <#if BOOL_DM_SEC_AUTO_ACCEPT == true>
    dmConfig.secAutoAccept = true;                          /* Auto Accept Security Request */
    <#else>
    dmConfig.secAutoAccept = false;                         /* Auto Accept Security Request */
    </#if>
    <#if BOOL_DM_CONN_AUTO_ACCEPT == false>
    dmConfig.connConfig.autoReplyUpdateRequest = false;     /* Auto Accept Connection Parameter Update Request */
    <#else>
    dmConfig.connConfig.autoReplyUpdateRequest = true;      /* Auto Accept Connection Parameter Update Request */
    dmConfig.connConfig.minAcceptConnInterval = ${DM_MIN_CONN_INTERVAL};    /* Minimum Connection Interval */
    dmConfig.connConfig.maxAcceptConnInterval = ${DM_MAX_CONN_INTERVAL};    /* Maximum Connection Interval */
    dmConfig.connConfig.minAcceptPeripheralLatency = ${DM_MIN_CONN_LATENCY};    /* Minimum Connection Latency */
    dmConfig.connConfig.maxAcceptPeripheralLatency = ${DM_MAX_CONN_LATENCY};    /* Maximum Connection Latency */
    </#if>
    BLE_DM_Config(&dmConfig);

    <#if BLE_BOOL_GATT_SVC_CHANGE == true>
    // GATT Service option
    gattServiceOptions.enable = true;       /* Enable GATT Service Change */
    GATTS_ConfigureBuildInService(&gattServiceOptions);
    </#if>

    <#if BLE_BOOL_GATT_CLIENT == true>
    // Configure BLE_DD middleware parameters
    ddConfig.waitForSecurity = false;
    ddConfig.initDiscInCentral = true;
    ddConfig.initDiscInPeripheral = false;
    </#if>
}

void APP_BleStackInitBasic()
{
    BLE_GAP_Init();

    <#if GAP_ADVERTISING == true>
    BLE_GAP_AdvInit();  /* Advertising */
    </#if>

    <#if GAP_PERIPHERAL == true>
    BLE_GAP_ConnPeripheralInit();   /* Peripheral */
    </#if>
}

void APP_BleStackInitAdvance()
{
    <#if BLE_BOOL_GATT_CACHING == true>
    uint16_t gattsInitParam=GATTS_CONFIG_CACHING_SUPPORT;       /* Enable GATT Caching */
    <#else>
    uint16_t gattsInitParam=GATTS_CONFIG_NONE;
    </#if>
    
    <#if BLE_BOOL_GATT_CLIENT == true>
    <#if BLE_BOOL_GATT_MANU_CONFIRM == true>
    uint16_t gattcInitParam=GATTC_CONFIG_MANUAL_CONFIRMATION;       /* Enable Manual Handle Confirmation */
    <#else>
    uint16_t gattcInitParam=GATTC_CONFIG_NONE;
    </#if>
    </#if>

    STACK_EventRegister(APP_BleStackCb);
    

    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == true>
    BLE_GAP_ExtAdvInit();   /* Enable Extended Advertising */
    </#if>

    <#if GAP_SCAN == true>
    BLE_GAP_ScanInit();     /* Scan */
    <#if BOOL_GAP_EXT_SCAN == true>
    BLE_GAP_ExtScanInit(${GAP_EXT_SCAN_DATA_LEN}, ${GAP_EXT_SCAN_NUM_2_PKT});       /* Enable Extended Scan, Maximum Data Len, Maximum Number of Secondary Packets */
    </#if>
    </#if>
    
    <#if GAP_CENTRAL == true>
    BLE_GAP_ConnCentralInit();  /* Central */
    </#if>

    BLE_L2CAP_Init();
    <#if BOOL_L2CAP_CREDIT_FLOWCTRL == true>
    BLE_L2CAP_CbInit();     /* Credit Base Flow Control */
    </#if>
    
    GATTS_Init(gattsInitParam);
    
    <#if BLE_BOOL_GATT_CLIENT == true>
    GATTC_Init(gattcInitParam);     /* Enable Client Role */
    </#if>

    BLE_SMP_Init();
    
    <#if GAP_DIRECT_TEST_MODE == true>
    BLE_DTM_Init();     /* Enable Direct Test Mode */
    </#if>

    //Initialize BLE middleware
    BLE_DM_Init();
    BLE_DM_EventRegister(APP_DmEvtHandler);

    <#if BLE_BOOL_GATT_CLIENT == true>
    BLE_DD_Init();
    BLE_DD_EventRegister(APP_DdEvtHandler);
    
        <#if BOOL_GCM_SCM == true>
    BLE_SCM_Init();
    BLE_SCM_EventRegister(APP_ScmEvtHandler);
        </#if>
    </#if>

    <#if BLE_SYS_LOG_EN == true>
    BLE_LOG_EventRegister(APP_BleLogOutput);
    BT_SYS_LogEnable(APP_BleStackLogCb);        /* Enable BLE Log */
    </#if>

    //Initialize BLE services
    <#if APP_HOGP_SERVER == true>
    BLE_BAS_Add();
    BLE_BAS_SetBatteryLevel(0x64);  /* Set the corresponding battery value. */
    </#if>

    //Initialize BLE profiles
    <#if APP_TRSP_SERVER == true>
    /* Transparent Profile */
    BLE_TRSPS_Init();                                   /* Enable Server Role */
    BLE_TRSPS_EventRegister(APP_TrspsEvtHandler);   /* Enable Server Role */
    </#if>

    <#if APP_TRSP_CLIENT == true>
    /* Transparent Profile */
    BLE_TRSPC_Init();                                   /* Enable Client Role */
    BLE_TRSPC_EventRegister(APP_TrspcEvtHandler);   /* Enable Client Role */
    </#if>

    <#if APP_TRCBP_SERVER == true>
    /* Transparent Credit Based Profile */
    BLE_TRCBPS_Init();                                  /* Enable Server Role */
    BLE_TRCBPS_EventRegister(APP_TrcbpsEvtHandler); /* Enable Server Role */
    </#if>

    <#if APP_PXP_SERVER == true>
    /* Proximity Profile */
    BLE_PXPR_Init();                                    /* Enable Reporter Role */
    BLE_PXPR_EventRegister(APP_PxprEvtHandler);     /* Enable Reporter Role */
    </#if>

    <#if APP_PXP_CLIENT == true>
    /* Proximity Profile */
    BLE_PXPM_Init();                                    /* Enable Monitor Role */
    BLE_PXPM_EventRegister(APP_PxpmEvtHandler);     /* Enable Monitor Role */
    </#if>

    <#if APP_OTAP_SERVER == true>
    /* Over-The-Air Profile */
    BLE_OTAPS_Init();                                   /* Enable Server Role */
    BLE_OTAPS_EventRegister(APP_OtapsEvtHandler);   /* Enable Server Role */
    </#if>

    <#if APP_OTAP_CLIENT == true>
    BLE_OTAPC_Init();                                   /* Enable Client Role */
    BLE_OTAPC_EventRegister(APP_OtapcEvtHandler);   /* Enable Client Role */
    </#if>

    <#if APP_ANP_SERVER == true>
    /* Alert Notification Profile */
    BLE_ANPS_Init();                                    /* Enable Server Role */
    BLE_ANPS_EventRegister(APP_AnpsEvtHandler);     /* Enable Server Role */
    </#if>

    <#if APP_ANP_CLIENT == true>
    BLE_ANPC_Init();                                    /* Enable Client Role */
    BLE_ANPC_EventRegister(APP_AnpcEvtHandler);     /* Enable Client Role */
    </#if>

    <#if APP_HOGP_SERVER == true>
    BLE_HOGPS_Init();
    BLE_HOGPS_EventRegister(APP_HogpsEvtHandler);
    </#if>

    <#if APP_ANCS_CLIENT == true>
    BLE_ANCS_Init();
    BLE_ANCS_EventRegister(APP_AncsEvtHandler);
    </#if>

    APP_BleConfigAdvance();
}

void APP_BleStackInit()
{
    APP_BleStackInitBasic();
    APP_BleConfigBasic();
    APP_BleStackInitAdvance();
}