![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)


# Microchip MPLAB® Harmony 3 Release Notes

## Wireless BLE Release v1.5.0

This release includes additions and improvements to the PIC32CX-BZ2, PIC32CX-BZ3, PIC32CX-BZ6, and the new PIC32CX-BZ36 device families, with support for the PIC32WM-BW1.

### New Features
+ **PIC32CX-BZ2/PIC32CX-BZ3**
	+ BT5.2: LE Path Loss Monitoring
	+ BT5.3:
		+ Connection Subrating
		+ AdvDataInfo in Periodic Advertising
		+ LE Channel Classification
		+ LL privacy update - Address change on advertising data change
	+ BT5.4:
		+ Advertising Coding Selection
		+ Encrypted Advertising Data
		+ Security Level Characteristics
	+ Support for Extended Advertising with deep sleep mode
	+ Support for modularized HCI initialization
	+ New API/Event for Connection Subrating:
		+ BLE_GAP_SetDefaultSubrat
		+ BLE_GAP_SubrateRequest
		+ BLE_GAP_EVT_SUBRATE_CHANGE
	+ New options for AdvDataInfo in Periodic Advertising
		+ BLE_GAP_PERIODIC_ADV_DATA_OPERAION
		+ BLE_GAP_PA_ENABLE_OPTION
		+ BLE_GAP_PA_ENABLE_RX_OPTION
	+ New API for LE Channel Classification:
		+ BLE_GAP_ReadAfhChannelAssessMode
		+ BLE_GAP_WriteAfhChannelAssessMode
	+ New options for Advertising Coding Selection:
		+ priPhyOptions
		+ secPhyOptions
	+ New API/Event for Encrypted advertising data:
		+ GATTS_EncDataKeyMatlRsp
		+ GATTS_UpdateEncDataKeyMatl
		+ GATTS_EVT_ENC_DATA_KEY_MATL_REQ
		+ charEncDataKeyMatl
		+ MW_MISC_EncryptAdvData
		+ MW_MISC_DecryptAdvData
	+ New options for Security Level Characteristics:
		+ charLeGattSecLvls
	+ New API for modularized HCI initialization:
		+ HCI_AdvInit
		+ HCI_ExtAdvInit
		+ HCI_PeriodicAdvInit
		+ HCI_ScanInit
		+ HCI_ExtScanInit
		+ HCI_SyncInit
		+ HCI_ConnCentralInit
		+ HCI_ExtConnCntrlInit
		+ HCI_ConnPeripheralInit
	+ New API for Bluetooth Core version configuation:
		+ BT_SYS_UpgradeBleVersion

+ **PIC32CX-BZ6**
	+ BT5.2: LE Path Loss Monitoring
	+ BT5.3:
		+ LL privacy update - Address change on advertising data change
	+ Support for Extended Advertising with deep sleep mode
	+ Support for modularized HCI initialization
	+ New API for modularized HCI initialization:
		+ HCI_AdvInit
		+ HCI_ExtAdvInit
		+ HCI_PeriodicAdvInit
		+ HCI_ScanInit
		+ HCI_ExtScanInit
		+ HCI_SyncInit
		+ HCI_ConnCentralInit
		+ HCI_ExtConnCntrlInit
		+ HCI_ConnPeripheralInit

+ **PIC32CX-BZ36**
	+ Initial Bluetooth Low Energy software support for the PIC32CX-BZ36 / PIC32WM-BZ36x families 32-bit microcontrollers
	+ All supported features are aligned with those of the PIC32CX-BZ3 devices

### Improvements
+ **PIC32CX-BZ2/PIC32CX-BZ3**
	+ Optimize data memory usage
	+ Improve vulnerability robustness by disconnecting upon detection of malicious behavior
	+ Optimize the procedure for minimum number of used channels
	+ Reduce advertising power consumption in sleep mode
	+ Support public and random static addresses in the second advertising set
	+ SMP initiator now checks the security level between the peripheral’s security request and the pairing response

+ **PIC32CX-BZ6**
	+ Optimize data memory usage
	+ Improve vulnerability robustness by disconnecting upon detection of malicious behavior
	+ Optimize the procedure for minimum number of used channels
	+ Reduce advertising power consumption in sleep mode
	+ Support public and random static addresses in the second advertising set

### Bug Fixes
+ **PIC32CX-BZ2/PIC32CX-BZ3**
	+ Improve channel management and coexistence (channel misuse, missing MAP/STATUS indications, PTA handling)
	+ Enhance connection stability across long-hour tests, LLCP collisions, smart phone interoperability, and legacy advertising scenarios
	+ Fix issues with multi-link BLE + 15.4 combo support and parameter handling (LL_CONN_PARAM_REQ, Data Length Update)
	+ Address potential BusFault and LLCP collision issues to improve robustness
	+ Fix ADV name filter not working properly when using BLE_GAP_SetScanningFilter
	+ Fix ATT CCCD list malfunction when a new service is added under an existing BLE connection
	+ Fix pairing procedure not being prohibited from execution before the current procedure was completed
	+ Fix GATT server malfunction when responding to ATT_READ_BY_TYPE_REQ with the default ATT MTU length

+ **PIC32CX-BZ6**
	+ Improve channel management and coexistence (channel misuse, missing MAP/STATUS indications, PTA handling)
	+ Enhance connection stability across long-hour tests, LLCP collisions, smart phone interoperability, and legacy advertising scenarios
	+ Fix issues with multi-link BLE + 15.4 combo support and parameter handling (LL_CONN_PARAM_REQ, Data Length Update)
	+ Address potential BusFault and LLCP collision issues to improve robustness
	+ Fix ADV name filter not working properly when using BLE_GAP_SetScanningFilter
	+ Fix ATT CCCD list malfunction when a new service is added under an existing BLE connection
	+ Fix pairing procedure not being prohibited from execution before the current procedure was completed

### Known Issues/Limitations
+ **PIC32CX-BZ2**
	+ With multiple BLE links, if Wi-Fi has higher priority and BLE cannot obtain a PTA grant for an extended period, BLE connections may be lost. (#5489)
	+ With multiple BLE roles and heavy traffic, the central role may lose connection if there are insufficient used channels. (#5499)

+ **PIC32CX-BZ3/PIC32CX-BZ36**
	+ With multiple BLE links, if Wi-Fi has higher priority and BLE cannot obtain a PTA grant for an extended period, BLE connections may be lost. (#1722)

+ **PIC32CX-BZ6**
	+ With multiple BLE links, if Wi-Fi has higher priority and BLE cannot obtain a PTA grant for an extended period, BLE connections may be lost. (#762)

### Development Tools
+ [MPLAB X v6.25](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.5.2 and above


## Wireless BLE Release v1.4.0

This release includes support for the new PIC32CX-BZ6 device.

### New Features
+ **PIC32CX-BZ6**
	+ This release includes initial Bluetooth Low Energy software support for PIC32CX-BZ6/PIC32WM-BZ6x families of 32-bit microcontrollers.

### Improvements
+ **PIC32CX-BZ2**
	+ Support for the SW PTA multi-protocol combo application.

+ **PIC32CX-BZ3**
	+ Support for the SW PTA multi-protocol combo application.

### Known Issues/Limitations
+ **PIC32CX-BZ2**
	+ Channel classification may fail and result in the use of all channels in particularly noisy environments. (#4953)

+ **PIC32CX-BZ3**
	+ Channel classification may fail and result in the use of all channels in particularly noisy environments. (#1441)

+ **PIC32CX-BZ6**
	+ Memory leakage might occur in the central role with six links during continuous connection and disconnection cycles, preventing the connection event from being sent to the application upon connection establishment. (#543)
	+ Channel classification may fail and result in the use of all channels in particularly noisy environments. (#526)

### Development Tools
+ [MPLAB X v6.20](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.45](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.5.1 and above

## Wireless BLE Release v1.3.0

This release includes additions and improvements to PIC32CX-BZ2 and PIC32CX-BZ3 devices.

### New Features
+ **PIC32CX-BZ2**
	+ Support for a maximum of eight concurrent connections.
	+ Support for WBZ451HPA high power module. (Refer to BT_SYS_HpaInit API)
	+ New option to disable LE Power Control feature. (Refer to BT_SYS_Option_T)

+ **PIC32CX-BZ3**
	+ Support for a maximum of eight concurrent connections.
	+ New option to disable LE Power Control feature. (Refer to BT_SYS_Option_T)

### Improvements
+ **PIC32CX-BZ2**
	+ Multilink enhancement.
	+ AFH channel classification enhancement.
	+ LE Power control enhancement.
	+ Disclose additional error codes to clarify issues.
	+ Revise the names of several APIs, events, and definitions for enhancement and MISRA C compliance fixes. [See detailed changes.](driver/ble/detailed_changes.md)

+ **PIC32CX-BZ3**
	+ Multilink enhancement.
	+ AFH channel classification enhancement.
	+ LE Power control enhancement.
	+ Disclose additional error codes to clarify issues.
	+ Revise the names of several APIs, events, and definitions for enhancement and MISRA C compliance fixes. [See detailed changes.](driver/ble/detailed_changes.md)

### Bug Fixes
+ **PIC32CX-BZ2**
	+ LE TX power may decrease suddenly after PHY changed. (#2443)
	+ LE Peripheral role may loss connection due to instant passed under multiple role scenario. (#2293)
	+ The LE central role processing two LLCP procedures, including one LL_CONNECTION_UPDATE_IND, in the same connection event might cause a bus fault. (#3782)
	+ Code execution may stuck in AES function if the project is generated by MCC. (#2850)

+ **PIC32CX-BZ3**
	+ LE TX power may decrease suddenly after PHY changed. (#626)
	+ LE Peripheral role may loss connection due to instant passed under multiple role scenario. (#627)
	+ The lenght of advertising data in BLE_GAP_SetExtAdvData, BLE_GAP_SetExtScanRspData and BLE_GAP_SetPeriAdvData is limited to 251 bytes.
	+ The LE central role processing two LLCP procedures, including one LL_CONNECTION_UPDATE_IND, in the same connection event might cause a bus fault. (#1143)

### Known Issues/Limitations
+ **PIC32CX-BZ2**
	+ The connection might be terminated under multiple-link conditions due to MIC error. (#3534)
	+ The connection might be terminated under multiple-link conditions due to the missing reception of the LL_CHANNEL_MAP_IND PDU if DUT plays as peripheral role. (#3548)
	+ The combo system with the central role will fail to establish the sixth BLE connection after Zigbee provisioning. (#3890)

+ **PIC32CX-BZ3**
	+ The connection might be terminated under multiple-link conditions due to the missing reception of the LL_CHANNEL_MAP_IND PDU if DUT plays as peripheral role. (#1125)
	+ The connection might be terminated under multiple-link conditions due to RF bandwidth conflicts when the DUT plays the role of a BLE peripheral. (#1152)

### Development Tools
+ [MPLAB X v6.20](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.40](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.5.0 and above

---
## Wireless BLE Release v1.2.0

This release includes support for the new PIC32CX-BZ3 device only.

### New Features
+ **PIC32CX-BZ3**
	+ This release includes initial Bluetooth Low Energy software support for PIC32CX-BZ3/WBZ35x families of 32-bit microcontrollers.

### Known Issues/Limitations
+ **PIC32CX-BZ3**
	+ Tx power may decrease suddenly after PHY changed. (#626)
	+ Peripheral role may loss connection due to instant passed under multiple role scenario(#627)
	+ The lenght of advertising data in BLE_GAP_SetExtAdvData, BLE_GAP_SetExtScanRspData and BLE_GAP_SetPeriAdvData is limited to 251 bytes.

### Development Tools
+ [MPLAB X v6.15](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.3.7 and above


---
## Wireless BLE Release v1.1.0

This release includes additions and improvements to PIC32CX-BZ2 device only.

### New Features
+ **PIC32CX-BZ2**
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
	+ Add local IRK and address information in paired device (DM)
   	+ New API for getting GATT discovery information (profiles)
   		+ XXX_GetCharList
   		+ XXX_GetDescList
   	+ Support OTA profile v1.2


### Improvements
+ **PIC32CX-BZ2**
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
	+ Issue BLE_DM_EVT_PAIRED_DEVICE_UPDATED event after paired device information was stored successfully (DM)

### Bug Fixes
+ **PIC32CX-BZ2**
	+ Connection update with latency may cause link loss due to instant passed (#1172)
	+ while GAP SetPhy API is called, Coded PHY option (S=2 or S=8) cannot be chanaged if present PHY type is Coded (#1596)
	+ Advertiser with some Public Device Address setting will not be discovered by Observer(#2407)


### Known Issues/Limitations
+ **PIC32CX-BZ2**
	+ Tx power may decrease suddenly after PHY changed. (#2443)
	+ Peripheral role may loss connection due to instant passed under multiple role scenario(#2293)


### Development Tools
+ [MPLAB X v6.10](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.21](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.3.7 and above


---
## Wireless BLE Release v1.0.0

This release includes support for the new PIC32CX-BZ2 device only.

### New Features
+ **PIC32CX-BZ2**
	+ This release includes initial Bluetooth Low Energy software support for PIC32CX-BZ2/WBZ45x families of 32-bit microcontrollers.

### Known Issues/Limitations
+ **PIC32CX-BZ2**
	+ Using the same Advertising Random Address for 2 different Advertising handles (#1338)


### Development Tools
+ [MPLAB X v6.05](https://www.microchip.com/mplab/mplab-x-ide)
+ [MPLAB® XC32 C/C++ Compiler v4.10](https://www.microchip.com/mplab/compilers)
+ MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.2.2 and above