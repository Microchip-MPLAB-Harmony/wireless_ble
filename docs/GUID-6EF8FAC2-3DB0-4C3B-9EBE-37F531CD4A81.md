# Downlink Credit Based Flow Control \(DCBFC\)

The procedure is used by a Transparent Client to enable Downlink Credit Based Flow<br /> Control for the Transparent Server. Once it is enabled, Client role can only send data<br /> to Server while the credit count is greater than zero.

When enabling Downlink Credit Based Flow Control, Client shall write TCP characteristic<br /> handle with â€œDownlink Credit Based Flow Control Enableâ€? opcode \(0x14\) to the Transparent<br /> Control Point on Transparent Server. And the Transparent Server shall send the MTU value<br /> and available credit numbers to Transparent Client by Response Parameters of 0x00<br /> opcode. Transparent Client will check the credit count to know how many data can be<br /> transmitted to the Transparent Server by performing the write command operation to TDD<br /> Characteristic handle. The Credit count will be decreased by the number of ATT packet<br /> sending by Transparent Client. Transparent Client can send data only while the credit<br /> count is greater than zero. Transparent Server shall return the credit to Transparent<br /> Client after the received data has been processed completed.

![](GUID-8027D185-122B-40AC-B920-B5DA8E3E0BA4-low.png)



**Parent topic:**[Transparent Control Point Characteristic Behavior](GUID-7A2BC74D-E714-41F0-8201-0D183867E570.md)

