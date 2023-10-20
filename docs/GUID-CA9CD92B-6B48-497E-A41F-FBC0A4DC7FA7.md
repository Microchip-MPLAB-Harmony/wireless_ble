# BLE\_ANPC\_EnableUnreadAlertNtfy

**Parent topic:**[Functions](GUID-5028AA06-788A-4B7E-B273-018FF2507CA1.md)

## C

```c
uint16_t BLE_ANPC_EnableUnreadAlertNtfy(uint16_t connHandle, bool enable);
```

## Description

Enable Unread Alert Notificaiton.

Application must call this API after starting BLE Alert Notification Client procedure.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Handle of the connection.|
|\[in\] enable|Set true to enable notification of Unread Alert. Otherwise set false.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Send command successful.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
|MBA\_RES\_OOM|Internal memory allocation failure.|

