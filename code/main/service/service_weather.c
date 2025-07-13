#include "service_weather.h"
#include "esp_http_client.h"
#include "utils_comm.h"

static char *http_data_buffer = NULL;
static int http_data_len = 0;

/**
 * 天气数据
 */
weather_data_t *weather_list_data = NULL;
size_t weather_count = 0;

void print_weather_data(weather_data_t *weatherData)
{
    for (int i = 1; i < 6; i++)
    {
        // 打印数据
        ESP_LOGI(TAG, "Day %d:", i + 1);
        ESP_LOGI(TAG, "  Day Weather: %s", weatherData[i].day_weather ? weatherData[i].day_weather : "N/A");
        ESP_LOGI(TAG, "  Night Weather: %s", weatherData[i].night_weather ? weatherData[i].night_weather : "N/A");
        ESP_LOGI(TAG, "  Day Air Temperature: %s°C", weatherData[i].day_air_temperature ? weatherData[i].day_air_temperature : "N/A");
        ESP_LOGI(TAG, "  Night Air Temperature: %s°C", weatherData[i].night_air_temperature ? weatherData[i].night_air_temperature : "N/A");
        ESP_LOGI(TAG, "  Wind Direction (Day): %s", weatherData[i].day_wind_direction ? weatherData[i].day_wind_direction : "N/A");
        ESP_LOGI(TAG, "  Wind Direction (Night): %s", weatherData[i].night_wind_direction ? weatherData[i].night_wind_direction : "N/A");
        ESP_LOGI(TAG, "  Wind Power (Day): %s", weatherData[i].day_wind_power ? weatherData[i].day_wind_power : "N/A");
        ESP_LOGI(TAG, "  Wind Power (Night): %s", weatherData[i].night_wind_power ? weatherData[i].night_wind_power : "N/A");
        ESP_LOGI(TAG, "  Weekday: %d", weatherData[i].weekday);
        ESP_LOGI(TAG, "  Jiangshui: %s", weatherData[i].jiangshui ? weatherData[i].jiangshui : "N/A");
        ESP_LOGI(TAG, "  Air Pressure: %s", weatherData[i].air_press ? weatherData[i].air_press : "N/A");
        ESP_LOGI(TAG, "  Ziwaixian: %s", weatherData[i].ziwaixian ? weatherData[i].ziwaixian : "N/A");
        ESP_LOGI(TAG, "----------------------------------------");
    }
}

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        logger_log(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        logger_log(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        logger_log(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        // logger_log(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        // 初始化缓冲区
        http_data_len = 0;
        if (http_data_buffer)
            free(http_data_buffer);
        http_data_buffer = NULL;
        break;
    case HTTP_EVENT_ON_DATA:
        logger_log(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        // 扩展缓冲区大小
        char *new_buf = realloc(http_data_buffer, http_data_len + evt->data_len + 1);
        if (new_buf == NULL)
        {
            logger_log(TAG, "Failed to allocate memory for chunked data");
            return ESP_FAIL;
        }
        http_data_buffer = new_buf;
        memcpy(http_data_buffer + http_data_len, evt->data, evt->data_len);
        http_data_len += evt->data_len;
        http_data_buffer[http_data_len] = '\0'; // 确保字符串结尾
        break;
    case HTTP_EVENT_ON_FINISH:
        logger_log(TAG, "HTTP_EVENT_ON_FINISH");
        if (http_data_buffer != NULL)
        {
            // logger_log(TAG, "Received data: %s\n", http_data_buffer);
            // 解析完整 JSON
            cJSON *root = cJSON_Parse(http_data_buffer);
            if (root == NULL)
            {
                logger_log(TAG, "Error parsing JSON");
                free(http_data_buffer);
                http_data_buffer = NULL;
                return ESP_FAIL;
            }
            // 清空数据
            memset(&weather_list_data, 0, sizeof(weather_list_data));
            memset(&weather_count, 0, sizeof(weather_count));
            cJSON *showapi_res_body = cJSON_GetObjectItemCaseSensitive(root, "showapi_res_body");
            // 添加 f1~f6 数据
            for (int i = 1; i <= 6; i++)
            {
                char fname[4];
                sprintf(fname, "f%d", i);
                cJSON *f_item = cJSON_GetObjectItemCaseSensitive(showapi_res_body, fname);
                if (f_item)
                {
                    weather_list_data = add_weather_item(weather_list_data, &weather_count, f_item);
                }
            }
            // print_weather_data(weather_list_data);
            send_xQueue(UI_WEATHER_REFRESH_ONCE);
            cJSON_Delete(root); // 释放 cJSON 对象
            free(http_data_buffer);
            http_data_buffer = NULL;
        }
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

void http_weather_task(void *pvParameters)
{
    esp_http_client_config_t config = {
        .url = weather_url,
        .method = HTTP_METHOD_GET,
        .event_handler = http_event_handler,
        .cert_pem = NULL, // 使用系统内置 CA 信任链
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // 添加自定义Header
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", app_code);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        logger_log(TAG, "HTTP GET Status = %d, content_length = %d",
                   esp_http_client_get_status_code(client),
                   esp_http_client_get_content_length(client));
    }
    else
    {
        logger_log(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}

void asynsWeatherTask(void *arg)
{
    while (1)
    {
        xTaskCreate(&http_weather_task, "http_weather_task", 4096, NULL, 5, NULL);
        // 1小时同步下天气
        vTaskDelay(1000 * 60 * 60 / portTICK_PERIOD_MS);
        
    }
}

void get_http_weather()
{
    xTaskCreate(asynsWeatherTask, "asynsWeatherTask", 1024 * 4, NULL, 1, NULL);
}