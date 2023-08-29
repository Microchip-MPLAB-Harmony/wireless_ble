def dfuExtFlashEnable(symbol, event):
    if event["id"] == "BOOL_BLE_UTIL_DFU":
        symbol.setVisible(event["value"])
        if Database.getSymbolValue("core", "DMA_ENABLE") != None:
            Database.setSymbolValue("core", "DMAC_ENABLE_CH_15", event["value"])
            Database.setSymbolValue("core", "DMA_ENABLE", event["value"])

    if event["value"] == True and symbol.getValue() == True:
        Database.activateComponents(["qspi", "drv_sst26"])
        Database.connectDependencies([["drv_sst26", "drv_sst26_SQI_dependency", "qspi", "QSPI_SQI"]])
        Database.setSymbolValue("pdsSystem", "PDS_USES_BOOT_FLASH", True)
    elif event["value"] == False:
        Database.deactivateComponents(["qspi","drv_sst26"])
        Database.setSymbolValue("pdsSystem", "PDS_USES_BOOT_FLASH", False)

############################################################################
### Utility Settings ###
############################################################################

# Utility Menu
menuBleUtil = libBLEStackComponent.createMenuSymbol('UTILITY_MENU', None)
menuBleUtil.setLabel('Utilities')
menuBleUtil.setVisible(True)
menuBleUtil.setDescription("UTILITY SETTINGS")


# BLE Virtual Sniffer Setting
bleVirtualSniffer = libBLEStackComponent.createBooleanSymbol('BLE_VIRTUAL_SNIFFER_EN', menuBleUtil)
bleVirtualSniffer.setLabel('Enable BLE Virtual Sniffer')
bleVirtualSniffer.setDescription('Enable BLE Virtual Sniffer')
bleVirtualSniffer.setDefaultValue(False)
bleVirtualSniffer.setVisible(True)


# DFU Module
bleUtilDfu = libBLEStackComponent.createBooleanSymbol('BOOL_BLE_UTIL_DFU', menuBleUtil)
bleUtilDfu.setLabel('DFU module')
bleUtilDfu.setDefaultValue(False)

# External Flash Setting
if( processor in pic32cx_bz3_family):
    bleUtilDfuExt = libBLEStackComponent.createBooleanSymbol('BOOL_BLE_UTIL_DFU_EXT_FLASH', bleUtilDfu)
    bleUtilDfuExt.setLabel('Enable external flash')
    bleUtilDfuExt.setDefaultValue(False)
    bleUtilDfuExt.setVisible(False)
    bleUtilDfuExt.setDependencies(dfuExtFlashEnable, ["BOOL_BLE_UTIL_DFU", "BOOL_BLE_UTIL_DFU_EXT_FLASH"])