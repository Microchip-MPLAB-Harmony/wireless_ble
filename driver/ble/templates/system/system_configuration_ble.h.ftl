<#if BLE_SYS_CTRL_ONLY_EN == false>
<#-- Not BLE_SYS_CTRL_ONLY_EN: Start -->
    <#if GAP_DEV_NAME_CHECK == true>
#define CONFIG_BLE_GAP_DEV_NAME_VALUE                    {"${GAP_DEV_NAME}"}
    <#else>
#define CONFIG_BLE_GAP_DEV_NAME_VALUE                    !!! Length of GAP Device Name is invalid !!!
    </#if>
    <#if GAP_CENTRAL == true || GAP_PERIPHERAL == true>
// GAP Service option
#define CONFIG_BLE_GAP_SVC_DEV_NAME_WRITE                ${GAP_SVC_DEV_NAME_WRITE?c}             /* Enable Device Name Write Property */
#define CONFIG_BLE_GAP_SVC_APPEARANCE                    0x${GAP_SVC_APPEARANCE}                 /* Appearance */
#define CONFIG_BLE_GAP_SVC_PERI_PRE_CP                   ${GAP_SVC_PERI_PRE_CP?c}                /* Enable Peripheral Preferred Connection Parameters */
        <#if GAP_SVC_PERI_PRE_CP == true>
#define CONFIG_BLE_GAP_SVC_CP_MIN_CONN_INTERVAL          ${GAP_SVC_CP_MIN_CONN_INTERVAL}    /* Minimum Connection Interval */
#define CONFIG_BLE_GAP_SVC_CP_MAX_CONN_INTERVAL          ${GAP_SVC_CP_MAX_CONN_INTERVAL}    /* Maximum Connection Interval */
#define CONFIG_BLE_GAP_SVC_CP_PERI_LATENCY               ${GAP_SVC_CP_PERI_LATENCY}         /* Peripheral Latency */
#define CONFIG_BLE_GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT   ${GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT} /* Connection Sup ervision Timeout Multiplier */
        </#if>
        <#if APP_BLE_DEVICE == "pic32cx_bz6_family">
#define CONFIG_BLE_GAP_SVC_ENC_DATA_KEY_MATL             ${GAP_SVC_ENC_DATA_KEY_MATL?c}                   /* Enable Encrypted Data Key Material */
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true>
#define CONFIG_BLE_GAP_SVC_EDKM_KEY_C                    {${GAP_SVC_EDKM_KEY_C}} /* Shared session key*/
#define CONFIG_BLE_GAP_SVC_EDKM_IV_C                     {${GAP_SVC_EDKM_IV_C}} /* Initialization vector*/
            </#if>
#define CONFIG_BLE_GAP_SVC_LE_GATT_SEC_LVLS              ${GAP_SVC_LE_GATT_SEC_LVLS?c}                     /* Enable LE GATT Security Levels */
        </#if>
    </#if>
    <#if BOOL_GAP_PRIVACY == true>
#define CONFIG_BLE_GAP_PRIV_ADDR_TYPE                    ${GAP_PRIV_ADDR_TYPE} /* Address Type */
#define CONFIG_BLE_GAP_PRIV_ADDR_TO                      ${GAP_PRIV_ADDR_TO} /* Address Timeout */
    </#if>

    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == false>
        <#if GAP_ADV_DATA_LEN != 0>
//Legacy Advertising set
#define CONFIG_BLE_GAP_ADV_DATA                      {${GAP_ADV_DATA}}
#define CONFIG_BLE_GAP_ADV_DATA_ORIG_LEN             ${GAP_ADV_DATA_ORIG_LEN}
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_ADV_DATA_ED_EN == true>
#define CONFIG_BLE_GAP_ADV_DATA_ED_C                 {${GAP_ADV_DATA_ED_C}}
            </#if>
        </#if>
        <#if GAP_SCAN_RSP_DATA_LEN != 0>
#define CONFIG_BLE_GAP_SCAN_RSP_DATA                 {${GAP_SCAN_RSP_DATA}}
#define CONFIG_BLE_GAP_SCAN_RSP_DATA_ORIG_LEN        ${GAP_SCAN_RSP_DATA_ORIG_LEN}
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_SCAN_RSP_DATA_ED_EN == true>
#define CONFIG_BLE_GAP_SCAN_RSP_DATA_ED_C            {${GAP_SCAN_RSP_DATA_ED_C}}
            </#if>
        </#if>
#define CONFIG_BLE_GAP_ADV_TX_PWR                    ${GAP_ADV_TX_PWR} /* Advertising TX Power */
#define CONFIG_BLE_GAP_ADV_INTERVAL_MIN              ${GAP_ADV_INTERVAL_MIN} /* Advertising Interval Min */
#define CONFIG_BLE_GAP_ADV_INTERVAL_MAX              ${GAP_ADV_INTERVAL_MAX} /* Advertising Interval Max */
#define CONFIG_BLE_GAP_ADV_TYPE                      ${GAP_ADV_TYPE} /* Advertising Type */
#define CONFIG_BLE_GAP_ADV_CHANNEL_MAP               ${GAP_CHANNEL_MAP} /* Advertising Channel Map */
#define CONFIG_BLE_GAP_ADV_FILT_POLICY               ${GAP_ADV_FILT_POLICY} /* Advertising Filter Policy */
    </#if>
    <#if GAP_ADVERTISING == true && BOOL_GAP_EXT_ADV == true>
    <#-- Extended Advertising Set 1: Start -->
//Extended Advertising Set 1
        <#if GAP_EXT_ADV_DATA_LEN != 0>
#define CONFIG_BLE_GAP_EXT_ADV_DATA                  {${GAP_EXT_ADV_DATA}}
#define CONFIG_BLE_GAP_EXT_ADV_DATA_ORIG_LEN         ${GAP_EXT_ADV_DATA_ORIG_LEN}
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_EXT_ADV_DATA_ED_EN == true>
#define CONFIG_BLE_GAP_EXT_ADV_DATA_ED_C             {${GAP_EXT_ADV_DATA_ED_C}}
            </#if>
        </#if>
        <#if GAP_EXT_SCAN_RSP_DATA_LEN != 0>
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA             {${GAP_EXT_SCAN_RSP_DATA}}
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA_ORIG_LEN    ${GAP_EXT_SCAN_RSP_DATA_ORIG_LEN}
            <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_EXT_SCAN_RSP_DATA_ED_EN == true>
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA_ED_C        {${GAP_EXT_SCAN_RSP_DATA_ED_C}}
            </#if>
        </#if>
#define CONFIG_BLE_GAP_EXT_ADV_ADV_SET_HANDLE        ${GAP_EXT_ADV_ADV_SET_HANDLE}        /* Advertising Handle */
        <#if GAP_EXT_ADV_EVT_PROPERTIES == '0'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)   /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '1'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)    /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '2'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)   /* Advertising Event Properties */
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '3'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '4'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
        <#elseif GAP_EXT_ADV_EVT_PROPERTIES == '5'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES        (0 <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
        </#if>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_INTERVAL_MIN      ${GAP_EXT_ADV_PRI_INTERVAL_MIN}   /* Primary Advertising Interval Min */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_INTERVAL_MAX      ${GAP_EXT_ADV_PRI_INTERVAL_MAX}   /* Primary Advertising Interval Max */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_CHANNEL_MAP       ${GAP_PRI_CHANNEL_MAP}       /* Primary Advertising Channel Map */
#define CONFIG_BLE_GAP_EXT_ADV_FILT_POLICY           ${GAP_EXT_ADV_FILT_POLICY}    /* Advertising Filter Policy */
#define CONFIG_BLE_GAP_EXT_ADV_TX_POWER              ${GAP_EXT_ADV_TX_POWER}    /* Advertising TX Power */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY           ${GAP_PRI_ADV_PHY}      /* Primary Advertising PHY */
#define CONFIG_BLE_GAP_EXT_ADV_MAX_SKIP              ${GAP_EXT_ADV_MAX_SKIP}     /* Secondary Advertising Max Skip */
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY           ${GAP_SEC_ADV_PHY}      /* Secondary Advertising PHY */
#define CONFIG_BLE_GAP_EXT_ADV_SID                   ${GAP_EXT_ADV_SID}     /* Advertising SID */
#define CONFIG_BLE_GAP_EXT_ADV_SCAN_ENABLE           ${GAP_EXT_ADV_SCAN_ENABLE?c}   /* Scan Request Notification Enable */
        <#if APP_BLE_DEVICE == "pic32cx_bz6_family">
            <#if GAP_PRI_ADV_PHY == 'BLE_GAP_PHY_TYPE_LE_CODED'>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY_OPT       ${GAP_PRI_ADV_PHY_OPTION}  /* Primary Advertising PHY Option */
            <#else>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY_OPT       0  /* Primary Advertising PHY Option */
            </#if>
            <#if GAP_SEC_ADV_PHY == 'BLE_GAP_PHY_TYPE_LE_CODED'>
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY_OPT       ${GAP_SEC_ADV_PHY_OPTION}  /* Secondary Advertising PHY Option */
            <#else>
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY_OPT       0  /* Secondary Advertising PHY Option */
            </#if>
        </#if>
    <#-- Extended Advertising Set 1: End -->
    <#-- Extended Advertising Set 2: Start -->
        <#if GAP_EXT_ADV_ADV_SET_2 == true>
//Extended Advertising Set 2
            <#if GAP_EXT_ADV_DATA_LEN_2 != 0>
#define CONFIG_BLE_GAP_EXT_ADV_DATA_2                {${GAP_EXT_ADV_DATA_2}}
#define CONFIG_BLE_GAP_EXT_ADV_DATA_ORIG_LEN_2       ${GAP_EXT_ADV_DATA_ORIG_LEN_2}
                <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_EXT_ADV_DATA_ED_EN_2 == true>
#define CONFIG_BLE_GAP_EXT_ADV_DATA_ED_C_2           {${GAP_EXT_ADV_DATA_ED_C_2}}
                </#if>
            </#if>
            <#if GAP_EXT_SCAN_RSP_DATA_LEN_2 != 0>
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA_2           {${GAP_EXT_SCAN_RSP_DATA_2}}
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA_ORIG_LEN_2  ${GAP_EXT_SCAN_RSP_DATA_ORIG_LEN_2}
                <#if GAP_SVC_ENC_DATA_KEY_MATL == true && GAP_EXT_SCAN_RSP_DATA_ED_EN_2 == true>
#define CONFIG_BLE_GAP_EXT_SCAN_RSP_DATA_ED_C_2      {${GAP_EXT_SCAN_RSP_DATA_ED_C_2}}
                </#if>
            </#if>
#define CONFIG_BLE_GAP_EXT_ADV_ADV_SET_HANDLE_2      ${GAP_EXT_ADV_ADV_SET_HANDLE_2}        /* Advertising Handle */
            <#if GAP_EXT_ADV_EVT_PROPERTIES_2 == '0'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)   /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '1'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)    /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '2'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV)   /* Advertising Event Properties */
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '3'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '4'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
            <#elseif GAP_EXT_ADV_EVT_PROPERTIES_2 == '5'>
#define CONFIG_BLE_GAP_EXT_ADV_EVT_PROPERTIES_2      (0 <#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_ANON_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_OMIT_ADDRESS </#if><#t>
                                                     <#if GAP_EXT_ADV_EVT_PROP_TX_POWER_2 == true>|BLE_GAP_EXT_ADV_EVT_PROP_TX_POWER </#if>)  /* Advertising Event Properties */<#lt>
            </#if>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_INTERVAL_MIN_2    ${GAP_EXT_ADV_PRI_INTERVAL_MIN_2}   /* Primary Advertising Interval Min */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_INTERVAL_MAX_2    ${GAP_EXT_ADV_PRI_INTERVAL_MAX_2}   /* Primary Advertising Interval Max */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_CHANNEL_MAP_2     ${GAP_PRI_CHANNEL_MAP_2}       /* Primary Advertising Channel Map */
#define CONFIG_BLE_GAP_EXT_ADV_FILT_POLICY_2         ${GAP_EXT_ADV_FILT_POLICY_2}    /* Advertising Filter Policy */
#define CONFIG_BLE_GAP_EXT_ADV_TX_POWER_2            ${GAP_EXT_ADV_TX_POWER_2}    /* Advertising TX Power */
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY_2         ${GAP_PRI_ADV_PHY_2}      /* Primary Advertising PHY */
#define CONFIG_BLE_GAP_EXT_ADV_MAX_SKIP_2            ${GAP_EXT_ADV_MAX_SKIP_2}     /* Secondary Advertising Max Skip */
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY_2         ${GAP_SEC_ADV_PHY_2}      /* Secondary Advertising PHY */
#define CONFIG_BLE_GAP_EXT_ADV_SID_2                 ${GAP_EXT_ADV_SID_2}     /* Advertising SID */
#define CONFIG_BLE_GAP_EXT_ADV_SCAN_ENABLE_2         ${GAP_EXT_ADV_SCAN_ENABLE_2?c}   /* Scan Request Notification Enable */
            <#if APP_BLE_DEVICE == "pic32cx_bz6_family">
                <#if GAP_PRI_ADV_PHY_2 == 'BLE_GAP_PHY_TYPE_LE_CODED'>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY_OPT_2     ${GAP_PRI_ADV_PHY_OPTION_2}  /* Primary Advertising PHY Option */
                <#else>
#define CONFIG_BLE_GAP_EXT_ADV_PRI_ADV_PHY_OPT_2     0  /* Primary Advertising PHY Option */
                </#if>
                <#if GAP_SEC_ADV_PHY_2 == 'BLE_GAP_PHY_TYPE_LE_CODED'>
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY_OPT_2     ${GAP_SEC_ADV_PHY_OPTION_2}  /* Secondary Advertising PHY Option */
                <#else>
#define CONFIG_BLE_GAP_EXT_ADV_SEC_ADV_PHY_OPT_2     0  /* Secondary Advertising PHY Option */
                </#if>
            </#if>
        </#if>
    <#-- Extended Advertising Set 2: End -->
    </#if>

    <#if GAP_SCAN == true>
        <#if BOOL_GAP_EXT_SCAN == false>
// Configure scan parameters
#define CONFIG_BLE_GAP_SCAN_TYPE                 ${GAP_SCAN_TYPE}      /* Scan Type */
#define CONFIG_BLE_GAP_SCAN_INTERVAL             ${GAP_SCAN_INTERVAL}      /* Scan Interval */
#define CONFIG_BLE_GAP_SCAN_WINDOW               ${GAP_SCAN_WINDOW}      /* Scan Window */
#define CONFIG_BLE_GAP_SCAN_FILT_POLICY          ${GAP_SCAN_FILT_POLICY}       /* Scan Filter Policy */
#define CONFIG_BLE_GAP_SCAN_DIS_CHANNEL_MAP      ${GAP_SCAN_DIS_CHANNEL_MAP}      /* Disable specific channel during scanning */
        <#else>
// Configure extended scan parameters
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
#define CONFIG_BLE_GAP_EXT_SCAN_PHY_1M                    true       /* Scan PHY */
#define CONFIG_BLE_GAP_EXT_SCAN_TYPE_1M                   ${GAP_EXT_SCAN_TYPE_1M}      /* Scan Type */
#define CONFIG_BLE_GAP_EXT_SCAN_INTERVAL_1M               ${GAP_EXT_SCAN_INTERVAL_1M}      /* Scan Interval */
#define CONFIG_BLE_GAP_EXT_SCAN_WINDOW_1M                 ${GAP_EXT_SCAN_WINDOW_1M}      /* Scan Window */
#define CONFIG_BLE_GAP_EXT_SCAN_DIS_CHANNEL_MAP_1M        ${GAP_EXT_SCAN_DIS_CHANNEL_MAP_1M}      /* Disable specific channel during scanning */
            <#else>
#define CONFIG_BLE_GAP_EXT_SCAN_PHY_1M                    false      /* Scan PHY */
            </#if>
            <#if (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_CODED') || (GAP_EXT_SCAN_PHY == 'BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED')>
#define CONFIG_BLE_GAP_EXT_SCAN_PHY_CODED                 true        /* Scan PHY */
#define CONFIG_BLE_GAP_EXT_SCAN_TYPE_CODED                ${GAP_EXT_SCAN_TYPE_CODED}        /* Scan Type */
#define CONFIG_BLE_GAP_EXT_SCAN_INTERVAL_CODED            ${GAP_EXT_SCAN_INTERVAL_CODED}        /* Scan Interval */
#define CONFIG_BLE_GAP_EXT_SCAN_WINDOW_CODED              ${GAP_EXT_SCAN_WINDOW_CODED}        /* Scan Interval */
#define CONFIG_BLE_GAP_EXT_SCAN_DIS_CHANNEL_MAP_CODED     ${GAP_EXT_SCAN_DIS_CHANNEL_MAP_CODED}      /* Disable specific channel during scanning */
            <#else>
#define CONFIG_BLE_GAP_EXT_SCAN_PHY_CODED                 false       //Scan PHY
            </#if>
#define CONFIG_BLE_GAP_EXT_SCAN_FILT_POLICY               ${GAP_EXT_SCAN_FILT_POLICY}
#define CONFIG_BLE_GAP_EXT_SCAN_NUM_2_PKT                 ${GAP_EXT_SCAN_NUM_2_PKT} /* Maximum Number of Secondary Packets */
#define CONFIG_BLE_GAP_EXT_SCAN_DATA_LEN                  ${GAP_EXT_SCAN_DATA_LEN} /* Maximum Data Len */
        </#if>
    </#if>

    <#if GAP_CENTRAL == true || GAP_PERIPHERAL == true>
#define CONFIG_BLE_GAP_CONN_TX_PWR               ${GAP_CONN_TX_PWR} /* Connection TX Power */
    </#if>

    <#if GAP_CENTRAL == true || GAP_PERIPHERAL == true>
// Configure SMP parameters
#define CONFIG_BLE_SMP_IOCAP_TYPE   ${SMP_IOCAP_TYPE}  /* IO Capability */
#define CONFIG_BLE_SMP_OPTION       (0 <#t>
                                    <#if SMP_AUTH_REQ_BOND == true>|BLE_SMP_OPTION_BONDING </#if><#t>
                                    <#if SMP_AUTH_REQ_MITM == true>|BLE_SMP_OPTION_MITM </#if><#t>
                                    <#if SMP_AUTH_REQ_SC == true>|BLE_SMP_OPTION_SECURE_CONNECTION</#if>) /* Authentication Setting */<#lt>
        <#if SMP_OOB == true>
#define CONFIG_BLE_SMP_OOB          true           /* OOB Authentication Data Present */
        </#if>
        <#if SMP_SC_ONLY == true && SMP_AUTH_REQ_SC == true>
#define CONFIG_BLE_SMP_SC_ONLY      true          /* Secure Connections Only */
        </#if>
        <#if SMP_AUTH_PAIR == true>
#define CONFIG_BLE_SMP_AUTH_PAIR    true          /* Authenticated pairing method is required */
        </#if>

// Configure BLE_DM middleware parameters
#define CONFIG_BLE_DM_SEC_AUTO_ACCEPT      ${BOOL_DM_SEC_AUTO_ACCEPT?c} /* Auto Accept Security Request */
        <#if BOOL_DM_CONN_AUTO_ACCEPT == false>
#define CONFIG_BLE_DM_AUTO_REPLY_UPD_REQ   false     /* Auto Accept Connection Parameter Update Request */
        <#else>
#define CONFIG_BLE_DM_AUTO_REPLY_UPD_REQ   true      /* Auto Accept Connection Parameter Update Request */
#define CONFIG_BLE_DM_MIN_CONN_INTERVAL    ${DM_MIN_CONN_INTERVAL}    /* Minimum Connection Interval */
#define CONFIG_BLE_DM_MAX_CONN_INTERVAL    ${DM_MAX_CONN_INTERVAL}   /* Maximum Connection Interval */
#define CONFIG_BLE_DM_MIN_CONN_LATENCY     ${DM_MIN_CONN_LATENCY}    /* Minimum Connection Latency */
#define CONFIG_BLE_DM_MAX_CONN_LATENCY     ${DM_MAX_CONN_LATENCY}    /* Maximum Connection Latency */
        </#if>

        <#if BLE_BOOL_GATT_SVC_CHANGE == true>
// GATT Service option
#define CONFIG_BLE_GATT_SVC_CHANGE         true       /* Enable GATT Service Change */
        </#if>

        <#if BLE_BOOL_GATT_CLIENT == true>
// Configure BLE_DD middleware parameters
#define CONFIG_BLE_GCM_DD_WAIT_FOR_SEC          ${BOOL_GCM_DD_WAIT_FOR_SEC?c} /* Wait for security */
#define CONFIG_BLE_GCM_DD_INIT_DISC_IN_CNTRL    ${BOOL_GCM_DD_INIT_DISC_IN_CNTRL?c} /* Init discovery with central role */
#define CONFIG_BLE_GCM_DD_INIT_DISC_IN_PER      ${BOOL_GCM_DD_INIT_DISC_IN_PER?c} /* Init discovery with peripheral role */
#define CONFIG_BLE_GCM_DD_DIS_CONN_DISC         false
        </#if>
    </#if>
<#-- Not BLE_SYS_CTRL_ONLY_EN: End -->
</#if>