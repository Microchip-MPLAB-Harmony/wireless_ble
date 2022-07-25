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

def instantiateComponent(serviceLlsComponent):
    print('Load Module: SERVICE_LLS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

    # Add ble_lls.c file
    bleLlsSourceFile = serviceLlsComponent.createFileSymbol(None, None)
    bleLlsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_lls/ble_lls.c')
    bleLlsSourceFile.setOutputName('ble_lls.c')
    bleLlsSourceFile.setOverwrite(True)
    bleLlsSourceFile.setDestPath('ble/service_ble/ble_lls')
    bleLlsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_lls')
    bleLlsSourceFile.setType('SOURCE')
    bleLlsSourceFile.setEnabled(True)
    bleLlsSourceFile.setMarkup(True)

    # Add ble_lls.h file
    bleLlsHeaderFile = serviceLlsComponent.createFileSymbol(None, None)
    bleLlsHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_lls/ble_lls.h')
    bleLlsHeaderFile.setOutputName('ble_lls.h')
    bleLlsHeaderFile.setOverwrite(True)
    bleLlsHeaderFile.setDestPath('ble/service_ble/ble_lls')
    bleLlsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_lls')
    bleLlsHeaderFile.setType('HEADER')
    bleLlsHeaderFile.setEnabled(True)
    bleLlsHeaderFile.setMarkup(True)


def finalizeComponent(serviceLlsComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceLlsComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
