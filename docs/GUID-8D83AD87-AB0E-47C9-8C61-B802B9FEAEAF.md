# BLE\_PXPM\_GetDescList

**Parent topic:**[Functions](GUID-3AAC9FAC-C500-4C78-92C5-AA58FA4A7442.md)

## C

```c
uint16_t BLE_PXPM_GetDescList(uint16_t connHandle, BLE_PXPM_DescList_T *p_descList);
```

## Description

Get information about descriptor list of the Proximity Service that has been discovered.<br />This API could be called only after **[BLE\_PXPM\_EVT\_DISC\_COMPLETE\_IND](GUID-82542FDE-2CB4-4B73-ADBC-FE76BDD7E6EF.md)** event is issued.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Handle of the connection.|
|\[out\] p\_descList|Descriptor information of the discovered service. When the descriptor is not found then descriptor list will be 0.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully get the descriptor list.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters. Connection handle is not valid.|

