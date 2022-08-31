# Procedure status definition

## C

```c
#define GATT_PROCEDURE_STATUS_CONTINUE                      0x00
#define GATT_PROCEDURE_STATUS_FINISH                        0x01
```

## Description

The definition of of procedure status

### Note

Some GATT procedures would not finish in one ATT request. Hence, more than one event will received in one procedure.

Status in the event indicates if this procedure will continue or end.



## Macro Definition

|Macro|Description|
|:---|:---|
|GATT_PROCEDURE_STATUS_CONTINUE|Querying procedure continues. Further events will be received.|
|GATT_PROCEDURE_STATUS_FINISH|Querying procedure is complete. Last event of this procedure.|
