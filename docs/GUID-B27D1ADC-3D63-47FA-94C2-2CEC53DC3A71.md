# BLE\_DTM\_StartCwTest

**Parent topic:**[Functions](GUID-347B9612-7A12-4EBB-AAAC-57FB3DD4960D.md)

## C

```c
uint16_t BLE_DTM_StartCwTest(uint8_t txChannel, int8_t txPower);
```

## Description

Start a Continuous Wave \(CW\) test with specific channel and power.

### Note

**[BLE\_DTM\_Init](GUID-11F607B9-DF00-4A77-B31A-5B7F57E1B990.md)** is required for this API.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] txChannel|Channel = \(F – 2402\) \(Frequency Range : 2402 MHz to 2480 MHz\) See **[Channel range of continuous wave test](GUID-B58671F6-428D-41C2-9E52-C8723DD02F0F.md)**.|
|\[in\] txPower|The transmit power level to be used by the transmitter. Range: -127 to +20. Units: dBm.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully start the CW test.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|

