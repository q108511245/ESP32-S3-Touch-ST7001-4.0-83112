#include "service_lcd.h"

IRAM_ATTR static bool rgb_lcd_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
{
    return lvgl_port_notify_rgb_vsync();
}

static const st7701_lcd_init_cmd_t lcd_init_cmds[] = {
    //  {cmd, { data }, data_size, delay_ms}
    {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0},
    {0xEF, (uint8_t[]){0x08}, 1, 0},
    {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    {0xC0, (uint8_t[]){0x3B, 0x00}, 2, 0},
    {0xC1, (uint8_t[]){0x0B, 0x02}, 2, 0},
    {0xC2, (uint8_t[]){0x37, 0x02}, 2, 0},
    {0xCC, (uint8_t[]){0x10}, 1, 0},
    {0xB0, (uint8_t[]){0x00, 0x0F, 0x16, 0x0E, 0x11, 0x07, 0x09, 0x09, 0x08, 0x23, 0x05, 0x11, 0x0F, 0x28, 0x2D, 0x18}, 16, 0},
    {0xB1, (uint8_t[]){0x00, 0x0F, 0x16, 0x0E, 0x11, 0x07, 0x09, 0x08, 0x09, 0x23, 0x05, 0x11, 0x0F, 0x28, 0x2D, 0x18}, 16, 0},
    {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x11}, 5, 0},
    {0xB0, (uint8_t[]){0x4D}, 1, 0},
    {0xB1, (uint8_t[]){0x33}, 1, 0},
    {0xB2, (uint8_t[]){0x87}, 1, 0},
    {0xB5, (uint8_t[]){0x4B}, 1, 0},
    {0xB7, (uint8_t[]){0x8C}, 1, 0},
    {0xB8, (uint8_t[]){0x20}, 1, 0},
    {0xC1, (uint8_t[]){0x78}, 1, 0},
    {0xC2, (uint8_t[]){0x78}, 1, 0},
    {0xD0, (uint8_t[]){0x88}, 1, 0},
    {0xE0, (uint8_t[]){0x00, 0x00, 0x02}, 3, 0},
    {0xE1, (uint8_t[]){0x02, 0xF0, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x44, 0x44}, 11, 0},
    {0xE2, (uint8_t[]){0x10, 0x10, 0x40, 0x40, 0xF2, 0xF0, 0x00, 0x00, 0xF2, 0xF0, 0x00, 0x00}, 12, 0},
    {0xE3, (uint8_t[]){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE4, (uint8_t[]){0x44, 0x44}, 2, 0},
    {0xE5, (uint8_t[]){0x07, 0xEF, 0xF0, 0xF0, 0x09, 0xF1, 0xF0, 0xF0, 0x03, 0xF3, 0xF0, 0xF0, 0x05, 0xED, 0xF0, 0xF0}, 16, 0},
    {0xE6, (uint8_t[]){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE7, (uint8_t[]){0x44, 0x44}, 2, 0},
    {0xE8, (uint8_t[]){0x08, 0xF0, 0xF0, 0xF0, 0x0A, 0xF2, 0xF0, 0xF0, 0x04, 0xF4, 0xF0, 0xF0, 0x06, 0xEE, 0xF0, 0xF0}, 16, 0},
    {0xEB, (uint8_t[]){0x00, 0x00, 0xE4, 0xE4, 0x44, 0x88, 0x40}, 7, 0},
    {0xEC, (uint8_t[]){0x78, 0x00}, 2, 0},
    {0xED, (uint8_t[]){0x20, 0xF9, 0x87, 0x76, 0x65, 0x54, 0x4F, 0xFF, 0xFF, 0xF4, 0x45, 0x56, 0x67, 0x78, 0x9F, 0x02}, 16, 0},
    {0xEF, (uint8_t[]){0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F}, 6, 0},

    // 退出睡眠模式和开启显示的命令保持不变
    {0x11, (uint8_t[]){0x00}, 0, 120}, // Sleep Out
    {0x29, (uint8_t[]){0x00}, 0, 0},   // Display On
};

void lcd_init()
{
    if (EXAMPLE_PIN_NUM_BK_LIGHT >= 0)
    {
        ESP_LOGI(TAG, "Turn off LCD backlight");
        gpio_config_t bk_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT};
        ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    }

    ESP_LOGI(TAG, "Install 3-wire SPI panel IO");
    spi_line_config_t line_config = {
        .cs_io_type = IO_TYPE_GPIO,
        .cs_gpio_num = EXAMPLE_LCD_IO_SPI_CS,
        .scl_io_type = IO_TYPE_GPIO,
        .scl_gpio_num = EXAMPLE_LCD_IO_SPI_SCL,
        .sda_io_type = IO_TYPE_GPIO,
        .sda_gpio_num = EXAMPLE_LCD_IO_SPI_SDA,
        .io_expander = NULL,
    };
    esp_lcd_panel_io_3wire_spi_config_t io_config = ST7701_PANEL_IO_3WIRE_SPI_CONFIG(line_config, 0);
    esp_lcd_panel_io_handle_t io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_3wire_spi(&io_config, &io_handle));

    ESP_LOGI(TAG, "Install ST7701 panel driver");
    esp_lcd_panel_handle_t lcd_handle = NULL;
    esp_lcd_rgb_panel_config_t rgb_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .psram_trans_align = 64,
        .data_width = EXAMPLE_RGB_DATA_WIDTH,
        .bits_per_pixel = EXAMPLE_RGB_BIT_PER_PIXEL,
        .de_gpio_num = EXAMPLE_LCD_IO_RGB_DE,
        .pclk_gpio_num = EXAMPLE_LCD_IO_RGB_PCLK,
        .vsync_gpio_num = EXAMPLE_LCD_IO_RGB_VSYNC,
        .hsync_gpio_num = EXAMPLE_LCD_IO_RGB_HSYNC,
        .disp_gpio_num = EXAMPLE_LCD_IO_RGB_DISP,
        .data_gpio_nums = {
            EXAMPLE_LCD_IO_RGB_DATA0,
            EXAMPLE_LCD_IO_RGB_DATA1,
            EXAMPLE_LCD_IO_RGB_DATA2,
            EXAMPLE_LCD_IO_RGB_DATA3,
            EXAMPLE_LCD_IO_RGB_DATA4,
            EXAMPLE_LCD_IO_RGB_DATA5,
            EXAMPLE_LCD_IO_RGB_DATA6,
            EXAMPLE_LCD_IO_RGB_DATA7,
            EXAMPLE_LCD_IO_RGB_DATA8,
            EXAMPLE_LCD_IO_RGB_DATA9,
            EXAMPLE_LCD_IO_RGB_DATA10,
            EXAMPLE_LCD_IO_RGB_DATA11,
            EXAMPLE_LCD_IO_RGB_DATA12,
            EXAMPLE_LCD_IO_RGB_DATA13,
            EXAMPLE_LCD_IO_RGB_DATA14,
            EXAMPLE_LCD_IO_RGB_DATA15,
        },
        .timings = {
            .pclk_hz = 15 * 1000 * 1000,
            .h_res = 480,
            .v_res = 480,
            .hsync_pulse_width = 10,
            .hsync_back_porch = 50,
            .hsync_front_porch = 20,
            .vsync_pulse_width = 10,
            .vsync_back_porch = 20,
            .vsync_front_porch = 10,
            .flags.pclk_active_neg = false,
        },
        .flags.fb_in_psram = true,
        .num_fbs = LVGL_PORT_LCD_RGB_BUFFER_NUMS,
        .bounce_buffer_size_px = EXAMPLE_RGB_BOUNCE_BUFFER_SIZE,
    };
    rgb_config.timings.h_res = EXAMPLE_LCD_H_RES;
    rgb_config.timings.v_res = EXAMPLE_LCD_V_RES;
    st7701_vendor_config_t vendor_config = {
        .rgb_config = &rgb_config,
        .init_cmds = lcd_init_cmds, // Uncomment these line if use custom initialization commands
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
        .flags = {
            .auto_del_panel_io = 0, /**
                                     * Set to 1 if panel IO is no longer needed after LCD initialization.
                                     * If the panel IO pins are sharing other pins of the RGB interface to save GPIOs,
                                     * Please set it to 1 to release the pins.
                                     */
            .mirror_by_cmd = 1,     // Set to 0 if `auto_del_panel_io` is enabled
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_IO_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = EXAMPLE_LCD_BIT_PER_PIXEL,
        .vendor_config = &vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7701(io_handle, &panel_config, &lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_handle));

    esp_lcd_panel_disp_on_off(lcd_handle, true);

    esp_lcd_touch_handle_t tp_handle = NULL;

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();

    ESP_LOGI(TAG, "Initialize I2C panel IO");
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)TOUCH_HOST, &tp_io_config, &tp_io_handle));

    ESP_LOGI(TAG, "Initialize touch controller ft5x06");
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = EXAMPLE_PIN_NUM_TOUCH_RST,
        .int_gpio_num = EXAMPLE_PIN_NUM_TOUCH_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, &tp_handle));
    /*初始化lvgl */
    ESP_ERROR_CHECK(lvgl_port_init(lcd_handle, tp_handle));
    esp_lcd_rgb_panel_event_callbacks_t cbs = {
        .on_bounce_frame_finish = rgb_lcd_on_vsync_event,
    };
    esp_lcd_rgb_panel_register_event_callbacks(lcd_handle, &cbs, NULL);

    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1))
    {
        // lv_demo_music();
        // lv_demo_widgets();
        ui_init();
        // Release the mutex
        lvgl_port_unlock();
    }
    
    //延时500毫秒，解决上电白屏的问题
    vTaskDelay(500 / portTICK_PERIOD_MS);
    if (EXAMPLE_PIN_NUM_BK_LIGHT >= 0)
    {
        ESP_LOGI(TAG, "Turn on LCD backlight");
        gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
    }
}
