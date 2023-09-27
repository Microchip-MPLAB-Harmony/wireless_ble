# ATT\_Uuid\_T

**Parent topic:**[Structures](GUID-033AEAE3-56F0-4C38-99A5-6315F4885209.md)

## C

```c
typedef struct ATT_Uuid_T
{
    uint8_t uuid[ATT_MAX_UUID_SIZE];
    uint8_t uuidLength;
} ATT_Uuid_T;
```

## Description

UUID parameters.

## Field Documentation

|Field|Description|
|-----|-----------|
|uuid|The UUID.|
|uuidLength|Length of the UUID.|

