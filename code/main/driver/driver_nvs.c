
#include "driver_nvs.h"
#include "project_config.h"

/**
 * @brief
 * 存储配置信息
 * @param config
 * @return esp_err_t
 */
esp_err_t nvs_save_config(configStruct config)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    nvs_handle handle;
    ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle));
    ESP_ERROR_CHECK(nvs_set_blob(handle, NVS_KEY, &config, sizeof(config)));
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
    return ESP_OK;
}

// /**
//  * @brief
//  * 存储配置信息
//  * @param config
//  * @return esp_err_t
//  */
// esp_err_t nvs_saveStatus_config(statusStruct config)
// {
//     ESP_ERROR_CHECK(nvs_flash_init());
//     nvs_handle handle;
//     ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle));
//     ESP_ERROR_CHECK(nvs_set_blob(handle, NVS_STATUS_KEY, &config, sizeof(config)));
//     ESP_ERROR_CHECK(nvs_commit(handle));
//     nvs_close(handle);
//     return ESP_OK;
// }

/**
 * @brief
 * 获取配置参数
 * @return configStruct
 */
configStruct nvs_get_config()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    nvs_handle handle;
    configStruct configStGet = {0};
    esp_err_t error = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (error != ESP_OK)
    {
        logger_log(TAG, "nvs_open error = %s\n", esp_err_to_name(error));
        return configStGet;
    }
    size_t len = sizeof(configStGet);
    error = nvs_get_blob(handle, NVS_KEY, &configStGet, &len);
    if (error != ESP_OK)
    {
        // logger_log(TAG, "nvs_get_blob error = %s\n", esp_err_to_name(error));
        return configStGet;
    }
    nvs_close(handle);
    return configStGet;
}

// statusStruct nvs_getStatus_config()
// {
//     ESP_ERROR_CHECK(nvs_flash_init());
//     nvs_handle handle;
//     statusStruct configStGet = {0};
//     esp_err_t error = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
//     if (error != ESP_OK)
//     {
//         logger_log(TAG, "nvs_open error = %s\n", esp_err_to_name(error));
//         return configStGet;
//     }
//     size_t len = sizeof(configStGet);
//     error = nvs_get_blob(handle, NVS_STATUS_KEY, &configStGet, &len);
//     if (error != ESP_OK)
//     {
//         // logger_log(TAG, "nvs_get_blob error = %s\n", esp_err_to_name(error));
//         return configStGet;
//     }
//     nvs_close(handle);
//     return configStGet;
// }

/**
 * @brief
 * 擦除nvs数据
 * @return esp_err_t
 */
esp_err_t nvs_delete_config()
{
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());
    return ESP_OK;
}

/**
 * 初始化mac地址
 */
esp_err_t get_and_save_mac_address()
{
    // 定义Mac地址存储空间
    uint8_t mac_addr[6] = {0};
    // 获取Mac地址
    esp_efuse_mac_get_default(mac_addr);
    logger_log(TAG, "mac: %02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    // 获取固件版本
    const esp_app_desc_t *desc = esp_app_get_description();
    logger_log(TAG, "project_name: %s", desc->project_name);
    logger_log(TAG, "version: %s", desc->version);
    configStruct config = nvs_get_config();
    if (strlen(config.uuid) == 0)
    {
        sprintf(config.uuid, "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    }
    sprintf(config.version, "%s", desc->version);
    sprintf(config.project, "%s", desc->project_name);
    return nvs_save_config(config);
}


/**
 * @brief
 * 初始化nvs
 * @return esp_err_t
 */
esp_err_t init_nvs_with_retry(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // 如果 NVS 分区版本不匹配或没有空闲页，则擦除整个分区
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}