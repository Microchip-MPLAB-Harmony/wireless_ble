# BLE\_GAP\_SetDeviceAddr

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_SetDeviceAddr(BLE_GAP_Addr_T *p_addr);
```

## Description

Set the Bluetooth device address \(identity address\). Type of address must be **[BLE\_GAP\_ADDR\_TYPE\_PUBLIC](GUID-D3CBB475-83FB-4E95-9E45-9861A41F3EA4.md)** or **[BLE\_GAP\_ADDR\_TYPE\_RANDOM\_STATIC](GUID-D3CBB475-83FB-4E95-9E45-9861A41F3EA4.md)**.<br />The address identifies the device to other Bluetooth devices.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_addr|Pointer to the device address buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set the device address.|
|MBA\_RES\_FAIL|Fail to set device addr. Some types of address can be set by application.|
|MBA\_RES\_OOM|Internal memory allocation failure.|

