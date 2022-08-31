# BLE_OTAPS_EventField_T

## C

```c
typedef union
{
    BLE_OTAPS_EvtUpdateReq_T            evtUpdateReq;
    BLE_OTAPS_EvtStartInd_T             evtStartInd;
    BLE_OTAPS_EvtUpdatingInd_T          evtUpdatingInd;
    BLE_OTAPS_EvtCompleteInd_T          evtCompleteInd;
} BLE_OTAPS_EventField_T;
```

## Description

Union of BLE OTA profile callback event data types.


## Field Documentation

|Field|Description|
|:---|:---|
|evtUpdateReq|Handle **[BLE_OTAPS_EVT_UPDATE_REQ](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**|
|evtStartInd|Handle **[BLE_OTAPS_EVT_START_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**|    
|evtUpdatingInd|Handle **[BLE_OTAPS_EVT_UPDATING_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**|        
|evtCompleteInd|Handle **[BLE_OTAPS_EVT_COMPLETE_IND](GUID-63B996F4-CEC1-4B2A-BDE5-37090FBFA514.md)**|        
