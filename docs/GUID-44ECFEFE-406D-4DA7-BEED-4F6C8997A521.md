# BT\_SYS\_TraceEnable

**Parent topic:**[Functions](GUID-BCD34C15-EAC6-45F0-97B7-E2EBA942CFEE.md)

## C

```c
void BT_SYS_TraceEnable(BT_SYS_TraceFmt_T fmt, uint32_t traceMask, BT_SYS_TraceEventCb_T traceCb);
```

## Description

Enable BT trace message indication.

## Parameters

|Parameter|Description|
|---------|-----------|
|\[in\] fmt|BT system trace format. See **[Trace format](GUID-ED109984-DEAA-480F-855C-8DB3B946557D.md)**.|
|\[in\] traceMask|BT system trace mask. See **[Trace mask](GUID-7D3B184F-37C8-494D-B415-C4FE08A36C1E.md)**.|
|\[in\] traceCb|BT system trace callback.|

