# Attribute settings

## C

```c
#define SETTING_MANUAL_WRITE_RSP                            0x01
#define SETTING_MANUAL_READ_RSP                             0x02
#define SETTING_UUID_16                                     0x04
#define SETTING_VARIABLE_LEN                                0x08
#define SETTING_ALLOW_OFFSET                                0x10
#define SETTING_CCCD                                        0x20
```

## Description

The definition of attribute settings.

## Macro Definition

|Macro|Description|
|:---|:---|
|SETTING_MANUAL_WRITE_RSP|Set if service need to know write operations on this attribute. Service also need to send response (if need) manually.|
|SETTING_MANUAL_READ_RSP|Set if service need to know read operations on this attribute. Service also need to send response manually.|
|SETTING_UUID_16|Set if the UUID is 16 bytes (128 bits) in length|
|SETTING_VARIABLE_LEN|Set if the attribute has a variable length|
|SETTING_ALLOW_OFFSET|Set if writes are allowed with an offset|
|SETTING_CCCD|Set if the attribute is CCCD|
