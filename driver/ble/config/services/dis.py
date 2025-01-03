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

def bleDisCheckHexFormat(hexValue):
    if (not all(c in string.hexdigits for c in hexValue)):
        return False
    else:
        return True

def genSystemId(symbol, event):

    sSystemId = []    # array of formatted hex bytes

    # Only generate System Id if format is correct
    if ((len(event["value"]) != 0) and ((len(event["value"]) % 2) == 0) and bleDisCheckHexFormat(event["value"])):
        formatHexString2HexBytes(event["value"].upper(), sSystemId)
    else:
        sSystemId.append("#InvalidSystemId")

    symbol.setValue(', '.join(sSystemId))
    
def genIEEEData(symbol, event):

    sIEEEData = []    # array of formatted hex bytes

    # Only generate IEEE Data if format is correct
    if ((len(event["value"]) != 0) and ((len(event["value"]) % 2) == 0) and bleDisCheckHexFormat(event["value"])):
        formatHexString2HexBytes(event["value"].upper(), sIEEEData)
    else:
        sIEEEData.append("#InvalidIEEEData")

    symbol.setValue(', '.join(sIEEEData))
    
def genPnPId(symbol, event):

    sPnPId = []    # array of formatted hex bytes

    # Only generate PnP Id if format is correct
    if ((len(event["value"]) != 0) and ((len(event["value"]) % 2) == 0) and bleDisCheckHexFormat(event["value"])):
        formatHexString2HexBytes(event["value"].upper(), sPnPId)
    else:
        sPnPId.append("#InvalidPnPId")

    symbol.setValue(', '.join(sPnPId))

def disEnableConfigChange(symbol, event):

    symbol.setValue(event["value"])

def instantiateComponent(serviceDisComponent):
    print('Load Module: SERVICE_DIS')
    configName = Variables.get('__CONFIGURATION_NAME')

    ##########################
    # Manufacture Name Enable
    ##########################
    disManuEnable = serviceDisComponent.createBooleanSymbol('DIS_MANU_NAME_ENABLE', None)
    disManuEnable.setLabel('Manufacturer Name')
    disManuEnable.setDefaultValue(True)
    disManuEnable.setVisible(True)

    # Manufacture Name
    disServiceManufacture = serviceDisComponent.createStringSymbol("DIS_MANU_NAME_STRING", disManuEnable)
    disServiceManufacture.setLabel('Manufacture Name')
    disServiceManufacture.setDefaultValue("Manufacture Name")
    disServiceManufacture.setVisible(True)
    disServiceManufacture.setReadOnly(False)

    # Comment on Manufacture Name
    cmntDisServiceManufacture = serviceDisComponent.createCommentSymbol("CMT_DIS_MANU_NAME_STRING", disManuEnable)
    cmntDisServiceManufacture.setLabel("**** Manufacture Name must be string. ****")

    ##########################
    # Model Number Enable
    ##########################
    disModelEnable = serviceDisComponent.createBooleanSymbol('DIS_MODEL_NUM_ENABLE', None)
    disModelEnable.setLabel('Model Name')
    disModelEnable.setDefaultValue(True)
    disModelEnable.setVisible(True)

    # Model Number
    disServiceModelNum = serviceDisComponent.createStringSymbol("DIS_MODEL_NUM_STRING", disModelEnable)
    disServiceModelNum.setLabel('Model Number')
    disServiceModelNum.setDefaultValue("Model Number")
    disServiceModelNum.setVisible(True)
    disServiceModelNum.setReadOnly(False)

    # Comment on Model Number
    cmntDisServiceModelNum = serviceDisComponent.createCommentSymbol("CMT_DIS_MODEL_NUM_STRING", disModelEnable)
    cmntDisServiceModelNum.setLabel("**** Model Number must be string. ****")

    ##########################
    # Serial Number Enable
    ##########################
    disSerialEnable = serviceDisComponent.createBooleanSymbol('DIS_SERIAL_NUM_ENABLE', None)
    disSerialEnable.setLabel('Serial Name')
    disSerialEnable.setDefaultValue(False)
    disSerialEnable.setVisible(True)

    # Serial Number
    disServiceSerialNum = serviceDisComponent.createStringSymbol("DIS_SERIAL_NUM_STRING", disSerialEnable)
    disServiceSerialNum.setLabel('Serial Number')
    disServiceSerialNum.setDefaultValue("Serial Number")
    disServiceSerialNum.setVisible(True)
    disServiceSerialNum.setReadOnly(False)

    # Comment on Serial Number
    cmntDisServiceSerialNum = serviceDisComponent.createCommentSymbol("CMT_DIS_SERIAL_NUM_STRING", disSerialEnable)
    cmntDisServiceSerialNum.setLabel("**** Serial Number must be string. ****")

    ##########################
    # Hardware Revision Enable
    ##########################
    disHardwareEnable = serviceDisComponent.createBooleanSymbol('DIS_HW_REV_ENABLE', None)
    disHardwareEnable.setLabel('Hardware Revision')
    disHardwareEnable.setDefaultValue(False)
    disHardwareEnable.setVisible(True)

    # Hardware Revision
    disServiceHWRevision = serviceDisComponent.createStringSymbol("DIS_HW_REV_STRING", disHardwareEnable)
    disServiceHWRevision.setLabel('Hardware Revision')
    disServiceHWRevision.setDefaultValue("Hardware Revision")
    disServiceHWRevision.setVisible(True)
    disServiceHWRevision.setReadOnly(False)

    # Comment on Hardware Revision
    cmntDisServiceHWRevision = serviceDisComponent.createCommentSymbol("CMT_DIS_HW_REV_STRING", disHardwareEnable)
    cmntDisServiceHWRevision.setLabel("**** Hardware Revision must be string. ****")

    ##########################
    # Firmware Revision Enable
    ##########################
    disFirmwareEnable = serviceDisComponent.createBooleanSymbol('DIS_FW_REV_ENABLE', None)
    disFirmwareEnable.setLabel('Firmware Revision')
    disFirmwareEnable.setDefaultValue(False)
    disFirmwareEnable.setVisible(True)

    # Firmware Revision
    disServiceFirmware = serviceDisComponent.createStringSymbol("DIS_FW_REV_STRING", disFirmwareEnable)
    disServiceFirmware.setLabel('Firmware Revision')
    disServiceFirmware.setDefaultValue("Firmware")
    disServiceFirmware.setVisible(True)
    disServiceFirmware.setReadOnly(False)

    # Comment on Firmware Revision
    cmntDisServiceFirmware = serviceDisComponent.createCommentSymbol("CMT_DIS_FW_REV_STRING", disFirmwareEnable)
    cmntDisServiceFirmware.setLabel("**** Firmware Revision must be string. ****")

    ##########################
    # Software Revision Enable
    ##########################
    disSoftwareEnable = serviceDisComponent.createBooleanSymbol('DIS_SW_REV_ENABLE', None)
    disSoftwareEnable.setLabel('Software Revision')
    disSoftwareEnable.setDefaultValue(False)
    disSoftwareEnable.setVisible(True)

    # Software Revision
    disServiceSoftware = serviceDisComponent.createStringSymbol("DIS_SW_REV_STRING", disSoftwareEnable)
    disServiceSoftware.setLabel('Software Revision')
    disServiceSoftware.setDefaultValue("Software")
    disServiceSoftware.setVisible(True)
    disServiceSoftware.setReadOnly(False)

    # Comment on Software Revision
    cmntDisServiceSoftware = serviceDisComponent.createCommentSymbol("CMT_DIS_SW_REV_STRING", disSoftwareEnable)
    cmntDisServiceSoftware.setLabel("**** Software Revision must be string. ****")

    ##########################
    # System Id Enable
    ##########################
    disSysIdEnable = serviceDisComponent.createBooleanSymbol('DIS_SYSTEM_ID_ENABLE', None)
    disSysIdEnable.setLabel('System Id')
    disSysIdEnable.setDefaultValue(False)
    disSysIdEnable.setVisible(True)

    # System Id
    disServiceSystemId = serviceDisComponent.createStringSymbol("DIS_SYSTEM_ID_STRING", disSysIdEnable)
    disServiceSystemId.setLabel('System Id')
    disServiceSystemId.setDefaultValue("0000")
    disServiceSystemId.setVisible(True)
    disServiceSystemId.setReadOnly(False)

    # Checked System Id
    checkedDisServiceSystemId = serviceDisComponent.createStringSymbol("CHECKED_DIS_SYSTEM_ID_STRING", None)
    checkedDisServiceSystemId.setLabel('System Id')
    checkedDisServiceSystemId.setDefaultValue("0x00,0x00")
    checkedDisServiceSystemId.setVisible(False)
    checkedDisServiceSystemId.setDependencies(genSystemId, ["DIS_SYSTEM_ID_STRING"])

    # Comment on System Id
    cmntDisServiceSystemId = serviceDisComponent.createCommentSymbol("CMT_DIS_SYSTEM_ID_STRING", disSysIdEnable)
    cmntDisServiceSystemId.setLabel("**** System Id must be at least 1 byte and input in hexadecimal(LSB). ****")

    ##########################
    # IEEE 11073-20601 Enable
    ##########################
    disIeeeEnable = serviceDisComponent.createBooleanSymbol('DIS_IEEE_ENABLE', None)
    disIeeeEnable.setLabel('IEEE 11073-20601')
    disIeeeEnable.setDefaultValue(False)
    disIeeeEnable.setVisible(True)

    # IEEE 11073-20601 Regulatory Certification Data
    disServiceIeee = serviceDisComponent.createStringSymbol("DIS_IEEE_STRING", disIeeeEnable)
    disServiceIeee.setLabel('IEEE 11073-20601 Data')
    disServiceIeee.setDefaultValue("0000")
    disServiceIeee.setVisible(True)
    disServiceIeee.setReadOnly(False)

    # Checked IEEE 11073-20601 Regulatory Certification Data
    checkedDisServiceIeee = serviceDisComponent.createStringSymbol("CHECKED_DIS_IEEE_STRING", None)
    checkedDisServiceIeee.setLabel('IEEE 11073-20601 Data')
    checkedDisServiceIeee.setDefaultValue("0x00,0x00")
    checkedDisServiceIeee.setVisible(False)
    checkedDisServiceIeee.setDependencies(genIEEEData, ["DIS_IEEE_STRING"])

    # Comment on IEEE 11073-20601 Regulatory Certification Data
    cmntDisServiceIeee = serviceDisComponent.createCommentSymbol("CMT_DIS_IEEE_STRING", disIeeeEnable)
    cmntDisServiceIeee.setLabel("**** IEEE 11073-20601 Data must be at least 1 byte and input in hexadecimal(LSB). ****")

    ##########################
    # PnP ID Enable
    ##########################
    disPnpIdEnable = serviceDisComponent.createBooleanSymbol('DIS_PNP_ID_ENABLE', None)
    disPnpIdEnable.setLabel('PnP ID')
    disPnpIdEnable.setDefaultValue(False)
    disPnpIdEnable.setVisible(True)

    # PnP ID
    disServicePnpId = serviceDisComponent.createStringSymbol("DIS_PNP_ID_STRING", disPnpIdEnable)
    disServicePnpId.setLabel('PnP Id')
    disServicePnpId.setDefaultValue("0000")
    disServicePnpId.setVisible(True)
    disServicePnpId.setReadOnly(False)

    # Checked PnP ID
    checkedDisServicePnpId = serviceDisComponent.createStringSymbol("CHECKED_DIS_PNP_ID_STRING", None)
    checkedDisServicePnpId.setLabel('PnP Id')
    checkedDisServicePnpId.setDefaultValue("0x00,0x00")
    checkedDisServicePnpId.setVisible(False)
    checkedDisServicePnpId.setDependencies(genPnPId, ["DIS_PNP_ID_STRING"])

    # Comment on PnP ID
    cmntDisServicePnpId = serviceDisComponent.createCommentSymbol("CMT_DIS_PNP_ID_STRING", disPnpIdEnable)
    cmntDisServicePnpId.setLabel("**** PnP Id must be at least 1 byte and input in hexadecimal(LSB). ****")

    ##########################
    # Config Enable
    ##########################
    disEnableConfig = serviceDisComponent.createBooleanSymbol('DIS_ENABLE_CONFIG', None)
    disEnableConfig.setLabel('Enable Config')
    disEnableConfig.setDefaultValue(False)
    disEnableConfig.setVisible(False)
    disEnableConfig.setDependencies(disEnableConfigChange, ["BLE_STACK_LIB.BLE_ENABLE_OUTPUT_CONFIG"])
    if (Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG') != None):
        disEnableConfig.setValue(Database.getSymbolValue("BLE_STACK_LIB", 'BLE_ENABLE_OUTPUT_CONFIG'))
    else:
        disEnableConfig.setValue(False)

    # Add ble_dis.c file
    bleDisSourceFile = serviceDisComponent.createFileSymbol(None, None)
    bleDisSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_dis/ble_dis.c')
    bleDisSourceFile.setOutputName('ble_dis.c')
    bleDisSourceFile.setOverwrite(True)
    bleDisSourceFile.setDestPath('ble/service_ble/ble_dis')
    bleDisSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_dis')
    bleDisSourceFile.setType('SOURCE')
    bleDisSourceFile.setEnabled(True)
    bleDisSourceFile.setMarkup(True)


    # Add ble_dis.h file
    bleDisHeaderFile = serviceDisComponent.createFileSymbol(None, None)
    bleDisHeaderFile.setSourcePath('driver/ble/templates/service/ble_dis.h.ftl')
    bleDisHeaderFile.setOutputName('ble_dis.h')
    bleDisHeaderFile.setOverwrite(True)
    bleDisHeaderFile.setDestPath('ble/service_ble/ble_dis')
    bleDisHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_dis')
    bleDisHeaderFile.setType('HEADER')
    bleDisHeaderFile.setEnabled(True)
    bleDisHeaderFile.setMarkup(True)


    # Add the configuration of DIS to configuration.h
    bleDisHeaderConfigFile = serviceDisComponent.createFileSymbol(None, None)
    bleDisHeaderConfigFile.setType('STRING')
    bleDisHeaderConfigFile.setOutputName('core.LIST_SYSTEM_CONFIG_H_APPLICATION_CONFIGURATION')
    bleDisHeaderConfigFile.setSourcePath('driver/ble/templates/service/ble_dis_config.h.ftl')
    bleDisHeaderConfigFile.setMarkup(True)


def finalizeComponent(serviceDisComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceDisComponent.getID()))
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
