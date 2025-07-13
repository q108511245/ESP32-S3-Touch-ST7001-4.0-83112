
#include "driver_buzzer.h"

esp_err_t buzzer_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUZZER_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    // 初始状态：关闭蜂鸣器
    return gpio_set_level(BUZZER_GPIO, 0);
}

void buzzer_on(void)
{
    gpio_set_level(BUZZER_GPIO, 1); // 高电平触发蜂鸣器
}

void buzzer_off(void)
{
    gpio_set_level(BUZZER_GPIO, 0); // 低电平关闭蜂鸣器
}


void buzzer_play(void)
{
    buzzer_on();
    vTaskDelay(500 / portTICK_PERIOD_MS);
    buzzer_off();
    vTaskDelay(500 / portTICK_PERIOD_MS);
}