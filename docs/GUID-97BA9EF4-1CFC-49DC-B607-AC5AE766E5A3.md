# GATT\_EvtReadUsingUuidResp\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATT_EvtReadUsingUuidResp_T
{
    uint16_t    connHandle;
    uint8_t     attrPairLength;
    uint16_t    attrDataLength;
    uint8_t     attrData[BLE_ATT_MAX_MTU_LEN-ATT_READ_BY_TYPE_RESP_HEADER_SIZE];
}   GATT_EvtReadUsingUuidResp_T;
```

## Description

Data structure for **[GATTC\_EVT\_READ\_USING\_UUID\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|attrPairLength|Length of each attribute handle-value pair in the list.|
|attrDataLength|Length of attributes data list.|
|attrData|Attribute data list. Data consists of one or more handle-value pairs. - Format of handle-value pair: <2-byte Characteristic Value handle\> <Characteristic Value Read\> - Number of handle-value pairs = \(attrDataLength/attrPairLength\) Note that the format of the data is little endian.|

