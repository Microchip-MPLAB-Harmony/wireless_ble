def ptaEnabledChange(symbol, event):
    value = event["value"]
    Log.writeInfoMessage('BLEStack:ptaEnabledChange value={}'.format(str(value)))
    pinNoBtPriority = ''
    pinNoBtActive = ''
    pinNoWlanActive = ''

    if devId == '451' or devId == '451H':
        pinNoBtPriority = 'BSP_PIN_33'
        pinNoBtActive = 'BSP_PIN_34'
        pinNoWlanActive = 'BSP_PIN_4'
    else: # devId: '350', '351'
        pinNoBtPriority = 'BSP_PIN_30'
        pinNoBtActive = 'BSP_PIN_36'
        pinNoWlanActive = 'BSP_PIN_29'

    if value == True:
        Database.setSymbolValue("core", pinNoBtPriority + "_FUNCTION_TYPE", "GPIO")
        Database.setSymbolValue("core", pinNoBtPriority + "_FUNCTION_NAME", "BT_PRIORITY")
        Database.setSymbolValue("core", pinNoBtPriority + "_DIR", "Out")
        Database.clearSymbolValue("core", pinNoBtPriority + "_LAT")
        Database.clearSymbolValue("core", pinNoBtPriority + "_OD")
        Database.clearSymbolValue("core", pinNoBtPriority + "_CN")
        Database.clearSymbolValue("core", pinNoBtPriority + "_PU")
        Database.setSymbolValue("core", pinNoBtPriority + "_PD", "True")
        Database.clearSymbolValue("core", pinNoBtPriority + "_SR")
        
        Database.setSymbolValue("core", pinNoBtActive + "_FUNCTION_TYPE", "GPIO")
        Database.setSymbolValue("core", pinNoBtActive + "_FUNCTION_NAME", "BT_ACTIVE")
        Database.setSymbolValue("core", pinNoBtActive + "_DIR", "Out")
        Database.clearSymbolValue("core", pinNoBtActive + "_LAT")
        Database.clearSymbolValue("core", pinNoBtActive + "_OD")
        Database.clearSymbolValue("core", pinNoBtActive + "_CN")
        Database.clearSymbolValue("core", pinNoBtActive + "_PU")
        Database.setSymbolValue("core", pinNoBtActive + "_PD", "True")
        Database.clearSymbolValue("core", pinNoBtActive + "_SR")
        
        Database.setSymbolValue("core", pinNoWlanActive + "_FUNCTION_TYPE", "GPIO")
        Database.setSymbolValue("core", pinNoWlanActive + "_FUNCTION_NAME", "WLAN_ACTIVE")
        Database.clearSymbolValue("core", pinNoWlanActive + "_DIR")
        Database.clearSymbolValue("core", pinNoWlanActive + "_OD")
        Database.clearSymbolValue("core", pinNoWlanActive + "_MODE")
        Database.setSymbolValue("core", pinNoWlanActive + "_CN", "True")
        Database.clearSymbolValue("core", pinNoWlanActive + "_PU")
        Database.setSymbolValue("core", pinNoWlanActive + "_PD", "True")
        Database.clearSymbolValue("core", pinNoWlanActive + "_SR")
    else:
        Database.clearSymbolValue("core", pinNoBtPriority + "_FUNCTION_TYPE")
        Database.clearSymbolValue("core", pinNoBtPriority + "_FUNCTION_NAME")
        Database.clearSymbolValue("core", pinNoBtPriority + "_DIR")
        Database.clearSymbolValue("core", pinNoBtPriority + "_PD")
        Database.clearSymbolValue("core", pinNoBtActive + "_FUNCTION_TYPE")
        Database.clearSymbolValue("core", pinNoBtActive + "_FUNCTION_NAME")
        Database.clearSymbolValue("core", pinNoBtActive + "_DIR")
        Database.clearSymbolValue("core", pinNoBtActive + "_PD")
        Database.clearSymbolValue("core", pinNoWlanActive + "_FUNCTION_TYPE")
        Database.clearSymbolValue("core", pinNoWlanActive + "_PD")
        Database.clearSymbolValue("core", pinNoWlanActive + "_FUNCTION_NAME")
        Database.clearSymbolValue("core", pinNoWlanActive + "_CN")

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
blePta = libBLEStackComponent.createBooleanSymbol('BLE_SYS_PTA_EN', menuBleSys)
blePta.setLabel('Enable Three-Wire PTA interface')
blePta.setDescription('Enable Three-Wire PTA interface')
blePta.setDefaultValue(False)
blePta.setVisible(devId != '450' and processor != 'PIC32CX1012BZ25032')
blePta.setDependencies(ptaEnabledChange, ["BLE_SYS_PTA_EN"])

# BLE Controller Only
global bleCtrlOnly
bleCtrlOnly = libBLEStackComponent.createBooleanSymbol('BLE_SYS_CTRL_ONLY_EN', menuBleSys)
bleCtrlOnly.setLabel('BLE Controller Only')
bleCtrlOnly.setDescription('BLE Controller Only')
bleCtrlOnly.setDefaultValue(False)
bleCtrlOnly.setVisible(True)
bleCtrlOnly.setDependencies(ctrlOnlyChange, ["BLE_SYS_CTRL_ONLY_EN"])

