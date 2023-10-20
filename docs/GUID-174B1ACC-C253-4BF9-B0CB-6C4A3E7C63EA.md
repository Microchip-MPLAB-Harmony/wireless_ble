# BLE\_GAP\_ReadTxPowerLevel

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_ReadTxPowerLevel(uint16_t connHandle, uint8_t phy, int8_t *p_txPower, int8_t *p_txPowerMax);
```

## Description

Read the current and maximum transmit power value of specific LE connection handle. \(unit: dBm\)

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] phy|PHY. See **[Transmitter PHY type](GUID-CA9CC4D6-A5FB-4168-9A8C-4A940EA04D2B.md)**.|
|\[out\] p\_txPower|Pointer to current transmit power level. \(Unit: dBm\)|
|\[out\] p\_txPowerMax|Pointer to maximum transmit power level. \(Unit: dBm\)|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully get the transmit power level.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

