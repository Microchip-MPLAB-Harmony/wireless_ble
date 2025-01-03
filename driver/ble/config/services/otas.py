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

def instantiateComponent(serviceOtasComponent):
    print('Load Module: SERVICE_OTAS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Add ble_otas.c file
    bleOtasSourceFile = serviceOtasComponent.createFileSymbol(None, None)
    bleOtasSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_otas/ble_otas.c')
    bleOtasSourceFile.setOutputName('ble_otas.c')
    bleOtasSourceFile.setOverwrite(True)
    bleOtasSourceFile.setDestPath('ble/service_ble/ble_otas')
    bleOtasSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_otas')
    bleOtasSourceFile.setType('SOURCE')
    bleOtasSourceFile.setEnabled(True)
    bleOtasSourceFile.setMarkup(True)

    # Add ble_otas.h file
    bleOtasHeaderFile = serviceOtasComponent.createFileSymbol(None, None)
    bleOtasHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_otas/ble_otas.h')
    bleOtasHeaderFile.setOutputName('ble_otas.h')
    bleOtasHeaderFile.setOverwrite(True)
    bleOtasHeaderFile.setDestPath('ble/service_ble/ble_otas')
    bleOtasHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_otas')
    bleOtasHeaderFile.setType('HEADER')
    bleOtasHeaderFile.setEnabled(True)
    bleOtasHeaderFile.setMarkup(True)


def finalizeComponent(serviceOtasComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceOtasComponent.getID()))
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
