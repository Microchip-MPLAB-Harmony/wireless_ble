# BLE\_SMP\_AcceptPairingRequest

**Parent topic:**[Functions](GUID-B4A018ED-CB34-4D52-A7F9-3E7808C43BF8.md)

## C

```c
uint16_t BLE_SMP_AcceptPairingRequest(uint16_t connHandle);
```

## Description

Accept the pairing request.

### Note

This API should be called to continue pairing procedure when receiving **[BLE\_SMP\_EVT\_PAIRING\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.

## Events generated

This API is called during pairing procedure, please see the events generated of **[BLE\_SMP\_InitiatePairing](GUID-87C9F8A4-1DC1-46A3-80E1-B84D5145BB8D.md)**.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue pairing response with accept parameter.|
|MBA\_RES\_INVALID\_PARA|The parameter is invalid.|

