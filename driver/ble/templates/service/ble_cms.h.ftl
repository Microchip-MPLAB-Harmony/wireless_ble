<#assign CMS_SVC_NAME_ID = "CMS_STR_SVC_NAME_" + "${CMS_SVC_NO}">
<#assign CMS_SVC_NAME_VALUE = CMS_SVC_NAME_ID?eval>
<#assign CMS_SVC_CHAR_COUNT_ID = "CMS_INT_SVC_CHAR_COUNT_" + "${CMS_SVC_NO}">
<#assign CMS_CHAR_PERM_CCC = []>
<#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
    <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_NOTIFY">
    <#if CMS_CHAR_PROP_ID?eval == true>
        <#assign CMS_CHAR_PERM_CCC += [true]>
    <#else>
        <#assign CMS_CHAR_PROP_ID = "CMS_BOOL_SVC_" + "${CMS_SVC_NO}" + "_CHAR_" + i + "_PROP_INDICATE">
        <#assign CMS_CHAR_PERM_CCC += [CMS_CHAR_PROP_ID?eval]>
    </#if>
</#list>
/*******************************************************************************
  BLE ${CMS_SVC_NAME_VALUE?capitalize} Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_${CMS_SVC_NAME_VALUE?lower_case}_svc.h

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

/**
 * @addtogroup BLE_${CMS_SVC_NAME_VALUE?upper_case} BLE ${CMS_SVC_NAME_VALUE?upper_case}
 * @{
 * @brief Header file for the BLE ${CMS_SVC_NAME_VALUE?capitalize} Service.
 * @note Definitions and prototypes for the BLE ${CMS_SVC_NAME_VALUE?capitalize} Service stack layer application programming interface.
 */
#ifndef BLE_${CMS_SVC_NAME_VALUE?upper_case}_H
#define BLE_${CMS_SVC_NAME_VALUE?upper_case}_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@defgroup BLE_${CMS_SVC_NAME_VALUE?upper_case}_ASSIGN_HANDLE BLE_${CMS_SVC_NAME_VALUE?upper_case}_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE ${CMS_SVC_NAME_VALUE?capitalize} Service.
 * @{ */
#define ${CMS_SVC_NAME_VALUE?upper_case}_START_HDL                               ${CMS_SVC_HANDLE}                                   /**< The start attribute handle of ${CMS_SVC_NAME_VALUE?lower_case} service. */
/** @} */

/**@brief Definition of BLE ${CMS_SVC_NAME_VALUE?capitalize} Service attribute handle */
typedef enum BLE_${CMS_SVC_NAME_VALUE?upper_case}_AttributeHandle_T
{
    ${CMS_SVC_NAME_VALUE?upper_case}_HDL_SVC = ${CMS_SVC_NAME_VALUE?upper_case}_START_HDL,           /**< Handle of Primary Service. */
    <#list 0..(CMS_SVC_CHAR_COUNT_ID?eval - 1) as i>
    ${CMS_SVC_NAME_VALUE?upper_case}_HDL_CHAR_${i},                            /**< Handle of characteristic ${i}. */
    ${CMS_SVC_NAME_VALUE?upper_case}_HDL_CHARVAL_${i},                         /**< Handle of characteristic ${i} value. */
    <#if CMS_CHAR_PERM_CCC[i] == true>
    ${CMS_SVC_NAME_VALUE?upper_case}_HDL_CCCD_${i},                            /**< Handle of characteristic ${i} CCCD . */
    </#if>
    </#list>
}BLE_${CMS_SVC_NAME_VALUE?upper_case}_AttributeHandle_T;

/**@defgroup BLE_${CMS_SVC_NAME_VALUE?upper_case}_ASSIGN_HANDLE BLE_${CMS_SVC_NAME_VALUE?upper_case}_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE ${CMS_SVC_NAME_VALUE?capitalize} Service.
 * @{ */
<#if CMS_CHAR_PERM_CCC[CMS_SVC_CHAR_COUNT_ID?eval - 1] == true>
#define ${CMS_SVC_NAME_VALUE?upper_case}_END_HDL                                 (${CMS_SVC_NAME_VALUE?upper_case}_HDL_CCCD_${CMS_SVC_CHAR_COUNT_ID?eval - 1})         /**< The end attribute handle of ${CMS_SVC_NAME_VALUE?lower_case} service. */
<#else>
#define ${CMS_SVC_NAME_VALUE?upper_case}_END_HDL                                 (${CMS_SVC_NAME_VALUE?upper_case}_HDL_CHARVAL_${CMS_SVC_CHAR_COUNT_ID?eval - 1})         /**< The end attribute handle of ${CMS_SVC_NAME_VALUE?lower_case} service. */
</#if>
/** @} */


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 *@brief Initialize BLE ${CMS_SVC_NAME_VALUE?capitalize} Service callback function.
 *
 *
 *@return MBA_RES_SUCCESS                    Successfully register BLE ${CMS_SVC_NAME_VALUE?capitalize} service.
 *@return MBA_RES_NO_RESOURCE                Fail to register service.
 *
 */
uint16_t BLE_${CMS_SVC_NAME_VALUE?upper_case}_Add();


#endif

/**
  @}
 */
