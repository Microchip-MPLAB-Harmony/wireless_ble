# BLE\_GAP\_WriteAuthPayloadTimeout

**Parent topic:**[Functions](GUID-0DD261BF-40D6-42CD-8806-9B93D259D1CC.md)

## C

```c
uint16_t BLE_GAP_WriteAuthPayloadTimeout(uint16_t connHandle, uint16_t authPayloadTo);
```

## Description

Write authenticated payload timeout parameter.

### Note

This configuration determine when to use the LE ping sequence.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] authPayloadTo|Authenticated payload timeout parameter \(Unit: 10 ms\). The authPayloadTo shall be equal to or greater than connInterval\*\(1 + peripheralLatency\). Refer to **[BLE\_GAP\_EvtConnect\_T](GUID-4C985581-7FBE-48BC-A815-1178C3173F64.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully write the authenticated payload timeout parameter.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_UNKNOWN\_CONN\_ID|Unknown connection identifier.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

