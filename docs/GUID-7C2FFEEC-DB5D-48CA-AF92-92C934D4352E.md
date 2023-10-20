# BLE\_OTAPS\_DevInfo\_T

**Parent topic:**[Structures](GUID-4EAA8368-F1C7-4125-A844-2BFFE5CAF2F9.md)

## C

```c
typedef struct BLE_OTAPS_DevInfo_T
{
    uint32_t  fwImageVer;
} BLE_OTAPS_DevInfo_T;
```

## Description

The structure contains current device information.

It should be used to response the event **[BLE\_OTAPS\_EVT\_UPDATE\_REQ](GUID-F3177CD0-E008-4DCD-AA8E-40F6DC495A8A.md)**.

## Field Documentation

|Field|Description|
|-----|-----------|
|fwImageVer|The version of current firmware image.|

