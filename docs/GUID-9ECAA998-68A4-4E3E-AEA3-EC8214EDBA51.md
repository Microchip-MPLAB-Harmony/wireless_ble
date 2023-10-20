# GATTC\_Read

**Parent topic:**[Functions](GUID-AA412A66-C329-47A0-BB6A-362B8F7A62FE.md)

## C

```c
uint16_t GATTC_Read(uint16_t connHandle, uint16_t charHandle, uint16_t valueOffset);
```

## Description

GATT Client initiates a read operation to GATT Server when this API is called.<br />This API should be used to read a Characteristic Value or Characteristic descriptor.

### Note

Read long attributes:

-   If the attribute is not long attribute, valueOffset should be set to 0 and Read request is sent to<br />GATT Server by this API.

-   If the attribute is a long attribute then first \(ATT\_MTU-1\) bytes are read with valueOffset set to 0<br />\(API uses Read Request\) and rest of the bytes are read using appropriate Non-zero valueOffset \(API<br />uses Read Blob Request\). The Read Blob Request should be initiated in the application by calling the API<br />**GATTC\_Read** with non-zero valueOffset when the event **[GATTC\_EVT\_READ\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** is received.<br />This should be repeated each time the event **[GATTC\_EVT\_READ\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** is received until expected number<br />of bytes of the long attribute are read.


## Events generated

**[GATTC\_EVT\_ERROR\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** Generated when fail to read.

**[GATTC\_EVT\_READ\_RESP](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** Generated when read successfully.

**[ATT\_EVT\_TIMEOUT](GUID-20EFFBD2-7D3F-40CA-B85C-8FD3202D9933.md)** Generated when server does not respond the request.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] connHandle|Handle of the connection.|
|\[in\] charHandle|Handle of the characteristic value or descriptor to be read.|
|\[in\] valueOffset|Non-zero offset value to initiate a read blob request.|

## Return values

|Return value|Description|
|------------|-----------|
|MBA\_RES\_SUCCESS|Successfully starts the read operation.|
|MBA\_RES\_OOM|Internal memory allocation failure.|
|MBA\_RES\_INVALID\_PARA|Invalid parameters. Connection handle is not valid.|
|MBA\_RES\_BUSY|GATT Client is busy. Another request is ongoing.|

