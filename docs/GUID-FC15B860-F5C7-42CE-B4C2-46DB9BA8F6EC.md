# BLE\_SMP\_SecKeys\_T

**Parent topic:**[Structures](GUID-636B185D-E80D-4839-A443-EF1750F1D2B1.md)

## C

```c
typedef struct BLE_SMP_SecKeys_T
{
    BLE_SMP_EncInfo_T       encInfo;
    BLE_SMP_IdInfo_T        idInfo;
    BLE_SMP_SignInfo_T      signInfo;
}BLE_SMP_SecKeys_T;
```

## Description

Security keys.

## Field Documentation

|Field|Description|
|-----|-----------|
|encInfo|The encryption information.|
|idInfo|The identity information.|
|signInfo|The signing information.|

