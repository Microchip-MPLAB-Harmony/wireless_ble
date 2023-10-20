# GATTS\_SendReadByTypeRespParams\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATTS_SendReadByTypeRespParams_T
{
    uint16_t    allPairsLength;
    uint8_t     eachPairLength;
    uint8_t     attrDataList[BLE_ATT_MAX_MTU_LEN - ATT_READ_BY_TYPE_RESP_HEADER_SIZE];
}   GATTS_SendReadByTypeRespParams_T;
```

## Description

GATT Send Read By Type Response parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|allPairsLength|Length of all handle-value pairs.|
|eachPairLength|Length of one handle-value pair. Read By Type Response may contain multiple handle-value pairs of the same length.|
|attrDataList|Handle-value pairs to be sent to Client. The data format should be little endian.|

