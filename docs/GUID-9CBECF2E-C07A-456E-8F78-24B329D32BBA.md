# BLE\_ANPS\_EvtAncpWriteInd\_T

**Parent topic:**[Structures](GUID-658D6517-5292-4E3D-91AD-09EE3CB53554.md)

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

Data structure for **[BLE\_ANPS\_EVT\_ANCP\_WRITE\_IND](GUID-652F4248-7986-49C9-B618-081627A19E74.md)** ievent.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|The connection handle.|
|cmdId|The command ID. See **[Command definition](GUID-911BADE8-F247-4255-BD0C-F34416C57963.md)**.|
|catId|The category ID. See **[Category Id](GUID-C2119D96-407C-4F6F-8BE2-F491FE84218B.md)**.|

