# BLE_GAP_CharPeriPreferConnParams_T

## C

```c
typedef struct BLE_GAP_CharPeriPreferConnParams_T
{
    bool                    enable;
    uint16_t                minConnInterval;
    uint16_t                maxConnInterval;
    uint16_t                peripheralLatency;
    uint16_t                connSupervisionTimeoutMulti;
} BLE_GAP_CharPeriPreferConnParams_T;
```

## Description

Peripheral prefered connection parameter characteristic.


## Field Documentation

|Field|Description|
|:---|:---|
|enable|If ture to enable the characteristic of "Peripheral Preferred Connection Parameters". Otherwise, disable it.|
|minConnInterval|Defines minimum value for the connection interval. See **[Connection parameters range](GUID-5ABC0266-6BD2-424C-B8AB-3024AE2E9771.md)**.|
|maxConnInterval|Defines maximum value for the connection interval. See **[Connection parameters range](GUID-5ABC0266-6BD2-424C-B8AB-3024AE2E9771.md)**.|
|peripheralLatency|Defines the peripheral latency for the connection. See **[Connection parameters range](GUID-5ABC0266-6BD2-424C-B8AB-3024AE2E9771.md)**.|
|connSupervisionTimeoutMulti|Defines the connection supervisor timeout multiplier as a multiple of 10ms.|
