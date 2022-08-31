# BLE_SCM_EvtServiceChange_T

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

Data structure for **[BLE_SCM_EVT_SVC_CHANGE](GUID-CFF6FA0B-8B74-4311-AA62-C04061423A98.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|Connection handle associated with this connection.|
|scStartHandle|Start attribute handle of service change at remote.|
|scEndHandle|End attribute handle of service change at remote.|
