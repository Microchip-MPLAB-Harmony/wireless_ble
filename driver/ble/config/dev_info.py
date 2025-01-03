def GetDeviceFamily():
    #processor = Variables.get("__PROCESSOR")

    if ("PIC32CX" in Variables.get("__PROCESSOR")) or ("WBZ" in Variables.get("__PROCESSOR")) or ("PIC32WM" in Variables.get("__PROCESSOR")):
        if "BZ2" in Variables.get("__PROCESSOR") or "WBZ45" in Variables.get("__PROCESSOR"):
            return "pic32cx_bz2_family"
        elif "BZ3" in Variables.get("__PROCESSOR") or "WBZ35" in Variables.get("__PROCESSOR"):
            return "pic32cx_bz3_family"
        elif "BZ6" in Variables.get("__PROCESSOR") or "WBZ65" in Variables.get("__PROCESSOR"):
            return "pic32cx_bz6_family"
        else:
            return "Not Support"