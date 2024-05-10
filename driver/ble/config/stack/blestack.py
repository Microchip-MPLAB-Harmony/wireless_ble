###############################################################################
######################### BLE Stack Configurations ############################
###############################################################################

from binascii import hexlify
import string


antennaGain = 3
txPwrMaxNonFHSS = 15
txPwrMaxFHSS = 15
txPwrMin = -26

execfile(Module.getPath() + '/driver/ble/config/dev_info.py')
processor = Variables.get("__PROCESSOR")
devFamily = GetDeviceFamily()
# print('Config Name: {}, device family: {}'.format(configName, devFamily))
if devFamily == "pic32cx_bz2_family":
    srcPath = "ble_src_bz2"
    devSupp = 'pic32cx_bz2_devsupport'
    libObj = "ble_stack_bz2_lib.a"
    if processor == 'WBZ450' or processor == 'PIC32CX1012BZ24032':
        devId = '450'
        conMinTxPwr = -25
    else:
        if (processor == 'WBZ451H'):
            devId = '451H'
            conMinTxPwr = -26
        else:
            devId = '451'
            conMinTxPwr = -29
elif devFamily == "pic32cx_bz3_family":
    srcPath = "ble_src_bz3"
    devSupp = 'pic32cx_bz3_devsupport'
    libObj = "ble_stack_bz3_lib.a"
    if processor == 'WBZ350' or processor == 'PIC32CX5109BZ31032':
        devId = '350'
    else:
        devId = '351'
    conMinTxPwr = -24
else:
    srcPath = ""
    devSupp = ''
    libObj = ""
    devId = ''
    conMinTxPwr = -29
    print("Device not support")


################################################################################
#### Component ####
################################################################################

def bleConfigEnable(symbol, event):
    if event["value"] == False:
        symbol.setEnabled(False)
    else:
        symbol.setEnabled(True)

def bleConfigDsadvEnable(symbol, event):
    global gapExtAdvEn
    global gapAdvertising
    global gapDsadvEn
    global bleCtrlOnly

    if bleCtrlOnly.getValue() == False:
        if gapExtAdvEn.getValue()==False and gapAdvertising.getValue()==True and gapDsadvEn.getValue()==True:
            symbol.setEnabled(True)
        else:
            symbol.setEnabled(False)
    else:
        symbol.setEnabled(False)


def ctrlOnlyFileChange(symbol, event):
    value = event["value"]
    if value == True:
        symbol.setEnabled(False)
    else:
        symbol.setEnabled(True)


def ddFileChange(symbol, event):
    if event["id"] == "BLE_BOOL_GATT_CLIENT":
        value = event["value"]
    else: #BLE_SYS_CTRL_ONLY_EN
        if event["value"] == False:
            value = event['source'].getSymbolValue('BLE_BOOL_GATT_CLIENT')
        else:
            value = False

    symbol.setEnabled(value)

def scmFileChange(symbol, event):
    if event["id"] == "BLE_SYS_CTRL_ONLY_EN":
        if event["value"] == False:
            value = event['source'].getSymbolValue('BLE_BOOL_GATT_CLIENT') and event['source'].getSymbolValue('BOOL_GCM_SCM')
        else:
            value = False
    elif event["value"] == False:
        value = False
    else:
        if event["id"] == "BLE_BOOL_GATT_CLIENT":
            value = event['source'].getSymbolValue('BOOL_GCM_SCM')
        else:
            value = event['source'].getSymbolValue('BLE_BOOL_GATT_CLIENT')

    symbol.setEnabled(value)


def dfuFileChange(symbol, event):
    if event["id"] == "BOOL_BLE_UTIL_DFU":
        value = event["value"]
    else: #BLE_SYS_CTRL_ONLY_EN
        if event["value"] == False:
            value = event['source'].getSymbolValue('BOOL_BLE_UTIL_DFU')
        else:
            value = False

    symbol.setEnabled(value)

def logFileChange(symbol, event):
    if event["id"] == "BLE_VIRTUAL_SNIFFER_EN":
        value = event["value"]
    else: #BLE_SYS_CTRL_ONLY_EN
        if event["value"] == False:
            value = event['source'].getSymbolValue('BLE_VIRTUAL_SNIFFER_EN')
        else:
            value = False

    symbol.setEnabled(value)

global sendSleepEnableMessage
def sendSleepEnableMessage(sleepEnable):
    Database.sendMessage(devSupp, "SLEEP_ENABLE", {"target": devSupp,
                                                    "source": "BLE_STACK_LIB","isEnabled":sleepEnable})

def handleMessage(messageID, args):
    # Log.writeInfoMessage('BLEStack:handleMessage ID={} argLen={}'.format(messageID, len(args)))
    ''' This message handler is designed to process messages
        sent from the driver/pic32cx-bz/config/device_support.py
        script.
    '''
    if(messageID == 'ANTENNA_GAIN_CHANGE'):
        global antennaGain
        global txPwrMaxNonFHSS
        global txPwrMaxFHSS
        global txPwrMin
        
        component = Database.getComponentByID(args['target'])
        Log.writeInfoMessage('{:<17}: Handling - target={}'.format('blestack.py', args['target']))

        devSuppComponent = Database.getComponentByID(devSupp)
        for arg in args:
            Log.writeInfoMessage('{:<17}: {}: {}'.format('', arg, args[arg]))
            if ('TX_PWR_MAX_NON_FHSS' == arg):
                txPwrMaxNonFHSS = args[arg]
            elif ('TX_PWR_MAX_FHSS' == arg):
                txPwrMaxFHSS = args[arg]
            elif ('CUSTOM_ANT_GAIN' == arg):
                antennaGain = args[arg]
        
        component.setSymbolValue('BLE_ANTENNA_GAIN', antennaGain)
        txPwrMin = conMinTxPwr + antennaGain

        # update tx power symbols
        gapConfigTxPwr(component, txPwrMaxNonFHSS, txPwrMaxFHSS, txPwrMin)


def instantiateComponent(libBLEStackComponent):
    global antennaGain

    print('libBLEStackComponent')
    configName = Variables.get('__CONFIGURATION_NAME')

    print('HarmonyCore.ENABLE_APP_FILE = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_APP_FILE'))))
    print('HarmonyCore.ENABLE_OSAL = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_OSAL'))))
    print('FreeRTOS.FREERTOS_USE_QUEUE_SETS = {}'.format(str(Database.getSymbolValue("FreeRTOS", 'FREERTOS_USE_QUEUE_SETS'))))

    print('Config Name: {} processor: {} devId: {}'.format(configName, processor, devId))

    # Inform H3 core that BLE has been loaded
    Database.setSymbolValue("core", "BLE_CLOCK_ENABLE", True)

    # Inform pdsSystem that BLE has been loaded
    if ('pdsSystem' in Database.getActiveComponentIDs()):
        if (Database.getSymbolValue("pdsSystem", "BLESTACK_LOADED") == False):
            Database.setSymbolValue("pdsSystem", "BLESTACK_LOADED", True)

  
    ############################################################################
    ### Execute additional H3 Python code ###
    ############################################################################
    #Load ble stack header files when adding ble_stack_lib.a
    execfile(Module.getPath() + "/driver/ble/config/stack/load_lib.py")


    ############################################################################
    ### Add ble_stack library files ###
    ############################################################################

    # Add ble_stack_lib.a library
    ble_stack_lib_a = libBLEStackComponent.createLibrarySymbol(None, None)
    ble_stack_lib_a.setDestPath('ble/lib')
    ble_stack_lib_a.setSourcePath('driver/ble/src/' + srcPath +'/'+libObj)
    ble_stack_lib_a.setOutputName(libObj)

    ############################################################################
    ### System Initialization Settings ###
    ############################################################################
    bleInitTemplates = [('blesysInitData', 'system_initialize_data.c.ftl', 'core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA'),
        ('blesysInitstart', 'system_initialize_start.c.ftl', 'core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_START'),
        ('blesysInitMiddleware', 'system_initialize_middleware.c.ftl', 'core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE'),
        ('blesysTaskMiddlewareDef', 'system_tasks_def.c.ftl', 'core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS'),
        ('blesysTaskMiddleware', 'system_tasks.c.ftl', 'core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS'),
        ('blesysDefZB', 'system_definitions.h.ftl', 'core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES'),
        ]

    n = 0
    bleInit = []
    for i, f, p in bleInitTemplates:
        bleInit.append( libBLEStackComponent.createFileSymbol(i, None))
        bleInit[n].setType('STRING')
        bleInit[n].setOutputName(p)
        bleInit[n].setSourcePath('driver/ble/templates/system/' + f)
        bleInit[n].setMarkup(True)
        print('{} file: {} pos: {}'.format(n, f, p))
        n = n + 1


    # Add BLE codes to app.c
    dsBleInitTemplates = [('dsBleInclude', 'ds_ble_include.c.ftl', 'LIST_DS_BLE_INCLUDE_C'),
        ('dsBleData', 'ds_ble_data.c.ftl', 'LIST_DS_BLE_DATA_C'),
        ('dsBleInit', 'ds_ble_init.c.ftl', 'LIST_DS_BLE_INIT_C'),
        ('dsBleTaskEntry', 'ds_ble_task_entry.c.ftl', 'LIST_DS_BLE_TASK_ENTRY_C'),
        ('dsBleMsgId', 'ds_ble_msg_id.h.ftl', 'LIST_DS_BLE_MSG_ID_H'),
        ]


    n = 0
    appBleInit = []
    for i, f, p in dsBleInitTemplates:
        appBleInit.append( libBLEStackComponent.createFileSymbol(i, None))
        appBleInit[n].setType('STRING')
        appBleInit[n].setOutputName(devSupp + '.' + p)
        appBleInit[n].setSourcePath('driver/ble/templates/ds_app/' + f)
        appBleInit[n].setMarkup(True)
        print('{} file: {} pos: {}'.format(n, f, p))
        n = n + 1

    ############################################################################
    ### Symbol Settings ###
    ############################################################################

    # BLE System Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/stack_sys.py")

    # GAP Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/stack_gap.py")

    # GATT Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/stack_gatt.py")

    # SMP Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/stack_smp.py")

    # L2CAP Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/stack_l2cap.py")

    # DM Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/mw_dm.py")

    # GCM Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/mw_gcm.py")

    # Utility Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/mw_utility.py")

    # Profile Setting
    execfile(Module.getPath() + "/driver/ble/config/stack/config_profile.py")


    ############################################################################
    ### Add Source/Header Files ###
    ############################################################################

    # Add app generated  files
    bleAppFile = [('app_ble.c.ftl', 'app_ble.c', 'SOURCE'),
                  ('app_ble.h.ftl', 'app_ble.h', 'HEADER'),
                  ('app_ble_handler.c.ftl', 'app_ble_handler.c', 'SOURCE'),
                  ('app_ble_handler.h.ftl', 'app_ble_handler.h', 'HEADER'),
                  ('app_ble_dsadv.c.ftl', 'app_ble_dsadv.c', 'SOURCE'),
                  ]


    for name, outputName, type in bleAppFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/templates/app/'+ name)
        fsymbol.setOutputName(outputName)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('../../app_ble')
        fsymbol.setProjectPath('app_ble')
        fsymbol.setType(type)
        fsymbol.setEnabled(True)
        fsymbol.setMarkup(True)
        if name == 'app_ble_dsadv.c.ftl':
            fsymbol.setDependencies(bleConfigDsadvEnable, ["GAP_DSADV_EN","BOOL_GAP_EXT_ADV","GAP_ADVERTISING","BLE_SYS_CTRL_ONLY_EN"])
            fsymbol.setEnabled(False)
        else:
            fsymbol.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])


    # Add controller only static files
    bleAppFile = [('app_ble_hci.h', 'HEADER'),
                  ('app_ble_hci.c', 'SOURCE'),
                  ]

    for name, type in bleAppFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/ble_app/'+ name)
        fsymbol.setOutputName(name)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('../../app_ble')
        fsymbol.setProjectPath('app_ble')
        fsymbol.setType(type)
        fsymbol.setEnabled(False)
        fsymbol.setDependencies(bleConfigEnable, ["BLE_SYS_CTRL_ONLY_EN"])
        print('App staic {} file: {} '.format(n, name))


    # Add pta static files
    bleAppFile = [('app_pta_handler.h', 'HEADER'),
                  ('app_pta_handler.c', 'SOURCE'),
                  ]

    for name, type in bleAppFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/ble_app/'+ name)
        fsymbol.setOutputName(name)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('../../app_ble')
        fsymbol.setProjectPath('app_ble')
        fsymbol.setType(type)
        fsymbol.setEnabled(False)
        fsymbol.setDependencies(bleConfigEnable, ["BLE_SYS_PTA_EN"])


    # Add ble app static files
    bleAppFile = [('app_ble_log_handler.h', 'HEADER'), 
                  ('app_ble_utility.h', 'HEADER'),
                  ('app_ble_dsadv.h', 'HEADER'),
                  ('app_ble_log_handler.c', 'SOURCE'),
                  ('app_ble_utility.c', 'SOURCE'),
                  ]

    for name, type in bleAppFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/ble_app/'+ name)
        fsymbol.setOutputName(name)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('../../app_ble')
        fsymbol.setProjectPath('app_ble')
        fsymbol.setType(type)
        if name.find('app_ble_log_handler') != -1:
            fsymbol.setEnabled(False)
            fsymbol.setDependencies(logFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BLE_VIRTUAL_SNIFFER_EN"])
        else:
            fsymbol.setEnabled(True)
            fsymbol.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])


    # Add mandatory middleware static files
    bleMiddlewareFile = [('mw_assert.h', 'ble_util', 'HEADER'), 
                         ('byte_stream.h', 'ble_util', 'HEADER'),
                         ('mw_aes.h', 'ble_util', 'HEADER'),
                         ('ble_dm.h', 'ble_dm', 'HEADER'),
                         ('ble_dm_conn.h', 'ble_dm', 'HEADER'),
                         ('ble_dm_sm.h', 'ble_dm', 'HEADER'),
                         ('ble_dm_info.h', 'ble_dm', 'HEADER'),
                         ('ble_dm_dds.h', 'ble_dm', 'HEADER'),
                         ('ble_dm_internal.h', 'ble_dm', 'HEADER'),
                         ('ble_dd.h', 'ble_gcm', 'HEADER'),
                         ('ble_scm.h', 'ble_gcm', 'HEADER'),
                         ('ble_log.h', 'ble_log', 'HEADER'),
                         ('mw_dfu.c', 'ble_util', 'SOURCE'),
                         ('mw_aes.c', 'ble_util', 'SOURCE'),
                         ('ble_dm.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_conn.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_sm.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_info.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_dds.c', 'ble_dm', 'SOURCE'),
                         ('ble_dd.c', 'ble_gcm', 'SOURCE'),
                         ('ble_scm.c', 'ble_gcm', 'SOURCE'),
                         ('ble_log.c', 'ble_log', 'SOURCE'),
                         ]

    for name, path, type in bleMiddlewareFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/' + srcPath + '/middleware_ble/'+ path +'/'+ name)
        fsymbol.setOutputName(name)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('ble/middleware_ble/' + path)
        fsymbol.setProjectPath('config/' + configName + '/ble/middleware_ble/' + path)
        fsymbol.setType(type)
        if name.find('ble_dd') != -1:
            fsymbol.setEnabled(False)
            fsymbol.setDependencies(ddFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BLE_BOOL_GATT_CLIENT"])
        elif name.find('ble_scm') != -1:
            fsymbol.setEnabled(False)
            fsymbol.setDependencies(scmFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BLE_BOOL_GATT_CLIENT", "BOOL_GCM_SCM"])
        elif name.find('mw_dfu') != -1:
            fsymbol.setEnabled(False)
            fsymbol.setDependencies(dfuFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BOOL_BLE_UTIL_DFU"])
        elif name.find('ble_log') != -1:
            fsymbol.setEnabled(False)
            fsymbol.setDependencies(logFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BLE_VIRTUAL_SNIFFER_EN"])
        else:
            fsymbol.setEnabled(True)
            fsymbol.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])


    # Add mw_dfu.h file - generated file
    bleMwDfuHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleMwDfuHeaderFile.setSourcePath('driver/ble/templates/middleware/' + srcPath + '/mw_dfu.h.ftl')
    bleMwDfuHeaderFile.setOutputName('mw_dfu.h')
    bleMwDfuHeaderFile.setOverwrite(True)
    bleMwDfuHeaderFile.setDestPath('ble/middleware_ble/ble_util/')
    bleMwDfuHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util/')
    bleMwDfuHeaderFile.setType('HEADER')
    bleMwDfuHeaderFile.setEnabled(False)
    bleMwDfuHeaderFile.setMarkup(True)
    bleMwDfuHeaderFile.setDependencies(dfuFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BOOL_BLE_UTIL_DFU"])


    # Add hpa static files
    if( processor == 'WBZ451H'):
        bleAppFile = [('app_ble_hpa_handler.h', 'HEADER'),
                      ('app_ble_hpa_handler.c', 'SOURCE'),
                      ]

        for name, type in bleAppFile:
            fsymbol = libBLEStackComponent.createFileSymbol(None, None)
            fsymbol.setSourcePath('driver/ble/src/ble_app/'+ name)
            fsymbol.setOutputName(name)
            fsymbol.setOverwrite(True)
            fsymbol.setDestPath('../../app_ble')
            fsymbol.setProjectPath('app_ble')
            fsymbol.setType(type)
            fsymbol.setEnabled(True)
    


    ############################################################################
    ### Antenna Gain Support ###
    ############################################################################

    # Antenna Gain
    bleaAntennaGain = libBLEStackComponent.createIntegerSymbol('BLE_ANTENNA_GAIN', None)
    bleaAntennaGain.setVisible(False)
    bleaAntennaGain.setReadOnly(True)
    bleaAntennaGain.setValue(antennaGain)

    ############################################################################
    ### Device family
    ############################################################################

    # Device family
    appBleDevice = libBLEStackComponent.createStringSymbol('APP_BLE_DEVICE', None)
    appBleDevice.setVisible(False)
    appBleDevice.setReadOnly(True)
    appBleDevice.setDefaultValue(devFamily)


def finalizeComponent(libBLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(libBLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()

    if devFamily == "pic32cx_bz2_family":
        print('finalizeComponent bz2')
        requiredComponents = ['pic32cx_bz2_devsupport', 'lib_crypto', 'lib_wolfcrypt']
        Database.setSymbolValue("core", "AES_CLOCK_ENABLE", True)
    elif devFamily == "pic32cx_bz3_family":
        print('finalizeComponent bz3')
        requiredComponents = ['pic32cx_bz3_devsupport']
    else:
        print("Device not support")

    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

    if devFamily == "pic32cx_bz2_family":
        res = Database.connectDependencies([['BLE_STACK_LIB', 'BLE_WolfCrypt_Dependency', 'lib_wolfcrypt', 'lib_wolfcrypt']])
        res = Database.connectDependencies([['lib_crypto', 'LIB_CRYPTO_WOLFCRYPT_Dependency', 'lib_wolfcrypt', 'lib_wolfcrypt']])


    devSuppComponent = Database.getComponentByID(devSupp)
    # update Antenna Gain
    if devSuppComponent.getSymbolValue('CUSTOM_ANT_ENABLE') == True:
        antennaGain = devSuppComponent.getSymbolValue('CUSTOM_ANT_GAIN')
    else:
        antennaGain = devSuppComponent.getSymbolByID('CUSTOM_ANT_GAIN').getDefaultValue()
    libBLEStackComponent.setSymbolValue('BLE_ANTENNA_GAIN', antennaGain)


def onAttachmentConnected(source, target):

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]
    Log.writeInfoMessage('blestack:onAttachmentConnected: source = {} remote = {}'.
            format(localComponent.getID(), targetID))


    if (connectID == "PIC32CX_BZ2_DevSupport_Dependency" or connectID == "PIC32CX_BZ3_DevSupport_Dependency"):
        print("BLE LIB:onAttachmentConnected configuring pic32xcx_wbz")
        bleSleepEnabled = localComponent.getSymbolByID("BLE_SYS_SLEEP_MODE_EN").getValue()

        if localComponent.getSymbolByID("BOOL_GAP_EXT_ADV").getValue()==False and localComponent.getSymbolByID("GAP_ADVERTISING").getValue()==True and localComponent.getSymbolByID("GAP_DSADV_EN").getValue()==True:
            bleDeepSleepEnabled = True
        else:
            bleDeepSleepEnabled = False

        if bleSleepEnabled == True:
            sendSleepEnableMessage(True)
        else:
            sendSleepEnableMessage(False)

        if bleDeepSleepEnabled == True:
            sendDeepSleepEnableMessage(True)
        else:
            sendDeepSleepEnableMessage(False)

    elif (connectID == "BLE_WolfCrypt_Dependency"):
        print("BLE LIB:onAttachmentConnected configuring lib_wolfcrypt")
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_hw", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_md5", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_sha1", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_sha256", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_hmac", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_tdes", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_hw", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_128", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_192", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_256", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_ecb", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_ecb_hw", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_cbc", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_cbc_hw", True)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_ctr", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_gcm", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_ofb", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_aes_ccm", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_ecc", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_rsa", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_oaep", False)
        Database.setSymbolValue("lib_wolfcrypt", "wolfcrypt_asn1", False)

