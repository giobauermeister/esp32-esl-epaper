#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "epd_display/epd_display.h"
#include "freertos/FreeRTOS.h"

static const char *TAG_MAIN = "MAIN";

void app_main(void) {
    ESP_LOGI(TAG_MAIN, "Initializing UC8253 EPD...");

    epd_gpio_init();
    epd_spi_init();

    epd_enable_power();
    vTaskDelay(pdMS_TO_TICKS(100));

    epd_init();
    epd_clear();
    epd_update();

    epd_test();
    epd_update();

    ESP_LOGI(TAG_MAIN, "UC8253 EPD Initialized and Cleared.");
}