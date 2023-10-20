# BLE\_HOGPS\_SendBatteryLevel

**Parent topic:**[Functions](GUID-4766BD96-39D9-49CC-825C-772FFD3D0082.md)

## C

```c
uint16_t BLE_HOGPS_SendBatteryLevel(uint16_t connHandle);
```

## Description

Send battery level of HID device by Battery service.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|The connection handle associated with the connection.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully send the input report.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|

