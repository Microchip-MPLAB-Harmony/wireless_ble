# BLE_SMP_EvtKeypress_T

## C

```c
typedef struct  BLE_SMP_EvtKeypress_T
{
    uint16_t                connHandle;
    uint8_t                 notifyType;
} BLE_SMP_EvtKeypress_T;
```

## Description

Data structure for **[BLE_SMP_EVT_KEYPRESS](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|Connection handle associated with this connection.|
|notifyType|Notification type. See **[Keypress notification type](GUID-A3107AB3-7CF4-4B0E-9D44-1C68B02E9A01.md)**.|
