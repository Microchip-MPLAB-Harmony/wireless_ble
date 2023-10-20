# BLE\_SMP\_UpdateBondingInfo

**Parent topic:**[Functions](GUID-B4A018ED-CB34-4D52-A7F9-3E7808C43BF8.md)

## C

```c
uint16_t BLE_SMP_UpdateBondingInfo(uint16_t connHandle, uint8_t encryptKeySize, BLE_SMP_PairInfo_T *p_pairInfo);
```

## Description

Update paired device information to SMP.

### Note

This function is suggested to be called once after connected if the connection is bonded.<br />SMP requires bonding information to check permission when processing requests.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|
|\[in\] encryptKeySize|Encryption key size.|
|\[in\] p\_pairInfo|Pointer to the pairing information buffer. See **[BLE\_SMP\_PairInfo\_T](GUID-B0692245-D5C0-4B6B-9B8B-7E413D4BF7BC.md)**.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully update the information to SMP.|
|MBA\_RES\_INVALID\_PARA|Invalid connection handle.|

