# Service Characteristics

This section contains characteristics and behavior that are defined in the OTA<br /> procedures.

**OTA Control Point**

Before performing an OTA procedure, the OTA Client shall configure the OTACP<br /> characteristic for notifications \(i.e., via the Client Characteristic Configuration<br /> descriptor\).

When OTA Client wants to send data to OTA Server, the OTA Client shall write a<br /> characteristic value to the OTACP Characteristic. The Server may respond it with a<br /> notification by Response Code that may include the Request Opcode and may also include a<br /> Response Parameter as defined in [\[1\]](GUID-6B274602-B628-48D8-9345-D483824E66AC.md).

Below shows the requirements for the OTACP opcode in the context of this profile:

<br />

|**OTAP opcode**|**Requirement**|
|---------------|---------------|
|Firmware Update Request|Mandatory if Firmware Update Request procedure is<br /> supported.|
|Firmware Update Start|Mandatory if Firmware Update Start procedure is<br /> supported.|
|Firmware Update Complete|Mandatory if Firmware Update Complete procedure is<br /> supported.|
|Device Reset Request|Mandatory if Device Reset Request procedure is supported;<br /> otherwise optional.|

<br />

**Parent topic:**[OTA Client Role Requirements](GUID-B2F97739-5023-45D6-9D0C-B935040A232B.md)

