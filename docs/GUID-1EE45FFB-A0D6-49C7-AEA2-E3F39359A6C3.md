# BLE\_GAP\_UpdateConnParam

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_UpdateConnParam(uint16_t connHandle, BLE_GAP_ConnParams_T *p_connParams);
```

## Description

Update connection parameters of an existing connection.

## Events generated

**[BLE\_GAP\_EVT\_CONN\_PARAM\_UPDATE](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** Generated when connection parameters update complete.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_connParams|Pointer to the **[BLE\_GAP\_ConnParams\_T](GUID-34487CE2-9BA8-45AF-A32B-2D8849CF682C.md)** structure buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully update the connection parameters.|
|MBA\_RES\_FAIL|Connection update is in progress.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters, if any of the connection update parameters are out of range. See **[Connection parameters range](GUID-A0E7191A-5AF6-44B4-9E1B-19D74354392D.md)** for range\). \(OR\) Connection with the specified handle does not exist.|
|MBA\_RES\_UNSUPPORT\_REMOTE\_FEATURE|Unsupported remote feature.|

