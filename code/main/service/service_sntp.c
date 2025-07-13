#include "service_sntp.h"

/**
 * @brief 时间
 */
ClockTimeStruct clockTime = {0};

/**
 * 初始化SNTP
 */
esp_err_t init_sntp(void)
{
    logger_log(TAG, "init_sntp");
    esp_sntp_stop();                // 停止 SNTP 客户端，避免报错,
    vTaskDelay(pdMS_TO_TICKS(100)); // 等待资源释放
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // 默认服务器
    esp_sntp_setservername(0, "pool.ntp.org");
    // 中国 NTP 服务器
    esp_sntp_setservername(1, "cn.pool.ntp.org");
    // 阿里云 NTP 服务器
    esp_sntp_setservername(2, "ntp1.aliyun.com");
    esp_sntp_init();
    return ESP_OK;
}

/**
 * 获取时间
 */
void obtain_time(uint8_t isGoMain)
{
    init_sntp();
    // 时间被重新设置后，重新设置时间
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        logger_log(TAG, "waiting time reset... (%d/%d)", retry, retry_count);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    if (isGoMain)
    {
        send_xQueue(UI_SNTP_CONNECTING_OVER);
    }
}

void asynsTimeTask(void *arg)
{
    while (1)
    {
        // 10分钟同步下时间
        vTaskDelay(1000 * 60 * 10 / portTICK_PERIOD_MS);
        // 显式停止（若已运行）
        // esp_sntp_stop();
        obtain_time(0);
    }
}

void setTimeTask(void *arg)
{
    while (1)
    {
        time_t now = 0;
        struct tm timeinfo = {0};
        time(&now);
        // 设置时区
        setenv("TZ", "CST-8", 1);
        tzset();
        localtime_r(&now, &timeinfo);

        clockTime.hours = timeinfo.tm_hour;
        clockTime.minutes = timeinfo.tm_min;
        clockTime.seconds = timeinfo.tm_sec;
        clockTime.day = timeinfo.tm_mday;
        clockTime.month = timeinfo.tm_mon + 1;
        clockTime.year = timeinfo.tm_year - 100;
        clockTime.weekday = timeinfo.tm_wday;
        send_xQueue(EVENT_SNTP_REFRESHTIME_ONCE);
        // setAngleClock(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year - 100, timeinfo.tm_wday);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// void obtainTimeTask(void *arg)
// {
//     // 同步系统时间
//     obtain_time(1);
//     // 删除任务
//     vTaskDelete(NULL);
// }
/**
 * 初始化时间
 */
void sntp_sync_init(void)
{
    // xTaskCreate(obtainTimeTask, "obtainTimeTask", 1024 * 4, NULL, 1, NULL);
    // 同步系统时间
    obtain_time(1);
    xTaskCreate(asynsTimeTask, "asynsTimeTask", 1024 * 4, NULL, 1, NULL);
    xTaskCreate(setTimeTask, "setTimeTask", 1024 * 4, NULL, 1, NULL);
}