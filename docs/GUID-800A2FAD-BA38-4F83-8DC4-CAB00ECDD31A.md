# BLE\_DM\_PairedDevInfo\_T

**Parent topic:**[Structures](GUID-E5E92890-D6CE-4B37-AD56-B4D84D746B1B.md)

## C

```c
typedef struct BLE_DM_PairedDevInfo_T
{
    BLE_GAP_Addr_T                  remoteAddr;
    uint8_t                         remoteIrk[16];
    BLE_GAP_Addr_T                  localAddr;
    uint8_t                         localIrk[16];
    uint8_t                         rv[8];
    uint8_t                         ediv[2];
    uint8_t                         ltk[16];
    uint8_t                         lesc:1;
    uint8_t                         auth:1;
    uint8_t                         encryptKeySize:6;
}BLE_DM_PairedDevInfo_T;
```

## Description

The structure contains information about paired device information.

## Field Documentation

|Field|Description|
|-----|-----------|
|remoteAddr|Paired device bluetooth address.|
|remoteIrk|Paired device BLE identity resolving key.|
|localAddr|Local device bluetooth address.|
|localIrk|Local device BLE identity resolving key.|
|rv|Paired device BLE rand value.|
|ediv|Paired device BLE encrypted diversifier.|
|ltk|Paired device BLE Link key.|
|lesc|Paired device using LE secure connection.|
|auth|Paired device using authenticated pairing method.|
|encryptKeySize|Paired device BLE encrpytion key size.|

