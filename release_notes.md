![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Wireless BLE Release v1.2.0

### New Features
+ This release includes initial Bluetooth Low Energy software support for PIC32CX-BZ3/WBZ35x families of 32-bit microcontrollers.


### Known Issues
+ Tx power may decrease suddenly after PHY changed. (#626)
+ Peripheral role may loss connection due to instant passed under multiple role scenario(#627)


### Development Tools
+ [MPLAB X v6.15](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.3.7 and above


---
## Wireless BLE Release v1.1.0

### New Features
+ BLE Stack Lib
	+ Support HCI mode
	+ Suuport Three-Wire Packet Traffic Arbitration (PTA) WLAN coexistence solution
	+ Support generating diffirent Random Private Device Address for second Advertising set
	+ Optimize BLE Stack static memory usage
	+ New API/Event for Path Loss Reporting features
		+ BLE_GAP_SetPathLossReportingParams
		+ BLE_GAP_SetPathLossReportingEnable
		+ BLE_GAP_EVT_PATH_LOSS_THRESHOLD
	+ New API for BT system error callback registration
		+ BT_SYS_ErrRegister


+ BLE Middleware
	+ Add local IRK and address information in paired device (DM)


+ BLE Profile/Service
    + New API for getting GATT discovery information (profiles)
    	+ XXX_GetCharList
    	+ XXX_GetDescList
    + Support OTA profile v1.2


### Changes
+ BLE Stack Lib
	+ Reduce multilink collision by moving connection event anchor
    + Change maximum value of RPA timeout to 30 minutes
    + Refine GATTS queue writes mechanism
    + Enlarge range of Bluetooth SIG defined GATT characteristic descriptors
    + Limite maximum number of ble gap Extended Advertising Set to 2
    + Change acceptable value of operation parameter in related API
    	+ BLE_GAP_SetExtAdvData
    	+ BLE_GAP_SetExtScanRspData
    	+ BLE_GAP_SetPeriAdvData 
    + Change the maximum upper value of golden receive power range from -30dBm to -50dBm 
    + Change default Antenna gain value as 5 for WBZ450 device

+ BLE Middleware
	+ Issue BLE_DM_EVT_PAIRED_DEVICE_UPDATED event after paired device information was stored successfully (DM)


### Bug Fixes
+ BLE Stack Lib
	+ Connection update with latency may cause link loss due to instant passed (#1172)
	+ while GAP SetPhy API is called, Coded PHY option (S=2 or S=8) cannot be chanaged if present PHY type is Coded (#1596)
	+ Advertiser with some Public Device Address setting will not be discovered by Observer(#2407)


### Known Issues
+ Tx power may decrease suddenly after PHY changed. (#2443)
+ Peripheral role may loss connection due to instant passed under multiple role scenario(#2293)


### Development Tools
+ [MPLAB X v6.10](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.21](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.3.7 and above


---
## Wireless BLE Release v1.0.0


### New Features
+ This release includes initial Bluetooth Low Energy software support for PIC32CX-BZ2/WBZ45x families of 32-bit microcontrollers.


### Known Issues
+ Using the same Advertising Random Address for 2 different Advertising handles (#1338)


### Development Tools
+ [MPLAB X v6.05](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.10](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.2.2 and above