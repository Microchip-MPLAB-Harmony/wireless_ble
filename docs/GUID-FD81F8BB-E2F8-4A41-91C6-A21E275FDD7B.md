# BLE\_ANCS\_EventField\_T

**Parent topic:**[Structures](GUID-9D4040A1-4922-48F6-BA70-EB4B094F9B91.md)

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
|evtDiscComplete|Handle **[BLE\_ANCS\_EVT\_DISC\_COMPLETE\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**|
|evtErrAttrBufInd|Handle **[BLE\_ANCS\_EVT\_ERR\_ATTR\_BUF\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|
|evtErrRecomposeBufInd|Handle **[BLE\_ANCS\_EVT\_ERR\_RECOMPOSE\_BUF\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|
|evtNtfyInd|Handle **[BLE\_ANCS\_EVT\_NTFY\_ADDED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**, **[BLE\_ANCS\_EVT\_NTFY\_MODIFIED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**, **[BLE\_ANCS\_EVT\_NTFY\_REMOVED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|
|evtNtfyAttrInd|Handle **[BLE\_ANCS\_EVT\_NTFY\_ATTR\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|
|evtAppAttrInd|Handle **[BLE\_ANCS\_EVT\_APP\_ATTR\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|
|evtErrInd|Handle **[BLE\_ANCS\_EVT\_ERR\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**.|

