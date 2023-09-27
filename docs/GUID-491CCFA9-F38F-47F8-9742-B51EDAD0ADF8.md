# BLE\_ANPC\_EvtUnreadAlertStatInd\_T

**Parent topic:**[Structures](GUID-4E49A4DF-FDD3-40FC-801F-BF51C85F516A.md)

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

Data structure for **[BLE\_ANPC\_EVT\_UNREAD\_ALERT\_STAT\_IND](GUID-6B001FFC-2FB3-4121-BBE3-7C382205F26C.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|The connection handle.|
|categoryId|The category Id. See **[Category Id](GUID-570F7538-C8C6-4C55-AF3F-EC331C1D2CF4.md)**.|
|unreadCnt|Unread Count.|

