# Uplink Credit Based Flow Control \(UCBFC\)

The procedure is used by a Transparent Client to enable Uplink Credit Based Flow Control on the<br /> Transparent Server. Once it is enabled, Server role can only send data to Client while<br /> the credit count is greater than zero.

When enable Uplink Credit Based Flow Control, Client shall write â€œUplink Credit Based<br /> Flow Control Enableâ€? opcode \(0x15\) to the Transparent Control Point on Transparent<br /> Server. There are given Credit value parameter in this opcode. Transparent Server will<br /> check the credit count to know how many packets can be uploaded to the Transparent<br /> Client by performing the notify operation to TUD Characteristic handle. The Credit count<br /> will be decreased by the number of ATT packet sending by Transparent Server. Transparent<br /> Server can send data only while the credit count is greater than zero. Transparent<br /> Client shall return the credit value to Transparent Server after the received data has<br /> been processed completed.

When enable Credit Based Flow Control Uplink and available credits on Client shall be<br /> written to the Transparent Control Point on Transparent Server. Transparent Server will<br /> check the number of credit to know how many available data list can transmit to the<br /> Transparent Client. The number of credit is decrease by the list of data sending by<br /> Transparent Server. Transparent Server is able to send data only while the number of<br /> credit is non-zero. Transparent Client shall return the credit to Transparent Server<br /> after the received data has been processed completed.

![](GUID-57939AB4-6EA5-4725-9F5F-2DF4237D7FFC-low.png)



**Parent topic:**[Transparent Control Point Characteristic Behavior](GUID-7A2BC74D-E714-41F0-8201-0D183867E570.md)

