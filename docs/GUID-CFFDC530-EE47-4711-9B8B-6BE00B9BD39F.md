# BLE\_ANPS\_SetSuppUnreadCat

**Parent topic:**[Functions](GUID-37FC87C4-D5D7-4E4F-B857-934E83BC1494.md)

## C

```c
uint16_t BLE_ANPS_SetSuppUnreadCat(uint16_t catMask);
```

## Description

Set unread alert category.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] catMask|Category of unread alert are supported. See **[Support category mask](GUID-A55FDA30-DEB1-47D2-8921-78636268C680.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully set supported unread alert category.|
|MBA\_RES\_FAIL|Supported unread alert category shall not change while in a connection.|
|MBA\_RES\_INVALID\_PARA|Parameter does not meet the spec.|

