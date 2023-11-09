# BLE\_TRSPS\_EvtTxStatus\_T

**Parent topic:**[Structures](GUID-CF652FF4-6E48-4AFE-98C2-AF0B4F1E2DFE.md)

## C

```c
typedef struct BLE_TRSPS_EvtTxStatus_T
{
    uint16_t         connHandle;
    uint8_t          status;
}   BLE_TRSPS_EvtTxStatus_T;
```

## Description

Data structure for **[BLE\_TRSPS\_EVT\_TX\_STATUS](GUID-7F1C8247-F6C3-4DAB-A8F7-B2AD44B52177.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|status|Connection status. See **[TRSPS Status](GUID-75F1611D-3EF8-4ABD-8843-67860CC21002.md)**.|

