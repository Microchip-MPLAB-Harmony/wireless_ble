# coding: utf-8
##############################################################################
# Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    # THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

## PIC32CX-BZ Radio stacks

print('Load Module: Harmony Wireless BLE Driver Profiles')

## BLE Profiles
profileTrspComponent = Module.CreateComponent('PROFILE_TRSP', 'Transparent Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/trsp.py')
profileTrspComponent.addDependency('BLE_TRS_Dependency', 'BLE_TRS', 'Transparent Service', False, True)
profileTrspComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

profileTrcbpComponent = Module.CreateComponent('PROFILE_TRCBP', 'Transparent Credit Based Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/trcbp.py')
profileTrcbpComponent.addDependency('BLE_TRCBS_Dependency', 'BLE_TRCBS', 'Transparent Credit Based Service', False, True)
profileTrcbpComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

profilePxpComponent = Module.CreateComponent('PROFILE_PXP', 'Promixity Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/pxp.py')
profilePxpComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)
profilePxpComponent.addDependency('BLE_LLS_Dependency', 'BLE_LLS', 'Link Loss Service', False, True)
profilePxpComponent.addDependency('BLE_IAS_Dependency', 'BLE_IAS', 'Immediate Alert Service', False, True)
profilePxpComponent.addDependency('BLE_TPS_Dependency', 'BLE_TPS', 'TX Power Service', False, True)

#profileOtapComponent = Module.CreateComponent('PROFILE_OTAP', 'Over-The-Air Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/otap.py')
#profileOtapComponent.addDependency('BLE_OTAS_Dependency', 'BLE_OTAS', 'Over-The-Air Service', False, True)
#profileOtapComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

profileAnpComponent = Module.CreateComponent('PROFILE_ANP', 'Alert Notification Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/anp.py')
profileAnpComponent.addDependency('BLE_ANS_Dependency', 'BLE_ANS', 'Alert Notification Service', False, True)
profileAnpComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

profileHogpComponent = Module.CreateComponent('PROFILE_HOGP', 'HID Over GATT Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/hogp.py')
profileHogpComponent.addDependency("BLE_HIDS_Dependency", "BLE_HIDS", 'Human Interface Devices Service', False, True)
profileHogpComponent.addDependency("BLE_BAS_Dependency", "BLE_BAS", 'Battery Service', False, True)
profileHogpComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

profileAncsComponent = Module.CreateComponent('PROFILE_ANCS', 'Apple Notification Center Service Profile', 'Wireless/Drivers/BLE/Profiles', '/driver/ble/config/profiles/ancs.py')
profileAncsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)
