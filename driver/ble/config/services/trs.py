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

def bleTrsCheckHexFormat(hexValue):
    if (not all(c in string.hexdigits for c in hexValue)):
        return False
    else:
        return True

def genSvcUuid(symbol, event):
    global trsServiceUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Service UUID if format is correct
    if (len(trsServiceUUID.getValue()) == 32 and bleTrsCheckHexFormat(trsServiceUUID.getValue())):
        formatHexString2HexBytes((trsServiceUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidServiceUUID")

    symbol.setValue(', '.join(svcUUID))

def genTxCharUuid(symbol, event):
    global trsServiceTxUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Service Tx Characteristic UUID if format is correct
    if (len(trsServiceTxUUID.getValue()) == 32 and bleTrsCheckHexFormat(trsServiceTxUUID.getValue())):
        formatHexString2HexBytes((trsServiceTxUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidTxCharUUID")

    symbol.setValue(', '.join(svcUUID))

def genRxCharUuid(symbol, event):
    global trsServiceRxUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Service Rx Characteristic UUID if format is correct
    if (len(trsServiceRxUUID.getValue()) == 32 and bleTrsCheckHexFormat(trsServiceRxUUID.getValue())):
        formatHexString2HexBytes((trsServiceRxUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidRxCharUUID")

    symbol.setValue(', '.join(svcUUID))

def genCpCharUuid(symbol, event):
    global trsServiceCpUUID

    svcUUID = []    # array of formatted hex bytes

    # Only genetate Service Cp Characteristic UUID if format is correct
    if (len(trsServiceCpUUID.getValue()) == 32 and bleTrsCheckHexFormat(trsServiceCpUUID.getValue())):
        formatHexString2HexBytes((trsServiceCpUUID.getValue()).upper(), svcUUID)
    else:
        svcUUID.append("#InvalidCpCharUUID")

    symbol.setValue(', '.join(svcUUID))

def trsEnableConfigChange(symbol, event):

    symbol.setValue(event["value"])

def instantiateComponent(serviceTrsComponent):
    global trsServiceUUID
    global trsServiceTxUUID
    global trsServiceRxUUID
    global trsServiceCpUUID

    print('Load Module: SERVICE_TRS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Service UUID
    trsServiceUUID = serviceTrsComponent.createStringSymbol("TRS_SERVICE_UUID", None)
    trsServiceUUID.setLabel('Service UUID')
    trsServiceUUID.setDefaultValue("55E405D2AF9FA98FE54A7DFE43535349")
    trsServiceUUID.setVisible(True)
    trsServiceUUID.setReadOnly(False)

    # Checked Service UUID
    checkedTrsServiceUUID = serviceTrsComponent.createStringSymbol("CHECKED_TRS_SERVICE_UUID", None)
    checkedTrsServiceUUID.setLabel("Service UUID")
    checkedTrsServiceUUID.setDefaultValue("0x55,0xE4,0x05,0xD2,0xAF,0x9F,0xA9,0x8F,0xE5,0x4A,0x7D,0xFE,0x43,0x53,0x53,0x49")
    checkedTrsServiceUUID.setVisible(False)
    checkedTrsServiceUUID.setDependencies(genSvcUuid, ["TRS_SERVICE_UUID"])

    # TX Char UUID
    trsServiceTxUUID = serviceTrsComponent.createStringSymbol("TRS_TX_UUID", None)
    trsServiceTxUUID.setLabel('TX Characteristic UUID')
    trsServiceTxUUID.setDefaultValue("16962447C62361BAD94B4D1E43535349")
    trsServiceTxUUID.setVisible(True)
    trsServiceTxUUID.setReadOnly(False)

    # Checked Tx Char UUID
    checkedTrsServiceTxUUID = serviceTrsComponent.createStringSymbol("CHECKED_TRS_TX_UUID", None)
    checkedTrsServiceTxUUID.setLabel("TX Characteristic UUID")
    checkedTrsServiceTxUUID.setDefaultValue("0x16,0x96,0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49")
    checkedTrsServiceTxUUID.setVisible(False)
    checkedTrsServiceTxUUID.setDependencies(genTxCharUuid, ["TRS_TX_UUID"])

    # RX Char UUID
    trsServiceRxUUID = serviceTrsComponent.createStringSymbol("TRS_RX_UUID", None)
    trsServiceRxUUID.setLabel('RX Characteristic UUID')
    trsServiceRxUUID.setDefaultValue("B39B7234BEECD4A8F443418843535349")
    trsServiceRxUUID.setVisible(True)
    trsServiceRxUUID.setReadOnly(False)

    # Checked Rx Char UUID
    checkedTrsServiceRxUUID = serviceTrsComponent.createStringSymbol("CHECKED_TRS_RX_UUID", None)
    checkedTrsServiceRxUUID.setLabel("RX Characteristic UUID")
    checkedTrsServiceRxUUID.setDefaultValue("0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49")
    checkedTrsServiceRxUUID.setVisible(False)
    checkedTrsServiceRxUUID.setDependencies(genRxCharUuid, ["TRS_RX_UUID"])

    # CP Char UUID
    trsServiceCpUUID = serviceTrsComponent.createStringSymbol("TRS_CP_UUID", None)
    trsServiceCpUUID.setLabel('CP Characteristic UUID')
    trsServiceCpUUID.setDefaultValue("7E3B07FF1C51492FB3398A4C43535349")
    trsServiceCpUUID.setVisible(True)
    trsServiceCpUUID.setReadOnly(False)

    # Checked CP Char UUID
    checkedTrsServiceCpUUID = serviceTrsComponent.createStringSymbol("CHECKED_TRS_CP_UUID", None)
    checkedTrsServiceCpUUID.setLabel("CP Characteristic UUID")
    checkedTrsServiceCpUUID.setDefaultValue("0x7E,0x3B,0x07,0xFF,0x1C,0x51,0x49,0x2F,0xB3,0x39,0x8A,0x4C,0x43,0x53,0x53,0x49")
    checkedTrsServiceCpUUID.setVisible(False)
    checkedTrsServiceCpUUID.setDependencies(genCpCharUuid, ["TRS_CP_UUID"])

    ##########################
    # Config Enable
    ##########################
    trsEnableConfig = serviceTrsComponent.createBooleanSymbol('TRS_ENABLE_CONFIG', None)
    trsEnableConfig.setLabel('Enable Config')
    trsEnableConfig.setDefaultValue(False)
    trsEnableConfig.setVisible(False)
    trsEnableConfig.setDependencies(trsEnableConfigChange, ["BLE_STACK_LIB.BLE_ENABLE_OUTPUT_CONFIG"])
    if (Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG') != None):
        trsEnableConfig.setValue(Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG'))
    else:
        trsEnableConfig.setValue(False)

    # Add ble_trs.c file
    bleTrsSourceFile = serviceTrsComponent.createFileSymbol(None, None)
    bleTrsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_trs/ble_trs.c')
    bleTrsSourceFile.setOutputName('ble_trs.c')
    bleTrsSourceFile.setOverwrite(True)
    bleTrsSourceFile.setDestPath('ble/service_ble/ble_trs')
    bleTrsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trs')
    bleTrsSourceFile.setType('SOURCE')
    bleTrsSourceFile.setEnabled(True)
    bleTrsSourceFile.setMarkup(True)

    # Add ble_trs.h file
    bleTrsHeaderFile = serviceTrsComponent.createFileSymbol(None, None)
    bleTrsHeaderFile.setSourcePath('driver/ble/templates/service/ble_trs.h.ftl')
    bleTrsHeaderFile.setOutputName('ble_trs.h')
    bleTrsHeaderFile.setOverwrite(True)
    bleTrsHeaderFile.setDestPath('ble/service_ble/ble_trs')
    bleTrsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_trs')
    bleTrsHeaderFile.setType('HEADER')
    bleTrsHeaderFile.setEnabled(True)
    bleTrsHeaderFile.setMarkup(True)

    # Add the configuration of TRS to configuration.h
    bleTrsHeaderConfigFile = serviceTrsComponent.createFileSymbol(None, None)
    bleTrsHeaderConfigFile.setType('STRING')
    bleTrsHeaderConfigFile.setOutputName('core.LIST_SYSTEM_CONFIG_H_APPLICATION_CONFIGURATION')
    bleTrsHeaderConfigFile.setSourcePath('driver/ble/templates/service/ble_trs_config.h.ftl')
    bleTrsHeaderConfigFile.setMarkup(True)


def finalizeComponent(serviceTrsComponent):
    #Log.writeInfoMessage('Finalizing: {}'.format(serviceTrsComponent.getID()))
    print('Finalizing: {}'.format(serviceTrsComponent.getID()))
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
