def smpConfigVisibility(symbol, event):
    symbol.setVisible(event["value"])

############################################################################
### SMP Settings ###
############################################################################

# SMP Menu
menuSMP = libBLEStackComponent.createMenuSymbol('SMP_MENU', None)
menuSMP.setLabel('Security Manager (SM)')
menuSMP.setVisible(True)
menuSMP.setDescription("SMP SETTINGS")


# IO Capability
smpIoCapability = libBLEStackComponent.createKeyValueSetSymbol('SMP_IOCAP_TYPE', menuSMP)
smpIoCapability.setLabel('IO Capability')
smpIoCapability.addKey('BLE_SMP_IO_DISPLAYONLY', '0', 'Display Only')
smpIoCapability.addKey('BLE_SMP_IO_DISPLAYYESNO', '1', 'Display YesNo')
smpIoCapability.addKey('BLE_SMP_IO_KEYBOARDONLY', '2', 'Keyboard Only')
smpIoCapability.addKey('BLE_SMP_IO_NOINPUTNOOUTPUT', '3', 'NoInputNoOutput')
smpIoCapability.addKey('BLE_SMP_IO_KEYBOARDDISPLAY', '4', 'Keyboard Display')
smpIoCapability.setDefaultValue(3)
smpIoCapability.setOutputMode('Key')
smpIoCapability.setDisplayMode('Description')


# Authentication Requirement Menu
smpAuthReqManu = libBLEStackComponent.createMenuSymbol('SMP_AUTH_REQ', menuSMP)
smpAuthReqManu.setLabel('Authentication Setting')
smpAuthReqManu.setVisible(True)
smpAuthReqManu.setDescription("SMP Authentication Requirement")

# Bonding
smpAuthReqBond = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_BOND', smpAuthReqManu)
smpAuthReqBond.setLabel('Bonding')
smpAuthReqBond.setDefaultValue(True)
smpAuthReqBond.setVisible(True)


# MITM
smpAuthReqMitm = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_MITM', smpAuthReqManu)
smpAuthReqMitm.setLabel('MITM')
smpAuthReqMitm.setDefaultValue(False)
smpAuthReqMitm.setVisible(True)


# Secure Connections
smpAuthReqSC = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_SC', smpAuthReqManu)
smpAuthReqSC.setLabel('Secure Connections')
smpAuthReqSC.setDefaultValue(True)
smpAuthReqSC.setVisible(True) 


# OOB
smpOob = libBLEStackComponent.createBooleanSymbol('SMP_OOB', menuSMP)
smpOob.setLabel('OOB Authentication Data Present')
smpOob.setDefaultValue(False)
smpOob.setVisible(True)


# Secure Connections Only
smpSCOnly = libBLEStackComponent.createBooleanSymbol('SMP_SC_ONLY', menuSMP)
smpSCOnly.setLabel('Secure Connections Only')
smpSCOnly.setDefaultValue(False)
smpSCOnly.setReadOnly(False)
smpSCOnly.setVisible(True)
smpSCOnly.setDependencies(smpConfigVisibility, ["SMP_AUTH_REQ_SC"])


# Authenticated pairing
smpAuthPair = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_PAIR', menuSMP)
smpAuthPair.setLabel('Authenticated pairing method is required')
smpAuthPair.setDefaultValue(False)
smpAuthPair.setVisible(True)