# GATT_EvtDiscPrimServByUuidResp_T

## C

```c
typedef struct GATT_EvtDiscPrimServByUuidResp_T
{
    uint16_t    connHandle;
    uint16_t    handleInfoLength;
    uint8_t     handleInfo[BLE_ATT_MAX_MTU_LEN-ATT_FIND_BY_TYPE_RESP_HEADER_SIZE];
    uint8_t     procedureStatus;
}   GATT_EvtDiscPrimServByUuidResp_T;
```

## Description

Data structure for **[GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP](GUID-506F6039-E62F-4121-8CA8-2335BAF7EFB6.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|Connection handle associated with this connection.|
|handleInfoLength|Total length of the handle information list in bytes.|
|handleInfo|Handle information list.                                                                                             Contains handle pairs in following format: \<2-byte service handle\> \<2-byte End Group Handle\>                                                                                              Service handle is the handle of service discovered, End Group Handle is the handle of the last attribute of the service discovered                                                                                             If the same primary service has multiple instances on the server then multiple handle pairs may exist.                                                                                              Note that the format of the data is little endian.|
|procedureStatus|Discover Primary Service By UUID may not finish in one ATT request. Status indicates if further events are expected. See **[Procedure status definition](GUID-9BD44B68-1DF0-497B-8AF2-AF07BD9BB2A9.md)**.|
