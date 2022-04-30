/**
 * @file key.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pin.h"
#include "key.h"

#include "iot_button.h"
#include "esp_log.h"

#define KEY_PIN BOOT_PIN
#define KEY_PIN_MSK (1UL << KEY_PIN)

static const char *log_tag = "key";

button_config_t key_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .gpio_button_config = {
        .gpio_num = KEY_PIN,
        .active_level = 0
    }
};

static button_handle_t g_btns;

static int get_btn_index(button_handle_t btn)
{
    return 0;
}

static void button_press_down_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_PRESS_DOWN", get_btn_index((button_handle_t)arg));
}

static void button_press_up_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_PRESS_UP", get_btn_index((button_handle_t)arg));
}

static void button_press_repeat_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_PRESS_REPEAT[%d]", get_btn_index((button_handle_t)arg), iot_button_get_repeat((button_handle_t)arg));
}

static void button_single_click_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_SINGLE_CLICK", get_btn_index((button_handle_t)arg));
}

static void button_double_click_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_DOUBLE_CLICK", get_btn_index((button_handle_t)arg));
}

static void button_long_press_start_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_LONG_PRESS_START", get_btn_index((button_handle_t)arg));
}

static void button_long_press_hold_cb(void *arg)
{
    ESP_LOGI(log_tag, "BTN%d: BUTTON_LONG_PRESS_HOLD", get_btn_index((button_handle_t)arg));
}

int key_init(void)
{
    g_btns = iot_button_create(&key_cfg);
    iot_button_register_cb(g_btns, BUTTON_PRESS_DOWN, button_press_down_cb);
    iot_button_register_cb(g_btns, BUTTON_PRESS_UP, button_press_up_cb);
    // iot_button_register_cb(g_btns, BUTTON_PRESS_REPEAT, button_press_repeat_cb);
    // iot_button_register_cb(g_btns, BUTTON_SINGLE_CLICK, button_single_click_cb);
    // iot_button_register_cb(g_btns, BUTTON_DOUBLE_CLICK, button_double_click_cb);
    // iot_button_register_cb(g_btns, BUTTON_LONG_PRESS_START, button_long_press_start_cb);
    // iot_button_register_cb(g_btns, BUTTON_LONG_PRESS_HOLD, button_long_press_hold_cb);

    return 0;

}

