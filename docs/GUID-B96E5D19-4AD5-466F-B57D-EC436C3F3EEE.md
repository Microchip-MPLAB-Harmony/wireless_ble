# BLE\_GAP\_EventField\_T

**Parent topic:**[Structures](GUID-A15AC144-CD72-427A-B096-33FC1E7FEA88.md)

## C

```c
typedef union
{
    BLE_GAP_EvtConnect_T                   evtConnect;
    BLE_GAP_EvtDisconnect_T                evtDisconnect;
    BLE_GAP_EvtConnParamUpdateParams_T     evtConnParamUpdate;
    BLE_GAP_EvtEncryptStatus_T             evtEncryptStatus;
    BLE_GAP_EvtAdvReport_T                 evtAdvReport;
    BLE_GAP_EvtEncInfoRequest_T            evtEncInfoReq;
    BLE_GAP_EvtRemoteConnParamRequest_T    evtRemoteConnParamReq;
    BLE_GAP_EvtExtAdvReport_T              evtExtAdvReport;
    BLE_GAP_EvtTxBufAvailable_T            evtTxBufAvailable;
    BLE_GAP_EvtDeviceNameChanged_T         evtDevNameChanged;
    BLE_GAP_EvtAuthPayloadTimeout_T        evtAuthPayloadTimeout;
    BLE_GAP_EvtPhyUpdate_T                 evtPhyUpdate;
    BLE_GAP_EvtScanReqReceived_T           evtScanReqReceived;
    BLE_GAP_EvtDirectAdvReport_T           evtDirectAdvReport;
    BLE_GAP_EvtPeriAdvSyncEst_T            evtPeriAdvSyncEst;
    BLE_GAP_EvtPeriAdvReport_T             evtPeriAdvReport;
    BLE_GAP_EvtPeriAdvSyncLost_T           evtPeriAdvSyncLost;
    BLE_GAP_EvtAdvSetTerminated_T          evtAdvSetTerminated;
    BLE_GAP_EvtTxPowerReporting_T          evtTxPowerReporting;
    BLE_GAP_EvtAdvCompl_T                  evtAdvCompl;
    BLE_GAP_EvtPathLossThreshold_T         evtPathLossThreshold;
} BLE_GAP_EventField_T;
```

## Description

Union of BLE GAP callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtConnect|Handle **[BLE\_GAP\_EVT\_CONNECTED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtDisconnect|Handle **[BLE\_GAP\_EVT\_DISCONNECTED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtConnParamUpdate|Handle **[BLE\_GAP\_EVT\_CONN\_PARAM\_UPDATE](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtEncryptStatus|Handle **[BLE\_GAP\_EVT\_ENCRYPT\_STATUS](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtAdvReport|Handle **[BLE\_GAP\_EVT\_ADV\_REPORT](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtEncInfoReq|Handle **[BLE\_GAP\_EVT\_ENC\_INFO\_REQUEST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtRemoteConnParamReq|Handle **[BLE\_GAP\_EVT\_REMOTE\_CONN\_PARAM\_REQUEST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtExtAdvReport|Handle **[BLE\_GAP\_EVT\_EXT\_ADV\_REPORT](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtTxBufAvailable|Handle **[BLE\_GAP\_EVT\_TX\_BUF\_AVAILABLE](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtDevNameChanged|Handle **[BLE\_GAP\_EVT\_DEVICE\_NAME\_CHANGED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtAuthPayloadTimeout|Handle **[BLE\_GAP\_EVT\_AUTH\_PAYLOAD\_TIMEOUT](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtPhyUpdate|Handle **[BLE\_GAP\_EVT\_PHY\_UPDATE](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtScanReqReceived|Handle **[BLE\_GAP\_EVT\_SCAN\_REQ\_RECEIVED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtDirectAdvReport|Handle **[BLE\_GAP\_EVT\_DIRECT\_ADV\_REPORT](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtPeriAdvSyncEst|Handle **[BLE\_GAP\_EVT\_PERI\_ADV\_SYNC\_EST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtPeriAdvReport|Handle **[BLE\_GAP\_EVT\_PERI\_ADV\_REPORT](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtPeriAdvSyncLost|Handle **[BLE\_GAP\_EVT\_PERI\_ADV\_SYNC\_LOST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtAdvSetTerminated|Handle **[BLE\_GAP\_EVT\_ADV\_SET\_TERMINATED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtTxPowerReporting|Handle **[BLE\_GAP\_EVT\_TRANSMIT\_POWER\_REPORTING](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtAdvCompl|Handle **[BLE\_GAP\_EVT\_ADV\_COMPL](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)**.|
|evtPathLossThreshold|Handle **[BLE\_GAP\_EVT\_PATH\_LOSS\_THRESHOLD]((GUID-085D2B3E-E5DB-4072-8916-29201399538E.md).md)**.|

