<#if TRS_ENABLE_CONFIG == true>
// Configuration of Service TRS
#define CONFIG_BLE_SVC_TRS_UUID_MCHP_PROPRIETARY_SERVICE_16        ${CHECKED_TRS_SERVICE_UUID}    /* Service UUID */
#define CONFIG_BLE_SVC_TRS_UUID_MCHP_TRANS_TX_16                   ${CHECKED_TRS_TX_UUID}    /* TX Characteristic UUID */
#define CONFIG_BLE_SVC_TRS_UUID_MCHP_TRANS_RX_16                   ${CHECKED_TRS_RX_UUID}    /* RX Characteristic UUID */
#define CONFIG_BLE_SVC_TRS_UUID_MCHP_TRANS_CTRL_16                 ${CHECKED_TRS_CP_UUID}    /* CP Characteristic UUID */
</#if>