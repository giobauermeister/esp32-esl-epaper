#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "epd_display/epd_display.h"
#include "epd_display/epd_graphics.h"
#include "freertos/FreeRTOS.h"

static const char *TAG_MAIN = "MAIN";

uint8_t fb[EPD_WIDTH * EPD_HEIGHT / 8];

void app_main(void) {
    ESP_LOGI(TAG_MAIN, "Initializing UC8253 EPD...");

    epd_set_buffer(fb, EPD_WIDTH, EPD_HEIGHT, 180, 1);
    epd_clear_buffer(WHITE);

    epd_spi_init();

    epd_gpio_init();
    epd_enable_power();

    epd_fast_init();
    epd_clear();
    epd_update();
    epd_part_init();

    epd_show_string(0, 0, "Hello World!", 24, 1);
    epd_display(fb);
    epd_update();

    ESP_LOGI(TAG_MAIN, "UC8253 EPD Initialized and Cleared.");
}