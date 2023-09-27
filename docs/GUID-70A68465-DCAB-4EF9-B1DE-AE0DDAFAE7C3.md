# BLE\_GAP\_CreateSyncCancel

**Parent topic:**[Functions](GUID-0DD261BF-40D6-42CD-8806-9B93D259D1CC.md)

## C

```c
uint16_t BLE_GAP_CreateSyncCancel(void);
```

## Description

Cancel the **[BLE\_GAP\_CreateSync](GUID-19848F3A-AC20-4003-82FC-E7D075207A5F.md)** procedure while it is pending.

### Note

This API belongs to extended advertising feature. It will be disallowed if any one API that belongs to legacy advertising feature is issued before. **[Categories of advertising APIs and events](GUID-6250C306-2D62-4631-A4F9-616BBCCC48AC.md)** for details of advertising APIs.

**[BLE\_GAP\_ScanInit](GUID-EABB24B0-3356-4103-A083-EB3A2F4DF22E.md)**, **[BLE\_GAP\_ExtScanInit](GUID-44D7F81A-66AC-4675-B160-B927F73DDB95.md)** and **[BLE\_GAP\_SyncInit](GUID-9313F520-6EF3-4B78-96C9-3858977D528E.md)** are required for this API.

## Events generated

**[BLE\_GAP\_EVT\_PERI\_ADV\_SYNC\_EST](GUID-ADCFB5AA-F06E-4ED9-9227-592A5CE40F39.md)** Generated with corresponding status code.

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully cancel the **[BLE\_GAP\_CreateSync](GUID-19848F3A-AC20-4003-82FC-E7D075207A5F.md)** procedure.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

