# GATT\_EvtDiscCharResp\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATT_EvtDiscCharResp_T
{
    uint16_t    connHandle;
    uint8_t     attrPairLength;
    uint16_t    attrDataLength;
    uint8_t     attrData[BLE_ATT_MAX_MTU_LEN-ATT_READ_BY_TYPE_RESP_HEADER_SIZE];
    uint8_t     procedureStatus;
}   GATT_EvtDiscCharResp_T;
```

## Description

Data structure for **[GATTC\_EVT\_DISC\_CHAR\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|attrPairLength|Length of each handle-value pair in the list.|
|attrDataLength|Total length of attribute data list.|
|attrData|Attribute data list. Consists of handle-value pairs in following format: <2-byte Characteristic declaration handle\> <1-byte Characteristic property\> <2-byte Value Handle\> <Characteristic UUID\> To extract the Characteristic property data see **[Characteristic property](GUID-BCAAB1F7-7FDB-4329-9496-25B53F843820.md)** Characteristic UUID may be 16-bit or 128-bit in length. Multiple Handle-value pairs received in a single event have the same characteristic UUID length Number of handle-value pairs = \(attrDataLength/attrPairLength Note that the format of the data is little endian.|
|procedureStatus|Discover All Characteristics procedure may not finish in one ATT request. Status indicates if further events are expected. See **[Procedure status definition](GUID-6596ABFB-83DF-4A71-9F32-47D23E1659EF.md)**.|

