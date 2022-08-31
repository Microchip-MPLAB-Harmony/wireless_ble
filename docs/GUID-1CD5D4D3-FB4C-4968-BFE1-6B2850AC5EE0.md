# Definition of downlink status

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
|:---|:---|
|BLE_TRSPC_DL_STATUS_DISABLED|Transparent downlink is disabled.|
|BLE_TRSPC_DL_STATUS_NONCBFCENABLED|Transparent downlink is enabled without credit based flow control|
|BLE_TRSPC_DL_STATUS_CBFCENABLED|Transparent downlink is enabled with credit based flow control|
