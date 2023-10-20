# GATT\_EvtDiscPrimServResp\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATT_EvtDiscPrimServResp_T
{
    uint16_t    connHandle;
    uint8_t     attrPairLength;
    uint16_t    attrDataLength;
    uint8_t     attrData[BLE_ATT_MAX_MTU_LEN-ATT_READ_BY_GROUP_RESP_HEADER_SIZE];
    uint8_t     procedureStatus;
}   GATT_EvtDiscPrimServResp_T;
```

## Description

Data structure for **[GATTC\_EVT\_DISC\_PRIM\_SERV\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|attrPairLength|Length of each tuple in attribute data.|
|attrDataLength|Total length of all tuples.|
|attrData|Attribute data list. Contains Tuples in following format: <2-byte service handle\> <2-byte End Group Handle\><Service UUID\> Service handle is the handle of service discovered, End Group Handle is the handle of the last attribute of the service discovered Service UUID may be 16 or 128-bit. Multiple tuples received in a single event have the same service UUID length Number of tuples = \(attrDataLength/attrPairLength\). Note that the format of the data is little endian.|
|procedureStatus|"Discover All Primary Services" Procedure may not finish in one ATT request. Status indicates if further events are expected. See **[Procedure status definition](GUID-6596ABFB-83DF-4A71-9F32-47D23E1659EF.md)**.|

