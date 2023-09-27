# BLE\_DD\_EventField\_T

**Parent topic:**[Structures](GUID-B7B198D6-037B-468B-9A14-943F83191073.md)

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

Union of BLE\_DD callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtDiscResult|Handle **[BLE\_DD\_EVT\_DISC\_COMPLETE](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtSecRequire|Handle **[BLE\_DD\_EVT\_SEC\_REQUIRED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtConnect|Handle **[BLE\_DD\_EVT\_CONNECTED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|
|evtDisconnect|Handle **[BLE\_DD\_EVT\_DISCONNECTED](GUID-45B8E598-9414-432D-9F5F-52044CCDE39E.md)**.|

