# BLE\_SCM\_EvtServiceChange\_T

**Parent topic:**[Structures](GUID-3783C6B6-D61B-4B5A-B558-E749DFE943E2.md)

## C

```c
typedef struct BLE_SCM_EvtServiceChange_T
{
    uint16_t                    connHandle;
    uint16_t                    scStartHandle;
    uint16_t                    scEndHandle;
} BLE_SCM_EvtServiceChange_T;
```

## Description

Data structure for **[BLE\_SCM\_EVT\_SVC\_CHANGE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|scStartHandle|Start attribute handle of service change at remote.|
|scEndHandle|End attribute handle of service change at remote.|

