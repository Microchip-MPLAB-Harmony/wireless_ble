# BLE\_GAP\_SetResolvingList

**Parent topic:**[Functions](GUID-D235316A-5434-4ADA-AEF5-10D073D0126B.md)

## C

```c
uint16_t BLE_GAP_SetResolvingList(uint8_t num, BLE_GAP_ResolvingListParams_T *p_resolvingList);
```

## Description

Set devices to the resolving list.

### Note

This API is not permitted in the following three scenarios:

-   Advertising is enabled.

-   Scanning is enabled.

-   **[BLE\_GAP\_CreateConnection](GUID-9630891B-6917-4AF0-86FF-6B7B106D524D.md)**, **[BLE\_GAP\_ExtCreateConnection](GUID-8142484B-2E7A-4BBC-9F11-32CE0D179C21.md)** or **[BLE\_GAP\_CreateSync](GUID-8680FA51-B717-4A7D-A1E9-05B7B54B3DB9.md)** API is outstanding.


## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] num|The number of the resolving list included. See **[BLE\_GAP\_MAX\_RESOLV\_NUM](GUID-31369F94-8A83-4482-AE19-47E48D961CE5.md)**.|
|\[in\] p\_resolvingList|Pointer to **[BLE\_GAP\_ResolvingListParams\_T](GUID-E3210C26-2A7D-47E2-9822-3CA93B7A2984.md)** structure buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully add the device to the resolving list.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|When number exceeds **[BLE\_GAP\_MAX\_RESOLV\_NUM](GUID-31369F94-8A83-4482-AE19-47E48D961CE5.md)**.|
|MBA\_RES\_BAD\_STATE|Resolving list cannot be configured while advertising, scanning or creating a connection.|

