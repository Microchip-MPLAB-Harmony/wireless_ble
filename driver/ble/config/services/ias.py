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

def instantiateComponent(serviceIasComponent):
    print('Load Module: SERVICE_IAS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

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
