# BLE_SMP_EventField_T

## C

```c
typedef union
{
    BLE_SMP_EvtPairingComplete_T         evtPairingComplete;
    BLE_SMP_EvtInputPasskeyReq_T         evtInputPasskeyReq;
    BLE_SMP_EvtDisplayCompareValueReq_T  evtDisplayCompareValueReq;
    BLE_SMP_EvtDisplayPasskeyReq_T       evtDisplayPasskeyReq;
    BLE_SMP_EvtSecurityReq_T             evtSecurityReq;
    BLE_SMP_EvtNotifyKeys_T              evtNotifyKeys;
    BLE_SMP_EvtPairingReq_T              evtPairingReq;
    BLE_SMP_EvtInputOobData_T            evtInputOobData;
    BLE_SMP_EvtInputScOobData_T          evtInputScOobData;
    BLE_SMP_EvtKeypress_T                evtKeypress;
    BLE_SMP_EvtGenScOobDataDone_T        evtGenScOobDataDone;
} BLE_SMP_EventField_T;
```

## Description

Union of BLE SMP callback event data types.


## Field Documentation

|Field|Description|
|:---|:---|
|evtPairingComplete|Handle **[BLE_SMP_EVT_PAIRING_COMPLETE](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputPasskeyReq|Handle **[BLE_SMP_EVT_INPUT_PASSKEY_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtDisplayCompareValueReq|Handle **[BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtDisplayPasskeyReq|Handle **[BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtSecurityReq|Handle **[BLE_SMP_EVT_SECURITY_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtNotifyKeys|Handle **[BLE_SMP_EVT_NOTIFY_KEYS](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtPairingReq|Handle **[BLE_SMP_EVT_PAIRING_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputOobData|Handle **[BLE_SMP_EVT_INPUT_OOB_DATA_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputScOobData|Handle **[BLE_SMP_EVT_INPUT_SC_OOB_DATA_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtKeypress|Handle **[BLE_SMP_EVT_KEYPRESS](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtGenScOobDataDone|Handle **[BLE_SMP_EVT_GEN_SC_OOB_DATA_DONE](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
