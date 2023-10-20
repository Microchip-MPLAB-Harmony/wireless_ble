# BLE\_ANPC\_EvtDiscComplete\_T

**Parent topic:**[Structures](GUID-EEC87BE4-9397-4DE3-B2A3-A61C788AA4DE.md)

## C

```c
typedef struct BLE_ANPC_EvtDiscComplete_T
{
    uint16_t        connHandle;
    uint16_t        ansStartHandle;
    uint16_t        ansEndHandle;
}BLE_ANPC_EvtDiscComplete_T;
```

## Description

Data structure for **[BLE\_ANPC\_EVT\_DISC\_COMPLETE\_IND](GUID-19DD8E02-2F17-4294-AD2C-97BEB9E9F804.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|The connection handle of discovery completion.|
|ansStartHandle|The start handle of ANS.|
|ansEndHandle|The end handle of ANS.|

