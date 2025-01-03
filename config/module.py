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
'''
    pic32cx_bz2_family = {'PIC32CX1012BZ25048',
                          'PIC32CX1012BZ25032',
                          'PIC32CX1012BZ24032',
                          'WBZ451',
                          'WBZ450',
                          'WBZ451H',
                         }

    pic32cx_bz3_family = {'PIC32CX5109BZ31032',
                          'PIC32CX5109BZ31048',
                          'WBZ351',
                          'WBZ350',
                         }
    
    pic32cx_bz6_family = {'PIC32CX2051BZ62132',
                          'PIC32CX2051BZ62064',
                          'PIC32CX2051BZ66048',
                          'WBZ653',
                          'WBZ652',
                          'WBZ651',
                         }
'''
def loadModule():
    print('Load Module: Harmony Wireless')

    print('processor={}'.format(Variables.get("__PROCESSOR")))

    if ("PIC32CX" in Variables.get("__PROCESSOR")) or ("WBZ" in Variables.get("__PROCESSOR")) or ("PIC32WM" in Variables.get("__PROCESSOR")):
        if "BZ2" in Variables.get("__PROCESSOR") or "WBZ45" in Variables.get("__PROCESSOR"):
            ## PIC32CX-BZ Radio stacks
            execfile(Module.getPath() + '/config/module_pic32cx_bz2_ble.py')
        
            execfile(Module.getPath() + '/config/module_ble_service.py')
        
            execfile(Module.getPath() + '/config/module_ble_profile.py')

        elif "BZ3" in Variables.get("__PROCESSOR") or "WBZ35" in Variables.get("__PROCESSOR"):
            ## PIC32CX-BZ3 Radio stacks
            execfile(Module.getPath() + '/config/module_pic32cx_bz3_ble.py')
        
            execfile(Module.getPath() + '/config/module_ble_service.py')
        
            execfile(Module.getPath() + '/config/module_ble_profile.py')

        elif "BZ6" in Variables.get("__PROCESSOR") or "WBZ65" in Variables.get("__PROCESSOR"):
            ## PIC32CX-BZ6 Radio stacks
            execfile(Module.getPath() + '/config/module_pic32cx_bz6_ble.py')
        
            execfile(Module.getPath() + '/config/module_pic32cx_bz6_ble_service.py')
        
            execfile(Module.getPath() + '/config/module_pic32cx_bz6_ble_profile.py')

        else:
            return "Not Support"