processor = 0
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

def instantiateComponent(serviceAnsComponent):
    print('Load Module: SERVICE_ANS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

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
