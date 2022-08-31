# BLE_GAP_CreateSyncCancel

## C

```c
uint16_t BLE_GAP_CreateSyncCancel();
```

## Description

Cancel the **[BLE_GAP_CreateSync](GUID-19848F3A-AC20-4003-82FC-E7D075207A5F.md)** procedure while it is pending.

### Note

This API belongs to extended advertising feature. It will be disallowed if any one API that belongs to legacy advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE_GAP_ScanInit](GUID-EABB24B0-3356-4103-A083-EB3A2F4DF22E.md)**, **[BLE_GAP_ExtScanInit](GUID-44D7F81A-66AC-4675-B160-B927F73DDB95.md)** and **[BLE_GAP_SyncInit](GUID-9313F520-6EF3-4B78-96C9-3858977D528E.md)** are required for this API.

## Events generated

**[BLE_GAP_EVT_PERI_ADV_SYNC_EST](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated with corresponding status code.

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully cancel the **[BLE_GAP_CreateSync](GUID-19848F3A-AC20-4003-82FC-E7D075207A5F.md)** procedure.|
MBA_RES_OOM|Internal memory allocation failure.|
MBA_RES_COMMAND_DISALLOWED|Command disallowed.|
