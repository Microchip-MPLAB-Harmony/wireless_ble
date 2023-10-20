# GATTS\_HandleValueParams\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATTS_HandleValueParams_T
{
    uint16_t    charHandle;
    uint16_t    charLength;
    uint8_t     charValue[BLE_ATT_MAX_MTU_LEN - ATT_HANDLE_VALUE_HEADER_SIZE];
    uint8_t     sendType;
}   GATTS_HandleValueParams_T;
```

## Description

GATT Handle Value Notification/Indication parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|charHandle|Handle of the characteristic to be notified/indicated.|
|charLength|Length of the characteristic to be notified/indicated. Max value limited to size of charValue.|
|charValue|Characteristic value to be notified/indicated.|
|sendType|Flag indicating Notification or Indication. See **[ATT opcodes](GUID-0D831528-B2BE-42F9-9185-11F8F17DC4E1.md)**. Only ATT\_HANDLE\_VALUE\_NTF/ATT\_HANDLE\_VALUE\_IND are allowed.|

