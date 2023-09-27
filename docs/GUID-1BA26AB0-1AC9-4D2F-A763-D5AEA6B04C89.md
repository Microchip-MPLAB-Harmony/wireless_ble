# GATT\_EvtProtocolAvailable\_T

**Parent topic:**[Structures](GUID-033AEAE3-56F0-4C38-99A5-6315F4885209.md)

## C

```c
typedef struct GATT_EvtProtocolAvailable_T
{
    uint16_t         connHandle;
} GATT_EvtProtocolAvailable_T;
```

## Description

Data structure for **[GATTC\_EVT\_PROTOCOL\_AVAILABLE](GUID-506F6039-E62F-4121-8CA8-2335BAF7EFB6.md)** or **[GATTS\_EVT\_PROTOCOL\_AVAILABLE](GUID-506F6039-E62F-4121-8CA8-2335BAF7EFB6.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|

