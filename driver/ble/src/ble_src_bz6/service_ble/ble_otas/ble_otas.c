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
  BLE OTA Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otas.c

  Summary:
    Implements the service functions for the Bluetooth Low Energy (BLE) Over-The-Air 
    Software Update feature.

  Description:
    This source file provides the interface and operational functions necessary for 
    managing BLE OTA updates. It is designed for application developers who need to 
    integrate firmware update capabilities over BLE communication.
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
#include "ble_otas.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define BLE_OTAS_CCCD_NUM                                    0x02   // Number of Client Characteristic Configuration Descriptors (CCCDs) for BLE OTA Service.
#define BLE_OTAS_MIN_KEY_SIZE                                0x10   // The minimum key size for attribute permissions (OTAS).

#define UUID_OTA_SERVICE_16                                  0xD7,0x15,0x82,0x8E,0x1B,0xE6,0x23,0x99,0xB3,0x46,0x3D,0x25,0x50,0x48,0x43,0x4D     // 128-bit UUID for OTA Service in little-endian format.
#define UUID_OTA_CHAR_FEATURE                                0x58,0x63,0x90,0x2F,0x4A,0x0C,0x03,0xAF,0x46,0x42,0xE4,0x22,0x50,0x48,0x43,0x4D     // 128-bit UUID for OTA Feauture Characteristic in little-endian format.
#define UUID_OTA_CHAR_CTRL_PT                                0x76,0x8A,0x02,0x39,0x7F,0xC9,0x82,0x88,0xDE,0x45,0x27,0x93,0x50,0x48,0x43,0x4D     // 128-bit UUID for OTA Control Point Characteristic in little-endian format.
#define UUID_OTA_CHAR_DATA                                   0x78,0xD4,0x8C,0x7C,0xF5,0x56,0x7E,0xBA,0xA6,0x40,0xD9,0x34,0x50,0x48,0x43,0x4D     // 128-bit UUID for OTA Data Characteristic in little-endian format.


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* Service Declaration */
static const uint8_t s_svcUuidOtas[] = {UUID_OTA_SERVICE_16};
static const uint16_t s_svcUuidOtasLen = (uint16_t)sizeof(s_svcUuidOtas);

/* Feature Characteristic Declaration */
static const uint8_t s_charOtasFeature[] = {ATT_PROP_READ, UINT16_TO_BYTES(BLE_OTAS_HDL_FEATURE_VAL), UUID_OTA_CHAR_FEATURE};
static const uint16_t s_charOtasFeatureLen = (uint16_t)sizeof(s_charOtasFeature);

/* Feature Characteristic Value */
static const uint8_t s_chUuidOtasFeature[] = {UUID_OTA_CHAR_FEATURE};
static uint8_t s_otasFeatureVal[1] = {0};
static uint16_t s_otasFeatureValLen = 1;

/* Control Point Characteristic Declaration */
static const uint8_t s_charOtasCtrl[] = {(ATT_PROP_WRITE_REQ|ATT_PROP_NOTIFY), UINT16_TO_BYTES(BLE_OTAS_HDL_CTRL_VAL), UUID_OTA_CHAR_CTRL_PT};
static const uint16_t s_charOtasCtrlLen = (uint16_t)sizeof(s_charOtasCtrl);

/* Control Point Characteristic Value */
static const uint8_t s_chUuidOtasCtrl[] = {UUID_OTA_CHAR_CTRL_PT};
static uint8_t s_otasCtrlVal[1] = {0};
static uint16_t s_otasCtrlValLen = 1;

/* Control Point Client Characteristic Configuration Descriptor */
static uint8_t s_descCccOtasCtrl[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_descCccOtasCtrlLen =(uint16_t)sizeof(s_descCccOtasCtrl);

/* Data Characteristic Declaration */
static const uint8_t s_charOtasData[] = {ATT_PROP_WRITE_CMD|ATT_PROP_NOTIFY, UINT16_TO_BYTES(BLE_OTAS_HDL_DATA_VAL), UUID_OTA_CHAR_DATA};
static const uint16_t s_charOtasDataLen = (uint16_t)sizeof(s_charOtasData);

/* Data Characteristic Value */
static const uint8_t s_chUuidOtasData[] = {UUID_OTA_CHAR_DATA};
static uint8_t s_otasDataVal[1] = {0};
static uint16_t s_otasDataValLen = 1;

/* Data Client Characteristic Configuration Descriptor */
static uint8_t s_descCccOtasData[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_descCccOtasDataLen = (uint16_t)sizeof(s_descCccOtasData);

/* Attributes list for OTA service */
static GATTS_Attribute_T s_otasList[] =
{
    /* Service Delcaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidOtas,
        (uint16_t *) &s_svcUuidOtasLen,
        (uint16_t)sizeof(s_svcUuidOtas),
        0,
        PERMISSION_READ
    },
    /* Characteristic Delcaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charOtasFeature,
        (uint16_t *) &s_charOtasFeatureLen,
        (uint16_t)sizeof(s_charOtasFeature),
        0,
        (PERMISSION_READ)
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidOtasFeature,
        (uint8_t *) s_otasFeatureVal,
        (uint16_t *) &s_otasFeatureValLen,
        (uint16_t)sizeof(s_otasFeatureVal),
        (SETTING_MANUAL_READ_RSP|SETTING_UUID_16),
        (PERMISSION_READ|PERMISSION_READ_ENC)
    },
    /* Characteristic Delcaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charOtasCtrl,
        (uint16_t *) &s_charOtasCtrlLen,
        (uint16_t)sizeof(s_charOtasCtrl),
        0,
        (PERMISSION_READ)
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidOtasCtrl,
        (uint8_t *) s_otasCtrlVal,
        (uint16_t *) &s_otasCtrlValLen,
        BLE_ATT_MAX_MTU_LEN,
        (SETTING_MANUAL_WRITE_RSP|SETTING_UUID_16|SETTING_VARIABLE_LEN),
        (PERMISSION_WRITE|PERMISSION_WRITE_ENC)
    },
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        s_descCccOtasCtrl,
        (uint16_t *) &s_descCccOtasCtrlLen,
        (uint16_t)sizeof(s_descCccOtasCtrl),
        (SETTING_MANUAL_WRITE_RSP|SETTING_CCCD),
        (PERMISSION_READ|PERMISSION_READ_ENC|PERMISSION_WRITE|PERMISSION_WRITE_ENC)
    },
    /* Characteristic Delcaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charOtasData,
        (uint16_t *) &s_charOtasDataLen,
        (uint16_t)sizeof(s_charOtasData),
        0,
        (PERMISSION_READ)
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidOtasData,
        (uint8_t *) s_otasDataVal,
        (uint16_t *) &s_otasDataValLen,
        BLE_ATT_MAX_MTU_LEN,
        (SETTING_MANUAL_WRITE_RSP|SETTING_UUID_16|SETTING_VARIABLE_LEN),
        (PERMISSION_WRITE|PERMISSION_WRITE_ENC)
    },
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        s_descCccOtasData,
        (uint16_t *) &s_descCccOtasDataLen,
        (uint16_t)sizeof(s_descCccOtasData),
        (SETTING_MANUAL_WRITE_RSP|SETTING_CCCD),
        (PERMISSION_READ|PERMISSION_READ_ENC|PERMISSION_WRITE|PERMISSION_WRITE_ENC)
    }
};

/* CCCD settings for the BLE OTA Service characteristics. */
static const GATTS_CccdSetting_T s_otasCccdSetting[] = 
{
    {(uint16_t)BLE_OTAS_HDL_CTRL_CCCD, (NOTIFICATION)},
    {(uint16_t)BLE_OTAS_HDL_DATA_CCCD, (NOTIFICATION)}
};

/* BLE OTA Service structure */
static GATTS_Service_T s_svcOtas =
{
    NULL,
    (GATTS_Attribute_T *) s_otasList,
    (GATTS_CccdSetting_T *)s_otasCccdSetting,
    (uint16_t)BLE_OTAS_START_HDL,
    (uint16_t)BLE_OTAS_END_HDL,
    BLE_OTAS_CCCD_NUM
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE OTA Service to the GATT server.
 *
 * This function adds the BLE OTA Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE OTA service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE OTA service.
 */
uint16_t BLE_OTAS_Add(void)
{
    return GATTS_AddService(&s_svcOtas, (uint8_t)((uint16_t)BLE_OTAS_END_HDL - (uint16_t)BLE_OTAS_START_HDL + 1U));
}
