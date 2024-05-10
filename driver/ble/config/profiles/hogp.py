execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"

def blehogpEnable(symbol, event):
    symbol.setEnabled(event["value"])

def hogpServerConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_HOGP: Load SERVICE_HIDS')
        Database.setSymbolValue("BLE_STACK_LIB", "APP_HOGP_SERVER", True)
        res = Database.activateComponents(["SERVICE_HIDS"])
        res = Database.connectDependencies([['PROFILE_HOGP', 'BLE_HIDS_Dependency', 'SERVICE_HIDS', 'BLE_HIDS_Capability']])
        if 'SERVICE_BAS' in Database.getActiveComponentIDs():
            print('PROFILE_HOGP: Connect SERVICE_HIDS')
            localComponent = symbol.getComponent()
            localComponent.setDependencyEnabled("BLE_BAS_Dependency", True)
            Database.connectDependencies([['PROFILE_HOGP', 'BLE_BAS_Dependency', 'SERVICE_BAS', 'BLE_BAS_Capability']])
        else:
            print('PROFILE_HOGP: Load SERVICE_HIDS')
            res = Database.activateComponents(["SERVICE_BAS"])
            res = Database.connectDependencies([['PROFILE_HOGP', 'BLE_BAS_Dependency', 'SERVICE_BAS', 'BLE_BAS_Capability']])
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_HOGP_SERVER", False)
        if 'SERVICE_HIDS' in Database.getActiveComponentIDs():
            print('PROFILE_HOGP: Remove SERVICE_HIDS')
            res = Database.deactivateComponents(["SERVICE_HIDS"])
        if 'SERVICE_BAS' in Database.getActiveComponentIDs():
            localComponent = symbol.getComponent()
            localComponent.setDependencyEnabled("BLE_BAS_Dependency", False)

def instantiateComponent(profileHogpComponent):
    print('Load Module: PROFILE_HOGP')

    configName = Variables.get('__CONFIGURATION_NAME')

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global hogpMenuServer
    hogpMenuServer = profileHogpComponent.createBooleanSymbol('HOGP_BOOL_SERVER', None)
    hogpMenuServer.setLabel('Enable Server Role')
    hogpMenuServer.setDefaultValue(True)
    hogpMenuServer.setDependencies(hogpServerConfig, ["HOGP_BOOL_SERVER"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_hogps.c file - static file
    bleHogpsSourceFile = profileHogpComponent.createFileSymbol(None, None)
    bleHogpsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_hogps/ble_hogps.c')
    bleHogpsSourceFile.setOutputName('ble_hogps.c')
    bleHogpsSourceFile.setOverwrite(True)
    bleHogpsSourceFile.setDestPath('ble/profile_ble/ble_hogps/')
    bleHogpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_hogps/')
    bleHogpsSourceFile.setType('SOURCE')
    bleHogpsSourceFile.setEnabled(True)
    bleHogpsSourceFile.setMarkup(True)
    bleHogpsSourceFile.setDependencies(blehogpEnable, ["HOGP_BOOL_SERVER"])

    # Add ble_hogps.h file - static file
    bleHogpsHeaderFile = profileHogpComponent.createFileSymbol(None, None)
    bleHogpsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_hogps/ble_hogps.h')
    bleHogpsHeaderFile.setOutputName('ble_hogps.h')
    bleHogpsHeaderFile.setOverwrite(True)
    bleHogpsHeaderFile.setDestPath('ble/profile_ble/ble_hogps/')
    bleHogpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_hogps/')
    bleHogpsHeaderFile.setType('HEADER')
    bleHogpsHeaderFile.setEnabled(True)
    bleHogpsHeaderFile.setMarkup(True)
    bleHogpsHeaderFile.setDependencies(blehogpEnable, ["HOGP_BOOL_SERVER"])

    # Add app_hogps_handler.c file - static file
    bleHogpsAppSourceFile = profileHogpComponent.createFileSymbol(None, None)
    bleHogpsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_hogps_handler.c')
    bleHogpsAppSourceFile.setOutputName('app_hogps_handler.c')
    bleHogpsAppSourceFile.setOverwrite(True)
    bleHogpsAppSourceFile.setDestPath('../../app_ble')
    bleHogpsAppSourceFile.setProjectPath('app_ble')
    bleHogpsAppSourceFile.setType('Source')
    bleHogpsAppSourceFile.setEnabled(True)
    bleHogpsAppSourceFile.setDependencies(blehogpEnable, ["HOGP_BOOL_SERVER"])

    # Add app_hogps_handler.h file - static file
    bleHogpsAppHeaderFile = profileHogpComponent.createFileSymbol(None, None)
    bleHogpsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_hogps_handler.h')
    bleHogpsAppHeaderFile.setOutputName('app_hogps_handler.h')
    bleHogpsAppHeaderFile.setOverwrite(True)
    bleHogpsAppHeaderFile.setDestPath('../../app_ble')
    bleHogpsAppHeaderFile.setProjectPath('app_ble')
    bleHogpsAppHeaderFile.setType('HEADER')
    bleHogpsAppHeaderFile.setEnabled(True)
    bleHogpsAppHeaderFile.setDependencies(blehogpEnable, ["HOGP_BOOL_SERVER"])

def finalizeComponent(profileHogpComponent):
    print('Finalize Component: {}'.format(profileHogpComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

    global hogpMenuServer
    if hogpMenuServer.getValue() == True:
        print('PROFILE_HOGP: Load SERVICE_HIDS')
        res = Database.activateComponents(["SERVICE_HIDS"])
        res = Database.connectDependencies([['PROFILE_HOGP', 'BLE_HIDS_Dependency', 'SERVICE_HIDS', 'BLE_HIDS_Capability']])
        print('PROFILE_HOGP: Load SERVICE_BAS')
        res = Database.activateComponents(["SERVICE_BAS"])
        res = Database.connectDependencies([['PROFILE_HOGP', 'BLE_BAS_Dependency', 'SERVICE_BAS', 'BLE_BAS_Capability']])

        Database.setSymbolValue("BLE_STACK_LIB", "APP_HOGP_SERVER", True)

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    print('PROFILE_HOGP:onAttachmentConnected: configuring')

    if connectID == 'BLE_STACK_Dependency':
        global hogpMenuServer
        if hogpMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_HOGP_SERVER", True)
