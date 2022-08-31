# GATT client configuration definitions

## C

```c
#define GATTC_CONFIG_NONE                                   0x0000
#define GATTC_CONFIG_MANUAL_CONFIRMATION                    0x0001
```

## Description

The definition of of GATT Client configuration bits

## Macro Definition

|Macro|Description|
|:---|:---|
|GATTC_CONFIG_NONE|Not configure anyone of the supported function.|
|GATTC_CONFIG_MANUAL_CONFIRMATION|bit 0: Configure ATT stack for auto/manual transmission of confirmation when an indication is received.|
