# BLE_SCM_EventField_T

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

Union of BLE_SCM callback event data types.


## Field Documentation

|Field|Description|
|:---|:---|
|evtServiceChange|Handle **[BLE_SCM_EVT_SVC_CHANGE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtBondedCharInfo|Handle **[BLE_SCM_EVT_BONDED_CHAR_INFO](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtConfigured|Handle **[BLE_SCM_EVT_CONFIGURED](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
|evtDiscComplete|Handle **[BLE_SCM_EVT_DISC_COMPLETE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)**.|
