# BLE\_GAP\_SetPathLossReportingParams\_T

**Parent topic:**[Structures](GUID-230368B0-FB2A-4967-A471-691387B35A9E.md)

## C

```c
typedef struct
{
    uint16_t 				connHandle;
    uint8_t  				highThreshold;
    uint8_t  				highHysteresis;
    uint8_t  				lowThreshold;
    uint8_t  				lowHysteresis;
    uint16_t 				minTimeSpent;
} BLE_GAP_SetPathLossReportingParams_T;
```

## Description

Set path loss reporting parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|highThreshold|High threshold for the path loss. \(Unit: dBm\)|
|highHysteresis|Hysteresis value for the high threshold. \(Unit: dBm\)|
|lowThreshold|Low threshold for the path loss. \(Unit: dBm\)|
|lowHysteresis|Hystersis value for the low threshold. \(Unit: dBm\)|
|minTimeSpent|Minimum time in number of connection events to be observed once the path crosses the threshold before an event is generated.|

