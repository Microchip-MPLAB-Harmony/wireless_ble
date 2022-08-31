# BLE_GAP_RemoveExtAdvSet

## C

```c
uint16_t BLE_GAP_RemoveExtAdvSet(uint8_t advHandle);
```

## Description

Remove an advertising set.

### Note

This API belongs to extended advertising feature. It will be disallowed if any one API that belongs to legacy advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE_GAP_AdvInit](GUID-00582C15-26DA-41D8-8125-1FDD13BCF632.md)** and **[BLE_GAP_ExtAdvInit](GUID-D2DBC15F-67D6-431E-9D69-DAE11D195641.md)** are required for this API.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] advHandle|Advertising handle.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully remove an advertising set.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_INVALID_PARA|Invalid parameters.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed.|
MBA_RES_UNKNOWN_ADV_ID|Unknown advertising identifier.|
