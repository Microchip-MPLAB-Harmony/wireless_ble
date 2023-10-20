# BLE\_ANPC\_EvtWriteCpRspInd\_T

**Parent topic:**[Structures](GUID-EEC87BE4-9397-4DE3-B2A3-A61C788AA4DE.md)

## C

```c
typedef struct BLE_ANPC_EvtWriteCpRspInd_T
{
    uint16_t        connHandle;
    uint16_t        errCode;
}BLE_ANPC_EvtWriteCpRspInd_T;
```

## Description

Data structure for **[BLE\_ANPC\_EVT\_WRITE\_CP\_RSP\_IND](GUID-19DD8E02-2F17-4294-AD2C-97BEB9E9F804.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|The connection handle.|
|errCode|The error code of config control point response. See **[Error code definition](GUID-FCD9FB33-76F7-47F9-87FF-C19DF80C6059.md)**.|

