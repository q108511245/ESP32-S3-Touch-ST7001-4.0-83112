#pragma once

#include "project_config.h"
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "esp_system.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"

typedef enum
{
    EVENT_EVENT_CLICKED = 0,     // 页面触发事件
    EVENT_WIFI_CONNECTSTART,     // wifi开始连接标志
    EVENT_WIFI_SCANSTART,        // WIFI扫描开始标志
    EVENT_WIFI_SELECT_RESULT,    // WIFI选择结果
    EVENT_WIFI_CONNECTFINISH,    // wifi结束连接标志
    EVENT_WIFI_CONNECT_FAIL,     // wifi连接失败标志
    EVENT_ANIM_LOGOFINISH,       // LOGO动画结束标志
    EVENT_MQTT_CONNECTED,        // mqtt连接成功标志
    EVENT_MQTT_DISCONNECTED,     // mqtt连接断开标志
    EVENT_MQTT_STOP,             // mqtt停止标志
    EVENT_MQTT_START,            // mqtt开始标志
    EVENT_OTA_REFRESH_START,     // ota按钮开始标志
    EVENT_OTA_REFRESH_CANCEL,    // ota按钮取消标志
    EVENT_CLICKED_CLOCK_VIEW,    // 点击时钟按钮标志
    EVENT_CLICKED_RESTWIFI,      // 点击重置wifi按钮标志
    EVENT_CLOCK_ADD_SUBMIT,      // 点击时钟按钮标志
    EVENT_CLOCK_LOADED_OVER,     // 时钟加载完成标志
    EVENT_CLOCK_DELETE_SURE,     // 时钟删除成功标志
    EVENT_CLOCK_STATUS_CHANGE,   // 时钟状态改变标志
    EVENT_SNTP_REFRESHTIME_ONCE, // sntp刷新时间一次标志

    UI_OPEN_SELECTWIFI_VIEW,   // 进入WiFi选择页面
    UI_OPEN_WIFIPASSWORD_VIEW, // 进入WiFi密码页面
    UI_OPEN_404_VIEW,          // 进入404页面
    UI_WIFI_SCAN_RESULT,       // WIFI扫描结果
    UI_WIFI_SELECT_RESULT,     // WIFI选择结果
    UI_SNTP_CONNECTING_OVER,   // sntp连接结束标志
    UI_SNTP_REFRESHTIME_ONCE,  // sntp刷新时间一次标志
    UI_OTA_REFRESH_START,      // ota刷新开始标志
    UI_OTA_REFRESH_CANCEL,     // ota刷新结束标志
    UI_OTA_UPDATING_PROGRESS,  // ota更新进度标志
    UI_OPEN_CLOCK_VIEW,        // 进入时钟页面
    UI_WIFI_CONNECTFINISH,     // wifi结束连接标志
    UI_MQTT_CONNECTED,         // mqtt连接成功标志
    UI_MQTT_DISCONNECTED,      // mqtt断开连接标志
    UI_CLOCK_ADD_SUBMIT,       // 点击时钟按钮标志
    UI_CLOCK_LIST_NODATA,      // 时钟列表无数据标志
    UI_CLOCK_LIST_SHOWNODATA,  // 时钟列表显示无数据标志
    UI_CLOCK_DELETE_SURE,      // 时钟删除成功标志
    UI_CLOCK_DELETE_ALL,       // 时钟删除全部标志
    UI_OPEN_ALARM_PAGE,        // 进入报警页面
    UI_WEATHER_REFRESH_ONCE,   // 天气刷新一次标志

} event_ui_event_type_t;

typedef struct
{
    uint8_t event_type;
    uint8_t isOpenNextPage; // 是否需要打开页面
} view_ui_event_type_t;

/**
 * @brief
 * 初始化UI task
 */
esp_err_t service_ui_init(void);

/**
 * 事件处理函数
 */
void handle_ui_event(view_ui_event_type_t *event);

/**
 * @brief
 * 发送事件
 * @param type
 * @return
 */
esp_err_t send_xQueue(event_ui_event_type_t type);
