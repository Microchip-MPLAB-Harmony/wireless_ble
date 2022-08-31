# OTA Data

The OTA Data characteristic allow client to send specified data to Server. Client shall enable<br /> the Client Characteristic Configuration Descriptor \(CCCD\) of OTA Data on Server before<br /> sending the specified data.

Requirements for this characteristic are defined in [Table 3.1](GUID-57858D1B-EA35-40DE-9714-EBD47F6C09F8.md).

The structure of the OTA Data characteristic is defined in Table 3.4.

<br />

|**Field**|**Requirement**|**Size \(octets\)**|**Description**|
|---------|---------------|-------------------|---------------|
|Fragmented\_Image|C.1|1 to ATT\_MTU-3|The fragmented image.|

<br />

C.1 It’s available only after client issue a Firmware Update Start procedure.

**Parent topic:**[Service Characteristics](GUID-57858D1B-EA35-40DE-9714-EBD47F6C09F8.md)

