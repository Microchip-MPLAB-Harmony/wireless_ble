# BLE\_DM\_EventId\_T

**Parent topic:**[Enums](GUID-791492BD-49C4-4844-8A55-D4AE2E1E91EC.md)

## C

```c
typedef enum BLE_DM_EventId_T
{
    BLE_DM_EVT_DISCONNECTED,
    BLE_DM_EVT_CONNECTED,
    BLE_DM_EVT_SECURITY_START,
    BLE_DM_EVT_SECURITY_SUCCESS,
    BLE_DM_EVT_SECURITY_FAIL,
    BLE_DM_EVT_PAIRED_DEVICE_FULL,
    BLE_DM_EVT_PAIRED_DEVICE_UPDATED,
    BLE_DM_EVT_CONN_UPDATE_SUCCESS,
    BLE_DM_EVT_CONN_UPDATE_FAIL,
}BLE_DM_EventId_T;
```

## Description

Enumeration type of BLE DM callback events.

|Enumerator|Description|
|----------|-----------|
|BLE\_DM\_EVT\_DISCONNECTED|BLE link is terminated. See **[BLE\_DM\_Event\_T](GUID-AF6224B1-E4B6-4572-9AE4-13788CE7DF78.md)** for the event details.|
|BLE\_DM\_EVT\_CONNECTED|BLE link is established. See **[BLE\_DM\_Event\_T](GUID-AF6224B1-E4B6-4572-9AE4-13788CE7DF78.md)** for the event details.|
|BLE\_DM\_EVT\_SECURITY\_START|Security procedure has started. See **[BLE\_DM\_EvtSecurityStart\_T](GUID-4125105D-DB1E-45F9-B443-D9B5C4D6165C.md)**. for the event detail.|
|BLE\_DM\_EVT\_SECURITY\_SUCCESS|Security procedure has finished successfully. See **[BLE\_DM\_EvtSecuritySuccess\_T](GUID-610666C6-91C4-4A97-A3F7-71407FE3E639.md)**. for the event detail.|
|BLE\_DM\_EVT\_SECURITY\_FAIL|Security procedure has failed. See **[BLE\_DM\_EvtSecurityFail\_T](GUID-A7013816-3375-4AB5-9DCA-83ECF1DDB5C3.md)**. for the event detail.|
|BLE\_DM\_EVT\_PAIRED\_DEVICE\_FULL|The maximum record number of paired device have been reached. DM cannot store the latest bonding data to flash. To solve this problem, delete paired device that is not needed anymore. See the **[BLE\_DM\_EvtPairedDeviceFull\_T](GUID-65616CE8-689A-4BB4-AE57-E5F9A5B3FBF3.md)** for the event content.|
|BLE\_DM\_EVT\_PAIRED\_DEVICE\_UPDATED|A paired device have been updated. Application can use peerDevId get paired device information by **[BLE\_DM\_GetPairedDevice](GUID-D8AED317-0F8D-4EEC-8284-1FEE79D560EE.md)**.|
|BLE\_DM\_EVT\_CONN\_UPDATE\_SUCCESS|Connection parameter update triggered by **[BLE\_DM\_ConnectionParameterUpdate](GUID-1CA8B302-0ADA-4F74-A94A-6D0A15923F76.md)** is success. See **[BLE\_DM\_Event\_T](GUID-AF6224B1-E4B6-4572-9AE4-13788CE7DF78.md)** for the event details.|
|BLE\_DM\_EVT\_CONN\_UPDATE\_FAIL|Connection parameter update triggered by **[BLE\_DM\_ConnectionParameterUpdate](GUID-1CA8B302-0ADA-4F74-A94A-6D0A15923F76.md)** is fail. See **[BLE\_DM\_Event\_T](GUID-AF6224B1-E4B6-4572-9AE4-13788CE7DF78.md)** for the event details.|

