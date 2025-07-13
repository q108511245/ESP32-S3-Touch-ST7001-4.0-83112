#pragma once

#include "project_config.h"

#define BUZZER_GPIO GPIO_NUM_42

/**
 * 初始化蜂鸣器
 */
esp_err_t buzzer_init(void);

void buzzer_play(void);