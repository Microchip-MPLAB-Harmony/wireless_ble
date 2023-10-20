# BLE\_DM\_GetPairedDevice

**Parent topic:**[Functions](GUID-6AC7354D-DE77-48C5-8724-3DCC98A65C57.md)

## C

```c
uint16_t BLE_DM_GetPairedDevice(uint8_t devId, BLE_DM_PairedDevInfo_T *p_pairedDevInfo);
```

## Description

Get the single paired device information via device ID.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] devId|The device ID. Should be in the range of 0 ~ \(**[BLE\_DM\_MAX\_PAIRED\_DEVICE\_NUM](GUID-9F845AE9-F918-4003-B311-E85F343C77CF.md)** - 1\).|
|\[out\] p\_pairedDevInfo|Pointer to **[BLE\_DM\_PairedDevInfo\_T](GUID-6C9F0A19-A980-4B58-BA29-F482CDEE6AE1.md)** structure buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully get single paired devices information.|
|MBA\_RES\_INVALID\_PARA|DevId exceeds the maximum range or the number of paired devices.|
|MBA\_RES\_FAIL|The get operation failure.|

