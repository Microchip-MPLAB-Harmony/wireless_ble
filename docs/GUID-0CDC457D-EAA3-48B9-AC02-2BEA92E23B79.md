# Firmware Update Start Operation

This procedure is used by OTA Client when it attempts to request OTA Server to start OTA<br /> firmware update process.

The OTA Client shall write the OTACP*Firmware Update Start*opcode to the OTACP<br /> with the image type.

If the response to the *Firmware Update Start*opcode is not â€œSuccessâ€?, OTA Client<br /> may issue Firmware Update Start operation again with valid parameter or issue the<br /> Firmware Update Complete operation to stop the OTA firmware update process.

If the response to the *Firmware Update Start*opcode is â€œSuccessâ€?, OTA Client shall<br /> be able to transmit image via OTA Data characteristic.

**Parent topic:**[OTA Firmware Update Procedures](GUID-91D6F8AA-2F12-40CF-B412-CC4CDC41A3C6.md)

