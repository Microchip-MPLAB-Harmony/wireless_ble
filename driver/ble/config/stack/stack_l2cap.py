############################################################################
### L2CAP Settings ###
############################################################################

# L2CAP Menu
menuL2CAP = libBLEStackComponent.createMenuSymbol('L2CAP_MENU', None)
menuL2CAP.setLabel('Logic Link Control And Adaption Protocol (L2CAP)')
menuL2CAP.setVisible(True)
menuL2CAP.setDescription("L2CAP SETTINGS")

# Credit Based Flow Control
enableL2CAPCreditFlowCtrl = libBLEStackComponent.createBooleanSymbol('BOOL_L2CAP_CREDIT_FLOWCTRL', menuL2CAP)
enableL2CAPCreditFlowCtrl.setLabel('Credit Based Flow Control')
enableL2CAPCreditFlowCtrl.setDefaultValue(False)