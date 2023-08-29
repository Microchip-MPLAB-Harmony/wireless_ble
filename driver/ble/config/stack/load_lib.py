''' File:   ble_stack_load_lib.py
    Date:   21-Jul-2020

    This module copies the ble_stack source files to the H3 generated project
    and adds the files to the MPLAB X IDE project setting.
    This is only intended for internal Microchip use.  For public release the
    ble_stack will be provided as a binary library.
'''


print('BLE LIB: loading BLE Stack library')

pic32cx_bz2_family = {'PIC32CX1012BZ25048',
                      'PIC32CX1012BZ25032',
                      'PIC32CX1012BZ24032',
                      'WBZ451',
                      'WBZ450',
                      }

pic32cx_bz3_family = {'PIC32CX5109BZ31032',
                      'PIC32CX5109BZ31048',
                      'WBZ351',
                      'WBZ350',
                      }


ble_stack_header = [
    ('ble_gap.h'), 
    ('ble_l2cap.h'), 
    ('ble_smp.h'), 
    ('att_uuid.h'),
    ('gap_defs.h'),
    ('gatt.h'),
    ('mba_error_defs.h'),
    ('stack_mgr.h'),
    ('bt_sys.h'),
    ('ble_dtm.h'),
    ('bt_sys_log.h'),
    ('host_hci_defs.h'),
    ('bt_sys_pta.h'),
    ('hci.h'),
    ]

ble_stack_includes = [
    ('../src/app_ble'),
    ('../src/config/' + configName + '/ble/lib/include'),
    ('../src/config/' + configName + '/ble/middleware_ble'),
    ('../src/config/' + configName + '/driver/pds/include'),
    ('../src/config/' + configName + '/ble/profile_ble'),
    ('../src/config/' + configName + '/ble/service_ble'),
    ]

def bleConfigXC32Setting(component, key, value):
    #print('ble_stack_load_src setting C32 config {} to {}'.format(key, value))
    blestack_macros = component.createSettingSymbol("BLE_STACK_C32_CONFIG", None)
    blestack_macros.setCategory("C32")
    blestack_macros.setKey(key)
    blestack_macros.setValue(value)


processor = Variables.get("__PROCESSOR")

if( processor in pic32cx_bz2_family):
    srcPath = "ble_src_bz2"
else:
    srcPath = "ble_src_bz3"

# Add header files
for name in ble_stack_header:
    fsymbol = libBLEStackComponent.createFileSymbol(None, None)
    fsymbol.setSourcePath('driver/ble/src/' + srcPath + '/include/'+ name)
    fsymbol.setOutputName(name)
    fsymbol.setOverwrite(True)
    fsymbol.setDestPath('/ble/lib/include')
    fsymbol.setProjectPath('config/' + configName + '/ble/lib/include')
    fsymbol.setType('HEADER')
    fsymbol.setEnabled(True)

# Add extra include declaration
n = 0
bleAddonPath = {}
for name in ble_stack_includes:
    #print('{} ble_stack_load_lib: include {}'.format(n, name))
    bleAddonPath[n] = libBLEStackComponent.createSettingSymbol("BLE_ADDON_PATH_" + str(n), None)
    bleAddonPath[n].setValue(name)
    bleAddonPath[n].setCategory("C32")
    bleAddonPath[n].setKey("extra-include-directories")
    bleAddonPath[n].setAppend(True, ";")
    bleAddonPath[n].setEnabled(True)
    n = n + 1


# The initial H3 ble_stack source compiles with many warnings.  To allow
# the compile to continue, the XC32 setting 'make-warnings-into-errors'
# must be turned off.
bleConfigXC32Setting(libBLEStackComponent, 'make-warnings-into-errors', 'false')
