# BLE\_GAP\_EventId\_T

**Parent topic:**[Enums](GUID-80C9765D-2A0B-4D37-B051-2CB71C3BE226.md)

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
    BLE_GAP_EVT_PATH_LOSS_THRESHOLD,
}BLE_GAP_EventId_T;
```

## Description

GAP events delivered to application from BLE Stack.

|Enumerator|Description|
|----------|-----------|
|BLE\_GAP\_EVT\_CONNECTED|BLE link is established. See **[BLE\_GAP\_EvtConnect\_T](GUID-4C985581-7FBE-48BC-A815-1178C3173F64.md)** for the event details.|
|BLE\_GAP\_EVT\_DISCONNECTED|BLE link is terminated. See **[BLE\_GAP\_EvtDisconnect\_T](GUID-48E2E234-B4A0-49AA-A47F-CF7DC384547F.md)** for the event details.|
|BLE\_GAP\_EVT\_CONN\_PARAM\_UPDATE|The connection parameters of BLE link are updated. See **[BLE\_GAP\_EvtConnParamUpdateParams\_T](GUID-55337ABD-8270-4DFB-A215-64165314F53F.md)** for the event details.|
|BLE\_GAP\_EVT\_ENCRYPT\_STATUS|The encryption status of BLE link. See **[BLE\_GAP\_EvtEncryptStatus\_T](GUID-B16DD6F5-6244-4EA7-A150-47D9193DC875.md)** for the event content.|
|BLE\_GAP\_EVT\_ADV\_REPORT|Received advertising report. See **[BLE\_GAP\_EvtAdvReport\_T](GUID-386E57A5-2117-4B00-A21B-C6AF1C2840F7.md)** for the event details.|
|BLE\_GAP\_EVT\_ENC\_INFO\_REQUEST|See **[BLE\_GAP\_EvtEncInfoRequest\_T](GUID-FF1D1C5A-DAC3-4F2F-AB46-3A796657A0EA.md)** for the event content.|
|BLE\_GAP\_EVT\_REMOTE\_CONN\_PARAM\_REQUEST|If remote device supports Link Layer Connection Parameters Request procedure, this request will receive when remote device wants to change the connection parameters. See **[BLE\_GAP\_EvtRemoteConnParamRequest\_T](GUID-F5207E96-1B43-4751-BF5B-C1592BE75A72.md)** for the event content.|
|BLE\_GAP\_EVT\_EXT\_ADV\_REPORT|Received extended advertising report. See **[BLE\_GAP\_EvtExtAdvReport\_T](GUID-502CC449-E705-43E0-8F5B-C75E7D635B6D.md)** for the event details.|
|BLE\_GAP\_EVT\_ADV\_TIMEOUT|Advertising terminated because timeout.|
|BLE\_GAP\_EVT\_TX\_BUF\_AVAILABLE|Notify the ACL tx buffer is available. See **[BLE\_GAP\_EvtTxBufAvailable\_T](GUID-2C724ED6-CDD6-4C34-B6B4-A23AF82E9B5C.md)**.|
|BLE\_GAP\_EVT\_DEVICE\_NAME\_CHANGED|The device name was changed by remote device via Generic Assess service. **[BLE\_GAP\_EvtDeviceNameChanged\_T](GUID-7E1F0AA2-BF30-47F1-93A9-7AD7E929340D.md)** for the event content.|
|BLE\_GAP\_EVT\_AUTH\_PAYLOAD\_TIMEOUT|Authenticated payload timeout. **[BLE\_GAP\_EvtAuthPayloadTimeout\_T](GUID-17D13553-82D8-47BD-B39B-1E6BD3428CBF.md)** for the event content.|
|BLE\_GAP\_EVT\_PHY\_UPDATE|PHY update. **[BLE\_GAP\_EvtPhyUpdate\_T](GUID-8038B28A-3E82-4188-877C-CF1E17CFF695.md)** for the event content.|
|BLE\_GAP\_EVT\_SCAN\_REQ\_RECEIVED|Received scan request. **[BLE\_GAP\_EvtScanReqReceived\_T](GUID-45875F52-95A3-4845-BEBA-E029198E9941.md)** for the event content.|
|BLE\_GAP\_EVT\_DIRECT\_ADV\_REPORT|Received direct advertising report. **[BLE\_GAP\_EvtDirectAdvReport\_T](GUID-350E2D86-917E-46F5-A27B-5B7B6CD0E740.md)** for the event content.|
|BLE\_GAP\_EVT\_PERI\_ADV\_SYNC\_EST|Periodic advertising sync established. **[BLE\_GAP\_EvtPeriAdvSyncEst\_T](GUID-01A09C07-8775-47C5-ABC7-C9A54B34E4B5.md)** for the event content.|
|BLE\_GAP\_EVT\_PERI\_ADV\_REPORT|Received periodic advertising report. **[BLE\_GAP\_EvtPeriAdvReport\_T](GUID-18188A91-010D-49FC-B9A1-5C9A227DC09E.md)** for the event content.|
|BLE\_GAP\_EVT\_PERI\_ADV\_SYNC\_LOST|Periodic advertising sync lost. **[BLE\_GAP\_EvtPeriAdvSyncLost\_T](GUID-B046D58E-A9AB-4EB5-9D08-7CB8E6064D68.md)** for the event content.|
|BLE\_GAP\_EVT\_ADV\_SET\_TERMINATED|Advertising set terminated. **[BLE\_GAP\_EvtAdvSetTerminated\_T](GUID-B5D2CD38-2685-4C00-AF72-B5B70E4AF0E5.md)** for the event content.|
|BLE\_GAP\_EVT\_SCAN\_TIMEOUT|Scan terminated because timeout.|
|BLE\_GAP\_EVT\_TRANSMIT\_POWER\_REPORTING|Report tx power level on the ACL connection identified by the Connection\_Handle. **[BLE\_GAP\_EvtTxPowerReporting\_T](GUID-E3A99701-8888-4242-8F49-AE79B537ECB3.md)** for the event content.|
|BLE\_GAP\_EVT\_ADV\_COMPL|Enable one time advertising is completed. See **[BLE\_GAP\_EvtAdvCompl\_T](GUID-54AA20AC-CB4E-411A-A4B5-2F05CA4BD89F.md)** for the event content.|
|BLE\_GAP\_EVT\_PATH\_LOSS\_THRESHOLD|Report a path loss threshold crossing on the ACL connection. See **[BLE\_GAP\_EvtPathLossThreshold\_T](GUID-13D1859A-DD03-4F2D-84F5-239D446B4AB7.md)** for the event content.|

