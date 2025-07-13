/*
 * CopyrightText: 2023-2026 ZhangJiaGang YunJunCheng CO LTD
 * All rights reserved.
 * File Name: main.c
 * File Path: /main/main.c
 * Author: 陈建峰
 *
 */
#include "project_config.h"

char InfoBuffer[512] = {0};

/**
 * 打印内存使用情况
 */
void Simulator‌Task(void *pvParameters)
{
    while (1)
    {
        // 打印当前堆内存大小
        ESP_LOGI(TAG, "free_heap_size = %lu\n", esp_get_free_heap_size());
        // // 打印最小可用堆内存大小
        // size_t min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
        // ESP_LOGI(TAG, "minimum_free_heap_size (DEFAULT) = %zu\n", min_free_size);
        // // 内部 SRAM 内存
        // min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
        // ESP_LOGI(TAG, "minimum_free_heap_size (INTERNAL) = %zu\n", min_free_size);
        // vTaskList((char *)&InfoBuffer);
        // logger_log(TAG, "任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        // logger_log(TAG, "\r\n%s\r\n", InfoBuffer);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    // 初始化nvs
    ESP_ERROR_CHECK(init_nvs_with_retry());
    // 删除数据
    if (ifEraserDate==1)
    {
        nvs_delete_config();
    }
    logger_init(isOpenLog);
    // 初始化设备mac
    ESP_ERROR_CHECK(get_and_save_mac_address());
    // 初始化i2c
    i2c0_init();
    // 初始化队列
    ESP_ERROR_CHECK(service_ui_init());
    lcd_init();
    buzzer_init();
    // buzzer_play();
    // 初始化wifi
    wifi_init();

    if (isOpenMinTask)
    {
        xTaskCreate(Simulator‌Task, "Simulator‌Task", 1024 * 4, NULL, 3, NULL);
    }

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
