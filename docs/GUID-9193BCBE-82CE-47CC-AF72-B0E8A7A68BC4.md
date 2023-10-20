# BLE\_ANCS\_EvtNtfyInd\_T

**Parent topic:**[Structures](GUID-9D4040A1-4922-48F6-BA70-EB4B094F9B91.md)

## C

```c
typedef struct
{
    uint16_t                    connHandle;
    uint32_t                    ntfyId;
    BLE_ANCS_NtfyEvtFlagMask_T  ntfyEvtFlagMask;
    BLE_ANCS_CategoryId_T       categoryId;
    uint8_t                     categoryCount;
} BLE_ANCS_EvtNtfyInd_T;
```

## Description

Data structure for **[BLE\_ANCS\_EVT\_NTFY\_ADDED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**, **[BLE\_ANCS\_EVT\_NTFY\_MODIFIED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)**, **[BLE\_ANCS\_EVT\_NTFY\_REMOVED\_IND](GUID-4BB8FD63-76FD-4D39-8EEF-FBC86B43A2DC.md)** event.

## Field Documentation

|Field|Description|
|-----|-----------|
|connHandle|Connection handle associated with this connection.|
|ntfyId|Notification UID. A 32-bit numerical value that is the unique identifier \(UID\) for the iOS notification.|
|ntfyEvtFlagMask|Bitmask to signal whether a special condition applies to the notification. For example, "Silent" or "Important".|
|categoryId|Classification of the notification type. For example, email or location.|
|categoryCount|Current number of active iOS notifications in the given category.|

