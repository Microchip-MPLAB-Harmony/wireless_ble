pic32cx_bz2_family = {'PIC32CX1012BZ25048',
                      'PIC32CX1012BZ25032',
                      'PIC32CX1012BZ24032',
                      'WBZ451',
                      'WBZ450',
                      }

pic32cx_bz3_family = {'PIC32CX5109BZ31032',
                      'PIC32CX5109BZ31048',
                      'WBZ351',
                      'WBZ350',
                      }

def bleTrspEnable(symbol, event):
    symbol.setEnabled(event["value"]) 

def trspClientConfig(symbol, event):
    if event["value"] == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_CLIENT", False)

def trspServerConfig(symbol, event):
    if event["value"] == True:
        print('PROFILE_TRSP: Load SERVICE_TRS')
        res = Database.activateComponents(["SERVICE_TRS"])
        res = Database.connectDependencies([['PROFILE_TRSP', 'BLE_TRS_Dependency', 'SERVICE_TRS', 'BLE_TRS_Capability']])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_SERVER", True)
    else:
        if 'SERVICE_TRS' in Database.getActiveComponentIDs():
            print('PROFILE_TRSP: Remove SERVICE_TRS')
            res = Database.deactivateComponents(["SERVICE_TRS"])
        Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_SERVER", False)

def instantiateComponent(profileTrspComponent):
    print('Load Module: PROFILE_TRSP')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"
    #################################################################
    ##################      Client Role Settings      ###############
    #################################################################
    global trpMenuClient
    trpMenuClient = profileTrspComponent.createBooleanSymbol('TRSP_BOOL_CLIENT', None)
    trpMenuClient.setLabel('Enable Client Role')
    trpMenuClient.setDefaultValue(False)
    trpMenuClient.setDependencies(trspClientConfig, ["TRSP_BOOL_CLIENT"])

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global trpMenuServer
    trpMenuServer = profileTrspComponent.createBooleanSymbol('TRSP_BOOL_SERVER', None)
    trpMenuServer.setLabel('Enable Server Role')
    trpMenuServer.setDefaultValue(False)
    trpMenuServer.setDependencies(trspServerConfig, ["TRSP_BOOL_SERVER"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_trsps.c file - static file
    bleTrspsSourceFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrspsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trsps/ble_trsps.c')
    bleTrspsSourceFile.setOutputName('ble_trsps.c')
    bleTrspsSourceFile.setOverwrite(True)
    bleTrspsSourceFile.setDestPath('ble/profile_ble/ble_trsps/')
    bleTrspsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trsps/')
    bleTrspsSourceFile.setType('SOURCE')
    bleTrspsSourceFile.setEnabled(False)
    bleTrspsSourceFile.setMarkup(True)
    bleTrspsSourceFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_SERVER"])

    # Add ble_trsps.h file - static file
    bleTrspsHeaderFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrspsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trsps/ble_trsps.h')
    bleTrspsHeaderFile.setOutputName('ble_trsps.h')
    bleTrspsHeaderFile.setOverwrite(True)
    bleTrspsHeaderFile.setDestPath('ble/profile_ble/ble_trsps/')
    bleTrspsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trsps/')
    bleTrspsHeaderFile.setType('HEADER')
    bleTrspsHeaderFile.setEnabled(False)
    bleTrspsHeaderFile.setMarkup(True)
    bleTrspsHeaderFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_SERVER"])

    # Add app_trsps_handler.c file - static file
    bleTrspsAppSourceFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrspsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_trsps_handler.c')
    bleTrspsAppSourceFile.setOutputName('app_trsps_handler.c')
    bleTrspsAppSourceFile.setOverwrite(True)
    bleTrspsAppSourceFile.setDestPath('../../app_ble')
    bleTrspsAppSourceFile.setProjectPath('app_ble')
    bleTrspsAppSourceFile.setType('Source')
    bleTrspsAppSourceFile.setEnabled(False)
    bleTrspsAppSourceFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_SERVER"])

    # Add app_trsps_handler.h file - static file
    bleTrspsAppHeaderFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrspsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_trsps_handler.h')
    bleTrspsAppHeaderFile.setOutputName('app_trsps_handler.h')
    bleTrspsAppHeaderFile.setOverwrite(True)
    bleTrspsAppHeaderFile.setDestPath('../../app_ble')
    bleTrspsAppHeaderFile.setProjectPath('app_ble')
    bleTrspsAppHeaderFile.setType('HEADER')
    bleTrspsAppHeaderFile.setEnabled(False)
    bleTrspsAppHeaderFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_SERVER"])


    # Add ble_trspc.c file - static file
    bleTrpcSourceFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrpcSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trspc/ble_trspc.c')
    bleTrpcSourceFile.setOutputName('ble_trspc.c')
    bleTrpcSourceFile.setOverwrite(True)
    bleTrpcSourceFile.setDestPath('ble/profile_ble/ble_trspc/')
    bleTrpcSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trspc/')
    bleTrpcSourceFile.setType('SOURCE')
    bleTrpcSourceFile.setEnabled(False)
    bleTrpcSourceFile.setMarkup(True)
    bleTrpcSourceFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_CLIENT"])

    # Add ble_trspc.h file - static file
    bleTrpcHeaderFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrpcHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_trspc/ble_trspc.h')
    bleTrpcHeaderFile.setOutputName('ble_trspc.h')
    bleTrpcHeaderFile.setOverwrite(True)
    bleTrpcHeaderFile.setDestPath('ble/profile_ble/ble_trspc/')
    bleTrpcHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_trspc/')
    bleTrpcHeaderFile.setType('HEADER')
    bleTrpcHeaderFile.setEnabled(False)
    bleTrpcHeaderFile.setMarkup(True)
    bleTrpcHeaderFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_CLIENT"])

    # Add app_trspc_handler.c file - static file
    bleTrpcAppSourceFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrpcAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_trspc_handler.c')
    bleTrpcAppSourceFile.setOutputName('app_trspc_handler.c')
    bleTrpcAppSourceFile.setOverwrite(True)
    bleTrpcAppSourceFile.setDestPath('../../app_ble')
    bleTrpcAppSourceFile.setProjectPath('app_ble')
    bleTrpcAppSourceFile.setType('Source')
    bleTrpcAppSourceFile.setEnabled(False)
    bleTrpcAppSourceFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_CLIENT"])

    # Add app_trspc_handler.h file - static file
    bleTrpcAppHeaderFile = profileTrspComponent.createFileSymbol(None, None)
    bleTrpcAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_trspc_handler.h')
    bleTrpcAppHeaderFile.setOutputName('app_trspc_handler.h')
    bleTrpcAppHeaderFile.setOverwrite(True)
    bleTrpcAppHeaderFile.setDestPath('../../app_ble')
    bleTrpcAppHeaderFile.setProjectPath('app_ble')
    bleTrpcAppHeaderFile.setType('HEADER')
    bleTrpcAppHeaderFile.setEnabled(False)
    bleTrpcAppHeaderFile.setDependencies(bleTrspEnable, ["TRSP_BOOL_CLIENT"])


def finalizeComponent(profileTrspComponent):
    print('Finalizing: {}'.format(profileTrspComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]
    #print('PROFILE_TRSP:onAttachmentConnected: source = {} target = {}'.format(sourceID, targetID))
    print('PROFILE_TRSP:onAttachmentConnected: configuring')

    if targetID == 'BLE_STACK_LIB':
        global trpMenuServer
        global trpMenuClient

        if trpMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_SERVER", True)

        if trpMenuClient.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
            Database.setSymbolValue("BLE_STACK_LIB", "APP_TRSP_CLIENT", True)