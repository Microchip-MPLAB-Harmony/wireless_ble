# BLE_GAP_WriteAuthPayloadTimeout

## C

```c
uint16_t BLE_GAP_WriteAuthPayloadTimeout(uint16_t connHandle, uint16_t authPayloadTo);
```

## Description

Write authenticated payload timeout parameter.

### Note

This configuration determine when to use the LE ping sequence.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] authPayloadTo|Authenticated payload timeout parameter (Unit: 10 ms). The authPayloadTo shall be equal to or greater than connInterval*(1 + peripheralLatency). Refer to **[BLE_GAP_EvtConnect_T](GUID-4C985581-7FBE-48BC-A815-1178C3173F64.md)**.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully write the authenticated payload timeout parameter.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_UNKNOWN_CONN_ID|Unknown connection identifier.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed.|
