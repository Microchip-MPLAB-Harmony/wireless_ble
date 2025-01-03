<#if DIS_ENABLE_CONFIG == true>
// Configuration of Service DIS
    <#if DIS_MANU_NAME_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_MANU_NAME        "${DIS_MANU_NAME_STRING}"    /* Manufacture Name */
    </#if>
    <#if DIS_MODEL_NUM_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_MODEL_NUM        "${DIS_MODEL_NUM_STRING}"    /* Model Number */
    </#if>
    <#if DIS_SERIAL_NUM_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_SERIAL_NUM       "${DIS_SERIAL_NUM_STRING}"    /* Serial Number */
    </#if>
    <#if DIS_HW_REV_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_HW_REVISION      "${DIS_HW_REV_STRING}"    /* Hardware Revision */
    </#if>
    <#if DIS_FW_REV_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_FW_REVISION      "${DIS_FW_REV_STRING}"    /* Firmware Revision */
    </#if>
    <#if DIS_SW_REV_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_SW_REVISION      "${DIS_SW_REV_STRING}"    /* Software Revision */
    </#if>
    <#if DIS_SYSTEM_ID_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_SYSTEM_ID        ${CHECKED_DIS_SYSTEM_ID_STRING}    /* System Id */
    </#if>
    <#if DIS_IEEE_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_IEEE_11073_20601 ${CHECKED_DIS_IEEE_STRING}    /* IEEE 11073-20601 Data */
    </#if>
    <#if DIS_PNP_ID_ENABLE == true>
#define CONFIG_BLE_SVC_DIS_PNP_ID           ${CHECKED_DIS_PNP_ID_STRING}    /* PnP Id */
    </#if>
</#if>