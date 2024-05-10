execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
else:
    print("Device not support")

def bleAncsEnable(symbol, event):
    symbol.setEnabled(event["value"])

def ancsClientConfig(symbol, event):
    if event["value"] == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANCS_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANCS_CLIENT", False)

def instantiateComponent(profileAncsComponent):
    print('Load Module: PROFILE_ANCS')
    configName = Variables.get('__CONFIGURATION_NAME')

    
    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    global ancsMenuClient
    ancsMenuClient = profileAncsComponent.createBooleanSymbol('ANCS_BOOL_CLIENT', None)
    ancsMenuClient.setLabel('Enable Client Role')
    ancsMenuClient.setDefaultValue(True)
    ancsMenuClient.setVisible(True)
    ancsMenuClient.setDependencies(ancsClientConfig, ["ANCS_BOOL_CLIENT"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################
    # Add ble_ancs.c file - static file
    bleAncsSourceFile = profileAncsComponent.createFileSymbol(None, None)
    bleAncsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_ancs/ble_ancs.c')
    bleAncsSourceFile.setOutputName('ble_ancs.c')
    bleAncsSourceFile.setOverwrite(True)
    bleAncsSourceFile.setDestPath('ble/profile_ble/ble_ancs/')
    bleAncsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_ancs/')
    bleAncsSourceFile.setType('SOURCE')
    bleAncsSourceFile.setEnabled(True)
    bleAncsSourceFile.setMarkup(True)
    bleAncsSourceFile.setDependencies(bleAncsEnable, ["ANCS_BOOL_CLIENT"])

    # Add ble_ancs.h file - static file
    bleAncsHeaderFile = profileAncsComponent.createFileSymbol(None, None)
    bleAncsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_ancs/ble_ancs.h')
    bleAncsHeaderFile.setOutputName('ble_ancs.h')
    bleAncsHeaderFile.setOverwrite(True)
    bleAncsHeaderFile.setDestPath('ble/profile_ble/ble_ancs/')
    bleAncsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_ancs/')
    bleAncsHeaderFile.setType('HEADER')
    bleAncsHeaderFile.setEnabled(True)
    bleAncsHeaderFile.setMarkup(True)
    bleAncsHeaderFile.setDependencies(bleAncsEnable, ["ANCS_BOOL_CLIENT"])

    # Add app_ancs_handler.c file - static file
    bleAncsAppSourceFile = profileAncsComponent.createFileSymbol(None, None)
    bleAncsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_ancs_handler.c')
    bleAncsAppSourceFile.setOutputName('app_ancs_handler.c')
    bleAncsAppSourceFile.setOverwrite(True)
    bleAncsAppSourceFile.setDestPath('../../app_ble')
    bleAncsAppSourceFile.setProjectPath('app_ble')
    bleAncsAppSourceFile.setType('Source')
    bleAncsAppSourceFile.setEnabled(True)
    bleAncsAppSourceFile.setDependencies(bleAncsEnable, ["ANCS_BOOL_CLIENT"])

    # Add app_ancs_handler.h file - static file
    bleAncsAppHeaderFile = profileAncsComponent.createFileSymbol(None, None)
    bleAncsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_ancs_handler.h')
    bleAncsAppHeaderFile.setOutputName('app_ancs_handler.h')
    bleAncsAppHeaderFile.setOverwrite(True)
    bleAncsAppHeaderFile.setDestPath('../../app_ble')
    bleAncsAppHeaderFile.setProjectPath('app_ble')
    bleAncsAppHeaderFile.setType('HEADER')
    bleAncsAppHeaderFile.setEnabled(True)
    bleAncsAppHeaderFile.setDependencies(bleAncsEnable, ["ANCS_BOOL_CLIENT"])


def finalizeComponent(profileAncsComponent):
    print('Finalizing: {}'.format(profileAncsComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

    if ancsMenuClient.getValue() == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANCS_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    print('PROFILE_ANCS:onAttachmentConnected: configuring')

    if connectID == 'BLE_STACK_Dependency':
        global ancsMenuClient

        if ancsMenuClient.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_ANCS_CLIENT", True)
            if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
                Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)