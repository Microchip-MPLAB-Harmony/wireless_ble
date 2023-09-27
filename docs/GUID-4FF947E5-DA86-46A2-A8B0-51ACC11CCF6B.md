# BLE\_SCM\_EventField\_T

**Parent topic:**[Structures](GUID-3783C6B6-D61B-4B5A-B558-E749DFE943E2.md)

## C

```c
typedef union
{
    BLE_SCM_EvtServiceChange_T  evtServiceChange;
    BLE_SCM_EvtBondedCharInfo_T evtBondedCharInfo;
    BLE_SCM_EvtConfigured_T     evtConfigured;
    BLE_SCM_EvtDiscComplete_T   evtDiscComplete;
} BLE_SCM_EventField_T;
```

## Description

Union of BLE\_SCM callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtServiceChange|Handle **[BLE\_SCM\_EVT\_SVC\_CHANGE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtBondedCharInfo|Handle **[BLE\_SCM\_EVT\_BONDED\_CHAR\_INFO](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtConfigured|Handle **[BLE\_SCM\_EVT\_CONFIGURED](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtDiscComplete|Handle **[BLE\_SCM\_EVT\_DISC\_COMPLETE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|

