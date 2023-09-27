# BLE\_TRSPS\_EventField\_T

**Parent topic:**[Structures](GUID-596C22F0-71C2-4FD3-9512-C8725C77BFA6.md)

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
|onCtrlStatus|Handle **[BLE\_TRSPS\_EVT\_CTRL\_STATUS](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onTxStatus|Handle **[BLE\_TRSPS\_EVT\_TX\_STATUS](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onCbfcEnabled|Handle **[BLE\_TRSPS\_EVT\_CBFC\_ENABLED](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onReceiveData|Handle **[BLE\_TRSPS\_EVT\_RECEIVE\_DATA](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onVendorCmd|Handle **[BLE\_TRSPS\_EVT\_VENDOR\_CMD](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|

