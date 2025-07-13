#pragma once

#include "project_config.h"

/**
 * @brief
 * 存储的配置信息
 */
#define DATA_NVS_NAMESPACE "CLOCK_NAMESPACE"
#define DATA_NVS_KEY "CLOCK_KEY"



typedef struct
{
    // 小时
    int hour;
    // 分钟
    int min;
    int status;
} clock_data_t;


// 保存结构体数组到NVS
esp_err_t save_struct_list(clock_data_t *data, size_t count);


// 从NVS读取结构体数组
esp_err_t load_struct_list(clock_data_t **data, size_t *count);

/**
 * @brief 从NVS中删除指定索引的结构体数据
 * 
 * @param index 要删除的索引
 * @return esp_err_t 操作结果
 */
esp_err_t delete_clock_data_at_index(size_t index);