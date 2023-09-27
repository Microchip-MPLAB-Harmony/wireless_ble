# BLE\_PXPR\_EvtAlertLevelWriteInd\_T

**Parent topic:**[Structures](GUID-A9588F6C-1E36-4F3E-9BB7-C1BF21F919CA.md)

## C

```c
typedef struct BLE_PXPR_EvtAlertLevelWriteInd_T
{
    uint16_t                            connHandle;
    BLE_PXPR_AlertLevel_T               alertLevel;
} BLE_PXPR_EvtAlertLevelWriteInd_T;
```

## Description

Data structure for **[BLE\_PXPR\_EVT\_LLS\_ALERT\_LEVEL\_WRITE\_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**, **[BLE\_PXPR\_EVT\_IAS\_ALERT\_LEVEL\_WRITE\_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|The connection handle to request writing alert level.|
|alertLevel|The level that the request wants to write. See **[BLE\_PXPR\_AlertLevel\_T](GUID-08BD9B95-4E02-4C40-AC3C-48929433890B.md)** for the detail.|

