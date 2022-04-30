/**
 * @file main.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"
#include "led.h"
#include "key.h"

void app_main() {
  printf("Hello world!\n");
  led_init();
  key_init();

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is ESP8266 chip with %d CPU cores, WiFi, ", chip_info.cores);

  printf("silicon revision %d, ", chip_info.revision);

  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded"
                                                       : "external");

//   for (int i = 10; i >= 0; i--) {
//     printf("Restarting in %d seconds...\n", i);
//   }
  for (;;) {
    led_toggle();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}

// 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
