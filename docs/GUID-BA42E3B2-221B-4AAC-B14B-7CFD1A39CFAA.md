# BLE\_TRCBPS\_SendData

**Parent topic:**[Functions](GUID-775797D8-E962-49BA-80EA-86C4074DC647.md)

## C

```c
uint16_t BLE_TRCBPS_SendData(uint16_t connHandle, uint16_t len, uint8_t *p_data);
```

## Description

Send data via BLE Transparent Credit Based Profile.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle.|
|\[in\] len|Data length.|
|\[in\] p\_data|Pointer to the transparent data.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue a send data command.|
|MBA\_RES\_OOM|No available memory.|
|MBA\_RES\_INVALID\_PARA|The L2CAP link doesn't exist or parameter does not meet the spec such as the data length exceeds the maximum MTU size of remote device.|
|MBA\_RES\_NO\_RESOURCE|There is no avaliable credit of the remote device.|
|MBA\_RES\_BAD\_STATE|The connection of Transparent Credit Based Profile is not established.|

