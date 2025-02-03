#ifndef _EPD_GRAPHICS_H
#define _EPD_GRAPHICS_H

#include <stdint.h>

#define WHITE 0xFF
#define BLACK 0x00

typedef struct {
    uint8_t *buffer;        // Pointer to image buffer
    uint16_t width;         // Display width in pixels
    uint16_t height;        // Display height in pixels
    uint16_t width_memory;  // Memory width (bytes)
    uint16_t height_memory; // Memory height (bytes)
    uint16_t color;         // Current drawing color (1 = black, 0 = white)
    uint16_t rotation;      // Rotation mode (0, 90, 180, 270)
    uint16_t width_bytes;   // Width in bytes (width / 8)
    uint16_t height_bytes;  // Height in bytes
} epd_buffer_t;

extern epd_buffer_t epd_buffer;

void epd_set_buffer(uint8_t *buffer, uint16_t width, uint16_t height, uint16_t rotate, uint16_t color);
void epd_clear_buffer(uint8_t color);
void epd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void epd_show_char(uint16_t x, uint16_t y, uint16_t chr, uint16_t size, uint16_t color);
void epd_show_string(uint16_t x, uint16_t y, const char *str, uint16_t size, uint16_t color);

#endif // _EPD_GRAPHICS_H