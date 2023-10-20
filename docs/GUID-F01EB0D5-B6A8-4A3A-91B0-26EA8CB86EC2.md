# BLE\_PXPR\_EventId\_T

**Parent topic:**[Enums](GUID-E05F98C1-D6B6-42D2-8EDE-F679AC9624A3.md)

## C

```c
typedef enum BLE_PXPR_EventId_T
{
    BLE_PXPR_EVT_LLS_ALERT_LEVEL_WRITE_IND = 0x00,
    BLE_PXPR_EVT_IAS_ALERT_LEVEL_WRITE_IND,
    BLE_PXPR_EVT_ERR_UNSPECIFIED_IND
}BLE_PXPR_EventId_T;
```

## Description

Enumeration type of BLE PXP Reporter callback events.

|Enumerator|Description|
|----------|-----------|
|BLE\_PXPR\_EVT\_LLS\_ALERT\_LEVEL\_WRITE\_IND|Event for Alert Level in Link Loss Service is written. See **[BLE\_PXPR\_EvtAlertLevelWriteInd\_T](GUID-3CECF792-F54B-4A5C-95FA-5A2D18F38B24.md)** for event details.|
|BLE\_PXPR\_EVT\_IAS\_ALERT\_LEVEL\_WRITE\_IND|Event for Alert Level in Immediate Alert service is written. See **[BLE\_PXPR\_EvtAlertLevelWriteInd\_T](GUID-3CECF792-F54B-4A5C-95FA-5A2D18F38B24.md)** for event details.|
|BLE\_PXPR\_EVT\_ERR\_UNSPECIFIED\_IND|Profile internal unspecified error occurs.|

