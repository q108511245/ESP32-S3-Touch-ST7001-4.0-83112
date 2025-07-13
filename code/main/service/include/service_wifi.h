#ifndef _WIFI_H
#define _WIFI_H

#include "driver_nvs.h"
#include "project_config.h"


/**
 * @brief 初始化WiFi
 */
void wifi_init();

/**
 * @brief 扫描WiFi
 */
void wifi_scan();

/**
 * @brief 启动WiFi
 */
esp_err_t wifi_init_sta(uint8_t mIsReConnect);

/**
 * @brief 保存WiFi配置
 */
esp_err_t saveWiFiConfigSsid(char *ssid);

/**
 * @brief 保存WiFi密码
 */
esp_err_t saveWiFiConfigPassWord(char *password);

#endif