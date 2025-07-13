#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


// 初始化日志系统
void logger_init(int enable_log);

// 销毁日志系统（关闭线程）
void logger_destroy(void);

// 设置日志开启/关闭状态
void logger_set_enable(int enable_log);

// 异步日志打印接口
void logger_log(const char *tag, const char *format, ...);