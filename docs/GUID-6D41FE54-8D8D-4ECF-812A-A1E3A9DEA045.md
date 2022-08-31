# BLE_TRSPS_EventField_T

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
|:---|:---|
|onCtrlStatus|Handle **[BLE_TRSPS_EVT_CTRL_STATUS](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onTxStatus|Handle **[BLE_TRSPS_EVT_TX_STATUS](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onCbfcEnabled|Handle **[BLE_TRSPS_EVT_CBFC_ENABLED](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onReceiveData|Handle **[BLE_TRSPS_EVT_RECEIVE_DATA](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
|onVendorCmd|Handle **[BLE_TRSPS_EVT_VENDOR_CMD](GUID-1D0AD6D8-972B-4D20-89ED-354F04B1AD8B.md)**.|
