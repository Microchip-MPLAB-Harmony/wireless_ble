    // Create BLE Stack Message QUEUE
    OSAL_QUEUE_Create(&bleRequestQueueHandle, QUEUE_LENGTH_BLE, QUEUE_ITEM_SIZE_BLE);

    // Retrieve BLE calibration data
    (void)memset(&btSysCfg, 0, sizeof(BT_SYS_Cfg_T));
    btSysCfg.addrValid = IB_GetBdAddr(&btSysCfg.devAddr[0]);
    btSysCfg.rssiOffsetValid =IB_GetRssiOffset(&btSysCfg.rssiOffset);

    if (!IB_GetAntennaGain(&btSysCfg.antennaGain))
    {
        btSysCfg.antennaGain = ${BLE_ANTENNA_GAIN};
    }

    <#if APP_BLE_DEVICE == "pic32cx_bz3_family">
    btSysCfg.adcTimingValid =IB_GetAdcTiming(&btSysCfg.adcTiming08, &btSysCfg.adcTiming51);
    </#if>

    //Configure BLE option
    (void)memset(&btOption, 0, sizeof(BT_SYS_Option_T));
<#if BLE_SYS_CTRL_ONLY_EN == false>
    btOption.hciMode = false;
<#else>
    btOption.hciMode = true;
</#if>
    btOption.cmnMemSize = EXT_COMMON_MEMORY_SIZE;
<#if APP_BLE_DEVICE == "pic32cx_bz2_family">
    btOption.p_cmnMemAddr = s_btMem;
<#else>
    //Configure BLE option
    btOption.p_cmnMemAddr = OSAL_Malloc(btOption.cmnMemSize);
</#if>
    btOption.deFeatMask = 0;

    // Initialize BLE Stack
    BT_SYS_Init(&bleRequestQueueHandle, &osalAPIList, &btOption, &btSysCfg);