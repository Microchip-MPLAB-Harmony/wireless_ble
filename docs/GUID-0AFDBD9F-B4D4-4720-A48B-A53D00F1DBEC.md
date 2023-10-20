# GATTS\_Attribute\_T

**Parent topic:**[Structures](GUID-3BBA6E22-85EE-4B8F-BC37-840881963D97.md)

## C

```c
typedef struct GATTS_Attribute_T
{
    uint8_t     *p_uuid;
    uint8_t     *p_value;
    uint16_t    *p_len;
    uint16_t    maxLen;
    uint8_t     settings;
    uint8_t     permissions;
} GATTS_Attribute_T;
```

## Description

GATT attribute.

## Field Documentation

|Field|Description|
|-----|-----------|
|p\_uuid|Pointer to the attribute's UUID \(attribut type\).|
|p\_value|Pointer to the attribute's value.|
|p\_len|Pointer to the length of the attribute's value.|
|maxLen|Maximum length of attribute's value.|
|settings|Attribute settings.|
|permissions|Attribute permissions.|

