#pragma once

#include "project_config.h"



/**
 * @brief 时钟时间
 */
typedef struct
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t weekday;
} ClockTimeStruct;

/**
 * @file sntp.h
 */
void sntp_sync_init(void);

