execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
elif devFamily == "pic32cx_bz6_family":
    srcPath = "ble_src_bz6"
else:
    print("Device not support")

def bleAnpEnable(symbol, event):
    symbol.setEnabled(event["value"])

def bleAnpsHanlderEnable(symbol, event):

    if (anpMenuServer.getValue() == True) and (Database.getSymbolValue("BLE_STACK_LIB", "DISABLE_APP_CODE_GEN") == False):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def bleAnpcHanlderEnable(symbol, event):

    if (anpMenuClient.getValue() == True) and (Database.getSymbolValue("BLE_STACK_LIB", "DISABLE_APP_CODE_GEN") == False):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def anpClientConfig(symbol, event):
    if event["value"] == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_CLIENT", False)
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_CLIENT", False)

def anpServerConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_ANP: Load SERVICE_ANS')
        res = Database.activateComponents(["SERVICE_ANS"])
        res = Database.connectDependencies([['PROFILE_ANP', 'BLE_ANS_Dependency', 'SERVICE_ANS', 'BLE_ANS_Capability']])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_SERVER", True)
    else:
        if 'SERVICE_ANS' in Database.getActiveComponentIDs():
            print('PROFILE_ANP: Remove SERVICE_ANS')
            res = Database.deactivateComponents(["SERVICE_ANS"])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_SERVER", False)

def instantiateComponent(profileAnpComponent):
    print('Load Module: PROFILE_ANP')
    configName = Variables.get('__CONFIGURATION_NAME')

    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    global anpMenuClient
    anpMenuClient = profileAnpComponent.createBooleanSymbol('ANP_BOOL_CLIENT', None)
    anpMenuClient.setLabel('Enable Client Role')
    anpMenuClient.setDefaultValue(False)
    anpMenuClient.setDependencies(anpClientConfig, ["ANP_BOOL_CLIENT"])

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global anpMenuServer
    anpMenuServer = profileAnpComponent.createBooleanSymbol('ANP_BOOL_SERVER', None)
    anpMenuServer.setLabel('Enable Server Role')
    anpMenuServer.setDefaultValue(False)
    anpMenuServer.setDependencies(anpServerConfig, ["ANP_BOOL_SERVER"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_anps.c file - static file
    bleAnpsSourceFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_anps/ble_anps.c')
    bleAnpsSourceFile.setOutputName('ble_anps.c')
    bleAnpsSourceFile.setOverwrite(True)
    bleAnpsSourceFile.setDestPath('ble/profile_ble/ble_anps/')
    bleAnpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_anps/')
    bleAnpsSourceFile.setType('SOURCE')
    bleAnpsSourceFile.setEnabled(False)
    bleAnpsSourceFile.setMarkup(True)
    bleAnpsSourceFile.setDependencies(bleAnpEnable, ["ANP_BOOL_SERVER"])

    # Add ble_anps.h file - static file
    bleAnpsHeaderFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_anps/ble_anps.h')
    bleAnpsHeaderFile.setOutputName('ble_anps.h')
    bleAnpsHeaderFile.setOverwrite(True)
    bleAnpsHeaderFile.setDestPath('ble/profile_ble/ble_anps/')
    bleAnpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_anps/')
    bleAnpsHeaderFile.setType('HEADER')
    bleAnpsHeaderFile.setEnabled(False)
    bleAnpsHeaderFile.setMarkup(True)
    bleAnpsHeaderFile.setDependencies(bleAnpEnable, ["ANP_BOOL_SERVER"])

    # Add app_anps_handler.c file - static file
    bleAnpsAppSourceFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_anps_handler.c')
    bleAnpsAppSourceFile.setOutputName('app_anps_handler.c')
    bleAnpsAppSourceFile.setOverwrite(True)
    bleAnpsAppSourceFile.setDestPath('../../app_ble')
    bleAnpsAppSourceFile.setProjectPath('app_ble')
    bleAnpsAppSourceFile.setType('Source')
    bleAnpsAppSourceFile.setEnabled(False)
    bleAnpsAppSourceFile.setDependencies(bleAnpsHanlderEnable, ["ANP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add app_anps_handler.h file - static file
    bleAnpsAppHeaderFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_anps_handler.h')
    bleAnpsAppHeaderFile.setOutputName('app_anps_handler.h')
    bleAnpsAppHeaderFile.setOverwrite(True)
    bleAnpsAppHeaderFile.setDestPath('../../app_ble')
    bleAnpsAppHeaderFile.setProjectPath('app_ble')
    bleAnpsAppHeaderFile.setType('HEADER')
    bleAnpsAppHeaderFile.setEnabled(False)
    bleAnpsAppHeaderFile.setDependencies(bleAnpsHanlderEnable, ["ANP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])


    # Add ble_anpc.c file - static file
    bleAnpcSourceFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpcSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_anpc/ble_anpc.c')
    bleAnpcSourceFile.setOutputName('ble_anpc.c')
    bleAnpcSourceFile.setOverwrite(True)
    bleAnpcSourceFile.setDestPath('ble/profile_ble/ble_anpc/')
    bleAnpcSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_anpc/')
    bleAnpcSourceFile.setType('SOURCE')
    bleAnpcSourceFile.setEnabled(False)
    bleAnpcSourceFile.setMarkup(True)
    bleAnpcSourceFile.setDependencies(bleAnpEnable, ["ANP_BOOL_CLIENT"])

    # Add ble_anpc.h file - static file
    bleAnpcHeaderFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpcHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_anpc/ble_anpc.h')
    bleAnpcHeaderFile.setOutputName('ble_anpc.h')
    bleAnpcHeaderFile.setOverwrite(True)
    bleAnpcHeaderFile.setDestPath('ble/profile_ble/ble_anpc/')
    bleAnpcHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_anpc/')
    bleAnpcHeaderFile.setType('HEADER')
    bleAnpcHeaderFile.setEnabled(False)
    bleAnpcHeaderFile.setMarkup(True)
    bleAnpcHeaderFile.setDependencies(bleAnpEnable, ["ANP_BOOL_CLIENT"])

    # Add app_anpc_handler.c file - static file
    bleAnpcAppSourceFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpcAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_anpc_handler.c')
    bleAnpcAppSourceFile.setOutputName('app_anpc_handler.c')
    bleAnpcAppSourceFile.setOverwrite(True)
    bleAnpcAppSourceFile.setDestPath('../../app_ble')
    bleAnpcAppSourceFile.setProjectPath('app_ble')
    bleAnpcAppSourceFile.setType('Source')
    bleAnpcAppSourceFile.setEnabled(False)
    bleAnpcAppSourceFile.setDependencies(bleAnpcHanlderEnable, ["ANP_BOOL_CLIENT", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add app_anpc_handler.h file - static file
    bleAnpcAppHeaderFile = profileAnpComponent.createFileSymbol(None, None)
    bleAnpcAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_anpc_handler.h')
    bleAnpcAppHeaderFile.setOutputName('app_anpc_handler.h')
    bleAnpcAppHeaderFile.setOverwrite(True)
    bleAnpcAppHeaderFile.setDestPath('../../app_ble')
    bleAnpcAppHeaderFile.setProjectPath('app_ble')
    bleAnpcAppHeaderFile.setType('HEADER')
    bleAnpcAppHeaderFile.setEnabled(False)
    bleAnpcAppHeaderFile.setDependencies(bleAnpcHanlderEnable, ["ANP_BOOL_CLIENT", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

def finalizeComponent(profileAnpComponent):
    print('Finalizing: {}'.format(profileAnpComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    #print('PROFILE_ANP:onAttachmentConnected: source = {} target = {}'.format(sourceID, targetID))
    print('PROFILE_ANP:onAttachmentConnected: configuring')

    if connectID == 'BLE_STACK_Dependency':
        global anpMenuServer
        global anpMenuClient

        if anpMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_SERVER", True)

        if anpMenuClient.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
            Database.setSymbolValue("BLE_STACK_LIB", "APP_ANP_CLIENT", True)

def handleMessage(messageID, args):
    '''
    message formats
        CONTROLLER_ONLY_MODE_ENABLED: Controller Only Mode is enabled
            payload:    {
                        'target':       <this module>
                        'source':       <module name>
                        }
    '''
    bleServiceProfileComponentIDs = []

    if(messageID == "CONTROLLER_ONLY_MODE_ENABLED"):
        bleServiceProfileComponentIDs.append(args["target"])
        Database.deactivateComponents(bleServiceProfileComponentIDs)
