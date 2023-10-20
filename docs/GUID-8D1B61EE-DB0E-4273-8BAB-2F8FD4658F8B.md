# BLE\_TRCBPS\_EventId\_T

**Parent topic:**[Enums](GUID-1A0D03AA-448E-4714-BBF8-67A0FC83B584.md)

## C

```c
typedef enum BLE_TRCBPS_EventId_T
{
    BLE_TRCBPS_EVT_CONNECTION_STATUS = 0x00,
    BLE_TRCBPS_EVT_RECEIVE_DATA,
    BLE_TRCBPS_EVT_VENDOR_CMD,
    BLE_TRCBPS_EVT_ERR_NO_MEM
} BLE_TRCBPS_EventId_T;
```

## Description

Enumeration type of BLE Transparent Credit Based Profile callback events.

|Enumerator|Description|
|----------|-----------|
|BLE\_TRCBPS\_EVT\_CONNECTION\_STATUS|Transparent Credit Based Profile Data/Control Channel connection status update event. See **[BLE\_TRCBPS\_EvtConnStatus\_T](GUID-5F2F020E-30D1-4FEC-B236-3213C40F6AED.md)** for event details.|
|BLE\_TRCBPS\_EVT\_RECEIVE\_DATA|Transparent Credit Based Profile Data Channel received notification event. See **[BLE\_TRCBPS\_EvtReceiveData\_T](GUID-049FC0EF-A122-407D-82A4-023D4EEA067C.md)** for event details.|
|BLE\_TRCBPS\_EVT\_VENDOR\_CMD|Transparent Credit Based Profile vendor command received notification event. See **[BLE\_TRCBPS\_EvtVendorCmd\_T](GUID-408CE073-DEDA-4662-839C-36C0405D996B.md)** for event details.|
|BLE\_TRCBPS\_EVT\_ERR\_NO\_MEM|Profile internal error occurs due to insufficient heap memory.|

