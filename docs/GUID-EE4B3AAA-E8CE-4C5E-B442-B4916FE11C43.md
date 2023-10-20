# BLE\_SMP\_AcceptDebugKey

**Parent topic:**[Functions](GUID-B4A018ED-CB34-4D52-A7F9-3E7808C43BF8.md)

## C

```c
uint16_t BLE_SMP_AcceptDebugKey();
```

## Description

Accept BLE SMP debug key during pairing procedure.

### Note

If this API is called, then **[BLE\_SMP\_EnableDebugMode](GUID-7DE416F7-2A25-4611-A1D6-49C2E62E5496.md)** API is prohibited.

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set to accept debug key.|
|MBA\_RES\_FAIL|Fail to set to accept debug key due to **[BLE\_SMP\_EnableDebugMode](GUID-7DE416F7-2A25-4611-A1D6-49C2E62E5496.md)** API is called.|

