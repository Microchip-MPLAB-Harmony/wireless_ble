# BLE\_OTAPS\_EventId\_T

**Parent topic:**[Enums](GUID-3CEC6A55-4A7A-40B2-A955-06E240196F95.md)

## C

```c
typedef enum BLE_OTAPS_EventId_T
{
    BLE_OTAPS_EVT_UPDATE_REQ,
    BLE_OTAPS_EVT_START_IND,
    BLE_OTAPS_EVT_UPDATING_IND,
    BLE_OTAPS_EVT_COMPLETE_IND,
    BLE_OTAPS_EVT_RESET_IND,
    BLE_OTAPS_EVT_ERR_UNSPECIFIED_IND
}BLE_OTAPS_EventId_T;
```

## Description

Enumeration type of BLE OTA profile callback events.

|Enumerator|Description|
|----------|-----------|
|BLE\_OTAPS\_EVT\_UPDATE\_REQ|Request to start update procedure. See **[BLE\_OTAPS\_EvtUpdateReq\_T](GUID-A3702509-FE68-4F09-8735-0FE47BD093F5.md)** for event details.|
|BLE\_OTAPS\_EVT\_START\_IND|Indication of starting one firmware image update. See **[BLE\_OTAPS\_EvtStartInd\_T](GUID-433DC6E6-5494-4A91-8212-39E5B2039F77.md)** for event details.|
|BLE\_OTAPS\_EVT\_UPDATING\_IND|Indication of fragmented firmware image update. See **[BLE\_OTAPS\_EvtUpdatingInd\_T](GUID-99591034-AC02-4E1D-B750-D2DE3A2A3C06.md)** for event details.|
|BLE\_OTAPS\_EVT\_COMPLETE\_IND|Indication of firmware update completed. See **[BLE\_OTAPS\_EvtCompleteInd\_T](GUID-DD9C9FD7-34EB-4636-8E95-2B72521AC371.md)** for event details.|
|BLE\_OTAPS\_EVT\_RESET\_IND|Indication of device reset request received. OTA client role may ask OTA Server device to rest after firmware update procedure is completed.|
|BLE\_OTAPS\_EVT\_ERR\_UNSPECIFIED\_IND|Profile internal unspecified error occurs.|

