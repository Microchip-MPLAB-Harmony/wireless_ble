# BT_SYS_Init

## C

```c
bool BT_SYS_Init(void *p_reqQueueHandle, void *p_osalApiList, BT_SYS_Option_T *p_option, BT_SYS_Cfg_T *p_cfg);
```

## Description

BT Init Entry

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] p_reqQueueHandle|OS handle for Bluetooth internal task.|
|\[in\] p_osalApiList|OSAL api list.|
|\[in\] p_option|Initialization option.|
|\[in\] p_cfg|System configuration.|

## Return values

|Return value|Description|
|:---|:---|
true|Success to initialize BT.|
false|Fail to initialize BT.|
