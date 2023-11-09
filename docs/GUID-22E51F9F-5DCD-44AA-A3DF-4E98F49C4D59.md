# BLE\_SCM\_SetBondedCharInfo

**Parent topic:**[Functions](GUID-A37CD39C-147D-4255-AC5F-4958ABC738EE.md)

## C

```c
void BLE_SCM_SetBondedCharInfo(uint16_t connHandle, BLE_DD_CharInfo_T *p_charInfo);
```

## Description

Set characteristics handles information of bonded connection.

### Note

Application help to cache handle information and set back to BLE\_SCM when bonded link connected.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] p\_charInfo|Point to the characteristic information of GATT service of this bonded remote peer. The maximum number is **[BLE\_SCM\_GATT\_DISC\_CHAR\_NUM](GUID-21BDD2E5-0993-4F18-A076-B875949E9A71.md)**.|

