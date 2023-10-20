# STACK\_Event\_T

**Parent topic:**[Enumerations](GUID-01B45B36-427D-4D04-92D1-DB14D4E57921.md)

## C

```c
typedef struct STACK_Event_T
{
    STACK_GroupId_T     groupId;
    uint16_t            evtLen;
    uint8_t             *p_event;
} STACK_Event_T;
```

## Description

STACK callback event.

## Field Documentation

|Field|Description|
|-----|-----------|
|groupId|Group id. See **STACK\_GroupId\_T**|
|evtLen|Length of the event|
|p\_event|Pointer to the event field|

