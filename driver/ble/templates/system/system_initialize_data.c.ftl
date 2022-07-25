#define QUEUE_LENGTH_BLE        (32)
#define QUEUE_ITEM_SIZE_BLE     (sizeof(void *))
<#if APP_BLE_DEV == "Buckland">
#define EXT_COMMON_MEMORY_SIZE  (26*1024)
</#if>
OSAL_QUEUE_HANDLE_TYPE bleRequestQueueHandle;
