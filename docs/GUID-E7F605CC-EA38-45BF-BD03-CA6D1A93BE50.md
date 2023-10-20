# BLE\_OTAPS\_EventField\_T

**Parent topic:**[Structures](GUID-4EAA8368-F1C7-4125-A844-2BFFE5CAF2F9.md)

## C

```c
typedef union
{
    BLE_OTAPS_EvtUpdateReq_T            evtUpdateReq;
    BLE_OTAPS_EvtStartInd_T             evtStartInd;
    BLE_OTAPS_EvtUpdatingInd_T          evtUpdatingInd;
    BLE_OTAPS_EvtCompleteInd_T          evtCompleteInd;
} BLE_OTAPS_EventField_T;
```

## Description

Union of BLE OTA profile callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtUpdateReq|Handle **[BLE\_OTAPS\_EVT\_UPDATE\_REQ](GUID-F3177CD0-E008-4DCD-AA8E-40F6DC495A8A.md)**.|
|evtStartInd|Handle **[BLE\_OTAPS\_EVT\_START\_IND](GUID-F3177CD0-E008-4DCD-AA8E-40F6DC495A8A.md)**.|
|evtUpdatingInd|Handle **[BLE\_OTAPS\_EVT\_UPDATING\_IND](GUID-F3177CD0-E008-4DCD-AA8E-40F6DC495A8A.md)**.|
|evtCompleteInd|Handle **[BLE\_OTAPS\_EVT\_COMPLETE\_IND](GUID-F3177CD0-E008-4DCD-AA8E-40F6DC495A8A.md)**.|

