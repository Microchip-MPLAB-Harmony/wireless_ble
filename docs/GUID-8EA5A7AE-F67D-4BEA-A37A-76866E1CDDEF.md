# BLE\_GAP\_EnableEncryption

**Parent topic:**[Functions](GUID-0DD261BF-40D6-42CD-8806-9B93D259D1CC.md)

## C

```c
uint16_t BLE_GAP_EnableEncryption(uint16_t connHandle, uint8_t *p_random, uint8_t *p_ediv, uint8_t *p_ltk);
```

## Description

Authenticate the given encryption key associated with the remote device specified by the connection,<br />and once authenticated will encrypt the connection.

### Note

This command shall only be used when the local devices role is Central.

## Events generated

**[BLE\_GAP\_EVT\_ENCRYPT\_STATUS](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated when encrypted status update.

**[BLE\_GAP\_EVT\_DISCONNECTED](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated when encryption fail.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_random|Pointer to 64 bit random number.|
|\[in\] p\_ediv|Pointer to 16 bit encrypted diversifier.|
|\[in\] p\_ltk|Pointer to 128 bit long term key \(ltk\).|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully started.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

