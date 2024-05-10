execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
else:
    print("Device not support")

#Column
OPT_COL_KBD_SUP     =0
OPT_COL_MOUSE_SUP   =1
OPT_COL_BOOT_SUP    =2
#Row
OPT_ROW_MOSUE       =0
OPT_ROW_KBD         =1
OPT_ROW_MOSUE_KBD   =2
OPT_ROW_DEFAULT     =OPT_ROW_MOSUE
#Keyboard Support, Mouse Support, Boot Mode Support
list_support_options=((False, True, True),    #HID Device: Mouse
                      (True, False, True),    #HID Device: Keyboard
                      (True, True, True)      #HID Device: Mouse and Keyboard
                      )
#String
STR_KBD_SUPPORT     ="Keyboard Support:"
STR_MOUSE_SUPPORT   ="Mouse Support:"
STR_BOOT_SUPPORT    ="Boot Mode Support:"

def enableFlags(symbol, event):
    row_idx=event["value"]
    symbol.getComponent().getSymbolByID("HIDS_CMNT_SUPPORT_KEYBOARD").setLabel("{} {}".format(STR_KBD_SUPPORT, str(list_support_options[row_idx][OPT_COL_KBD_SUP])))
    symbol.getComponent().getSymbolByID("HIDS_CMNT_SUPPORT_MOUSE").setLabel("{} {}".format(STR_MOUSE_SUPPORT, str(list_support_options[row_idx][OPT_COL_MOUSE_SUP])))
    symbol.getComponent().getSymbolByID("HIDS_CMNT_SUPPORT_BOOT").setLabel("{} {}".format(STR_BOOT_SUPPORT, str(list_support_options[row_idx][OPT_COL_BOOT_SUP])))
    
    symbol.getComponent().getSymbolByID("HIDS_KB_SUPPORT").setValue(list_support_options[row_idx][OPT_COL_KBD_SUP])
    symbol.getComponent().getSymbolByID("HIDS_M_SUPPORT").setValue(list_support_options[row_idx][OPT_COL_MOUSE_SUP])
    symbol.getComponent().getSymbolByID("HIDS_BOOTMODE_SUPPORT").setValue(list_support_options[row_idx][OPT_COL_BOOT_SUP])
    
def instantiateComponent(serviceHidsComponent):
    print('Load Module: SERVICE_HIDS')
    configName = Variables.get('__CONFIGURATION_NAME')

    #################################################################
    ##################      Supported Features Settings      ###############
    #################################################################
    # List of supporting device type
    kvsDevType = serviceHidsComponent.createKeyValueSetSymbol('HIDS_DEVICE_TYPE', None)
    kvsDevType.setLabel('HIDS device:')
    kvsDevType.addKey('BLE_HIDS_DEVICE_MOUSE', '0', 'Mouse')
    kvsDevType.addKey('BLE_HIDS_DEVICE_KBD', '1', 'Keyboard')
    kvsDevType.addKey('BLE_HIDS_DEVICE_MOUSE_KBD', '2', 'Mouse and Keyboard')
    kvsDevType.setDefaultValue(OPT_ROW_DEFAULT)
    kvsDevType.setOutputMode('Key')
    kvsDevType.setDisplayMode('Description')
    kvsDevType.setDependencies(enableFlags, ["HIDS_DEVICE_TYPE"])
    
    # Comment of supporting Keyboard
    cmntKbd = serviceHidsComponent.createCommentSymbol("HIDS_CMNT_SUPPORT_KEYBOARD", kvsDevType)
    cmntKbd.setLabel("{} {}".format(STR_KBD_SUPPORT, str(list_support_options[OPT_ROW_DEFAULT][OPT_COL_KBD_SUP])))
    
    # Comment of supporting Mouse
    cmntMouse = serviceHidsComponent.createCommentSymbol("HIDS_CMNT_SUPPORT_MOUSE", kvsDevType)
    cmntMouse.setLabel("{} {}".format(STR_MOUSE_SUPPORT, str(list_support_options[OPT_ROW_DEFAULT][OPT_COL_MOUSE_SUP])))
    
    # Comment of supporting Boot Mode
    cmntBoot = serviceHidsComponent.createCommentSymbol("HIDS_CMNT_SUPPORT_BOOT", kvsDevType)
    cmntBoot.setLabel("{} {}".format(STR_BOOT_SUPPORT, str(list_support_options[OPT_ROW_DEFAULT][OPT_COL_BOOT_SUP])))  

    # HID keyboard support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_KB_SUPPORT', None)
    pxpmIas.setLabel('Support HID keyboard')
    pxpmIas.setDescription('Support HID Keyboard')
    pxpmIas.setDefaultValue(list_support_options[OPT_ROW_DEFAULT][OPT_COL_KBD_SUP])
    pxpmIas.setVisible(False)

    # HID mouse support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_M_SUPPORT', None)
    pxpmIas.setLabel('Support HID mouse')
    pxpmIas.setDescription('Support HID mouse')
    pxpmIas.setDefaultValue(list_support_options[OPT_ROW_DEFAULT][OPT_COL_MOUSE_SUP])
    pxpmIas.setVisible(False)

    # HID boot mode support
    pxpmIas = serviceHidsComponent.createBooleanSymbol('HIDS_BOOTMODE_SUPPORT', None)
    pxpmIas.setLabel('Support HID boot mode')
    pxpmIas.setDescription('Support HID boot mode')
    pxpmIas.setDefaultValue(list_support_options[OPT_ROW_DEFAULT][OPT_COL_BOOT_SUP])
    pxpmIas.setVisible(False)

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