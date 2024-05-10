execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
else:
    print("Device not support")

def instantiateComponent(serviceTpsComponent):
    print('Load Module: SERVICE_TPS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Add ble_tps.c file
    bleTpsSourceFile = serviceTpsComponent.createFileSymbol(None, None)
    bleTpsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_tps/ble_tps.c')
    bleTpsSourceFile.setOutputName('ble_tps.c')
    bleTpsSourceFile.setOverwrite(True)
    bleTpsSourceFile.setDestPath('ble/service_ble/ble_tps')
    bleTpsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_tps')
    bleTpsSourceFile.setType('SOURCE')
    bleTpsSourceFile.setEnabled(True)
    bleTpsSourceFile.setMarkup(True)

    # Add ble_tps.h file
    bleTpsHeaderFile = serviceTpsComponent.createFileSymbol(None, None)
    bleTpsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_tps/ble_tps.h')
    bleTpsHeaderFile.setOutputName('ble_tps.h')
    bleTpsHeaderFile.setOverwrite(True)
    bleTpsHeaderFile.setDestPath('ble/service_ble/ble_tps')
    bleTpsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_tps')
    bleTpsHeaderFile.setType('HEADER')
    bleTpsHeaderFile.setEnabled(True)
    bleTpsHeaderFile.setMarkup(True)


def finalizeComponent(serviceTpsComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceTpsComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
