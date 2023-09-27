# Definition of downlink status

**Parent topic:**[Defines](GUID-6EDD6ECC-FF39-46BA-9B00-972ED3CD5AFC.md)

## C

```c
#define BLE_TRSPC_DL_STATUS_DISABLED            0x00
#define BLE_TRSPC_DL_STATUS_NONCBFCENABLED      0x10
#define BLE_TRSPC_DL_STATUS_CBFCENABLED         0x20
```

## Description

The definition of BLE transparent service downlink status.

## Macro Definition

|Macro|Description|
|-----|-----------|
|BLE\_TRSPC\_DL\_STATUS\_DISABLED|Transparent downlink is disabled.|
|BLE\_TRSPC\_DL\_STATUS\_NONCBFCENABLED|Transparent downlink is enabled without credit based flow control.|
|BLE\_TRSPC\_DL\_STATUS\_CBFCENABLED|Transparent downlink is enabled with credit based flow control.|

