execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
else:
    print("Device not support")

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
    disServiceSystemId.setDefaultValue("System Id")
    disServiceSystemId.setVisible(True)
    disServiceSystemId.setReadOnly(False)


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
    disServiceIeee.setDefaultValue("NULL")
    disServiceIeee.setVisible(True)
    disServiceIeee.setReadOnly(False)


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
    disServicePnpId.setDefaultValue("NULL")
    disServicePnpId.setVisible(True)
    disServicePnpId.setReadOnly(False) 

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


def finalizeComponent(serviceDisComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceDisComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
