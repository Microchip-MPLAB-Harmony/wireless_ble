# BLE\_SCM\_EventField\_T

**Parent topic:**[Structures](GUID-649AA2A5-B480-478B-AD34-137EDE75C855.md)

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
|evtServiceChange|Handle **[BLE\_SCM\_EVT\_SVC\_CHANGE](GUID-835AA322-4AA4-473C-9BB5-F6F7368CD9EC.md)**.|
|evtBondedCharInfo|Handle **[BLE\_SCM\_EVT\_BONDED\_CHAR\_INFO](GUID-835AA322-4AA4-473C-9BB5-F6F7368CD9EC.md)**.|
|evtConfigured|Handle **[BLE\_SCM\_EVT\_CONFIGURED](GUID-835AA322-4AA4-473C-9BB5-F6F7368CD9EC.md)**.|
|evtDiscComplete|Handle **[BLE\_SCM\_EVT\_DISC\_COMPLETE](GUID-835AA322-4AA4-473C-9BB5-F6F7368CD9EC.md)**.|

