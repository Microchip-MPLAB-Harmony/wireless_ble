# BLE\_PXPR\_EventField\_T

**Parent topic:**[Structures](GUID-A9588F6C-1E36-4F3E-9BB7-C1BF21F919CA.md)

## C

```c
typedef union
{
    BLE_PXPR_EvtAlertLevelWriteInd_T    evtLlsAlertLevelWriteInd;
    BLE_PXPR_EvtAlertLevelWriteInd_T    evtIasAlertLevelWriteInd;
} BLE_PXPR_EventField_T;
```

## Description

Union of BLE PXP Reporter callback event data types.

## Field Documentation

|Field|Description|
|-----|-----------|
|evtLlsAlertLevelWriteInd|Handle **[BLE\_PXPR\_EVT\_LLS\_ALERT\_LEVEL\_WRITE\_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**.|
|evtIasAlertLevelWriteInd|Handle **[BLE\_PXPR\_EVT\_IAS\_ALERT\_LEVEL\_WRITE\_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**.|

