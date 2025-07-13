#pragma once

#include "project_config.h"

extern QueueHandle_t xProgressQueue;



/**
 * ota升级
 */
void start_ota_upgrade(void);

/**
 * 取消更新
 */
void cancel_ota_upgrade(void);

/** 升级信息 */
typedef struct {
    char status[64];
    char percent[16];
    int16_t progress;
} ProgressInfo;