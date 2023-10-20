# BLE\_GAP\_ReadChannelMap

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_ReadChannelMap(uint16_t connHandle, uint8_t *p_map);
```

## Description

Get the channel map for the specific connection.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[out\] p\_map|5-byte channel map array containing 37 1-bit fields corresponding to 37 data channels. 0 indicates corresponding channel is bad and 1 indicates channel status is unknown. The most significant bits are reserved and shall be set to 0.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully read the channel map.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_UNKNOWN\_CONN\_ID|Unknown connection identifier.|

