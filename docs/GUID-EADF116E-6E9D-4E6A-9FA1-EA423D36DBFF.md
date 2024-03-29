# BLE\_DD\_DiscSvc\_T

**Parent topic:**[Structures](GUID-B7B198D6-037B-468B-9A14-943F83191073.md)

## C

```c
typedef struct BLE_DD_DiscSvc_T
{
    ATT_Uuid_T              svcUuid;
    BLE_DD_DiscInfo_T       *p_discInfo;
    BLE_DD_DiscChar_T       **p_discChars;
    BLE_DD_CharList_T       *p_charList;
    uint8_t                 discCharsNum;
} BLE_DD_DiscSvc_T;
```

## Description

Service for discovery procedure.

## Field Documentation

|Field|Description|
|-----|-----------|
|svcUuid|UUID of the service to be discovered.|
|p\_discInfo|Discoveried service information.|
|p\_discChars|Characteristic list of the service to be discoverd. Note: only assigned characteristic will be discovered.|
|p\_charList|Structure storing discovered information of the characteristic. Application/profile need to provide the storage.|
|discCharsNum|Number of the characteristics in the list.|

