# BLE_DD_DiscSvc_T

## C

```c
typedef struct BLE_DD_DiscSvc_T
{
    ATT_Uuid_T              svcUuid;
    BLE_DD_DiscChar_T       **p_discChars;
    BLE_DD_CharList_T       *p_charList;
    uint8_t                 discCharsNum;
} BLE_DD_DiscSvc_T;
```

## Description

Service for discovery procedure.


## Field Documentation

|Field|Description|
|:---|:---|
|svcUuid|UUID of the service to be discovered.|
|p_discChars|Characteristic list of the service to be discoverd. Note: only assigned characteristic will be discovered.|
|p_charList|Structure storing discovered information of the characteristic. Application/profile need to provide the storage.|
|discCharsNum|Number of the characteristics in the list.|
