# Key flags

**Parent topic:**[Defines](GUID-E98A07E1-AB35-4D3D-B408-251AB8127825.md)

## C

```c
#define BLE_SMP_FLAG_ENCRYPT_INFO                           (1<<0)
#define BLE_SMP_FLAG_CENTRAL_ID                             (1<<1)
#define BLE_SMP_FLAG_ID_INFO                                (1<<2)
#define BLE_SMP_FLAG_ID_ADDR_INFO                           (1<<3)
#define BLE_SMP_FLAG_SIGNING_INFO                           (1<<4)
```

## Description

The definition of valid remote keys flag.

## Macro Definition

|Macro|Description|
|-----|-----------|
|BLE\_SMP\_FLAG\_ENCRYPT\_INFO|Indicate LTK is valid.|
|BLE\_SMP\_FLAG\_CENTRAL\_ID|Indicate EDIV and Rand are valid.|
|BLE\_SMP\_FLAG\_ID\_INFO|Indicate IRK is valid.|
|BLE\_SMP\_FLAG\_ID\_ADDR\_INFO|Indicate identity address is valid.|
|BLE\_SMP\_FLAG\_SIGNING\_INFO|Indicate SRK is valid.|

