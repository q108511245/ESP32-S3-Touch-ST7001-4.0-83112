#pragma once

#include "project_config.h"


// MQTT 配置
#define MQTT_URI "mqtt://bemfa.com:9501"
#define MQTT_CLIENT_ID "00e452149b96dd726cf62efb4f05a156"

// 订阅的主题和消息内容
#define SUBSCRIBE_TOPIC  "ESP32"
// #define PUBLISH_TOPIC  "ESP32"

/**
 * 初始化 MQTT 客户端
 */
esp_err_t init_mqtt();

/**
 * 关闭mqtt
 */
esp_err_t remove_mqtt();