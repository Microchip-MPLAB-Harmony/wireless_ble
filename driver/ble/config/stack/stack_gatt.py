def gattConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

############################################################################
### GATT Settings ###
############################################################################

# GATT Menu
menuGATT = libBLEStackComponent.createMenuSymbol('GATT_MENU', None)
menuGATT.setLabel('Generic Attribute Profile (GATT)')
menuGATT.setVisible(True)
menuGATT.setDescription("GATT SETTINGS")


# GATT Server Menu
gattServerRole = libBLEStackComponent.createMenuSymbol('BLE_MENU_GATT_SERVER', menuGATT)
gattServerRole.setLabel('Server Role')


# GATT Caching
gattCachingEnable = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_CACHING', gattServerRole)
gattCachingEnable.setLabel('Enable GATT Caching')
gattCachingEnable.setDefaultValue(False)
gattCachingEnable.setVisible(True)


# Service Change
gattSvc = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_SVC_CHANGE', gattServerRole)
gattSvc.setLabel('Enable GATT Service Change')
gattSvc.setDefaultValue(False)
gattSvc.setVisible(True)


# GATT Client Menu
gattClientRole = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_CLIENT', menuGATT)
gattClientRole.setLabel('Enable Client Role')
gattClientRole.setDefaultValue(False)


# Enable Manual Handle Confirmation
gattManuConfirm = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_MANU_CONFIRM', gattClientRole)
gattManuConfirm.setLabel('Enable Manual Handle Confirmation')
gattManuConfirm.setDefaultValue(False)
gattManuConfirm.setVisible(False)
gattManuConfirm.setDependencies(gattConfigVisibility, ["BLE_BOOL_GATT_CLIENT"])