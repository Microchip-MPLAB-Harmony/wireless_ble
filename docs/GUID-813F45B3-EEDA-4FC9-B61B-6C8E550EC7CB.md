# BLE_SMP_Config_T

## C

```c
typedef struct BLE_SMP_Config_T
{
    uint8_t ioCapability;
    uint8_t authReqFlag;
    bool    oobDataFlag;
    bool    scOnly;
    bool    authPairingRequired;
}BLE_SMP_Config_T;
```

## Description

Pairing parameters.


## Field Documentation

|Field|Description|
|:---|:---|
|ioCapability|Input and output capabilities of a device handle. See **[IO capability](GUID-805B05C3-A591-4CB9-B131-C5600D129ECD.md)**.|
|authReqFlag|Authentication Requirement Flag. See **[Pairing options](GUID-18C06DA4-1B84-4764-8D8B-2AF416ADE471.md)**.|
|oobDataFlag|OOB data flag.                                                                              - In LE legacy pairing, set true to indicate device has OOB authentication data.                                                                             If both devices have OOB authentication data, then OOB pairing method shall be used.                                                                             - In LE Secure Connections pairing, set true to indicate device has the OOB authentication data from remote device.                                                                             If one or both devices have the OOB authentication data from remote device, then OOB pairing method shall be used.|
|scOnly|Secure Connections only mode. Set true to enable secure connection only mode.|
|authPairingRequired|Set true if authenticated pairing method is required. If local device plays as peripheral role in the connection and the final mapping method in pairing is unauthenticated.  Then pairing fail packet will be issued automatically after receiving pairing request packet.|
