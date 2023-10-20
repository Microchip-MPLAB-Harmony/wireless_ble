# GATT\_EvtClientFeatureChange\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATT_EvtClientFeatureChange_T
{
    uint16_t    connHandle;
    uint8_t     clientSupportFeature;
} GATT_EvtClientFeatureChange_T;
```

## Description

Data structure for **[GATTS\_EVT\_CLIENT\_FEATURE\_CHANGE](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|clientSupportFeature|Bit field represents client support features. **[GATT client features](GUID-29F82178-858F-42F7-B519-9197AE12D4FE.md)**.|

