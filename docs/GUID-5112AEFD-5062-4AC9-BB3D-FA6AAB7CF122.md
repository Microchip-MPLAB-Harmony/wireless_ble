# BLE\_DM\_EventRegister

**Parent topic:**[Functions](GUID-6AC7354D-DE77-48C5-8724-3DCC98A65C57.md)

## C

```c
uint16_t BLE_DM_EventRegister(BLE_DM_EventCb_T eventCb);
```

## Description

Register BLE\_DM callback.

### Note

This API should be called in the application initialization routine.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] eventCb|Client callback function.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully register BLE\_DM callback function.|
|MBA\_RES\_NO\_RESOURCE|Fail to register DM callback funcation.|

