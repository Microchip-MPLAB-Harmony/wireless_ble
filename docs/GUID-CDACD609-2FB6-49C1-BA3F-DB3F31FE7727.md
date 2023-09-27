# BLE\_ANCS\_EventField\_T

**Parent topic:**[Structures](GUID-A2656700-B0A1-443C-903C-42AE1A0A1AD8.md)

## C

```c
typedef union
{
    BLE_ANCS_EvtDiscComplete_T         evtDiscComplete;
    BLE_ANCS_EvtErrAttrBufInd_T        evtErrAttrBufInd;
    BLE_ANCS_EvtErrRecomposeBufInd_T   evtErrRecomposeBufInd;
    BLE_ANCS_EvtNtfyInd_T              evtNtfyInd;
    BLE_ANCS_EvtNtfyAttrInd_T          evtNtfyAttrInd;
    BLE_ANCS_EvtAppAttrInd_T           evtAppAttrInd;
    BLE_ANCS_EvtErrInd_T               evtErrInd;
} BLE_ANCS_EventField_T;
```

## Description

Union of BLE ANCS callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtDiscComplete|Handle **[BLE\_ANCS\_EVT\_DISC\_COMPLETE\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**|
|evtErrAttrBufInd|Handle **[BLE\_ANCS\_EVT\_ERR\_ATTR\_BUF\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtErrRecomposeBufInd|Handle **[BLE\_ANCS\_EVT\_ERR\_RECOMPOSE\_BUF\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtNtfyInd|Handle **[BLE\_ANCS\_EVT\_NTFY\_ADDED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**, **[BLE\_ANCS\_EVT\_NTFY\_MODIFIED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**, **[BLE\_ANCS\_EVT\_NTFY\_REMOVED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtNtfyAttrInd|Handle **[BLE\_ANCS\_EVT\_NTFY\_ATTR\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtAppAttrInd|Handle **[BLE\_ANCS\_EVT\_APP\_ATTR\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtErrInd|Handle **[BLE\_ANCS\_EVT\_ERR\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|

