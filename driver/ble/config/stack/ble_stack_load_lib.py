''' File:   ble_stack_load_src.py
    Date:   21-Jul-2020

    This module copies the ble_stack source files to the H3 generated project
    and adds the files to the MPLAB X IDE project setting.
    This is only intended for internal Microchip use.  For public release the
    ble_stack will be provided as a binary library.
'''


print('BLE LIB: loading BLE Stack library')

processor = 0
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

def add_ble_lib_files(component, flist, ftype, parent, config):
    """
    Create H3 File Symbols for a list of ble_lib source files

    The source files are located in the H3 module folder under:
        driver/ble/src/ble_lib/<parent>/<folder>/<path>/<fname>

    The files are copied to the project path config/<config>:
        lib_src/ble_stack/<parent>/<folder>/<path>'

    The files are added to the MPLAB X IDE project based on the
    type of file; SOURCE or HEADER.  The files are added to the
    MPLABX IDE logical folder under:
        ('config/<config>/lib_src/ble_stack/<parent>/<path>'
        
    Where:
        <parent> and <config> are passed in as parameters.
        <path> and <fname> are passed in flist
        <folder> is derived from <ftype>
            <ftype> == 'SOURCE' then <folder> = 'src'
            <ftype> == 'HEADER' then <folder> = 'inc'

    Parameters
    ----------
    component   : (localComponent)
                The H3 component parameter to instantiateComponent()
    flist       : list of tuples
                The list contains the paths and filenames
    parent      : string
                The name of the folder containing the path and file
    config      : string
                The name of the MPLAB X IDE project's config folder
    """
    processor = Variables.get("__PROCESSOR")

    if( processor in pic32cx_bz2_family):
        srcPath = "ble_src_bz2"
    else:
        srcPath = "ble_src_bz3"

    #print('ble_load_src:add_ble_lib_files(comp, list, {}, {}, {})'.format(ftype, parent, config))

    folder = ''

    for path, fname in flist:

        #print('  parent {}, path {}, fname {}'.format(parent, path, fname))
        fsymbol = component.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver\\ble\\src\\' + srcPath + '\\{}\\{}\\{}'.format(parent, path, fname))
        fsymbol.setOutputName(fname)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('/ble/lib/{}/{}/{}'.format(parent, folder, path))
        fsymbol.setProjectPath('config/{}/ble/lib/{}/{}'.format(config, parent, path))
        fsymbol.setType(ftype)
        fsymbol.setEnabled(True)


#def add_lib_util_files(component, flist, ftype, parent, config):
#
#    print('ble_load_src:add_lib_util_files(comp, flist[0] {}, {}, {}, {})'.format(repr(flist[0]), ftype, parent, config))
#
#    if 'SOURCE' == ftype:
#        folder = 'src/'
#    elif 'HEADER' == ftype:
#        folder = 'inc/'
#    else:
#        folder = ''
#
#
#    for path, fname in flist:
#
#        print('  parent {}, path {}, fname {}'.format(parent, path, fname))
#        fsymbol = component.createFileSymbol(None, None)
#        fsymbol.setSourcePath('driver\\ble\\src\\{}\\{}\\{}'.format(parent, path, fname))
#        fsymbol.setOutputName(fname)
#        fsymbol.setOverwrite(True)
#        fsymbol.setDestPath('\\lib\\{}\\{}'.format(parent, path))
#        fsymbol.setProjectPath('config\\{}\\lib\\ble\\{}\\{}'.format(config, parent, path))
#        fsymbol.setType(ftype)
#        fsymbol.setEnabled(True)


def addExtraIncludes(component, plist):
    # print('ble_stack_load_src:addExtraIncludes' + ''.join(plist))
    blestack_includes = component.createSettingSymbol("BLE_STACK_SRC_INCLUDES", None)
    blestack_includes.setCategory("C32")
    blestack_includes.setKey("extra-include-directories")
    blestack_includes.setValue(';'.join(plist))
    blestack_includes.setAppend(True, ";")


#def addPreprocessorMacros(component, mlist):
#    # print('ble_stack_load_src:addPreprocessorMacros' + ''.join(mlist))
#    blestack_macros = component.createSettingSymbol("BLE_STACK_SRC_MACROS", None)
#    blestack_macros.setCategory("C32")
#    blestack_macros.setKey("preprocessor-macros")
#    blestack_macros.setValue(';'.join(mlist))
#    blestack_macros.setAppend(True, ";")


def setConfigItemC32(component, key, value):
    #print('ble_stack_load_src setting C32 config {} to {}'.format(key, value))
    blestack_macros = component.createSettingSymbol("BLE_STACK_C32_CONFIG", None)
    blestack_macros.setCategory("C32")
    blestack_macros.setKey(key)
    blestack_macros.setValue(value)



ble_stack_header = [
    ('', 'ble_gap.h'), 
    ('', 'ble_l2cap.h'), 
    ('', 'ble_smp.h'), 
    ('', 'att_uuid.h'),
    ('', 'gap_defs.h'),
    ('', 'gatt.h'),
    ('', 'mba_error_defs.h'),
    ('', 'stack_mgr.h'),
    ('', 'bt_sys.h'),
    ('', 'ble_dtm.h'),
    ('', 'bt_sys_log.h'),
    ('', 'host_hci_defs.h'),
    
    ]

#utils_header_files = [
#    ('', 'compiler.h'),
#    ('', 'status_codes.h'),
#    ('preprocessor', 'preprocessor.h'),
#    ('preprocessor', 'tpaste.h'),
#    ('preprocessor', 'stringz.h'),
#    ('preprocessor', 'mrepeat.h'),
#    ('preprocessor', 'mrecursion.h'),
#    ('', 'interrupt.h'),
#    ('interrupt', 'interrupt_sam_nvic.h'),
#    ('calibration', 'calibration.h'),
#    ('calibration', 'info_block.h'),
#    ('', 'clock.h'),
#    ('rf', 'rf.h'),
#    ]


ble_stack_includes = [
    '../src/app_ble',
    '../src/config/' + configName + '/ble/lib/include',
    '../src/config/' + configName + '/ble/middleware_ble',
    '../src/config/' + configName + '/driver/pds/include',
    '../src/config/' + configName + '/ble/profile_ble',
    '../src/config/' + configName + '/ble/service_ble',
    ]


#ble_stack_macros = [
#    'PLATFORM=PLATFORM_CHIMERA',
#    'BT_EMBEDDED_MODE',
#    ]


add_ble_lib_files(libBLEStackComponent, ble_stack_header, 'HEADER', 'include', configName)
#add_lib_util_files(libBLEStackComponent, utils_header_files, 'HEADER', 'utils', configName)


addExtraIncludes(libBLEStackComponent, ble_stack_includes)
#addPreprocessorMacros(libBLEStackComponent, ble_stack_macros)


# The initial H3 ble_stack source compiles with many warnings.  To allow
# the compile to continue, the XC32 setting 'make-warnings-into-errors'
# must be turned off.
setConfigItemC32(libBLEStackComponent, 'make-warnings-into-errors', 'false')
