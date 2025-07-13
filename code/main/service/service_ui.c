#include "service_ui.h"
#include "service_view.h"
#include "project_config.h"
#include "driver_nvs.h"
#include "lvgl.h"
#include "service_clock.h"
#include "utils_comm.h"
#include "service_weather.h"

/**
 * 天气数据
 */
extern weather_data_t *weather_list_data;

// UI队列
QueueHandle_t ui_event_queue;

// 事件队列
QueueHandle_t event_event_queue;

// 选择的WiFi名称
extern char mSsid[50];
// 选择的WiFi密码
extern char mPassword[50];

// 当前选中的数据的索引
extern uint32_t current_item_index;
/**
 * 闹钟数据
 */
clock_data_t *clock_list_data;
size_t count;

/**
 * @brief 发送事件
 * @param type
 */
esp_err_t send_xQueue(event_ui_event_type_t type)
{
    view_ui_event_type_t event = {
        .event_type = type,
    };
    if (xQueueSend(event_event_queue, &event, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        logger_log(TAG, "queue send timeout");
    }
    return ESP_OK;
}

esp_err_t send_ui_xQueue(event_ui_event_type_t type)
{
    view_ui_event_type_t event = {
        .event_type = type,
    };
    if (xQueueSend(ui_event_queue, &event, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        logger_log(TAG, "queue send timeout");
    }
    return ESP_OK;
}

// wifis扫描结果
extern char wifiScanResult[512];

/**
 * @brief 时间
 */
extern ClockTimeStruct clockTime;

/**
 * @brief OTA 升级进度信息
 */
extern ProgressInfo info;

void handle_ui_event(view_ui_event_type_t *event)
{
    switch (event->event_type)
    {
    case UI_OPEN_SELECTWIFI_VIEW:
        // logger_log(TAG, "进入WiFi选择页面");
        send_xQueue(EVENT_WIFI_SCANSTART);
        goWiFiSelectView(0);
        break;
    case UI_WIFI_SELECT_RESULT:
        // 进入loading页面，连接WiFi
        showSelectPasswordViewLoading();
        break;
    case UI_WIFI_SCAN_RESULT:
        logger_log(TAG, "result:%s", wifiScanResult);
        hindSelectViewLoading();
        listWiFi(wifiScanResult);
        break;
    case UI_SNTP_CONNECTING_OVER:
        // 进入主页
        hindSelectPasswordViewLoading();
        goMainView();
        break;
    case UI_SNTP_REFRESHTIME_ONCE:
        // logger_log(TAG, "刷新时间");
        setMainClock(clockTime.hours, clockTime.minutes, clockTime.seconds, clockTime.day, clockTime.month, clockTime.year, clockTime.weekday);
        setClockView(clockTime.hours, clockTime.minutes, clockTime.seconds, clockTime.day, clockTime.month, clockTime.year, clockTime.weekday);
        break;
    case UI_OTA_REFRESH_START:
        // 更新开始
        startUpgrade();
        break;
    case UI_OTA_REFRESH_CANCEL:
        // 取消更新
        cancelUpgrade();
        break;
    case UI_OTA_UPDATING_PROGRESS:
        // 下载进度
        setUpgradeText(info.status, info.percent, info.progress);
        break;
    case UI_OPEN_CLOCK_VIEW:
        // 进入时钟页面
        goClockView();
        break;
    case UI_OPEN_404_VIEW:
        // 进入404页面
        hindSelectPasswordViewLoading();
        setWiFiStatusText("未连接");
        setMQTTStatusText("未连接");
        go404Page();
        break;
    case UI_WIFI_CONNECTFINISH:
        setWiFiStatusText("已连接");
        setWiFiText(mSsid, mPassword);
        break;
    case UI_MQTT_CONNECTED:
        setMQTTStatusText("已连接");
        setMQTTText(MQTT_URI, SUBSCRIBE_TOPIC);
        break;
    case UI_MQTT_DISCONNECTED:
        setMQTTStatusText("未连接");
        break;

    case UI_CLOCK_ADD_SUBMIT:
        hideClockSureDialog();
        break;
    case UI_CLOCK_LIST_NODATA:
        hideClockNodataView();
        break;
    case UI_CLOCK_LIST_SHOWNODATA:
        showClockNodataView();
        break;
    case UI_CLOCK_DELETE_SURE:
        hideDeleteDialog();
        break;
    case UI_WEATHER_REFRESH_ONCE:
        // 刷新天气数据
        showWeatherData(weather_list_data);
        break;
    case UI_CLOCK_DELETE_ALL:
        clear_clock_list();
        // send_xQueue(EVENT_CLOCK_LOADED_OVER);
        // 获取到闹钟列表
        memset(&clock_list_data, 0, sizeof(clock_list_data));
        memset(&count, 0, sizeof(count));
        if (load_struct_list(&clock_list_data, &count) == ESP_OK)
        {
            logger_log(TAG, "count: %d", (int)count);
            for (size_t i = 0; i < count; i++)
            {
                addClockItem(format_time(clock_list_data[i].hour, clock_list_data[i].min), clock_list_data[i].status, i);
            }
        };
        count == 0 ? send_ui_xQueue(UI_CLOCK_LIST_SHOWNODATA) : send_ui_xQueue(UI_CLOCK_LIST_NODATA);
        break;
    case UI_OPEN_ALARM_PAGE:
        goAlarmView();
        break;
    case UI_OPEN_WIFIPASSWORD_VIEW:
        // 进入WiFi密码页面
        setWiFiTextPasswordPage(mSsid);
        goWiFiPasswordView();
        break;
    default:
        break;
    }
}

void EventRefreshTask(void *pvParameters)
{
    view_ui_event_type_t event;
    static uint32_t inactivity_counter = 0; // 空闲计数器（单位：ms）
    bool is_timer_page_shown = false;       // 是否已跳转到计时页面
    while (1)
    {
        if (xQueueReceive(event_event_queue, &event, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            switch (event.event_type)
            {
            case EVENT_EVENT_CLICKED:
                // logger_log(TAG, "事件触发");
                inactivity_counter = 0; // 用户点击了屏幕或其他 UI 控件
                break;
            case EVENT_CLICKED_RESTWIFI:
                // logger_log(TAG, "重置WiFi");
                // 删除数据
                nvs_delete_config();
                send_ui_xQueue(UI_OPEN_SELECTWIFI_VIEW);
                break;
            case EVENT_CLICKED_CLOCK_VIEW:
                logger_log(TAG, "时钟页面点击");
                is_timer_page_shown = false;
                break;
            case EVENT_ANIM_LOGOFINISH:
                logger_log(TAG, "LOGO anim finish");
                // 获取配置信息进行判断
                configStruct config = nvs_get_config();
                config.reset == 1 ? send_xQueue(EVENT_WIFI_CONNECTSTART) : send_ui_xQueue(UI_OPEN_SELECTWIFI_VIEW);
                break;
            case EVENT_WIFI_CONNECTSTART:
                // logger_log(TAG, "WiFi开始连接");
                // 进行WiFi连接
                wifi_init_sta(0);
                break;
            case EVENT_WIFI_SCANSTART:
                // logger_log(TAG, "获取所有的WiFi");
                wifi_scan();
                break;
            case EVENT_WIFI_CONNECT_FAIL:
                logger_log(TAG, "WIFI连接失败");
                send_ui_xQueue(UI_OPEN_404_VIEW);
                break;
            case EVENT_WIFI_SELECT_RESULT:
                // 进入loading页面，连接WiFi
                send_ui_xQueue(UI_WIFI_SELECT_RESULT);
                wifi_init_sta(1);
                break;
            case EVENT_WIFI_CONNECTFINISH:
                send_ui_xQueue(UI_WIFI_CONNECTFINISH);
                // 初始化sntp
                sntp_sync_init();
                // 初始化mqtt
                send_xQueue(EVENT_MQTT_START);
                // 初始化闹钟数据
                send_xQueue(EVENT_CLOCK_LOADED_OVER);
                // 获取天气数据
                get_http_weather();
                break;
            case EVENT_MQTT_CONNECTED:
                // mqtt连接成功
                send_ui_xQueue(UI_MQTT_CONNECTED);
                logger_log(TAG, "MQTT Connected successfully");
                break;
            case EVENT_OTA_REFRESH_START:
                // 开始更新
                logger_log(TAG, "ota start");
                start_ota_upgrade();
                send_ui_xQueue(UI_OTA_REFRESH_START);
                break;
            case EVENT_OTA_REFRESH_CANCEL:
                // 取消更新
                logger_log(TAG, "ota cancel");
                cancel_ota_upgrade();
                send_ui_xQueue(UI_OTA_REFRESH_CANCEL);
                break;
            case EVENT_MQTT_STOP:
                remove_mqtt();
                send_ui_xQueue(UI_MQTT_DISCONNECTED);
                break;
            case EVENT_MQTT_START:
                // 初始化mqtt
                init_mqtt();
                break;
            case EVENT_CLOCK_DELETE_SURE:
                // 删除闹钟
                logger_log(TAG, "EVENT_CLOCK_DELETE_SURE: %d", (int)current_item_index);
                esp_err_t result = delete_clock_data_at_index(current_item_index);
                send_ui_xQueue(UI_CLOCK_DELETE_SURE);
                if (result == ESP_OK)
                {
                    logger_log(TAG, "删除成功");
                    send_xQueue(EVENT_CLOCK_LOADED_OVER);
                }
                else
                {
                    logger_log(TAG, "删除失败: %s", esp_err_to_name(result));
                }
                break;
            case EVENT_SNTP_REFRESHTIME_ONCE:
                send_ui_xQueue(UI_SNTP_REFRESHTIME_ONCE);
                // 判断是否要开启闹钟
                if (clock_list_data != NULL)
                {
                    for (size_t i = 0; i < count; i++)
                    {
                        if (clock_list_data[i].status == 1 && clock_list_data[i].hour == clockTime.hours && clock_list_data[i].min == clockTime.minutes && clockTime.seconds == 0)
                        {
                            logger_log(TAG, "开启闹钟");
                            send_ui_xQueue(UI_OPEN_ALARM_PAGE);
                        }
                    }
                }
                break;
            case EVENT_MQTT_DISCONNECTED:
                // mqtt断开连接
                send_ui_xQueue(UI_MQTT_DISCONNECTED);
                break;
            case EVENT_CLOCK_STATUS_CHANGE:
                // 闹钟状态改变
                logger_log(TAG, "EVENT_CLOCK_STATUS_CHANGE: %d", (int)current_item_index);
                if (clock_list_data != NULL && current_item_index < count)
                {
                    clock_list_data[current_item_index].status = clock_list_data[current_item_index].status == 0 ? 1 : 0;
                    save_struct_list(clock_list_data, count);
                }
                send_ui_xQueue(UI_CLOCK_ADD_SUBMIT);
                send_xQueue(EVENT_CLOCK_LOADED_OVER);
                break;
            case EVENT_CLOCK_ADD_SUBMIT:
                logger_log(TAG, "闹钟新增");
                // 获取数据
                char hour[32];
                lv_roller_get_selected_str(ui_comp_get_child(ui_ContainerSelectTime,
                                                             UI_COMP_CONTAINERSELECTTIME_CONTAINERALL_CONTAINER4_ROLLERHOUR),
                                           hour, sizeof(hour));
                char min[32];
                lv_roller_get_selected_str(ui_comp_get_child(ui_ContainerSelectTime,
                                                             UI_COMP_CONTAINERSELECTTIME_CONTAINERALL_CONTAINER4_ROLLERMIN),
                                           min, sizeof(min));
                int hourInt = atoi(hour);
                int minInt = atoi(min);
                clock_data_t item = {
                    .hour = hourInt,
                    .min = minInt,
                    .status = 0};
                save_struct_list(add_clock_item(clock_list_data, &count, item), count);
                send_ui_xQueue(UI_CLOCK_ADD_SUBMIT);
                send_xQueue(EVENT_CLOCK_LOADED_OVER);
                break;
            case EVENT_CLOCK_LOADED_OVER:
                send_xQueue(UI_CLOCK_DELETE_ALL);
                break;
            default:
                send_ui_xQueue(event.event_type);
                break;
            }
        }
        // 获取当前活动的输入设备
        lv_indev_t *indev = lv_indev_get_act();
        if (indev != NULL && !is_timer_page_shown && isMainView())
        {
            if (indev->proc.state == LV_INDEV_STATE_PRESSED)
            {
                // 用户正在操作屏幕，重置计数器
                inactivity_counter = 0;
                logger_log(TAG, "用户正在操作屏幕，重置计数器");
            }
            else
            {
                // 用户未操作，累加时间
                inactivity_counter++;                 // 每次延迟 50ms
                if (inactivity_counter * 50 >= 60000) // 超过60秒无操作
                {
                    logger_log(TAG, "60秒无操作，进入计时页面");
                    inactivity_counter = 0;
                    send_ui_xQueue(UI_OPEN_CLOCK_VIEW);
                    is_timer_page_shown = true; // 防止重复触发
                }
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief UI初始化
 */
esp_err_t service_ui_init(void)
{
    ui_event_queue = xQueueCreate(20, sizeof(event_ui_event_type_t));
    event_event_queue = xQueueCreate(20, sizeof(event_ui_event_type_t));
    if (event_event_queue == NULL)
    {
        ESP_LOGE(TAG, "event_event_queue创建失败");
    }
    BaseType_t baseType = xTaskCreate(EventRefreshTask, "EventRefreshTask", 1024 * 4, NULL, 1, NULL);
    if (baseType != pdPASS)
    {
        ESP_LOGE(TAG, "UI任务创建失败");
        return ESP_FAIL;
    }
    else
    {
        return ESP_OK;
    }
}