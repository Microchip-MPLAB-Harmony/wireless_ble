# BLE_ANPS_SetSuppUnreadCat

## C

```c
uint16_t BLE_ANPS_SetSuppUnreadCat(uint16_t catMask);
```

## Description

Set unread alert category.


## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] catMask|Category of unread alert are supported. See **[Support category mask](GUID-A55FDA30-DEB1-47D2-8921-78636268C680.md)**.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully set supported unread alert category.|
MBA_RES_FAIL|Supported unread alert category shall not change while in a connection.|
MBA_RES_INVALID_PARA|Parameter does not meet the spec.|
