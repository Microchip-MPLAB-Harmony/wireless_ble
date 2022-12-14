# GATTS_GetHandleValue

## C

```c
uint16_t GATTS_GetHandleValue(uint16_t attrHandle, uint8_t *p_attrValue, uint16_t *p_attrLength);
```

## Description

Gets the attribute value of a valid attribute from GATT Server. If the attribute is permitted to read, value will be return
in application provided buffer.

### Note

The format of the attribute value is raw-data.

## Parameters

|Parameter|Description|
|:---|:---|
|\[in\] attrHandle|Attribute Handle.|
|\[out\] p_attrValue|Pointer of buffer to store getting attribute value. Valid if API return status is success.|
|\[in\] p_attrLength|As input parameter, describe size of application provided buffer.  As output parameter, describe actual size of getting attribute value when API return status is success.|

## Return values

|Return value|Description|
|:---|:---|
MBA_RES_SUCCESS|Successfully gets the attribute value.|
MBA_RES_FAIL|Operation is not permitted. The attribute handle is valid but the attribute can't be read.|
MBA_RES_INVALID_PARA|Invalid parameters.|
