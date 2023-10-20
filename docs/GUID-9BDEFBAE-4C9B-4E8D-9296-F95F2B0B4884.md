# BLE\_L2CAP\_CbSendSdu

**Parent topic:**[Functions](GUID-B780FD08-A101-4686-A604-9AF23DB222E0.md)

## C

```c
uint16_t BLE_L2CAP_CbSendSdu(uint8_t leL2capId, uint16_t length, uint8_t *p_payload);
```

## Description

Send a L2cap SDU over credit based connection channel.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] leL2capId|L2cap instance.|
|\[in\] length|Length of L2cap SDU. See the maximum definition **[Maximum SDU size](GUID-153239C5-D97F-41B6-95AD-467048F32CB7.md)**.|
|\[in\] p\_payload|Point to the L2cap SDU buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully send a L2cap data.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_NO\_RESOURCE|No Transmit buffers available for sending L2cap data.|
|MBA\_RES\_FAIL|Fail to send L2cap data by error L2cap instance.|

