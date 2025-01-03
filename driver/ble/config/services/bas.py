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

def instantiateComponent(serviceBasComponent):
    print('Load Module: SERVICE_BAS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Battery level with Notify
    basNotifyEnable = serviceBasComponent.createBooleanSymbol('BAS_NOTIFY_ENABLE', None)
    basNotifyEnable.setLabel('Enble Notify Property')
    basNotifyEnable.setDefaultValue(False)
    basNotifyEnable.setVisible(True)

    # Add ble_bas.c file
    bleBasSourceFile = serviceBasComponent.createFileSymbol(None, None)
    bleBasSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_bas/ble_bas.c')
    bleBasSourceFile.setOutputName('ble_bas.c')
    bleBasSourceFile.setOverwrite(True)
    bleBasSourceFile.setDestPath('ble/service_ble/ble_bas')
    bleBasSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_bas')
    bleBasSourceFile.setType('SOURCE')
    bleBasSourceFile.setEnabled(True)
    bleBasSourceFile.setMarkup(True)

    # Add ble_bas.h file
    bleBasHeaderFile = serviceBasComponent.createFileSymbol(None, None)
    bleBasHeaderFile.setSourcePath('driver/ble/templates/service/ble_bas.h.ftl')
    bleBasHeaderFile.setOutputName('ble_bas.h')
    bleBasHeaderFile.setOverwrite(True)
    bleBasHeaderFile.setDestPath('ble/service_ble/ble_bas')
    bleBasHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_bas')
    bleBasHeaderFile.setType('HEADER')
    bleBasHeaderFile.setEnabled(True)
    bleBasHeaderFile.setMarkup(True)


def finalizeComponent(serviceBasComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceBasComponent.getID()))
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
