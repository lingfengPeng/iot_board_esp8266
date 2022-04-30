/**
 * @file pin.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pin.h"
#include "led.h"

#define LED_PIN_MSK (1UL << LED_PIN)

int led_init(void)
{
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = LED_PIN_MSK;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    return gpio_config(&io_conf);
}

int led_on(void)
{
    return gpio_set_level(LED_PIN, 0);
}

int led_off(void)
{
    return gpio_set_level(LED_PIN, 1);
}

int led_toggle(void)
{
    int level = gpio_get_level(LED_PIN);
    return gpio_set_level(LED_PIN, !level);
}
