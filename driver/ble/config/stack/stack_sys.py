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
        if (devDefaultEnOutputConfig == True):
            symbol.setValue(True)
        else: # Bz2/ Bz3
            if bleDisAppCodeGen.getValue():
                symbol.setValue(True)
            else:
                symbol.setValue(False)
    else:
        symbol.setValue(False)

def ctrlOnlyConfigVisibilityFirstLayer(symbol, event):
    if (symbol.getID() == event["id"]):
        ctrlOnlySymbol = symbol.getComponent().getSymbolByID("BLE_SYS_CTRL_ONLY_EN")
        symbol.setVisible(ctrlOnlySymbol.getValue())
    else:
        symbol.setVisible(event["value"])

def ctrlOnlyConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

#################################################################
##################      BLE System Settings       ###############
#################################################################

menuBleSys = libBLEStackComponent.createMenuSymbol('BLE_SYS_MENU', None)
menuBleSys.setLabel('System')
menuBleSys.setVisible(True)
menuBleSys.setDescription("BLE System Settings")

# BT Version Supported
bleSysBtVerSupp = libBLEStackComponent.createKeyValueSetSymbol('BLE_SYS_BT_VER_SUPP', menuBleSys)
bleSysBtVerSupp.setLabel('Bluetooth Core Version')
bleSysBtVerSupp.addKey('BT_VER_5_2', '0', '5.2')
bleSysBtVerSupp.addKey('BT_VER_6_0', '1', '6.0')
if devDefaultBtSupp == "SUPP_BT_5_2":
    bleSysBtVerSupp.setDefaultValue(0)
    bleSysBtVerSupp.setVisible(True)
else:
    bleSysBtVerSupp.setDefaultValue(1)
    bleSysBtVerSupp.setVisible(False)
bleSysBtVerSupp.setOutputMode('Key')
bleSysBtVerSupp.setDisplayMode('Description')

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

# BLE Controller Only-Advertising
ctrlOnlyAdvertising = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_ADVERTISING', bleCtrlOnly)
ctrlOnlyAdvertising.setLabel('Advertising')
ctrlOnlyAdvertising.setDefaultValue(True)
ctrlOnlyAdvertising.setVisible(bleCtrlOnly.getValue())
ctrlOnlyAdvertising.setDescription("Advertising Configuration")
ctrlOnlyAdvertising.setDependencies(ctrlOnlyConfigVisibilityFirstLayer, ["BOOL_CTRL_ONLY_ADVERTISING", "BLE_SYS_CTRL_ONLY_EN"])

# BLE Controller Only-Advertising-Extended Advertising
ctrlOnlyExtAdvEn = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_EXT_ADV', ctrlOnlyAdvertising)
ctrlOnlyExtAdvEn.setLabel('Enable Extended Advertising')
ctrlOnlyExtAdvEn.setDefaultValue(True)
ctrlOnlyExtAdvEn.setVisible(ctrlOnlyAdvertising.getValue())
ctrlOnlyExtAdvEn.setDescription("Enable Extended Advertising")
ctrlOnlyExtAdvEn.setDependencies(ctrlOnlyConfigVisibility, ["BOOL_CTRL_ONLY_ADVERTISING"])

# BLE Controller Only-Advertising-Extended Advertising-Periodic Advertising
ctrlOnlyPeriodicAdv = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_PERIODIC_ADV', ctrlOnlyExtAdvEn)
ctrlOnlyPeriodicAdv.setLabel('Enable Periodic Advertising')
ctrlOnlyPeriodicAdv.setDefaultValue(True)
ctrlOnlyPeriodicAdv.setVisible(ctrlOnlyExtAdvEn.getValue())
ctrlOnlyPeriodicAdv.setDescription("Enable Periodic Advertising")
ctrlOnlyPeriodicAdv.setDependencies(ctrlOnlyConfigVisibility, ["BOOL_CTRL_ONLY_EXT_ADV"])

# BLE Controller Only-Scan setting
ctrlOnlyScan = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_SCAN', bleCtrlOnly)
ctrlOnlyScan.setLabel('Scan')
ctrlOnlyScan.setDefaultValue(True)
ctrlOnlyScan.setVisible(bleCtrlOnly.getValue())
ctrlOnlyScan.setDescription("OBSERVER ROLE CONFIGURATION")
ctrlOnlyScan.setDependencies(ctrlOnlyConfigVisibilityFirstLayer, ["BOOL_CTRL_ONLY_SCAN", "BLE_SYS_CTRL_ONLY_EN"])

# BLE Controller Only-Scan setting-Extended Scan
ctrlOnlyExtScanEn = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_EXT_SCAN', ctrlOnlyScan)
ctrlOnlyExtScanEn.setLabel('Enable Extended Scan')
ctrlOnlyExtScanEn.setDefaultValue(True)
ctrlOnlyExtScanEn.setVisible(ctrlOnlyScan.getValue())
ctrlOnlyExtScanEn.setDescription("Enable Extended Scan")
ctrlOnlyExtScanEn.setDependencies(ctrlOnlyConfigVisibility, ["BOOL_CTRL_ONLY_SCAN"])

# BLE Controller Only-Peripheral Role
ctrlOnlyPeripheralRole = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_PERIPHERAL', bleCtrlOnly)
ctrlOnlyPeripheralRole.setLabel('Peripheral')
ctrlOnlyPeripheralRole.setDefaultValue(True)
ctrlOnlyPeripheralRole.setVisible(bleCtrlOnly.getValue())
ctrlOnlyPeripheralRole.setDescription("PERIPHERAL ROLE CONFIGURATION")
ctrlOnlyPeripheralRole.setDependencies(ctrlOnlyConfigVisibilityFirstLayer, ["BOOL_CTRL_ONLY_PERIPHERAL", "BLE_SYS_CTRL_ONLY_EN"])

# BLE Controller Only-Central Role
ctrlOnlyCentralRole = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_CENTRAL', bleCtrlOnly)
ctrlOnlyCentralRole.setLabel('Central')
ctrlOnlyCentralRole.setDefaultValue(True)
ctrlOnlyCentralRole.setVisible(bleCtrlOnly.getValue())
ctrlOnlyCentralRole.setDescription("CENTRAL ROLE CONFIGURATION")
ctrlOnlyCentralRole.setDependencies(ctrlOnlyConfigVisibilityFirstLayer, ["BOOL_CTRL_ONLY_CENTRAL", "BLE_SYS_CTRL_ONLY_EN"])

# BLE Controller Only-Central Role-Extended Central
ctrlOnlyExtCentralRole = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_EXT_CENTRAL', ctrlOnlyCentralRole)
ctrlOnlyExtCentralRole.setLabel('Enable Extended Central')
ctrlOnlyExtCentralRole.setDefaultValue(True)
ctrlOnlyExtCentralRole.setVisible(ctrlOnlyCentralRole.getValue())
ctrlOnlyExtCentralRole.setDescription("Enable Extended Central")
ctrlOnlyExtCentralRole.setDependencies(ctrlOnlyConfigVisibility, ["BOOL_CTRL_ONLY_CENTRAL"])

# BLE Controller Only-Synchronization
ctrlOnlySync = libBLEStackComponent.createBooleanSymbol('BOOL_CTRL_ONLY_SYNC', bleCtrlOnly)
ctrlOnlySync.setLabel('Enable Synchronization')
ctrlOnlySync.setDefaultValue(True)
ctrlOnlySync.setVisible(bleCtrlOnly.getValue())
ctrlOnlySync.setDescription("Enable Synchronization")
ctrlOnlySync.setDependencies(ctrlOnlyConfigVisibilityFirstLayer, ["BOOL_CTRL_ONLY_SYNC", "BLE_SYS_CTRL_ONLY_EN"])

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
bleEnOutputConfig.setValue(devDefaultEnOutputConfig)

