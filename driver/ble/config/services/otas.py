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

def instantiateComponent(serviceOtasComponent):
    print('Load Module: SERVICE_OTAS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

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
