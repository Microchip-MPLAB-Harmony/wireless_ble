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

def instantiateComponent(serviceAnsComponent):
    print('Load Module: SERVICE_ANS')
    configName = Variables.get('__CONFIGURATION_NAME')

    # Add ble_ans.c file
    bleAnsSourceFile = serviceAnsComponent.createFileSymbol(None, None)
    bleAnsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_ans/ble_ans.c')
    bleAnsSourceFile.setOutputName('ble_ans.c')
    bleAnsSourceFile.setOverwrite(True)
    bleAnsSourceFile.setDestPath('ble/service_ble/ble_ans')
    bleAnsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_ans')
    bleAnsSourceFile.setType('SOURCE')
    bleAnsSourceFile.setEnabled(True)
    bleAnsSourceFile.setMarkup(True)

    # Add ble_ans.h file
    bleAnsHeaderFile = serviceAnsComponent.createFileSymbol(None, None)
    bleAnsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_ans/ble_ans.h')
    bleAnsHeaderFile.setOutputName('ble_ans.h')
    bleAnsHeaderFile.setOverwrite(True)
    bleAnsHeaderFile.setDestPath('ble/service_ble/ble_ans')
    bleAnsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_ans')
    bleAnsHeaderFile.setType('HEADER')
    bleAnsHeaderFile.setEnabled(True)
    bleAnsHeaderFile.setMarkup(True)


def finalizeComponent(serviceAnsComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceAnsComponent.getID()))
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
