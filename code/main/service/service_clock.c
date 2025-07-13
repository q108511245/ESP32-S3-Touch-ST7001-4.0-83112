#include "service_clock.h"

/**
 * @brief 从NVS中删除指定索引的结构体数据
 * 
 * @param index 要删除的索引
 * @return esp_err_t 操作结果
 */
esp_err_t delete_clock_data_at_index(size_t index)
{
    nvs_handle_t handle;
    esp_err_t err;

    // 打开 NVS 命名空间
    err = nvs_open(DATA_NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        logger_log(TAG, "nvs_open error = %s\n", esp_err_to_name(err));
        return err;
    }

    // 获取当前 blob 的大小
    size_t required_size = 0;
    err = nvs_get_blob(handle, DATA_NVS_KEY, NULL, &required_size);
    if (err != ESP_OK)
    {
        logger_log(TAG, "nvs_get_blob error = %s\n", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 分配内存并读取数据
    clock_data_t *data = malloc(required_size);
    err = nvs_get_blob(handle, DATA_NVS_KEY, data, &required_size);
    if (err != ESP_OK)
    {
        logger_log(TAG, "nvs_get_blob error = %s\n", esp_err_to_name(err));
        free(data);
        nvs_close(handle);
        return err;
    }

    size_t count = required_size / sizeof(clock_data_t);

    // 检查索引是否合法
    if (index >= count)
    {
        logger_log(TAG, "Index out of bounds\n");
        free(data);
        nvs_close(handle);
        return ESP_ERR_INVALID_ARG;
    }

    // 删除指定索引的数据：将后面的元素前移一位
    for (size_t i = index; i < count - 1; i++)
    {
        data[i] = data[i + 1];
    }

    // 缩小数组长度
    count--;

    // 更新 NVS 中的数据
    err = nvs_set_blob(handle, DATA_NVS_KEY, data, count * sizeof(clock_data_t));
    if (err != ESP_OK)
    {
        logger_log(TAG, "nvs_set_blob error = %s\n", esp_err_to_name(err));
    }
    else
    {
        nvs_commit(handle); // 提交更改
    }

    // 清理资源
    free(data);
    nvs_close(handle);

    return err;
}

// 保存结构体数组到NVS
esp_err_t save_struct_list(clock_data_t *data, size_t count)
{
    nvs_handle_t handle;
    esp_err_t error = nvs_open(DATA_NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (error != ESP_OK)
    {
        logger_log(TAG, "nvs_open error = %s\n", esp_err_to_name(error));
        return error;
    }
    // 先写入数量
    esp_err_t err = nvs_set_blob(handle, DATA_NVS_KEY, data, count * sizeof(clock_data_t));
    nvs_commit(handle);
    nvs_close(handle);
    return err;
}

// 从NVS读取结构体数组
esp_err_t load_struct_list(clock_data_t **data, size_t *count)
{
    nvs_handle_t handle;
    esp_err_t error = nvs_open(DATA_NVS_NAMESPACE, NVS_READONLY, &handle);
    if (error != ESP_OK)
    {
        logger_log(TAG, "nvs_open error = %s\n", esp_err_to_name(error));
        return error;
    }
    size_t required_size = 0;
    esp_err_t err = nvs_get_blob(handle, DATA_NVS_KEY, NULL, &required_size);
    if (err != ESP_OK)
    {
        nvs_close(handle);
        return err;
    }

    *data = malloc(required_size);
    *count = required_size / sizeof(clock_data_t);
    err = nvs_get_blob(handle, DATA_NVS_KEY, *data, &required_size);
    nvs_close(handle);
    return err;
}
