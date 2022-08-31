# BLE_DM_EvtSecuritySuccess_T

## C

```c
typedef struct BLE_DM_EvtSecuritySuccess_T
{
    BLE_DM_SecurityProc_T           procedure;
    uint8_t                         bonded;
} BLE_DM_EvtSecuritySuccess_T;
```

## Description

Data structure for **[BLE_DM_EVT_SECURITY_SUCCESS](GUID-65F57110-C2EB-4489-BF68-33BCBDD490B1.md)** event.


## Field Documentation

|Field|Description|
|:---|:---|
|procedure|The procedure that has finished successfully.|
|bonded|The pairing procedure is bonded or not.|
