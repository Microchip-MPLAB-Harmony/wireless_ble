# BLE\_GAP\_SyncSetDev\_T

**Parent topic:**[Structures](GUID-A15AC144-CD72-427A-B096-33FC1E7FEA88.md)

## C

```c
typedef struct BLE_GAP_SyncSetDev_T
{
    BLE_GAP_Addr_T          advAddr;
    uint8_t                 advSid;
}BLE_GAP_SyncSetDev_T;
```

## Description

Set sync device parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|advAddr|Advertiser address type. Public or random \(static\) identity address.|
|advSid|Advertising SID. It used to identify the Periodic Advertising. See **[Advertising SID range](GUID-AD7FCE19-B510-4A42-B223-E0EAE1950AE8.md)**.|

