import string

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

def formatHexString2HexBytes(hexString, byteList):
    hexStringLength = len(hexString)

    for byte in range(0, hexStringLength, 2):
        byteList.append('0x' + hexString[byte:byte+2])

def bleTrcbsCheckHexFormat(hexValue):
    if (not all(c in string.hexdigits for c in hexValue)):
        return False
    else:
        return True

def genSvcUuid(symbol, event):
    global trcbsServiceUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Service UUID if format is correct
    if (len(trcbsServiceUUID.getValue()) == 32 and bleTrcbsCheckHexFormat(trcbsServiceUUID.getValue())):
        formatHexString2HexBytes((trcbsServiceUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidServiceUUID")

    symbol.setValue(', '.join(svcUUID))

def genPsmUuid(symbol, event):
    global trcbsPsmTxUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate PSM UUID if format is correct
    if (len(trcbsPsmTxUUID.getValue()) == 32 and bleTrcbsCheckHexFormat(trcbsPsmTxUUID.getValue())):
        formatHexString2HexBytes((trcbsPsmTxUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidPsmUUID")

    symbol.setValue(', '.join(svcUUID))

def genCtrlUuid(symbol, event):
    global trcbsCtrlUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Ctrl UUID if format is correct
    if (len(trcbsCtrlUUID.getValue()) == 32 and bleTrcbsCheckHexFormat(trcbsCtrlUUID.getValue())):
        formatHexString2HexBytes((trcbsCtrlUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidCtrlUUID")

    symbol.setValue(', '.join(svcUUID))

def trcbsEnableConfigChange(symbol, event):

    symbol.setValue(event["value"])

def instantiateComponent(svcBLE_TRCBSComponent):
    global trcbsServiceUUID
    global trcbsPsmTxUUID
    global trcbsCtrlUUID

    print('Load Module: SERVICE_TRCBS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Service UUID
    trcbsServiceUUID = svcBLE_TRCBSComponent.createStringSymbol("TRCBS_SERVICE_UUID", None)
    trcbsServiceUUID.setLabel('Service UUID')
    trcbsServiceUUID.setDefaultValue("50ECED1AA0E8DBBDFC45202143535349")
    trcbsServiceUUID.setVisible(True)
    trcbsServiceUUID.setReadOnly(False)

    # Checked Service UUID
    checkedTrcbsServiceUUID = svcBLE_TRCBSComponent.createStringSymbol("CHECKED_TRCBS_SERVICE_UUID", None)
    checkedTrcbsServiceUUID.setLabel("Service UUID")
    checkedTrcbsServiceUUID.setDefaultValue("0x50,0xEC,0xED,0x1A,0xA0,0xE8,0xDB,0xBD,0xFC,0x45,0x20,0x21,0x43,0x53,0x53,0x49")
    checkedTrcbsServiceUUID.setVisible(False)
    checkedTrcbsServiceUUID.setDependencies(genSvcUuid, ["TRCBS_SERVICE_UUID"])

    # PSM UUID
    trcbsPsmTxUUID = svcBLE_TRCBSComponent.createStringSymbol("TRCBS_PSM_UUID", None)
    trcbsPsmTxUUID.setLabel('PSM UUID')
    trcbsPsmTxUUID.setDefaultValue("1FDD253BC1689F9A9149DBC243535349")
    trcbsPsmTxUUID.setVisible(True)
    trcbsPsmTxUUID.setReadOnly(False)

    # Checked PSM UUID
    checkedTrcbsPsmTxUUID = svcBLE_TRCBSComponent.createStringSymbol("CHECKED_TRCBS_PSM_UUID", None)
    checkedTrcbsPsmTxUUID.setLabel("PSM UUID")
    checkedTrcbsPsmTxUUID.setDefaultValue("0x1F,0xDD,0x25,0x3B,0xC1,0x68,0x9F,0x9A,0x91,0x49,0xDB,0xC2,0x43,0x53,0x53,0x49")
    checkedTrcbsPsmTxUUID.setVisible(False)
    checkedTrcbsPsmTxUUID.setDependencies(genPsmUuid, ["TRCBS_PSM_UUID"])

    # Ctrl UUID
    trcbsCtrlUUID = svcBLE_TRCBSComponent.createStringSymbol("TRCBS_CTRL_UUID", None)
    trcbsCtrlUUID.setLabel('CTRL UUID')
    trcbsCtrlUUID.setDefaultValue("3CD0F71AE935461EAE18840243535349")
    trcbsCtrlUUID.setVisible(True)
    trcbsCtrlUUID.setReadOnly(False)

    # Checked Ctrl UUID
    checkedTrcbsCtrlUUID = svcBLE_TRCBSComponent.createStringSymbol("CHECKED_TRCBS_CTRL_UUID", None)
    checkedTrcbsCtrlUUID.setLabel("CTRL UUID")
    checkedTrcbsCtrlUUID.setDefaultValue("0x3C,0xD0,0xF7,0x1A,0xE9,0x35,0x46,0x1E,0xAE,0x18,0x84,0x02,0x43,0x53,0x53,0x49")
    checkedTrcbsCtrlUUID.setVisible(False)
    checkedTrcbsCtrlUUID.setDependencies(genCtrlUuid, ["TRCBS_CTRL_UUID"])

    ##########################
    # Config Enable
    ##########################
    trcbsEnableConfig = svcBLE_TRCBSComponent.createBooleanSymbol('TRCBS_ENABLE_CONFIG', None)
    trcbsEnableConfig.setLabel('Enable Config')
    trcbsEnableConfig.setDefaultValue(False)
    trcbsEnableConfig.setVisible(False)
    trcbsEnableConfig.setDependencies(trcbsEnableConfigChange, ["BLE_STACK_LIB.BLE_ENABLE_OUTPUT_CONFIG"])
    if (Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG') != None):
        trcbsEnableConfig.setValue(Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG'))
    else:
        trcbsEnableConfig.setValue(False)

    # Add ble_trcbs.c file
    bleTrcbsSourceFile = svcBLE_TRCBSComponent.createFileSymbol(None, None)
    bleTrcbsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_trcbs/ble_trcbs.c')
    bleTrcbsSourceFile.setOutputName('ble_trcbs.c')
    bleTrcbsSourceFile.setOverwrite(True)
    bleTrcbsSourceFile.setDestPath('ble/service_ble/ble_trcbs')
    bleTrcbsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trcbs')
    bleTrcbsSourceFile.setType('SOURCE')
    bleTrcbsSourceFile.setEnabled(True)
    bleTrcbsSourceFile.setMarkup(True)

    # Add ble_trcbs.h file
    bleTrcbsHeaderFile = svcBLE_TRCBSComponent.createFileSymbol(None, None)
    bleTrcbsHeaderFile.setSourcePath('driver/ble/templates/service/ble_trcbs.h.ftl')
    bleTrcbsHeaderFile.setOutputName('ble_trcbs.h')
    bleTrcbsHeaderFile.setOverwrite(True)
    bleTrcbsHeaderFile.setDestPath('ble/service_ble/ble_trcbs')
    bleTrcbsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trcbs')
    bleTrcbsHeaderFile.setType('HEADER')
    bleTrcbsHeaderFile.setEnabled(True)
    bleTrcbsHeaderFile.setMarkup(True)

    # Add the configuration of TRCBS to configuration.h
    bleTrcbsHeaderConfigFile = svcBLE_TRCBSComponent.createFileSymbol(None, None)
    bleTrcbsHeaderConfigFile.setType('STRING')
    bleTrcbsHeaderConfigFile.setOutputName('core.LIST_SYSTEM_CONFIG_H_APPLICATION_CONFIGURATION')
    bleTrcbsHeaderConfigFile.setSourcePath('driver/ble/templates/service/ble_trcbs_config.h.ftl')
    bleTrcbsHeaderConfigFile.setMarkup(True)


def finalizeComponent(svcBLE_TRCBSComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(svcBLE_TRCBSComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

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
