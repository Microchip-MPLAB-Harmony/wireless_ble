# BLE_TRCBPS_SendData

## C

```c
uint16_t BLE_TRCBPS_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data);
```

## Description

Send data via BLE Transparent Credit Based Profile.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Connection handle.|
|\[in\] len|Data length.|
|\[in\] p_data|Pointer to the transparent data.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully issue a send data command.|
MBA_RES_OOM|No available memory.|
MBA_RES_INVALID_PARA|The L2CAP link doesn't exist or parameter does not meet the spec such as the data length exceeds the maximum MTU size of remote device.|
MBA_RES_NO_RESOURCE|There is no avaliable credit of the remote device.|
MBA_RES_BAD_STATE|The connection of Transparent Credit Based Profile is not established.|
