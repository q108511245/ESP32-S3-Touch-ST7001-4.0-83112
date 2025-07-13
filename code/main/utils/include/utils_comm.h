#pragma once

#include "project_config.h"
#include "service_clock.h"
#include "service_weather.h"

#define COPY_JSON_STR(obj, key) do { \
    cJSON *_item = cJSON_GetObjectItemCaseSensitive(obj, #key); \
    ((weather_data_t *)current)->key = _item ? strdup(_item->valuestring) : NULL; \
} while (0)

char *concat_strings(const char *s1, const char *s2);

clock_data_t *add_clock_item(clock_data_t *list, size_t *count, clock_data_t new_item);

char *format_time(int hour, int min);

weather_data_t *add_weather_item(weather_data_t *list, size_t *count, cJSON *f6);