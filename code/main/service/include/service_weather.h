#pragma once


typedef struct
{
    char *day_wind_power; 
    char *night_wind_power;
    char *night_weather_code;
    char *day_weather;
    char *day;
    char *night_weather;
    char *ziwaixian;
    char *jiangshui;
    char *day_wind_direction;
    char *night_wind_direction;
    char *night_air_temperature;
    char *air_press;
    char *day_air_temperature;
    int weekday;
} weather_data_t;

/**
 * @brief 获取天气信息
 *
 */
void get_http_weather();