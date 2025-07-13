#include "utils_comm.h"
#include "service_clock.h"

char *concat_strings(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }
    return result;
}

clock_data_t *add_clock_item(clock_data_t *list, size_t *count, clock_data_t new_item)
{
    list = realloc(list, (*count + 1) * sizeof(clock_data_t));
    if (list)
    {
        list[*count] = new_item;
        (*count)++;
    }
    return list;
}

char *format_time(int hour, int min)
{
    char *time_str = malloc(6); // 分配空间
    if (time_str == NULL)
        return NULL;

    snprintf(time_str, 6, "%02d:%02d", hour, min);
    return time_str;
}

/**
 * 添加数据
 */
weather_data_t *add_weather_item(weather_data_t *list, size_t *count, cJSON *item)
{
    if (!item) return list;

    // 扩展数组
    list = realloc(list, (*count + 1) * sizeof(weather_data_t));
    if (!list) {
        logger_log(TAG, "Failed to allocate memory for weather data");
        return list;
    }

    weather_data_t *current = &list[*count];

    // 初始化所有字段为 NULL
    memset(current, 0, sizeof(weather_data_t));

    // 依次赋值
    COPY_JSON_STR(item, day_wind_power);
    COPY_JSON_STR(item, night_wind_power);
    COPY_JSON_STR(item, night_weather_code);
    COPY_JSON_STR(item, day_weather);
    COPY_JSON_STR(item, day);
    COPY_JSON_STR(item, night_weather);
    COPY_JSON_STR(item, ziwaixian);
    COPY_JSON_STR(item, jiangshui);
    COPY_JSON_STR(item, day_wind_direction);
    COPY_JSON_STR(item, night_wind_direction);
    COPY_JSON_STR(item, night_air_temperature);
    COPY_JSON_STR(item, air_press);
    COPY_JSON_STR(item, day_air_temperature);

    // 特别处理 weekday 字段为整数
    cJSON *weekdayItem = cJSON_GetObjectItemCaseSensitive(item, "weekday");
    current->weekday = weekdayItem ? weekdayItem->valueint : -1;

    (*count)++;
    return list;
}
