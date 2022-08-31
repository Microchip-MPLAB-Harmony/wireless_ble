# BLE_GAP_SetAdvData

## C

```c
uint16_t BLE_GAP_SetAdvData(BLE_GAP_AdvDataParams_T *p_advData);
```

## Description

Set advertising data from the input "p_advData".

### Note

If advertising is currently enabled, the new data shall be used in
subsequent advertising events. If an advertising event is in progress 
when this command is issued, the new data may not be used for that event.

This API belongs to legacy advertising feature. It will be disallowed if any one API belongs extended advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE_GAP_AdvInit](GUID-00582C15-26DA-41D8-8125-1FDD13BCF632.md)** is required for this API.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] p_advData|Pointer to the structure containing advertising data to be set.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully set advertising data.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_INVALID_PARA|Invalid parameters, advertisement data length is more than **[BLE_GAP_ADV_DATA_LEN](GUID-4D05CBBF-8E4B-49B8-A1F1-D37781265701.md)**.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed when extended Adv. feature is in-use.|
