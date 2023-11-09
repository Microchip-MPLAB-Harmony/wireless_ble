# BLE\_TRSPS\_EventField\_T

**Parent topic:**[Structures](GUID-CF652FF4-6E48-4AFE-98C2-AF0B4F1E2DFE.md)

## C

```c
typedef union
{
    BLE_TRSPS_EvtCtrlStatus_T        onCtrlStatus;
    BLE_TRSPS_EvtTxStatus_T          onTxStatus;
    BLE_TRSPS_EvtCbfcEnabled_T       onCbfcEnabled;
    BLE_TRSPS_EvtReceiveData_T       onReceiveData;
    BLE_TRSPS_EvtVendorCmd_T         onVendorCmd;
} BLE_TRSPS_EventField_T;
```

## Description

The union of BLE Transparent profile server event types.

## Field Documentation

|Field|Description|
|-----|-----------|
|onCtrlStatus|Handle **[BLE\_TRSPS\_EVT\_CTRL\_STATUS](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)**.|
|onTxStatus|Handle **[BLE\_TRSPS\_EVT\_TX\_STATUS](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)**.|
|onCbfcEnabled|Handle **[BLE\_TRSPS\_EVT\_CBFC\_ENABLED](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)**.|
|onReceiveData|Handle **[BLE\_TRSPS\_EVT\_RECEIVE\_DATA](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)**.|
|onVendorCmd|Handle **[BLE\_TRSPS\_EVT\_VENDOR\_CMD](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)**.|

