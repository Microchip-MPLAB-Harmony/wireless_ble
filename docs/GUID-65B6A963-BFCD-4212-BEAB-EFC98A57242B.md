# BLE\_DD\_BleEventHandler

**Parent topic:**[Functions](GUID-1FCC6916-764B-44EA-9DC2-BBC5DE39C89C.md)

## C

```c
void BLE_DD_BleEventHandler(BLE_DD_Config_T *p_config, STACK_Event_T *p_stackEvent);
```

## Description

Handle BLE events.

### Note

This function should be called for every BLE event.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_config|Pointer to the **[BLE\_DD\_Config\_T](GUID-0CEBE01F-6108-4607-856C-EE7D58219502.md)** structure buffer.|
|\[in\] p\_stackEvent|Pointer to **[STACK\_Event\_T](GUID-7D4B4619-0088-47EB-9D17-E58AF6F9A92D.md)** structure buffer.|

