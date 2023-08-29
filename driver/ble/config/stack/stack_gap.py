global gapGenTxPwrCap
def gapGenTxPwrCap():
    global devId
    global txPwrCap
    
    if devId == 451:
        if (Database.getSymbolValue("pic32cx_bz2_devsupport", 'ETSI_REGION') == True 
            or Database.getSymbolValue("pic32cx_bz2_devsupport", 'KOREA_REGION') == True):
            txPwrCap = 9
        elif Database.getSymbolValue("pic32cx_bz2_devsupport", 'JAPAN_REGION') == True:
            txPwrCap = 12
        else:
            txPwrCap = 15
    elif devId == 450:
        if (Database.getSymbolValue("pic32cx_bz2_devsupport", 'ETSI_REGION') == True 
            or Database.getSymbolValue("pic32cx_bz2_devsupport", 'KOREA_REGION') == True
            or Database.getSymbolValue("pic32cx_bz2_devsupport", 'JAPAN_REGION') == True):
            txPwrCap = 11
        else:
            txPwrCap = 15
    else: # pic32cx_bz3_family will be redefined in the future
        if Database.getSymbolValue("pic32cx_bz3_devsupport", 'ETSI_REGION') == True:
            txPwrCap = 11
        else:
            txPwrCap = 15
        
    #print "gapGenTxPwrCap devId={}, txPwrCap={}".format(devId,txPwrCap)

global gapGenDefTxPwr
def gapGenDefTxPwr(isLegacyAdv):
    global devId
    global txPwrCap

    #print "gapGenDefTxPwr {}".format(devId)

    if devId == 451:
        if isLegacyAdv:
            return txPwrCap
        else:
            return 15
    elif devId == 450:
        return 11
    else:   #pic32cx_bz3_family
        return 13

global gapGenMaxTxPwr
def gapGenMaxTxPwr(isLegacyAdv):
    global devId
    global antennaGain
    global txPwrCap

    #print "gapGenMaxTxPwr dev:{}, cap:{}, gain:{}".format(devId, txPwrCap, antennaGain)

    if devId == 451:
        if isLegacyAdv and txPwrCap < 15:
            return min(12 + antennaGain, txPwrCap)
        else:
            return 12 + antennaGain
    elif devId == 450:
        if isLegacyAdv and txPwrCap < 15:
            return min(6 + antennaGain, txPwrCap)
        else:
            return 6 + antennaGain
    else:   #pic32cx_bz3_family
        if isLegacyAdv and txPwrCap < 15:
            return min(10 + antennaGain, 11)
        else:
            return 10 + antennaGain

global gapGenMinTxPwr
def gapGenMinTxPwr():
    global devId
    global antennaGain

    if devId == 451:
        return -29 + antennaGain
    elif devId == 450:
        return -25 + antennaGain
    else:   #pic32cx_bz3_family
        return -25 + antennaGain


global gapConfigTxPwr
def gapConfigTxPwr(component, regionChange):
    global devId

    if regionChange == True:
        gapGenTxPwrCap()

    gapTxPwr = component.getSymbolByID('GAP_CONN_TX_PWR')
    gapTxPwr.setMax(gapGenMaxTxPwr(False))
    gapTxPwr.setMin(gapGenMinTxPwr())

    gapTxPwr = component.getSymbolByID('GAP_ADV_TX_PWR')
    gapTxPwr.setMax(gapGenMaxTxPwr(True))
    gapTxPwr.setMin(gapGenMinTxPwr())
    if devId == 451 and regionChange == True:
        if gapTxPwr.getDefaultValue() != gapGenDefTxPwr(True):
            # we cannot change symbol's default value here, so use dynamical value to simulate default value.
            gapTxPwr.setValue(gapGenDefTxPwr(True))
        else:
            gapTxPwr.clearValue()

    gapTxPwr = component.getSymbolByID('GAP_EXT_ADV_TX_POWER')
    gapTxPwr.setMax(gapGenMaxTxPwr(component.getSymbolValue('GAP_EXT_ADV_EVT_PROPERTIES') < 3))
    gapTxPwr.setMin(gapGenMinTxPwr())
    if devId == 451 and regionChange == True:
        if component.getSymbolValue('GAP_EXT_ADV_EVT_PROPERTIES') < 3:
            if gapTxPwr.getDefaultValue() != gapGenDefTxPwr(True):
                gapTxPwr.setValue(gapGenDefTxPwr(True))
            else:
                gapTxPwr.clearValue()

    gapTxPwr = component.getSymbolByID('GAP_EXT_ADV_TX_POWER_2')
    gapTxPwr.setMax(gapGenMaxTxPwr(component.getSymbolValue('GAP_EXT_ADV_EVT_PROPERTIES_2') < 3))
    gapTxPwr.setMin(gapGenMinTxPwr())
    if devId == 451 and regionChange == True:
        if component.getSymbolValue('GAP_EXT_ADV_EVT_PROPERTIES_2') < 3:
            if gapTxPwr.getDefaultValue() != gapGenDefTxPwr(True):
                gapTxPwr.setValue(gapGenDefTxPwr(True))
            else:
                gapTxPwr.clearValue()

def bleDevNameCheck(symbol, event):
    name = event["value"]

    if(len(name) <= 32):
        symbol.setValue(True)
    else:
        symbol.setValue(False)

def bleDevNameCheckComment(symbol, event):
    name = event["value"]

    if(len(name) <= 32):
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)

def gapConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

def gapLegacyAdvVisibility(symbol, event):
    global gapExtAdvEn
    global gapAdvertising
    symbol.setVisible(not gapExtAdvEn.getValue() and gapAdvertising.getValue())

def gapExtAdvEvtPropVisibility(symbol, event):
    symbol.setVisible(not event["value"] < 3)
    
def gapExtAdvTxPwr(symbol, event):
    global devId

    if event["id"] == "GAP_EXT_ADV_ADV_SET_2":
        symbol.setVisible(event["value"])
        return
    symbol.setMax(gapGenMaxTxPwr(event["value"] < 3))
    symbol.setMin(gapGenMinTxPwr())
    
    if devId == 451:
        if symbol.getDefaultValue() != gapGenDefTxPwr(event["value"] < 3):
            # we cannot change symbol's default value here, so use dynamical value to simulate default value.
            symbol.setValue(gapGenDefTxPwr(event["value"] < 3))
        else:
            symbol.clearValue()
    
    
global bleStackCheckHexFormat
def bleStackCheckHexFormat(hexValue):
    if (len(hexValue) % 2 != 0) or not all(c in string.hexdigits for c in hexValue):
        #print("Invalid Hex Format")
        return False
    else:
        #print("Valid Hex Format")
        return True

global gapGenExtAdvData
def gapGenExtAdvData(symbol):
    if -1 == symbol.getID().find("_2"):
        advDataFlagEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_EN")
        advDataFlag = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG")
        advDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_EN")
        advDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME")
        advDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_EN")
        advDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA")
        advDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_UUID")
        advDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_EN")
        advDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD")
        advDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_CID")
        advDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_EN")
        advDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD")
        advertisingDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LEN")
        advertisingDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_VALUE")
    else:
        advDataFlagEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_EN_2")
        advDataFlag = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_2")
        advDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2")
        advDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_2")
        advDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2")
        advDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_2")
        advDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_UUID_2")
        advDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_EN_2")
        advDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_2")
        advDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_CID_2")
        advDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_EN_2")
        advDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_2")
        advertisingDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LEN_2")
        advertisingDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_VALUE_2")

    advDataList = []        # array of formatted hex bytes
    advDataLength = 0       # total number of bytes of advDataList
    MAX_ADV_DATA_LEN = 251  # maximum number of bytes of advDataList

    if advDataFlagEn.getValue() == True:
        totalFlagLen=getValueLen(int(advDataFlag.getSelectedKey(), 16))+1
        advDataList.append('0x{:02X}'.format(totalFlagLen))
        advDataList.append('0x01')

        # get the Flag Ad Type value
        formatInt2HexBytes(int(advDataFlag.getSelectedKey(), 16), advDataList, 0)
        advDataLength+=totalFlagLen+1

    if advDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(advDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        advDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to advDataList
        advDataList.append('0x{:02X}'.format(localNameHexLen))

        advDataList.append('0x09')  # some hex key
        advDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            advDataList.append('0x' + localNameHex[byte:byte+2])

    if advDataSvcDataEn.getValue() == True:
        # Only genetate Service Data if format is correct
        if (len(advDataSvcUUID.getValue()) == 4 and bleStackCheckHexFormat(advDataSvcUUID.getValue()) and bleStackCheckHexFormat(advDataSvcData.getValue())):
            totalSvcDataLen=2+1
            totalSvcDataLen+=(len(advDataSvcData.getValue())/2)
            advDataList.append('0x{:02X}'.format(totalSvcDataLen))
            advDataList.append('0x16')

            # get svc uuid as string
            formatHexString2HexBytes((advDataSvcUUID.getValue()).upper(), advDataList)

            # get svcData as string
            formatHexString2HexBytes((advDataSvcData.getValue()).upper(), advDataList)

            advDataLength+=totalSvcDataLen+1
        else:
            advDataList.append("#InvalidServiceData")
            advDataLength+=1

    if advDataManuSpecDataEn.getValue() == True:
        if (len(advDataManuSpecDataCid.getValue()) == 4 and bleStackCheckHexFormat(advDataManuSpecDataCid.getValue()) and bleStackCheckHexFormat(advDataManuSpecData.getValue())):
            totalMsdLen=2+1
            totalMsdLen+=(len(advDataManuSpecData.getValue())/2)
            advDataList.append('0x{:02X}'.format(totalMsdLen))
            advDataList.append('0xff')

            # get svc uuid as string
            formatHexString2HexBytes((advDataManuSpecDataCid.getValue()).upper(), advDataList)

            # get Manufacturer data as string
            formatHexString2HexBytes((advDataManuSpecData.getValue()).upper(), advDataList)

            advDataLength+=totalMsdLen+1
        else:
            advDataList.append("#InvalidManufacturerData")
            advDataLength+=1

    if advDataUserDefEn.getValue() == True:
        # Only genetate User Defined Data if format is correct
        if (bleStackCheckHexFormat(advDataUserDef.getValue())):
            totalUsdLen=(len(advDataUserDef.getValue())/2)
            formatHexString2HexBytes((advDataUserDef.getValue()).upper(), advDataList)
            advDataLength+=totalUsdLen
        else:
            advDataList.append("#InvalidUserDefinedData")
            advDataLength+=1


    #for e in advDataList:
    #    print('advDataList: {}'.format(e))

    #clear data list if data length exceeds the limit
    if (advDataLength > MAX_ADV_DATA_LEN):
        advDataList[:] = []
        advDataLength = 1
        advDataList.append("#ExceedMaxDataLength_251BYTES")

    symbol.setValue(', '.join(advDataList))
    advertisingDataLen.setValue(advDataLength)
    advertisingDataValue.setLabel(symbol.getValue())
    #print('gapSetAdvData advData = {}'.format(symbol.getValue()))
    #print('gapSetAdvData advDataLength = {}'.format(advDataLength))

def gapSetExtAdvData(symbol, event):
    #print('gapSetAdvData symbol.getType = ' + symbol.getType())
    #print('gapSetAdvData event.id       = ' + event["id"])
    #print('gapSetAdvData event.value    = ' + str(event["value"]))

    gapGenExtAdvData(symbol)

global gapGenExtScanRspData
def gapGenExtScanRspData(symbol):
    if -1 == symbol.getID().find("_2"):
        scanRspDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN")
        scanRspDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME")
        scanRspDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN")
        scanRspDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA")
        scanRspDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID")
        scanRspDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_EN")
        scanRspDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD")
        scanRspDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_CID")
        scanRspDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_EN")
        scanRspDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD")
        scanRspDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LEN")
        scanRspDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_VALUE")
    else:
        scanRspDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2")
        scanRspDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2")
        scanRspDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2")
        scanRspDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2")
        scanRspDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2")
        scanRspDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_EN_2")
        scanRspDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_2")
        scanRspDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_CID_2")
        scanRspDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_EN_2")
        scanRspDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_2")
        scanRspDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LEN_2")
        scanRspDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_VALUE_2")
    # build the GAP_SCAN_RSP_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type
            xx..n   - data
    '''
    scanRspDataList = []        # array of formatted hex bytes
    scanRspDataLength = 0       # total number of bytes of scanRspDataList
    MAX_SCAN_RSP_DATA_LEN = 251 # maximum number of bytes of scanRspDataList

    if scanRspDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(scanRspDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        scanRspDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to scanRspDataList
        scanRspDataList.append('0x{:02X}'.format(localNameHexLen))

        scanRspDataList.append('0x09')  # some hex key
        scanRspDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            scanRspDataList.append('0x' + localNameHex[byte:byte+2])

    if scanRspDataSvcDataEn.getValue() == True:
        # Only genetate Service Data if format is correct
        if (len(scanRspDataSvcUUID.getValue()) == 4 and bleStackCheckHexFormat(scanRspDataSvcUUID.getValue()) and bleStackCheckHexFormat(scanRspDataSvcData.getValue())):
            totalSvcDataLen=2+1
            totalSvcDataLen+=(len(scanRspDataSvcData.getValue())/2)
            scanRspDataList.append('0x{:02X}'.format(totalSvcDataLen))
            scanRspDataList.append('0x16')

            # get svc uuid as string
            formatHexString2HexBytes((scanRspDataSvcUUID.getValue()).upper(), scanRspDataList)

            # get svcData as string
            formatHexString2HexBytes((scanRspDataSvcData.getValue()).upper(), scanRspDataList)

            scanRspDataLength+=totalSvcDataLen+1
        else:
            scanRspDataList.append("#InvalidServiceData")
            scanRspDataLength+=1

    if scanRspDataManuSpecDataEn.getValue() == True:
        if (len(scanRspDataManuSpecDataCid.getValue()) == 4 and bleStackCheckHexFormat(scanRspDataManuSpecDataCid.getValue()) and bleStackCheckHexFormat(scanRspDataManuSpecData.getValue())):
            totalMsdLen=2+1
            totalMsdLen+=(len(scanRspDataManuSpecData.getValue())/2)
            scanRspDataList.append('0x{:02X}'.format(totalMsdLen))
            scanRspDataList.append('0xff')

            # get svc uuid as string
            formatHexString2HexBytes((scanRspDataManuSpecDataCid.getValue()).upper(), scanRspDataList)

            # get Manufacturer data as string
            formatHexString2HexBytes((scanRspDataManuSpecData.getValue()).upper(), scanRspDataList)

            scanRspDataLength+=totalMsdLen+1
        else:
            scanRspDataList.append("#InvalidManufacturerData")
            scanRspDataLength+=1

    if scanRspDataUserDefEn.getValue() == True:
        # Only genetate User Defined Data if format is correct
        if (bleStackCheckHexFormat(scanRspDataUserDef.getValue())):
            totalUsdLen=(len(scanRspDataUserDef.getValue())/2)
            formatHexString2HexBytes((scanRspDataUserDef.getValue()).upper(), scanRspDataList)
            scanRspDataLength+=totalUsdLen
        else:
            scanRspDataList.append("#InvalidUserDefinedData")
            scanRspDataLength+=1

    #for e in scanRspDataList:
        #print('scanRspDataList: {}'.format(e))

    #clear data list if data length exceeds the limit
    if (scanRspDataLength > MAX_SCAN_RSP_DATA_LEN):
        scanRspDataList[:] = []
        scanRspDataLength=1
        scanRspDataList.append("#ExceedMaxDataLength_251BYTES")

    symbol.setValue(', '.join(scanRspDataList))
    scanRspDataLen.setValue(scanRspDataLength)
    scanRspDataValue.setLabel(symbol.getValue())
    #print('gapSetScanRspData advData = {}'.format(symbol.getValue()))
    #print('gapSetScanRspData advDataLength = {}'.format(scanRspDataLength))

def gapSetExtScanRspData(symbol, event):
    #print('gapSetScanRspData symbol.getType = ' + symbol.getType())
    #print('gapSetScanRspData event.id       = ' + event["id"])
    #print('gapSetScanRspData event.value    = ' + str(event["value"]))

    gapGenExtScanRspData(symbol)

global gapGenAdvData
def gapGenAdvData(symbol):
    advDataFlagEn = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_FLAG_EN")
    advDataFlag = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_FLAG")
    advDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_LOCAL_NAME_EN")
    advDataLocalName = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_LOCAL_NAME")
    advDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_SERVICE_DATA_EN")
    advDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_SERVICE_UUID")
    advDataSvcData = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_SERVICE_DATA")
    advDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_MSD_EN")
    advDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_MSD")
    advDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_MSD_CID")
    advDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_UDD_EN")
    advDataUserDef = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_UDD")
    advertisingDataLen = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_LEN")
    advertisingDataValue = symbol.getComponent().getSymbolByID("GAP_ADV_DATA_VALUE")

    # build the GAP_ADV_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type
            xx..n   - data
    '''
    advDataList = []        # array of formatted hex bytes
    advDataLength = 0       # total number of bytes of advDataList
    MAX_ADV_DATA_LEN = 31   # maximum number of bytes of advDataList

    if advDataFlagEn.getValue() == True:
        totalFlagLen=getValueLen(int(advDataFlag.getSelectedKey(), 16))+1
        advDataList.append('0x{:02X}'.format(totalFlagLen))
        advDataList.append('0x01')

        # get the Flag Ad Type value
        formatInt2HexBytes(int(advDataFlag.getSelectedKey(), 16), advDataList, 0)
        advDataLength+=totalFlagLen+1

    if advDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(advDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        advDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to advDataList
        advDataList.append('0x{:02X}'.format(localNameHexLen))

        advDataList.append('0x09')  # some hex key
        advDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            advDataList.append('0x' + localNameHex[byte:byte+2])

    if advDataSvcDataEn.getValue() == True:
        # Only genetate Service Data if format is correct
        if (len(advDataSvcUUID.getValue()) == 4 and bleStackCheckHexFormat(advDataSvcUUID.getValue()) and bleStackCheckHexFormat(advDataSvcData.getValue())):
            totalSvcDataLen=2+1
            totalSvcDataLen+=(len(advDataSvcData.getValue())/2)
            advDataList.append('0x{:02X}'.format(totalSvcDataLen))
            advDataList.append('0x16')

            # get svc uuid as string
            formatHexString2HexBytes((advDataSvcUUID.getValue()).upper(), advDataList)

            # get svcData as string
            formatHexString2HexBytes((advDataSvcData.getValue()).upper(), advDataList)

            advDataLength+=totalSvcDataLen+1
        else:
            advDataList.append("#InvalidServiceData")
            advDataLength+=1

    if advDataManuSpecDataEn.getValue() == True:
        if (len(advDataManuSpecDataCid.getValue()) == 4 and bleStackCheckHexFormat(advDataManuSpecDataCid.getValue()) and bleStackCheckHexFormat(advDataManuSpecData.getValue())):
            totalMsdLen=2+1
            totalMsdLen+=(len(advDataManuSpecData.getValue())/2)
            advDataList.append('0x{:02X}'.format(totalMsdLen))
            advDataList.append('0xff')

            # get svc uuid as string
            formatHexString2HexBytes((advDataManuSpecDataCid.getValue()).upper(), advDataList)

            # get Manufacturer data as string
            formatHexString2HexBytes((advDataManuSpecData.getValue()).upper(), advDataList)

            advDataLength+=totalMsdLen+1
        else:
            advDataList.append("#InvalidManufacturerData")
            advDataLength+=1

    if advDataUserDefEn.getValue() == True:
        # Only genetate User Defined Data if format is correct
        if (bleStackCheckHexFormat(advDataUserDef.getValue())):
            totalUsdLen=(len(advDataUserDef.getValue())/2)
            formatHexString2HexBytes((advDataUserDef.getValue()).upper(), advDataList)
            advDataLength+=totalUsdLen
        else:
            advDataList.append("#InvalidUserDefinedData")
            advDataLength+=1


    #for e in advDataList:
    #    print('advDataList: {}'.format(e))

    #clear data list if data length exceeds the limit
    if (advDataLength > MAX_ADV_DATA_LEN):
        advDataList[:] = []
        advDataLength = 1
        advDataList.append("#ExceedMaxDataLength_31BYTES")

    symbol.setValue(', '.join(advDataList))
    advertisingDataLen.setValue(advDataLength)
    advertisingDataValue.setLabel(symbol.getValue())
    #print('gapSetAdvData advData = {}'.format(symbol.getValue()))
    #print('gapSetAdvData advDataLength = {}'.format(advDataLength))

def gapSetAdvData(symbol, event):
    #print('gapSetAdvData symbol.getType = ' + symbol.getType())
    #print('gapSetAdvData event.id       = ' + event["id"])
    #print('gapSetAdvData event.value    = ' + str(event["value"]))

    gapGenAdvData(symbol)

global gapGenScanRspData
def gapGenScanRspData(symbol):
    scanRspDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_LOCAL_NAME_EN")
    scanRspDataLocalName = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_LOCAL_NAME")
    scanRspDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_SERVICE_DATA_EN")
    scanRspDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_SERVICE_UUID")
    scanRspDataSvcData = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_SERVICE_DATA")
    scanRspDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_MSD_EN")
    scanRspDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_MSD")
    scanRspDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_MSD_CID")
    scanRspDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_UDD_EN")
    scanRspDataUserDef = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_UDD")
    scanRspDataValue = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_VALUE")
    scanRspDataLen = symbol.getComponent().getSymbolByID("GAP_SCAN_RSP_DATA_LEN")

    # build the GAP_SCAN_RSP_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type
            xx..n   - data
    '''
    scanRspDataList = []        # array of formatted hex bytes
    scanRspDataLength = 0       # total number of bytes of scanRspDataList
    MAX_SCAN_RSP_DATA_LEN = 31  # maximum number of bytes of scanRspDataList

    if scanRspDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(scanRspDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        scanRspDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to scanRspDataList
        scanRspDataList.append('0x{:02X}'.format(localNameHexLen))

        scanRspDataList.append('0x09')  # some hex key
        scanRspDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            scanRspDataList.append('0x' + localNameHex[byte:byte+2])

    if scanRspDataSvcDataEn.getValue() == True:
        # Only genetate Service Data if format is correct
        if (len(scanRspDataSvcUUID.getValue()) == 4 and bleStackCheckHexFormat(scanRspDataSvcUUID.getValue()) and bleStackCheckHexFormat(scanRspDataSvcData.getValue())):
            totalSvcDataLen=2+1
            totalSvcDataLen+=(len(scanRspDataSvcData.getValue())/2)
            scanRspDataList.append('0x{:02X}'.format(totalSvcDataLen))
            scanRspDataList.append('0x16')

            # get svc uuid as string
            formatHexString2HexBytes((scanRspDataSvcUUID.getValue()).upper(), scanRspDataList)

            # get svcData as string
            formatHexString2HexBytes((scanRspDataSvcData.getValue()).upper(), scanRspDataList)

            scanRspDataLength+=totalSvcDataLen+1
        else:
            scanRspDataList.append("#InvalidServiceData")
            scanRspDataLength+=1

    if scanRspDataManuSpecDataEn.getValue() == True:
        if (len(scanRspDataManuSpecDataCid.getValue()) == 4 and bleStackCheckHexFormat(scanRspDataManuSpecDataCid.getValue()) and bleStackCheckHexFormat(scanRspDataManuSpecData.getValue())):
            totalMsdLen=2+1
            totalMsdLen+=(len(scanRspDataManuSpecData.getValue())/2)
            scanRspDataList.append('0x{:02X}'.format(totalMsdLen))
            scanRspDataList.append('0xff')

            # get svc uuid as string
            formatHexString2HexBytes((scanRspDataManuSpecDataCid.getValue()).upper(), scanRspDataList)

            # get Manufacturer data as string
            formatHexString2HexBytes((scanRspDataManuSpecData.getValue()).upper(), scanRspDataList)

            scanRspDataLength+=totalMsdLen+1
        else:
            scanRspDataList.append("#InvalidManufacturerData")
            scanRspDataLength+=1

    if scanRspDataUserDefEn.getValue() == True:
        # Only genetate User Defined Data if format is correct
        if (bleStackCheckHexFormat(scanRspDataUserDef.getValue())):
            totalUsdLen=(len(scanRspDataUserDef.getValue())/2)
            formatHexString2HexBytes((scanRspDataUserDef.getValue()).upper(), scanRspDataList)
            scanRspDataLength+=totalUsdLen
        else:
            scanRspDataList.append("#InvalidUserDefinedData")
            scanRspDataLength+=1

    #for e in scanRspDataList:
        #print('scanRspDataList: {}'.format(e))

    #clear data list if data length exceeds the limit
    if (scanRspDataLength > MAX_SCAN_RSP_DATA_LEN):
        scanRspDataList[:] = []
        scanRspDataLength = 1
        scanRspDataList.append("#ExceedMaxDataLength_31BYTES")

    symbol.setValue(', '.join(scanRspDataList))
    scanRspDataLen.setValue(scanRspDataLength)
    scanRspDataValue.setLabel(symbol.getValue())
    #print('gapSetScanRspData advData = {}'.format(symbol.getValue()))
    #print('gapSetScanRspData advDataLength = {}'.format(scanRspDataLength))

def gapSetScanRspData(symbol, event):
    #print('gapSetScanRspData symbol.getType = ' + symbol.getType())
    #print('gapSetScanRspData event.id       = ' + event["id"])
    #print('gapSetScanRspData event.value    = ' + str(event["value"]))

    gapGenScanRspData(symbol)

def gapExtScanPhy1mVisibility(symbol, event):
    global gapScanPhy
    symbol.setVisible(gapScanPhy.getValue() == 0 or gapScanPhy.getValue() == 2)

def gapExtScanPhyCodedVisibility(symbol, event):
    global gapScanPhy
    symbol.setVisible(gapScanPhy.getValue() == 1 or gapScanPhy.getValue() == 2)

def gapLegacyScanVisibility(symbol, event):
    global gapExtScanEn
    global gapScan
    symbol.setVisible(not gapExtScanEn.getValue() and gapScan.getValue())

global sendRTCSupportMessage
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
global sendDeepSleepEnableMessage
def sendDeepSleepEnableMessage(dsEnable):
    processor = Variables.get("__PROCESSOR")
    if( processor in pic32cx_bz2_family):
        Database.sendMessage("pic32cx_bz2_devsupport", "DEEP_SLEEP_ENABLE", {"isEnabled":dsEnable})
    else:
        Database.sendMessage("pic32cx_bz3_devsupport", "DEEP_SLEEP_ENABLE", {"isEnabled":dsEnable})


def deepSleepEnabledChange(symbol, event):
    global sysSleepEn
    global gapExtAdvEn
    global gapAdvertising
    dsadvEn = False

    if gapExtAdvEn.getValue()==False and gapAdvertising.getValue()==True:
        symbol.setVisible(True)
        if symbol.getValue()==True:
            dsadvEn = True
    else:
        symbol.setVisible(False)

    if dsadvEn == True:
        if sysSleepEn.getValue()==False:
            #print('BLEStack:deepSleepEnabledChange value={}'.format(str(value))) 
            sendRTCSupportMessage(True)

        sendDeepSleepEnableMessage(True)
    else:
        if sysSleepEn.getValue()==False:
            #print('BLEStack:deepSleepEnabledChange value={}'.format(str(value)))
            sendRTCSupportMessage(False)

        sendDeepSleepEnableMessage(False)

global getValueLen
def getValueLen(intValue):
    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    totalLength = (hexStrLen >> 1)    # length byte + hexStr length
    #print('hexStrlen = {}, totalLength = {}'.format(hexStrLen, totalLength))
    return totalLength
    
global formatHexString2HexBytes
def formatHexString2HexBytes(hexString, byteList):
    hexStringLength = len(hexString)

    for byte in range(0, hexStringLength, 2):
        byteList.append('0x' + hexString[byte:byte+2])
        
global formatInt2HexBytes
def formatInt2HexBytes(intValue, byteList, chkLen):

    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    if chkLen != 0 :
        if hexStrLen/2 < chkLen:
            pedLen = chkLen - hexStrLen/2
            for i in range(pedLen):
                hexStr = '0' + '0' + hexStr
                hexStrLen += 2
    # add length
    #print('hexStrlen = {}'.format(hexStrLen))

    # add hex Str hex bytes as formatted bytes to byteList
    for byte in range(0, hexStrLen, 2):
        byteList.append('0x' + hexStr[byte:byte+2])
        
global formatInt2HexBytesInvert
def formatInt2HexBytesInvert(intValue, byteList, chkLen):

    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    if chkLen != 0 :
        if hexStrLen/2 < chkLen:
            pedLen = chkLen - hexStrLen/2
            for i in range(pedLen):
                hexStr = '0' + '0' + hexStr
                hexStrLen += 2
    # add length
    #print('hexStrlen = {}'.format(hexStrLen))

    # add hex Str hex bytes as formatted bytes to byteList
    for byte in range(0, hexStrLen, 2):
        byteList.append('0x' + hexStr[hexStrLen-byte-2:hexStrLen-byte])
############################################################################
### GAP Settings ###
############################################################################

if devSup != None:
    if devSup.getSymbolValue('CUSTOM_ANT_ENABLE') == True:
        antennaGain = devSup.getSymbolValue('CUSTOM_ANT_GAIN')
    else:
        antennaGain = devSup.getSymbolByID('CUSTOM_ANT_GAIN').getDefaultValue()
gapGenTxPwrCap()


# GAP Menu
menuGAP = libBLEStackComponent.createMenuSymbol('GAP_MENU', None)
menuGAP.setLabel('Generic Access Profile (GAP)')
menuGAP.setVisible(True)
menuGAP.setDescription("GAP SETTINGS")


# Device Name
gapDeviceName = libBLEStackComponent.createStringSymbol("GAP_DEV_NAME", menuGAP)
gapDeviceName.setLabel("Device Name")
gapDeviceName.setDefaultValue("Microchip")
gapDeviceName.setVisible(True)
gapDeviceName.setReadOnly(False)


# Device Name Check Flag
gapDeviceNameCheck = libBLEStackComponent.createBooleanSymbol("GAP_DEV_NAME_CHECK", menuGAP)
gapDeviceNameCheck.setVisible(False)
gapDeviceNameCheck.setDefaultValue(True)
gapDeviceNameCheck.setDependencies(bleDevNameCheck, ["GAP_DEV_NAME"])


# Device Name Check Warning Message
gapDeviceNameComment = libBLEStackComponent.createCommentSymbol("GAP_DEV_NAME_CMT", menuGAP)
gapDeviceNameComment.setLabel("**** Device Name maximum is 32 bytes. ****")
gapDeviceNameComment.setVisible(False)
gapDeviceNameComment.setDependencies(bleDevNameCheckComment, ["GAP_DEV_NAME"])


# Device Name Write Property
gapSvc = libBLEStackComponent.createBooleanSymbol('GAP_SVC_DEV_NAME_WRITE', menuGAP)
gapSvc.setLabel('Enable Device Name Write Property')
gapSvc.setDescription('Enable to permit remote modify device name')
gapSvc.setDefaultValue(False)
gapSvc.setVisible(True)


# Appearance
gapSvc = libBLEStackComponent.createHexSymbol('GAP_SVC_APPEARANCE', menuGAP)
gapSvc.setLabel('Appearance')
gapSvc.setDescription('Please refer to https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf for valid value')
gapSvc.setDefaultValue(0x0000)
gapSvc.setMin(0x0000)
gapSvc.setMax(0xFFFF)
gapSvc.setVisible(True)


# Peripheral prefered connection parameters
gapSvc = libBLEStackComponent.createBooleanSymbol('GAP_SVC_PERI_PRE_CP', menuGAP)
gapSvc.setLabel('Enable Peripheral Preferred Connection Parameters')
gapSvc.setDefaultValue(False)
gapSvc.setVisible(True)


# Minimum Connection Interval
gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_MIN_CONN_INTERVAL", gapSvc)
gapSvcPeri.setLabel("Minimum Connection Interval(Unit: 1.25ms)")
gapSvcPeri.setDescription("Range: 6 to 3200. Value of 0xFFFF indicates no specific minimum.")
gapSvcPeri.setDefaultValue(32)
gapSvcPeri.setMin(6)
gapSvcPeri.setMax(65535)
gapSvcPeri.setVisible(False)
gapSvcPeri.setDependencies(gapConfigVisibility, ["GAP_SVC_PERI_PRE_CP"])


# Maximum Connection Interval
gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_MAX_CONN_INTERVAL", gapSvc)
gapSvcPeri.setLabel("Maximum Connection Interval(Unit: 1.25ms)")
gapSvcPeri.setDescription("Range: 6 to 3200. Value of 0xFFFF indicates no specific maximum.")
gapSvcPeri.setDefaultValue(32)
gapSvcPeri.setMin(6)
gapSvcPeri.setMax(65535)
gapSvcPeri.setVisible(False)
gapSvcPeri.setDependencies(gapConfigVisibility, ["GAP_SVC_PERI_PRE_CP"])


# Peripheral latency
gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_PERI_LATENCY", gapSvc)
gapSvcPeri.setLabel("Peripheral Latency")
gapSvcPeri.setDefaultValue(0)
gapSvcPeri.setMin(0)
gapSvcPeri.setMax(0x01F3)
gapSvcPeri.setVisible(False)
gapSvcPeri.setDependencies(gapConfigVisibility, ["GAP_SVC_PERI_PRE_CP"])


# Connection Supervision timeout multipilier
gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT", gapSvc)
gapSvcPeri.setLabel("Connection Supervision Timeout Multiplier(Unit: 10ms)")
gapSvcPeri.setDescription("Range: 10 to 3200. 65535 indicates no specific value requested.")
gapSvcPeri.setDefaultValue(600)
gapSvcPeri.setMin(0xA)
gapSvcPeri.setMax(0xFFFF)
gapSvcPeri.setVisible(False)
gapSvcPeri.setDependencies(gapConfigVisibility, ["GAP_SVC_PERI_PRE_CP"])


# Privacy
gapPrivacy = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_PRIVACY', menuGAP)
gapPrivacy.setLabel('Enable Privacy')
gapPrivacy.setDefaultValue(False)
gapPrivacy.setVisible(True)


# Privacy Address Type
gapPriAddrType = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRIV_ADDR_TYPE', gapPrivacy)
gapPriAddrType.setLabel('Address Type')
gapPriAddrType.addKey('BLE_GAP_ADDR_TYPE_RANDOM_RESOLVABLE', '0', 'Resolvable Random Address')
gapPriAddrType.addKey('BLE_GAP_ADDR_TYPE_RANDOM_NON_RESOLVABLE', '1', 'Non-resolvable Random Address')
gapPriAddrType.setDefaultValue(0)
gapPriAddrType.setVisible(False)
gapPriAddrType.setOutputMode('Key')
gapPriAddrType.setDisplayMode('Description')
gapPriAddrType.setDescription("Privacy address type")
gapPriAddrType.setDependencies(gapConfigVisibility, ["BOOL_GAP_PRIVACY"])


# Privacy Address Timeout
gapPriAddrTimeout = libBLEStackComponent.createIntegerSymbol('GAP_PRIV_ADDR_TO', gapPrivacy)
gapPriAddrTimeout.setLabel("Address Timeout(Unit: 1sec)")
gapPriAddrTimeout.setDefaultValue(900)
gapPriAddrTimeout.setMax(65535)
gapPriAddrTimeout.setMin(1)
gapPriAddrTimeout.setVisible(False)
gapPriAddrTimeout.setDescription("Privacy address timeout")
gapPriAddrTimeout.setDependencies(gapConfigVisibility, ["BOOL_GAP_PRIVACY"])


# Advertising
global gapAdvertising
gapAdvertising = libBLEStackComponent.createBooleanSymbol('GAP_ADVERTISING', menuGAP)
gapAdvertising.setLabel('Advertising')
gapAdvertising.setDefaultValue(True)
gapAdvertising.setVisible(True)
gapAdvertising.setDescription("Advertising Configuration")


# Extended Advertising
global gapExtAdvEn
gapExtAdvEn = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_EXT_ADV', gapAdvertising)
gapExtAdvEn.setLabel('Enable Extended Advertising')
gapExtAdvEn.setDefaultValue(False)
gapExtAdvEn.setVisible(True)
gapExtAdvEn.setDescription("Enable Extended Advertising")
gapExtAdvEn.setDependencies(gapConfigVisibility, ["GAP_ADVERTISING"])


# Advertising Tx Power
gapAdvTxPwr = libBLEStackComponent.createIntegerSymbol('GAP_ADV_TX_PWR', gapAdvertising)
gapAdvTxPwr.setLabel('Advertising TX Power (dBm)')
gapAdvTxPwr.setDefaultValue(gapGenDefTxPwr(True))
gapAdvTxPwr.setMax(gapGenMaxTxPwr(True))
gapAdvTxPwr.setMin(gapGenMinTxPwr())
gapAdvTxPwr.setVisible(True)
gapAdvTxPwr.setDescription("Advertising TX Power")
gapAdvTxPwr.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Interval Min
gapAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_ADV_INTERVAL_MIN", gapAdvertising)
gapAdvIntMin.setLabel("Advertising Interval Min(Unit: 0.625ms)")
gapAdvIntMin.setDefaultValue(32)
gapAdvIntMin.setMin(1)
gapAdvIntMin.setMax(65535)
gapAdvIntMin.setVisible(True)
gapAdvIntMin.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Interval Max
gapAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_ADV_INTERVAL_MAX", gapAdvertising)
gapAdvIntMax.setLabel("Advertising Interval Max(Unit: 0.625ms)")
gapAdvIntMax.setDefaultValue(32)
gapAdvIntMax.setMin(1)
gapAdvIntMax.setMax(65535)
gapAdvIntMax.setVisible(True)
gapAdvIntMax.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Type
gapAdvType = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_TYPE', gapAdvertising)
gapAdvType.setLabel('Advertising Type')
gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_IND', '0', 'Connectable and scannable undirected advertising')
gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_SCAN_IND', '2', 'Scannable undirected advertising')
gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_NONCONN_IND', '3', 'Non connectable undirected advertising')
gapAdvType.setDefaultValue(0)
gapAdvType.setVisible(True)
gapAdvType.setOutputMode('Key')
gapAdvType.setDisplayMode('Description')
gapAdvType.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Channel Map
gapAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_CHANNEL_MAP', gapAdvertising)
gapAdvChannelMap.setLabel('Advertising Channel Map')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
gapAdvChannelMap.setDefaultValue(0)
gapAdvChannelMap.setVisible(True)
gapAdvChannelMap.setOutputMode('Key')
gapAdvChannelMap.setDisplayMode('Description')
gapAdvChannelMap.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Filter Policy
gapAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_FILT_POLICY', gapAdvertising)
gapAdvFiltPolicy.setLabel('Advertising Filter Policy')
gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
gapAdvFiltPolicy.setDefaultValue(0)
gapAdvFiltPolicy.setOutputMode('Key')
gapAdvFiltPolicy.setDisplayMode('Description')
gapAdvFiltPolicy.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Set
gapExtAdvAdvSet = libBLEStackComponent.createMenuSymbol("GAP_EXT_ADV_ADV_SET", gapExtAdvEn)
gapExtAdvAdvSet.setLabel("Advertising Set 1")
gapExtAdvAdvSet.setVisible(False)
gapExtAdvAdvSet.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_ADV"])


# Advertising Handle
gapExtAdvAdvHandle = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_ADV_SET_HANDLE", gapExtAdvAdvSet)
gapExtAdvAdvHandle.setLabel("Advertising Handle")
gapExtAdvAdvHandle.setDefaultValue(0x00)
gapExtAdvAdvHandle.setMin(0x00)
gapExtAdvAdvHandle.setMax(0xEF)
gapExtAdvAdvHandle.setVisible(False)
gapExtAdvAdvHandle.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_ADV"])


# Advertising Event Properties
gapExtAdvEvtProp = libBLEStackComponent.createKeyValueSetSymbol("GAP_EXT_ADV_EVT_PROPERTIES", gapExtAdvAdvSet)
gapExtAdvEvtProp.setLabel("Advertising Event Properties")
gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '0', 'Connectable and scannable undirected advertising')
gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '1', 'Scannable undirected advertising')
gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '2', 'Non-connectable and non-scannable undirected advertising')
gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV', '3', 'Extended connectable undirected advertising')
gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV', '4', 'Extended scannable undirected advertising')
gapExtAdvEvtProp.addKey('0', '5', 'Extended non-connectable and non-scannable undirected advertising')
gapExtAdvEvtProp.setDefaultValue(0)
gapExtAdvEvtProp.setOutputMode('Value')
gapExtAdvEvtProp.setDisplayMode('Description')


# Advertising Event Properties - Anonymous advertising
gapExtAdvEvtPropAnon = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_ANON', gapExtAdvEvtProp)
gapExtAdvEvtPropAnon.setLabel('Anonymous advertising')
gapExtAdvEvtPropAnon.setDefaultValue(False)
gapExtAdvEvtPropAnon.setVisible(not gapExtAdvEvtProp.getDefaultValue() < 3)
gapExtAdvEvtPropAnon.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES"])


# Advertising Event Properties - Tx Power
gapExtAdvEvtPropTxPower = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_TX_POWER', gapExtAdvEvtProp)
gapExtAdvEvtPropTxPower.setLabel('Include TxPower in the extended header')
gapExtAdvEvtPropTxPower.setDefaultValue(False)
gapExtAdvEvtPropTxPower.setVisible(not gapExtAdvEvtProp.getDefaultValue() < 3)
gapExtAdvEvtPropTxPower.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES"])


# Primary Advertising Interval Min
gapExtAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MIN", gapExtAdvAdvSet)
gapExtAdvIntMin.setLabel("Primary Advertising Interval Min(Unit: 0.625ms)")
gapExtAdvIntMin.setDefaultValue(0x20)
gapExtAdvIntMin.setMin(0x20)
gapExtAdvIntMin.setMax(0xFFFFFF)


# Primary Advertising Interval Max
gapExtAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MAX", gapExtAdvAdvSet)
gapExtAdvIntMax.setLabel("Primary Advertising Interval Max(Unit: 0.625ms)")
gapExtAdvIntMax.setDefaultValue(0x20)
gapExtAdvIntMax.setMin(0x20)
gapExtAdvIntMax.setMax(0xFFFFFF)


# Primary Advertising Channel Map
gapExtAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_CHANNEL_MAP', gapExtAdvAdvSet)
gapExtAdvChannelMap.setLabel('Primary Advertising Channel Map')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
gapExtAdvChannelMap.setDefaultValue(0)
gapExtAdvChannelMap.setOutputMode('Key')
gapExtAdvChannelMap.setDisplayMode('Description')


# Advertising Filter Policy
gapExtAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_FILT_POLICY', gapExtAdvAdvSet)
gapExtAdvFiltPolicy.setLabel('Advertising Filter Policy')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
gapExtAdvFiltPolicy.setDefaultValue(0)
gapExtAdvFiltPolicy.setOutputMode('Key')
gapExtAdvFiltPolicy.setDisplayMode('Description')


# Advertising Tx Power
gapExtAdvTxPower = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_TX_POWER", gapExtAdvAdvSet)
gapExtAdvTxPower.setLabel("Advertising TX Power(Unit: dBm)")
gapExtAdvTxPower.setDefaultValue(gapGenDefTxPwr(gapExtAdvEvtProp.getValue() < 3))
gapExtAdvTxPower.setMin(gapGenMinTxPwr())
gapExtAdvTxPower.setMax(gapGenMaxTxPwr(gapExtAdvEvtProp.getValue() < 3))
gapExtAdvTxPower.setDependencies(gapExtAdvTxPwr, ["GAP_EXT_ADV_EVT_PROPERTIES"])


# Primary Advertising PHY
gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_ADV_PHY', gapExtAdvAdvSet)
gapExtAdvPhy.setLabel('Primary Advertising PHY')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '1', 'LE Coded')
gapExtAdvPhy.setDefaultValue(0)
gapExtAdvPhy.setOutputMode('Key')
gapExtAdvPhy.setDisplayMode('Description')


# Secondary Advertising Max Skip
gapExtAdvSecMaxSkip = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_MAX_SKIP", gapExtAdvAdvSet)
gapExtAdvSecMaxSkip.setLabel("Secondary Advertising Max Skip")
gapExtAdvSecMaxSkip.setDefaultValue(0)
gapExtAdvSecMaxSkip.setMin(0)
gapExtAdvSecMaxSkip.setMax(0xFF)


# Secondary Advertising PHY
gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SEC_ADV_PHY', gapExtAdvAdvSet)
gapExtAdvPhy.setLabel('Secondary Advertising PHY')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_2M', '1', 'LE 2M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '2', 'LE Coded')
gapExtAdvPhy.setDefaultValue(0)
gapExtAdvPhy.setOutputMode('Key')
gapExtAdvPhy.setDisplayMode('Description')


# Advertising SID
gapExtAdvSid = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_SID", gapExtAdvAdvSet)
gapExtAdvSid.setLabel("Advertising SID")
gapExtAdvSid.setDefaultValue(0)
gapExtAdvSid.setMin(0)
gapExtAdvSid.setMax(0x0F)


# Scan Request Notification Enable
gapExtAdvScanEnable = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_SCAN_ENABLE', gapExtAdvAdvSet)
gapExtAdvScanEnable.setLabel('Scan Request Notification Enable')
gapExtAdvScanEnable.setDefaultValue(False)


# Ext Advertising Data Menu
menuExtAdvData = libBLEStackComponent.createMenuSymbol('GAP_EXT_ADV_DATA_MENU', gapExtAdvAdvSet)
menuExtAdvData.setLabel('Advertising Data')
menuExtAdvData.setDescription("Advertising Data")


# Ext Advertising Data-Flag Bool
gapExtAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_FLAG_EN', menuExtAdvData)
gapExtAdvDataFlagEn.setLabel('Flag')
gapExtAdvDataFlagEn.setDefaultValue(True)
gapExtAdvDataFlagEn.setDescription("Flag")


# Ext Advertising Data-Flag
gapExtAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_DATA_FLAG', gapExtAdvDataFlagEn)
gapExtAdvDataFlag.setLabel(' ')
gapExtAdvDataFlag.addKey('0x05', '0', 'LE Limited Discoverable Mode')
gapExtAdvDataFlag.addKey('0x06', '1', 'LE General Discoverable Mode')
gapExtAdvDataFlag.setDefaultValue(0)
gapExtAdvDataFlag.setOutputMode('Key')
gapExtAdvDataFlag.setDisplayMode('Description')
gapExtAdvDataFlag.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_FLAG_EN"])


# Ext Advertising Data-Local Name Bool
gapExtAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_LOCAL_NAME_EN', menuExtAdvData)
#print('libBLEStackComponent:gapExtAdvDataLocalNameEn type({})'.format(type(gapExtAdvDataLocalNameEn)))
gapExtAdvDataLocalNameEn.setLabel('Local Name')
gapExtAdvDataLocalNameEn.setDefaultValue(True)
gapExtAdvDataLocalNameEn.setDescription("Local Name")


# Ext Advertising Data-Local Name
gapExtAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_LOCAL_NAME", gapExtAdvDataLocalNameEn)
gapExtAdvDataLocalName.setLabel(" ")
gapExtAdvDataLocalName.setDefaultValue("pic32cx-bz")
gapExtAdvDataLocalName.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_LOCAL_NAME_EN"])


# Ext Advertising Data-Service Data Bool
gapExtAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_SERVICE_DATA_EN', menuExtAdvData)
gapExtAdvDataSvcDataEn.setLabel('Service Data')
gapExtAdvDataSvcDataEn.setDefaultValue(False)
gapExtAdvDataSvcDataEn.setDescription("Service Data")


# Ext Advertising Data-Service UUID
gapExtAdvDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcUUID.setLabel("Service UUID")
gapExtAdvDataSvcUUID.setDescription("Service UUID")
gapExtAdvDataSvcUUID.setDefaultValue("DAFE")
gapExtAdvDataSvcUUID.setVisible(False)
gapExtAdvDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])


# Ext Advertising Data-Service UUID Warning Message
gapExtAdvDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID_CMT", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB). Invalid value will result in compiling error! ****")
gapExtAdvDataSvcUUIDComment.setVisible(False)
gapExtAdvDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])


# Ext Advertising Data- Service Data
gapExtAdvDataSvcData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcData.setLabel("Service Data")
gapExtAdvDataSvcData.setDescription("Service Data")
gapExtAdvDataSvcData.setDefaultValue("00")
gapExtAdvDataSvcData.setVisible(False)
gapExtAdvDataSvcData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])


# Ext Advertising Data- Service Data Warning Message
gapExtAdvDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA_CMT", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataSvcDataComment.setVisible(False)
gapExtAdvDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])


# Ext Advertising Data-Manufacture Specific Data Bool
gapExtAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_MSD_EN', menuExtAdvData)
gapExtAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapExtAdvDataManuSpecDataEn.setDefaultValue(False)
gapExtAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")


# Ext Advertising Data-Manufacture Specific Data Company Id
gapExtAdvDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_MSD_CID", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataCid.setLabel("Company Id")
gapExtAdvDataManuSpecDataCid.setDefaultValue("0001")
gapExtAdvDataManuSpecDataCid.setVisible(False)
gapExtAdvDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])


# Ext Advertising Data-Manufacture Specific Data Company Id Warning Message
gapExtAdvDataManuSpecDataCidComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_MSD_CID_CMT", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataCidComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataManuSpecDataCidComment.setVisible(False)
gapExtAdvDataManuSpecDataCidComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])


# Ext Advertising Data-Manufacture Specific Data
gapExtAdvDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_MSD", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecData.setLabel("Manufacturer Data")
gapExtAdvDataManuSpecData.setDescription("Manufacturer Data")
gapExtAdvDataManuSpecData.setDefaultValue("00")
gapExtAdvDataManuSpecData.setVisible(False)
gapExtAdvDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])


# Ext Advertising Data-Manufacture Specific Data Warning Message
gapExtAdvDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_MSD_CMT", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataManuSpecDataComment.setVisible(False)
gapExtAdvDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])


# Ext Advertising Data-User Defined Data Bool
gapExtAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_UDD_EN', menuExtAdvData)
gapExtAdvDataUserDefEn.setLabel('User Defined Data')
gapExtAdvDataUserDefEn.setDefaultValue(False)


# Ext Advertising Data-User Defined Data
gapExtAdvDataUserDef = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_UDD", gapExtAdvDataUserDefEn)
gapExtAdvDataUserDef.setLabel("User Defined Data")
gapExtAdvDataUserDef.setDescription("User Defined Data")
gapExtAdvDataUserDef.setDefaultValue("00")
gapExtAdvDataUserDef.setVisible(False)
gapExtAdvDataUserDef.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_UDD_EN"])


# Ext Advertising Data-User Defined Data Warning Message
gapExtAdvDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_UDD_CMT", gapExtAdvDataUserDefEn)
gapExtAdvDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataUserDefComment.setVisible(False)
gapExtAdvDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_UDD_EN"])


# Ext Advertising Data
gapExtAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA", gapExtAdvAdvSet)
gapExtAdvertisingData.setLabel("Advertising Data")
gapExtAdvertisingData.setDefaultValue("Advertising Data")
gapExtAdvertisingData.setVisible(False)
gapExtAdvertisingData.setDependencies(gapSetExtAdvData, ["GAP_EXT_ADV_DATA_FLAG_EN", "GAP_EXT_ADV_DATA_FLAG",
                                                   "GAP_EXT_ADV_DATA_LOCAL_NAME_EN", "GAP_EXT_ADV_DATA_LOCAL_NAME",
                                                   "GAP_EXT_ADV_DATA_SERVICE_DATA_EN",
                                                   "GAP_EXT_ADV_DATA_SERVICE_UUID",
                                                   "GAP_EXT_ADV_DATA_SERVICE_DATA",
                                                   "GAP_EXT_ADV_DATA_MSD_EN", "GAP_EXT_ADV_DATA_MSD", "GAP_EXT_ADV_DATA_MSD_CID",
                                                   "GAP_EXT_ADV_DATA_UDD_EN", "GAP_EXT_ADV_DATA_UDD", ])


# Ext Advertising Data Length
gapExtAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_DATA_LEN", gapExtAdvAdvSet)
gapExtAdvertisingDataLen.setLabel("Advertising Data Len")
gapExtAdvertisingDataLen.setDefaultValue(0)
gapExtAdvertisingDataLen.setVisible(False)


# Ext Advertising Data Value
gapExtAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_VALUE", gapExtAdvAdvSet)
gapExtAdvertisingDataValue.setVisible(False)
gapGenExtAdvData(gapExtAdvertisingData)


# Ext Scan Response Data
menuExtScanRspData = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_RSP_DATA_MENU', gapExtAdvAdvSet)
menuExtScanRspData.setLabel('Scan Response Data')
menuExtScanRspData.setDescription("Scan Response Data")


# Ext Scan Response Data-Local Name Bool
gapExtScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN', menuExtScanRspData)
gapExtScanRspDataLocalNameEn.setLabel('Local Name')
gapExtScanRspDataLocalNameEn.setDefaultValue(True)
gapExtScanRspDataLocalNameEn.setDescription("Local Name")


# Ext Scan Response Data-Local Name
gapExtScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME", gapExtScanRspDataLocalNameEn)
gapExtScanRspDataLocalName.setLabel(" ")
gapExtScanRspDataLocalName.setDefaultValue("pic32cx-bz")
gapExtScanRspDataLocalName.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN"])


# Ext Scan Response Data-Service Data Bool
gapExtScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN', menuExtScanRspData)
gapExtScanRspDataSvcDataEn.setLabel('Service Data')
gapExtScanRspDataSvcDataEn.setDefaultValue(False)
gapExtScanRspDataSvcDataEn.setDescription("Service Data")


# Ext Scan Response Data-Service UUID
gapExtScanRspDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcUUID.setLabel("Service UUID")
gapExtScanRspDataSvcUUID.setDescription("Service UUID")
gapExtScanRspDataSvcUUID.setDefaultValue("DAFE")
gapExtScanRspDataSvcUUID.setVisible(False)
gapExtScanRspDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Ext Scan Response Data-Service UUID Warning Message
gapExtScanRspDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_CMT", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB).Invalid value will result in compiling error! ****")
gapExtScanRspDataSvcUUIDComment.setVisible(False)
gapExtScanRspDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Ext Scan Response Data- Service Data
gapExtScanRspDataSvcData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcData.setLabel("Service Data")
gapExtScanRspDataSvcData.setDescription("Service Data")
gapExtScanRspDataSvcData.setDefaultValue("00")
gapExtScanRspDataSvcData.setVisible(False)
gapExtScanRspDataSvcData.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Ext Scan Response Data- Service Data Warning Message
gapExtScanRspDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_CMT", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataSvcDataComment.setVisible(False)
gapExtScanRspDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Ext Scan Response Data-Manufacture Specific Data Bool
gapExtScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_MSD_EN', menuExtScanRspData)
gapExtScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapExtScanRspDataManuSpecDataEn.setDefaultValue(False)
gapExtScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")


# Ext Scan Response Data-Manufacture Specific Data Company Id
gapExtScanRspDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CID", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecDataCid.setLabel("Company Id")
gapExtScanRspDataManuSpecDataCid.setDefaultValue("0001")
gapExtScanRspDataManuSpecDataCid.setVisible(False)
gapExtScanRspDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])


# Ext Scan Response Data-Manufacture Specific Data Company Id Warning Message
gapExtScanRspDataManuSpecDataCidComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CID_CMT", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecDataCidComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataManuSpecDataCidComment.setVisible(False)
gapExtScanRspDataManuSpecDataCidComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])


# Ext Scan Response Data-Manufacture Specific Data
gapExtScanRspDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_MSD", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecData.setLabel("Manufacture Data")
gapExtScanRspDataManuSpecData.setDescription("Manufacturer Data")
gapExtScanRspDataManuSpecData.setDefaultValue("00")
gapExtScanRspDataManuSpecData.setVisible(False)
gapExtScanRspDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])


# Ext Scan Response Data-Manufacture Specific Data Warning Message
gapExtScanRspDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CMT", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataManuSpecDataComment.setVisible(False)
gapExtScanRspDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])


# Ext Scan Response Data-User Defined Data Bool
gapExtScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_UDD_EN', menuExtScanRspData)
gapExtScanRspDataUserDefEn.setLabel('User Defined Data')
gapExtScanRspDataUserDefEn.setDefaultValue(False)


# Ext Scan Response Data-User Defined Data
gapExtScanRspDataUserDef = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_UDD", gapExtScanRspDataUserDefEn)
gapExtScanRspDataUserDef.setLabel("User Defined Data")
gapExtScanRspDataUserDef.setDescription("User Defined Data")
gapExtScanRspDataUserDef.setDefaultValue("00")
gapExtScanRspDataUserDef.setVisible(False)
gapExtScanRspDataUserDef.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN"])


# Ext Scan Response Data-User Defined Data Warning Message
gapExtScanRspDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_UDD_CMT", gapExtScanRspDataUserDefEn)
gapExtScanRspDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataUserDefComment.setVisible(False)
gapExtScanRspDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN"])


# Ext Scan Response Data
gapExtScanRspData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA", gapExtAdvAdvSet)
gapExtScanRspData.setLabel("Scan Response Data")
gapExtScanRspData.setVisible(False)
gapExtScanRspData.setDependencies(gapSetExtScanRspData, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN","GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME",
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN",
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID",
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA",
                                                   "GAP_EXT_SCAN_RSP_DATA_MSD_EN", "GAP_EXT_SCAN_RSP_DATA_MSD", "GAP_EXT_SCAN_RSP_DATA_MSD_CID",
                                                   "GAP_EXT_SCAN_RSP_DATA_UDD_EN", "GAP_EXT_SCAN_RSP_DATA_UDD", ])


# Ext Scan Response Data Length
gapExtScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_RSP_DATA_LEN", gapExtAdvAdvSet)
gapExtScanRspDataLen.setLabel("Scan Response Data Len")
gapExtScanRspDataLen.setDefaultValue(0)
gapExtScanRspDataLen.setVisible(False)


# Ext Scan Response Data Value
gapExtScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_VALUE", gapExtAdvAdvSet)
gapExtScanRspDataValue.setVisible(False)

# Generate Extended Scan Response Data
gapGenExtScanRspData(gapExtScanRspData)


# Advertising Set 2
gapExtAdvAdvSet2 = libBLEStackComponent.createBooleanSymbol("GAP_EXT_ADV_ADV_SET_2", gapExtAdvEn)
gapExtAdvAdvSet2.setLabel("Advertising Set 2")
gapExtAdvAdvSet2.setDefaultValue(False)
gapExtAdvAdvSet2.setVisible(False)
gapExtAdvAdvSet2.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_ADV"])


# Advertising Handle
gapExtAdvAdvHandle = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_ADV_SET_HANDLE_2", gapExtAdvAdvSet2)
gapExtAdvAdvHandle.setLabel("Advertising Handle")
gapExtAdvAdvHandle.setDefaultValue(0x01)
gapExtAdvAdvHandle.setMin(0x00)
gapExtAdvAdvHandle.setMax(0xEF)
gapExtAdvAdvHandle.setVisible(False)
gapExtAdvAdvHandle.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Advertising Event Properties
gapExtAdvEvtProp2 = libBLEStackComponent.createKeyValueSetSymbol("GAP_EXT_ADV_EVT_PROPERTIES_2", gapExtAdvAdvSet2)
gapExtAdvEvtProp2.setLabel("Advertising Event Properties")
gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '0', 'Connectable and scannable undirected advertising')
gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '1', 'Scannable undirected advertising')
gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '2', 'Non-connectable and non-scannable undirected advertising')
gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV', '3', 'Extended connectable undirected advertising')
gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV', '4', 'Extended scannable undirected advertising')
gapExtAdvEvtProp2.addKey('0', '5', 'Extended non-connectable and non-scannable undirected advertising')
gapExtAdvEvtProp2.setDefaultValue(0)
gapExtAdvEvtProp2.setVisible(False)
gapExtAdvEvtProp2.setOutputMode('Value')
gapExtAdvEvtProp2.setDisplayMode('Description')
gapExtAdvEvtProp2.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Advertising Event Properties - Anonymous advertising
gapExtAdvEvtPropAnon = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_ANON_2', gapExtAdvEvtProp2)
gapExtAdvEvtPropAnon.setLabel('Anonymous advertising')
gapExtAdvEvtPropAnon.setDefaultValue(False)
gapExtAdvEvtPropAnon.setVisible(not gapExtAdvEvtProp2.getDefaultValue() < 3)
gapExtAdvEvtPropAnon.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES_2"])


# Advertising Event Properties - Tx Power
gapExtAdvEvtPropTxPower = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_TX_POWER_2', gapExtAdvEvtProp2)
gapExtAdvEvtPropTxPower.setLabel('Include Tx Power in the extended header')
gapExtAdvEvtPropTxPower.setDefaultValue(False)
gapExtAdvEvtPropTxPower.setVisible(not gapExtAdvEvtProp2.getDefaultValue() < 3)
gapExtAdvEvtPropTxPower.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES_2"])


# Primary Advertising Interval Min
gapExtAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MIN_2", gapExtAdvAdvSet2)
gapExtAdvIntMin.setLabel("Primary Advertising Interval Min(Unit: 0.625ms)")
gapExtAdvIntMin.setDefaultValue(0x20)
gapExtAdvIntMin.setMin(0x20)
gapExtAdvIntMin.setMax(0xFFFFFF)
gapExtAdvIntMin.setVisible(False)
gapExtAdvIntMin.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Primary Advertising Interval Max
gapExtAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MAX_2", gapExtAdvAdvSet2)
gapExtAdvIntMax.setLabel("Primary Advertising Interval Max(Unit: 0.625ms)")
gapExtAdvIntMax.setDefaultValue(0x20)
gapExtAdvIntMax.setMin(0x20)
gapExtAdvIntMax.setMax(0xFFFFFF)
gapExtAdvIntMax.setVisible(False)
gapExtAdvIntMax.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Primary Advertising Channel Map
gapExtAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_CHANNEL_MAP_2', gapExtAdvAdvSet2)
gapExtAdvChannelMap.setLabel('Primary Advertising Channel Map')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
gapExtAdvChannelMap.setDefaultValue(0)
gapExtAdvChannelMap.setVisible(False)
gapExtAdvChannelMap.setOutputMode('Key')
gapExtAdvChannelMap.setDisplayMode('Description')
gapExtAdvChannelMap.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Advertising Filter Policy
gapExtAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_FILT_POLICY_2', gapExtAdvAdvSet2)
gapExtAdvFiltPolicy.setLabel('Advertising Filter Policy')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
gapExtAdvFiltPolicy.setDefaultValue(0)
gapExtAdvFiltPolicy.setVisible(False)
gapExtAdvFiltPolicy.setOutputMode('Key')
gapExtAdvFiltPolicy.setDisplayMode('Description')
gapExtAdvFiltPolicy.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

# Advertising Tx Power
gapExtAdvTxPower = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_TX_POWER_2", gapExtAdvAdvSet2)
gapExtAdvTxPower.setLabel("Advertising TX Power(Unit: dBm)")
gapExtAdvTxPower.setDefaultValue(gapGenDefTxPwr(gapExtAdvEvtProp2.getValue() < 3))
gapExtAdvTxPower.setMin(gapGenMinTxPwr())
gapExtAdvTxPower.setMax(gapGenMaxTxPwr(gapExtAdvEvtProp2.getValue() < 3))
gapExtAdvTxPower.setVisible(False)
gapExtAdvTxPower.setDependencies(gapExtAdvTxPwr, ["GAP_EXT_ADV_ADV_SET_2", "GAP_EXT_ADV_EVT_PROPERTIES_2"])


# Primary Advertising PHY
gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_ADV_PHY_2', gapExtAdvAdvSet2)
gapExtAdvPhy.setLabel('Primary Advertising PHY')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '1', 'LE Coded')
gapExtAdvPhy.setDefaultValue(0)
gapExtAdvPhy.setVisible(False)
gapExtAdvPhy.setOutputMode('Key')
gapExtAdvPhy.setDisplayMode('Description')
gapExtAdvPhy.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Secondary Advertising Max Skip
gapExtAdvSecMaxSkip = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_MAX_SKIP_2", gapExtAdvAdvSet2)
gapExtAdvSecMaxSkip.setLabel("Secondary Advertising Max Skip")
gapExtAdvSecMaxSkip.setDefaultValue(0)
gapExtAdvSecMaxSkip.setMin(0)
gapExtAdvSecMaxSkip.setMax(0xFF)
gapExtAdvSecMaxSkip.setVisible(False)
gapExtAdvSecMaxSkip.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Secondary Advertising PHY
gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SEC_ADV_PHY_2', gapExtAdvAdvSet2)
gapExtAdvPhy.setLabel('Secondary Advertising PHY')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_2M', '1', 'LE 2M')
gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '2', 'LE Coded')
gapExtAdvPhy.setDefaultValue(0)
gapExtAdvPhy.setVisible(False)
gapExtAdvPhy.setOutputMode('Key')
gapExtAdvPhy.setDisplayMode('Description')
gapExtAdvPhy.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Advertising SID
gapExtAdvSid = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_SID_2", gapExtAdvAdvSet2)
gapExtAdvSid.setLabel("Advertising SID")
gapExtAdvSid.setDefaultValue(0)
gapExtAdvSid.setMin(0)
gapExtAdvSid.setMax(0x0F)
gapExtAdvSid.setVisible(False)
gapExtAdvSid.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Scan Request Notification Enable
gapExtAdvScanEnable = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_SCAN_ENABLE_2', gapExtAdvAdvSet2)
gapExtAdvScanEnable.setLabel('Scan Request Notification Enable')
gapExtAdvScanEnable.setDefaultValue(False)
gapExtAdvScanEnable.setVisible(False)
gapExtAdvScanEnable.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data Menu
menuExtAdvData = libBLEStackComponent.createMenuSymbol('GAP_EXT_ADV_DATA_MENU_2', gapExtAdvAdvSet2)
menuExtAdvData.setLabel('Advertising Data')
menuExtAdvData.setVisible(False)
menuExtAdvData.setDescription("Advertising Data")
menuExtAdvData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-Flag Bool
gapExtAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_FLAG_EN_2', menuExtAdvData)
gapExtAdvDataFlagEn.setLabel('Flag')
gapExtAdvDataFlagEn.setDefaultValue(True)
gapExtAdvDataFlagEn.setVisible(True)
gapExtAdvDataFlagEn.setDescription("Flag")
gapExtAdvDataFlagEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-Flag
gapExtAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_DATA_FLAG_2', gapExtAdvDataFlagEn)
gapExtAdvDataFlag.setLabel(' ')
gapExtAdvDataFlag.addKey('0x05', '0', 'LE Limited Discoverable Mode')
gapExtAdvDataFlag.addKey('0x06', '1', 'LE General Discoverable Mode')
gapExtAdvDataFlag.setDefaultValue(0)
gapExtAdvDataFlag.setVisible(True)
gapExtAdvDataFlag.setOutputMode('Key')
gapExtAdvDataFlag.setDisplayMode('Description')
gapExtAdvDataFlag.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_FLAG_EN_2"])


# Ext Advertising Data-Local Name Bool
gapExtAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2', menuExtAdvData)
#print('libBLEStackComponent:gapExtAdvDataLocalNameEn type({})'.format(type(gapExtAdvDataLocalNameEn)))
gapExtAdvDataLocalNameEn.setLabel('Local Name')
gapExtAdvDataLocalNameEn.setDefaultValue(True)
gapExtAdvDataLocalNameEn.setVisible(True)
gapExtAdvDataLocalNameEn.setDescription("Local Name")
gapExtAdvDataLocalNameEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-Local Name
gapExtAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_LOCAL_NAME_2", gapExtAdvDataLocalNameEn)
gapExtAdvDataLocalName.setLabel(" ")
gapExtAdvDataLocalName.setDefaultValue("pic32cx-bz")
gapExtAdvDataLocalName.setVisible(True)
gapExtAdvDataLocalName.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2"])


# Ext Advertising Data-Service Data Bool
gapExtAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2', menuExtAdvData)
gapExtAdvDataSvcDataEn.setLabel('Service Data')
gapExtAdvDataSvcDataEn.setDefaultValue(False)
gapExtAdvDataSvcDataEn.setVisible(True)
gapExtAdvDataSvcDataEn.setDescription("Service Data")
gapExtAdvDataSvcDataEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-Service UUID
gapExtAdvDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID_2", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcUUID.setLabel("Service UUID")
gapExtAdvDataSvcUUID.setDescription("Service UUID")
gapExtAdvDataSvcUUID.setDefaultValue("DAFE")
gapExtAdvDataSvcUUID.setVisible(False)
gapExtAdvDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])


# Ext Advertising Data-Service UUID Warning Message
gapExtAdvDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID_2_CMT", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB). Invalid value will result in compiling error! ****")
gapExtAdvDataSvcUUIDComment.setVisible(False)
gapExtAdvDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])


# Ext Advertising Data- Service Data
gapExtAdvDataSvcData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA_2", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcData.setLabel("Service Data")
gapExtAdvDataSvcData.setDescription("Service Data")
gapExtAdvDataSvcData.setDefaultValue("00")
gapExtAdvDataSvcData.setVisible(False)
gapExtAdvDataSvcData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])


# Ext Advertising Data- Service Data Warning Message
gapExtAdvDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA_2_CMT", gapExtAdvDataSvcDataEn)
gapExtAdvDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataSvcDataComment.setVisible(False)
gapExtAdvDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])


# Ext Advertising Data-Manufacture Specific Data Bool
gapExtAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_MSD_EN_2', menuExtAdvData)
gapExtAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapExtAdvDataManuSpecDataEn.setDefaultValue(False)
gapExtAdvDataManuSpecDataEn.setVisible(True)
gapExtAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
gapExtAdvDataManuSpecDataEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-Manufacture Specific Data Company Id
gapExtAdvDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_MSD_CID_2", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataCid.setLabel("Company Id")
gapExtAdvDataManuSpecDataCid.setDefaultValue("0001")
gapExtAdvDataManuSpecDataCid.setVisible(False)
gapExtAdvDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])


# Ext Advertising Data-Manufacture Specific Data Company Id Warning Message
gapExtAdvDataManuSpecDataCidComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_MSD_CID_2_CMT", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataCidComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataManuSpecDataCidComment.setVisible(False)
gapExtAdvDataManuSpecDataCidComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])


# Ext Advertising Data-Manufacture Specific Data
gapExtAdvDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_MSD_2", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecData.setLabel("Manufacturer Data")
gapExtAdvDataManuSpecData.setDescription("Manufacturer Data")
gapExtAdvDataManuSpecData.setDefaultValue("00")
gapExtAdvDataManuSpecData.setVisible(False)
gapExtAdvDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])


# Ext Advertising Data-Manufacture Specific Data Warning Message
gapExtAdvDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_MSD_2_CMT", gapExtAdvDataManuSpecDataEn)
gapExtAdvDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataManuSpecDataComment.setVisible(False)
gapExtAdvDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])


# Ext Advertising Data-User Defined Data Bool
gapExtAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_UDD_EN_2', menuExtAdvData)
gapExtAdvDataUserDefEn.setLabel('User Defined Data')
gapExtAdvDataUserDefEn.setDefaultValue(False)
gapExtAdvDataUserDefEn.setVisible(True)
gapExtAdvDataUserDefEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Advertising Data-User Defined Data
gapExtAdvDataUserDef = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_UDD_2", gapExtAdvDataUserDefEn)
gapExtAdvDataUserDef.setLabel("User Defined Data")
gapExtAdvDataUserDef.setDescription("User Defined Data")
gapExtAdvDataUserDef.setDefaultValue("00")
gapExtAdvDataUserDef.setVisible(False)
gapExtAdvDataUserDef.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_UDD_EN_2"])


# Ext Advertising Data-User Defined Data Warning Message
gapExtAdvDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_UDD_2_CMT", gapExtAdvDataUserDefEn)
gapExtAdvDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtAdvDataUserDefComment.setVisible(False)
gapExtAdvDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_DATA_UDD_EN_2"])


# Ext Advertising Data
gapExtAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_2", gapExtAdvAdvSet2)
gapExtAdvertisingData.setLabel("Advertising Data")
gapExtAdvertisingData.setDefaultValue("Advertising Data")
gapExtAdvertisingData.setVisible(False)
gapExtAdvertisingData.setDependencies(gapSetExtAdvData, ["GAP_EXT_ADV_DATA_FLAG_EN_2", "GAP_EXT_ADV_DATA_FLAG_2",
                                                   "GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2", "GAP_EXT_ADV_DATA_LOCAL_NAME_2",
                                                   "GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2",
                                                   "GAP_EXT_ADV_DATA_SERVICE_UUID_2",
                                                   "GAP_EXT_ADV_DATA_SERVICE_DATA_2",
                                                   "GAP_EXT_ADV_DATA_MSD_EN_2", "GAP_EXT_ADV_DATA_MSD_2", "GAP_EXT_ADV_DATA_MSD_CID_2",
                                                   "GAP_EXT_ADV_DATA_UDD_EN_2", "GAP_EXT_ADV_DATA_UDD_2", ])


# Ext Advertising Data Length
gapExtAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_DATA_LEN_2", gapExtAdvAdvSet2)
gapExtAdvertisingDataLen.setLabel("Advertising Data Len")
gapExtAdvertisingDataLen.setDefaultValue(0)
gapExtAdvertisingDataLen.setVisible(False)


# Ext Advertising Data Value
gapExtAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_VALUE_2", gapExtAdvAdvSet2)
gapExtAdvertisingDataValue.setVisible(False)


# Generate Extended Advertising Data
gapGenExtAdvData(gapExtAdvertisingData)


# Ext Scan Response Data
menuExtScanRspData = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_RSP_DATA_MENU_2', gapExtAdvAdvSet2)
menuExtScanRspData.setLabel('Scan Response Data')
menuExtScanRspData.setVisible(False)
menuExtScanRspData.setDescription("Scan Response Data")
menuExtScanRspData.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Scan Response Data-Local Name Bool
gapExtScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2', menuExtScanRspData)
gapExtScanRspDataLocalNameEn.setLabel('Local Name')
gapExtScanRspDataLocalNameEn.setDefaultValue(True)
gapExtScanRspDataLocalNameEn.setDescription("Local Name")


# Ext Scan Response Data-Local Name
gapExtScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2", gapExtScanRspDataLocalNameEn)
gapExtScanRspDataLocalName.setLabel(" ")
gapExtScanRspDataLocalName.setDefaultValue("pic32cx-bz")
gapExtScanRspDataLocalName.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2"])


# Ext Scan Response Data-Service Data Bool
gapExtScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2', menuExtScanRspData)
gapExtScanRspDataSvcDataEn.setLabel('Service Data')
gapExtScanRspDataSvcDataEn.setDefaultValue(False)
gapExtScanRspDataSvcDataEn.setVisible(True)
gapExtScanRspDataSvcDataEn.setDescription("Service Data")
gapExtScanRspDataSvcDataEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Scan Response Data-Service UUID
gapExtScanRspDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcUUID.setLabel("Service UUID")
gapExtScanRspDataSvcUUID.setDescription("Service UUID")
gapExtScanRspDataSvcUUID.setDefaultValue("DAFE")
gapExtScanRspDataSvcUUID.setVisible(False)
gapExtScanRspDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])


# Ext Scan Response Data-Service UUID Warning Message
gapExtScanRspDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2_CMT", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB). Invalid value will result in compiling error! ****")
gapExtScanRspDataSvcUUIDComment.setVisible(False)
gapExtScanRspDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])


# Ext Scan Response Data- Service Data
gapExtScanRspDataSvcData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcData.setLabel("Service Data")
gapExtScanRspDataSvcData.setDescription("Service Data")
gapExtScanRspDataSvcData.setDefaultValue("00")
gapExtScanRspDataSvcData.setVisible(False)
gapExtScanRspDataSvcData.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])


# Ext Scan Response Data- Service Data Warning Message
gapExtScanRspDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2_CMT", gapExtScanRspDataSvcDataEn)
gapExtScanRspDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataSvcDataComment.setVisible(False)
gapExtScanRspDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])


# Ext Scan Response Data-Manufacture Specific Data Bool
gapExtScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_MSD_EN_2', menuExtScanRspData)
gapExtScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapExtScanRspDataManuSpecDataEn.setDefaultValue(False)
gapExtScanRspDataManuSpecDataEn.setVisible(True)
gapExtScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
gapExtScanRspDataManuSpecDataEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Scan Response Data-Manufacture Specific Data Company Id
gapExtScanRspDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CID_2", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecDataCid.setLabel("Company Id")
gapExtScanRspDataManuSpecDataCid.setDefaultValue("0001")
gapExtScanRspDataManuSpecDataCid.setVisible(False)
gapExtScanRspDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN_2"])


# Ext Scan Response Data-Manufacture Specific Data
gapExtScanRspDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_2", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecData.setLabel("Manufacture Data")
gapExtScanRspDataManuSpecData.setDescription("Manufacturer Data")
gapExtScanRspDataManuSpecData.setDefaultValue("00")
gapExtScanRspDataManuSpecData.setVisible(False)
gapExtScanRspDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN_2"])


# Ext Scan Response Data-Manufacture Specific Data Warning Message
gapExtScanRspDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_2_CMT", gapExtScanRspDataManuSpecDataEn)
gapExtScanRspDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataManuSpecDataComment.setVisible(False)
gapExtScanRspDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN_2"])


# Ext Scan Response Data-User Defined Data Bool
gapExtScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_UDD_EN_2', menuExtScanRspData)
gapExtScanRspDataUserDefEn.setLabel('User Defined Data')
gapExtScanRspDataUserDefEn.setDefaultValue(False)
gapExtScanRspDataUserDefEn.setVisible(True)
gapExtScanRspDataUserDefEn.setDependencies(gapConfigVisibility, ["GAP_EXT_ADV_ADV_SET_2"])


# Ext Scan Response Data-User Defined Data
gapExtScanRspDataUserDef = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_UDD_2", gapExtScanRspDataUserDefEn)
gapExtScanRspDataUserDef.setLabel("User Defined Data")
gapExtScanRspDataUserDef.setDescription("User Defined Data")
gapExtScanRspDataUserDef.setDefaultValue("00")
gapExtScanRspDataUserDef.setVisible(False)
gapExtScanRspDataUserDef.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN_2"])


# Ext Scan Response Data-User Defined Data Warning Message
gapExtScanRspDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_UDD_2_CMT", gapExtScanRspDataUserDefEn)
gapExtScanRspDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapExtScanRspDataUserDefComment.setVisible(False)
gapExtScanRspDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN_2"])


# Ext Scan Response Data
gapExtScanRspData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_2", gapExtAdvAdvSet2)
gapExtScanRspData.setLabel("Scan Response Data")
gapExtScanRspData.setVisible(False)
gapExtScanRspData.setDependencies(gapSetExtScanRspData, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2","GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2",
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2", 
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2", 
                                                   "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2", 
                                                   "GAP_EXT_SCAN_RSP_DATA_MSD_EN_2", "GAP_EXT_SCAN_RSP_DATA_MSD_2", "GAP_EXT_SCAN_RSP_DATA_MSD_CID_2",
                                                   "GAP_EXT_SCAN_RSP_DATA_UDD_EN_2", "GAP_EXT_SCAN_RSP_DATA_UDD_2", ])


# Ext Advertising Data Length
gapExtScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_RSP_DATA_LEN_2", gapExtAdvAdvSet2)
gapExtScanRspDataLen.setLabel("Scan Response Data Len")
gapExtScanRspDataLen.setDefaultValue(0)
gapExtScanRspDataLen.setVisible(False)


# Ext Advertising Data Value
gapExtScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_VALUE_2", gapExtAdvAdvSet2)
gapExtScanRspDataValue.setVisible(False)


# Generate Extended Scan Rsp Data
gapGenExtScanRspData(gapExtScanRspData)


# Advertising Data Menu
menuAdvData = libBLEStackComponent.createMenuSymbol('GAP_ADV_DATA_MENU', gapAdvertising)
menuAdvData.setLabel('Advertising Data')
menuAdvData.setVisible(True)
menuAdvData.setDescription("Advertising Data")
menuAdvData.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Flag Bool
gapAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_FLAG_EN', menuAdvData)
gapAdvDataFlagEn.setLabel('Flag')
gapAdvDataFlagEn.setDefaultValue(True)
gapAdvDataFlagEn.setVisible(True)
gapAdvDataFlagEn.setDescription("Flag")
gapAdvDataFlagEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Flag
gapAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_DATA_FLAG', gapAdvDataFlagEn)
gapAdvDataFlag.setLabel(' ')
gapAdvDataFlag.addKey('0x05', '0', 'LE Limited Discoverable Mode')
gapAdvDataFlag.addKey('0x06', '1', 'LE General Discoverable Mode')
gapAdvDataFlag.setDefaultValue(0)
gapAdvDataFlag.setVisible(True)
gapAdvDataFlag.setOutputMode('Key')
gapAdvDataFlag.setDisplayMode('Description')
gapAdvDataFlag.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Local Name Bool
gapAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_LOCAL_NAME_EN', menuAdvData)
#print('libBLEStackComponent:gapAdvDataLocalNameEn type({})'.format(type(gapAdvDataLocalNameEn)))
gapAdvDataLocalNameEn.setLabel('Local Name')
gapAdvDataLocalNameEn.setDefaultValue(True)
gapAdvDataLocalNameEn.setVisible(True)
gapAdvDataLocalNameEn.setDescription("Local Name")
gapAdvDataLocalNameEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Local Name
gapAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_LOCAL_NAME", gapAdvDataLocalNameEn)
gapAdvDataLocalName.setLabel(" ")
gapAdvDataLocalName.setDefaultValue("pic32cx-bz")
gapAdvDataLocalName.setVisible(True)
gapAdvDataLocalName.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Service Data Bool
gapAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_SERVICE_DATA_EN', menuAdvData)
gapAdvDataSvcDataEn.setLabel('Service Data')
gapAdvDataSvcDataEn.setDefaultValue(False)
gapAdvDataSvcDataEn.setVisible(True)
gapAdvDataSvcDataEn.setDescription("Service Data")
gapAdvDataSvcDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Service UUID
gapAdvDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_SERVICE_UUID", gapAdvDataSvcDataEn)
gapAdvDataSvcUUID.setLabel("Service UUID")
gapAdvDataSvcUUID.setDescription("Service UUID")
gapAdvDataSvcUUID.setDefaultValue("DAFE")
gapAdvDataSvcUUID.setVisible(False)
gapAdvDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])


# Advertising Data-Service UUID Warning Message
gapAdvDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_SERVICE_UUID_CMT", gapAdvDataSvcDataEn)
gapAdvDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB). Invalid value will result in compiling error! ****")
gapAdvDataSvcUUIDComment.setVisible(False)
gapAdvDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])


# Advertising Data- Service Data
gapAdvDataSvcData = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_SERVICE_DATA", gapAdvDataSvcDataEn)
gapAdvDataSvcData.setLabel("Service Data")
gapAdvDataSvcData.setDescription("Service Data")
gapAdvDataSvcData.setDefaultValue("00")
gapAdvDataSvcData.setVisible(False)
gapAdvDataSvcData.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])


# Advertising Data- Service Data Warning Message
gapAdvDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_SERVICE_DATA_CMT", gapAdvDataSvcDataEn)
gapAdvDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapAdvDataSvcDataComment.setVisible(False)
gapAdvDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])


# Advertising Data-Manufacture Specific Data Bool
gapAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol("GAP_ADV_DATA_MSD_EN", menuAdvData)
gapAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapAdvDataManuSpecDataEn.setDefaultValue(False)
gapAdvDataManuSpecDataEn.setVisible(True)
gapAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
gapAdvDataManuSpecDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-Manufacture Specific Data Company Id
gapAdvDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_MSD_CID", gapAdvDataManuSpecDataEn)
gapAdvDataManuSpecDataCid.setLabel("Company Id")
gapAdvDataManuSpecDataCid.setDefaultValue("0001")
gapAdvDataManuSpecDataCid.setVisible(False)
gapAdvDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_MSD_EN"])


# Advertising Data-Manufacture Specific Data Company Id Warning Message
gapAdvDataManuSpecDataCidComment = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_MSD_CID_CMT", gapAdvDataManuSpecDataEn)
gapAdvDataManuSpecDataCidComment.setLabel("**** 2 bytes in hexadecimal format. Invalid value will result in compiling error! ****")
gapAdvDataManuSpecDataCidComment.setVisible(False)
gapAdvDataManuSpecDataCidComment.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_MSD_EN"])


# Advertising Data-Manufacture Specific Data
gapAdvDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_MSD", gapAdvDataManuSpecDataEn)
gapAdvDataManuSpecData.setLabel("Manufacturer Data")
gapAdvDataManuSpecData.setDescription("Manufacturer Data")
gapAdvDataManuSpecData.setDefaultValue("00")
gapAdvDataManuSpecData.setVisible(False)
gapAdvDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_MSD_EN"])


# Advertising Data-Manufacture Specific Data Warning Message
gapAdvDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_MSD_CMT", gapAdvDataManuSpecDataEn)
gapAdvDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapAdvDataManuSpecDataComment.setVisible(False)
gapAdvDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_MSD_EN"])


# Advertising Data-User Defined Data Bool
gapAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_UDD_EN', menuAdvData)
gapAdvDataUserDefEn.setLabel('User Defined Data')
gapAdvDataUserDefEn.setDefaultValue(False)
gapAdvDataUserDefEn.setVisible(True)
gapAdvDataUserDefEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Advertising Data-User Defined Data
gapAdvDataUserDef = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_UDD", gapAdvDataUserDefEn)
gapAdvDataUserDef.setLabel("User Defined Data")
gapAdvDataUserDef.setDescription("User Defined Data")
gapAdvDataUserDef.setDefaultValue("00")
gapAdvDataUserDef.setVisible(False)
gapAdvDataUserDef.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_UDD_EN"])


# Advertising Data-User Defined Data Warning Message
gapAdvDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_UDD_CMT", gapAdvDataUserDefEn)
gapAdvDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapAdvDataUserDefComment.setVisible(False)
gapAdvDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_ADV_DATA_UDD_EN"])


# Advertising Data
gapAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA", gapAdvertising)
gapAdvertisingData.setLabel("Advertising Data")
gapAdvertisingData.setDefaultValue("Advertising Data")
gapAdvertisingData.setVisible(False)
gapAdvertisingData.setDependencies(gapSetAdvData, ["GAP_ADV_DATA_FLAG_EN", "GAP_ADV_DATA_FLAG",
                                                   "GAP_ADV_DATA_LOCAL_NAME_EN", "GAP_ADV_DATA_LOCAL_NAME",
                                                   "GAP_ADV_DATA_SERVICE_DATA_EN",
                                                   "GAP_ADV_DATA_SERVICE_UUID",
                                                   "GAP_ADV_DATA_SERVICE_DATA",
                                                   "GAP_ADV_DATA_MSD_EN", "GAP_ADV_DATA_MSD", "GAP_ADV_DATA_MSD_CID",
                                                   "GAP_ADV_DATA_UDD_EN", "GAP_ADV_DATA_UDD"])

# Advertising Data length
gapAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_ADV_DATA_LEN", gapAdvertising)
gapAdvertisingDataLen.setLabel("Advertising Data Len")
gapAdvertisingDataLen.setDefaultValue(0)
gapAdvertisingDataLen.setVisible(False)


# Advertising Data Value
gapAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_VALUE", gapAdvertising)
gapAdvertisingDataValue.setVisible(False)


# Generate Advertising Data
gapGenAdvData(gapAdvertisingData)


# Scan Response Data
menuScanRspData = libBLEStackComponent.createMenuSymbol('GAP_SCAN_RSP_DATA_MENU', gapAdvertising)
menuScanRspData.setLabel('Scan Response Data')
menuScanRspData.setVisible(True)
menuScanRspData.setDescription("Scan Response Data")
menuScanRspData.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Scan Response Data-Local Name Bool
gapScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_LOCAL_NAME_EN', menuScanRspData)
gapScanRspDataLocalNameEn.setLabel('Local Name')
gapScanRspDataLocalNameEn.setDefaultValue(True)
gapScanRspDataLocalNameEn.setDescription("Local Name")


# Scan Response Data-Local Name
gapScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_LOCAL_NAME", gapScanRspDataLocalNameEn)
gapScanRspDataLocalName.setLabel(" ")
gapScanRspDataLocalName.setDefaultValue("pic32cx-bz")
gapScanRspDataLocalName.setDependencies(gapLegacyAdvVisibility, ["GAP_SCAN_RSP_DATA_LOCAL_NAME_EN"])


# Scan Response Data-Service Data Bool
gapScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_SERVICE_DATA_EN', menuScanRspData)
gapScanRspDataSvcDataEn.setLabel('Service Data')
gapScanRspDataSvcDataEn.setDefaultValue(False)
gapScanRspDataSvcDataEn.setVisible(True)
gapScanRspDataSvcDataEn.setDescription("Service Data")
gapScanRspDataSvcDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Scan Response Data-Service UUID
gapScanRspDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_SERVICE_UUID", gapScanRspDataSvcDataEn)
gapScanRspDataSvcUUID.setLabel("Service UUID")
gapScanRspDataSvcUUID.setDescription("Service UUID")
gapScanRspDataSvcUUID.setDefaultValue("DAFE")
gapScanRspDataSvcUUID.setVisible(False)
gapScanRspDataSvcUUID.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Scan Response Data-Service UUID Warning Message
gapScanRspDataSvcUUIDComment = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_SERVICE_UUID_CMT", gapScanRspDataSvcDataEn)
gapScanRspDataSvcUUIDComment.setLabel("**** 2 bytes in hexadecimal format(LSB). Invalid value will result in compiling error! ****")
gapScanRspDataSvcUUIDComment.setVisible(False)
gapScanRspDataSvcUUIDComment.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Scan Response Data- Service Data
gapScanRspDataSvcData = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_SERVICE_DATA", gapScanRspDataSvcDataEn)
gapScanRspDataSvcData.setLabel("Service Data")
gapScanRspDataSvcData.setDescription("Service Data")
gapScanRspDataSvcData.setDefaultValue("00")
gapScanRspDataSvcData.setVisible(False)
gapScanRspDataSvcData.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Scan Response Data- Service Data Warning Message
gapScanRspDataSvcDataComment = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_SERVICE_DATA_CMT", gapScanRspDataSvcDataEn)
gapScanRspDataSvcDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapScanRspDataSvcDataComment.setVisible(False)
gapScanRspDataSvcDataComment.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])


# Scan Response Data-Manufacture Specific Data Bool
gapScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_MSD_EN', menuScanRspData)
gapScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
gapScanRspDataManuSpecDataEn.setDefaultValue(False)
gapScanRspDataManuSpecDataEn.setVisible(True)
gapScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
gapScanRspDataManuSpecDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Scan Response Data-Manufacture Specific Data Company Id
gapScanRspDataManuSpecDataCid = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_MSD_CID", gapScanRspDataManuSpecDataEn)
gapScanRspDataManuSpecDataCid.setLabel("Company Id")
gapScanRspDataManuSpecDataCid.setDefaultValue("0001")
gapScanRspDataManuSpecDataCid.setVisible(False)
gapScanRspDataManuSpecDataCid.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])


# Scan Response Data-Manufacture Specific Data Company Id Warning Message
gapScanRspDataManuSpecDataCidComment = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_MSD_CID_CMT", gapScanRspDataManuSpecDataEn)
gapScanRspDataManuSpecDataCidComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapScanRspDataManuSpecDataCidComment.setVisible(False)
gapScanRspDataManuSpecDataCidComment.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])


# Scan Response Data-Manufacture Specific Data
gapScanRspDataManuSpecData = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_MSD", gapScanRspDataManuSpecDataEn)
gapScanRspDataManuSpecData.setLabel("Manufacture Data")
gapScanRspDataManuSpecData.setDescription("Manufacturer Data")
gapScanRspDataManuSpecData.setDefaultValue("00")
gapScanRspDataManuSpecData.setVisible(False)
gapScanRspDataManuSpecData.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])


# Scan Response Data-Manufacture Specific Data Warning Message
gapScanRspDataManuSpecDataComment = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_MSD_CMT", gapScanRspDataManuSpecDataEn)
gapScanRspDataManuSpecDataComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapScanRspDataManuSpecDataComment.setVisible(False)
gapScanRspDataManuSpecDataComment.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])


# Scan Response Data-User Defined Data Bool
gapScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_UDD_EN', menuScanRspData)
gapScanRspDataUserDefEn.setLabel('User Defined Data')
gapScanRspDataUserDefEn.setDefaultValue(False)
gapScanRspDataUserDefEn.setVisible(True)
gapScanRspDataUserDefEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Scan Response Data-User Defined Data
gapScanRspDataUserDef = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_UDD", gapScanRspDataUserDefEn)
gapScanRspDataUserDef.setLabel("User Defined Data")
gapScanRspDataUserDef.setDescription("User Defined Data")
gapScanRspDataUserDef.setDefaultValue("00")
gapScanRspDataUserDef.setVisible(False)
gapScanRspDataUserDef.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_UDD_EN"])


# Scan Response Data-User Defined Data Warning Message
gapScanRspDataUserDefComment = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_UDD_CMT", gapScanRspDataUserDefEn)
gapScanRspDataUserDefComment.setLabel("**** Hexadecimal format. Invalid value will result in compiling error! ****")
gapScanRspDataUserDefComment.setVisible(False)
gapScanRspDataUserDefComment.setDependencies(gapConfigVisibility, ["GAP_SCAN_RSP_DATA_UDD_EN"])


# Scan Response Data
gapScanRspData = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA", gapAdvertising)
gapScanRspData.setLabel("Scan Response Data")
gapScanRspData.setVisible(False)
gapScanRspData.setDependencies(gapSetScanRspData, ["GAP_SCAN_RSP_DATA_LOCAL_NAME_EN","GAP_SCAN_RSP_DATA_LOCAL_NAME",
                                                   "GAP_SCAN_RSP_DATA_SERVICE_DATA_EN",
                                                   "GAP_SCAN_RSP_DATA_SERVICE_UUID",
                                                   "GAP_SCAN_RSP_DATA_SERVICE_DATA",
                                                   "GAP_SCAN_RSP_DATA_MSD_EN", "GAP_SCAN_RSP_DATA_MSD", "GAP_SCAN_RSP_DATA_MSD_CID",
                                                   "GAP_SCAN_RSP_DATA_UDD_EN", "GAP_SCAN_RSP_DATA_UDD", ])

# Scan Response Data length
gapScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_RSP_DATA_LEN", gapAdvertising)
gapScanRspDataLen.setLabel("Scan Response Data Len")
gapScanRspDataLen.setDefaultValue(0)
gapScanRspDataLen.setVisible(False)
gapScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_VALUE", gapAdvertising)
gapScanRspDataValue.setVisible(False)


#Generate Scan Rsp Data
gapGenScanRspData(gapScanRspData)


# DSADV setting
global gapDsadvEn
gapDsadvEn = libBLEStackComponent.createBooleanSymbol('GAP_DSADV_EN', gapAdvertising)
gapDsadvEn.setLabel('Enable Deep Sleep Advertising')
gapDsadvEn.setDescription('Enable Deep Sleep Advertising')
gapDsadvEn.setDefaultValue(False)
gapDsadvEn.setVisible(True)
gapDsadvEn.setDependencies(deepSleepEnabledChange, ["GAP_DSADV_EN","BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])


# Scan setting
global gapScan
gapScan = libBLEStackComponent.createBooleanSymbol('GAP_SCAN', menuGAP)
gapScan.setLabel('Scan')
gapScan.setDefaultValue(False)
gapScan.setVisible(True)
gapScan.setDescription("OBSERVER ROLE CONFIGURATION")


# Extended Scan
global gapExtScanEn
gapExtScanEn = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_EXT_SCAN', gapScan)
gapExtScanEn.setLabel('Enable Extended Scan')
gapExtScanEn.setDefaultValue(False)
gapExtScanEn.setVisible(False)
gapExtScanEn.setDescription("Enable Extended Scan")
gapExtScanEn.setDependencies(gapConfigVisibility, ["GAP_SCAN"])


# Extended Scan Data Len
gapExtScanDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_DATA_LEN", gapExtScanEn)
gapExtScanDataLen.setLabel("Maximum Data Len")
gapExtScanDataLen.setDefaultValue(74)
gapExtScanDataLen.setMin(74)
gapExtScanDataLen.setMax(1650)
gapExtScanDataLen.setVisible(False)
gapExtScanDataLen.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_SCAN"])


# Extended Scan Num
gapExtScanNum2Pkt = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_NUM_2_PKT", gapExtScanEn)
gapExtScanNum2Pkt.setLabel("Maximum Num of Secondary Packets")
gapExtScanNum2Pkt.setDefaultValue(0)
gapExtScanNum2Pkt.setMin(0)
gapExtScanNum2Pkt.setMax(16)
gapExtScanNum2Pkt.setVisible(False)
gapExtScanNum2Pkt.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_SCAN"])


# Ext Scan Filter Policy
gapScanFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_FILT_POLICY', gapExtScanEn)
gapScanFiltPolicy.setLabel('Scan Filter Policy')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_ALL', '0', 'Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to this device')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST', '1', 'Accept only advertising and scan response PDUs from devices where the advertisers address is in the White List')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_NOT_RESOLVED_DIRECTED', '2', 'Accept all advertising and scan response PDUs except directed advertising PDUs where the identity address corresponding to TargetA does not address this device')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST_NOT_RESOLVED_DIRECTED', '3', 'Accept all advertising and scan response PDUs except: the advertiser identity address is not in the White List and TargetA does not address this device')
gapScanFiltPolicy.setDefaultValue(0)
gapScanFiltPolicy.setOutputMode('Key')
gapScanFiltPolicy.setDisplayMode('Description')
gapScanFiltPolicy.setVisible(False)
gapScanFiltPolicy.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_SCAN"])


# Ext Scan PHY
global gapScanPhy
gapScanPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_PHY', gapExtScanEn)
gapScanPhy.setLabel('Scan PHY')
gapScanPhy.addKey('BLE_GAP_SCAN_PHY_1M', '0', 'Scan advertisements on the LE 1M PHY')
gapScanPhy.addKey('BLE_GAP_SCAN_PHY_CODED', '1', 'Scan advertisements on the LE Coded PHY')
gapScanPhy.addKey('BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED', '2', 'Scan advertisements on the LE 1M PHY and LE Coded PHY')
gapScanPhy.setDefaultValue(0)
gapScanPhy.setOutputMode('Key')
gapScanPhy.setDisplayMode('Description')
gapScanPhy.setVisible(False)
gapScanPhy.setDependencies(gapConfigVisibility, ["BOOL_GAP_EXT_SCAN"])


# Ext 1M PHY
menuScanPhy = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_PHY_1M', gapScanPhy)
menuScanPhy.setLabel('LE 1M PHY')
menuScanPhy.setDescription("LE 1M PHY")
menuScanPhy.setVisible(gapScanPhy.getDefaultValue() == 0 or gapScanPhy.getDefaultValue() == 2)
menuScanPhy.setDependencies(gapExtScanPhy1mVisibility, ["GAP_EXT_SCAN_PHY"])


# Ext Advertising Type 1M
gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_TYPE_1M', menuScanPhy)
gapScanType.setLabel('Scan Type')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
gapScanType.setDefaultValue(0)
gapScanType.setOutputMode('Key')
gapScanType.setDisplayMode('Description')


# Ext Scan Interval 1M
gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_INTERVAL_1M", menuScanPhy)
gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
gapScanInterval.setDefaultValue(160)
gapScanInterval.setMin(1)
gapScanInterval.setMax(65535)


# Ext Scan Window 1M
gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_WINDOW_1M", menuScanPhy)
gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
gapScanWindow.setDefaultValue(32)
gapScanWindow.setMin(1)
gapScanWindow.setMax(65535)


# Ext Disable Channel Map
gapDisChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_DIS_CHANNEL_MAP_1M', menuScanPhy)
gapDisChannelMap.setLabel('Disable specific channel during scanning')
gapDisChannelMap.addKey('0', '0', 'None')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_38', '1', 'Channel 37, and 38')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38|BLE_GAP_SCAN_CHANNEL_39', '2', 'Channel 38, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_39', '3', 'Channel 37, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37', '4', 'Channel 37 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38', '5', 'Channel 38 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_39', '6', 'Channel 39 Only')
gapDisChannelMap.setDefaultValue(0)
gapDisChannelMap.setOutputMode('Key')
gapDisChannelMap.setDisplayMode('Description')


# Ext Coded PHY
menuScanPhy = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_PHY_CODED', gapScanPhy)
menuScanPhy.setLabel('LE Coded PHY')
menuScanPhy.setDescription("LE Coded PHY")
menuScanPhy.setVisible(gapScanPhy.getDefaultValue() == 1 or gapScanPhy.getDefaultValue() == 2)
menuScanPhy.setDependencies(gapExtScanPhyCodedVisibility, ["GAP_EXT_SCAN_PHY"])


# Ext Advertising Type Coded
gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_TYPE_CODED', menuScanPhy)
gapScanType.setLabel('Scan Type')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
gapScanType.setDefaultValue(0)
gapScanType.setOutputMode('Key')
gapScanType.setDisplayMode('Description')


# Ext Scan Interval Coded
gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_INTERVAL_CODED", menuScanPhy)
gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
gapScanInterval.setDefaultValue(160)
gapScanInterval.setMin(1)
gapScanInterval.setMax(65535)


# Ext Scan Window Coded
gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_WINDOW_CODED", menuScanPhy)
gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
gapScanWindow.setDefaultValue(32)
gapScanWindow.setMin(1)
gapScanWindow.setMax(65535)


# Ext Disable Channel Map
gapDisChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_DIS_CHANNEL_MAP_CODED', menuScanPhy)
gapDisChannelMap.setLabel('Disable specific channel during scanning')
gapDisChannelMap.addKey('0', '0', 'None')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_38', '1', 'Channel 37, and 38')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38|BLE_GAP_SCAN_CHANNEL_39', '2', 'Channel 38, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_39', '3', 'Channel 37, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37', '4', 'Channel 37 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38', '5', 'Channel 38 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_39', '6', 'Channel 39 Only')
gapDisChannelMap.setDefaultValue(0)
gapDisChannelMap.setOutputMode('Key')
gapDisChannelMap.setDisplayMode('Description')


# Advertising Type
gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_SCAN_TYPE', gapScan)
gapScanType.setLabel('Scan Type')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
gapScanType.setDefaultValue(0)
gapScanType.setOutputMode('Key')
gapScanType.setDisplayMode('Description')
gapScanType.setVisible(False)
gapScanType.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])


# Scan Interval
gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_INTERVAL", gapScan)
gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
gapScanInterval.setDefaultValue(160)
gapScanInterval.setMin(1)
gapScanInterval.setMax(65535)
gapScanInterval.setVisible(False)
gapScanInterval.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])


# Scan Window
gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_WINDOW", gapScan)
gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
gapScanWindow.setDefaultValue(32)
gapScanWindow.setMin(1)
gapScanWindow.setMax(65535)
gapScanWindow.setVisible(False)
gapScanWindow.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])


# Scan Filter Policy
gapScanFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SCAN_FILT_POLICY', gapScan)
gapScanFiltPolicy.setLabel('Scan Filter Policy')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_ALL', '0', 'Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to this device')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST', '1', 'Accept only advertising and scan response PDUs from devices where the advertisers address is in the White List')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_NOT_RESOLVED_DIRECTED', '2', 'Accept all advertising and scan response PDUs except directed advertising PDUs where the identity address corresponding to TargetA does not address this device')
gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST_NOT_RESOLVED_DIRECTED', '3', 'Accept all advertising and scan response PDUs except: the advertiser identity address is not in the White List and TargetA does not address this device')
gapScanFiltPolicy.setDefaultValue(0)
gapScanFiltPolicy.setOutputMode('Key')
gapScanFiltPolicy.setDisplayMode('Description')
gapScanFiltPolicy.setVisible(False)
gapScanFiltPolicy.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])


# Disable Channel Map
gapDisChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_SCAN_DIS_CHANNEL_MAP', gapScan)
gapDisChannelMap.setLabel('Disable specific channel during scanning')
gapDisChannelMap.addKey('0', '0', 'None')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_38', '1', 'Channel 37, and 38')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38|BLE_GAP_SCAN_CHANNEL_39', '2', 'Channel 38, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37|BLE_GAP_SCAN_CHANNEL_39', '3', 'Channel 37, and 39')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_37', '4', 'Channel 37 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_38', '5', 'Channel 38 Only')
gapDisChannelMap.addKey('BLE_GAP_SCAN_CHANNEL_39', '6', 'Channel 39 Only')
gapDisChannelMap.setDefaultValue(0)
gapDisChannelMap.setVisible(False)
gapDisChannelMap.setOutputMode('Key')
gapDisChannelMap.setDisplayMode('Description')
gapDisChannelMap.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])

 
# Peripheral Role
gapPeripheralRole = libBLEStackComponent.createBooleanSymbol('GAP_PERIPHERAL', menuGAP)
gapPeripheralRole.setLabel('Peripheral')
gapPeripheralRole.setDefaultValue(True)
gapPeripheralRole.setVisible(True)
gapPeripheralRole.setDescription("PERIPHERAL ROLE CONFIGURATION")

# Central Role
gapCentralRole = libBLEStackComponent.createBooleanSymbol('GAP_CENTRAL', menuGAP)
gapCentralRole.setLabel('Central')
gapCentralRole.setDefaultValue(False)
gapCentralRole.setVisible(True)
gapCentralRole.setDescription("CENTRAL ROLE CONFIGURATION")


# Direct Test Mode
gapDirectTestMode = libBLEStackComponent.createBooleanSymbol('GAP_DIRECT_TEST_MODE', menuGAP)
gapDirectTestMode.setLabel('Direct Test Mode')
gapDirectTestMode.setDefaultValue(False)
gapDirectTestMode.setVisible(True)
gapDirectTestMode.setDescription("Include Direct Test Mode module")


# Connection Tx Power
gapConnTxPwr = libBLEStackComponent.createIntegerSymbol('GAP_CONN_TX_PWR', menuGAP)
gapConnTxPwr.setLabel('Connection TX Power (dBm)')
gapConnTxPwr.setDefaultValue(gapGenDefTxPwr(False))
gapConnTxPwr.setMax(gapGenMaxTxPwr(False))
gapConnTxPwr.setMin(gapGenMinTxPwr())
gapConnTxPwr.setVisible(True)
gapConnTxPwr.setDescription("Connection TX Power")