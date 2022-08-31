# Firmware Update Start operation

When the Firmware Update Start opcode is written to the OTACP along with image type parameter and<br /> an error condition does not occur, the Server shall start the firmware update<br /> procedure.

The format of the Firmware Update Start operation is defined in Table 4.6.

<br />

|**Opcode**|**Requirement**|**Size \(octets\)**|**Description**|
|----------|---------------|-------------------|---------------|
|0x03|M|1|Firmware Update Start|
|**Opcode**|**Requirement**|**Size \(octets\)**|**Description**|
|Image\_Type|M|1|The image used for firmware update process. 0x01: Firmware image0x02: Metadata All other<br /> values: RFU|

<br />

However, if an error condition occurs, the â€œSuccessâ€? result code shall not be sent and<br /> the appropriate error response shall be sent as defined in Table 4.7.

<br />

|**Error Condition**|**Error Response**|
|-------------------|------------------|
|Requested image type is not supported.|Not Supported|
|The received data length is invalid.|Not Supported|
|The requested operation is not valid in the current state of the<br /> firmware update process.|Invalid State|
|The received image type is not requested in Firmware Update<br /> Request opcode. \(Image size is 0\)|Operation Failed|
|Active flash head is not available|Operation Failed|

<br />

**Parent topic:**[OTA Control Operations](GUID-FA1AB790-BF05-4FBA-B346-9D4CB88158DB.md)

