# General Error Handling

This is the start of your topic. The OTA Client shall be tolerant and behave appropriately \(i.e.,<br /> the OTA Client shall be able to continue to process operations and/or receive data<br /> normally\) when receiving the Result Codes, ATT Application Error Codes, and ATT Error<br /> Codes defined in the OTA Service [\[1\]](GUID-6B274602-B628-48D8-9345-D483824E66AC.md).

If the OTA Client reads a characteristic with Reserved for Future Use \(RFU\) bits that are<br /> non-zero \(i.e., RFU bits in the OTA Feature characteristic\), it shall ignore those bits<br /> and continue to process the characteristic as if all the RFU bits had been zero.

**Parent topic:**[OTA Client Role Requirements](GUID-B2F97739-5023-45D6-9D0C-B935040A232B.md)

