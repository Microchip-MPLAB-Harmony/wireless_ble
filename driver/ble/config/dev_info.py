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

def GetDeviceFamily():
    processor = Variables.get("__PROCESSOR")

    if processor in pic32cx_bz2_family:
        return "pic32cx_bz2_family"
    elif processor in pic32cx_bz3_family:
        return "pic32cx_bz3_family"
    else:
        return "Not Support"