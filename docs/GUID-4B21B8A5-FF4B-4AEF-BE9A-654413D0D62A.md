# BLE\_LOG\_EventCb\_T

**Parent topic:**[Structures](GUID-FDE2FF00-C7B3-4400-8915-2529E317E0CD.md)

## C

```c
typedef void (*BLE_LOG_EventCb_T)(uint8_t logType, uint16_t logLength, uint8_t *p_logPayload);
```

## Description

BLE\_LOG callback type. This callback function sends BLE\_LOG events to the application.

