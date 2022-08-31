# BLE_ANPS_EvtAncpWriteInd_T

## C

```c
typedef struct BLE_ANPS_EvtAncpWriteInd_T
{
    uint16_t        connHandle;
    uint8_t         cmdId;
    uint8_t         catId;
}BLE_ANPS_EvtAncpWriteInd_T;
```

## Description

Data structure for **[BLE_ANPS_EVT_ANCP_WRITE_IND](GUID-652F4248-7986-49C9-B618-081627A19E74.md)** ievent.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|The connection handle.|
|cmdId|The command ID.  See **[Command definition](GUID-911BADE8-F247-4255-BD0C-F34416C57963.md)**.|
|catId|The category ID. See **[Category Id](GUID-C2119D96-407C-4F6F-8BE2-F491FE84218B.md)**.|
