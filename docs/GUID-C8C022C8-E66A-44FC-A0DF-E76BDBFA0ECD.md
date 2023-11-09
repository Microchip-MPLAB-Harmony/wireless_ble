# BLE\_SCM\_Init

**Parent topic:**[Functions](GUID-0F246B53-0B75-4B62-B20D-872C3E430FA8.md)

## C

```c
uint16_t BLE_SCM_Init(void);
```

## Description

Initialize BLE\_SCM module.

### Note

This module is required only when GATT client is supported and service change status needs to be taken care at remote.

This module required BLE\_DD supported.

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully initialize BLE SCM module.|
|MBA\_RES\_FAIL|Fail initialize BLE SCM module.|

