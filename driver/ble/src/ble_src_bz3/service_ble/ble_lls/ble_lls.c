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
  BLE Link Loss Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_lls.c

  Summary:
    Implements the BLE Link Loss Service, providing functions to manage the
    service's behavior for the application user.

  Description:
    This source file includes the necessary functions to manage the BLE Link 
    Loss Service as part of the application's BLE stack. It allows the application 
    to handle events related to the link loss between a BLE connection.
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
#include "ble_lls.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_LLS_MIN_KEY_SIZE                                0x10        // The minimum key size for attribute permissions (LLS).

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* UUIDs for Link Loss Service and its Characteristics */
static const uint8_t s_svcUuidLls[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_LINKLOSS_SERVICE)};
static const uint8_t s_chUuidLlsAlertLevel[ATT_UUID_LENGTH_2] =        {UINT16_TO_BYTES(UUID_ALERT_LEVEL)};

/* Link Loss Service Declaration */
static uint16_t s_svcUuidLlsLen = (uint16_t)sizeof(s_svcUuidLls);

/* Link Loss Alert Level Characteristic Declaration */
static const uint8_t s_charLlsAlertLevel[] = {(ATT_PROP_READ | ATT_PROP_WRITE_REQ), UINT16_TO_BYTES(LLS_HDL_CHARVAL_ALERT_LEVEL), UINT16_TO_BYTES(UUID_ALERT_LEVEL)};
static const uint16_t s_charLlsAlertLevelLen = (uint16_t)sizeof (s_charLlsAlertLevel);

/* Link Loss Alert Level Characteristic Value */
static uint8_t s_llsAlertLevelVal[1] = {0};
static uint16_t s_llsAlertLevelValLen = 1;


/* Attribute list for Link Loss service */
static GATTS_Attribute_T s_llsList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_svcUuidLls,
        (uint16_t *) & s_svcUuidLlsLen,
        (uint16_t)sizeof (s_svcUuidLls),
        0,
        PERMISSION_READ
    },
    /* Characteristic Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_charLlsAlertLevel,
        (uint16_t *) & s_charLlsAlertLevelLen,
        (uint16_t)sizeof (s_charLlsAlertLevel),
        0,
        PERMISSION_READ
    },
    /* Characteristic Value */
    {
        (uint8_t *) s_chUuidLlsAlertLevel,
        (uint8_t *) s_llsAlertLevelVal,
        (uint16_t *) & s_llsAlertLevelValLen,
        (uint16_t)sizeof(s_llsAlertLevelVal),
        SETTING_MANUAL_READ_RSP,
        (PERMISSION_READ | PERMISSION_WRITE | PERMISSION_WRITE_ENC)
    },
};

/* Link Loss Service structure */
static GATTS_Service_T s_svcLls = 
{
    NULL,
    (GATTS_Attribute_T *) s_llsList,
    NULL,
    (uint16_t)LLS_START_HDL,
    (uint16_t)LLS_END_HDL,
    0
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE Link Loss Service to the GATT server.
 *
 * This function adds the BLE Link Loss Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Link Loss service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Link Loss service.
 */
uint16_t BLE_LLS_Add(void)
{
    return GATTS_AddService(&s_svcLls, (uint8_t)((uint16_t)LLS_END_HDL - (uint16_t)LLS_START_HDL + 1U));
}
