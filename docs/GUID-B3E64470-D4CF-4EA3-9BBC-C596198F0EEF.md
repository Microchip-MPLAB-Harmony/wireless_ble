# BLE_ANCS_PerformNtfyAction

## C

```c
uint16_t BLE_ANCS_PerformNtfyAction(uint16_t connHandle, uint32_t ntfyId, BLE_ANCS_ActionId_T actId);
```

## Description

Function for The Perform Notification Action command allows an NC to perform a predetermined action on a specific iOS notification.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] connHandle|The connection handle.|
|\[in\] ntfyId|A 32-bit numerical value representing the UID of the iOS notification on which the client wants to perform an action.   The ntfyId could be retrieved when receiving **[BLE_ANCS_EVT_NTFY_ADDED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)** or **[BLE_ANCS_EVT_NTFY_MODIFIED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)** or **[BLE_ANCS_EVT_NTFY_REMOVED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**. events.|
|\[in\] actId|The desired action the NC wants to be performed on the iOS notification.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully complete the API.|
MBA_RES_FAIL|Operation is not permitted.|
MBA_RES_OOM|No available buffer.|
MBA_RES_INVALID_PARA|Invalid parameters. One of the following reasons: - Connection handle is not valid - Invalid write parameters. See **[GATTC_WriteParams_T](GUID-2D95DF00-4758-4DCE-8562-F6A1A150A365.md)** for valid values.|
MBA_RES_NO_RESOURCE|No available resource to send write operation.|
MBA_RES_BUSY|GATT Client is busy. Another request is ongoing.|
