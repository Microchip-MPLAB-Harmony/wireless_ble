# BLE\_GAP\_ExtCreateConnPhy\_T

**Parent topic:**[Structures](GUID-230368B0-FB2A-4967-A471-691387B35A9E.md)

## C

```c
typedef struct BLE_GAP_ExtCreateConnPhy_T
{
    BLE_GAP_ExtCreateConnParams_T    le1mPhy;
    BLE_GAP_ExtCreateConnParams_T    le2mPhy;
    BLE_GAP_ExtCreateConnParams_T    leCodedPhy;
}BLE_GAP_ExtCreateConnPhy_T;
```

## Description

Extended create connection phy setting.

## Field Documentation

|Field|Description|
|-----|-----------|
|le1mPhy|Scan connectable advertisements on the LE 1M PHY. Connection parameters for the LE 1M PHY are provided.|
|le2mPhy|Connection parameters for the LE 2M PHY are provided.|
|leCodedPhy|Scan connectable advertisements on the LE Coded PHY. Connection parameters for the LE Coded PHY are provided.|

