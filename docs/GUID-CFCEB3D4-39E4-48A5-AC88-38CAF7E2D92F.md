# GATT client configuration definitions

**Parent topic:**[Defines](GUID-F7CF3BA1-36B6-40A6-9257-19D4863812CA.md)

## C

```c
#define GATTC_CONFIG_NONE                                   0x0000
#define GATTC_CONFIG_MANUAL_CONFIRMATION                    0x0001
```

## Description

The definition of GATT Client configuration bits.

## Macro Definition

|Macro|Description|
|-----|-----------|
|GATTC\_CONFIG\_NONE|Not configure anyone of the supported function.|
|GATTC\_CONFIG\_MANUAL\_CONFIRMATION|bit 0: Configure ATT stack for auto/manual transmission of confirmation when an indication is received.|

