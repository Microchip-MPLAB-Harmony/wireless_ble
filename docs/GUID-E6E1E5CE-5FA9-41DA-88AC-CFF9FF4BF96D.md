# BLE_SMP_EvtInputPasskeyReq_T

## C

```c
typedef struct  BLE_SMP_EvtInputPasskeyReq_T
{
    uint16_t                connHandle;
    bool                    keypress;
} BLE_SMP_EvtInputPasskeyReq_T;
```

## Description

Data structure for **[BLE_SMP_EVT_INPUT_PASSKEY_REQUEST](GUID-DA3C91C3-3ACA-4850-B469-FDF748DD2D87.md)** event


## Field Documentation

|Field|Description|
|:---|:---|
|connHandle|Connection handle associated with this connection.|
|keypress|Is required to inform the remote device when keys have been entered or erased.                                                                             If this is true, please using **[BLE_SMP_Keypress](GUID-DE4C319B-5ABC-417B-A86A-9E97050B4D1B.md)** to send keypress notifications.|
