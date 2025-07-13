#include "service_ota.h"
#include "esp_ota_ops.h"
#include "esp_https_ota.h"
#include "esp_heap_caps.h"

extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

// 定义取消标志
bool cancelOtaUpgrade = false;

// OTA 升级任务句柄
TaskHandle_t ui_task_handle = NULL;

/**
 * @brief OTA 升级进度信息
 */
ProgressInfo info = {0};

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        logger_log(TAG, "HTTP_EVENT_ERROR");
        // setUpgradeText("开始升级", "0%", 0);
        break;
    case HTTP_EVENT_ON_CONNECTED:
        logger_log(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        logger_log(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        logger_log(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // 获取已接收的数据长度 evt->data_len
            static size_t total_received = 0;
            total_received += evt->data_len;
            // 获取总文件大小（Content-Length）
            int content_length = esp_http_client_get_content_length(evt->client);
            // 计算并打印进度
            float progress = ((float)total_received / content_length) * 100;

            strncpy(info.status, "正在下载", sizeof(info.status));
            snprintf(info.percent, sizeof(info.percent), "%.0f%%", progress);
            info.progress = (int16_t)progress;
            // logger_log(TAG, "Download Progress: %.2f%%", progress);
            send_xQueue(UI_OTA_UPDATING_PROGRESS);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        logger_log(TAG, "HTTP_EVENT_ON_FINISH");
        // setUpgradeText("下载完成", "100%", 100);
        vTaskDelete(ui_task_handle);
        break;
    case HTTP_EVENT_DISCONNECTED:
        logger_log(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        logger_log(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

void upgrade()
{
    // 从 PSRAM 分配自定义 HTTP 客户端缓冲区（示例）
    char *cert_pem = (char *)heap_caps_malloc(4096, MALLOC_CAP_SPIRAM);
    if (cert_pem != NULL)
    {
        memcpy(cert_pem, server_cert_pem_start, 4096);
        esp_http_client_config_t config = {
            .url = OTAURL,
            .cert_pem = (char *)cert_pem,
            .event_handler = _http_event_handler,
            .keep_alive_enable = true,
        };
        esp_https_ota_config_t ota_config = {
            .http_config = &config,
        };
        esp_https_ota_handle_t ota_handle = NULL;
        esp_err_t err = esp_https_ota_begin(&ota_config, &ota_handle);
        if (err != ESP_OK)
        {
            logger_log(TAG, "OTA begin failed");
            free(cert_pem);
            return;
        }
        // 循环执行分片处理
        while (1)
        {
            if (cancelOtaUpgrade) // 检查取消标志
            {
                logger_log(TAG, "OTA upgrade canceled");
                esp_https_ota_abort(ota_handle); // 终止 OTA 升级
                break;
            }
            esp_err_t ret = esp_https_ota_perform(ota_handle);
            if (ret == ESP_ERR_HTTPS_OTA_IN_PROGRESS)
            {
                // 插入小延时，释放 CPU 给 UI 任务刷新
                vTaskDelay(pdMS_TO_TICKS(200));
                continue; // 未完成则继续
            }
            else if (ret != ESP_OK)
            {
                logger_log(TAG, "OTA perform failed: %s", esp_err_to_name(ret));
                break;
            }
            else
            {
                logger_log(TAG, "OTA perform finished");
                break;
            }
        }

        free(cert_pem);
        if (!cancelOtaUpgrade)
        {
            // 结束处理
            esp_https_ota_finish(ota_handle);
            esp_restart(); // 只有在未取消的情况下重启设备
        }
        else
        {
            cancelOtaUpgrade = false; // 重置标志以便下次使用
        }
    }
}

void ota_task(void *pvParameter)
{
    logger_log(TAG, "ota start");
    upgrade();         // 执行 OTA 下载和升级
    vTaskDelete(NULL); // 任务完成后自删除
}

void start_ota_upgrade(void)
{
    xTaskCreate(&ota_task, "ota_task", 1024 * 5, NULL, 1, NULL);
}

void cancel_ota_upgrade(void)
{
    // 设置取消标志
    cancelOtaUpgrade = true;
}