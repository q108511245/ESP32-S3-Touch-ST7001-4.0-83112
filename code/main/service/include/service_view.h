#pragma once

#include "ui/ui.h"
#include "service_weather.h"
#include "project_config.h"


/**
 * @brief 跳转到WiFi选择界面
 * @param hasBack 是否有返回按钮
 */
void goWiFiSelectView(uint8_t hasBack);

/**
 * @brief 显示WiFi选择界面的加载动画
 */
void showSelectViewLoading();

/**
 * @brief 跳转到404界面
 */
void go404Page();
/**
 * @brief 跳转到时钟界面
 */
void goClockView();

// 函数：listWiFi
// 功能：列出WiFi选项
// 参数：const char * options：WiFi选项
void listWiFi(const char *options);

/**
 * 进入主页
 */
void goMainView();

/**
 * @brief 设置主界面时钟
 */
void setMainClock(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year, uint8_t weekday);

/**
 * 设置时钟显示
 */
void setClockView(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year, uint8_t weekday);

/**
 * 启动升级
 */
void startUpgrade(void);

/**
 * 取消升级
 */
void cancelUpgrade(void);

/**
 * 下载进度
 */
void setUpgradeText(const char *upgradeText, const char *progressText, int16_t value);

/**
 * 隐藏选择页面的加载动画
 */
void hindSelectViewLoading();

/**
 * 显示WiFi连接的加载动画
 */
void showSelectPasswordViewLoading();

/**
 * 隐藏WiFi连接的加载动画
 */
void hindSelectPasswordViewLoading();

/**
 * 当前是否是主页面
 */
bool isMainView();

/**
 * 设置WiFi名称和密码
 */
void setWiFiText(char *ssid, char *password);

/**
 * 设置wifi状态
 */
void setWiFiStatusText(char *status);

/**
 * 设置MQTT名称和密码
 */
void setMQTTText(char *uri, char *topic);

/**
 * 设置MQTT状态
 */
void setMQTTStatusText(char *status);

/**
 * 进入wifi密码
 */
void goWiFiPasswordView();

/**
 * 设置WiFi名称
 */
void setWiFiTextPasswordPage(char *ssid);

/**
 * 添加时钟项
 */
void addClockItem(char *textTime, uint8_t status, uint8_t position);

/**
 * 隐藏时钟无数据视图
 */
void hideClockNodataView(void);

/**
 * 显示时钟无数据视图
 */
void showClockNodataView(void);

/**
 * 隐藏时钟删除对话框
 */
void hideDeleteDialog(void);

/**
 * 清空时钟列表
 */
void clear_clock_list(void);

/**
 * 隐藏新增闹钟对话框
 */
void hideClockSureDialog(void);

/**
 * 进入报警页面
 */
void goAlarmView(void);

/**
 * 显示天气数据
 */
void showWeatherData(weather_data_t *weatherData);