# BLE\_PXPM\_EvtLlsAlertLvInd\_T

**Parent topic:**[Structures](GUID-CB548428-A46D-43EA-9047-32BC79F54282.md)

## C

```c
typedef struct BLE_PXPM_EvtLlsAlertLvInd_T
{
    uint16_t                connHandle;
    BLE_PXPM_AlertLevel_T   alertLevel;
} BLE_PXPM_EvtLlsAlertLvInd_T;
```

## Description

Data structure for **[BLE\_PXPM\_EVT\_LLS\_ALERT\_LEVEL\_IND](GUID-18A9DADE-E30C-443A-9D82-4219BC843237.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle.|
|alertLevel|Alert level.|

