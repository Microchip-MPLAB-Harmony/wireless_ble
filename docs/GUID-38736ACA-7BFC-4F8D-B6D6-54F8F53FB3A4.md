# BLE\_TRSPC\_EvtVendorCmd\_T

**Parent topic:**[Structures](GUID-E036F038-9556-4640-A277-570194EFBEC6.md)

## C

```c
typedef struct BLE_TRSPC_EvtVendorCmd_T
{
    uint16_t        connHandle;
    uint8_t         payloadLength;
    uint8_t         *p_payLoad;
}   BLE_TRSPC_EvtVendorCmd_T;
```

## Description

Data structure for **[BLE\_TRSPC\_EVT\_VENDOR\_CMD](GUID-629BEAFF-0BDD-4521-8E20-F81D3FA47153.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|payloadLength|Length of payload.|
|p\_payLoad|Vendor command payload pointer.|

