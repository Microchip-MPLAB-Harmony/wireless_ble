# BLE\_GAP\_RemoteConnParamsReqNegativeReply

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_RemoteConnParamsReqNegativeReply(uint16_t connHandle, uint8_t reason);
```

## Description

Reply to an LE remote connection parameters request event if device rejects the remote devices request to change connection parameters.

This API is called to reply **[BLE\_GAP\_EVT\_REMOTE\_CONN\_PARAM\_REQUEST](GUID-085D2B3E-E5DB-4072-8916-29201399538E.md)** event.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] reason|The reason of remote connection parameter request negative reply. See **[Status definitions](GUID-B6870242-2E8B-4919-B74D-F61748BF0B4B.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully issue a negative reply of LE remote connection parameters request event.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters.|
|MBA\_RES\_COMMAND\_DISALLOWED|Command disallowed.|

