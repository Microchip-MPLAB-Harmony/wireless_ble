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
  BLE Log Middleware Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_log.c

  Summary:
    Implements logging functionality for Bluetooth Low Energy (BLE) events.

  Description:
    Provides an interface for logging and debugging BLE events within the
    application. Functions include initializing the log system, writing log
    messages, and setting log levels.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "osal/osal_freertos_extend.h"
#include "ble_util/byte_stream.h"
#include "ble_log.h"
#include "host_hci_defs.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define HCI_COMMAND_OPCODE_SIZE             0x02U               // Size in bytes of the opcode parameter in an HCI command packet.
#define HCI_COMMAND_PARAMETER_LENGTH_SIZE   0x01U               // Size in bytes of the length parameter in an HCI command packet.

#define HCI_EVENT_CODE_SIZE                 0x01U               // Size in bytes of the event code parameter in an HCI event packet.
#define HCI_EVENT_PARAMETER_LENGTH_SIZE     0x01U               // Size in bytes of the length parameter in an HCI event packet.
#define HCI_EVENT_CC_PARAMETER_NUMBER_SIZE  0x01U               // Size in bytes of the number of HCI command parameters in a command complete event.
#define HCI_EVENT_CC_PARAMETER_OPCODE_SIZE  0x02U               // Size in bytes of the opcode parameter in a command complete event.
#define HCI_EVENT_CC_PARAMETER_STATUS_SIZE  0x01U               // Size in bytes of the status parameter in a command complete event.
#define HCI_EVENT_CS_PARAMETER_NUMBER_SIZE  0x01U               // Size in bytes of the number of HCI command parameters in a command status event.
#define HCI_EVENT_CS_PARAMETER_OPCODE_SIZE  0x02U               // Size in bytes of the opcode parameter in a command status event.
#define HCI_EVENT_CS_PARAMETER_STATUS_SIZE  0x01U               // Size in bytes of the status parameter in a command status event.
#define HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE 0x01U               // Size in bytes of the sub-event code parameter in an HCI LE meta event.

#define HCI_ACL_HEADER_SIZE                 0x02U               // Size in bytes of the header parameters (BC:2, PB: 2, Handle:10) in an HCI ACL packet.
#define HCI_ACL_PKT_PARAMETER_LENGTH_SIZE   0x02U               // Size in bytes of the length parameter in an HCI ACL packet. */


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Type definition for a function pointer to a BLE log function.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet data.
 * @param logLength     The length of the log packet.
 */
typedef void (*_BLE_LOG_FunTbl_T)(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLentgh);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
static void ble_log_PackDisconnect(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadAuthenticatedPayloadTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackWriteAuthenticatedPayloadTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadBdAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadRssi(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetRandomAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetAdvParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetScanRspData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetScanParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetScanEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackCreateConn(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackCreateConnCancel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackClearFilterAcceptList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAddDevToFilterAcceptList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackConnectionUpdate(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetHostChannelClassification(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadChannelMap(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadRemoteFeature(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackEnableEncryption(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackLtkReqReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackLtkReqNegReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackEndTest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackRemoteConnParamReqReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackRemoteConnParamReqNegReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetDataLength(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAddDevToResolvingList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackClearResolvingList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetAddrResolutionEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_packSetRpaTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetDefaultPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReceiverTestV2(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetAdvSetRandomAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtAdvParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtScanRspData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackRemoveAdvSet(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackClearAdvSet(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPeriodicAdvParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPeriodicAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPeriodicAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtScanParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetExtScanEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackExtCreateConn(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvCreateSync(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvCreateSyncCancel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvTerminateSync(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAddDevToPeriodicAdvList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackClearPeriodicAdvList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadTransmitPower(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPrivacyMode(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPeriodicAdvReceiveEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackEnhancedReadTransmitPowerLevel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadRemoteTransmitPowerLevel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetTransmitPowerReportingEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackTransmitterTestV4(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPathLossParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetPathLossEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetHostFeature(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSetDefaultSubrate(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSubrateRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);



static void ble_log_PackDiscComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackEncryptionChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadRemoteVersionInfomationComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackNumberOfCompletedPacket(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackKeyRefreshComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackConnectComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackConnectionUpdateComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackReadRemoteFeaturesComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackLtkRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackRemoteConnectionUpdateRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackDataLengthChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackDirectAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPhyUpdateComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackExtAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvSyncEstablished(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPeriodicAdvSyncLost(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackScanTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAdvSetTerminated(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackScanRequestReceived(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackChannelSelectAlgorithm(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackTransmitPowerReporting(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackPathLossThreshold(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackSubrateChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);
static void ble_log_PackAuthenticatedPayloadTimeoutExpired(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength);

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************
static BLE_LOG_EventCb_T                 s_logEventCb = NULL;

static const _BLE_LOG_FunTbl_T  s_hciCmdFun[] =
{
    NULL,   // (0x00)
    NULL,   // (0x01)
    NULL,   // (0x02)
    NULL,   // (0x03)
    NULL,   // (0x04)
    ble_log_PackSetRandomAddr,                          // HCI_LE_SET_RANDOM_ADDRESS (0x05)
    ble_log_PackSetAdvParams,                           // HCI_LE_SET_ADVERTISING_PARAMS (0x06)
    NULL,   // (0x07)
    ble_log_PackSetAdvData,                             // HCI_LE_SET_ADVERTISING_DATA (0x08)
    ble_log_PackSetScanRspData,                         // HCI_LE_SET_SCAN_RESPONSE_DATA (0x09)
    ble_log_PackSetAdvEnable,                           // HCI_LE_SET_ADVERTISING_ENABLE (0x0A)
    ble_log_PackSetScanParams,                          // HCI_LE_SET_SCAN_PARAMS (0x0B)
    ble_log_PackSetScanEnable,                          // HCI_LE_SET_SCAN_ENABLE (0x0C)
    ble_log_PackCreateConn,                             // HCI_LE_CREATE_CONNECTION (0x0D)
    ble_log_PackCreateConnCancel,                       // HCI_LE_CREATE_CONNECTION_CANCEL (0x0E)
    NULL,   // (0x0F)
    ble_log_PackClearFilterAcceptList,                  // HCI_LE_CLEAR_FILTER_ACCEPT_LIST (0x10)
    ble_log_PackAddDevToFilterAcceptList,               // HCI_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST (0x11)
    NULL,   // (0x12)
    ble_log_PackConnectionUpdate,                       // HCI_LE_CONNECTION_UPDATE (0x13)
    ble_log_PackSetHostChannelClassification,           // HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION (0x14)
    ble_log_PackReadChannelMap,                         // HCI_LE_READ_CHANNEL_MAP (0x15)
    ble_log_PackReadRemoteFeature,                      // HCI_LE_READ_REMOTE_FEATURES (0X16)
    NULL,   // (0x17)
    NULL,   // (0x18)
    ble_log_PackEnableEncryption,                       // HCI_LE_ENABLE_ENCRYPTION (0x19)
    ble_log_PackLtkReqReply,                            // HCI_LE_LONG_TERM_KEY_REQ_REPLY (0x1A)
    ble_log_PackLtkReqNegReply,                         // HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY (0x1B)
    0x00,   // (0x1C)
    NULL,   // (0x1D)
    NULL,   // (0x1E)
    ble_log_PackEndTest,                                // HCI_LE_TEST_END (0x1F)
    ble_log_PackRemoteConnParamReqReply,                // HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY (0x20)
    ble_log_PackRemoteConnParamReqNegReply,             // HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY (0x21)
    ble_log_PackSetDataLength,                          // HCI_LE_SET_DATA_LENGTH (0x22)
    NULL,   // (0x23)
    NULL,   // (0x24)
    NULL,   // (0x25)
    NULL,   // (0x26)
    ble_log_PackAddDevToResolvingList,                  // HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST (0x27)
    NULL,   // (0x28)
    ble_log_PackClearResolvingList,                     // HCI_LE_CLEAR_RESOLVING_LIST (0x29)
    NULL,   // (0x2A)
    NULL,   // (0x2B)
    NULL,   // (0x2C)
    ble_log_PackSetAddrResolutionEnable,                // HCI_LE_SET_ADDR_RESOLUTION_ENABLE (0x2D)
    ble_log_packSetRpaTimeout,                          // HCI_LE_SET_RPA_TIMEOUT (0x2E)
    NULL,   // (0x2F)
    ble_log_PackReadPhy,                                // HCI_LE_READ_PHY (0x30)
    ble_log_PackSetDefaultPhy,                          // HCI_LE_SET_DEFAULT_PHY (0x31)
    ble_log_PackSetPhy,                                 // HCI_LE_SET_PHY (0x32)
    ble_log_PackReceiverTestV2,                         // HCI_LE_RECEIVER_TEST_V1 (0x33)
    NULL,   // (0x34)
    ble_log_PackSetAdvSetRandomAddr,                    // HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDR (0x35)
    ble_log_PackSetExtAdvParam,                         // HCI_LE_SET_EXT_ADV_PARAM (0x36)
    ble_log_PackSetExtAdvData,                          // HCI_LE_SET_EXT_ADV_DATA (0x37)
    ble_log_PackSetExtScanRspData,                      // HCI_LE_SET_EXTENDED_SCAN_RSP_DATA (0x38)
    ble_log_PackSetExtAdvEnable,                        // HCI_LE_SET_EXT_ADV_ENABLE (0x39)
    NULL,   // (0x3A)
    NULL,   // (0x3B)
    ble_log_PackRemoveAdvSet,                           // HCI_LE_REMOVE_ADVERTISING_SET (0x3C)
    ble_log_PackClearAdvSet,                            // HCI_LE_CLEAR_ADVERTISING_SETS (0x3D)
    ble_log_PackSetPeriodicAdvParam,                    // HCI_LE_SET_PA_PARAM (0x3E)
    ble_log_PackSetPeriodicAdvData,                     // HCI_LE_SET_PA_DATA (0x3F)
    ble_log_PackSetPeriodicAdvEnable,                   // HCI_LE_SET_PA_ENABLE (0x40)
    ble_log_PackSetExtScanParam,                        // HCI_LE_SET_EXTENDED_SCAN_PARAM (0x41)
    ble_log_PackSetExtScanEnable,                       // HCI_LE_SET_EXTENDED_SCAN_ENABLE (0x42)
    ble_log_PackExtCreateConn,                          // HCI_LE_EXTENDED_CREATE_CONNECTION (0x43)
    ble_log_PackPeriodicAdvCreateSync,                  // HCI_LE_PA_CREATE_SYNC (0x44)
    ble_log_PackPeriodicAdvCreateSyncCancel,            // HCI_LE_PA_CREATE_SYNC_CANCEL (0x45)
    ble_log_PackPeriodicAdvTerminateSync,               // HCI_LE_PA_TERMINATE_SYNC (0x46)
    ble_log_PackAddDevToPeriodicAdvList,                // HCI_LE_ADD_DEV_TO_PERIODIC_ADVERTISER_LIST (0x47)
    NULL,   // (0x48)
    ble_log_PackClearPeriodicAdvList,                   // HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST (0x49)
    NULL,   // (0x4A)
    ble_log_PackReadTransmitPower,                      // HCI_LE_READ_TRANSMIT_POWER (0x4B)
    NULL,   // (0x4C)
    NULL,   // (0x4D)
    ble_log_PackSetPrivacyMode,                         // HCI_LE_SET_PRIVACY_MODE (0x4E)
    NULL,   // (0x4F)
    NULL,   // (0x50)
    NULL,   // (0x51)
    NULL,   // (0x52)
    NULL,   // (0x53)
    NULL,   // (0x54)
    NULL,   // (0x55)
    NULL,   // (0x56)
    NULL,   // (0x57)
    NULL,   // (0x58)
    ble_log_PackSetPeriodicAdvReceiveEnable,            // HCI_LE_SET_PA_RECEIVE_ENABLE (0x59)
    NULL,   // (0x5A)
    NULL,   // (0x5B)
    NULL,   // (0x5C)
    NULL,   // (0x5D)
    NULL,   // (0x5E)
    NULL,   // (0x5F)
    NULL,   // (0x60)
    NULL,   // (0x61)
    NULL,   // (0x62)
    NULL,   // (0x63)
    NULL,   // (0x64)
    NULL,   // (0x65)
    NULL,   // (0x66)
    NULL,   // (0x67)
    NULL,   // (0x68)
    NULL,   // (0x69)
    NULL,   // (0x6A)
    NULL,   // (0x6B)
    NULL,   // (0x6C)
    NULL,   // (0x6D)
    NULL,   // (0x6E)
    NULL,   // (0x6F)
    NULL,   // (0x70)
    NULL,   // (0x71)
    NULL,   // (0x72)
    NULL,   // (0x73)
    ble_log_PackSetHostFeature,                         // HCI_LE_SET_HOST_FEATURE (0x74)
    NULL,   // (0x75)
    ble_log_PackEnhancedReadTransmitPowerLevel,         // HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL (0x76)
    ble_log_PackReadRemoteTransmitPowerLevel,           // HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL (0x77)
    ble_log_PackSetPathLossParams,                      // HCI_LE_SET_PLR_PARAMS (0x78)
    ble_log_PackSetPathLossEnable,                      // HCI_LE_SET_PLR_ENABLE (0x79)
    ble_log_PackSetTransmitPowerReportingEnable,        // HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE (0x7A)
    ble_log_PackTransmitterTestV4,                      // HCI_LE_TRANSMITTER_TEST_V4 (0x7B)
    NULL,   // (0x7C)
    ble_log_PackSetDefaultSubrate,                      // HCI_LE_SET_DEFAULT_SUBRATE (0x7D)
    ble_log_PackSubrateRequest                          // HCI_LE_SUBRATE_REQUEST (0x7E)
};

static const _BLE_LOG_FunTbl_T  s_hciLeMetaEvtFun[] =
{
    NULL,   // Reserved (0x00)
    ble_log_PackConnectComplete,                        // HCI_LE_CONNECT_COMPLETE (0x01)
    ble_log_PackAdvReport,                              // HCI_LE_ADVERTISING_REPORT (0x02)
    ble_log_PackConnectionUpdateComplete,               // HCI_LE_CONNECTION_UPDATE_COMPLETE (0x03)
    ble_log_PackReadRemoteFeaturesComplete,             // HCI_LE_READ_REMOTE_FEATURES_COMPLETE (0x04)
    ble_log_PackLtkRequest,                             // HCI_LE_LONG_TERM_KEY_REQUEST (0x05)
    ble_log_PackRemoteConnectionUpdateRequest,          // HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST (0x06)
    ble_log_PackDataLengthChange,                       // HCI_LE_DATA_LENGTH_CHANGE (0x07)
    NULL,   // (0x08)
    NULL,   // (0x09)
    NULL,   // (0x0A)
    ble_log_PackDirectAdvReport,                        // HCI_LE_DIRECT_ADVERTISING_REPORT (0x0B)
    ble_log_PackPhyUpdateComplete,                      // HCI_LE_PHY_UPDATE_COMPLETE (0x0C)
    ble_log_PackExtAdvReport,                           // HCI_LE_EXTENDED_ADVERTISING_REPORT (0x0D)
    ble_log_PackPeriodicAdvSyncEstablished,             // HCI_LE_PA_SYNC_ESTABLISHED (0x0E)
    ble_log_PackPeriodicAdvReport,                      // HCI_LE_PA_REPORT (0x0F)
    ble_log_PackPeriodicAdvSyncLost,                    // HCI_LE_PA_SYNC_LOST (0x10)
    ble_log_PackScanTimeout,                            // HCI_LE_SCAN_TIMEOUT (0x11)
    ble_log_PackAdvSetTerminated,                       // HCI_LE_ADVERTISING_SET_TERMINATED (0x12)
    ble_log_PackScanRequestReceived,                    // HCI_LE_SCAN_REQUEST_RECEIVED (0x13)
    ble_log_PackChannelSelectAlgorithm,                 // HCI_LE_CHANNEL_SELECTION_ALGORITHM (0x14)
    NULL,   // (0x15)
    NULL,   // (0x16)
    NULL,   // (0x17)
    NULL,   // (0x18)
    NULL,   // (0x19)
    NULL,   // (0x1A)
    NULL,   // (0x1B)
    NULL,   // (0x1C)
    NULL,   // (0x1D)
    NULL,   // (0x1E)
    NULL,   // (0x1F)
    ble_log_PackPathLossThreshold,                      // HCI_LE_PATH_LOSS_THRESHOLD (0x20)
    ble_log_PackTransmitPowerReporting,                 // HCI_LE_TRANSMIT_POWER_REPORTING (0x21)
    NULL,   // (0x22)
    NULL,   // (0x23)
    NULL,   // (0x24)
    NULL,   // (0x25)
    NULL,   // (0x26)
    NULL,   // (0x27)
    NULL,   // (0x28)
    NULL,   // (0x29)
    NULL,   // (0x2A)
    NULL,   // (0x2B)
    NULL,   // (0x2C)
    NULL,   // (0x2D)
    NULL,   // (0x2E)
    NULL,   // (0x2F)
    NULL,   // (0x30)
    NULL,   // (0x31)
    NULL,   // (0x32)
    NULL,   // (0x33)
    NULL,   // (0x34)
    NULL,   // (0x35)
    NULL,   // (0x36)
    NULL,   // (0x37)
    NULL,   // (0x38)
    NULL,   // (0x39)
    NULL,   // (0x3A)
    NULL,   // (0x3B)
    NULL,   // (0x3C)
    NULL,   // (0x3D)
    ble_log_PackSubrateChange                           // HCI_LE_SUBRATE_CHANGE (0x3E)
};

static const uint8_t s_hciCmdPktSize[] =
{
    0x00,   // (0x00)
    0x00,   // (0x01)
    0x00,   // (0x02)
    0x00,   // (0x03)
    0x00,   // (0x04)
    HCI_CMD_SIZE_SET_RANDOM_ADDR,                    // HCI_LE_SET_RANDOM_ADDRESS (0x05)
    HCI_CMD_SIZE_SET_ADV_PARAMS,                     // HCI_LE_SET_ADVERTISING_PARAMS (0x06)
    0x00,   // (0x07)
    HCI_CMD_SIZE_SET_ADV_DATA,                       // HCI_LE_SET_ADVERTISING_DATA (0x08)
    HCI_CMD_SIZE_SET_SCAN_RSP_DATA,                  // HCI_LE_SET_SCAN_RESPONSE_DATA (0x09)
    HCI_CMD_SIZE_SET_ADV_ENABLE,                     // HCI_LE_SET_ADVERTISING_ENABLE (0x0A)
    HCI_CMD_SIZE_SET_SCAN_PARAMS,                    // HCI_LE_SET_SCAN_PARAMS (0x0B)
    HCI_CMD_SIZE_SET_SCAN_ENABLE,                    // HCI_LE_SET_SCAN_ENABLE (0x0C)
    HCI_CMD_SIZE_CREATE_CONN,                        // HCI_LE_CREATE_CONNECTION (0x0D)
    HCI_CMD_SIZE_CREATE_CONN_CANCEL,                 // HCI_LE_CREATE_CONNECTION_CANCEL (0x0E)
    0x00, // (0x0F)
    HCI_CMD_SIZE_CLEAR_FILTER_ACCEPT_LIST,           // HCI_LE_CLEAR_FILTER_ACCEPT_LIST (0x10)
    HCI_CMD_SIZE_ADD_DEV_TO_FILTER_ACCEPT_LIST,      // HCI_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST (0x11)
    0x00, // (0x12)
    HCI_CMD_SIZE_CONN_UPDATE,                        // HCI_LE_CONNECTION_UPDATE (0x13)
    HCI_CMD_SIZE_SET_HOST_CHANNEL_CLASSIFY,          // HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION (0x14)
    HCI_CMD_SIZE_READ_CHANNEL_MAP,                   // HCI_LE_READ_CHANNEL_MAP (0x15)
    HCI_CMD_SIZE_READ_REMOTE_FEATURES,               // HCI_LE_READ_REMOTE_FEATURES (0X16)
    0x00, // (0x17)
    0x00, // (0x18)
    HCI_CMD_SIZE_ENABLE_ENCRYPTION,                  // HCI_LE_ENABLE_ENCRYPTION (0x19)
    HCI_CMD_SIZE_LTK_REQ_REPLY,                      // HCI_LE_LONG_TERM_KEY_REQ_REPLY (0x1A)
    HCI_CMD_SIZE_LTK_REQ_NEG_REPLY,                  // HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY (0x1B)
    0x00, // (0x1C)
    HCI_CMD_SIZE_RECEIVER_TEST_V1,                   // HCI_LE_RECEIVER_TEST_V1 (0x1D)
    HCI_CMD_SIZE_TX_TEST_V1,                         // HCI_LE_TRANSMITTER_TEST_V1 (0x1E)
    HCI_CMD_SIZE_TEST_END,                           // HCI_LE_TEST_END (0x1F)
    HCI_CMD_SIZE_REMOTE_CP_REQ_REPLY,                // HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY (0x20)
    HCI_CMD_SIZE_REMOTE_CP_REQ_NEG_REPLY,            // HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY (0x21)
    HCI_CMD_SIZE_SET_DATA_LENGTH,                    // HCI_LE_SET_DATA_LENGTH (0x22)
    0x00, // (0x23)
    0x00, // (0x24)
    0x00, // (0x25)
    0x00, // (0x26)
    HCI_CMD_SIZE_ADD_DEV_TO_RES_LIST,                // HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST (0x27)
    0x00, // (0x28)
    HCI_CMD_SIZE_CLEAR_RES_LIST,                     // HCI_LE_CLEAR_RESOLVING_LIST (0x29)
    0x00, // (0x2A)
    0x00, // (0x2B)
    0x00, // (0x2C)
    HCI_CMD_SIZE_SET_ADDR_RES_ENABLE,                // HCI_LE_SET_ADDR_RESOLUTION_ENABLE (0x2D)
    HCI_CMD_SIZE_SET_RPA_TIMEOUT,                    // HCI_LE_SET_RPA_TIMEOUT (0x2E)
    0x00, // (0x2F)
    HCI_CMD_SIZE_READ_PHY,                           // HCI_LE_READ_PHY (0x30)
    HCI_CMD_SIZE_SET_DEFAULT_PHY,                    // HCI_LE_SET_DEFAULT_PHY (0x31)
    HCI_CMD_SIZE_SET_PHY,                            // HCI_LE_SET_PHY (0x32)
    HCI_CMD_SIZE_RECEIVER_TEST_V2,                   // HCI_LE_RECEIVER_TEST_V1 (0x33)
    0x00, // (0x34)
    HCI_CMD_SIZE_SET_ADV_SET_RANDOM_ADDR,            // HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDR (0x35)
    HCI_CMD_SIZE_SET_EXT_ADV_PARAMS,                 // HCI_LE_SET_EXT_ADV_PARAM (0x36)
    HCI_CMD_SIZE_SET_EXT_ADV_DATA,                   // HCI_LE_SET_EXT_ADV_DATA (0x37)
    HCI_CMD_SIZE_SET_EXT_SCAN_RSP_DATA,              // HCI_LE_SET_EXTENDED_SCAN_RSP_DATA (0x38)
    HCI_CMD_SIZE_SET_EXT_ADV_ENABLE,                 // HCI_LE_SET_EXT_ADV_ENABLE (0x39)
    0x00, // (0x3A)
    0x00, // (0x3B)
    HCI_CMD_SIZE_REMOVE_ADV_SET,                     // HCI_LE_REMOVE_ADVERTISING_SET (0x3C)
    HCI_CMD_SIZE_CLEAR_ADV_SET,                      // HCI_LE_CLEAR_ADVERTISING_SETS (0x3D)
    HCI_CMD_SIZE_SET_PERIODIC_ADV_PARAMS,            // HCI_LE_SET_PA_PARAM (0x3E)
    HCI_CMD_SIZE_SET_PERIODIC_ADV_DATA,              // HCI_LE_SET_PA_DATA (0x3F)
    HCI_CMD_SIZE_SET_PERIODIC_ADV_ENABLE,            // HCI_LE_SET_PA_ENABLE (0x40)
    HCI_CMD_SIZE_SET_EXT_SCAN_PARAMS,                // HCI_LE_SET_EXTENDED_SCAN_PARAM (0x41)
    HCI_CMD_SIZE_SET_EXT_SCAN_ENABLE,                // HCI_LE_SET_EXTENDED_SCAN_ENABLE (0x42)
    HCI_CMD_SIZE_EXT_CREATE_CONN,                    // HCI_LE_EXTENDED_CREATE_CONNECTION (0x43)
    HCI_CMD_SIZE_PA_CREATE_SYNC,                     // HCI_LE_PA_CREATE_SYNC (0x44)
    HCI_CMD_SIZE_PA_CREATE_SYNC_CANCEL,              // HCI_LE_PA_CREATE_SYNC_CANCEL (0x45)
    HCI_CMD_SIZE_PA_TERMINATE_SYNC,                  // HCI_LE_PA_TERMINATE_SYNC (0x46)
    HCI_CMD_SIZE_ADD_DEV_TO_PERIODIC_ADV_LIST,       // HCI_LE_ADD_DEV_TO_PERIODIC_ADVERTISER_LIST (0x47)
    0x00, // (0x48)
    HCI_CMD_SIZE_CLEAR_PERIODIC_ADV_LIST,            // HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST (0x49)
    0x00, // (0x4A)
    HCI_CMD_SIZE_READ_TRANSMIT_POWER,                // HCI_LE_READ_TRANSMIT_POWER (0x4B)
    0x00, // (0x4C)
    0x00, // (0x4D)
    HCI_CMD_SIZE_SET_PRIVACY_MODE,                   // HCI_LE_SET_PRIVACY_MODE (0x4E)
    0x00, // (0x4F)
    0x00, // (0x50)
    0x00, // (0x51)
    0x00, // (0x52)
    0x00, // (0x53)
    0x00, // (0x54)
    0x00, // (0x55)
    0x00, // (0x56)
    0x00, // (0x57)
    0x00, // (0x58)
    HCI_CMD_SIZE_SET_PERIODIC_ADV_RX_ENABLE,         // HCI_LE_SET_PA_RECEIVE_ENABLE (0x59)
    0x00, // (0x5A)
    0x00, // (0x5B)
    0x00, // (0x5C)
    0x00, // (0x5D)
    0x00, // (0x5E)
    0x00, // (0x5F)
    0x00, // (0x60)
    0x00, // (0x61)
    0x00, // (0x62)
    0x00, // (0x63)
    0x00, // (0x64)
    0x00, // (0x65)
    0x00, // (0x66)
    0x00, // (0x67)
    0x00, // (0x68)
    0x00, // (0x69)
    0x00, // (0x6A)
    0x00, // (0x6B)
    0x00, // (0x6C)
    0x00, // (0x6D)
    0x00, // (0x6E)
    0x00, // (0x6F)
    0x00, // (0x70)
    0x00, // (0x71)
    0x00, // (0x72)
    0x00, // (0x73)
    HCI_CMD_SIZE_SET_HOST_FEATURE,                   // HCI_LE_SET_HOST_FEATURE (0x74)
    0x00, // (0x75)
    HCI_CMD_SIZE_ENHANCED_READ_TRANSMIT_POWER_LEVEL, // HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL (0x76)
    HCI_CMD_SIZE_READ_REMOTE_TRANSMIT_POWER_LEVEL,   // HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL (0x77)
    HCI_CMD_SIZE_SET_PATH_LOSS_PARAMS,               // HCI_LE_SET_PLR_PARAMS (0x78)
    HCI_CMD_SIZE_SET_PATH_LOSS_ENABLE,               // HCI_LE_SET_PLR_ENABLE (0x79)
    HCI_CMD_SIZE_SET_TRANSMIT_POWER_REPORTING_ENABLE,// HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE (0x7A)
    HCI_CMD_SIZE_TX_TEST_V4,                         // HCI_LE_TRANSMITTER_TEST_V4 (0x7B)
    0x00, // (0x7C)
    HCI_CMD_SIZE_SET_DEFAULT_SUBRATE,                // HCI_LE_SET_DEFAULT_SUBRATE (0X7D)
    HCI_CMD_SIZE_SUBRATE_REQUEST                     // HCI_LE_SUBRATE_REQUEST (0X7E)

};

static const uint8_t s_hciLeMetaEvtSize[] =
{
    0x00,   // Reserved (0x00)
    HCI_EVT_SIZE_CONNECT_COMPLETE,                   // HCI_LE_CONNECT_COMPLETE (0x01)
    HCI_EVT_SIZE_ADV_REPORT,                         // HCI_LE_ADVERTISING_REPORT (0x02)
    HCI_EVT_SIZE_CONNECTION_UPDATE_COMPLETE,         // HCI_LE_CONNECTION_UPDATE_COMPLETE (0x03)
    HCI_EVT_SIZE_READ_REMOTE_FEATURES_COMPLETE,      // HCI_LE_READ_REMOTE_FEATURES_COMPLETE (0x04)
    HCI_EVT_SIZE_LONG_TERM_KEY_REQUEST,              // HCI_LE_LONG_TERM_KEY_REQUEST (0x05)
    HCI_EVT_SIZE_REMOTE_CONNECTION_UPDATE_REQUEST,   // HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST (0x06)
    HCI_EVT_SIZE_DATA_LENGTH_CHANGE,                 // HCI_LE_DATA_LENGTH_CHANGE (0x07)
    0x00,   // (0x08)
    0x00,   // (0x09)
    0x00,   // (0x0A)
    HCI_EVT_SIZE_DIRECT_ADV_REPORT,                  // HCI_LE_DIRECT_ADVERTISING_REPORT (0x0B)
    HCI_EVT_SIZE_PHY_UPDATE_COMPLETE,                // HCI_LE_PHY_UPDATE_COMPLETE (0x0C)
    HCI_EVT_SIZE_EXTENDED_ADV_REPORT,                // HCI_LE_EXTENDED_ADVERTISING_REPORT (0x0D)
    HCI_EVT_SIZE_PA_SYNC_ESTABLISHED,                // HCI_LE_PA_SYNC_ESTABLISHED (0x0E)
    HCI_EVT_SIZE_PA_REPORT,                          // HCI_LE_PA_REPORT (0x0F)
    HCI_EVT_SIZE_PA_SYNC_LOST,                       // HCI_LE_PA_SYNC_LOST (0x10)
    HCI_EVT_SIZE_SCAN_TIMEOUT,                       // HCI_LE_SCAN_TIMEOUT (0x11)
    HCI_EVT_SIZE_ADV_SET_TERMINATED,                 // HCI_LE_ADVERTISING_SET_TERMINATED (0x12)
    HCI_EVT_SIZE_SCAN_REQUEST_RECEIVED,              // HCI_LE_SCAN_REQUEST_RECEIVED (0x13)
    HCI_EVT_SIZE_CHANNEL_SELECT_ALGORITHM,           // HCI_LE_CHANNEL_SELECTION_ALGORITHM (0x14)
    0x00,   // (0x15)
    0x00,   // (0x16)
    0x00,   // (0x17)
    0x00,   // (0x18)
    0x00,   // (0x19)
    0x00,   // (0x1A)
    0x00,   // (0x1B)
    0x00,   // (0x1C)
    0x00,   // (0x1D)
    0x00,   // (0x1E)
    0x00,   // (0x1F)
    HCI_EVT_SIZE_PATH_LOSS_THRESHOLD,               // HCI_LE_PATH_LOSS_THRESHOLD (0x20)
    HCI_EVT_SIZE_TX_POWER_REPORTING,                // HCI_LE_TRANSMIT_POWER_REPORTING (0x21)
    0x00,   // (0x22)
    0x00,   // (0x23)
    0x00,   // (0x24)
    0x00,   // (0x25)
    0x00,   // (0x26)
    0x00,   // (0x27)
    0x00,   // (0x28)
    0x00,   // (0x29)
    0x00,   // (0x2A)
    0x00,   // (0x2B)
    0x00,   // (0x2C)
    0x00,   // (0x2D)
    0x00,   // (0x2E)
    0x00,   // (0x2F)
    0x00,   // (0x30)
    0x00,   // (0x31)
    0x00,   // (0x32)
    0x00,   // (0x33)
    0x00,   // (0x34)
    0x00,   // (0x35)
    0x00,   // (0x36)
    0x00,   // (0x37)
    0x00,   // (0x38)
    0x00,   // (0x39)
    0x00,   // (0x3A)
    0x00,   // (0x3B)
    0x00,   // (0x3C)
    0x00,   // (0x3D)
    HCI_EVT_SIZE_SUBRATE_CHANGE                     // HCI_LE_SUBRATE_CHANGE (0x3E)
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Prints a BLE log packet if a log event callback is set.
 *
 * @param logType       The type of log to print.
 * @param logLength     The length of the log packet.
 * @param p_logPacket   Pointer to the log packet data.
 */
static void ble_log_Print(uint8_t logType, uint16_t logLength, uint8_t *p_logPacket)
{
    if (s_logEventCb != NULL)
    {
        s_logEventCb(logType, logLength, p_logPacket);
    }
}


/**
 * @brief Generates an HCI command complete event and prints it using ble_log_Print.
 *
 * @param logId         The ID of the log, which contains the OGF and OCF.
 * @param result        The result to be included in the command complete event.
 * @param paramsLength  The length of the return parameters.
 * @param p_returnParams Pointer to the return parameters data.
 */
static void ble_log_GenerateHciCommandComplete(uint16_t logId, uint8_t result, uint16_t paramsLength, uint8_t *p_returnParams)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t eventCode = HCI_COMMAND_COMPLETE;
    uint8_t numberOfHciPacket = 1;
    uint16_t ogf;
    uint16_t logLength;
    uint16_t ocf;

    ogf = (logId >> 10U);
    ocf = (logId & 0x03FFU);
    logLength = (HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+
                HCI_EVENT_CC_PARAMETER_NUMBER_SIZE+HCI_EVENT_CC_PARAMETER_OPCODE_SIZE+
                paramsLength);
    p_logPacket = OSAL_Malloc(logLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U8_TO_STREAM(&p_buf, eventCode);
    U8_TO_STREAM(&p_buf, (uint8_t)(logLength-HCI_EVENT_CODE_SIZE-HCI_EVENT_PARAMETER_LENGTH_SIZE));
    U8_TO_STREAM(&p_buf, numberOfHciPacket);
    U16_TO_STREAM_LE(&p_buf, logId);

    /* To do: mapping result code. */
    U8_TO_STREAM(&p_buf, result);

    if (ogf == HCI_LE_CTRL)
    {
        if (ocf == HCI_LE_READ_CHANNEL_MAP)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_CHANNEL_MAP-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_LONG_TERM_KEY_REQ_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_LTK_REQUEST_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_LTK_REQUEST_NEGATIVE_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_TEST_END)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_TEST_END-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_REMOTE_CP_REQ_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_REMOTE_CONN_PARAM_REQ_NEGATIVE_REPLY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_REMOTE_CP_REQ_NEG_REPLY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_DATA_LENGTH)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_DATA_LENGTH-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_READ_PHY)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_PHY-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_EXT_ADV_PARAM)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_EXT_ADV_PARAMS-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_READ_TRANSMIT_POWER)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_TRANSMIT_POWER-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_ENHANCED_READ_TRANSMIT_POWER-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_PLR_PARAMS)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_PLR_PARAMS-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_PLR_ENABLE)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_PLR_ENABLE-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_SET_TRANSMIT_POWER_REPORTING_ENABLE-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else
        {
            //Shall not enter here
        }
    }
    else if (ogf == HCI_HC)
    {
        if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_AUTH_PAYLOAD_TO-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
        {
            VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_WRITE_AUTH_PAYLOAD_TO-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
        }
        else
        {
            //Shall not enter here
        }
    }
    else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
    {
        VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_BD_ADDR-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
    }
    else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
    {
        VARIABLE_COPY_TO_STREAM(&p_buf, (p_returnParams+HCI_EVENT_CC_PARAMETER_STATUS_SIZE), (HCI_CC_EVT_SIZE_READ_RSSI-HCI_EVENT_CC_PARAMETER_STATUS_SIZE));
    }
    else
    {
        //Shall not enter here
    }

    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
    OSAL_Free(p_logPacket);
}


/**
 * @brief Generates an HCI command status event and prints it using ble_log_Print.
 *
 * @param logId     The ID of the log, which contains the OGF and OCF.
 * @param result    The result to be included in the command status event.
 */
static void ble_log_GenerateHciCommandStatus(uint16_t logId, uint8_t result)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t eventCode = HCI_COMMAND_STATUS;
    uint8_t numberOfHciPacket = 1;
    uint16_t logLength;

    logLength = (HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+
                HCI_EVENT_CS_PARAMETER_STATUS_SIZE+HCI_EVENT_CS_PARAMETER_NUMBER_SIZE+HCI_EVENT_CS_PARAMETER_OPCODE_SIZE);
    p_logPacket = OSAL_Malloc(logLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U8_TO_STREAM(&p_buf, eventCode);
    U8_TO_STREAM(&p_buf, ((uint8_t)logLength-(uint8_t)HCI_EVENT_CODE_SIZE-(uint8_t)HCI_EVENT_PARAMETER_LENGTH_SIZE));

    /* To do: mapping result code. */
    U8_TO_STREAM(&p_buf, result);

    U8_TO_STREAM(&p_buf, numberOfHciPacket);
    U16_TO_STREAM_LE(&p_buf, logId);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
    OSAL_Free(p_logPacket);
}


/**
 * @brief Packs the disconnect log event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackDisconnect(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_Disconnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_Disconnect_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the read authenticated payload timeout event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadAuthenticatedPayloadTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_ReadAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the write authenticated payload timeout event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackWriteAuthenticatedPayloadTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_WriteAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_WriteAuthenticatedPayloadTimeout_T *)p_log->p_logPayload)->authenticatedPayloadTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the read BD_ADDR event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadBdAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the read RSSI event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadRssi(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_ReadRssi_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the set random address event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetRandomAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetRandomAddress_T *)p_log->p_logPayload)->randomAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the set advertising parameters event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetAdvParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->intervalMax);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->type);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->advChannelMap);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingParameters_T *)p_log->p_logPayload)->filterPolicy);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}

/**
 * @brief Packs the set advertising data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingData_T *)p_log->p_logPayload)->advData, HCI_MAX_ADV_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the set scan response data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetScanRspData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanResponseData_T *)p_log->p_logPayload)->advScanRspLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetScanResponseData_T *)p_log->p_logPayload)->advScanRspData, HCI_MAX_ADV_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the set advertising enable command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, *p_log->p_logPayload);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**@brief Packs the set scan parameters command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetScanParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanType);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->scanWindow);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanParameters_T *)p_log->p_logPayload)->filterPolicy);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the set scan enable command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetScanEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetScanEnable_T *)p_log->p_logPayload)->filterDuplicates);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the create connection command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackCreateConn(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->scanInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->scanWindow);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->filterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->ownAddrType);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_CreateConnection_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the create connection cancel command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackCreateConnCancel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the clear filter accept list command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackClearFilterAcceptList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the add device to filter accept list command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackAddDevToFilterAcceptList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToFilterAcceptList_T *)p_log->p_logPayload)->addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToFilterAcceptList_T *)p_log->p_logPayload)->addr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the connection update command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackConnectionUpdate(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ConnectionUpdate_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the Set Host Channel Classification command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackSetHostChannelClassification(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    VARIABLE_COPY_TO_STREAM(&p_buf, (uint8_t *)p_log->p_logPayload, HCI_CMD_SIZE_SET_HOST_CHANNEL_CLASSIFY);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Read Channel Map command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackReadChannelMap(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadChannelMap_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Read Remote Feature command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackReadRemoteFeature(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadRemoteFeatures_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the Enable Encryption command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackEnableEncryption(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->randomNum, HCI_LTK_RANDOM_SIZE);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->encryptedDiversifier);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_EnableEncryption_T *)p_log->p_logPayload)->longTermKey, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the Long Term Key Request Reply command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackLtkReqReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_LongTermKeyRequestReply_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_LongTermKeyRequestReply_T *)p_log->p_logPayload)->longTermKey, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Long Term Key Request Negative Reply command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackLtkReqNegReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_LongTermKeyRequestNegativeReply_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the End Test command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackEndTest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Remote Connection Parameter Request Reply command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackRemoteConnParamReqReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->supervisionTimeout);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->minCeLength);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqReply_T *)p_log->p_logPayload)->maxCeLength);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Remote Connection Parameter Request Negative Reply command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackRemoteConnParamReqNegReply(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_RemoteConnParamReqNegReply_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_RemoteConnParamReqNegReply_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Data Length command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackSetDataLength(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->txOctets);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDataLength_T *)p_log->p_logPayload)->txTime);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Add Device To Resolving List command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackAddDevToResolvingList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIdAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIdAddr, HCI_BD_ADDRESS_LEN);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->peerIrk, HCI_MAX_KEY_LENGTH);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDeviceToResolvingList_T *)p_log->p_logPayload)->localIrk, HCI_MAX_KEY_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Clear Resolving List command data into the log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log data.
 */
static void ble_log_PackClearResolvingList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Address Resolution Enable command into the log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetAddrResolutionEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAddressResolutionEnable_T *)p_log->p_logPayload)->addressResolutionEnable);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set RPA Timeout command into the log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_packSetRpaTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetRpaTimeout_T *)p_log->p_logPayload)->rpaTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Read PHY command into the log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadPhy_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Default PHY command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetDefaultPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->allPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->txPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetDefaultPhy_T *)p_log->p_logPayload)->rxPhys);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set PHY command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPhy(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->allPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->txPhys);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->rxPhys);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPhy_T *)p_log->p_logPayload)->phyOptions);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the Receiver Test V2 command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReceiverTestV2(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->rxChannel);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->phy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReceiverTestV2_T *)p_log->p_logPayload)->modulationIndex);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Advertising Set Random Address command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetAdvSetRandomAddr(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingSetRandomAddr_T *)p_log->p_logPayload)->advHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetAdvertisingSetRandomAddr_T *)p_log->p_logPayload)->advRandomAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Extended Advertising Parameters command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtAdvParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->advhandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->evtProperies);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priIntervalMin, 3);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priIntervalMax, 3);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priChannelMap);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->filterPolicy);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->txPower);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priPhy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->secMaxSkip);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->secPhy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->scanReqNotifiEnable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->priPhyOptions);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingParameters_T *)p_log->p_logPayload)->secPhyOptions);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Extended Advertising Data command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+1U+((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen;
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->fragPreference);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advData, ((HCI_LE_SetExtAdvertisingData_T *)p_log->p_logPayload)->advLen);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the extended scan response data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtScanRspData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+1U+((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen;
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->fragPreference);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspData, ((HCI_LE_SetExtScanRespData_T *)p_log->p_logPayload)->scanRspLen);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the extended advertising enable parameters into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength, i;

    /* Correct actual length */
    cmdParamLength = 1U+1U+4U*((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets;
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets);
    for (i=0; i< ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->numOfSets; i++)
    {
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].advHandle);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].duration);
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtAdvertisingEnable_T *)p_log->p_logPayload)->advSet[i].maxExtAdvEvts);
    }
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the remove advertising set command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackRemoveAdvSet(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_RemoveAdvertisingSet_T *)p_log->p_logPayload)->advHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the clear advertising sets command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackClearAdvSet(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the periodic advertising parameters into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPeriodicAdvParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->advHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvertisingParams_T *)p_log->p_logPayload)->properties);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the periodic advertising data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPeriodicAdvData(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t cmdParamLength;

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen;
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->operation);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advData, ((HCI_LE_SetPeriodicAdvertisingData_T *)p_log->p_logPayload)->advLen);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the enable or disable command for periodic advertising into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPeriodicAdvEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvertisingEnable_T *)p_log->p_logPayload)->advHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the extended scan parameters into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtScanParam(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t usedPhys, i;
    uint8_t cmdParamLength;

    if (((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_CODED))
    {
        usedPhys = 2;
    }
    else if ((((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == HCI_PHY_OPTION_1M)|| (((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys == HCI_PHY_OPTION_CODED))
    {
        usedPhys = 1;
    }
    else
    {
        usedPhys = 0;
    }

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+(usedPhys*5U);
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanFilterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->scanPhys);

    for (i=0; i<usedPhys; i++)
    {
        U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanType);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanInterval);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanParams_T *)p_log->p_logPayload)->paramsByPhy[i].scanWindow);
    }

    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the enable or disable command for scan into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetExtScanEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->enable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->filterDuplicates);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->duration);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetExtScanEnable_T *)p_log->p_logPayload)->period);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the extended create connection command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackExtCreateConn(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t phys = ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initPhys;
    uint8_t usedPhys, i;
    uint8_t cmdParamLength;

    if (phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_2M|HCI_PHY_OPTION_CODED))
    {
        usedPhys = 3;
    }
    else if ((phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_2M)) || (phys == (HCI_PHY_OPTION_1M|HCI_PHY_OPTION_CODED)) || (phys == (HCI_PHY_OPTION_2M|HCI_PHY_OPTION_CODED)))
    {
        usedPhys = 2;
    }
    else if ((phys == HCI_PHY_OPTION_1M) || (phys == HCI_PHY_OPTION_2M) || (phys == HCI_PHY_OPTION_CODED))
    {
        usedPhys = 1;
    }
    else
    {
        usedPhys = 0;
    }

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+6U+1U+(usedPhys*16U);
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    /* Pack packet */
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initFilterPolicy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->ownAddrType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->initPhys);

    for (i=0; i<usedPhys; i++)
    {
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].scanInterval);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].scanWindow);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].intervalMin);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].intervalMax);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].latency);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].supervisionTimeout);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].minCeLength);
        U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ExtCreateConnParam_T *)p_log->p_logPayload)->paramsByPhy[i].maxCeLength);
    }

    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the periodic advertising create sync command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvCreateSync(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->options);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->skip);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->syncTimeout);
    U8_TO_STREAM(&p_buf, ((HCI_LE_PeriodicAdvertisingCreateSync_T *)p_log->p_logPayload)->syncCteType);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the cancelling the creation of a periodic advertising sync command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvCreateSyncCancel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the terminating a periodic advertising sync command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvTerminateSync(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_PeriodicAdvertisingTerminateSync_T *)p_log->p_logPayload)->syncHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the adding a device to the periodic advertising list command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackAddDevToPeriodicAdvList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_AddDevToPeriodicAdvList_T *)p_log->p_logPayload)->advSid);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the clearing the periodic advertising list command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackClearPeriodicAdvList(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the clearing the reading the transmit power level command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadTransmitPower(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Privacy Mode command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPrivacyMode(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->peerIdentifyAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->peerIdentifyAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPrivacyMode_T *)p_log->p_logPayload)->privacyMode);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Periodic Advertisement Receive Enable command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPeriodicAdvReceiveEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPeriodicAdvReceiveEnable_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPeriodicAdvReceiveEnable_T *)p_log->p_logPayload)->enable);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Host Feature command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetHostFeature(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetHostFeature_T *)p_log->p_logPayload)->bitNumber);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetHostFeature_T *)p_log->p_logPayload)->bitValue);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Enhanced Read Transmit Power Level command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackEnhancedReadTransmitPowerLevel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_EnhancedReadTransmitPowerLevel_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_EnhancedReadTransmitPowerLevel_T *)p_log->p_logPayload)->phy);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Read Remote Transmit Power Level command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadRemoteTransmitPowerLevel(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_ReadRemoteTransmitPowerLevel_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_ReadRemoteTransmitPowerLevel_T *)p_log->p_logPayload)->phy);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Packs the Set Transmit Power Reporting Enable command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetTransmitPowerReportingEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->localEnable);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetTransmitPowerReportingEnable_T *)p_log->p_logPayload)->remoteEnable);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Transmitter Test V4 command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackTransmitterTestV4(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t     *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    uint8_t    cmdParamLength;

    /* Correct actual length */
    cmdParamLength = 1U+1U+1U+1U+1U+1U+1U+((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen+1U;
    logLength = (uint16_t)HCI_COMMAND_OPCODE_SIZE+(uint16_t)HCI_COMMAND_PARAMETER_LENGTH_SIZE+(uint16_t)cmdParamLength;
    *(p_logPacket+HCI_COMMAND_OPCODE_SIZE) = cmdParamLength;

    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->txChannel);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->testDataLen);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->packetPayload);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->phy);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->cteLen);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->cteType);
    U8_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->antennaIds, (uint8_t)((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->switchingPatternLen);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_LE_TransmitterTestV4_T *)p_log->p_logPayload)->txPowerLevel);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Path Loss Reporting Parameters command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPathLossParams(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->highThreshold);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->highHysteresis);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->lowThreshold);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->lowHysteresis);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPathLossReportingParams_T *)p_log->p_logPayload)->minTimeSpent);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Path Loss Enable command into a log packet.
 * 
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetPathLossEnable(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);
    
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetPathLossReportingEnable_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_LE_SetPathLossReportingEnable_T *)p_log->p_logPayload)->reportEnable);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Default Subrate command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSetDefaultSubrate(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDefaultSubrate_T *)p_log->p_logPayload)->subrateMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDefaultSubrate_T *)p_log->p_logPayload)->subrateMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDefaultSubrate_T *)p_log->p_logPayload)->maxLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDefaultSubrate_T *)p_log->p_logPayload)->continuationNum);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SetDefaultSubrate_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandComplete(p_log->logId, *(uint8_t *)p_log->p_returnParams, p_log->paramsLength, p_log->p_returnParams);
}


/**
 * @brief Packs the Set Subrate Request command into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet buffer.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSubrateRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->subrateMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->subrateMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->maxLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->continuationNum);
    U16_TO_STREAM_LE(&p_buf, ((HCI_LE_SubrateRequest_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_COMMAND, logLength, p_logPacket);
    ble_log_GenerateHciCommandStatus(p_log->logId, *(uint8_t *)p_log->p_returnParams);
}


/**
 * @brief Get the size of HCI command parameters based on OGF and OCF.
 *
 * @param ogf Opcode    Group Field of the HCI command.
 * @param ocf Opcode    Command Field of the HCI command.
 * 
 * @retval uint16_t     Size of the HCI command parameters.
 */
static uint16_t ble_log_GetHciCommandParametersSize(uint8_t ogf, uint16_t ocf)
{
    uint16_t cmdParamsSize = 0;

    if (ogf == HCI_LE_CTRL)
    {
        cmdParamsSize = s_hciCmdPktSize[ocf];
    }
    else
    {
        if ((ogf == HCI_LC) && (ocf == HCI_DISCONNECT))
        {
            cmdParamsSize = HCI_CMD_SIZE_DISCONNECT;
        }
        else if (ogf == HCI_HC)
        {
            if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                cmdParamsSize = HCI_CMD_SIZE_READ_AUTH_PAYLOAD_TO;
            }
            else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                cmdParamsSize = HCI_CMD_SIZE_WRITE_AUTH_PAYLOAD_TO;
            }
            else
            {
                //Shall not enter here
            }
        }
        else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
        {
            cmdParamsSize = HCI_CMD_SIZE_READ_BD_ADDR;
        }
        else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
        {
            cmdParamsSize = HCI_CMD_SIZE_READ_RSSI;
        }
        else
        {
            //Shall not enter here
        }
    }

    return cmdParamsSize;
}


/**
 * @brief Generate an HCI command log.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the log packet data.
 * @param logLength     Length of the log packet.
 * @param ogf       Opcode Group Field of the HCI command.
 * @param ocf   Opcode Command Field of the HCI command.
 */
static void ble_log_GenerateHciCommand(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength, uint8_t ogf, uint16_t ocf)
{
    if (ogf == HCI_LE_CTRL)
    {
        if (s_hciCmdFun[ocf] != NULL)
        {
            s_hciCmdFun[ocf](p_log, p_logPacket, logLength);
        }
    }
    else
    {
        if ((ogf == HCI_LC) && (ocf == HCI_DISCONNECT))
        {
            ble_log_PackDisconnect(p_log, p_logPacket, logLength);
        }
        else if (ogf == HCI_HC)
        {
            if (ocf == HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                ble_log_PackReadAuthenticatedPayloadTimeout(p_log, p_logPacket, logLength);
            }
            else if (ocf == HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT)
            {
                ble_log_PackWriteAuthenticatedPayloadTimeout(p_log, p_logPacket, logLength);
            }
            else
            {
                //Shall not enter here
            }
        }
        else if ((ogf == HCI_IP) && (ocf == HCI_READ_BD_ADDR))
        {
            ble_log_PackReadBdAddr(p_log, p_logPacket, logLength);
        }
        else if ((ogf == HCI_SP) && (ocf == HCI_READ_RSSI))
        {
            ble_log_PackReadRssi(p_log, p_logPacket, logLength);
        }
        else
        {
            //Shall not enter here
        }
    }
}

/**
 * @brief Process an HCI command log.
 *
 * @param p_log     Pointer to the log event structure.
 */
static void ble_log_ProcHciCommand(BT_SYS_LogEvent_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t ogf;
    uint16_t cmdParamsSize;
    uint16_t logLength;
    uint16_t ocf;

    /* Retrieve ogf/ocf */
    ogf = (uint8_t)(p_log->logId >> 0X000AU);
    ocf = (p_log->logId & 0x03FFU);

    /* HCI command size */
    cmdParamsSize = ble_log_GetHciCommandParametersSize(ogf, ocf);

    /* Calculate log length */
    logLength = HCI_COMMAND_OPCODE_SIZE+HCI_COMMAND_PARAMETER_LENGTH_SIZE+cmdParamsSize;
    p_logPacket = OSAL_Malloc(logLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, p_log->logId);
    U8_TO_STREAM(&p_buf, (uint8_t)cmdParamsSize);

    /* Generate HCI event log */
    ble_log_GenerateHciCommand(p_log, p_logPacket, logLength, ogf, ocf);
    OSAL_Free(p_logPacket);
}


/**
 * @brief Packs the disconnect complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackDiscComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtDisconnect_T *)p_log->p_logPayload)->reason);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the encryption change event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackEncryptionChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtEncryptChange_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtEncryptChange_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtEncryptChange_T *)p_log->p_logPayload)->encEnable);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the read remote version information complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadRemoteVersionInfomationComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtReadRemoteVer_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtReadRemoteVer_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtReadRemoteVer_T *)p_log->p_logPayload)->version);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtReadRemoteVer_T *)p_log->p_logPayload)->manuName);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtReadRemoteVer_T *)p_log->p_logPayload)->subVersion);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the number of completed packets event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackNumberOfCompletedPacket(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);
    uint8_t i;

    U8_TO_STREAM(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles);
    for (i=0; i<((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles; i++)
    {
        U16_TO_STREAM_LE(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->completePacket[i].connHandle);
        U16_TO_STREAM_LE(&p_buf, ((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->completePacket[i].numPackets);
    }
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the key refresh complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackKeyRefreshComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtKeyRefreshComplete_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtKeyRefreshComplete_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the connect complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackConnectComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->role);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->peerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->peerAddr, HCI_BD_ADDRESS_LEN);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->connLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->supervisionTimeout);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeEnhanceConnect_T *)p_log->p_logPayload)->centralClkAcc);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the advertising report event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->numReports);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advData, ((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the connection update complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackConnectionUpdateComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connInterval);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->connLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeConnParamUpdate_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the read remote features complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackReadRemoteFeaturesComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeReadRemoteFeature_T *)p_log->p_logPayload)->leFeatures, HCI_FEATURES_BITMASK_LENGTH);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Long Term Key Request event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackLtkRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->connHandle);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->randomNum, HCI_LTK_RANDOM_SIZE);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeLongTermKeyReq_T *)p_log->p_logPayload)->encDiver);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Remote Connection Parameter Update Request event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackRemoteConnectionUpdateRequest(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->intervalMin);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->intervalMax);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->latency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeRemoteConnParamsReq_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Data Length Change event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackDataLengthChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxTxOctects);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxTxTime);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxRxOctets);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeDataLengthChange_T *)p_log->p_logPayload)->maxRxTime);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Direct Advertising Report event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackDirectAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->numReports);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].directAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].directAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeDirectAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a PHY Update Complete event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPhyUpdateComplete(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->txPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePhyUpdate_T *)p_log->p_logPayload)->rxPhy);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs an Extended Advertising Report event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackExtAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->numReports);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].eventType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].addrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].addr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].priPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].secPhy);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].sid);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].txPower);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].rssi);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].periodicAdvInterval);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].directAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].directAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advData, ((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Periodic Advertising Sync Established event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvSyncEstablished(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advSid);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advAddr, HCI_BD_ADDRESS_LEN);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advPhy);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->periodicAdvInterval);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvSyncEst_T *)p_log->p_logPayload)->advClkAccuracy);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Periodic Advertising Report event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvReport(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->syncHandle);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->txPower);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->rssi);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->cteType);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->dataStatus);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advData, ((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Periodic Advertising Report event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPeriodicAdvSyncLost(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePeriodicAdvSyncLost_T *)p_log->p_logPayload)->syncHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Scan Timeout event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackScanTimeout(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs an Advertising Set Terminated event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackAdvSetTerminated(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->status);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->advHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeAdvSetTerminated_T *)p_log->p_logPayload)->numComplExtAdvEvents);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Scan Request Received event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackScanRequestReceived(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->advHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->scannerAddrType);
    VARIABLE_COPY_TO_STREAM(&p_buf, ((HCI_EvtLeScanReqReceived_T *)p_log->p_logPayload)->scannerAddr, HCI_BD_ADDRESS_LEN);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs a Channel Selection Algorithm event into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackChannelSelectAlgorithm(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeChannelSelectAlgorithm_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeChannelSelectAlgorithm_T *)p_log->p_logPayload)->channelSelectAlgorithm);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the transmit power reporting data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackTransmitPowerReporting(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->reason);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->phy);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->txPowerLevel);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->txPowerLevelFlag);
    U8_TO_STREAM(&p_buf, (uint8_t)((HCI_EvtLeTxPowerReporting_T *)p_log->p_logPayload)->delta);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the path loss threshold data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackPathLossThreshold(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLePathLossThreshold_T *)p_log->p_logPayload)->connHandle);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePathLossThreshold_T *)p_log->p_logPayload)->currentPathLoss);
    U8_TO_STREAM(&p_buf, ((HCI_EvtLePathLossThreshold_T *)p_log->p_logPayload)->zoneEntered);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}

/**
 * @brief Packs the subrate change into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackSubrateChange(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE+HCI_EVENT_LE_PARAMETER_SUBCODE_SIZE);

    U8_TO_STREAM(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->status);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->connHandle);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->subrateFactor);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->periLatency);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->continuationNum);
    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtLeSubrateChange_T *)p_log->p_logPayload)->supervisionTimeout);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Packs the authenticated payload timeout expired data into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 */
static void ble_log_PackAuthenticatedPayloadTimeoutExpired(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength)
{
    uint8_t *p_buf = (p_logPacket+HCI_EVENT_CODE_SIZE+HCI_EVENT_PARAMETER_LENGTH_SIZE);

    U16_TO_STREAM_LE(&p_buf, ((HCI_EvtAuthPayloadTimeout_T *)p_log->p_logPayload)->connHandle);
    ble_log_Print(BLE_LOG_TYPE_HCI_EVENT, logLength, p_logPacket);
}


/**
 * @brief Gets the size of HCI event parameters based on the event code and sub-event code.
 *
 * @param p_log         Pointer to the log event structure.
 * @param evtCode       Event code to determine the size of parameters.
 * @param subEvtCode    Sub-event code to determine the size of parameters.
 * 
 * @retval Size of the HCI event parameters.
 */
static uint8_t ble_log_GetHciEventParametersSize(BT_SYS_LogEvent_T *p_log, uint8_t evtCode, uint8_t subEvtCode)
{
    uint8_t evtParamsSize = 0;

    if (evtCode == HCI_DISCONNECTION_COMPLETE)
    {
        evtParamsSize = HCI_EVT_SIZE_DISCONNECTION;
    }
    else if (evtCode == HCI_ENCRYPTION_CHANGE)
    {
        evtParamsSize = HCI_EVT_SIZE_ENCRYPTION_CHANGE;
    }
    else if (evtCode == HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE)
    {
        evtParamsSize = HCI_EVT_SIZE_REMOTE_VERSION_INFORMATION_COMPLETE;
    }
    else if (evtCode == HCI_NBR_OF_COMPLETED_PACKETS)
    {
        evtParamsSize = 1U + (((HCI_EvtNumOfCompletePacket_T *)p_log->p_logPayload)->numHandles*(uint8_t)sizeof(HCI_CompletePacket_T));
    }
    else if (evtCode == HCI_ENCRYPTION_KEY_REFRESH_COMPLETE)
    {
        evtParamsSize = HCI_EVT_SIZE_KEY_REFRESH_COMPLETE;
    }
    else if (evtCode == HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED)
    {
        evtParamsSize = HCI_EVT_SIZE_AUTH_PAYLOAD_TO_EXPIRED;
    }
    else
    {
        /* HCI_LE_META_EVENT */
        evtParamsSize = (uint8_t)sizeof(subEvtCode) + s_hciLeMetaEvtSize[subEvtCode];

        /* Adjust event length of specific event */
        if (subEvtCode == HCI_LE_ADVERTISING_REPORT)
        {
            evtParamsSize -= (HCI_MAX_ADV_LENGTH-((HCI_EvtLeAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
        }
        else if (subEvtCode == HCI_LE_EXTENDED_ADVERTISING_REPORT)
        {
            evtParamsSize -= (HCI_EVT_MAX_EXTENDED_ADVERTISING_DATA_LENGTH-((HCI_EvtLeExtAdvReport_T *)p_log->p_logPayload)->reports[0].advLength);
        }
        else if (subEvtCode == HCI_LE_PA_REPORT)
        {
            evtParamsSize -= (HCI_EVT_MAX_PERIODIC_ADVERTISING_DATA_LENGTH-((HCI_EvtLePeriodicAdvReport_T *)p_log->p_logPayload)->advLength);
        }
        else
        {
            //Shall not enter here
        }
    }

    return evtParamsSize;
}


/**
 * @brief Generates an HCI event based on the event code and packs it into a log packet.
 *
 * @param p_log         Pointer to the log event structure.
 * @param p_logPacket   Pointer to the buffer where the log packet will be stored.
 * @param logLength     Length of the log packet.
 * 
 * @param evtCode Event code to determine which event to generate.
 */
static void ble_log_GenerateHciEvent(BT_SYS_LogEvent_T *p_log, uint8_t *p_logPacket, uint16_t logLength, uint8_t evtCode)
{
    if (evtCode == HCI_DISCONNECTION_COMPLETE)
    {
        ble_log_PackDiscComplete(p_log, p_logPacket, logLength);
    }
    else if (evtCode == HCI_ENCRYPTION_CHANGE)
    {
        ble_log_PackEncryptionChange(p_log, p_logPacket, logLength);
    }
    else if (evtCode == HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE)
    {
        ble_log_PackReadRemoteVersionInfomationComplete(p_log, p_logPacket, logLength);
    }
    else if (evtCode == HCI_NBR_OF_COMPLETED_PACKETS)
    {
        ble_log_PackNumberOfCompletedPacket(p_log, p_logPacket, logLength);
    }
    else if (evtCode == HCI_ENCRYPTION_KEY_REFRESH_COMPLETE)
    {
        ble_log_PackKeyRefreshComplete(p_log, p_logPacket, logLength);
    }
    else if (evtCode == HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED)
    {
        ble_log_PackAuthenticatedPayloadTimeoutExpired(p_log, p_logPacket, logLength);
    }
    else
    {
        //Shall not enter here
    }
}

/**
 * @brief Processes HCI events and generates corresponding log packets.
 *
 * @param p_log     Pointer to the log event structure.
 */
static void ble_log_ProcHciEvent(BT_SYS_LogEvent_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint8_t evtParamsSize;
    uint8_t evtCode, subEvtCode = 0;
    uint16_t logLength;

    /* Retrieve event codes */
    evtCode = (uint8_t)p_log->logId;
    subEvtCode = (uint8_t)(p_log->logId>>8);

    /* HCI event size */
    evtParamsSize = ble_log_GetHciEventParametersSize(p_log, evtCode, subEvtCode);

    /* Calculate log length */
    logLength = (uint16_t)HCI_EVENT_CODE_SIZE+(uint16_t)HCI_EVENT_PARAMETER_LENGTH_SIZE+(uint16_t)evtParamsSize;
    p_logPacket = OSAL_Malloc(logLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U8_TO_STREAM(&p_buf, evtCode);
    U8_TO_STREAM(&p_buf, evtParamsSize);

    /* Generate HCI event log */
    if (evtCode == HCI_LE_META_EVENT)
    {
        /* HCI_LE_META_EVENT */
        U8_TO_STREAM(&p_buf, subEvtCode);
        if (s_hciLeMetaEvtFun[subEvtCode] != NULL)
        {
            s_hciLeMetaEvtFun[subEvtCode](p_log, p_logPacket, logLength);
        }
    }
    else
    {
        ble_log_GenerateHciEvent(p_log, p_logPacket, logLength, evtCode);
    }

    OSAL_Free(p_logPacket);
}

/**
 * @brief Processes HCI ACL data and generates corresponding log packets.
 *
 * @param p_log     Pointer to the log event structure.
 */
static void ble_log_ProcHciAcl(BT_SYS_LogEvent_T *p_log)
{
    uint8_t *p_logPacket;
    uint8_t *p_buf;
    uint16_t logLength;

    logLength = HCI_ACL_HEADER_SIZE+HCI_ACL_PKT_PARAMETER_LENGTH_SIZE+p_log->payloadLength;
    p_logPacket = OSAL_Malloc(logLength);

    if (p_logPacket == NULL)
    {
        return;
    }

    p_buf = p_logPacket;
    U16_TO_STREAM_LE(&p_buf, p_log->logId); // Reuse ID as connection handle
    U16_TO_STREAM_LE(&p_buf, p_log->payloadLength);
    VARIABLE_COPY_TO_STREAM(&p_buf, p_log->p_logPayload, p_log->payloadLength);
    if (p_log->logType == (uint16_t)BT_SYS_LOG_TYPE_HCI_ACL_TX)
    {
        ble_log_Print(BLE_LOG_TYPE_HCI_ACL_TX, logLength, p_logPacket);
    }
    else
    {
        ble_log_Print(BLE_LOG_TYPE_HCI_ACL_RX, logLength, p_logPacket);
    }

    OSAL_Free(p_logPacket);
}

/**
 * @brief Handles stack log events and processes them based on their type.
 *
 * @param p_log     Pointer to the log event structure.
 */
void BLE_LOG_StackLogHandler(BT_SYS_LogEvent_T *p_log)
{
    if (p_log->logType == (uint16_t)BT_SYS_LOG_TYPE_HCI_COMMAND)
    {
        ble_log_ProcHciCommand(p_log);
    }
    else if (p_log->logType == (uint16_t)BT_SYS_LOG_TYPE_HCI_EVENT)
    {
        ble_log_ProcHciEvent(p_log);
    }
    else if (p_log->logType == (uint16_t)BT_SYS_LOG_TYPE_HCI_ACL_TX)
    {
        ble_log_ProcHciAcl(p_log);
    }
    else if (p_log->logType == (uint16_t)BT_SYS_LOG_TYPE_HCI_ACL_RX)
    {
        ble_log_ProcHciAcl(p_log);
    }
    else
    {
        //Shall not enter here
    }

    if (p_log->p_logPayload != NULL)
    {
        OSAL_Free(p_log->p_logPayload);
    }

    if (p_log->p_returnParams!= NULL)
    {
        OSAL_Free(p_log->p_returnParams);
    }

    OSAL_Free(p_log);
}


/**
 * @brief Registers a callback for BLE log events.
 *
 * @param eventCb   Callback function to be registered for log events.
 */
void BLE_LOG_EventRegister(BLE_LOG_EventCb_T eventCb)
{
    s_logEventCb = NULL;

    if (eventCb != NULL)
    {
        s_logEventCb = eventCb;
    }
}

