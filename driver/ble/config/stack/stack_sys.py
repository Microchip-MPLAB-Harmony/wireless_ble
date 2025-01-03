def sleepEnabledChange(symbol, event):
    value = event["value"]
    if value == True:
        Log.writeInfoMessage('BLEStack:sleepEnabledChange value={}'.format(str(value)))
        sendSleepEnableMessage(value)
    else:
        Log.writeInfoMessage('BLEStack:sleepEnabledChange value={}'.format(str(value)))
        sendSleepEnableMessage(value)

def ctrlOnlyChange(symbol, event):
    value = event["value"]
    bleStackSymbolIDs = ["GAP_MENU", "GATT_MENU", "DM_MENU", "L2CAP_MENU", "UTILITY_MENU", "SMP_MENU"]
    bleServiceProfileComponentIDs = []

    # Main menu visible settings
    for n in bleStackSymbolIDs:
        localSymbol = symbol.getComponent().getSymbolByID(n)
        if value == True:
            localSymbol.setVisible(False)
        else:
            localSymbol.setVisible(True)

    #GCM menu visible setting. GCM is dependent on GATTC
    localGattcSymbol = symbol.getComponent().getSymbolByID("BLE_BOOL_GATT_CLIENT")
    localGcmSymbol = symbol.getComponent().getSymbolByID("GCM_MENU")

    if localGattcSymbol.getValue() == True:
        if value == True:
            localGcmSymbol.setVisible(False)
        else:
            localGcmSymbol.setVisible(True)

    # Deactivate Profile/Service components if BLE Controller Only is enabled
    if value == True:
        idList = Database.getActiveComponentIDs()
        for idName in idList:
            if (idName.find('SERVICE_') != -1) or (idName.find('PROFILE_') != -1):
                Database.sendMessage(idName, "CONTROLLER_ONLY_MODE_ENABLED", {"target": idName,
                                                    "source": "BLE_STACK_LIB"})

def configOutputChange(symbol, event):

    if bleCtrlOnly.getValue() == False:
        if (devFamily == "pic32cx_bz6_family"):
            symbol.setValue(True)
        else: # Bz2/ Bz3
            if bleDisAppCodeGen.getValue():
                symbol.setValue(True)
            else:
                symbol.setValue(False)
    else:
        symbol.setValue(False)
#################################################################
##################      BLE System Settings       ###############
#################################################################

menuBleSys = libBLEStackComponent.createMenuSymbol('BLE_SYS_MENU', None)
menuBleSys.setLabel('System')
menuBleSys.setVisible(True)
menuBleSys.setDescription("BLE System Settings")

# Sleep Mode setting
global sysSleepEn
sysSleepEn = libBLEStackComponent.createBooleanSymbol('BLE_SYS_SLEEP_MODE_EN', menuBleSys)
sysSleepEn.setLabel('Enable Sleep Mode')
sysSleepEn.setDescription('Enable BLE Sleep Mode')
sysSleepEn.setDefaultValue(False)
sysSleepEn.setVisible(True)
sysSleepEn.setDependencies(sleepEnabledChange, ["BLE_SYS_SLEEP_MODE_EN"])

# BLE PTA setting
global blePta
blePta = libBLEStackComponent.createBooleanSymbol('BLE_SYS_PTA_EN', menuBleSys)
blePta.setLabel('Enable Three-Wire PTA interface')
blePta.setDescription('Enable Three-Wire PTA interface')
blePta.setDefaultValue(False)
blePta.setVisible(False)

# BLE Controller Only
global bleCtrlOnly
bleCtrlOnly = libBLEStackComponent.createBooleanSymbol('BLE_SYS_CTRL_ONLY_EN', menuBleSys)
bleCtrlOnly.setLabel('BLE Controller Only')
bleCtrlOnly.setDescription('BLE Controller Only')
bleCtrlOnly.setDefaultValue(False)
bleCtrlOnly.setVisible(True)
bleCtrlOnly.setDependencies(ctrlOnlyChange, ["BLE_SYS_CTRL_ONLY_EN"])

# BLE Disable App Code Generation setting
global bleDisAppCodeGen
bleDisAppCodeGen = libBLEStackComponent.createBooleanSymbol('DISABLE_APP_CODE_GEN', menuBleSys)
bleDisAppCodeGen.setLabel('Disable App Code Gen')
bleDisAppCodeGen.setDescription('Disable App Code Gen')
bleDisAppCodeGen.setDefaultValue(False)
bleDisAppCodeGen.setVisible(False)

# BLE Output user setting to configuration.h
global bleEnOutputConfig
bleEnOutputConfig = libBLEStackComponent.createBooleanSymbol('BLE_ENABLE_OUTPUT_CONFIG', menuBleSys)
bleEnOutputConfig.setLabel('Enable User Setting in Configuration.h')
bleEnOutputConfig.setDescription('Enable User Setting in Configuration.h')
bleEnOutputConfig.setDefaultValue(False)
bleEnOutputConfig.setVisible(False)
bleEnOutputConfig.setDependencies(configOutputChange, ["BLE_SYS_CTRL_ONLY_EN", "DISABLE_APP_CODE_GEN"])
bleEnOutputConfig.setValue((devFamily == "pic32cx_bz6_family"))

