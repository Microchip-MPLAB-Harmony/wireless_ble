# BLE\_SMP\_EncInfo\_T

**Parent topic:**[Structures](GUID-32B57AF4-FA13-419A-852F-73C4E0457A07.md)

## C

```c
typedef struct BLE_SMP_EncInfo_T
{
    uint8_t                 ltk[16];
    uint8_t                 ediv[2];
    uint8_t                 rand[8];
    uint8_t                 lesc:1;
    uint8_t                 auth:1;
    uint8_t                 ltkLen:6;
}BLE_SMP_EncInfo_T;
```

## Description

Encryption information.

## Field Documentation

|Field|Description|
|-----|-----------|
|ltk|The long term key.|
|ediv|The encrypted diversifier value.|
|rand|The randon number value.|
|lesc|Key generated using LE security connection.|
|auth|Is Authenticated Key?|
|ltkLen|The LTK length in bytes.|

