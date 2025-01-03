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

def blePxpEnable(symbol, event):
    symbol.setEnabled(event["value"])

def blePxpsHanlderEnable(symbol, event):

    if (pxpMenuServer.getValue() == True) and (Database.getSymbolValue("BLE_STACK_LIB", "DISABLE_APP_CODE_GEN") == False):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def blePxpcHanlderEnable(symbol, event):

    if (pxpMenuClient.getValue() == True) and (Database.getSymbolValue("BLE_STACK_LIB", "DISABLE_APP_CODE_GEN") == False):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def pxpClientConfig(symbol, event):
    if event["value"] == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_CLIENT", False)
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_CLIENT", False)

def pxpServerConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_PXP: Load SERVICE_LLS')
        res = Database.activateComponents(["SERVICE_LLS"])
        res = Database.connectDependencies([['PROFILE_PXP', 'BLE_LLS_Dependency', 'SERVICE_LLS', 'BLE_LLS_Capability']])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_SERVER", True)
    else:
        if 'SERVICE_LLS' in Database.getActiveComponentIDs():
            print('PROFILE_PXP: Remove SERVICE_LLS')
            res = Database.deactivateComponents(["SERVICE_LLS"])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_SERVER", False)

def pxpIasConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_PXP: Load SERVICE_IAS')
        res = Database.activateComponents(["SERVICE_IAS"])
        res = Database.connectDependencies([['PROFILE_PXP', 'BLE_IAS_Dependency', 'SERVICE_IAS', 'BLE_IAS_Capability']])
    else:
        if 'SERVICE_IAS' in Database.getActiveComponentIDs():
             print('PROFILE_PXP: Remove SERVICE_IAS')
             res = Database.deactivateComponents(["SERVICE_IAS"])

def pxpTpsConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_PXP: Load SERVICE_TPS')
        res = Database.activateComponents(["SERVICE_TPS"])
        res = Database.connectDependencies([['PROFILE_PXP', 'BLE_TPS_Dependency', 'SERVICE_TPS', 'BLE_TPS_Capability']])
    else:
         if 'SERVICE_TPS' in Database.getActiveComponentIDs():
             print('PROFILE_PXP: Remove SERVICE_TPS')
             res = Database.deactivateComponents(["SERVICE_TPS"])

def instantiateComponent(profilePxpComponent):
    print('Load Module: PROFILE_PXP')
    configName = Variables.get('__CONFIGURATION_NAME')

    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    global pxpMenuClient
    pxpMenuClient = profilePxpComponent.createBooleanSymbol('PXP_BOOL_CLIENT', None)
    pxpMenuClient.setLabel('Enable Monitor Role')
    pxpMenuClient.setDefaultValue(False)
    pxpMenuClient.setDependencies(pxpClientConfig, ["PXP_BOOL_CLIENT"])

    # Immediate Alert Service Enable
    pxpmIas = profilePxpComponent.createBooleanSymbol('PXP_CLIENT_IAS', pxpMenuClient)
    pxpmIas.setLabel('Support Immediate Alert Service')
    pxpmIas.setDescription('Support Immediate Alert Service')
    pxpmIas.setDefaultValue(False)
    pxpmIas.setVisible(True)

    # TX Power Service Enable
    pxpmTps = profilePxpComponent.createBooleanSymbol('PXP_CLIENT_TPS', pxpMenuClient)
    pxpmTps.setLabel('Support TX Power Service')
    pxpmTps.setDescription('Support TX Power Service')
    pxpmTps.setDefaultValue(False)
    pxpmTps.setVisible(True)

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global pxpMenuServer
    pxpMenuServer = profilePxpComponent.createBooleanSymbol('PXP_BOOL_SERVER', None)
    pxpMenuServer.setLabel('Enable Reporter Role')
    pxpMenuServer.setDefaultValue(False)
    pxpMenuServer.setDependencies(pxpServerConfig, ["PXP_BOOL_SERVER"])

    # Immediate Alert Service Enable
    pxprIas = profilePxpComponent.createBooleanSymbol('PXP_SERVER_IAS', pxpMenuServer)
    pxprIas.setLabel('Enable Immediate Alert Service')
    pxprIas.setDescription('Enable Immediate Alert Service')
    pxprIas.setDefaultValue(False)
    pxprIas.setVisible(True)
    pxprIas.setDependencies(pxpIasConfig, ["PXP_SERVER_IAS"])

    # TX Power Service Enable
    pxprTps = profilePxpComponent.createBooleanSymbol('PXP_SERVER_TPS', pxpMenuServer)
    pxprTps.setLabel('Enable TX Power Service')
    pxprTps.setDescription('Enable TX Power Service')
    pxprTps.setDefaultValue(False)
    pxprTps.setVisible(True)
    pxprTps.setDependencies(pxpTpsConfig, ["PXP_SERVER_TPS"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################
    
    # Add ble_pxpr.c file - static file
    blePxprSourceFile = profilePxpComponent.createFileSymbol(None, None)
    blePxprSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_pxpr/ble_pxpr.c')
    blePxprSourceFile.setOutputName('ble_pxpr.c')
    blePxprSourceFile.setOverwrite(True)
    blePxprSourceFile.setDestPath('ble/profile_ble/ble_pxpr/')
    blePxprSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_pxpr/')
    blePxprSourceFile.setType('SOURCE')
    blePxprSourceFile.setEnabled(False)
    blePxprSourceFile.setMarkup(True)
    blePxprSourceFile.setDependencies(blePxpEnable, ["PXP_BOOL_SERVER"])

    # Add ble_pxpr.h file - static file
    blePxprHeaderFile = profilePxpComponent.createFileSymbol(None, None)
    blePxprHeaderFile.setSourcePath('driver/ble/templates/profile/ble_pxpr.h.ftl')
    blePxprHeaderFile.setOutputName('ble_pxpr.h')
    blePxprHeaderFile.setOverwrite(True)
    blePxprHeaderFile.setDestPath('ble/profile_ble/ble_pxpr/')
    blePxprHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_pxpr/')
    blePxprHeaderFile.setType('HEADER')
    blePxprHeaderFile.setEnabled(False)
    blePxprHeaderFile.setMarkup(True)
    blePxprHeaderFile.setDependencies(blePxpEnable, ["PXP_BOOL_SERVER"])

    # Add app_pxpr_handler.c file - static file
    blePxprAppSourceFile = profilePxpComponent.createFileSymbol(None, None)
    blePxprAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_pxpr_handler.c')
    blePxprAppSourceFile.setOutputName('app_pxpr_handler.c')
    blePxprAppSourceFile.setOverwrite(True)
    blePxprAppSourceFile.setDestPath('../../app_ble')
    blePxprAppSourceFile.setProjectPath('app_ble')
    blePxprAppSourceFile.setType('Source')
    blePxprAppSourceFile.setEnabled(False)
    blePxprAppSourceFile.setDependencies(blePxpsHanlderEnable, ["PXP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add app_pxpr_handler.h file - static file
    blePxprAppHeaderFile = profilePxpComponent.createFileSymbol(None, None)
    blePxprAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_pxpr_handler.h')
    blePxprAppHeaderFile.setOutputName('app_pxpr_handler.h')
    blePxprAppHeaderFile.setOverwrite(True)
    blePxprAppHeaderFile.setDestPath('../../app_ble')
    blePxprAppHeaderFile.setProjectPath('app_ble')
    blePxprAppHeaderFile.setType('HEADER')
    blePxprAppHeaderFile.setEnabled(False)
    blePxprAppHeaderFile.setDependencies(blePxpsHanlderEnable, ["PXP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])


    # Add ble_pxpm.c file - static file
    blePxpmSourceFile = profilePxpComponent.createFileSymbol(None, None)
    blePxpmSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_pxpm/ble_pxpm.c')
    blePxpmSourceFile.setOutputName('ble_pxpm.c')
    blePxpmSourceFile.setOverwrite(True)
    blePxpmSourceFile.setDestPath('ble/profile_ble/ble_pxpm/')
    blePxpmSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_pxpm/')
    blePxpmSourceFile.setType('SOURCE')
    blePxpmSourceFile.setEnabled(False)
    blePxpmSourceFile.setMarkup(True)
    blePxpmSourceFile.setDependencies(blePxpEnable, ["PXP_BOOL_CLIENT"])

    # Add ble_pxpm.h file - static file
    blePxpmHeaderFile = profilePxpComponent.createFileSymbol(None, None)
    blePxpmHeaderFile.setSourcePath('driver/ble/templates/profile/ble_pxpm.h.ftl')
    blePxpmHeaderFile.setOutputName('ble_pxpm.h')
    blePxpmHeaderFile.setOverwrite(True)
    blePxpmHeaderFile.setDestPath('ble/profile_ble/ble_pxpm/')
    blePxpmHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_pxpm/')
    blePxpmHeaderFile.setType('HEADER')
    blePxpmHeaderFile.setEnabled(False)
    blePxpmHeaderFile.setMarkup(True)
    blePxpmHeaderFile.setDependencies(blePxpEnable, ["PXP_BOOL_CLIENT"])

    # Add app_pxpm_handler.c file - static file
    blePxpmAppSourceFile = profilePxpComponent.createFileSymbol(None, None)
    blePxpmAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_pxpm_handler.c')
    blePxpmAppSourceFile.setOutputName('app_pxpm_handler.c')
    blePxpmAppSourceFile.setOverwrite(True)
    blePxpmAppSourceFile.setDestPath('../../app_ble')
    blePxpmAppSourceFile.setProjectPath('app_ble')
    blePxpmAppSourceFile.setType('Source')
    blePxpmAppSourceFile.setEnabled(False)
    blePxpmAppSourceFile.setDependencies(blePxpcHanlderEnable, ["PXP_BOOL_CLIENT", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add app_pxpm_handler.h file - static file
    blePxpmAppHeaderFile = profilePxpComponent.createFileSymbol(None, None)
    blePxpmAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_pxpm_handler.h')
    blePxpmAppHeaderFile.setOutputName('app_pxpm_handler.h')
    blePxpmAppHeaderFile.setOverwrite(True)
    blePxpmAppHeaderFile.setDestPath('../../app_ble')
    blePxpmAppHeaderFile.setProjectPath('app_ble')
    blePxpmAppHeaderFile.setType('HEADER')
    blePxpmAppHeaderFile.setEnabled(False)
    blePxpmAppHeaderFile.setDependencies(blePxpcHanlderEnable, ["PXP_BOOL_CLIENT", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])


def finalizeComponent(profilePxpComponent):
    print('Finalizing: {}'.format(profilePxpComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    #print('PROFILE_PXP:onAttachmentConnected: source = {} target = {}'.format(sourceID, targetID))
    print('PROFILE_PXP:onAttachmentConnected: configuring')

    if connectID == 'BLE_STACK_Dependency':
        global pxpMenuServer
        global pxpMenuClient

        if pxpMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_SERVER", True)

        if pxpMenuClient.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
            Database.setSymbolValue("BLE_STACK_LIB", "APP_PXP_CLIENT", True)

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
