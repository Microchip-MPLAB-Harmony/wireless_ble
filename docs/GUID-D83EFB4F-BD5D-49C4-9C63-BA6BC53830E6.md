# BLE\_PXPM\_EvtLlsAlertLvWriteRspInd\_T

**Parent topic:**[Structures](GUID-CB548428-A46D-43EA-9047-32BC79F54282.md)

## C

```c
typedef struct BLE_PXPM_EvtLlsAlertLvWriteRspInd_T
{
    uint16_t        connHandle;
    uint16_t        errCode;
} BLE_PXPM_EvtLlsAlertLvWriteRspInd_T;
```

## Description

Data structure for **[BLE\_PXPM\_EVT\_LLS\_ALERT\_LEVEL\_WRITE\_RSP\_IND](GUID-18A9DADE-E30C-443A-9D82-4219BC843237.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection Handle.|
|errCode|The error code of the writing alert level response. See **[ATT error code](GUID-053481D7-C98A-4E78-B7AD-4D71F3A1B03B.md)**.|

