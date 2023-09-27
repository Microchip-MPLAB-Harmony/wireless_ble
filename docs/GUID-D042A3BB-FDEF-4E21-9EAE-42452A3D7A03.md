# BLE\_ANCS\_DecodeAppAttrs\_T

**Parent topic:**[Structures](GUID-A2656700-B0A1-443C-903C-42AE1A0A1AD8.md)

## C

```c
typedef struct
{
    uint8_t                     appId[BLE_ANCS_MAX_APPID_LEN];
    uint8_t                     displayName[BLE_ANCS_MAX_APP_NAME_LEN];
} BLE_ANCS_DecodeAppAttrs_T;
```

## Description

The structure contains iOS app attributes decoded information.

## Field Documentation

|Field|Description|
|-----|-----------|
|appId|App Identifier.|
|displayName|Display Name.|

