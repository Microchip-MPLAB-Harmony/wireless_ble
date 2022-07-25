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

def instantiateComponent(serviceBasComponent):
    print('Load Module: SERVICE_BAS')

    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

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
