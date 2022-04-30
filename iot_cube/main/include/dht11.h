/**
 * @file dht11.h
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pin.h"

#include "rom/ets_sys.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DHT11_IO_PIN DAT_PIN
#define DHT11_PIN_READ gpio_get_level(DHT11_IO_PIN)

typedef struct dht11_data_s
{
    uint8_t  humi_int; //湿度的整数部分
    uint8_t  humi_deci; //湿度的小数部分
    uint8_t  temp_int; //温度的整数部分
    uint8_t  temp_deci; //温度的小数部分
    uint8_t  check_sum; //校验和
} dht11_data_t;

int dht11_init(void);
int8_t dht11_read(dht11_data_t *dht11_data);

extern float temperature, humidity;

#ifdef __cplusplus
}
#endif

#endif
