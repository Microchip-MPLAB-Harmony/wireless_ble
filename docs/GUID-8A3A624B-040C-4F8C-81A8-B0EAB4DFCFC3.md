# BT\_SYS\_Init

**Parent topic:**[Functions](GUID-BCD34C15-EAC6-45F0-97B7-E2EBA942CFEE.md)

## C

```c
bool BT_SYS_Init(void *p_reqQueueHandle, void *p_osalApiList, BT_SYS_Option_T *p_option, BT_SYS_Cfg_T *p_cfg);
```

## Description

BT Init Entry.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_reqQueueHandle|OS handle for Bluetooth internal task.|
|\[in\] p\_osalApiList|OSAL api list.|
|\[in\] p\_option|Initialization option.|
|\[in\] p\_cfg|System configuration.|

## Return values

|Return value|Description|
|------------|-----------|
|true|Success to initialize BT.|
|false|Fail to initialize BT.|

