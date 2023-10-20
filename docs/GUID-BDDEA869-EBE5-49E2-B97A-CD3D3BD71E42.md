# BLE\_DD\_RestartServicesDiscovery

**Parent topic:**[Functions](GUID-1162ABF9-6321-488C-990D-4DF90A24C1BA.md)

## C

```c
uint16_t BLE_DD_RestartServicesDiscovery(uint16_t connHandle);
```

## Description

Restart database discovery for specific connection.

### Note

This function shall only be called when remote services changed and application wants to restart database discovery.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Connection handle associated with this connection.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully restart discovery.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|The connHandle is invalid.|

