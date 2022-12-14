# BLE_GAP_SetExtScanningParams

## C

```c
uint16_t BLE_GAP_SetExtScanningParams(uint8_t filterPolicy, BLE_GAP_ExtScanningPhy_T *p_extScanPhy);
```

## Description

Set the extended scan parameters to be used on the advertising physical channels.

### Note

This API belongs to extended advertising feature. It will be disallowed if any one API that belongs to legacy advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE_GAP_ScanInit](GUID-EABB24B0-3356-4103-A083-EB3A2F4DF22E.md)** and **[BLE_GAP_ExtScanInit](GUID-44D7F81A-66AC-4675-B160-B927F73DDB95.md)** are required for this API.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] filterPolicy|Scanning filter policy. See **[Scan filter policy](GUID-5F91B9BC-EF22-411B-9A8A-549CC564550C.md)**.|
|\[in\] p_extScanPhy|Pointer to **[BLE_GAP_ExtScanningPhy_T](GUID-FE86BC3A-2103-4744-B10C-F6928B891D6F.md)** structure buffer.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully set the extended scan parameters.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_INVALID_PARA|Invalid parameters.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed.|
