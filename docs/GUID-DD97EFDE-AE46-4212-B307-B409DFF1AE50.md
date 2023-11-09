# BLE\_SMP\_PasskeyNegativeReply

**Parent topic:**[Functions](GUID-B4A018ED-CB34-4D52-A7F9-3E7808C43BF8.md)

## C

```c
uint16_t BLE_SMP_PasskeyNegativeReply(uint16_t connHandle);
```

## Description

Reply to a passkey request event if local device cannot provide the information.

### Note

The API should be called in response to passkey entry request or passkey display request.<br />Hence, it should be called in SMP callback function when **[BLE\_SMP\_EVT\_INPUT\_PASSKEY\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)** event or<br />**[BLE\_SMP\_EVT\_DISPLAY\_PASSKEY\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)** event is generated.<br />These events are received only when passkey entry method is selected for pairing \(LE secure connections or Legacy pairing\).

## Events generated

This API is called during pairing procedure, please see the events generated of **[BLE\_SMP\_InitiatePairing](GUID-87C9F8A4-1DC1-46A3-80E1-B84D5145BB8D.md)**.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully reply passkey is not available.|
|MBA\_RES\_FAIL|The specified connection handle is invalid.|

