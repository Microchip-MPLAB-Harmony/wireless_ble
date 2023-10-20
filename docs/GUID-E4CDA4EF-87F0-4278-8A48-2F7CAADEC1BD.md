# BLE\_DM\_SetResolvingList

**Parent topic:**[Functions](GUID-6AC7354D-DE77-48C5-8724-3DCC98A65C57.md)

## C

```c
uint16_t BLE_DM_SetResolvingList(uint8_t devCnt, uint8_t const *p_devId, uint8_t const * p_privacyMode);
```

## Description

Set resolving list.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] devCnt|The number of paired device to add to resolving list. The number must be less than or equel to **[BLE\_DM\_MAX\_RESOLVING\_LIST\_NUM](GUID-7232FC14-AEFC-4A64-99AC-9C79078F11EB.md)**. Pass zero to clear resolving list.|
|\[in\] p\_devId|The paired device list to add to resolving list. Pass NULL to clear resolving list.|

-   @param p\_privacyMode; The privacy mode list \(see **[Privacy mode](GUID-D48255CA-7475-4040-8269-C535B60EE906.md)**.\). Each entry in the array maps to the entry in p\_devId at the same index. Pass NULL to clear resolving list.


## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set or clear resolving list.|
|MBA\_RES\_INVALID\_PARA|The devCnt is greater than **[BLE\_DM\_MAX\_RESOLVING\_LIST\_NUM](GUID-7232FC14-AEFC-4A64-99AC-9C79078F11EB.md)**.|

