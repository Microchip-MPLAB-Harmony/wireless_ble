# BLE\_SMP\_EventField\_T

**Parent topic:**[Structures](GUID-636B185D-E80D-4839-A443-EF1750F1D2B1.md)

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
|evtPairingComplete|Handle **[BLE\_SMP\_EVT\_PAIRING\_COMPLETE](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtInputPasskeyReq|Handle **[BLE\_SMP\_EVT\_INPUT\_PASSKEY\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtDisplayCompareValueReq|Handle **[BLE\_SMP\_EVT\_NUMERIC\_COMPARISON\_CONFIRM\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtDisplayPasskeyReq|Handle **[BLE\_SMP\_EVT\_DISPLAY\_PASSKEY\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtSecurityReq|Handle **[BLE\_SMP\_EVT\_SECURITY\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtNotifyKeys|Handle **[BLE\_SMP\_EVT\_NOTIFY\_KEYS](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtPairingReq|Handle **[BLE\_SMP\_EVT\_PAIRING\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtInputOobData|Handle **[BLE\_SMP\_EVT\_INPUT\_OOB\_DATA\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtInputScOobData|Handle **[BLE\_SMP\_EVT\_INPUT\_SC\_OOB\_DATA\_REQUEST](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtKeypress|Handle **[BLE\_SMP\_EVT\_KEYPRESS](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|
|evtGenScOobDataDone|Handle **[BLE\_SMP\_EVT\_GEN\_SC\_OOB\_DATA\_DONE](GUID-184B99E4-8C26-4312-8593-3EE58F9E842B.md)**.|

