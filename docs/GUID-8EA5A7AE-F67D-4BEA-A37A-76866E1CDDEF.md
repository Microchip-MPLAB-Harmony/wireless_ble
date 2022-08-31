# BLE_GAP_EnableEncryption

## C

```c
uint16_t BLE_GAP_EnableEncryption(uint16_t connHandle, uint8_t *p_random, uint8_t *p_ediv, uint8_t *p_ltk);
```

## Description

Authenticate the given encryption key associated with the remote device specified by the connection,
and once authenticated will encrypt the connection.

### Note

This command shall only be used when the local devices role is Central.

## Events generated

**[BLE_GAP_EVT_ENCRYPT_STATUS](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated when encrypted status update. 

**[BLE_GAP_EVT_DISCONNECTED](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated when encryption fail.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p_random|Pointer to 64 bit random number.|
|\[in\] p_ediv|Pointer to 16 bit encrypted diversifier.|
|\[in\] p_ltk|Pointer to 128 bit long term key (ltk).|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully started.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_INVALID_PARA|Invalid parameters.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed.|
