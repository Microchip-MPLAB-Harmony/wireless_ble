# PTA Software Design Information

-   **PTA APIs in BLE\_Stack library**: Two APIs are designed in BLE\_Stack library.
    -   BT\_SYS\_PtaInit: It is called within APP\_PtaInit\(\) to enable BLE stack PTA feature.
    -   BT\_SYS\_PtaWlanActiveIrqHandler: It is called within APP\_PtaWlanActiveIrqHandler\(\) to handle WLAN\_ACTIVE signal.
-   **MCC configuration**: Below MCC configurations will apply when BLE MCC component PTA option is enabled.
    -   Modify GPIO MCC configuration to fit PTA pin definition.
    -   Including app\_pta\_handler.c and app\_pta\_handler.h to project.
    -   Generate application template code to call APP\_PtaInit\(\) in APP\_BleStackInitAdvance\(\).
-   **BT Request priority design:** Except for the BT device is scanning for advertising packets or RSSI measurements for channel classification, others BT activities are high priority.

**Parent topic:**[BLE PTA](GUID-C76FA981-3CAC-4973-AE4F-8FFEE405F570.md)

