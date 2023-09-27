# BLE\_SMP\_PasskeyReply

**Parent topic:**[Functions](GUID-9CB8F27C-7921-4D62-8EBB-A41DD9F9D4D4.md)

## C

```c
uint16_t BLE_SMP_PasskeyReply(uint16_t connHandle, uint8_t *p_passkey);
```

## Description

Reply to a passkey request event with passkey information.

### Note

The API should be called in response to passkey entry request or passkey display request.<br />Hence, it should be called in SMP callback function when **[BLE\_SMP\_EVT\_INPUT\_PASSKEY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)** event or<br />**[BLE\_SMP\_EVT\_DISPLAY\_PASSKEY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)** event is generated.<br />These events are received only when passkey entry method is selected for pairing \(LE secure connections or Legacy pairing\).

## Events generated

This API is called during pairing procedure, please see the events generated of **[BLE\_SMP\_InitiatePairing](GUID-8F2CEAE0-3C28-4A9B-8A69-166F2E5F2B67.md)**.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_passkey|Pointer to the passkey buffer. The length of passkey should be 6 bytes in ASCII format.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully reply with passkey for pairing process.|
|MBA\_RES\_FAIL|The specified connection handle is invalid.|

