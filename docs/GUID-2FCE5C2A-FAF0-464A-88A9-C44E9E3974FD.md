# BLE\_GAP\_ScanFilterServiceData\_T

**Parent topic:**[Structures](GUID-230368B0-FB2A-4967-A471-691387B35A9E.md)

## C

```c
typedef struct
{
    uint8_t                 uuidLen;
    uint8_t                 uuidData[16];
    uint8_t                 svcDataLen;
    uint8_t                 svcData[BLE_GAP_SCAN_SERVICE_DATA_MAX_LEN];
} BLE_GAP_ScanFilterServiceData_T;
```

## Description

Service data filter parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|uuidLen|Byte length of Service UUID. There are 3 Service UUID format definitions. The valid UUID length is 2, 4 or 16 for 16-bit, 32-bit or 128-bit Service UUID.|
|uuidData|Contents of Compared Service UUID.|
|svcDataLen|Length of compared Service Data. The max value is **[BLE\_GAP\_SCAN\_SERVICE\_DATA\_MAX\_LEN](GUID-EABF91D3-8F80-4F9B-9A82-64AED3AC1DA0.md)**.|
|svcData|Contents of Compared Service Data.|

