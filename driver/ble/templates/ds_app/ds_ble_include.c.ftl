<#if BLE_SYS_CTRL_ONLY_EN == false>
#include "app_ble.h"
<#else>
#include "app_ble_hci.h"
</#if>
<#if GAP_DSADV_EN>
#include "app_ble_dsadv.h"
</#if>