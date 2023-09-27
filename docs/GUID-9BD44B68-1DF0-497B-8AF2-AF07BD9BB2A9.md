# Procedure status definition

**Parent topic:**[Defines](GUID-F7CF3BA1-36B6-40A6-9257-19D4863812CA.md)

## C

```c
#define GATT_PROCEDURE_STATUS_CONTINUE                      0x00
#define GATT_PROCEDURE_STATUS_FINISH                        0x01
```

## Description

The definition of of procedure status.

### Note

Some GATT procedures would not finish in one ATT request. Hence, more than one event will be sent in one procedure.

Status in the event indicates if this procedure is still in progress or complete.

## Macro Definition

|Macro|Description|
|-----|-----------|
|GATT\_PROCEDURE\_STATUS\_CONTINUE|Querying procedure continues. Further events will be received.|
|GATT\_PROCEDURE\_STATUS\_FINISH|Querying procedure is complete. Last event of this procedure.|

