# Operation of extended advertising data

**Parent topic:**[Defines](GUID-FB430BFE-A9A9-473D-A588-1240BBD25ADD.md)

## Description

BLE LIB supports both legacy and extended advertising features. Since there exists usage limitation defined by specification, the following tables list the legacy and extended advertising APIs and events.

In Spec. Vol. 4, Part E, Sec. 3.1, there's limitation when controller supports both legacy and extended advertising:<br />If, since the last power-on or reset, the Host has ever issued a legacy advertising command and then issues an extended advertising command, or has ever issued an extended advertising command and then issues a legacy advertising command, the command will be disallowed.

## APIs

|Legacy advertising API|Extended advertising API|
|----------------------|------------------------|
|BLE\_GAP\_SetAdvParams|BLE\_GAP\_SetExtAdvParams|
|BLE\_GAP\_SetAdvData|BLE\_GAP\_SetExtAdvData|
|BLE\_GAP\_SetScanRspData|BLE\_GAP\_SetExtScanRspData|
|BLE\_GAP\_SetAdvEnable|BLE\_GAP\_SetExtAdvEnable|
|none|BLE\_GAP\_RemoveExtAdvSet|
|none|BLE\_GAP\_ClearExtAdvSet|
|none|BLE\_GAP\_SetPeriAdvParams|
|none|BLE\_GAP\_SetPeriAdvData|
|none|BLE\_GAP\_SetPeriAdvEnable|
|BLE\_GAP\_SetScanningParam|BLE\_GAP\_SetExtScanningParams|
|BLE\_GAP\_SetScanningEnable|BLE\_GAP\_SetExtScanningEnable|
|BLE\_GAP\_CreateConnection|BLE\_GAP\_ExtCreateConnection|
|none|BLE\_GAP\_CreateSync|
|none|BLE\_GAP\_CreateSyncCancel|
|none|BLE\_GAP\_TerminateSync|
|none|BLE\_GAP\_SetDevToPeriAdvList|
|none|BLE\_GAP\_SetPeriAdvRxEnable|
|BLE\_GAP\_EnableOneTimeAdv|none|

## Events

|Legacy advertising event|Extended advertising event|
|------------------------|--------------------------|
|BLE\_GAP\_EVT\_ADV\_REPORT|BLE\_GAP\_EVT\_EXT\_ADV\_REPORT|

