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
  Bluetooth Sys PTA Header File

  Company:
    Microchip Technology Inc.

  File Name:
    bt_sys_pta.h

  Summary:
    Defines interfaces for Bluetooth Packet Traffic Arbitration (PTA) management.

  Description:
    This header file declares the API and necessary data structures for managing
    Bluetooth coexistence through Packet Traffic Arbitration. It is part of the
    Bluetooth software stack, which ensures harmonious wireless communication
    in environments with multiple competing radio signals.
 *******************************************************************************/
#ifndef BT_SYS_PTA_H
#define BT_SYS_PTA_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @addtogroup BT_SYS
 * @{
 */

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
/**
 * @addtogroup BT_SYS_PTA_STRUCTS Structures
 * @{
 */

/**
 * @brief Callback type for BT active event in PTA (Packet Traffic Arbitration).
 *
 * @param[in] btActive         Indicates whether BT is requesting access to the RF interface. 
 *                             A value of true means BT is requesting RF, and false means BT is not requesting RF.
 */
typedef void (*BT_SYS_PtaEventBtActiveCb_T)(bool btActive);

/**@brief Callback type for BT priority event in PTA.
 *
 * @param[in] btPriority       Indicates the priority level of BT traffic. 
 *                             A value of true signifies high priority, while false signifies low priority.
 */
typedef void (*BT_SYS_PtaEventBtPriorityCb_T)(bool btPriority);


/**
 * @brief Callback type for retrieving WLAN active status in PTA.
 *
 * @retval true                Indicates that WLAN is currently active.
 * @retval false               Indicates that WLAN is currently inactive.
 */
typedef bool (*BT_SYS_PtaGetWlanStatusCb_T)(void);

/** @} */ //BT_SYS_PTA_STRUCTS

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************

/**
 * @addtogroup BT_SYS_PTA_FUNS Functions
 * @{
 */

/**
 * @brief Initializes the Bluetooth (BT) Priority Traffic Arbitration (PTA) system.
 *
 * @param[in] notifyBtActiveCb              Callback function to notify when BT becomes active. See @ref BT_SYS_PtaEventBtActiveCb_T.
 * @param[in] notifyBtPriorityCb            Callback function to notify about the priority of BT traffic. See @ref BT_SYS_PtaEventBtPriorityCb_T.
 * @param[in] getWlanActiveStatusCb         Callback function to check the active status of WLAN. See @ref BT_SYS_PtaGetWlanStatusCb_T.
 *
 */
void BT_SYS_PtaInit(BT_SYS_PtaEventBtActiveCb_T notifyBtActiveCb, BT_SYS_PtaEventBtPriorityCb_T notifyBtPriorityCb, BT_SYS_PtaGetWlanStatusCb_T getWlanActiveStatusCb);

/**
 * @brief Handles the WLAN active interrupt.
 * @note  This function must be called from the interrupt service routine (ISR) or a similar context where the WLAN active interrupt is detected.
 */
void BT_SYS_PtaWlanActiveIrqHandler();

/** @} */ //BT_SYS_PTA_FUNS

/** @} */

/** @} */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif//BT_SYS_PTA_H