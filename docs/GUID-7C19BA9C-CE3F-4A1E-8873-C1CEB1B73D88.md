# BLE\_SMP\_EventField\_T

**Parent topic:**[Structures](GUID-32B57AF4-FA13-419A-852F-73C4E0457A07.md)

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
|-----|-----------|
|evtPairingComplete|Handle **[BLE\_SMP\_EVT\_PAIRING\_COMPLETE](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputPasskeyReq|Handle **[BLE\_SMP\_EVT\_INPUT\_PASSKEY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtDisplayCompareValueReq|Handle **[BLE\_SMP\_EVT\_NUMERIC\_COMPARISON\_CONFIRM\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtDisplayPasskeyReq|Handle **[BLE\_SMP\_EVT\_DISPLAY\_PASSKEY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtSecurityReq|Handle **[BLE\_SMP\_EVT\_SECURITY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtNotifyKeys|Handle **[BLE\_SMP\_EVT\_NOTIFY\_KEYS](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtPairingReq|Handle **[BLE\_SMP\_EVT\_PAIRING\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputOobData|Handle **[BLE\_SMP\_EVT\_INPUT\_OOB\_DATA\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtInputScOobData|Handle **[BLE\_SMP\_EVT\_INPUT\_SC\_OOB\_DATA\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtKeypress|Handle **[BLE\_SMP\_EVT\_KEYPRESS](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|
|evtGenScOobDataDone|Handle **[BLE\_SMP\_EVT\_GEN\_SC\_OOB\_DATA\_DONE](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)**.|

