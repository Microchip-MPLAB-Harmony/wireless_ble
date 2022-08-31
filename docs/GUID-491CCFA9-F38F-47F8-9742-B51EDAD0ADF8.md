# BLE_ANPC_EvtUnreadAlertStatInd_T

## C

```c
typedef struct BLE_ANPC_EvtUnreadAlertStatInd_T
{
    uint16_t        connHandle;
    uint8_t         categoryId;
    uint8_t         unreadCnt;
}BLE_ANPC_EvtUnreadAlertStatInd_T;
```

## Description

Data structure for **[BLE_ANPC_EVT_UNREAD_ALERT_STAT_IND](GUID-6B001FFC-2FB3-4121-BBE3-7C382205F26C.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|The connection handle.|
|categoryId|The category Id. See **[Category Id](GUID-570F7538-C8C6-4C55-AF3F-EC331C1D2CF4.md)**.|
|unreadCnt|Unread Count.|
