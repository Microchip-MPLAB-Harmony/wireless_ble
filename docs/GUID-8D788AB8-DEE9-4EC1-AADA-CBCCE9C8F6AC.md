# BLE\_L2CAP\_ConnParamUpdateRsp

**Parent topic:**[Functions](GUID-B780FD08-A101-4686-A604-9AF23DB222E0.md)

## C

```c
uint16_t BLE_L2CAP_ConnParamUpdateRsp(uint16_t connHandle, uint16_t result);
```

## Description

Reply a connection parameter update request.

### Note

This function should be called when BLE\_L2CAP\_EVT\_CONN\_PARA\_UPDATE\_REQ received.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] result|Accept or reject the request. See **[Connection parameter update result](GUID-C35721F1-C7BA-41E0-8385-5332C466D116.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue a L2cap connection parameters update response.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_NO\_RESOURCE|No Transmit buffers available for sending L2cap connection parameters update response.|
|MBA\_RES\_FAIL|Fail to send L2cap connection parameters update response by error L2cap instance.|

