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
  BLE Transparent Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_trcbs.c

  Summary:
    Implements the BLE Transparent Credit Based Service (TRCB) for data transfer.

  Description:
    This source file provides the interface and handling functions for the BLE
    Transparent Credit Based Service, enabling the application to facilitate
    data transfer operations over BLE with credit-based flow control.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include <stdint.h>
#include "mba_error_defs.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_trcbs.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define BLE_TRCBS_MAX_MTU_LEN                                      BLE_ATT_MAX_MTU_LEN  // Maximum MTU length for BLE Transparent Credit Based Service.
#define BLE_TRCBS_CCCD_NUM                                         0x01                 // Number of Client Characteristic Configuration Descriptors (CCCDs) for BLE Transparent Credit Based Service.
#define BLE_TRCBS_MIN_KEY_SIZE                                     0x10                 // The minimum key size for attribute permissions (TRCBS).

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* BLE Transparent Credit Based Service Declaration */
static uint8_t s_svcUuidBleTrcb[] = {UUID_MCHP_PROPRIETARY_SERVICE_TRCB_16};
static const uint16_t s_svcUuidBleTrcbLen = (uint16_t)sizeof (s_svcUuidBleTrcb);

/* BLE Transparent Credit Based Ctrl Point Characteristic Declaration */
static uint8_t s_charBleTrcbCtrl[] = {(ATT_PROP_WRITE_CMD | ATT_PROP_WRITE_REQ | ATT_PROP_NOTIFY), UINT16_TO_BYTES(BLE_TRCB_HDL_CHARVAL_CTRL), UUID_MCHP_TRCB_CTRL_16};
static const uint16_t s_charBleTrcbCtrlLen = (uint16_t)sizeof (s_charBleTrcbCtrl);

/* BLE Transparent Credit Based Ctrl Point Characteristic Value */
static uint8_t s_chUuidBleTrcbCtrl[] = {UUID_MCHP_TRCB_CTRL_16};
static uint8_t s_bleTrcbCtrlVal[1] = {0};
static uint16_t s_bleTrcbCtrlValLen = 1;

/* BLE Transparent Credit Based Ctrl Point Client Characteristic Configuration Descriptor */
static uint8_t s_descCccBleTrcbCtrl[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_descCccBleTrcbCtrlLen = (uint16_t)sizeof (s_descCccBleTrcbCtrl);


/* BLE Transparent Credit Based PSM Characteristic Declaration */
static const uint8_t s_charBleTrcbPsm[] = {ATT_PROP_READ, UINT16_TO_BYTES(BLE_TRCB_HDL_CHARVAL_L2CAP_PSM), UUID_MCHP_TRCB_L2CAP_PSM_16};
static const uint16_t s_charBleTrcbPsmLen = (uint16_t)sizeof(s_charBleTrcbPsm);

/* BLE Transparent Credit Based PSM Characteristic Value */
static uint8_t s_chUuidBleTrcbPsm[] = {UUID_MCHP_TRCB_L2CAP_PSM_16};
static uint8_t s_bleTrcbPsmVal[2] = {UINT16_TO_BYTES_BE(BLE_TRCB_DATA_PSM)};
static uint16_t s_bleTrcbPsmValLen = (uint16_t)sizeof(s_bleTrcbPsmVal);


/* Attribute list for Transparent service */
static GATTS_Attribute_T s_bleTrcbList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidBleTrcb,
        (uint16_t *) & s_svcUuidBleTrcbLen,
        (uint16_t)sizeof (s_svcUuidBleTrcb),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charBleTrcbCtrl,
        (uint16_t *) & s_charBleTrcbCtrlLen,
        (uint16_t)sizeof (s_charBleTrcbCtrl),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidBleTrcbCtrl,
        (uint8_t *) s_bleTrcbCtrlVal,
        (uint16_t *) & s_bleTrcbCtrlValLen,
        BLE_TRCBS_MAX_MTU_LEN,
        (SETTING_MANUAL_WRITE_RSP | SETTING_UUID_16 | SETTING_VARIABLE_LEN),
        PERMISSION_WRITE
    },
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        (uint8_t *) s_descCccBleTrcbCtrl,
        (uint16_t *) & s_descCccBleTrcbCtrlLen,
        (uint16_t)sizeof (s_descCccBleTrcbCtrl),
        (SETTING_MANUAL_WRITE_RSP | SETTING_CCCD),
        (PERMISSION_READ | PERMISSION_WRITE)
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charBleTrcbPsm,
        (uint16_t *) & s_charBleTrcbPsmLen,
        (uint16_t)sizeof (s_charBleTrcbPsm),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidBleTrcbPsm,
        (uint8_t *) s_bleTrcbPsmVal,
        (uint16_t *) & s_bleTrcbPsmValLen,
        (uint16_t)sizeof(s_bleTrcbPsmVal),
        0,
        PERMISSION_READ
    }
};


/* CCCD settings for the Transparent Credit Based Service characteristics. */
static const GATTS_CccdSetting_T s_trcbCccdSetting[] =
{
    {(uint16_t)BLE_TRCB_HDL_CCCD_CTRL, (NOTIFICATION)}
};

/* BLE Transparent Credit Based Service structure */
static GATTS_Service_T s_svcBleTrcb = 
{
    NULL,
    (GATTS_Attribute_T *) s_bleTrcbList,
    (GATTS_CccdSetting_T *)s_trcbCccdSetting,
    (uint16_t)BLE_TRCB_START_HDL,
    (uint16_t)BLE_TRCB_END_HDL,
    BLE_TRCBS_CCCD_NUM
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE Transparent Credit Based Service to the GATT server.
 *
 * This function adds the BLE Transparent Credit Based Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Transparent Credit Based service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Transparent Credit Based service.
 */
uint16_t BLE_TRCBS_Add(void)
{
    return GATTS_AddService(&s_svcBleTrcb, (uint8_t)((uint16_t)BLE_TRCB_END_HDL - (uint16_t)BLE_TRCB_START_HDL + 1U));
}
