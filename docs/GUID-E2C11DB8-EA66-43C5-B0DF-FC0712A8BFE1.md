# MW_DFU_FwImageStart

## C

```c
uint16_t MW_DFU_FwImageStart();
```

## Description

The API is used to start or restart firmware image update procedure. 
The state machine and parameters of this module would be reset after this API is called. 

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Start or restart firmware image update procedure successfully.|
MBA_RES_INVALID_PARA|This API cannot be executed in current DFU process state.|