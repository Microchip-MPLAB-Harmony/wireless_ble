# BLE\_DM\_GetFilterAcceptList

**Parent topic:**[Functions](GUID-6AC7354D-DE77-48C5-8724-3DCC98A65C57.md)

## C

```c
uint16_t BLE_DM_GetFilterAcceptList(uint8_t *p_devCnt, BLE_GAP_Addr_T *p_addr);
```

## Description

Get filter accept list.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[out\] p\_devCnt|The number of devices in filter accept list.|
|\[out\] p\_addr|Pointer to the **[BLE\_GAP\_Addr\_T](GUID-39F32807-8A6F-4AC8-93AD-2AA3B1C75F46.md)** structure buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully get filter accept list.|

