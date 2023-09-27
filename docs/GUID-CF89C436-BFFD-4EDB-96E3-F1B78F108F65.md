# BLE\_SMP\_KeyList\_T

**Parent topic:**[Structures](GUID-32B57AF4-FA13-419A-852F-73C4E0457A07.md)

## C

```c
typedef struct BLE_SMP_KeyList_T
{
    BLE_SMP_SecKeys_T       local;
    BLE_SMP_SecKeys_T       remote;
} BLE_SMP_KeyList_T;
```

## Description

Key list.

## Field Documentation

|Field|Description|
|-----|-----------|
|local|Local security keys.|
|remote|Remote security keys.|

