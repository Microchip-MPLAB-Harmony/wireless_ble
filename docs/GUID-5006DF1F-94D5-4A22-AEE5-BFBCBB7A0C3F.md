# BLE_DD_EventField_T

## C

```c
typedef union
{
    BLE_DD_EvtDiscResult_T  evtDiscResult;
    BLE_DD_EvtSecRequire_T  evtSecRequire;
    BLE_DD_EvtConnect_T     evtConnect;
    BLE_DD_EvtDisconnect_T  evtDisconnect;
} BLE_DD_EventField_T;
```

## Description

Union of BLE_DD callback event data types.


## Field Documentation

|Field|Description|
|:---|:---|
|evtDiscResult|Handle **[BLE_DD_EVT_DISC_COMPLETE](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtSecRequire|Handle **[BLE_DD_EVT_SEC_REQUIRED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtConnect|Handle **[BLE_DD_EVT_CONNECTED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtDisconnect|Handle **[BLE_DD_EVT_DISCONNECTED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
