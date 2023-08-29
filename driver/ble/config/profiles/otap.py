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
                      
def bleOtapDfuEnable(symbol, event):
    global otapMenuServer
    if otapMenuServer.getValue() == True:
        if Database.getSymbolValue("BLE_STACK_LIB", "BOOL_BLE_UTIL_DFU") == True:
            symbol.setVisible(False)
        else:
            symbol.setVisible(True)
    else:
        symbol.setVisible(False)



def bleOtapEnable(symbol, event):
    symbol.setEnabled(event["value"])

def otapClientConfig(symbol, event):
    if event["value"] == True:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_CLIENT", True)
        if (Database.getSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT") == False):
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
    else:
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_CLIENT", False)

def otapServerConfig(symbol, event):
    print('PROFILE_OTAP: otapServerConfig')
    if event["value"] == True:
        print('PROFILE_OTAP: Load SERVICE_OTAS')
        res = Database.activateComponents(["SERVICE_OTAS"])
        res = Database.connectDependencies([['PROFILE_OTAP', 'BLE_OTAS_Dependency', 'SERVICE_OTAS', 'BLE_OTAS_Capability']])
        Database.setSymbolValue("BLE_STACK_LIB", "BOOL_BLE_UTIL_DFU", True)
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_SERVER", True)
    else:
        if 'SERVICE_OTAS' in Database.getActiveComponentIDs():
            print('PROFILE_OTAP: Remove SERVICE_OTAS')
            res = Database.deactivateComponents(["SERVICE_OTAS"])
        Database.setSymbolValue("BLE_STACK_LIB", "BOOL_BLE_UTIL_DFU", False)
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_SERVER", False)

def instantiateComponent(profileOtapComponent):
    print('Load Module: PROFILE_OTAP')
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
    global otapMenuClient
    otapMenuClient = profileOtapComponent.createBooleanSymbol('OTAP_BOOL_CLIENT', None)
    otapMenuClient.setLabel('Enable Client Role')
    otapMenuClient.setDefaultValue(False)
    otapMenuClient.setVisible(True)
    otapMenuClient.setDependencies(otapClientConfig, ["OTAP_BOOL_CLIENT"])

    #################################################################
    ##################      Server Role Settings      ###############
    #################################################################
    global otapMenuServer
    otapMenuServer = profileOtapComponent.createBooleanSymbol('OTAP_BOOL_SERVER', None)
    otapMenuServer.setLabel('Enable Server Role')
    otapMenuServer.setDefaultValue(True)
    otapMenuServer.setDependencies(otapServerConfig, ["OTAP_BOOL_SERVER"])
    
    # DFU module message
    otapCmt = profileOtapComponent.createCommentSymbol("OTAP_CMT_DFU", None)
    otapCmt.setLabel("**** DFU module must be enabled. ****")
    otapCmt.setDependencies(bleOtapDfuEnable, ["OTAP_BOOL_SERVER", "BLE_STACK_LIB.BOOL_BLE_UTIL_DFU"])

    #################################################################
    ##################        Add Source File         ###############
    #################################################################
    # Add ble_otaps.c file - static file
    bleOtapsSourceFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_otaps/ble_otaps.c')
    bleOtapsSourceFile.setOutputName('ble_otaps.c')
    bleOtapsSourceFile.setOverwrite(True)
    bleOtapsSourceFile.setDestPath('ble/profile_ble/ble_otaps/')
    bleOtapsSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_otaps/')
    bleOtapsSourceFile.setType('SOURCE')
    bleOtapsSourceFile.setEnabled(True)
    bleOtapsSourceFile.setMarkup(True)
    bleOtapsSourceFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])
    
    # Add ble_otaps.h file - static file
    bleOtapsHeaderFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_otaps/ble_otaps.h')
    bleOtapsHeaderFile.setOutputName('ble_otaps.h')
    bleOtapsHeaderFile.setOverwrite(True)
    bleOtapsHeaderFile.setDestPath('ble/profile_ble/ble_otaps/')
    bleOtapsHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_otaps/')
    bleOtapsHeaderFile.setType('HEADER')
    bleOtapsHeaderFile.setEnabled(True)
    bleOtapsHeaderFile.setMarkup(True)
    bleOtapsHeaderFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])

    # Add app_otaps_handler.c file - static file
    bleOtapsAppSourceFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapsAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_otaps_handler.c')
    bleOtapsAppSourceFile.setOutputName('app_otaps_handler.c')
    bleOtapsAppSourceFile.setOverwrite(True)
    bleOtapsAppSourceFile.setDestPath('../../app_ble')
    bleOtapsAppSourceFile.setProjectPath('app_ble')
    bleOtapsAppSourceFile.setType('Source')
    bleOtapsAppSourceFile.setEnabled(True)
    bleOtapsAppSourceFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])

    # Add app_otaps_handler.h file - static file
    bleOtapsAppHeaderFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapsAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_otaps_handler.h')
    bleOtapsAppHeaderFile.setOutputName('app_otaps_handler.h')
    bleOtapsAppHeaderFile.setOverwrite(True)
    bleOtapsAppHeaderFile.setDestPath('../../app_ble')
    bleOtapsAppHeaderFile.setProjectPath('app_ble')
    bleOtapsAppHeaderFile.setType('HEADER')
    bleOtapsAppHeaderFile.setEnabled(True)
    bleOtapsAppHeaderFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])

    # Add app_ble_utility.c - static file
    bleUtilAppSourceFile = profileOtapComponent.createFileSymbol(None, None)
    bleUtilAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_ble_utility.c')
    bleUtilAppSourceFile.setOutputName('app_ble_utility.c')
    bleUtilAppSourceFile.setOverwrite(True)
    bleUtilAppSourceFile.setDestPath('../../app_ble')
    bleUtilAppSourceFile.setProjectPath('app_ble')
    bleUtilAppSourceFile.setType('SOURCE')
    bleUtilAppSourceFile.setEnabled(True)
    bleUtilAppSourceFile.setMarkup(True)
    bleUtilAppSourceFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])

    # Add app_ble_utility.h - static file
    bleUtilAppHeaderFile = profileOtapComponent.createFileSymbol(None, None)
    bleUtilAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_ble_utility.h')
    bleUtilAppHeaderFile.setOutputName('app_ble_utility.h')
    bleUtilAppHeaderFile.setOverwrite(True)
    bleUtilAppHeaderFile.setDestPath('../../app_ble')
    bleUtilAppHeaderFile.setProjectPath('app_ble')
    bleUtilAppHeaderFile.setType('HEADER')
    bleUtilAppHeaderFile.setEnabled(True)
    bleUtilAppHeaderFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_SERVER"])


    # Add ble_otapc.c file - static file
    bleOtapcSourceFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapcSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_otapc/ble_otapc.c')
    bleOtapcSourceFile.setOutputName('ble_otapc.c')
    bleOtapcSourceFile.setOverwrite(True)
    bleOtapcSourceFile.setDestPath('ble/profile_ble/ble_otapc/')
    bleOtapcSourceFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_otapc/')
    bleOtapcSourceFile.setType('SOURCE')
    bleOtapcSourceFile.setEnabled(False)
    bleOtapcSourceFile.setMarkup(True)
    bleOtapcSourceFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_CLIENT"])

    # Add ble_otapc.h file - static file
    bleOtapcHeaderFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapcHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/profile_ble/ble_otapc/ble_otapc.h')
    bleOtapcHeaderFile.setOutputName('ble_otapc.h')
    bleOtapcHeaderFile.setOverwrite(True)
    bleOtapcHeaderFile.setDestPath('ble/profile_ble/ble_otapc/')
    bleOtapcHeaderFile.setProjectPath('config/' + configName + '/ble/profile_ble/ble_otapc/')
    bleOtapcHeaderFile.setType('HEADER')
    bleOtapcHeaderFile.setEnabled(False)
    bleOtapcHeaderFile.setMarkup(True)
    bleOtapcHeaderFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_CLIENT"])

    # Add app_otapc_handler.c file - static file
    bleOtapcAppSourceFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapcAppSourceFile.setSourcePath('driver/ble/src/ble_app/app_otapc_handler.c')
    bleOtapcAppSourceFile.setOutputName('app_otapc_handler.c')
    bleOtapcAppSourceFile.setOverwrite(True)
    bleOtapcAppSourceFile.setDestPath('../../app_ble')
    bleOtapcAppSourceFile.setProjectPath('app_ble')
    bleOtapcAppSourceFile.setType('Source')
    bleOtapcAppSourceFile.setEnabled(False)
    bleOtapcAppSourceFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_CLIENT"])

    # Add app_otapc_handler.h file - static file
    bleOtapcAppHeaderFile = profileOtapComponent.createFileSymbol(None, None)
    bleOtapcAppHeaderFile.setSourcePath('driver/ble/src/ble_app/app_otapc_handler.h')
    bleOtapcAppHeaderFile.setOutputName('app_otapc_handler.h')
    bleOtapcAppHeaderFile.setOverwrite(True)
    bleOtapcAppHeaderFile.setDestPath('../../app_ble')
    bleOtapcAppHeaderFile.setProjectPath('app_ble')
    bleOtapcAppHeaderFile.setType('HEADER')
    bleOtapcAppHeaderFile.setEnabled(False)
    bleOtapcAppHeaderFile.setDependencies(bleOtapEnable, ["OTAP_BOOL_CLIENT"])


def finalizeComponent(profileOtapComponent):
    print('Finalizing: {}'.format(profileOtapComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

    if otapMenuServer.getValue() == True:
        Database.activateComponents(['SERVICE_OTAS'])
        res = Database.connectDependencies([['PROFILE_OTAP', 'BLE_OTAS_Dependency', 'SERVICE_OTAS', 'BLE_OTAS_Capability']])

        Database.setSymbolValue("BLE_STACK_LIB", "BOOL_BLE_UTIL_DFU", True)
        Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_SERVER", True)

def onAttachmentConnected(source, target):
    targetID = target["component"].getID()
    sourceID = source["component"].getID()
    connectID = source["id"]

    #print('PROFILE_OTAP:onAttachmentConnected: source = {} target = {}'.format(sourceID, targetID))
    print('PROFILE_OTAP:onAttachmentConnected: configuring')

    if connectID == 'BLE_STACK_Dependency':
        global otapMenuServer
        global otapMenuClient

        if otapMenuServer.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "BOOL_BLE_UTIL_DFU", True)
            Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_SERVER", True)

        if otapMenuClient.getValue() == True:
            Database.setSymbolValue("BLE_STACK_LIB", "BLE_BOOL_GATT_CLIENT", True)
            Database.setSymbolValue("BLE_STACK_LIB", "APP_OTAP_CLIENT", True)