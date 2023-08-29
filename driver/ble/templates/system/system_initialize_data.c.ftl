#define QUEUE_LENGTH_BLE        (32)
#define QUEUE_ITEM_SIZE_BLE     (sizeof(void *))
#define EXT_COMMON_MEMORY_SIZE  (28*1024)
<#if APP_BLE_DEVICE == "pic32cx_bz2">
static uint8_t __attribute__((section (".bss.s_btMem"), noload, address(0x20018C00))) s_btMem[EXT_COMMON_MEMORY_SIZE];
</#if>
OSAL_QUEUE_HANDLE_TYPE bleRequestQueueHandle;
