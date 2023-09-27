# BLE\_SMP\_AcceptDebugKey

**Parent topic:**[Functions](GUID-9CB8F27C-7921-4D62-8EBB-A41DD9F9D4D4.md)

## C

```c
uint16_t BLE_SMP_AcceptDebugKey();
```

## Description

Accept BLE SMP debug key during pairing procedure.

### Note

If this API is called, then **[BLE\_SMP\_EnableDebugMode](GUID-3ECEE352-858E-43C7-B2DD-09DC2AC5E665.md)** API is prohibited.

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set to accept debug key.|
|MBA\_RES\_FAIL|Fail to set to accept debug key due to **[BLE\_SMP\_EnableDebugMode](GUID-3ECEE352-858E-43C7-B2DD-09DC2AC5E665.md)** API is called.|

