<#macro cmsChkSvcName>
<#local id = "CMS_BOOL_SVC_NAME_" + "${CMS_SVC_NO}">
<#if id?eval == false>
!!! Service Name of Service ${CMS_SVC_NO} is invalid !!!
</#if>
</#macro>
<@cmsChkSvcName/>
<#assign CMS_SVC_NAME_ID = "CMS_STR_SVC_NAME_" + "${CMS_SVC_NO}">
<#assign CMS_SVC_NAME_VALUE = CMS_SVC_NAME_ID?eval>
<#assign CMS_SVC_UUID_ID = "CMS_STR_SVC_UUID_C_" + "${CMS_SVC_NO}">
<#assign CMS_SVC_UUID_VALUE = CMS_SVC_UUID_ID?eval>
<#assign CMS_SVC_CHAR_COUNT_ID = "CMS_INT_SVC_CHAR_COUNT_" + "${CMS_SVC_NO}">
<#assign CMS_CHAR_PERM_READ = []>
<#assign CMS_CHAR_PERM_WRITE = []>
<#assign CMS_CHAR_PERM_CCC = []>
<#assign CMS_CHAR_CCC_COUNT = 0>
<#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
    <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_READ">
    <#assign CMS_CHAR_PERM_READ += [CMS_CHAR_PROP_ID?eval]>
    <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_WRITE_CMD">
    <#if CMS_CHAR_PROP_ID?eval == true>
        <#assign CMS_CHAR_PERM_WRITE += [true]>
    <#else>
        <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_WRITE_REQ">
        <#assign CMS_CHAR_PERM_WRITE += [CMS_CHAR_PROP_ID?eval]>
    </#if>
    <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_NOTIFY">
    <#if CMS_CHAR_PROP_ID?eval == true>
        <#assign CMS_CHAR_PERM_CCC += [true]>
    <#else>
        <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_INDICATE">
        <#assign CMS_CHAR_PERM_CCC += [CMS_CHAR_PROP_ID?eval]>
    </#if>
    <#if CMS_CHAR_PERM_CCC[i] == true>
        <#assign CMS_CHAR_CCC_COUNT++>
    </#if>
</#list>
/*******************************************************************************
  BLE ${CMS_SVC_NAME_VALUE?capitalize} Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_${CMS_SVC_NAME_VALUE?lower_case}_svc.c

  Summary:
    This file contains the BLE ${CMS_SVC_NAME_VALUE?capitalize} Service functions for application user.

  Description:
    This file contains the BLE ${CMS_SVC_NAME_VALUE?capitalize} Service functions for application user.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stddef.h>
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_cms/ble_${CMS_SVC_NAME_VALUE?lower_case}_svc.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/* Little Endian. */
<#if CMS_SVC_UUID_VALUE?length gt 0>
#define UUID_${CMS_SVC_NAME_VALUE?upper_case}_PRIMARY_SVC_LE         ${CMS_SVC_UUID_VALUE}
<#else>
#define UUID_${CMS_SVC_NAME_VALUE?upper_case}_PRIMARY_SVC_LE         !!! Service UUID of Service ${CMS_SVC_NO} is invalid !!!
</#if>

<#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
<#assign CMS_CHAR_UUID_ID = "CMS_STR_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_UUID_C">
<#assign CMS_CHAR_UUID_VALUE = CMS_CHAR_UUID_ID?eval>
<#if CMS_SVC_UUID_VALUE?length gt 0>
#define UUID_${CMS_SVC_NAME_VALUE?upper_case}_CHARACTERISTIC_${i}_LE         ${CMS_CHAR_UUID_VALUE}
<#else>
#define UUID_${CMS_SVC_NAME_VALUE?upper_case}_CHARACTERISTIC_${i}_LE         !!! Characteristic ${i} UUID of Service ${CMS_SVC_NO} is invalid !!!
</#if>
</#list>

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

/* Primary Service Declaration */
static const uint8_t s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuid[] = {UUID_${CMS_SVC_NAME_VALUE?upper_case}_PRIMARY_SVC_LE};
static const uint16_t s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuidLen = sizeof(s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuid);

<#macro cmsChkProp charNo>
<#local empty = true>
<#local cms_props  = ["_PROP_READ", "_PROP_WRITE_CMD", "_PROP_WRITE_REQ", "_PROP_NOTIFY", "_PROP_INDICATE"] >
<#list cms_props as prop>
    <#local CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + prop>
    <#if CMS_CHAR_PROP_ID?eval == true>
        <#local empty = false>
        <#break>
    </#if>
</#list>
<#if empty>
#error Characteristic ${charNo} Property of Service ${CMS_SVC_NO} not configured
/* TODO Fix this configuration error
 * The user must select at least one property in the according characteristic
 * declaration in the Harmony configurator.
 * Or user could manually add the property into the first field of the 
 * s_${CMS_SVC_NAME_VALUE?lower_case}Char${charNo} array. Please search the keyword
 * "GATT_CHARACTERISTIC_PROPERTIES" in the gatt.h for the valid setting.
 */
</#if>
</#macro>
<#t>
<#macro cmsGenProp charNo>
<#local propSeg = []>
<#local cms_props  = ["_PROP_READ", "_PROP_WRITE_CMD", "_PROP_WRITE_REQ", "_PROP_NOTIFY", "_PROP_INDICATE"] >
<#list cms_props as prop>
    <#local CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + prop>
    <#if CMS_CHAR_PROP_ID?eval == true>
        <#local propSeg += ["ATT" + prop]>
    </#if>
</#list>
${propSeg?join("|", "not configured")}<#rt>
</#macro>
<#t>
<#t>
<#macro cmsGenCharValLen charNo>
<#if CMS_CHAR_PERM_WRITE[charNo] == true>
    <#local cms_char_value_max_len_id = "CMS_INT_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_LEN">
    ${cms_char_value_max_len_id?eval}<#t>
</#if>
</#macro>
<#t>
<#macro cmsGenCharVal charNo>
<#if CMS_CHAR_PERM_READ[charNo] == true>
    <#local cms_char_value_id = "CMS_STR_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_VALUE_C">
    ${cms_char_value_id?eval}<#t>
<#elseif CMS_CHAR_PERM_WRITE[charNo] == false>
    0x00<#t>
</#if>
</#macro>
<#t>
<#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
/* ${CMS_SVC_NAME_VALUE?capitalize} Characteristic ${i} Characteristic */
<@cmsChkProp charNo = i/>
static const uint8_t s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}[] = {<@cmsGenProp charNo = i/>, UINT16_TO_BYTES(${CMS_SVC_NAME_VALUE?upper_case}_HDL_CHARVAL_${i}), UUID_${CMS_SVC_NAME_VALUE?upper_case}_CHARACTERISTIC_${i}_LE};
static const uint16_t s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Len = sizeof(s_${CMS_SVC_NAME_VALUE?lower_case}Char${i});

/* ${CMS_SVC_NAME_VALUE?capitalize} Characteristic ${i} Characteristic Value */
static const uint8_t s_${CMS_SVC_NAME_VALUE?lower_case}UuidChar${i}[] = {UUID_${CMS_SVC_NAME_VALUE?upper_case}_CHARACTERISTIC_${i}_LE};
static uint8_t s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Val[<@cmsGenCharValLen charNo = i/>] = {<@cmsGenCharVal charNo = i/>};
static uint16_t s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}ValLen = sizeof(s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Val);

<#if CMS_CHAR_PERM_CCC[i] == true>
/* ${CMS_SVC_NAME_VALUE?capitalize} Characteristic ${i} Client Characteristic Configuration Descriptor */
static uint8_t s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i}[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i}Len = sizeof(s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i});

</#if>
</#list>
<#t>
<#macro cmsGenCharSetting charNo>
<#local setting = []>
<#if CMS_CHAR_PERM_READ[charNo] == true>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_SETTING_READ">
    <#if cms_id?eval == true>
        <#local setting += ["SETTING_MANUAL_READ_RSP"]>
    </#if>
</#if>
<#if CMS_CHAR_PERM_WRITE[charNo] == true>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_SETTING_WRITE">
    <#if cms_id?eval == true>
        <#local setting += ["SETTING_MANUAL_WRITE_RSP"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_SETTING_VAR">
    <#if cms_id?eval == true>
        <#local setting += ["SETTING_VARIABLE_LEN"]>
    </#if>
</#if>
<#local cms_id = "CMS_STR_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_UUID_C">
<#if (cms_id?eval?length > 10)>
    <#local setting += ["SETTING_UUID_16"]>
</#if>
${setting?join("|", "0")}<#rt>
</#macro>
<#t>
<#macro cmsGenCharPerm charNo>
<#local perm = []>
<#if CMS_CHAR_PERM_READ[charNo] == true>
    <#local perm += ["PERMISSION_READ"]>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_READ_AUTH">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_READ_AUTHEN"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_READ_AUTH_SC">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_READ_AUTHEN_SC"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_READ_ENC">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_READ_ENC"]>
    </#if>
</#if>
<#if CMS_CHAR_PERM_WRITE[charNo] == true>
    <#local perm += ["PERMISSION_WRITE"]>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_WRITE_AUTH">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_WRITE_AUTHEN"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_WRITE_AUTH_SC">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_WRITE_AUTHEN_SC"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PERMS_WRITE_ENC">
    <#if cms_id?eval == true>
        <#local perm += ["PERMISSION_WRITE_ENC"]>
    </#if>
</#if>
${perm?join("|", "0")}<#rt>
</#macro>
<#t>
<#macro cmsGenCccdSetting charNo>
<#local setting = []>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_READ">
<#if cms_id?eval == true>
    <#local setting += ["SETTING_MANUAL_READ_RSP"]>
</#if>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_WRITE">
<#if cms_id?eval == true>
    <#local setting += ["SETTING_MANUAL_WRITE_RSP"]>
</#if>
${setting?join("|", "", "|")}<#rt>
</#macro>
<#t>
<#macro cmsGenCccdPerm charNo>
<#local perm = []>
<#local perm += ["PERMISSION_READ"]>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_READ_AUTH">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_READ_AUTHEN"]>
</#if>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_READ_AUTH_SC">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_READ_AUTHEN_SC"]>
</#if>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_READ_ENC">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_READ_ENC"]>
</#if>
<#local perm += ["PERMISSION_WRITE"]>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_WRITE_AUTH">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_WRITE_AUTHEN"]>
</#if>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_WRITE_AUTH_SC">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_WRITE_AUTHEN_SC"]>
</#if>
<#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_CCCD_WRITE_ENC">
<#if cms_id?eval == true>
    <#local perm += ["PERMISSION_WRITE_ENC"]>
</#if>
${perm?join("|")}<#rt>
</#macro>
<#t>
/* Attribute list for ${CMS_SVC_NAME_VALUE?capitalize} service */
static GATTS_Attribute_T s_${CMS_SVC_NAME_VALUE?lower_case}List[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuid,
        (uint16_t *) & s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuidLen,
        sizeof (s_${CMS_SVC_NAME_VALUE?lower_case}SvcUuid),
        0,
        PERMISSION_READ
    },
    <#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
    /* Characteristic ${i} Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_${CMS_SVC_NAME_VALUE?lower_case}Char${i},
        (uint16_t *) & s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Len,
        sizeof (s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}),
        0,
        PERMISSION_READ
    },
    /* Characteristic ${i} Value */
    {
        (uint8_t *) s_${CMS_SVC_NAME_VALUE?lower_case}UuidChar${i},
        (uint8_t *) s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Val,
        (uint16_t *) & s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}ValLen,
        sizeof(s_${CMS_SVC_NAME_VALUE?lower_case}Char${i}Val),
        <@cmsGenCharSetting charNo = i/>,
        <@cmsGenCharPerm charNo = i/><#nt>
    },
    <#if CMS_CHAR_PERM_CCC[i] == true>
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        (uint8_t *) s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i},
        (uint16_t *) & s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i}Len,
        sizeof (s_${CMS_SVC_NAME_VALUE?lower_case}CccChar${i}),
        <@cmsGenCccdSetting charNo = i/>SETTING_CCCD,
        <@cmsGenCccdPerm charNo = i/><#nt>
    },
    </#if>
    </#list>
};

<#if CMS_CHAR_PERM_CCC?seq_contains(true) == true>
<#macro cmsGenCccdAccept charNo>
<#local accept = []>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PROP_NOTIFY">
    <#if cms_id?eval == true>
        <#local accept += ["NOTIFICATION"]>
    </#if>
    <#local cms_id = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + charNo + "_PROP_INDICATE">
    <#if cms_id?eval == true>
        <#local accept += ["INDICATION"]>
    </#if>
${accept?join("|")}<#rt>
</#macro>
static const GATTS_CccdSetting_T s_${CMS_SVC_NAME_VALUE?lower_case}CccdSetting[] = 
{
    <#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
    <#if CMS_CHAR_PERM_CCC[i] == true>
    {${CMS_SVC_NAME_VALUE?upper_case}_HDL_CCCD_${i}, <@cmsGenCccdAccept charNo = i/>},
    </#if>
    </#list>
};
</#if>

/* ${CMS_SVC_NAME_VALUE?capitalize} Service structure */
static GATTS_Service_T s_${CMS_SVC_NAME_VALUE?lower_case}Svc = 
{
    NULL,
    (GATTS_Attribute_T *) s_${CMS_SVC_NAME_VALUE?lower_case}List,
<#if CMS_CHAR_PERM_CCC?seq_contains(true) == true>    
    (GATTS_CccdSetting_T const *)s_${CMS_SVC_NAME_VALUE?lower_case}CccdSetting,
<#else>
    NULL,
</#if>    
    ${CMS_SVC_NAME_VALUE?upper_case}_START_HDL,
    ${CMS_SVC_NAME_VALUE?upper_case}_END_HDL,
    ${CMS_CHAR_CCC_COUNT}
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint16_t BLE_${CMS_SVC_NAME_VALUE?upper_case}_Add()
{
    return GATTS_AddService(&s_${CMS_SVC_NAME_VALUE?lower_case}Svc, (${CMS_SVC_NAME_VALUE?upper_case}_END_HDL - ${CMS_SVC_NAME_VALUE?upper_case}_START_HDL + 1));
}
