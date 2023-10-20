# BLE\_GAP\_EncInfoReqReply

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_EncInfoReqReply(uint16_t connHandle, uint8_t *p_ltk);
```

## Description

Reply to an LE encrypt information request event.<br />This API is called to reply **[BLE\_GAP\_EVT\_ENC\_INFO\_REQUEST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** event.

### Note

This API shall be called only if device is peripheral role.

## Events generated

**[BLE\_GAP\_EVT\_ENCRYPT\_STATUS](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** Generated when encrypted status update.

**[BLE\_GAP\_EVT\_DISCONNECTED](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** Generated when encryption information is wrong.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_ltk|Pointer to 128 bit long term key \(ltk\).|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue a reply of LE encrypt information request event.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

