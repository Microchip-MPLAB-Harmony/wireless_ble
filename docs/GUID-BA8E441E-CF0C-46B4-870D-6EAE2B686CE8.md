# BLE\_GAP\_WriteAuthPayloadTimeout

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

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
|\[in\] authPayloadTo|Authenticated payload timeout parameter \(Unit: 10 ms\). The authPayloadTo shall be equal to or greater than connInterval\*\(1 + peripheralLatency\). Refer to **[BLE\_GAP\_EvtConnect\_T](GUID-930F7421-6023-4E98-980F-20CDC5D99566.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully write the authenticated payload timeout parameter.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_UNKNOWN\_CONN\_ID|Unknown connection identifier.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

