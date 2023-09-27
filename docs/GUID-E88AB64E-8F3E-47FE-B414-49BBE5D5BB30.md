# BLE\_GAP\_ReadPhy

**Parent topic:**[Functions](GUID-0DD261BF-40D6-42CD-8806-9B93D259D1CC.md)

## C

```c
uint16_t BLE_GAP_ReadPhy(uint16_t connHandle, uint8_t *p_txPhy, uint8_t *p_rxPhy);
```

## Description

Read the current transmitter PHY and receiver PHY on the specified connection

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_txPhy|Pointer to TX PHY buffer. See **[PHY type](GUID-3539A8AA-7029-4450-B077-5B732D664B49.md)** for the definition.|
|\[in\] p\_rxPhy|Pointer to RX PHY buffer. See **[PHY type](GUID-3539A8AA-7029-4450-B077-5B732D664B49.md)** for the definition.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully read the current transmitter PHY and receiver PHY.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_UNKNOWN\_CONN\_ID|Unknown connection identifier.|

