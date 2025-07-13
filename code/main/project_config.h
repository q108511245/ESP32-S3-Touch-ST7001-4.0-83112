#pragma once

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_check.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "cJSON.h"
#include <math.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_ota_ops.h"
#include "driver_i2c.h"
#include "service_lcd.h"
#include "service_ui.h"
#include "driver_nvs.h"
#include "service_wifi.h"
#include "service_view.h"
#include "service_sntp.h"
#include "service_mqtt.h"
#include "driver_buzzer.h"
#include "utils_log.h"
#include "service_ota.h"


// 日志标签
static const char *TAG = "ZJGSYJC_ESP32_LCD_RGB_TOUCH";

/**
 * @brief
 * 升级URL
 */
static const char *OTAURL = "http://bin.bemfa.com/b/1BcMDBlNDUyMTQ5Yjk2ZGQ3MjZjZjYyZWZiNGYwNWExNTY=ESP32.bin";

static const char *weather_url = "http://ali-weather.showapi.com/gps-to-weather?lng=120.68&lat=31.32&needMoreDay=1&needIndex=0&needHourData=0&need3HourForcast=0&needAlarm=0";

static const char *app_code = "APPCODE 123";

// 是否开启日志
static const uint8_t isOpenLog = 0;

// 是否打开监视任务
static const uint8_t isOpenMinTask = 0;

/**
 * @brief
 * 是否擦除数据
 */
static const uint8_t ifEraserDate = 0;
