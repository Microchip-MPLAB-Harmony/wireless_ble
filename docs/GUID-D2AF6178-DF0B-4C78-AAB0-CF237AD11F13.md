# BLE\_GAP\_ReadFactoryTxPowerLevel

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_ReadFactoryTxPowerLevel(int8_t *p_minTxPower, int8_t *p_maxTxPower);
```

## Description

Read the maximum and minimum factory RF transmit power level.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[out\] p\_minTxPower|Point to the minimum transmit power level. \(Unit: dBm\)|
|\[out\] p\_maxTxPower|Point to the maximum transmit power level. \(Unit: dBm\)|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue the request.|
|MBA\_RES\_OOM|Internal memory allocation failure.|

