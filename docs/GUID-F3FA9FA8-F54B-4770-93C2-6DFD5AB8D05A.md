# BLE_GAP_ExtAdvDataParams_T

## C

```c
typedef struct BLE_GAP_ExtAdvDataParams_T
{
    uint8_t                 advHandle;
    uint8_t                 operation;
    uint8_t                 fragPreference;
    uint8_t                 advLen;
    uint8_t                 *p_advData;
} BLE_GAP_ExtAdvDataParams_T;
```

## Description

Extended advertising parameters.


## Field Documentation

|Field|Description|
|:---|:---|
|advHandle|Identifies the advertising set whose parameters are being configured                                                                         (Range: 0 to 0xEF). See **[Maximum advertising handle](GUID-1E8B41D5-7783-46F2-AD0F-B5013B9B85E3.md)**|
|operation|Set the advertising data in one or more operations. See **[Operation of extended advertising data](GUID-7E060534-5AC3-4F33-8830-C410F4B8F2CD.md)**                                                                         BLE_GAP_EXT_ADV_DATA_OP_UNCHANGED can't be used in BLE_GAP_SetExtScanRspData|
|fragPreference|Configure whether advertising data should be fragmented. See **[Fragment preference of extended advertising data](GUID-2F260089-BD42-42F8-A2E0-94CAC5BA6A7C.md)**|
|advLen|Length of advertising data, valid value is 0 ~ 251.|
|p_advData|Advertising data|
