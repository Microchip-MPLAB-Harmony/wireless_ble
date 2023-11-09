# BLE\_SCM\_EvtBondedCharInfo\_T

**Parent topic:**[Structures](GUID-649AA2A5-B480-478B-AD34-137EDE75C855.md)

## C

```c
typedef struct BLE_SCM_EvtBondedCharInfo_T
{
    uint16_t                    connHandle;
    BLE_DD_CharInfo_T           charInfo[BLE_SCM_GATT_DISC_CHAR_NUM];
} BLE_SCM_EvtBondedCharInfo_T;
```

## Description

Data structure for **[BLE\_SCM\_EVT\_BONDED\_CHAR\_INFO](GUID-835AA322-4AA4-473C-9BB5-F6F7368CD9EC.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|charInfo|Characteristic information of GATT service of this bonded remote peer.|

