/**
 * @file dht11.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "dht11.h"
#include "esp_log.h"

static const char *log_tag = "DHT11_Task";

float temperature, humidity;

static void start_dht11_task(void *pvParameters);

static void dht11_gpio_init(void)
{
    gpio_config_t io_config;

    io_config.pin_bit_mask = (1ULL << DHT11_IO_PIN);
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_config);
    gpio_set_level(DHT11_IO_PIN, 1);
}

void os_delay_ms(int nms)
{
    while (nms--) {
        os_delay_us(1000);
    }
}

extern void oled_start(void);
int dht11_init(void)
{
    dht11_gpio_init();
    int ret = xTaskCreate((TaskFunction_t)start_dht11_task, (const char *)"dht11_task", (uint16_t)1024, (void *)NULL,
                (UBaseType_t)4, (TaskHandle_t *)NULL);
    return ret;
}

static uint8_t dht11_read_bit(void)
{
    uint8_t retry = 0;
    while (gpio_get_level(DHT11_IO_PIN) && retry < 100) //等待变为低电平
    {
        retry++;
        os_delay_us(1);
    }
    retry = 0;
    while (!gpio_get_level(DHT11_IO_PIN) && retry < 100) //等待变高电平
    {
        retry++;
        os_delay_us(1);
    }
    os_delay_us(40); //等待 40us
    if (gpio_get_level(DHT11_IO_PIN)) {
        return 1;
    } else {
        return 0;
    }
}

int8_t dht11_read(dht11_data_t *dht11_data)
{
    uint8_t *data = (uint8_t *)dht11_data;
    // 设置连接DHT11的引脚输出为高，持续20毫秒
    gpio_set_direction(DHT11_IO_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT11_IO_PIN, 1);
    os_delay_ms(20);

    // 设置连接DHT11的引脚输出为低，持续20毫秒
    gpio_set_level(DHT11_IO_PIN, 0);
    os_delay_ms(20);

    // 设置连接DHT11的引脚设置为输入，并延时40微秒,准备读写
    gpio_set_level(DHT11_IO_PIN, 1);
    os_delay_us(30);
    gpio_set_direction(DHT11_IO_PIN, GPIO_MODE_INPUT);

    // 限时等待连接DHT11的引脚输入状态变成0，即等待获取DHT响应，如长时间未变为0则转为读取失败状态
    uint8_t i = 0;
    while (gpio_get_level(DHT11_IO_PIN) == 1) {
        if (i >= 100) {
            return -1;
        }
        i++;
        os_delay_us(1);
    }

    if (i < 100) {
        i = 0;
        while (gpio_get_level(DHT11_IO_PIN) == 0) {
            if (i >= 100) {
                return -1;
            }
            i++;
            os_delay_us(1);
        }
    }

    if (i < 100) {
        for (i = 0; i < 5; i++) {
            // 每次循环计数归零
            for (uint8_t j = 0; j < 8; j++) {
                data[i] = data[i] << 1;
                data[i] |= dht11_read_bit();
            }
        }
    }
    if (dht11_data->check_sum !=
        dht11_data->humi_int + dht11_data->humi_deci + dht11_data->temp_int + dht11_data->temp_deci) {
        return -1;
    }

    // ESP_LOGI(log_tag, "humidity：%d.%d,temperature:%d.%d", data[0], data[1], data[2], data[3]);
    return 0;
}

static void start_dht11_task(void *pvParameters)
{
    dht11_data_t dht11_data;
    oled_start();

    while (1) {
        unsigned long tick = xTaskGetTickCount();
        if (dht11_read(&dht11_data) == 0) {
            humidity = (float)(dht11_data.humi_int * 100 + dht11_data.humi_deci) / 100;
            temperature = (float)(dht11_data.temp_int * 100 + dht11_data.temp_deci) / 100;
            ESP_LOGI(log_tag, "humidity：%d.%d,temperature:%d.%d", dht11_data.humi_int, dht11_data.humi_deci,
                     dht11_data.temp_int, dht11_data.temp_deci);
        } else {
            ESP_LOGI(log_tag, "read dht11 error");
        }

        vTaskDelayUntil((TickType_t *)&tick, 2000 / portTICK_PERIOD_MS);
    }
}
