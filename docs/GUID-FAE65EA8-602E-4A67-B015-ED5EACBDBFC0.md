# GATTS\_AddService

**Parent topic:**[Functions](GUID-AA412A66-C329-47A0-BB6A-362B8F7A62FE.md)

## C

```c
uint16_t GATTS_AddService(GATTS_Service_T *p_service, uint8_t numAttributes);
```

## Description

This API registers service to GATT server.

### Note

Note that the start handle of the registering service should be greater than **[GATTS\_APP\_SVC\_START\_HDL](GUID-6FD01CF9-9D7C-4157-AB4F-70FE7B99D7F8.md)**.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_service|Pointer to the registering service information.|
|\[in\] numAttributes|Number of the attributes in the registering service.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully registering the service.|
|MBA\_RES\_FAIL|Fail to register the service. The assigned attribute handles in the service conflict or the start handle of the service is smaller than **[GATTS\_APP\_SVC\_START\_HDL](GUID-6FD01CF9-9D7C-4157-AB4F-70FE7B99D7F8.md)**.|

