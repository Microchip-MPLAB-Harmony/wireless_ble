# BLE\_GAP\_EvtEncryptStatus\_T

**Parent topic:**[Structures](GUID-A15AC144-CD72-427A-B096-33FC1E7FEA88.md)

## C

```c
typedef struct BLE_GAP_EvtEncryptStatus_T
{
    uint16_t                connHandle;
    uint8_t                 status;
} BLE_GAP_EvtEncryptStatus_T;
```

## Description

Data structure for **[BLE\_GAP\_EVT\_ENCRYPT\_STATUS](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|status|Encryption status. See **[Encryption status](GUID-8AF80EE7-43B9-4ED9-9A89-8DDCAD43D0F5.md)** for possible values.|

