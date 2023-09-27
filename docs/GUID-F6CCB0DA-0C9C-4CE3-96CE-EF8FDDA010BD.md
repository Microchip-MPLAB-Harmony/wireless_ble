# MW\_AES\_EcbEncryptInit

**Parent topic:**[Functions](GUID-EFCDC69F-77B9-4BFF-8256-6A99E252D223.md)

## C

```c
uint16_t MW_AES_EcbEncryptInit(MW_AES_Ctx_T * p_ctx, uint8_t *p_aesKey);
```

## Description

The API is used to initialize AES ECB block cipher Encryption.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[out\] p\_ctx|Pointer to the structure of AES context. Refer to **[MW\_AES\_Ctx\_T](GUID-02DB18AB-0B83-4EC7-8BA6-03EB1C68EEDE.md)** for detail structure info.|
|\[in\] p\_aesKey|Pointer to the 16 bytes encryption key.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Initialize successfully.|
|MBA\_RES\_FAIL|Failed to initialize.|

