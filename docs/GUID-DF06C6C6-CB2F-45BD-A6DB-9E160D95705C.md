# BLE\_ANPC\_ReadSuppNewAlertCat

**Parent topic:**[Functions](GUID-5028AA06-788A-4B7E-B273-018FF2507CA1.md)

## C

```c
uint16_t BLE_ANPC_ReadSuppNewAlertCat(uint16_t connHandle);
```

## Description

Read the value of Supported New Alert Category.

Application must call this API after the BLE Alert Notification Client procedure has been initiated.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Handle of the connection.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue a read Supported New Alert Category packet.|
|MBA\_RES\_INVALID\_PARA|Invalid connection handle. Connection handle is not valid.|
|MBA\_RES\_FAIL|Fail to issue a read Supported New Alert Category packet.|

