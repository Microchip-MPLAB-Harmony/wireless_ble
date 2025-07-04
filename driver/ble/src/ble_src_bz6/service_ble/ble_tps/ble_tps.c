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
  BLE Tx Power Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_tps.c

  Summary:
    Implements the BLE) TX Power Service (TPS), providing interfaces for managing 
    Tx power levels in a BLE application.

  Description:
    This source file includes the necessary functions to interact with the BLE
    TX Power Service, allowing an application to monitor the transmission power 
    of a BLE device .
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
#include "ble_tps.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_TPS_MIN_KEY_SIZE                                0x10        // The minimum key size for attribute permissions (TPS).

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* UUIDs for BLE TX Power Service and its Characteristics */
static const uint8_t s_svcUuidTps[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_TXPOWER_SERVICE)};
static const uint8_t s_chUuidTxPowerLevel[ATT_UUID_LENGTH_2] =         {UINT16_TO_BYTES(UUID_TXPOWER_LEVEL)};

/* BLE TX Power Service Declaration */
static uint16_t s_svcUuidTpsLen = (uint16_t)sizeof(s_svcUuidTps);

/* BLE TX Power Level Characteristic Declaration */
static const uint8_t s_charTxPowerLevel[] = {(ATT_PROP_READ), UINT16_TO_BYTES(TPS_HDL_CHARVAL_TXPOWER_LEVEL), UINT16_TO_BYTES(UUID_TXPOWER_LEVEL)};
static const uint16_t s_charTxPowerLevelLen = (uint16_t)sizeof (s_charTxPowerLevel);

/* BLE TX Power Level Characteristic Value */
static uint8_t s_txPowerLevelVal[1] = {0};
static uint16_t s_txPowerLevelValLen = 1;

/* Attribute list for BLE TX Power service */
static GATTS_Attribute_T s_tpsList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidTps,
        (uint16_t *) & s_svcUuidTpsLen,
        (uint16_t)sizeof (s_svcUuidTps),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charTxPowerLevel,
        (uint16_t *) & s_charTxPowerLevelLen,
        (uint16_t)sizeof (s_charTxPowerLevel),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidTxPowerLevel,
        (uint8_t *) s_txPowerLevelVal,
        (uint16_t *) & s_txPowerLevelValLen,
        (uint16_t)sizeof(s_txPowerLevelVal),
        (SETTING_MANUAL_READ_RSP),
        PERMISSION_READ
    },
};

/* BLE TX Power Service structure */
static GATTS_Service_T s_svcTps =
{
    NULL,
    (GATTS_Attribute_T *) s_tpsList,
    NULL,
    (uint16_t)TPS_START_HDL,
    (uint16_t)TPS_END_HDL,
    0
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE TX Power Service to the GATT server.
 *
 * This function adds the BLE TX Power Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE TX Power service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE TX Power service.
 */
uint16_t BLE_TPS_Add(void)
{
    return GATTS_AddService(&s_svcTps, (uint8_t)((uint16_t)TPS_END_HDL - (uint16_t)TPS_START_HDL + 1U));
}
