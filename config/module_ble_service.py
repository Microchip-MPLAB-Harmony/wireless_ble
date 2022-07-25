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

print('Load Module: Harmony Wireless BLE Services')


## BLE Services
serviceTrsComponent = Module.CreateComponent('SERVICE_TRS', 'Transparent Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/trs.py')
serviceTrsComponent.addCapability('BLE_TRS_Capability', 'BLE_TRS', None, False)
serviceTrsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceTrcbsComponent = Module.CreateComponent('SERVICE_TRCBS', 'Transparent Credit Based Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/trcbs.py')
serviceTrcbsComponent.setDisplayType('Transparent Credit Based Service')
serviceTrcbsComponent.addCapability('BLE_TRCBS_Capability', 'BLE_TRCBS', None, False)
serviceTrcbsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)
 
serviceDisComponent = Module.CreateComponent('SERVICE_DIS', 'Device Information Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/dis.py')
serviceDisComponent.addCapability('BLE_DIS_Capability', 'BLE_DIS', True)
serviceDisComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceBasComponent = Module.CreateComponent('SERVICE_BAS', 'Battery Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/bas.py')
serviceBasComponent.addMultiCapability("BLE_BAS_Capability", "BLE_BAS", "BLE_BAS")
serviceBasComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', None, True, True)

serviceCmsComponent = Module.CreateComponent('SERVICE_CMS', 'Customized Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/cms.py')
serviceCmsComponent.addCapability('BLE_CMS_Capability', 'BLE_CMS', True)
serviceCmsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceLlsComponent = Module.CreateComponent('SERVICE_LLS', 'Link Loss Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/lls.py')
serviceLlsComponent.addCapability('BLE_LLS_Capability', 'BLE_LLS', None, False)
serviceLlsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceTpsComponent = Module.CreateComponent('SERVICE_TPS', 'TX Power Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/tps.py')
serviceTpsComponent.addCapability('BLE_TPS_Capability', 'BLE_TPS', None, False)
serviceTpsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', None, True, True)

serviceIasComponent = Module.CreateComponent('SERVICE_IAS', 'Immediate Alert Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/ias.py')
serviceIasComponent.addCapability('BLE_IAS_Capability', 'BLE_IAS', None, False)
serviceIasComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceOtasComponent = Module.CreateComponent('SERVICE_OTAS', 'Over-The-Air Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/otas.py')
serviceOtasComponent.addCapability("BLE_OTAS_Capability", "BLE_OTAS", None, False)
serviceOtasComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceAnsComponent = Module.CreateComponent('SERVICE_ANS', 'Alert Notification Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/ans.py')
serviceAnsComponent.addCapability('BLE_ANS_Capability', 'BLE_ANS', None, False)
serviceAnsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)

serviceHidsComponent = Module.CreateComponent('SERVICE_HIDS', 'Human Interface Devices Service', 'Wireless/Driver/BLE/Services', '/driver/ble/config/services/hids.py')
serviceHidsComponent.addCapability("BLE_HIDS_Capability", "BLE_HIDS", None, False)
serviceHidsComponent.addDependency('BLE_STACK_Dependency', 'BLE_STACK', 'BLE Stack', True, True)
