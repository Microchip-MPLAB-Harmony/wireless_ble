# BLE\_DM\_EvtSecuritySuccess\_T

**Parent topic:**[Structures](GUID-E5E92890-D6CE-4B37-AD56-B4D84D746B1B.md)

## C

```c
typedef struct BLE_DM_EvtSecuritySuccess_T
{
    BLE_DM_SecurityProc_T           procedure;
    uint8_t                         bonded;
} BLE_DM_EvtSecuritySuccess_T;
```

## Description

Data structure for **[BLE\_DM\_EVT\_SECURITY\_SUCCESS](GUID-65F57110-C2EB-4489-BF68-33BCBDD490B1.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|procedure|The procedure that has finished successfully.|
|bonded|The pairing procedure is bonded or not.|

