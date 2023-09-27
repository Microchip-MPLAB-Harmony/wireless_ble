# BLE\_OTAPS\_EventField\_T

**Parent topic:**[Structures](GUID-DD4DCB53-DB4E-479A-9365-B7F9312729B4.md)

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
|evtUpdateReq|Handle **[BLE\_OTAPS\_EVT\_UPDATE\_REQ](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**.|
|evtStartInd|Handle **[BLE\_OTAPS\_EVT\_START\_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**.|
|evtUpdatingInd|Handle **[BLE\_OTAPS\_EVT\_UPDATING\_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**.|
|evtCompleteInd|Handle **[BLE\_OTAPS\_EVT\_COMPLETE\_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**.|

