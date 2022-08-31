# BLE_ANPC_EnableUnreadAlertNtfy

## C

```c
uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable);
```

## Description

Enable Unread Alert Notificaiton.

Application must call this API after starting BLE Alert Notification Client procedure.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Handle of the connection.|
|\[in\] enable|Set true to enable notification of Unread Alert. Otherwise set false.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Send command successful.|
MBA_RES_INVALID_PARA|Invalid parameters.|
MBA_RES_OOM|Internal memory allocation failure.|
