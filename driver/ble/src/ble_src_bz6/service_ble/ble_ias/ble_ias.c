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
  BLE Immediate Alert Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_ias.c

  Summary:
    Implements the server-side functionality of the BLE Immediate Alert Service.

  Description:
    This source file provides the implementation of the server-side functions
    for the Bluetooth Low Energy (BLE) Immediate Alert Service, enabling the
    device to receive alert notifications from a connected BLE client.
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
#include "ble_ias.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_IAS_MIN_KEY_SIZE                                0x10        // The minimum key size for attribute permissions (IAS).

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* UUIDs for Immediate Alert Service and its Characteristics */
static const uint8_t s_svcUuidIas[ATT_UUID_LENGTH_2] =              {UINT16_TO_BYTES(UUID_IMMEDIATE_ALERT_SERVICE)};
static const uint8_t s_chUuidAlertLevel[ATT_UUID_LENGTH_2] =        {UINT16_TO_BYTES(UUID_ALERT_LEVEL)};

/* Immediate Alert Service Declaration */
static uint16_t s_svcUuidIasLen = (uint16_t)sizeof(s_svcUuidIas);

/* Immediate Alert Alert Level Characteristic Declaration */
static const uint8_t s_charIasAlertLevel[] = {(ATT_PROP_WRITE_CMD), UINT16_TO_BYTES(IAS_HDL_CHARVAL_ALERT_LEVEL), UINT16_TO_BYTES(UUID_ALERT_LEVEL)};
static const uint16_t s_charIasAlertLevelLen = (uint16_t)sizeof (s_charIasAlertLevel);

/* Immediate Alert Alert Level Characteristic Value */
static uint8_t s_iasAlertLevelVal[1] = {0};
static uint16_t s_iasAlertLevelValLen = 1;


/* Attribute list for Immediate Alert service */
static GATTS_Attribute_T s_iasList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidIas,
        (uint16_t *) & s_svcUuidIasLen,
        (uint16_t)sizeof (s_svcUuidIas),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charIasAlertLevel,
        (uint16_t *) & s_charIasAlertLevelLen,
        (uint16_t)sizeof (s_charIasAlertLevel),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidAlertLevel,
        (uint8_t *) s_iasAlertLevelVal,
        (uint16_t *) & s_iasAlertLevelValLen,
        (uint16_t)sizeof(s_iasAlertLevelVal),
        0,
        PERMISSION_WRITE | PERMISSION_WRITE_ENC
    },
};

/* Immediate Alert Service structure */
static GATTS_Service_T s_svcIas =
{
    NULL,
    (GATTS_Attribute_T *) s_iasList,
    NULL,
    (uint16_t)IAS_START_HDL,
    (uint16_t)IAS_END_HDL,
    0
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE Immediate Alert Service to the GATT server.
 *
 * This function adds the BLE Immediate Alert Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Immediate Alert service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Immediate Alert service.
 */
uint16_t BLE_IAS_Add(void)
{
    return GATTS_AddService(&s_svcIas, (uint8_t)((uint16_t)IAS_END_HDL - (uint16_t)IAS_START_HDL + 1U));
}
