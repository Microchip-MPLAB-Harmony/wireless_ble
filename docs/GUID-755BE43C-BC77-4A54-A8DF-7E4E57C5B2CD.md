# BLE\_GAP\_SetDefaultPhy

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_SetDefaultPhy(uint8_t txPhys, uint8_t rxPhys);
```

## Description

Specify the preferred values for the transmitter PHY and receiver PHY to be used for all subsequent connections.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] txPhys|Indicates the transmitter PHYs that the user prefers to use.|
|\[in\] rxPhys|Indicates the receiver PHYs that the user prefers to use.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully specify the preferred values for the transmitter PHY and receiver PHY to be used for all subsequent connections.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
