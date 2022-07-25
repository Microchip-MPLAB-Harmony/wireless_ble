
    if (xTaskCreate(BM_Task,     "BLE", TASK_BLE_STACK_SIZE, NULL  , TASK_BLE_PRIORITY, NULL) != pdPASS)
        while (1);
