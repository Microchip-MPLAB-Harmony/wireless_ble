# BLE_ANCS_EventField_T

## C

```c
typedef union
{
    BLE_ANCS_EvtDiscComplete_T         evtDiscComplete;
    BLE_ANCS_EvtErrAttrBufInd_T        evtErrAttrBufInd;
    BLE_ANCS_EvtErrRecomposeBufInd_T   evtErrRecomposeBufInd;
    BLE_ANCS_EvtNtfyInd_T              evtNtfyInd;
    BLE_ANCS_EvtNtfyAttrInd_T          evtNtfyAttrInd;
    BLE_ANCS_EvtAppAttrInd_T           evtAppAttrInd;
} BLE_ANCS_EventField_T;
```

## Description

Union of BLE ANCS callback event data types.


## Field Documentation

|Field|Description|
|:---|:---|
|evtDiscComplete|Handle **[BLE_ANCS_EVT_DISC_COMPLETE_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**|
|evtErrAttrBufInd|Handle **[BLE_ANCS_EVT_ERR_ATTR_BUF_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtErrRecomposeBufInd|Handle **[BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtNtfyInd|Handle **[BLE_ANCS_EVT_NTFY_ADDED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**, **[BLE_ANCS_EVT_NTFY_MODIFIED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**, **[BLE_ANCS_EVT_NTFY_REMOVED_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtNtfyAttrInd|Handle **[BLE_ANCS_EVT_NTFY_ATTR_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
|evtAppAttrInd|Handle **[BLE_ANCS_EVT_APP_ATTR_IND](GUID-09E4D761-E240-4D15-8065-2AB976C30FAB.md)**.|
