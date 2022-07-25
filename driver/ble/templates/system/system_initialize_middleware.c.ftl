    // Create BLE Stack Message QUEUE
    OSAL_QUEUE_Create(&bleRequestQueueHandle, QUEUE_LENGTH_BLE, QUEUE_ITEM_SIZE_BLE);

    // Retrieve BLE calibration data
    btSysCfg.addrValid = IB_GetBdAddr(&btSysCfg.devAddr[0]);
    btSysCfg.rssiOffsetValid =IB_GetRssiOffset(&btSysCfg.rssiOffset);
<#if USE_CUSTOM_ANT_GAIN>
    btSysCfg.antennaGainValid = true;
    btSysCfg.antennaGain = CUSTOM_ANT_GAIN;
<#else>
    btSysCfg.antennaGainValid = IB_GetAntennaGain(&btSysCfg.antennaGain);
</#if>

<#if APP_BLE_DEV == "Buckland">
    //Configure BLE option
    btOption.cmnMemSize = EXT_COMMON_MEMORY_SIZE;
    btOption.p_cmnMemAddr = OSAL_Malloc(btOption.cmnMemSize);
</#if>

    // Initialize BLE Stack
    <#if APP_BLE_DEV == "Chimera">
    BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, NULL, &btSysCfg);
    </#if>    
    <#if APP_BLE_DEV == "Buckland">
    BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, &btOption, &btSysCfg);
    </#if>
    


