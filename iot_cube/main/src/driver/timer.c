/**
 * @file timer.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdbool.h>
#include "driver/hw_timer.h"

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static const char *log_tag = "hw_timer";

#define TIMER_ONE_SHOT false
#define TIMER_RELOAD   true

static bool is_init;
static SemaphoreHandle_t timer_semaphore = NULL;

static void hw_timer_callback(void *arg)
{
    BaseType_t HigherPriority = pdFALSE;
    xSemaphoreGiveFromISR(timer_semaphore, &HigherPriority);
}

void timer_delay(uint32_t us)
{
    if (is_init == false) {
        timer_semaphore = xSemaphoreCreateBinary();
        if (timer_semaphore == NULL) {
            ESP_LOGE(log_tag, "timer semaphore create fail!");
        }

        hw_timer_init(hw_timer_callback, NULL);
        is_init = true;
    }
    hw_timer_alarm_us(us, TIMER_ONE_SHOT);
    BaseType_t HigherPriority = pdFALSE;
    xSemaphoreTakeFromISR(timer_semaphore, &HigherPriority);
}
