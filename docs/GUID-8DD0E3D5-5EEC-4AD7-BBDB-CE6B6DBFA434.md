# BLE_PXPR_EvtAlertLevelWriteInd_T

## C

```c
typedef struct BLE_PXPR_EvtAlertLevelWriteInd_T
{
    uint16_t                            connHandle;
    BLE_PXPR_AlertLevel_T               alertLevel;
} BLE_PXPR_EvtAlertLevelWriteInd_T;
```

## Description

Data structure for **[BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**, **[BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|The connection handle to request writing alert level|
|alertLevel|The level that the request wants to write. See **[BLE_PXPR_AlertLevel_T](GUID-08BD9B95-4E02-4C40-AC3C-48929433890B.md)** for the detail.|
