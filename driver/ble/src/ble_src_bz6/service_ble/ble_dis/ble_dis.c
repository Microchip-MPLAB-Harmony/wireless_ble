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
  BLE Device Information Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_dis.c

  Summary:
    Implements the functions for the BLE Device Information Service (DIS) used 
    by the application.

  Description:
    This source file provides the interface and functionality required to interact
    with the BLE Device Information Service, which allows an application to expose
    manufacturer and device information to a connected BLE peer.
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
#include "ble_dis.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define BLE_DIS_MIN_KEY_SIZE                           0x10    // The minimum key size for attribute permissions (DIS).

#define UUID_MANUFACTURER_NAME                         0x2A29  /**< Manufacturer Name. */
#define UUID_MODEL_NUMBER                              0x2A24  /**< Model Numbr. */
#define UUID_SERIAL_NUMBER                             0x2A25  /**< Serial Number. */
#define UUID_HARDWARE_REVISION                         0x2A27  /**< Hardware Revision. */
#define UUID_FIRMWARE_REVISION                         0x2A26  /**< Firmware Revision. */
#define UUID_SOFTWARE_REVISION                         0x2A28  /**< Software Revision. */
#define UUID_SYSTEM_ID                                 0x2A23  /**< System ID. */
#define UUID_IEEE_11073_20601_RCDL                     0x2A2A  /**< IEEE 11073-20601 Regulatory Certification Data List. */
#define UUID_PNP_ID                                    0x2A50  /**< PNP ID. */


// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
/* UUIDs for Device Information Service and its Characteristics */
static const uint8_t s_svcUuidDis[ATT_UUID_LENGTH_2] =                 {UINT16_TO_BYTES(UUID_DEVICE_INFO_SERVICE)};

#ifdef DIS_MANU_NAME_ENABLE
static const uint8_t s_chUuidManuName[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_MANUFACTURER_NAME)};
#endif

#ifdef DIS_MODEL_NUM_ENABLE
static const uint8_t s_chUuidModelNum[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_MODEL_NUMBER)};
#endif

#ifdef DIS_SERIAL_NUM_ENABLE
static const uint8_t s_chUuidSerialNum[ATT_UUID_LENGTH_2] =            {UINT16_TO_BYTES(UUID_SERIAL_NUMBER)};
#endif

#ifdef DIS_HW_REV_ENABLE
static const uint8_t s_chUuidHwRev[ATT_UUID_LENGTH_2] =                {UINT16_TO_BYTES(UUID_HARDWARE_REVISION)};
#endif

#ifdef DIS_FW_REV_ENABLE
static const uint8_t s_chUuidFwRev[ATT_UUID_LENGTH_2] =                {UINT16_TO_BYTES(UUID_FIRMWARE_REVISION)};
#endif

#ifdef DIS_SW_REV_ENABLE
static const uint8_t s_chUuidSwRev[ATT_UUID_LENGTH_2] =                {UINT16_TO_BYTES(UUID_SOFTWARE_REVISION)};
#endif

#ifdef DIS_SYSTEM_ID_ENABLE
static const uint8_t s_chUuidSystemId[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_SYSTEM_ID)};
#endif

#ifdef DIS_IEEE_ENABLE
static const uint8_t s_chUuidIeeeRcdl[ATT_UUID_LENGTH_2] =             {UINT16_TO_BYTES(UUID_IEEE_11073_20601_RCDL)};
#endif

#ifdef DIS_PNP_ID_ENABLE
static const uint8_t s_chUuidPnpId[ATT_UUID_LENGTH_2] =                {UINT16_TO_BYTES(UUID_PNP_ID)};
#endif

/* Device Information Service Declaration */
static const uint16_t s_svcUuidDisLen = (uint16_t)sizeof(s_svcUuidDis);

#ifdef DIS_MANU_NAME_ENABLE
/* Manufacture Name Characteristic */
static const uint8_t s_charManuName[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_MANU_NAME), UINT16_TO_BYTES(UUID_MANUFACTURER_NAME)};
static const uint16_t s_charManuNameLen = (uint16_t)sizeof(s_charManuName);

/* Manufacture Name Value */
static uint8_t s_manuNameVal[]={DIS_MANU_NAME};
static uint16_t s_manuNameValLen = (uint16_t)sizeof(s_manuNameVal);
#endif

#ifdef DIS_MODEL_NUM_ENABLE
/* Model Number Characteristic */
static const uint8_t s_charModelNum[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_MODEL_NUM), UINT16_TO_BYTES(UUID_MODEL_NUMBER)};
static const uint16_t s_charModelNumLen = (uint16_t)sizeof(s_charModelNum);

/* Model Number Value */
static uint8_t s_modelNumVal[] = {DIS_MODEL_NUM};
static uint16_t s_modelNumValLen = (uint16_t)sizeof(s_modelNumVal);
#endif

#ifdef DIS_SERIAL_NUM_ENABLE
/* Serial Number Characteristic */
static const uint8_t s_charSerialNum[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_SERIAL_NUM), UINT16_TO_BYTES(UUID_SERIAL_NUMBER)};
static const uint16_t s_charSerialNumLen = (uint16_t)sizeof(s_charSerialNum);

/* Serial Number Value */
static uint8_t s_serialNumVal[] = {DIS_SERIAL_NUM};
static uint16_t s_serialNumValLen = (uint16_t)sizeof(s_serialNumVal);
#endif

#ifdef DIS_HW_REV_ENABLE
/* Hardware Revision Characteristic */
static const uint8_t s_charHwRev[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_HW_REV), UINT16_TO_BYTES(UUID_HARDWARE_REVISION)};
static const uint16_t s_charHwRevLen = (uint16_t)sizeof(s_charHwRev);

/* Hardware Revision Value */
static uint8_t s_hwRevVal[] = {DIS_HW_REVISION};
static uint16_t s_hwRevValLen = (uint16_t)sizeof(s_hwRevVal);
#endif

#ifdef DIS_FW_REV_ENABLE
/* Firmware Revision Characteristic */
static const uint8_t s_charFwRev[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_FW_REV), UINT16_TO_BYTES(UUID_FIRMWARE_REVISION)};
static const uint16_t s_charFwRevLen = (uint16_t)sizeof(s_charFwRev);

/* Firmware Revision Value */
static uint8_t s_fwRevVal[] = {DIS_FW_REVISION};
static uint16_t s_fwRevValLen = (uint16_t)sizeof(s_fwRevVal);
#endif

#ifdef DIS_SW_REV_ENABLE
/* Software Revision Characteristic */
static const uint8_t s_charSwRev[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_SW_REV), UINT16_TO_BYTES(UUID_SOFTWARE_REVISION)};
static const uint16_t s_charSwRevLen = (uint16_t)sizeof(s_charSwRev);

/* Software Revision Value */
static uint8_t s_swRevVal[] = {DIS_SW_REVISION};
static uint16_t s_swRevValLen = (uint16_t)sizeof(s_swRevVal);
#endif

#ifdef DIS_SYSTEM_ID_ENABLE
/* System Id Characteristic */
static const uint8_t s_charSystemId[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_SYSTEM_ID), UINT16_TO_BYTES(UUID_SYSTEM_ID)};
static const uint16_t s_charSystemIdLen = (uint16_t)sizeof(s_charSystemId);

/* System Id Value */
static uint8_t s_systemIdVal[] = {DIS_SYSTEM_ID};
static uint16_t s_systemIdValLen = (uint16_t)sizeof(s_systemIdVal);
#endif

#ifdef DIS_IEEE_ENABLE
/* IEEE 11073-20601 Regulatory Certification Data List Characteristic */
static const uint8_t s_charIeee[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_IEEE_RCDL), UINT16_TO_BYTES(UUID_IEEE_11073_20601_RCDL)};
static const uint16_t s_charIeeeLen = (uint16_t)sizeof(s_charIeee);

/* IEEE 11073-20601 Regulatory Certification Data List Value */
static uint8_t s_ieeeVal[] = {DIS_IEEE_11073_20601};
static uint16_t s_ieeeValLen = (uint16_t)sizeof(s_ieeeVal);
#endif


#ifdef DIS_PNP_ID_ENABLE
/* PnpID Characteristic */
static const uint8_t s_charPnpId[] = {(ATT_PROP_READ), UINT16_TO_BYTES(DIS_HDL_CHARVAL_PNP_ID), UINT16_TO_BYTES(UUID_PNP_ID)};
static const uint16_t s_charPnpIdLen = (uint16_t)sizeof(s_charPnpId);

/* PnpID Value */
static uint8_t s_pnpIdVal[] = {DIS_PNP_ID};
static uint16_t s_pnpIdValLen = (uint16_t)sizeof(s_pnpIdVal);
#endif

/* Attribute list for Device Information service */
static GATTS_Attribute_T s_disList[DIS_END_HDL-DIS_START_HDL+1];

/* Device Information Service structure */
static GATTS_Service_T s_svcDis;

/* Index to keep track of the current attribute within the Device Information Service */
static uint8_t s_disAttrIndex;

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Adds the BLE Device Information Service to the GATT server.
 *
 * This function adds the BLE Device Information Service to the BLE stack's GATT server,
 * enabling the service to be discovered and accessed by remote BLE devices.
 * 
 * @retval MBA_RES_SUCCESS                    The BLE Device Information service was successfully added.
 * @retval MBA_RES_NO_RESOURCE                Insufficient resource to add the BLE Device Information service.
 *
 */
uint16_t BLE_DIS_Add(void)
{
    uint16_t result;

    s_disAttrIndex=0;

    /* Initialize attribute list */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidPrimSvc;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_svcUuidDis;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_svcUuidDisLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_svcUuidDis);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

#ifdef DIS_MANU_NAME_ENABLE
    /* Manufacturer Name */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charManuName;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charManuNameLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charManuName);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidManuName;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_manuNameVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_manuNameValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_manuNameVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_MODEL_NUM_ENABLE
    /* Model Number */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charModelNum;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charModelNumLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charModelNum);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidModelNum;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_modelNumVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_modelNumValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_modelNumVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_SERIAL_NUM_ENABLE
    /* Serial Number */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charSerialNum;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charSerialNumLen;
    s_disList[s_disAttrIndex].maxLen =(uint16_t) sizeof(s_charSerialNum);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidSerialNum;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_serialNumVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_serialNumValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_serialNumVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_HW_REV_ENABLE
    /* Hardware Revision */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charHwRev;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charHwRevLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charHwRev);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidHwRev;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_hwRevVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_hwRevValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_hwRevVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_FW_REV_ENABLE
    /* Firmware Revision */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charFwRev;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charFwRevLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charFwRev);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidFwRev;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_fwRevVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_fwRevValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_fwRevVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_SW_REV_ENABLE
    /* Software Revision */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charSwRev;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charSwRevLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charSwRev);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidSwRev;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_swRevVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_swRevValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_swRevVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_SYSTEM_ID_ENABLE
    /* System ID */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charSystemId;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charSystemIdLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charSystemId);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidSystemId;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_systemIdVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_systemIdValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_systemIdVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_IEEE_ENABLE
    /* IEEE 11073-20601 Regulatory Certification Data List */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charIeee;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charIeeeLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charIeee);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidIeeeRcdl;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_ieeeVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_ieeeValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_ieeeVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

#ifdef DIS_PNP_ID_ENABLE
    /* PNP ID */
    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) g_gattUuidChar;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_charPnpId;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_charPnpIdLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_charPnpId);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;

    s_disList[s_disAttrIndex].p_uuid = (uint8_t *) s_chUuidPnpId;
    s_disList[s_disAttrIndex].p_value = (uint8_t *) s_pnpIdVal;
    s_disList[s_disAttrIndex].p_len = (uint16_t *) &s_pnpIdValLen;
    s_disList[s_disAttrIndex].maxLen = (uint16_t)sizeof(s_pnpIdVal);
    s_disList[s_disAttrIndex].settings = 0;
    s_disList[s_disAttrIndex].permissions = PERMISSION_READ;
    s_disAttrIndex++;
#endif

    /* Initialize Service */
    s_svcDis.p_next = NULL;
    s_svcDis.p_attrList = (GATTS_Attribute_T *) s_disList;
    s_svcDis.p_cccdSetting = NULL;
    s_svcDis.startHandle = DIS_START_HDL;
    s_svcDis.endHandle = DIS_START_HDL+s_disAttrIndex-1;
    s_svcDis.cccdNumber = 0;
    result=GATTS_AddService(&s_svcDis, s_disAttrIndex);
    return result;
}
