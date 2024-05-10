def gcmConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

############################################################################
### GCM Settings ###
############################################################################

# GCM Menu
menuGcm = libBLEStackComponent.createMenuSymbol('GCM_MENU', None)
menuGcm.setLabel('GATT Client Manager (GCM)')
menuGcm.setVisible(False)
menuGcm.setDescription("GCM SETTINGS")
menuGcm.setDependencies(gcmConfigVisibility, ["BLE_BOOL_GATT_CLIENT"])


# Service Change Manager
gcmScm = libBLEStackComponent.createBooleanSymbol('BOOL_GCM_SCM', menuGcm)
gcmScm.setLabel('Service Change Manager')
gcmScm.setDefaultValue(False)


# DD Menu
menuDd = libBLEStackComponent.createMenuSymbol('GCM_DD_MENU', menuGcm)
menuDd.setLabel('Device Discovery(DD)')
menuDd.setDescription("Service discovery")


# Wait for security
ddWaitForSecurity = libBLEStackComponent.createBooleanSymbol('BOOL_GCM_DD_WAIT_FOR_SEC', menuDd)
ddWaitForSecurity.setLabel('Wait for security')
ddWaitForSecurity.setDefaultValue(False)


# Init discovery in central Role
ddInitDisInCentral = libBLEStackComponent.createBooleanSymbol('BOOL_GCM_DD_INIT_DISC_IN_CNTRL', menuDd)
ddInitDisInCentral.setLabel('Init discovery with central role')
ddInitDisInCentral.setDefaultValue(True)


# Init discovery in peripheral Role
ddInitDisInPeripheral = libBLEStackComponent.createBooleanSymbol('BOOL_GCM_DD_INIT_DISC_IN_PER', menuDd)
ddInitDisInPeripheral.setLabel('Init discovery with peripheral role')
ddInitDisInPeripheral.setDefaultValue(False)
