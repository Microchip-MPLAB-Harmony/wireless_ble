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
  BLE Battery Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_bas.c

  Summary:
    Implements the BLE Battery Service, providing battery level information
    over BLE to connected devices.

  Description:
    This source file includes the necessary functions to manage the BLE Battery 
    Service as part of a BLE application. It allows for reporting the battery 
    status to a connected BLE device.
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
#include "ble_bas.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define BLE_BAS_MIN_KEY_SIZE            0x10U                    // The minimum key size for attribute permissions (BAS).
#define BLE_BAS_BATTERY_LEVEL_0         0x00U                    // The minimum battery level (0%) in the BLE Battery Service.
#define BLE_BAS_BATTERY_LEVEL_100       0x64U                    // The maximum battery level (100%) in the BLE Battery Service.

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* UUIDs for Battery Device Service and its Characteristics */
static const uint8_t s_svcUuidBas[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_BATTERY_SERVICE)};
static const uint8_t s_chUuidBatLevel[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};

/* Battery Service Declaration */
static uint16_t s_svcUuidBasLen = (uint16_t)sizeof(s_svcUuidBas);

/* Battery Level Characteristic Declaration */
#ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
static const uint8_t s_charBatLevel[] = {(ATT_PROP_READ | ATT_PROP_NOTIFY), UINT16_TO_BYTES(BAS_HDL_CHARVAL_BAT_LEVEL), UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};
#else
static const uint8_t s_charBatLevel[] = {(ATT_PROP_READ ), UINT16_TO_BYTES(BAS_HDL_CHARVAL_BAT_LEVEL), UINT16_TO_BYTES(UUID_BATTERY_LEVEL)};
#endif

static const uint16_t s_charBatLevelLen = (uint16_t)sizeof (s_charBatLevel);

/* Battery Level Characteristic Value */
static uint8_t s_batLevelVal[1] = {0};
static uint16_t s_batLevelValLen = 1;

#ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
/* Battery Level Client Characteristic Configuration Descriptor */
static uint8_t s_descCccBatLevel[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_descCccBatLevelLen = (uint16_t)sizeof (s_descCccBatLevel);
#endif

/* Attribute list for Battery service */
static GATTS_Attribute_T s_basList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidBas,
        (uint16_t *) &s_svcUuidBasLen,
        (uint16_t)sizeof (s_svcUuidBas),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charBatLevel,
        (uint16_t *) &s_charBatLevelLen,
        (uint16_t)sizeof (s_charBatLevel),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidBatLevel,
        (uint8_t *) s_batLevelVal,
        (uint16_t *) &s_batLevelValLen,
        (uint16_t)sizeof(s_batLevelVal),
        0,
        PERMISSION_READ
    },
#ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        (uint8_t *) s_descCccBatLevel,
        (uint16_t *) &s_descCccBatLevelLen,
        (uint16_t)sizeof (s_descCccBatLevel),
        (SETTING_CCCD),
        (PERMISSION_READ | PERMISSION_WRITE)
    },
#endif
};

#ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
/* CCCD settings for the Battery Service characteristics. */
static const GATTS_CccdSetting_T s_basCccdSetting[] =
{
    {BAS_HDL_CCCD_BAT_LEVEL, (NOTIFICATION)}
};
#endif

/* Battery Service structure */
static GATTS_Service_T s_svcBas = 
{
    NULL,
    (GATTS_Attribute_T *) s_basList,
    #ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
    (GATTS_CccdSetting_T const *)s_basCccdSetting,
    #else
    NULL,
    #endif
    BAS_START_HDL,
    BAS_END_HDL,
    #ifdef BAS_BAT_LEVEL_NOTIFY_ENABLE
    1
    #else
    0
    #endif
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE Battery Service to the GATT server.
 *
 * This function adds the BLE Battery Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Battery service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Battery service.
 *
 */
uint16_t BLE_BAS_Add(void)
{
    return GATTS_AddService(&s_svcBas, (uint8_t)((uint16_t)BAS_END_HDL - (uint16_t)BAS_START_HDL + 1U));
}


/**
 * @brief Sets the battery level for the BLE Battery Service.
 *
 * @param[in] batteryLevel                   The new battery level to set (0-100%).
 *
 * @retval MBA_RES_SUCCESS                   The battery level was successfully set.
 * @retval MBA_RES_INVALID_PARA              The provided batteryLevel is out of the expected range.
 *
 */
uint16_t BLE_BAS_SetBatteryLevel(uint8_t batteryLevel)
{
    if (batteryLevel <= BLE_BAS_BATTERY_LEVEL_100)
    {
        s_batLevelVal[0] = batteryLevel;
        return MBA_RES_SUCCESS;
    }
    else
    {
        return MBA_RES_INVALID_PARA;
    }
}


/**
 * @brief Retrieves the current battery level from the BLE Battery Service.
 *
 * @param[in] p_batteryLevel                 Pointer to a variable where the battery level will be stored.
 *
 * @retval MBA_RES_SUCCESS                   The battery level was successfully retrieved.
 *
 */
uint16_t BLE_BAS_GetBatteryLevel(uint8_t *p_batteryLevel)
{
    *p_batteryLevel = s_batLevelVal[0];
    return MBA_RES_SUCCESS;
}
