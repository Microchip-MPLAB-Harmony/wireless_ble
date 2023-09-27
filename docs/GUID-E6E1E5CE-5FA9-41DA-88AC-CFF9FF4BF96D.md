# BLE\_SMP\_EvtInputPasskeyReq\_T

**Parent topic:**[Structures](GUID-32B57AF4-FA13-419A-852F-73C4E0457A07.md)

## C

```c
typedef struct  BLE_SMP_EvtInputPasskeyReq_T
{
    uint16_t                connHandle;
    bool                    keypress;
} BLE_SMP_EvtInputPasskeyReq_T;
```

## Description

Data structure for **[BLE\_SMP\_EVT\_INPUT\_PASSKEY\_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|keypress|Is required to inform the remote device when keys have been entered or erased. If this is true, please using **[BLE\_SMP\_Keypress](GUID-DE4C319B-5ABC-417B-A86A-9E97050B4D1B.md)** to send keypress notifications.|

