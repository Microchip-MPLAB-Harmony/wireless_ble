execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
else:
    print("Device not support")

def instantiateComponent(serviceIasComponent):
    print('Load Module: SERVICE_IAS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Add ble_ias.c file
    bleIasSourceFile = serviceIasComponent.createFileSymbol(None, None)
    bleIasSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_ias/ble_ias.c')
    bleIasSourceFile.setOutputName('ble_ias.c')
    bleIasSourceFile.setOverwrite(True)
    bleIasSourceFile.setDestPath('ble/service_ble/ble_ias')
    bleIasSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_ias')
    bleIasSourceFile.setType('SOURCE')
    bleIasSourceFile.setEnabled(True)
    bleIasSourceFile.setMarkup(True)

    # Add ble_ias.h file
    bleIasHeaderFile = serviceIasComponent.createFileSymbol(None, None)
    bleIasHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_ias/ble_ias.h')
    bleIasHeaderFile.setOutputName('ble_ias.h')
    bleIasHeaderFile.setOverwrite(True)
    bleIasHeaderFile.setDestPath('ble/service_ble/ble_ias')
    bleIasHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_ias')
    bleIasHeaderFile.setType('HEADER')
    bleIasHeaderFile.setEnabled(True)
    bleIasHeaderFile.setMarkup(True)


def finalizeComponent(serviceIasComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceIasComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
