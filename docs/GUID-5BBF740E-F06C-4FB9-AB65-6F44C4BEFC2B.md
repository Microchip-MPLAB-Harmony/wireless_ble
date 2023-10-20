# BLE\_DD\_DiscChar\_T

**Parent topic:**[Structures](GUID-CC1B3141-5BBC-438E-A918-B7BD42C2F3CA.md)

## C

```c
typedef struct BLE_DD_DiscChar_T
{
    ATT_Uuid_T const        *p_uuid;
    uint8_t                 settings;
} BLE_DD_DiscChar_T;
```

## Description

Characteristic for discovery procedure.

## Field Documentation

|Field|Description|
|-----|-----------|
|p\_uuid|Pointer to UUID.|
|settings|Characteristic discovery settings.|

