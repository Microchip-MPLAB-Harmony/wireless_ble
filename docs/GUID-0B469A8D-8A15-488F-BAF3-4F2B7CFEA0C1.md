# BLE_TRSPC_EventId_T

## C

```c
typedef enum BLE_TRSPC_EventId_T
{
    BLE_TRSPC_EVT_UL_STATUS,
    BLE_TRSPC_EVT_DL_STATUS,
    BLE_TRSPC_EVT_RECEIVE_DATA,
    BLE_TRSPC_EVT_VENDOR_CMD,
    BLE_TRSPC_EVT_VENDOR_CMD_RSP,
    BLE_TRSPC_EVT_DISC_COMPLETE,
    BLE_TRSPC_EVT_ERR_NO_MEM
}BLE_TRSPC_EventId_T;
```

## Description

Enumeration type of BLE transparent profile callback events.


|Enumerator|Description|
|:---|:---|
|BLE_TRSPC_EVT_UL_STATUS|Transparent Profile Uplink status update event. See **[BLE_TRSPC_EvtUplinkStatus_T](GUID-70A271BB-A73E-4A15-9A25-88337CBA35FD.md)** for event details.|
|BLE_TRSPC_EVT_DL_STATUS|Transparent Profile Downlink status update event. See **[BLE_TRSPC_EvtDownlinkStatus_T](GUID-69A8EA5B-6918-4669-B516-A89473982E16.md)** for event details.|
|BLE_TRSPC_EVT_RECEIVE_DATA|Transparent Profile Data Channel received notification event. See **[BLE_TRSPC_EvtReceiveData_T](GUID-46B318DF-2896-4717-AF9F-0650AA0B8931.md)** for event details.|
|BLE_TRSPC_EVT_VENDOR_CMD|Transparent Profile vendor command received notification event. See **[BLE_TRSPC_EvtVendorCmd_T](GUID-7E1233DC-2DB2-4215-A166-6741E3B5A29A.md)** for event details.|
|BLE_TRSPC_EVT_VENDOR_CMD_RSP|Transparent Profile Vendor command response received notification event. See **[BLE_TRSPC_EvtVendorCmdRsp_T](GUID-69A557FA-6773-44B9-A226-1EDDD8C08D97.md)** for event details.|
|BLE_TRSPC_EVT_DISC_COMPLETE|Transparent Profile discovery complete event. See **[BLE_TRSPC_EvtDiscComplete_T](GUID-11CC101E-6888-47F7-A7D6-A43FF7315A36.md)** for event details.|
|BLE_TRSPC_EVT_ERR_NO_MEM|Profile internal error occurs due to insufficient heap memory.|
