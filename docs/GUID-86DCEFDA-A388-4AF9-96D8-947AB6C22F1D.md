# Definition of TRCBPS parameters

**Parent topic:**[Defines](GUID-C11241E1-5A96-4825-ABA7-B4045381B602.md)

## C

```c
#define BLE_TRCBPS_SDU_LEN_FIELD_LEN          0x02
#define BLE_TRCBPS_CTRL_MTU                   BLE_ATT_MAX_MTU_LEN - BLE_TRCBPS_SDU_LEN_FIELD_LEN
#define BLE_TRCBPS_DATA_MTU                   BLE_ATT_MAX_MTU_LEN - BLE_TRCBPS_SDU_LEN_FIELD_LEN
#define BLE_TRCBPS_CTRL_MPS                   BLE_L2CAP_MAX_PDU_SIZE
#define BLE_TRCBPS_DATA_MPS                   BLE_L2CAP_MAX_PDU_SIZE
#define BLE_TRCBPS_CTRL_MAX_CREDITS           0x0002
#define BLE_TRCBPS_CTRL_MAX_ACCU_CREDITS      0x0001
#define BLE_TRCBPS_DATA_MAX_CREDITS           0x0008
#define BLE_TRCBPS_DATA_MAX_ACCU_CREDITS      0x0005
#define BLE_TRCBPS_PERMISSION                 0x00
```

## Description

The definition of BLE Transparent Credit Based Profile parameters.

## Macro Definition

|Macro|Description|
|-----|-----------|
|BLE\_TRCBPS\_SDU\_LEN\_FIELD\_LEN|Length of the SDU length field.|
|BLE\_TRCBPS\_CTRL\_MTU|MTU size of control channel.|
|BLE\_TRCBPS\_DATA\_MTU|MTU size of data channel.|
|BLE\_TRCBPS\_CTRL\_MPS|MPS size of control channel.|
|BLE\_TRCBPS\_DATA\_MPS|MPS size of data channel.|
|BLE\_TRCBPS\_CTRL\_MAX\_CREDITS|Maximum credit value of control channel.|
|BLE\_TRCBPS\_CTRL\_MAX\_ACCU\_CREDITS|Maximum accumulation credits which will be sent to the peer device of control channel.|
|BLE\_TRCBPS\_DATA\_MAX\_CREDITS|Maximum credit value of data channel.|
|BLE\_TRCBPS\_DATA\_MAX\_ACCU\_CREDITS|Maximum accumulation credits which will be sent to the peer device of data channel.|
|BLE\_TRCBPS\_PERMISSION|Permission setting.|
