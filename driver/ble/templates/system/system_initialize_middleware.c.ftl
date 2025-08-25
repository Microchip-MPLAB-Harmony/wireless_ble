    // Create BLE Stack Message SEM
    OSAL_SEM_Create(&bleRequestSemHandle, OSAL_SEM_TYPE_BINARY, 0, 0);

    // Retrieve BLE calibration data
    (void)memset(&btSysCfg, 0, sizeof(BT_SYS_Cfg_T));
    btSysCfg.addrValid = IB_GetBdAddr(&btSysCfg.devAddr[0]);
    btSysCfg.rssiOffsetValid =IB_GetRssiOffset(&btSysCfg.rssiOffset);

    if (!IB_GetAntennaGain(&btSysCfg.antennaGain))
    {
        btSysCfg.antennaGain = ${BLE_ANTENNA_GAIN};
    }

    <#if (APP_BLE_DEVICE == "pic32cx_bz3_family") || (APP_BLE_DEVICE == "pic32cx_bz36_family") || (APP_BLE_DEVICE == "pic32cx_bz6_family")>
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
    btOption.deFeatMask = (BT_SYS_FEAT_CHC);

    // Initialize BLE Stack
    BT_SYS_Init(&bleRequestSemHandle, &osalAPIList, &btOption, &btSysCfg);
    <#if (APP_BLE_DEVICE == "pic32cx_bz2_family") || (APP_BLE_DEVICE == "pic32cx_bz3_family") || (APP_BLE_DEVICE == "pic32cx_bz36_family")>
        <#if BLE_SYS_BT_VER_SUPP == "BT_VER_6_0">
    BT_SYS_UpgradeBleVersion(BT_SYS_BLE_VER_6_0);
        </#if>
    </#if>