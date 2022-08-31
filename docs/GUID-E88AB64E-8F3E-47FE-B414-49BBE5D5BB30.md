# BLE_GAP_ReadPhy

## C

```c
uint16_t BLE_GAP_ReadPhy(uint16_t connHandle, uint8_t *p_txPhy, uint8_t *p_rxPhy);
```

## Description

Read the current transmitter PHY and receiver PHY on the specified connection

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p_txPhy|Pointer to TX PHY buffer. See **[PHY type](GUID-3539A8AA-7029-4450-B077-5B732D664B49.md)** for the definition.|
|\[in\] p_rxPhy|Pointer to RX PHY buffer. See **[PHY type](GUID-3539A8AA-7029-4450-B077-5B732D664B49.md)** for the definition.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully read the current transmitter PHY and receiver PHY.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_UNKNOWN_CONN_ID|Unknown connection identifier.|
