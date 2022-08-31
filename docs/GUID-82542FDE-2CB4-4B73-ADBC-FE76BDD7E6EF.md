# BLE_PXPM_EventId_T

## C

```c
typedef enum BLE_PXPM_EventId_T
{
    BLE_PXPM_EVT_DISC_COMPLETE_IND = 0x00,
    BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND,
    BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND,
    BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND,
    BLE_PXPM_EVT_ERR_UNSPECIFIED_IND
}BLE_PXPM_EventId_T;
```

## Description

Enumeration type of BLE PXP Monitor callback events.


|Enumerator|Description|
|:---|:---|
|BLE_PXPM_EVT_DISC_COMPLETE_IND|PXP Reporter Service discovery completed and ready to perform PXP procedure. See **[BLE_PXPM_EvtDiscComplete_T](GUID-22085E52-409B-4BC2-994C-757BA5774B6C.md)** for event details.|
|BLE_PXPM_EVT_LLS_ALERT_LEVEL_WRITE_RSP_IND|Event for receiving LLS Alert Level write response. See **[BLE_PXPM_EvtLlsAlertLvWriteRspInd_T](GUID-5F78080A-4130-4EDF-82EA-C512B6B1C104.md)** for event details.|
|BLE_PXPM_EVT_LLS_ALERT_LEVEL_IND|Event for receiving LLS Alert Level read response. See **[BLE_PXPM_EvtLlsAlertLvInd_T](GUID-442B7CED-3027-4FC5-B29C-11A81136ED8F.md)** for event details.|
|BLE_PXPM_EVT_TPS_TX_POWER_LEVEL_IND|Event for receiving TPS Tx Power Level read response. See **[BLE_PXPM_EvtTpsTxPwrLvInd_T](GUID-257171C3-8C69-48D8-B115-28DB3CF49A35.md)** for event details.|
|BLE_PXPM_EVT_ERR_UNSPECIFIED_IND|Profile internal unspecified error occurs.|