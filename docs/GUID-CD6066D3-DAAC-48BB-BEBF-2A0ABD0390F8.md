# BLE_PXPR_EventField_T

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
|:---|:---|
|evtLlsAlertLevelWriteInd|Handle **[BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**|
|evtIasAlertLevelWriteInd|Handle **[BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND](GUID-7FECC045-859E-429B-987B-C6BB9271911A.md)**|
