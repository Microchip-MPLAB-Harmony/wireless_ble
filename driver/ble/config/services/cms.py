import re

cmsSvcMaxCount          = 5
cmsSvcConfMenu          = []
cmsSvcName              = []
cmsSvcUUID              = []
cmsSvcCharNum           = []
cmsFileSrc              = []
cmsFileHdr              = []

cmsSvcCharMaxCount      = 10
cmsSvcCharConfMenu      = []
################################################################################
#### Component ####
################################################################################

def cmsChkUuid(uuid):
    import string
    #print("gattUuidCheck: uuid = " + uuid)
    if len(uuid) != 4 and len(uuid) != 32 or not all(c in string.hexdigits for c in uuid):
        #print("gattUUidCheck uuid invalid")
        return False
    else:
        return True

def cmsChkSvcName(symbol, event):
    name = event["value"]
    # Make a regular expression
    # for identify valid identifier
    regex = '^[a-z_][a-z0-9_]*'

    # pass the regualar expression
    # and the string in search() method
    if(len(name) <= 10 and re.search(regex, name)): 
        #print("Valid Identifier")
        symbol.setValue(True)
    else: 
        #print("Invalid Identifier")
        symbol.setValue(False)

def cmsGenCChar(symbol, event):
    value = event["value"]
    char = []

    for idx in range(0, len(value), 2):
        char.append('0x' + value[idx:idx+2])

    symbol.setValue(', '.join(char))

def cmsGenSvcUuidChar(symbol, event):

    if cmsChkUuid(event["value"]) == False:
        symbol.setValue("")
        return

    cmsGenCChar(symbol, event)

def cmsGenFileSrc(symbol, event):
    #"CMS_INT_SERVICE_COUNT", "CMS_STR_SVC_NAME_" + str(count)

    if event["id"] == "CMS_INT_SERVICE_COUNT":
        for count in range(0, cmsSvcMaxCount):
            cmsFileSrc[count].setEnabled(False)
            cmsFileHdr[count].setEnabled(False)

        svcCount = event["value"]
        for count in range(0, svcCount):
            cmsFileSrc[count].setEnabled(True)
            cmsFileHdr[count].setEnabled(True)
    else:
        svc = int(event["id"][event["id"].rfind("_")+1:])
        cmsFileSrc[svc].setOutputName("ble_" + event["value"].lower() + "_svc.c")
        cmsFileHdr[svc].setOutputName("ble_" + event["value"].lower() + "_svc.h")

def cmsSvcConfMenuVisible(symbol, event):
    #global cmsSvcConfMenu
    #global cmsSvcMaxCount

    appCount = event["value"]

    for count in range(0, cmsSvcMaxCount):
        cmsSvcConfMenu[count].setVisible(False)

    for count in range(0, appCount):
        cmsSvcConfMenu[count].setVisible(True)

def cmsSvcCharConfMenuVisible(symbol, event):
    #global cmsSvcConfMenu
    #global cmsSvcMaxCount

    appCount = event["value"]
    svc = int(event["id"][event["id"].rfind("_")+1:])
    
    for charCount in range(0, cmsSvcCharMaxCount):
        cmsSvcCharConfMenu[svc][charCount].setVisible(False)

    for charCount in range(0, appCount):
        cmsSvcCharConfMenu[svc][charCount].setVisible(True) 

def cmsCharReadWriteVisible(symbol, event):
    if event["value"] == True:
       symbol.setVisible(True)
    
    else:
        id = event["id"]
        
        #"CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"
        if id.find("READ") != -1:
            symbol.setVisible(False)
            return

        #"CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD"
        #"CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"
        if id.find("CMD") != -1:
            prop = symbol.getComponent().getSymbolByID(id.replace("CMD","REQ"))
        else:
            prop = symbol.getComponent().getSymbolByID(id.replace("REQ","CMD"))
    
        if prop.getValue() == False:
            symbol.setVisible(False)

def cmsCccdVisible(symbol, event):
    if event["value"] == True:
       symbol.setVisible(True)
    else:
        id = event["id"]

        #"CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_NOTIFY"
        #"CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_INDICATE"
        if id.find("NOTIFY") != -1:
            prop = symbol.getComponent().getSymbolByID(id.replace("NOTIFY","INDICATE"))
        else:
            prop = symbol.getComponent().getSymbolByID(id.replace("INDICATE","NOTIFY"))

        if prop.getValue() == False:
            symbol.setVisible(False)

def instantiateComponent(serviceCmsComponent):
    print('Load Module: SERVICE_CMS')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    #print('Config Name: {} processor: {}'.format(configName, processor))


    cmsDescManualWrite = "Set if service need to know write operations on this attribute. Service also need to send response (if need) manually."
    cmsDescManualRead = "Set if service need to know read operations on this attribute. Service also need to send response manually."
    cmsDescAuth = "Access requires encryption, MITM protection."
    cmsDescAuthSc = "Access requires encryption, MITM protection, Secure Connections."
    cmsDescEnc = "Access requires encryption, no MITM protection."

    cmsSvcNum = serviceCmsComponent.createIntegerSymbol("CMS_INT_SERVICE_COUNT", None)
    cmsSvcNum.setLabel("Number of Services")
    cmsSvcNum.setDescription("Number of Services")
    cmsSvcNum.setMin(1)
    cmsSvcNum.setMax(cmsSvcMaxCount)
    cmsSvcNum.setDefaultValue(1)
    cmsSvcNum = serviceCmsComponent.createHexSymbol("CMS_INT_MAX_CHAR_COUNT", None)
    cmsSvcNum.setVisible(False)
    cmsSvcNum.setDefaultValue(cmsSvcCharMaxCount)


    for count in range(0, cmsSvcMaxCount):
        # generate ble_[service name]_svc.c
        cmsFileSrc.append(count)
        cmsFileSrc[count] = serviceCmsComponent.createFileSymbol("CMS_FILE_" + str(count) + "_C", None)
        cmsFileSrc[count].setSourcePath("driver/ble/templates/service/ble_cms.c.ftl")
        cmsFileSrc[count].setOutputName("ble_svc" + str(count) + "_svc.c")

        cmsFileSrc[count].setMarkup(True)
        cmsFileSrc[count].setOverwrite(True)
        cmsFileSrc[count].setDestPath("ble/service_ble/ble_cms")
        cmsFileSrc[count].setProjectPath('config/' + configName + '/ble/service_ble/ble_cms')
        cmsFileSrc[count].setType("SOURCE")
        if count == 0:
            cmsFileSrc[count].setDependencies(cmsGenFileSrc, ["CMS_INT_SERVICE_COUNT", "CMS_STR_SVC_NAME_" + str(count)])
            cmsFileSrc[count].setEnabled(True)
        else:
            cmsFileSrc[count].setEnabled(False)
        cmsFileSrc[count].addMarkupVariable("CMS_SVC_NO", str(count))

        # generate ble_[service name]_svc.h
        cmsFileHdr.append(count)
        cmsFileHdr[count] = serviceCmsComponent.createFileSymbol("CMS_FILE_" + str(count) + "_H", None)
        cmsFileHdr[count].setSourcePath("driver/ble/templates/service/ble_cms.h.ftl")
        cmsFileHdr[count].setOutputName("ble_svc" + str(count) + "_svc.h")

        cmsFileHdr[count].setMarkup(True)
        cmsFileHdr[count].setOverwrite(True)
        cmsFileHdr[count].setDestPath("ble/service_ble/ble_cms")
        cmsFileHdr[count].setProjectPath('config/' + configName + '/ble/service_ble/ble_cms')
        cmsFileHdr[count].setType("HEADER")
        if count == 0:
            cmsFileHdr[count].setEnabled(True)
        else:
            cmsFileHdr[count].setEnabled(False)
        cmsFileHdr[count].addMarkupVariable("CMS_SVC_NO", str(count))
        cmsFileHdr[count].addMarkupVariable("CMS_SVC_HANDLE", "0x%X" % (0x8000 + count * 48))



        cmsSvcConfMenu.append(count)
        cmsSvcConfMenu[count] = serviceCmsComponent.createMenuSymbol("CMS_MENU_SERVICE_" + str(count), None)
        cmsSvcConfMenu[count].setLabel("Service " + str(count) + " Configuration")
        cmsSvcConfMenu[count].setDescription("Service " + str(count) + " Configuration")
        # Only 1 callback is sufficient
        cmsSvcConfMenu[0].setDependencies(cmsSvcConfMenuVisible, ["CMS_INT_SERVICE_COUNT"])
        if (count == 0):
            cmsSvcConfMenu[count].setVisible(True)
        else:
            cmsSvcConfMenu[count].setVisible(False)

        cmsSvcName.append(count)
        cmsSvcName[count] = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_NAME_" + str(count), cmsSvcConfMenu[count])
        cmsSvcName[count].setLabel("Service Name")
        cmsSvcName[count].setDescription("Service Name")
        cmsSvcName[count].setDefaultValue("svc" + str(count))
        cmnt = serviceCmsComponent.createCommentSymbol("CMS_CMNT_SVC_NAME_" + str(count), cmsSvcConfMenu[count])
        cmnt.setLabel("**** Service name must be valid C-Language identifier and maximum is 10 bytes and lowercase. ****")

        cmsCheck = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_NAME_" + str(count), cmsSvcConfMenu[count])
        cmsCheck.setVisible(False)
        cmsCheck.setDefaultValue(True)
        cmsCheck.setDependencies(cmsChkSvcName, ["CMS_STR_SVC_NAME_" + str(count)])

        cmsSvcUUID.append(count)
        cmsSvcUUID[count] = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_UUID_C_" + str(count),  cmsSvcConfMenu[count])
        cmsSvcUUID[count].setVisible(False)
        #cmsSvcUUID[count].setDefaultValue("0x00, 0x00")
        cmsSvcUUID[count].setDependencies(cmsGenSvcUuidChar, ["CMS_STR_SVC_UUID_" + str(count)])
        svcUUID = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_UUID_" + str(count),  cmsSvcConfMenu[count])
        svcUUID.setLabel("Service UUID")
        svcUUID.setDefaultValue("%04X" % count)
        #gattSvcUUID.setDependencies(gattUuidCheck, ["CMS_STR_SVC_UUID_" + str(count)])

        cmnt = serviceCmsComponent.createCommentSymbol("CMS_CMNT_SVC_UUID_" + str(count), cmsSvcConfMenu[count])
        cmnt.setLabel("**** Service UUID must be 2 bytes or 16 bytes and should input in hexidecimal(LSB). ****")
        #gattCmnt.setDependencies(gattUuidCheck, ["CMS_STR_SVC_UUID_" + str(count)])


        cmsSvcCharNum.append(count)
        cmsSvcCharNum[count] = serviceCmsComponent.createIntegerSymbol("CMS_INT_SVC_CHAR_COUNT_" + str(count), cmsSvcConfMenu[count])
        cmsSvcCharNum[count].setLabel("Number of Characteristics")
        cmsSvcCharNum[count].setDescription("Number of Characteristics")
        cmsSvcCharNum[count].setMin(1)
        cmsSvcCharNum[count].setMax(cmsSvcCharMaxCount)
        cmsSvcCharNum[count].setDefaultValue(1)


        #cmsSvcCharConfMenu.append([0]*cmsSvcCharMaxCount)
        charConfMenu = []
        for charCount in range(0, cmsSvcCharMaxCount):
            charConfMenu.append(charCount)
            charConfMenu[charCount] = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount), cmsSvcCharNum[count])
            charConfMenu[charCount].setLabel("Characteristic " + str(charCount) + " Configuration")
            charConfMenu[charCount].setDescription("Characteristic " + str(charCount) + " Configuration")
            # Only 1 callback is sufficient
            charConfMenu[0].setDependencies(cmsSvcCharConfMenuVisible, ["CMS_INT_SVC_CHAR_COUNT_" + str(count)])
            if (charCount == 0):
                charConfMenu[charCount].setVisible(True)
            else:
                charConfMenu[charCount].setVisible(False)

            charDecMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_DEC", charConfMenu[charCount])
            charDecMenu.setLabel("Characteristic Declaration")
            charDecMenu.setDescription("Characteristic Declaration")

            charPropMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP", charDecMenu)
            charPropMenu.setLabel("Properties")
            charPropMenu.setDescription("Properties")
            charProp = serviceCmsComponent.createCommentSymbol("CMS_CMNT_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP", charPropMenu)
            charProp.setLabel("**** At least one property must be selected.****")

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ", charPropMenu)
            charProp.setLabel("Read")
            charProp.setDefaultValue(False)
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", charPropMenu)
            charProp.setLabel("Write without response")
            charProp.setDefaultValue(False)
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ", charPropMenu)
            charProp.setLabel("Write with response")
            charProp.setDefaultValue(False)
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_NOTIFY", charPropMenu)
            charProp.setLabel("Notifiy")
            charProp.setDefaultValue(False)
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_INDICATE", charPropMenu)
            charProp.setLabel("Indicate")
            charProp.setDefaultValue(False)

            charUuidC = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_UUID_C" ,  charDecMenu)
            charUuidC.setVisible(False)
            charUuidC.setDependencies(cmsGenSvcUuidChar, ["CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_UUID"])

            charUuid = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_UUID",  charDecMenu)
            charUuid.setLabel("UUID")
            charUuid.setDefaultValue("%02X%02X" % (count, charCount))

            gattCmnt = serviceCmsComponent.createCommentSymbol("CMS_CMNT_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_UUID", charDecMenu)
            gattCmnt.setLabel("****UUID must be 2 bytes or 16 bytes and should input in hexidecimal(LSB).****")


            # Char. Value
            charValueMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_VALUE", charConfMenu[charCount])
            charValueMenu.setLabel("Characteristic Value")
            charValueMenu.setDescription("Characteristic Value")

            charConf = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_VALUE_C" ,  charValueMenu)
            charConf.setVisible(False)
            charConf.setDependencies(cmsGenCChar, ["CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_VALUE"])
            charConf = serviceCmsComponent.createStringSymbol("CMS_STR_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_VALUE",  charValueMenu)
            charConf.setLabel("Default Value")
            charConf.setDefaultValue("00")
            charConf.setVisible(False)
            charConf.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])

            gattCmnt = serviceCmsComponent.createCommentSymbol("CMS_CMNT_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_VALUE", charValueMenu)
            gattCmnt.setLabel("****Characteristic value should input in hexidecimal. ****")
            gattCmnt.setVisible(False)
            gattCmnt.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])

            charConf = serviceCmsComponent.createIntegerSymbol("CMS_INT_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_LEN", charValueMenu)
            charConf.setLabel("Maximum Length")
            charConf.setDescription("Maximum Length")
            charConf.setMin(1)
            charConf.setMax(247)
            charConf.setDefaultValue(1)
            charConf.setVisible(False)
            charConf.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])



            # Attribute Settings
            charPropMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_SETTING", charValueMenu)
            charPropMenu.setLabel("Settings")
            charPropMenu.setDescription("Settings")

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_SETTING_READ", charPropMenu)
            charProp.setLabel("Manual Read Resonpse")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescManualRead)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_SETTING_WRITE", charPropMenu)
            charProp.setLabel("Manual Write Response")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescManualWrite)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_SETTING_VAR", charPropMenu)
            charProp.setLabel("Variable Length")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription("Set if writes are allowed with an offset.")
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])

            # Attribute Permissions
            charPropMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS", charValueMenu)
            charPropMenu.setLabel("Permissions")
            charPropMenu.setDescription("Permissions")

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_READ_ENC", charPropMenu)
            charProp.setLabel("Read Encryption")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescEnc)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_READ_AUTH", charPropMenu)
            charProp.setLabel("Read Authenticated")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescAuth)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_READ_AUTH_SC", charPropMenu)
            charProp.setLabel("Read Authenticated Secure Connections")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescAuthSc)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_READ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_WRITE_ENC", charPropMenu)
            charProp.setLabel("Write Encryption")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescEnc)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_WRITE_AUTH", charPropMenu)
            charProp.setLabel("Write Authenticated")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescAuth)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PERMS_WRITE_AUTH_SC", charPropMenu)
            charProp.setLabel("Write Authenticated Secure Connections")
            charProp.setDefaultValue(False)
            charProp.setVisible(False)
            charProp.setDescription(cmsDescAuthSc)
            charProp.setDependencies(cmsCharReadWriteVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_CMD", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_WRITE_REQ"])

            # CCCD Settings
            charCccdMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD", charConfMenu[charCount])
            charCccdMenu.setLabel("Client Characteristic Configuration")
            charCccdMenu.setDescription("Client Characteristic Configuration")
            charCccdMenu.setVisible(False)
            charCccdMenu.setDependencies(cmsCccdVisible, ["CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_NOTIFY", "CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_PROP_INDICATE"])

            charPropMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_SETTING", charCccdMenu)
            charPropMenu.setLabel("Settings")
            charPropMenu.setDescription("Settings")

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_READ", charPropMenu)
            charProp.setLabel("Manual Read Resonpse")
            charProp.setDefaultValue(False)
            charProp.setDescription(cmsDescManualRead)
            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_WRITE", charPropMenu)
            charProp.setLabel("Manual Write Response")
            charProp.setDefaultValue(False)
            charProp.setDescription(cmsDescManualWrite)

            # CCCD Permissions
            charPropMenu = serviceCmsComponent.createMenuSymbol("CMS_MENU_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_PERMS", charCccdMenu)
            charPropMenu.setLabel("Permissions")
            charPropMenu.setDescription("Permissions")

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_READ_ENC", charPropMenu)
            charProp.setLabel("Read Encryption")
            charProp.setDescription(cmsDescEnc)
            charProp.setDefaultValue(False)

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_READ_AUTH", charPropMenu)
            charProp.setLabel("Read Authenticated")
            charProp.setDescription(cmsDescAuth)
            charProp.setDefaultValue(False)

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_READ_AUTH_SC", charPropMenu)
            charProp.setLabel("Read Authenticated Secure Connections")
            charProp.setDescription(cmsDescAuthSc)
            charProp.setDefaultValue(False)

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_WRITE_ENC", charPropMenu)
            charProp.setLabel("Write Encryption Required")
            charProp.setDescription(cmsDescEnc)
            charProp.setDefaultValue(False)

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_WRITE_AUTH", charPropMenu)
            charProp.setLabel("Write Authenticated")
            charProp.setDescription(cmsDescAuth)
            charProp.setDefaultValue(False)

            charProp = serviceCmsComponent.createBooleanSymbol("CMS_BOOL_SVC_" + str(count) + "_CHAR_" + str(charCount) + "_CCCD_WRITE_AUTH_SC", charPropMenu)
            charProp.setLabel("Write Authenticated Secure Connections")
            charProp.setDescription(cmsDescAuthSc)
            charProp.setDefaultValue(False)

        cmsSvcCharConfMenu.append(charConfMenu)


def finalizeComponent(serviceCmsComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(serviceCmsComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['BLE_STACK_LIB']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])
