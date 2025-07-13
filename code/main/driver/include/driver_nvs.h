#pragma once

#include "project_config.h"

/**
 * @brief
 * 存储的配置信息
 */
#define NVS_NAMESPACE "YJC_NAMESPACE"
#define NVS_KEY "YJC_CONFIG_KEY"
#define NVS_STATUS_KEY "YJC_STATUS_KEY"

/**
 * @brief
 * 配置信息
 */
typedef struct
{
    char uuid[50];
    char ssid[50];
    char password[50];
    char url[50];
    char port[50];
    char version[50];
    char project[50];
    int reset;
} configStruct;

// /**
//  * 状态信息
//  */
// typedef struct
// {
//     // 选择的WiFi名称
//     char ssid[50];
//     // 选择的WiFi密码
//     char password[50];
//     int reset;
// } statusStruct;

/**
 * @brief
 * 存储配置信息
 * @return esp_err_t
 */
esp_err_t nvs_save_config(configStruct config);

// /**
//  * 存储重启状态信息
//  */
// esp_err_t nvs_saveStatus_config(statusStruct config);

/**
 * @brief
 * 获取配置信息
 * @return configStruct
 */
configStruct nvs_get_config();

// /**
//  * 获取状态信息
//  */
// statusStruct nvs_getStatus_config();

/**
 * @brief
 * 删除配置信息
 * @return esp_err_t
 */
esp_err_t nvs_delete_config();

/**
 * 初始化芯片唯一值，取mac
 */
esp_err_t get_and_save_mac_address();

/**
 * @brief
 * 初始化nvs
 * @return esp_err_t
 */
esp_err_t init_nvs_with_retry(void);
