def dmConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

############################################################################
### DM Settings ###
############################################################################

# DM Menu
menuDm = libBLEStackComponent.createMenuSymbol('DM_MENU', None)
menuDm.setLabel('Device Manager (DM)')
menuDm.setVisible(True)
menuDm.setDescription("DM SETTINGS")


# Auto Accept Security Request
dmSecAutoAccept = libBLEStackComponent.createBooleanSymbol('BOOL_DM_SEC_AUTO_ACCEPT', menuDm)
dmSecAutoAccept.setLabel('Auto Accept Security Request')
dmSecAutoAccept.setDefaultValue(True)


# Auto Accept Connect Parameters Update Request
dmConnAudoAccept = libBLEStackComponent.createBooleanSymbol('BOOL_DM_CONN_AUTO_ACCEPT', menuDm)
dmConnAudoAccept.setLabel('Auto Accept Connect Parameters Update Request')
dmConnAudoAccept.setDefaultValue(True)


# Minimum Connection Interval
dmMinConnInterval = libBLEStackComponent.createIntegerSymbol("DM_MIN_CONN_INTERVAL", dmConnAudoAccept)
dmMinConnInterval.setLabel("Minimum Connection Interval (Unit: 1.25ms)")
dmMinConnInterval.setDefaultValue(6)
dmMinConnInterval.setMin(6)
dmMinConnInterval.setMax(3200)
dmMinConnInterval.setVisible(True)
dmMinConnInterval.setDependencies(dmConfigVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"]) 


# Maximum Connection Interval
dmMaxConnInterval = libBLEStackComponent.createIntegerSymbol("DM_MAX_CONN_INTERVAL", dmConnAudoAccept)
dmMaxConnInterval.setLabel("Maximum Connection Interval (Unit: 1.25ms)")
dmMaxConnInterval.setDefaultValue(3200)
dmMaxConnInterval.setMin(6)
dmMaxConnInterval.setMax(3200)
dmMaxConnInterval.setVisible(True)
dmMaxConnInterval.setDependencies(dmConfigVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"]) 


# Minimum Connection Latency
dmMinConnLatency = libBLEStackComponent.createIntegerSymbol("DM_MIN_CONN_LATENCY", dmConnAudoAccept)
dmMinConnLatency.setLabel("Minimum Connection Latency")
dmMinConnLatency.setDefaultValue(0)
dmMinConnLatency.setMin(0)
dmMinConnLatency.setMax(499)
dmMinConnLatency.setVisible(True)
dmMinConnLatency.setDependencies(dmConfigVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"])


# Maximum Connection Latency
dmMaxConnLatency = libBLEStackComponent.createIntegerSymbol("DM_MAX_CONN_LATENCY", dmConnAudoAccept)
dmMaxConnLatency.setLabel("Maximum Connection Latency")
dmMaxConnLatency.setDefaultValue(499)
dmMaxConnLatency.setMin(0)
dmMaxConnLatency.setMax(499)
dmMaxConnLatency.setVisible(True)
dmMaxConnLatency.setDependencies(dmConfigVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"])