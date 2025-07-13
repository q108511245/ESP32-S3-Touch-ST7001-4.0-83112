#include "utils_log.h"
#include "service_sntp.h"

#define LOG_BUFFER_SIZE 512
#define MAX_LOG_QUEUE 20

extern ClockTimeStruct clockTime;

typedef struct
{
    char tag[32];
    char message[LOG_BUFFER_SIZE];
} log_entry_t;

static QueueHandle_t log_queue;
static TaskHandle_t log_task_handle = NULL;
static int is_log_enabled = 1;
static int stop_task = 0;

// 获取当前时间戳字符串
static void get_timestamp(char *buf, size_t len)
{
    snprintf(buf, len, "20%02d-%02d-%02d %02d:%02d:%02d",
             clockTime.year, clockTime.month, clockTime.day,
             clockTime.hours, clockTime.minutes, clockTime.seconds);
}

// 日志任务处理函数
static void log_task_func(void *arg)
{
    (void)arg;
    while (1)
    {
        log_entry_t entry;
        if (xQueueReceive(log_queue, &entry, pdMS_TO_TICKS(10)))
        {
            if (stop_task)
                break;

            char timestamp[64];
            get_timestamp(timestamp, sizeof(timestamp));
            printf("[%s] [%s] %s\n", timestamp, entry.tag, entry.message);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

// 初始化日志系统
void logger_init(int enable_log)
{
    if (log_queue == NULL)
    {
        log_queue = xQueueCreate(MAX_LOG_QUEUE, sizeof(log_entry_t));
        is_log_enabled = enable_log;
        stop_task = 0;
        xTaskCreate(log_task_func, "log_task", 1024 * 8, NULL, 0, &log_task_handle);
    }
}

// 销毁日志系统
void logger_destroy(void)
{
    if (log_queue != NULL)
    {
        stop_task = 1;
        // 发送空消息唤醒任务以便退出
        log_entry_t dummy = {0};
        xQueueSend(log_queue, &dummy, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS); // 等待任务结束
        vQueueDelete(log_queue);
        log_queue = NULL;
        log_task_handle = NULL;
    }
}

// 设置日志开启/关闭
void logger_set_enable(int enable_log)
{
    is_log_enabled = enable_log;
}

// 异步日志打印
void logger_log(const char *tag, const char *format, ...)
{
    if (!is_log_enabled || log_queue == NULL)
        return;

    va_list args;
    va_start(args, format);
    char buffer[LOG_BUFFER_SIZE];

    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log_entry_t entry;
    strncpy(entry.tag, tag, sizeof(entry.tag) - 1);
    strncpy(entry.message, buffer, sizeof(entry.message) - 1);

    // 尝试发送日志到队列
    if (xQueueSend(log_queue, &entry, pdMS_TO_TICKS(10)) != pdTRUE)
    {
        // 队列满时丢弃日志
    }
}