# BLE\_ANCS\_PerformNtfyAction

**Parent topic:**[Functions](GUID-1986696F-023D-4DD6-86D6-1D32A8DDAA6F.md)

## C

```c
uint16_t BLE_ANCS_PerformNtfyAction(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_ActionId_T actId);
```

## Description

Function for The Perform Notification Action command allows an NC to perform a predetermined action on a specific iOS notification.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|The connection handle.|
|\[in\] ntfyId|A 32-bit numerical value representing the UID of the iOS notification on which the client wants to perform an action. The ntfyId could be retrieved when receiving **[BLE\_ANCS\_EVT\_NTFY\_ADDED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)** or **[BLE\_ANCS\_EVT\_NTFY\_MODIFIED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)** or **[BLE\_ANCS\_EVT\_NTFY\_REMOVED\_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)** events.|
|\[in\] actId|The desired action the NC wants to be performed on the iOS notification.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully complete the API.|
|MBA\_RES\_FAIL|Operation is not permitted.|
|MBA\_RES\_OOM|No available buffer.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters. One of the following reasons: - Connection handle is not valid - Invalid write parameters. See **[GATTC\_WriteParams\_T](GUID-2D95DF00-4758-4DCE-8562-F6A1A150A365.md)** for valid values.|
|MBA\_RES\_NO\_RESOURCE|No available resource to send write operation.|
|MBA\_RES\_BUSY|GATT Client is busy. Another request is ongoing.|

