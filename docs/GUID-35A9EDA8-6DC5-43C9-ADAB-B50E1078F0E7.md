# BLE\_GAP\_SetAdvData

**Parent topic:**[Functions](GUID-0DD261BF-40D6-42CD-8806-9B93D259D1CC.md)

## C

```c
uint16_t BLE_GAP_SetAdvData(BLE_GAP_AdvDataParams_T *p_advData);
```

## Description

Set advertising data from the input "p\_advData".

### Note

If advertising is currently enabled, the new data shall be used in<br />subsequent advertising events. If an advertising event is in progress<br />when this command is issued, the new data may not be used for that event.

This API belongs to legacy advertising feature. It will be disallowed if any one API belongs extended advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE\_GAP\_AdvInit](GUID-00582C15-26DA-41D8-8125-1FDD13BCF632.md)** is required for this API.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_advData|Pointer to the structure containing advertising data to be set.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set advertising data.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters, advertisement data length is more than **[BLE\_GAP\_ADV\_DATA\_LEN](GUID-4D05CBBF-8E4B-49B8-A1F1-D37781265701.md)**.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed when extended Adv. feature is in-use.|

