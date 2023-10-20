###############################################################################
######################### BLE Stack Configurations ############################
###############################################################################

from binascii import hexlify
import string

devId = 0
antennaGain = 3
txPwrCap = 15
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

def sendRTCSupportMessage(rtcRequiredValue):

    processor = Variables.get("__PROCESSOR")
    if( processor in pic32cx_bz2_family):
        Database.sendMessage("pic32cx_bz2_devsupport", "RTC_SUPPORT", {"target": "pic32cx_bz2_devsupport",
                                                    "source": "BLE_STACK_LIB",
                                                    "rtcRequired": rtcRequiredValue})
    else:
        Database.sendMessage("pic32cx_bz3_devsupport", "RTC_SUPPORT", {"target": "pic32cx_bz3_devsupport",
                                                    "source": "BLE_STACK_LIB",
                                                    "rtcRequired": rtcRequiredValue})


def handleMessage(messageID, args):
    # Log.writeInfoMessage('BLEStack:handleMessage ID={} argLen={}'.format(messageID, len(args)))
    ''' This message handler is designed to process messages
        sent from the driver/pic32cx-bz/config/device_support.py
        script.
    '''
    if(messageID == 'ANTENNA_GAIN_CHANGE'):
        global antennaGain
        
        component = Database.getComponentByID(args['target'])
        regionChange = False
        Log.writeInfoMessage('{:<17}: Handling - target={}'.format('blestack.py', args['target']))
        if Variables.get("__PROCESSOR") in pic32cx_bz2_family:
            devSup = Database.getComponentByID("pic32cx_bz2_devsupport")
        else:
            devSup = Database.getComponentByID("pic32cx_bz3_devsupport")
        
        for arg in args:
            Log.writeInfoMessage('{:<17}: {}: {}'.format('', arg, args[arg]))
            if('CUSTOM_ANT_ENABLE' == arg):
                if args[arg] == True:
                    antennaGain = devSup.getSymbolValue('CUSTOM_ANT_GAIN')
                else:
                    antennaGain = devSup.getSymbolByID('CUSTOM_ANT_GAIN').getDefaultValue()
            elif('CUSTOM_ANT_GAIN' == arg):
                # device support will send gain in onAttachmentConnected even if 'CUSTOM_ANT_ENABLE' is disabled
                if devSup.getSymbolValue('CUSTOM_ANT_ENABLE') == True:
                    antennaGain = args[arg]
            elif arg.find('REGION') != -1:
                regionChange = True
        
        component.setSymbolValue('BLE_ANTENNA_GAIN', antennaGain)
        
        # update tx power symbols
        gapConfigTxPwr(component, regionChange)


def instantiateComponent(libBLEStackComponent):
    global devId
    global antennaGain

    print('libBLEStackComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('HarmonyCore.ENABLE_APP_FILE = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_APP_FILE'))))
    print('HarmonyCore.ENABLE_OSAL = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_OSAL'))))
    print('FreeRTOS.FREERTOS_USE_QUEUE_SETS = {}'.format(str(Database.getSymbolValue("FreeRTOS", 'FREERTOS_USE_QUEUE_SETS'))))

    print('Config Name: {} processor: {}'.format(configName, processor))

    # Inform H3 core that BLE has been loaded
    Database.setSymbolValue("core", "BLE_CLOCK_ENABLE", True)

    # Inform pdsSystem that BLE has been loaded
    if ('pdsSystem' in Database.getActiveComponentIDs()):
        if (Database.getSymbolValue("pdsSystem", "BLESTACK_LOADED") == False):
            Database.setSymbolValue("pdsSystem", "BLESTACK_LOADED", True)

    
    if( processor in pic32cx_bz2_family):
        if processor == 'WBZ450' or processor == 'PIC32CX1012BZ24032':
            devId = 450
        else:
            devId = 451

        srcPath = "ble_src_bz2"
        libObj='ble_stack_bz2_lib.a'
        
        devSup = Database.getComponentByID("pic32cx_bz2_devsupport")
    else:
        if processor == 'WBZ350' or processor == 'PIC32CX5109BZ31032':
            devId = 350
        else:
            devId = 351

        srcPath = "ble_src_bz3"
        libObj='ble_stack_bz3_lib.a'
        
        devSup = Database.getComponentByID("pic32cx_bz3_devsupport")

    
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


    if( processor in pic32cx_bz2_family):
        ds = 'pic32cx_bz2_devsupport'
    else:
        ds = 'pic32cx_bz3_devsupport'

    n = 0
    appBleInit = []
    for i, f, p in dsBleInitTemplates:
        appBleInit.append( libBLEStackComponent.createFileSymbol(i, None))
        appBleInit[n].setType('STRING')
        appBleInit[n].setOutputName(ds + '.' + p)
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
                  ('app_ble_handler.c.ftl', 'app_ble_handler.c', 'SOURCE'),
                  ('app_ble_handler.h.ftl', 'app_ble_handler.h', 'HEADER'),
                  ('app_ble_dsadv.c.ftl', 'app_ble_dsadv.c', 'SOURCE'),
                  ]

    n = 0
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
        #print('App generated {} file: {} output: {} '.format(n, name, outputName))
        n = n + 1

    # Add controller only static files
    bleAppFile = [('app_ble_hci.h', 'HEADER'),
                  ('app_ble_hci.c', 'SOURCE'),
                  ]

    n = 0
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
        n = n + 1

    # Add pta static files
    bleAppFile = [('app_pta_handler.h', 'HEADER'),
                  ('app_pta_handler.c', 'SOURCE'),
                  ]

    n = 0
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
        #print('App staic {} file: {} '.format(n, name))
        n = n + 1

    # Add ble app static files
    bleAppFile = [('app_ble.h', 'HEADER'),
                  ('app_ble_log_handler.h', 'HEADER'), 
                  ('app_ble_utility.h', 'HEADER'),
                  ('app_ble_dsadv.h', 'HEADER'),
                  ('app_ble_log_handler.c', 'SOURCE'),
                  ('app_ble_utility.c', 'SOURCE'),
                  ]

    n = 0
    for name, type in bleAppFile:
        fsymbol = libBLEStackComponent.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/ble_app/'+ name)
        fsymbol.setOutputName(name)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('../../app_ble')
        fsymbol.setProjectPath('app_ble')
        fsymbol.setType(type)
        fsymbol.setEnabled(True)
        fsymbol.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])
        #print('App staic {} file: {} '.format(n, name))
        n = n + 1

    # Add mandatory middleware static files
    bleMiddlewareFile = [('mw_assert.h', 'ble_util', 'HEADER'), 
                         ('byte_stream.h', 'ble_util', 'HEADER'),
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
                         ('ble_dm.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_conn.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_sm.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_info.c', 'ble_dm', 'SOURCE'),
                         ('ble_dm_dds.c', 'ble_dm', 'SOURCE'),
                         ('ble_dd.c', 'ble_gcm', 'SOURCE'),
                         ('ble_scm.c', 'ble_gcm', 'SOURCE'),
                         ('ble_log.c', 'ble_log', 'SOURCE'),
                         ]

    n = 0
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
        else:
            fsymbol.setEnabled(True)
            fsymbol.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])
        #print('Middleware {} file: {} path: {} '.format(n, name, path))
        n = n + 1


    # Add mw_dfu.h file - generated file
    bleMwDfuHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    if( processor in pic32cx_bz2_family):
        bleMwDfuHeaderFile.setSourcePath('driver/ble/src/ble_src_bz2/middleware_ble/ble_util/mw_dfu.h')
    else:
        bleMwDfuHeaderFile.setSourcePath('driver/ble/templates/middleware/bz3/mw_dfu.h.ftl')
    bleMwDfuHeaderFile.setOutputName('mw_dfu.h')
    bleMwDfuHeaderFile.setOverwrite(True)
    bleMwDfuHeaderFile.setDestPath('ble/middleware_ble/ble_util/')
    bleMwDfuHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util/')
    bleMwDfuHeaderFile.setType('HEADER')
    bleMwDfuHeaderFile.setEnabled(False)
    bleMwDfuHeaderFile.setMarkup(True)
    bleMwDfuHeaderFile.setDependencies(dfuFileChange, ["BLE_SYS_CTRL_ONLY_EN", "BOOL_BLE_UTIL_DFU"])


    # Add mw_aes.h file - static file
    blepUtilAesHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    blepUtilAesHeaderFile.setSourcePath('driver/ble/src/' + srcPath + '/middleware_ble/ble_util/mw_aes.h')
    blepUtilAesHeaderFile.setOutputName('mw_aes.h')
    blepUtilAesHeaderFile.setDestPath('ble/middleware_ble/ble_util')
    blepUtilAesHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util')
    blepUtilAesHeaderFile.setOverwrite(True)
    blepUtilAesHeaderFile.setType('HEADER')
    blepUtilAesHeaderFile.setEnabled(True)
    blepUtilAesHeaderFile.setMarkup(True)
    blepUtilAesHeaderFile.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])


    # Add mw_aes.c - static file
    blepUtilAesSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    blepUtilAesSourceFile.setSourcePath('driver/ble/src/' + srcPath + '/middleware_ble/ble_util/mw_aes.c')
    blepUtilAesSourceFile.setOutputName('mw_aes.c')
    blepUtilAesSourceFile.setDestPath('ble/middleware_ble/ble_util/')
    blepUtilAesSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util/')
    blepUtilAesSourceFile.setOverwrite(True)
    blepUtilAesSourceFile.setType('SOURCE')
    blepUtilAesSourceFile.setEnabled(True)
    blepUtilAesSourceFile.setMarkup(True)
    blepUtilAesSourceFile.setDependencies(ctrlOnlyFileChange, ["BLE_SYS_CTRL_ONLY_EN"])


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
    if( processor in pic32cx_bz2_family):
        appBleDevice.setDefaultValue("pic32cx_bz2")
    else:
        appBleDevice.setDefaultValue("pic32cx_bz3")

def finalizeComponent(libBLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(libBLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    processor = Variables.get("__PROCESSOR")

    if( processor in pic32cx_bz2_family):
        print('finalizeComponent bz2')
        requiredComponents = ['pic32cx_bz2_devsupport', 'lib_crypto', 'lib_wolfcrypt']
    else:
        print('finalizeComponent bz3')
        requiredComponents = ['pic32cx_bz3_devsupport']

    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

    if( processor in pic32cx_bz2_family):
        res = Database.connectDependencies([['BLE_STACK_LIB', 'BLE_WolfCrypt_Dependency', 'lib_wolfcrypt', 'lib_wolfcrypt']])
        res = Database.connectDependencies([['lib_crypto', 'LIB_CRYPTO_WOLFCRYPT_Dependency', 'lib_wolfcrypt', 'lib_wolfcrypt']])

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

        if bleSleepEnabled == True or bleDeepSleepEnabled==True:
            sendRTCSupportMessage(True)

        if bleSleepEnabled == False and bleDeepSleepEnabled==False:
            sendRTCSupportMessage(False)

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

