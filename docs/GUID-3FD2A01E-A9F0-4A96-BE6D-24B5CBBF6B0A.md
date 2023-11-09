# BLE\_TRSPS\_Uuids\_T

**Parent topic:**[Structures](GUID-CF652FF4-6E48-4AFE-98C2-AF0B4F1E2DFE.md)

## C

```c
typedef struct  BLE_TRSPS_Uuids_T
{
    uint8_t *p_primaryService;
    uint8_t *p_transTx;
    uint8_t *p_transRx;
    uint8_t *p_transCtrl;
} BLE_TRSPS_Uuids_T;
```

## Description

The structure contains information about change UUID function parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|p\_primaryService|The user-defined 128-bit primary service UUID.|
|p\_transTx|The user-defined 128-bit trans tx characteristic UUID.|
|p\_transRx|The user-defined 128-bit trans rx characteristic UUID.|
|p\_transCtrl|The user-defined 128-bit trans ctrl characteristic UUID.|

