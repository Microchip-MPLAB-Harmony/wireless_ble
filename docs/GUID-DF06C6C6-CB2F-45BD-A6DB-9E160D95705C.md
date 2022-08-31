# BLE_ANPC_ReadSuppNewAlertCat

## C

```c
uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle);
```

## Description

Read the value of Supported New Alert Category.

Application must call this API after starting BLE Alert Notification Client procedure.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Handle of the connection.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Send command successful.|
MBA_RES_FAIL|Fail.|
