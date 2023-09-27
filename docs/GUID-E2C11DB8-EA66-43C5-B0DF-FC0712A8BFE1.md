# MW\_DFU\_FwImageStart

**Parent topic:**[Functions](GUID-B1BD071D-9DA2-4C1B-8D22-E7909F11135C.md)

## C

```c
uint16_t MW_DFU_FwImageStart(void);
```

## Description

The API is used to start or restart firmware image update procedure.<br />The state machine and parameters of this module would be reset after this API is called.

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Start or restart firmware image update procedure successfully.|
|MBA\_RES\_INVALID\_PARA|This API cannot be executed in current DFU process state.|

