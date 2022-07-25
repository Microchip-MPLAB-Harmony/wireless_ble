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

def instantiateComponent(serviceHidsComponent):
    print('Load Module: SERVICE_HIDS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))
    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

    #################################################################
    ##################      Supported Features Settings      ###############
    #################################################################

    # HID keyboard support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_KB_SUPPORT', None)
    pxpmIas.setLabel('Support HID keyboard')
    pxpmIas.setDescription('Support HID Keyboard')
    pxpmIas.setDefaultValue(False)
    pxpmIas.setVisible(True)

    # HID mouse support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_M_SUPPORT', None)
    pxpmIas.setLabel('Support HID mouse')
    pxpmIas.setDescription('Support HID mouse')
    pxpmIas.setDefaultValue(True)
    pxpmIas.setVisible(True)

    # HID boot mode support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_BOOTMODE_SUPPORT', None)
    pxpmIas.setLabel('Support HID boot mode')
    pxpmIas.setDescription('Support HID boot mode')
    pxpmIas.setDefaultValue(True)
    pxpmIas.setVisible(True)

    #################################################################
    ##################        Add Source File         ###############
    #################################################################

    # Add ble_hids.c file
    bleHidsSourceFile = serviceHidsComponent.createFileSymbol(None, None)
    bleHidsSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/service_ble/ble_hids/ble_hids.c')
    bleHidsSourceFile.setOutputName('ble_hids.c')
    bleHidsSourceFile.setOverwrite(True)
    bleHidsSourceFile.setDestPath('ble/service_ble/ble_hids')
    bleHidsSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_hids')
    bleHidsSourceFile.setType('SOURCE')
    bleHidsSourceFile.setEnabled(True)
    bleHidsSourceFile.setMarkup(True)

    # Add ble_hids.h file
    bleHidsHeaderFile = serviceHidsComponent.createFileSymbol(None, None)
    bleHidsHeaderFile.setSourcePath('driver/ble/templates/service/ble_hids.h.ftl')
    bleHidsHeaderFile.setOutputName('ble_hids.h')
    bleHidsHeaderFile.setOverwrite(True)
    bleHidsHeaderFile.setDestPath('ble/service_ble/ble_hids')
    bleHidsHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_hids')
    bleHidsHeaderFile.setType('HEADER')
    bleHidsHeaderFile.setEnabled(True)
    bleHidsHeaderFile.setMarkup(True)

def finalizeComponent(serviceHidsComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceHidsComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])