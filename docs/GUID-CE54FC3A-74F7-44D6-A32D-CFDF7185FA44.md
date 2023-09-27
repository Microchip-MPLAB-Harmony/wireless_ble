# BLE\_DTM\_EndTest

**Parent topic:**[Functions](GUID-965015D2-EDE8-4792-85DF-17B7B0E47E36.md)

## C

```c
uint16_t BLE_DTM_EndTest(uint16_t *p_packetCount, int8_t *p_avgRssi);
```

## Description

Stop any test which is in progress.

### Note

**[BLE\_DTM\_Init](GUID-21714A75-A3D2-41AE-AF6B-7AB79CE96BAE.md)** is required for this API.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[out\] p\_packetCount|Pointer to the packet count parameter buffer. The number of count shall be zero for a transmitter test and shall be received packets for a reveiver test.|
|\[out\] p\_avgRssi|Pointer to the average rssi value buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully stop the test which is in progress.|
|MBA\_RES\_OOM|Internal memory allocation failure.|

