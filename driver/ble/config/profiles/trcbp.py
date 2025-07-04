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

def bleTrcbpEnable(symbol, event):
    symbol.setEnabled(event["value"])

def bleTrcbpHanlderEnable(symbol, event):

    if (trcbpMenuServer.getValue() == True) and (Database.getSymbolValue("BLE_STACK_LIB", "DISABLE_APP_CODE_GEN") == False):
        symbol.setEnabled(True)
    else:
        symbol.setEnabled(False)

def trcbpServerConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_TRCBP: Load SERVICE_TRCBS')
        res = Database.activateComponents(["SERVICE_TRCBS"])
        res = Database.connectDependencies([['PROFILE_TRCBP', 'BLE_TRCBS_Dependency', 'SERVICE_TRCBS', 'BLE_TRCBS_Capability']])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRCBP_SERVER", True)
    else:
        if 'SERVICE_TRCBS' in Database.getActiveComponentIDs():
            print('PROFILE_TRCBP: Remove SERVICE_TRCBS')
            res = Database.deactivateComponents(["SERVICE_TRCBS"])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRCBP_SERVER", False)

def instantiateComponent(profileTrcbpComponent):
    print('Load Module: PROFILE_TRCBP')
    configName = Variables.get('__CONFIGURATION_NAME')

    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    trcbpMenuClient = profileTrcbpComponent.createBooleanSymbol('TRCBP_BOOL_CLIENT', None)
    trcbpMenuClient.setLabel('Enable Client Role')
    trcbpMenuClient.setDefaultValue(False)
    trcbpMenuClient.setVisible(False)


    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global trcbpMenuServer
    trcbpMenuServer = profileTrcbpComponent.createBooleanSymbol('TRCBP_BOOL_SERVER', None)
    trcbpMenuServer.setLabel('Enable Server Role')
    trcbpMenuServer.setDefaultValue(False)
    trcbpMenuServer.setDependencies(trcbpServerConfig, ["TRCBP_BOOL_SERVER"])


    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_trcbps.c file
    bleTrcbpsSourceFile = profileTrcbpComponent.createFileSymbol(None, None)
    bleTrcbpsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trcbps/ble_trcbps.c')
    bleTrcbpsSourceFile.setOutputName('ble_trcbps.c')
    bleTrcbpsSourceFile.setOverwrite(True)
    bleTrcbpsSourceFile.setDestPath('ble/profile_ble/ble_trcbps/')
    bleTrcbpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbps/')
    bleTrcbpsSourceFile.setType('SOURCE')
    bleTrcbpsSourceFile.setEnabled(False)
    bleTrcbpsSourceFile.setMarkup(True)
    bleTrcbpsSourceFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_SERVER"])

    # Add ble_trcbps.h file
    bleTrcbpsHeaderFile = profileTrcbpComponent.createFileSymbol(None, None)
    bleTrcbpsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trcbps/ble_trcbps.h')
    bleTrcbpsHeaderFile.setOutputName('ble_trcbps.h')
    bleTrcbpsHeaderFile.setOverwrite(True)
    bleTrcbpsHeaderFile.setDestPath('ble/profile_ble/ble_trcbps/')
    bleTrcbpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbps/')
    bleTrcbpsHeaderFile.setType('HEADER')
    bleTrcbpsHeaderFile.setEnabled(False)
    bleTrcbpsHeaderFile.setMarkup(True)
    bleTrcbpsHeaderFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_SERVER"])

    # Add app_trcbps_handler.c file - static file
    bleTrcbpsAppSourceFile = profileTrcbpComponent.createFileSymbol(None, None)
    bleTrcbpsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_trcbps_handler.c')
    bleTrcbpsAppSourceFile.setOutputName('app_trcbps_handler.c')
    bleTrcbpsAppSourceFile.setOverwrite(True)
    bleTrcbpsAppSourceFile.setDestPath('../../app_ble')
    bleTrcbpsAppSourceFile.setProjectPath('app_ble')
    bleTrcbpsAppSourceFile.setType('Source')
    bleTrcbpsAppSourceFile.setEnabled(False)
    bleTrcbpsAppSourceFile.setDependencies(bleTrcbpHanlderEnable, ["TRCBP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add app_trcbps_handler.h file - static file
    bleTrcbpsAppHeaderFile = profileTrcbpComponent.createFileSymbol(None, None)
    bleTrcbpsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_trcbps_handler.h')
    bleTrcbpsAppHeaderFile.setOutputName('app_trcbps_handler.h')
    bleTrcbpsAppHeaderFile.setOverwrite(True)
    bleTrcbpsAppHeaderFile.setDestPath('../../app_ble')
    bleTrcbpsAppHeaderFile.setProjectPath('app_ble')
    bleTrcbpsAppHeaderFile.setType('HEADER')
    bleTrcbpsAppHeaderFile.setEnabled(False)
    bleTrcbpsAppHeaderFile.setDependencies(bleTrcbpHanlderEnable, ["TRCBP_BOOL_SERVER", "BLE_STACK_LIB.DISABLE_APP_CODE_GEN"])

    # Add ble_trcbpc.c file
    #bleTrcbpsSourceFile = profileTrcbpComponent.createFileSymbol(None, None)
    #bleTrcbpsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trcbpc/ble_trcbpc.c')
    #bleTrcbpsSourceFile.setOutputName('ble_trcbpc.c')
    #bleTrcbpsSourceFile.setOverwrite(True)
    #bleTrcbpsSourceFile.setDestPath('ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsSourceFile.setType('SOURCE')
    #bleTrcbpsSourceFile.setEnabled(False)
    #bleTrcbpsSourceFile.setMarkup(True)
    #bleTrcbpsSourceFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_CLIENT"])

    # Add ble_trcbpc.h file
    #bleTrcbpsHeaderFile = profileTrcbpComponent.createFileSymbol(None, None)
    #bleTrcbpsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trcbpc/ble_trcbpc.h')
    #bleTrcbpsHeaderFile.setOutputName('ble_trcbpc.h')
    #bleTrcbpsHeaderFile.setOverwrite(True)
    #bleTrcbpsHeaderFile.setDestPath('ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trcbpc/')
    #bleTrcbpsHeaderFile.setType('HEADER')
    #bleTrcbpsHeaderFile.setEnabled(False)
    #bleTrcbpsHeaderFile.setMarkup(True)
    #bleTrcbpsHeaderFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_CLIENT"])

    # Add app_trcbpc.c file - static file
    #bleTrcbpcAppSourceFile = profileTrcbpComponent.createFileSymbol(None, None)
    #bleTrcbpcAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_trcbpc_handler.c')
    #bleTrcbpcAppSourceFile.setOutputName('app_trcbpc_handler.c')
    #bleTrcbpcAppSourceFile.setOverwrite(True)
    #bleTrcbpcAppSourceFile.setDestPath('../../')
    #bleTrcbpcAppSourceFile.setProjectPath('')
    #bleTrcbpcAppSourceFile.setType('Source')
    #bleTrcbpcAppSourceFile.setEnabled(True)
    #bleTrcbpcAppSourceFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_CLIENT"])

    # Add app_trcbpc.h file - static file
    #bleTrcbpcAppHeaderFile = profileTrcbpComponent.createFileSymbol(None, None)
    #bleTrcbpcAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_trcbpc_handler.h')
    #bleTrcbpcAppHeaderFile.setOutputName('app_trcbpc_handler.h')
    #bleTrcbpcAppHeaderFile.setOverwrite(True)
    #bleTrcbpcAppHeaderFile.setDestPath('../../')
    #bleTrcbpcAppHeaderFile.setProjectPath('')
    #bleTrcbpcAppHeaderFile.setType('HEADER')
    #bleTrcbpcAppHeaderFile.setEnabled(True)
    #bleTrcbpcAppHeaderFile.setDependencies(bleTrcbpEnable, ["TRCBP_BOOL_SERVER"])


def finalizeComponent(profileTrcbpComponent):
    print('Finalizing: {}'.format(profileTrcbpComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    #print('PROFILE_TRCBP:onAttachmentConnected: source = {} target = {}'.format(sourceID, targetID))
    print('PROFILE_TRCBP:onAttachmentConnected')

    if connectID == 'BLE_STACK_Dependency':
        global trcbpMenuServer

        if trcbpMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_TRCBP_SERVER", True)

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
