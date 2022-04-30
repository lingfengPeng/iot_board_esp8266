/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "pin.h"

#define I2C_MASTER_SCL_IO SCL_PIN
#define I2C_MASTER_SDA_IO SDA_PIN
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 300

static const char *TAG = "ssd1306 test";

static ssd1306_handle_t dev = NULL;
static bool test_running;

time_t g_now;

/******** Test Function ****************/
esp_err_t ssd1306_show_time(ssd1306_handle_t dev)
{
    struct tm timeinfo;
    char strftime_buf[64];
    time(&g_now);
    g_now++;
    setenv("TZ", "GMT-8", 1);
    tzset();
    localtime_r(&g_now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    ssd1306_draw_3216char(dev, 0, 16, strftime_buf[11]);
    ssd1306_draw_3216char(dev, 16, 16, strftime_buf[12]);
    ssd1306_draw_3216char(dev, 32, 16, strftime_buf[13]);
    ssd1306_draw_3216char(dev, 48, 16, strftime_buf[14]);
    ssd1306_draw_3216char(dev, 64, 16, strftime_buf[15]);
    ssd1306_draw_1616char(dev, 80, 32, strftime_buf[16]);
    ssd1306_draw_1616char(dev, 96, 32, strftime_buf[17]);
    ssd1306_draw_1616char(dev, 112, 32, strftime_buf[18]);

    char *day = strftime_buf;
    day[3] = '\0';
    ssd1306_draw_string(dev, 87, 16, (const uint8_t *)day, 14, 1);
    ssd1306_draw_string(dev, 0, 52, (const uint8_t *)"MUSIC", 12, 0);
    ssd1306_draw_string(dev, 52, 52, (const uint8_t *)"MENU", 12, 0);
    ssd1306_draw_string(dev, 98, 52, (const uint8_t *)"PHONE", 12, 0);

    return ssd1306_refresh_gram(dev);
}

esp_err_t ssd1306_show_signs(ssd1306_handle_t dev)
{
    ssd1306_clear_screen(dev, 0x00);

    ssd1306_draw_bitmap(dev, 0, 2, &c_chSingal816[0], 16, 8);
    ssd1306_draw_bitmap(dev, 24, 2, &c_chBluetooth88[0], 8, 8);
    ssd1306_draw_bitmap(dev, 40, 2, &c_chMsg816[0], 16, 8);
    ssd1306_draw_bitmap(dev, 64, 2, &c_chGPRS88[0], 8, 8);
    ssd1306_draw_bitmap(dev, 90, 2, &c_chAlarm88[0], 8, 8);
    ssd1306_draw_bitmap(dev, 112, 2, &c_chBat816[0], 16, 8);

    return ssd1306_refresh_gram(dev);
}
/**
 * @brief i2c master initialization
 */
static void i2c_bus_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.clk_stretch_tick = I2C_MASTER_FREQ_HZ;

    i2c_driver_install(I2C_MASTER_NUM, conf.mode);
    i2c_param_config(I2C_MASTER_NUM, &conf);
}

static void dev_ssd1306_initialization(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "OLED initialization");
    i2c_bus_init();
    dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);

    ret = ssd1306_refresh_gram(dev);

    ret = ssd1306_show_signs(dev);
    ESP_ERROR_CHECK(ret);
}

static void ssd1306_test_task(void *pvParameters)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "OLED task start");
    dev_ssd1306_initialization();
    while (test_running) {
        ret = ssd1306_show_time(dev);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "OLED cleaning-up...");

    ssd1306_clear_screen(dev, 0);

    ret = ssd1306_refresh_gram(dev);

    // ssd1306_delete(dev);
    // ret = i2c_driver_delete(I2C_MASTER_NUM);
    // ESP_ERROR_CHECK(ret);

    // vTaskDelete(NULL);
}

void oled_start(void)
{
    test_running = true;
    xTaskCreate(&ssd1306_test_task, "ssd1306_test_task", 2048 * 2, NULL, 5, NULL);
}
