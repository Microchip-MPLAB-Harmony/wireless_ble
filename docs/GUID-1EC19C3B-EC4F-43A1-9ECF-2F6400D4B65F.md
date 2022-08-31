# BLE_SMP_AcceptDebugKey

## C

```c
uint16_t BLE_SMP_AcceptDebugKey();
```

## Description

Accept BLE SMP debug key during pairing procedure.

### Note

If this API is called, then **[BLE_SMP_EnableDebugMode](GUID-3ECEE352-858E-43C7-B2DD-09DC2AC5E665.md)** API is prohibited.

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully set to accept debug key.|
MBA_RES_FAIL|Fail to set to accept debug key due to **[BLE_SMP_EnableDebugMode](GUID-3ECEE352-858E-43C7-B2DD-09DC2AC5E665.md)** API is called.|
