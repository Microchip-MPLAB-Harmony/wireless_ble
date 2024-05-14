![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Detailed Changes

## Wireless BLE Release v1.3.0

### PIC32CX-BZ2
+ **BLE Stack Lib**
    + Change the type/name of fields in the following structures
		+ BT_SYS_Cfg_T
		+ BT_SYS_Option_T
		+ BLE_SMP_PairInfo_T
		+ BLE_SMP_EncInfo_T
		+ BLE_SMP_EvtGenScOobDataDone_T
	+ Change names of definitions in the following groups
		+ ATT_ERROR_CODES
		+ HCI_OCF_LE
		+ HCI_LE_META_SUB_EVT_CODE
		+ HCI_CMD_SIZE
		+ HCI_LE_META_SUB_EVT_SIZE
		+ HCI_CC_EVT_SIZE
		+ GAP_STATUS
		+ BLE_GAP_ADV_TYPE
		+ BLE_GAP_CP_RANGE
		+ BLE_GAP_INIT_FP
		+ BLE_GAP_ADV_REPORT_EVT_TYPE
		+ BLE_GAP_EXT_ADV_DATA_FRAG_PREF
		+ BLE_GAP_EXT_PRIMARY_ADV_INTERVAL
		+ BLE_GAP_PRIODIC_ADV_INTERVAL
		+ BLE_GAP_EXT_ADV_REPORT_EVT_TYPE
		+ BLE_GAP_EXT_ADV_REPORT_DATA_STATUS
		+ BLE_GAP_EXT_ADV_REPORT_PHY
		+ BLE_GAP_TRANSMITTER_PHY_TYPE
		+ BLE_DTM_TRANSMITTER_PAYLOAD_TYPE
		+ BLE_DTM_TRANSMITTER_PHY_TYPE
		+ BLE_DTM_RECEIVER_PHY_TYPE
	+ Change the structure name of GATTC_DiscoverPrimaryServiceByUuidParams_T to GATTC_DiscoverPrimaryServiceParams_T
	+ Disclose additional error codes to clarify issues in the group of STACK_ERR_CODE
		+ MBA_RES_AUTHENTICATION_FAILURE
		+ MBA_RES_PIN_OR_KEY_MISSING
		+ MBA_RES_CONN_TIMEOUT
		+ MBA_RES_CONN_REJ_LIMITED_RESOURCES
		+ MBA_RES_CONN_REJ_SECURITY_REASONS
		+ MBA_RES_REMOTE_USER_TERM_CONN
		+ MBA_RES_PAIRING_NOT_ALLOWED
		+ MBA_RES_INSUFFICIENT_SECURITY
		+ MBA_RES_ADVERTISING_TIMEOUT
		+ MBA_RES_CONN_TERM_MIC_FAILURE
	+ Change the name of enumerators in enum:
		+ BLE_L2CAP_EventId_T
	+ Increase the number of maximum connection from 6 to 8
		+ BLE_GAP_MAX_LINK_NBR
	+ Remove the group of BLE_GAP_ENCRYPT_STATUS
	+ Add definitions which are grouped as BLE_GAP_PATH_LOSS_ZONE for the field of zoneEntered in BLE_GAP_EvtPathLossThreshold_T
	+ Add an event in BLE_GAP_EventId_T
		+ BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL
	+ Make the style of the declaration of the following structure uniform with others
		+ BLE_GAP_ScanFilterMsd_T
	    + BLE_GAP_ScanFilterServiceData_T
		+ BLE_GAP_ScanFilterName_T
	    + BLE_GAP_PathLossReportingParams_T
		+ BLE_GAP_ScanFilterConfig_T
	+ Change the structure name of BLE_GAP_SetPathLossReportingParams_T to BLE_GAP_PathLossReportingParams_T
+ **BLE Middleware**
	+ Change the type/name of fields in the following structures
		+ BLE_DD_Config_T
		    + BLE_DM_EvtSecuritySuccess_T
		    + BLE_DM_PairedDevInfo_T
	+ Change the type of parameters in the following functions
			+ BLE_DM_ProceedSecurity

+ **BLE Profile/Service**
	+ Change the type/name of fields in the following structures
		+ BLE_TRSPS_Event_T
		+ BLE_PXPM_DescList_T
		+ BLE_ANPC_DescList_T
		+ BLE_ANCS_NtfyAttrsMask_T
		+ BLE_ANCS_AppAttrsMask_T
		+ BLE_ANCS_NtfyEvtFlagMask_T
		+ BLE_ANCS_EvtErrAttrBufInd_T
    + Remove the following functions:
		+ BLE_TRSPS_ChangeUuids
	+ Add more restriction about the exposure of related functions by the following macro
		+ BLE_PXPM_IAS_ENABLE
		+ BLE_PXPM_TPS_ENABLE
	+ Add more following macro to restrict the exposure of related functions:
		+ HIDS_KEYBOARD_SUPPORT
		+ HIDS_BOOT_PROTOCOL_MODE_SUPPORT
	+ Change the name of parameters in the following functions
		+ BLE_ANPS_SendNewAlert


### PIC32CX-BZ3
+ **BLE Stack Lib**
	+ Change the type/name of fields in the following structures
		+ BT_SYS_Cfg_T
		+ BT_SYS_Option_T
		+ HCI_LE_SetExtAdvertisingData_T
		+ HCI_LE_SetExtScanRespData_T
		+ HCI_LE_SetPeriodicAdvertisingData_T
		+ BLE_SMP_PairInfo_T
		+ BLE_SMP_EncInfo_T
		+ BLE_SMP_EvtGenScOobDataDone_T
	+ Change names of definitions in the following groups
		+ ATT_ERROR_CODES
		+ HCI_OCF_LE
		+ HCI_LE_META_SUB_EVT_CODE
		+ HCI_CMD_SIZE
		+ HCI_LE_META_SUB_EVT_SIZE
		+ HCI_CC_EVT_SIZE
		+ GAP_STATUS
		+ BLE_GAP_ADV_TYPE
		+ BLE_GAP_CP_RANGE
		+ BLE_GAP_INIT_FP
		+ BLE_GAP_ADV_REPORT_EVT_TYPE
		+ BLE_GAP_EXT_ADV_DATA_FRAG_PREF
		+ BLE_GAP_EXT_PRIMARY_ADV_INTERVAL
		+ BLE_GAP_PRIODIC_ADV_INTERVAL
		+ BLE_GAP_EXT_ADV_REPORT_EVT_TYPE
		+ BLE_GAP_EXT_ADV_REPORT_DATA_STATUS
		+ BLE_GAP_EXT_ADV_REPORT_PHY
		+ BLE_GAP_TRANSMITTER_PHY_TYPE
		+ BLE_DTM_TRANSMITTER_PAYLOAD_TYPE
		+ BLE_DTM_TRANSMITTER_PHY_TYPE
		+ BLE_DTM_RECEIVER_PHY_TYPE
	+ Change the structure name of GATTC_DiscoverPrimaryServiceByUuidParams_T to GATTC_DiscoverPrimaryServiceParams_T
		+ Add the following definitions in the group of STACK_ERR_CODE
		+ MBA_RES_AUTHENTICATION_FAILURE
		+ MBA_RES_PIN_OR_KEY_MISSING
		+ MBA_RES_CONN_TIMEOUT
		+ MBA_RES_CONN_REJ_LIMITED_RESOURCES
		+ MBA_RES_CONN_REJ_SECURITY_REASONS
		+ MBA_RES_REMOTE_USER_TERM_CONN
		+ MBA_RES_PAIRING_NOT_ALLOWED
		+ MBA_RES_INSUFFICIENT_SECURITY
		+ MBA_RES_ADVERTISING_TIMEOUT
		+ MBA_RES_CONN_TERM_MIC_FAILURE
	+ Change the name of enumerators in enum:
		+ BLE_L2CAP_EventId_T
	+ Increase the number of maximum connection from 6 to 8
		+ BLE_GAP_MAX_LINK_NBR
	+ Remove the group of BLE_GAP_ENCRYPT_STATUS
	+ Add an event in BLE_GAP_EventId_T
		+ BLE_GAP_EVT_FEATURE_EXCHANGE_COMPL
	+ Make the style of the declaration of the following structure uniform with others
		+ BLE_GAP_ScanFilterMsd_T
		+ BLE_GAP_ScanFilterServiceData_T
		+ BLE_GAP_ScanFilterName_T
		+ BLE_GAP_PathLossReportingParams_T
		+ BLE_GAP_ScanFilterConfig_T
	+ Change the structure name of BLE_GAP_SetPathLossReportingParams_T to BLE_GAP_PathLossReportingParams_T

+ **BLE Middleware**
	+ Change the type/name of fields in the following structures
		+ BLE_DD_Config_T
		+ BLE_DM_EvtSecuritySuccess_T
		+ BLE_DM_PairedDevInfo_T
	+ Change the type of parameters in the following functions
		+ BLE_DM_ProceedSecurity
+ **BLE Profile/Service**
	+ Change the type/name of fields in the following structures
		+ BLE_TRSPS_Event_T
		+ BLE_PXPM_DescList_T
		+ BLE_ANPC_DescList_T
		+ BLE_ANCS_NtfyAttrsMask_T
		+ BLE_ANCS_AppAttrsMask_T
		+ BLE_ANCS_NtfyEvtFlagMask_T
		+ BLE_ANCS_EvtErrAttrBufInd_T
	+ Remove the following functions:
		+ BLE_TRSPS_ChangeUuids
	+ Add more restriction about the exposure of related functions by the following macro
		+ BLE_PXPM_IAS_ENABLE
		+ BLE_PXPM_TPS_ENABLE
	+ Add more following macro to restrict the exposure of related functions
		+ HIDS_KEYBOARD_SUPPORT
		+ HIDS_BOOT_PROTOCOL_MODE_SUPPORT
	+ Change the name of parameters in the following functions
		+ BLE_ANPS_SendNewAlert

