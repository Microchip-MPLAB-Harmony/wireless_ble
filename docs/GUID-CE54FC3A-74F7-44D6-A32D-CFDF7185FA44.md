# BLE_DTM_EndTest

## C

```c
uint16_t BLE_DTM_EndTest(uint16_t *p_packetCount, int8_t *p_avgRssi);
```

## Description

Stop any test which is in progress.

### Note

**[BLE_DTM_Init](GUID-21714A75-A3D2-41AE-AF6B-7AB79CE96BAE.md)** is required for this API.

## Parameters

|Parameter|Description|
|:---|:---|
|\[out\] p_packetCount|Pointer to the packet count parameter buffer. The number of count shall be zero for a transmitter test and shall be received packets for a reveiver test.|
|\[out\] p_avgRssi|Pointer to the average rssi value buffer.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully stop the test which is in progress.|
MBA_RES_OOM|Internal memory allocation failure.|
