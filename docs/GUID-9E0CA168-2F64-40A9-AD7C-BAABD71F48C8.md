# BLE\_GAP\_EvtPathLossThreshold\_T

**Parent topic:**[Structures](GUID-A15AC144-CD72-427A-B096-33FC1E7FEA88.md)

## C

```c
typedef struct BLE_GAP_EvtPathLossThreshold_T
{
    uint16_t                connHandle;
    uint8_t                 currentPathLoss;
    uint8_t                 zoneEntered;
}BLE_GAP_EvtPathLossThreshold_T;
```

## Description

Data structure for **[BLE\_GAP\_EVT\_PATH\_LOSS\_THRESHOLD](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|currentPathLoss|Current path loss \(always zero or positive\).|
|zoneEntered|Zone entered.|

