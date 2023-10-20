# BLE\_SMP\_Config

**Parent topic:**[Functions](GUID-B4A018ED-CB34-4D52-A7F9-3E7808C43BF8.md)

## C

```c
uint16_t BLE_SMP_Config(BLE_SMP_Config_T * p_config);
```

## Description

Configure SMP pairing parameters.

### Note

MITM \(man-in-the-middle protection\) is requested if ioCapability is other than NoInputNoOutput.<br />If this API is not called then following are the defaults:

-   ioCapability : **[BLE\_SMP\_IO\_NOINPUTNOOUTPUT](GUID-45146E7D-EC04-4A22-BA0E-0F6CAD3858A7.md)**

-   oob: Set false to indicate device does not have OOB authentication data.

-   authReqFlag : **[BLE\_SMP\_OPTION\_BONDING](GUID-365CC6A9-9C72-4FCB-ADEC-405CF68B0426.md)** \| **[BLE\_SMP\_OPTION\_SECURE\_CONNECTION](GUID-365CC6A9-9C72-4FCB-ADEC-405CF68B0426.md)**

-   scOnly: Set true to enable LE secure connection only.


## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] p\_config|Pointer to the **[BLE\_SMP\_Config\_T](GUID-238354B2-CA7F-4A8F-B3A3-002AB74ACAA8.md)** structure buffer.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully configure SMP pairing parameters.|
|MBA\_RES\_INVALID\_PARA|The configured parameter\(s\) is/are invalid.|

