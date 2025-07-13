#pragma once

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_io_additions.h"
#include "esp_lcd_touch_ft5x06.h"
#include "esp_lcd_st7701.h"
#include "lv_demos.h"
#include "service_lvgl_port.h"
#include "ui.h"
#include "project_config.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_H_RES (480)
#define EXAMPLE_LCD_V_RES (480)
#define EXAMPLE_LCD_BIT_PER_PIXEL (18)
#define EXAMPLE_RGB_BIT_PER_PIXEL (16)
#define EXAMPLE_RGB_DATA_WIDTH (16)
#define EXAMPLE_RGB_BOUNCE_BUFFER_SIZE (EXAMPLE_LCD_H_RES * 10)
#define EXAMPLE_LCD_IO_SPI_CS (GPIO_NUM_45)
#define EXAMPLE_LCD_IO_SPI_SCL (GPIO_NUM_38)
#define EXAMPLE_LCD_IO_SPI_SDA (GPIO_NUM_39)

#define EXAMPLE_LCD_IO_RGB_DISP (-1) // -1 if not used
#define EXAMPLE_LCD_IO_RGB_VSYNC (GPIO_NUM_21)
#define EXAMPLE_LCD_IO_RGB_HSYNC (GPIO_NUM_14)
#define EXAMPLE_LCD_IO_RGB_DE (GPIO_NUM_47)
#define EXAMPLE_LCD_IO_RGB_PCLK (GPIO_NUM_48)

#define EXAMPLE_LCD_IO_RGB_DATA0 (GPIO_NUM_0)
#define EXAMPLE_LCD_IO_RGB_DATA1 (GPIO_NUM_12)
#define EXAMPLE_LCD_IO_RGB_DATA2 (GPIO_NUM_11)
#define EXAMPLE_LCD_IO_RGB_DATA3 (GPIO_NUM_10)
#define EXAMPLE_LCD_IO_RGB_DATA4 (GPIO_NUM_9)
#define EXAMPLE_LCD_IO_RGB_DATA5 (GPIO_NUM_46)
#define EXAMPLE_LCD_IO_RGB_DATA6 (GPIO_NUM_3)
#define EXAMPLE_LCD_IO_RGB_DATA7 (GPIO_NUM_20)
#define EXAMPLE_LCD_IO_RGB_DATA8 (GPIO_NUM_19)
#define EXAMPLE_LCD_IO_RGB_DATA9 (GPIO_NUM_8)
#define EXAMPLE_LCD_IO_RGB_DATA10 (GPIO_NUM_18)
#define EXAMPLE_LCD_IO_RGB_DATA11 (GPIO_NUM_17)
#define EXAMPLE_LCD_IO_RGB_DATA12 (GPIO_NUM_16)
#define EXAMPLE_LCD_IO_RGB_DATA13 (GPIO_NUM_15)
#define EXAMPLE_LCD_IO_RGB_DATA14 (GPIO_NUM_7)
#define EXAMPLE_LCD_IO_RGB_DATA15 (GPIO_NUM_6)

#define EXAMPLE_LCD_IO_RST (-1)                
#define EXAMPLE_PIN_NUM_BK_LIGHT (GPIO_NUM_13) 
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TOUCH_HOST (I2C_NUM_0)
#define EXAMPLE_PIN_NUM_TOUCH_SCL (GPIO_NUM_4)
#define EXAMPLE_PIN_NUM_TOUCH_SDA (GPIO_NUM_5)
#define EXAMPLE_PIN_NUM_TOUCH_RST (-1) 
#define EXAMPLE_PIN_NUM_TOUCH_INT (-1) 
// #endif

/**
 * @brief Initialize LCD
 */
void lcd_init(void);