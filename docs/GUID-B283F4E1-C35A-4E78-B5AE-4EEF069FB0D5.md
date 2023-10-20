# BLE\_PXPM\_ReadTpsTxPowerLevel

**Parent topic:**[Functions](GUID-B1B3B94F-2A47-4042-83D2-C565B5E5F44B.md)

## C

```c
uint16_t BLE_PXPM_ReadTpsTxPowerLevel(uint16_t connHandle);
```

## Description

Read Tx Power Level value from Tx Power Service table of peer PXP Reporter device.

Application must call this API after starting BLE PXP Monitor procedure.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|The connection handle want to be set.|

