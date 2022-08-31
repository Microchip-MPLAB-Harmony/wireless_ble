# BLE_GAP_EventId_T

## C

```c
typedef enum BLE_GAP_EventId_T
{
    BLE_GAP_EVT_CONNECTED,
    BLE_GAP_EVT_DISCONNECTED,
    BLE_GAP_EVT_CONN_PARAM_UPDATE,
    BLE_GAP_EVT_ENCRYPT_STATUS,
    BLE_GAP_EVT_ADV_REPORT,
    BLE_GAP_EVT_ENC_INFO_REQUEST,
    BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST,
    BLE_GAP_EVT_EXT_ADV_REPORT,
    BLE_GAP_EVT_ADV_TIMEOUT,
    BLE_GAP_EVT_TX_BUF_AVAILABLE,
    BLE_GAP_EVT_DEVICE_NAME_CHANGED,
    BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT,
    BLE_GAP_EVT_PHY_UPDATE,
    BLE_GAP_EVT_SCAN_REQ_RECEIVED,
    BLE_GAP_EVT_DIRECT_ADV_REPORT,
    BLE_GAP_EVT_PERI_ADV_SYNC_EST,
    BLE_GAP_EVT_PERI_ADV_REPORT,
    BLE_GAP_EVT_PERI_ADV_SYNC_LOST,
    BLE_GAP_EVT_ADV_SET_TERMINATED,
    BLE_GAP_EVT_SCAN_TIMEOUT,
    BLE_GAP_EVT_TRANSMIT_POWER_REPORTING,
    BLE_GAP_EVT_ADV_COMPL,
}BLE_GAP_EventId_T;
```

## Description

GAP events delivered to application from BLE Stack.


|Enumerator|Description|
|:---|:---|
|BLE_GAP_EVT_CONNECTED|BLE link is established. See **[BLE_GAP_EvtConnect_T](GUID-4C985581-7FBE-48BC-A815-1178C3173F64.md)** for the event details.|
|BLE_GAP_EVT_DISCONNECTED|BLE link is terminated. See **[BLE_GAP_EvtDisconnect_T](GUID-48E2E234-B4A0-49AA-A47F-CF7DC384547F.md)** for the event details.|
|BLE_GAP_EVT_CONN_PARAM_UPDATE|The connection parameters of BLE link are updated. See **[BLE_GAP_EvtConnParamUpdateParams_T](GUID-55337ABD-8270-4DFB-A215-64165314F53F.md)** for the event details.|
|BLE_GAP_EVT_ENCRYPT_STATUS|The encryption status of BLE link. See **[BLE_GAP_EvtEncryptStatus_T](GUID-B16DD6F5-6244-4EA7-A150-47D9193DC875.md)** for the event content.|
|BLE_GAP_EVT_ADV_REPORT|Received advertising report. See **[BLE_GAP_EvtAdvReport_T](GUID-386E57A5-2117-4B00-A21B-C6AF1C2840F7.md)** for the event details.|
|BLE_GAP_EVT_ENC_INFO_REQUEST|See **[BLE_GAP_EvtEncInfoRequest_T](GUID-FF1D1C5A-DAC3-4F2F-AB46-3A796657A0EA.md)** for the event content.|
|BLE_GAP_EVT_REMOTE_CONN_PARAM_REQUEST|If remote device supports Link Layer Connection Parameters Request procedure, this request will receive when remote device wants to change the connection parameters. See **[BLE_GAP_EvtRemoteConnParamRequest_T](GUID-F5207E96-1B43-4751-BF5B-C1592BE75A72.md)** for the event content.| 
|BLE_GAP_EVT_EXT_ADV_REPORT|Received extended advertising report. See **[BLE_GAP_EvtExtAdvReport_T](GUID-502CC449-E705-43E0-8F5B-C75E7D635B6D.md)** for the event details.|
|BLE_GAP_EVT_ADV_TIMEOUT|Advertising terminated because timeout.|
|BLE_GAP_EVT_TX_BUF_AVAILABLE|Notify the ACL tx buffer is available. See **[BLE_GAP_EvtTxBufAvailable_T](GUID-2C724ED6-CDD6-4C34-B6B4-A23AF82E9B5C.md)**.|
|BLE_GAP_EVT_DEVICE_NAME_CHANGED|The device name was changed by remote device via Generic Assess service. **[BLE_GAP_EvtDeviceNameChanged_T](GUID-7E1F0AA2-BF30-47F1-93A9-7AD7E929340D.md)** for the event content.|
|BLE_GAP_EVT_AUTH_PAYLOAD_TIMEOUT|Authenticated payload timeout. **[BLE_GAP_EvtAuthPayloadTimeout_T](GUID-17D13553-82D8-47BD-B39B-1E6BD3428CBF.md)** for the event content.|
|BLE_GAP_EVT_PHY_UPDATE|PHY update. **[BLE_GAP_EvtPhyUpdate_T](GUID-8038B28A-3E82-4188-877C-CF1E17CFF695.md)** for the event content.|
|BLE_GAP_EVT_SCAN_REQ_RECEIVED|Received scan request. **[BLE_GAP_EvtScanReqReceived_T](GUID-45875F52-95A3-4845-BEBA-E029198E9941.md)** for the event content.|
|BLE_GAP_EVT_DIRECT_ADV_REPORT|Received direct advertising report. **[BLE_GAP_EvtDirectAdvReport_T](GUID-350E2D86-917E-46F5-A27B-5B7B6CD0E740.md)** for the event content.|
|BLE_GAP_EVT_PERI_ADV_SYNC_EST|Periodic advertising sync established. **[BLE_GAP_EvtPeriAdvSyncEst_T](GUID-01A09C07-8775-47C5-ABC7-C9A54B34E4B5.md)** for the event content.|
|BLE_GAP_EVT_PERI_ADV_REPORT|Received periodic advertising report. **[BLE_GAP_EvtPeriAdvReport_T](GUID-18188A91-010D-49FC-B9A1-5C9A227DC09E.md)** for the event content.|
|BLE_GAP_EVT_PERI_ADV_SYNC_LOST|Periodic advertising sync lost. **[BLE_GAP_EvtPeriAdvSyncLost_T](GUID-B046D58E-A9AB-4EB5-9D08-7CB8E6064D68.md)** for the event content.|
|BLE_GAP_EVT_ADV_SET_TERMINATED|Advertising set terminated. **[BLE_GAP_EvtAdvSetTerminated_T](GUID-B5D2CD38-2685-4C00-AF72-B5B70E4AF0E5.md)** for the event content.|
|BLE_GAP_EVT_SCAN_TIMEOUT|Scan terminated because timeout.|
|BLE_GAP_EVT_TRANSMIT_POWER_REPORTING|Report tx power level on the ACL connection identified by the Connection_Handle. **[BLE_GAP_EvtTxPowerReporting_T](GUID-E3A99701-8888-4242-8F49-AE79B537ECB3.md)** for the event content.|
|BLE_GAP_EVT_ADV_COMPL|Enable one time advertising is completed. See **[BLE_GAP_EvtAdvCompl_T](GUID-54AA20AC-CB4E-411A-A4B5-2F05CA4BD89F.md)** for the event content.|
