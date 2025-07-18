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

print('Load Module: Harmony Wireless BLE Stack Library')

## BLE Stack/Library
bleStackLibComponent = Module.CreateComponent('BLE_STACK_LIB', 'BLE Stack', 'Wireless/Drivers/BLE', 'driver/ble/config/stack/blestack.py')
bleStackLibComponent.setDisplayType('BLE Stack Library')
bleStackLibComponent.addDependency('PIC32CX_BZ2_DevSupport_Dependency', 'Device_Support', None, True, True)
bleStackLibComponent.addDependency("BLE_CRYPTO", "LIB_CRYPTO", None, True, True)
bleStackLibComponent.addDependency("BLE_WolfCrypt_Dependency", "LIB_WOLFCRYPT", None, False, True)
bleStackLibComponent.addCapability('BLE_Stack_Capability', 'BLE_STACK', True)
bleStackLibComponent.setHelpKeyword("WIRELESS_BLE_SW")