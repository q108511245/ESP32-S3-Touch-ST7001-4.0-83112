#pragma once

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "project_config.h"

#define I2C_HOST_0        (I2C_NUM_0)
#define PIN_NUM_HOST0_SCL (GPIO_NUM_4)
#define PIN_NUM_HOST0_SDA (GPIO_NUM_5)

/**
 * 初始化i2c0
 */
void i2c0_init(void);