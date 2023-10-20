# BLE\_ANCS\_EventId\_T

**Parent topic:**[Enums](GUID-EAAE0DDB-EBEC-493D-98DB-21037F1C06C6.md)

## C

```c
typedef enum BLE_ANCS_EventId_T
{
    BLE_ANCS_EVT_DISC_COMPLETE_IND,
    BLE_ANCS_EVT_ERR_ATTR_BUF_IND,
    BLE_ANCS_EVT_ERR_RECOMPOSE_BUF_IND,
    BLE_ANCS_EVT_NTFY_ADDED_IND,
    BLE_ANCS_EVT_NTFY_MODIFIED_IND,
    BLE_ANCS_EVT_NTFY_REMOVED_IND,
    BLE_ANCS_EVT_NTFY_ATTR_IND,
    BLE_ANCS_EVT_APP_ATTR_IND,
    BLE_ANCS_EVT_ERR_UNSPECIFIED_IND,
    BLE_ANCS_EVT_ERR_NO_MEM_IND,
    BLE_ANCS_EVT_ERR_IND,
} BLE_ANCS_EventId_T;
```

## Description

BLE ANCS event received by the application.

|Enumerator|Description|
|----------|-----------|
|BLE\_ANCS\_EVT\_DISC\_COMPLETE\_IND|Event for discovery completion. See **[BLE\_ANCS\_EvtDiscComplete\_T](GUID-C7627C14-126E-4A0D-8719-8B364D58795A.md)** for event details.|
|BLE\_ANCS\_EVT\_ERR\_ATTR\_BUF\_IND|The size of received data is larger than the attribute buffer. See **[BLE\_ANCS\_EvtErrAttrBufInd\_T](GUID-CBF298F4-008A-4DA8-80A8-95750D39CFDA.md)** for event details.|
|BLE\_ANCS\_EVT\_ERR\_RECOMPOSE\_BUF\_IND|The size of received data is larger than the recomposed buffer. See **[BLE\_ANCS\_EvtErrRecomposeBufInd\_T](GUID-888910F0-8007-4C13-9BD0-FD969CA34980.md)** for event details.|
|BLE\_ANCS\_EVT\_NTFY\_ADDED\_IND|The arrival of a new iOS notification on the NP. See **[BLE\_ANCS\_EvtNtfyInd\_T](GUID-9193BCBE-82CE-47CC-AF72-B0E8A7A68BC4.md)** for event details.|
|BLE\_ANCS\_EVT\_NTFY\_MODIFIED\_IND|The modification of an iOS notification on the NP. See **[BLE\_ANCS\_EvtNtfyInd\_T](GUID-9193BCBE-82CE-47CC-AF72-B0E8A7A68BC4.md)** for event details.|
|BLE\_ANCS\_EVT\_NTFY\_REMOVED\_IND|The removal of an iOS notification on the NP. See **[BLE\_ANCS\_EvtNtfyInd\_T](GUID-9193BCBE-82CE-47CC-AF72-B0E8A7A68BC4.md)** for event details.|
|BLE\_ANCS\_EVT\_NTFY\_ATTR\_IND|Notification attribute received. See **[BLE\_ANCS\_EvtNtfyAttrInd\_T](GUID-ACD78EEF-16F8-44DE-8C0D-1A056F124D0E.md)** for event details.|
|BLE\_ANCS\_EVT\_APP\_ATTR\_IND|App attribute received. See **[BLE\_ANCS\_EvtAppAttrInd\_T](GUID-954F2906-D8D5-45CD-BD41-35EE7D8DA084.md)** for event details.|
|BLE\_ANCS\_EVT\_ERR\_UNSPECIFIED\_IND|Profile internal unspecified error occurs.|
|BLE\_ANCS\_EVT\_ERR\_NO\_MEM\_IND|Profile internal error occurs due to insufficient heap memory.|
|BLE\_ANCS\_EVT\_ERR\_IND|Profile writing to the Control Point characteristic, an NC may receive the following ANCS-specific error codes.|

