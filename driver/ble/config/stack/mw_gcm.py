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