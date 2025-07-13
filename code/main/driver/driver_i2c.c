#include "driver_i2c.h"


void i2c0_init(void)
{
    logger_log(TAG, "Initialize I2C bus");
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = PIN_NUM_HOST0_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = PIN_NUM_HOST0_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400 * 1000,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_HOST_0, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_HOST_0, i2c_conf.mode, 0, 0, 0));
}
