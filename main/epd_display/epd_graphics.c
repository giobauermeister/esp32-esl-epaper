#include <stdint.h>
#include <stdlib.h>
#include "epd_graphics.h"
#include "epd_font.h"

epd_buffer_t epd_buffer;

/*******************************************************************
 * Function Description: Initializes the e-paper image buffer.
 * 
 * Parameters:
 *   *buffer - Pointer to an externally allocated image buffer
 *   width   - Image width
 *   height  - Image height
 *   rotate  - Display rotation (0, 90, 180, 270)
 *   color   - Default drawing color (1 for black, 0 for white)
 * 
 * Returns: None
 *******************************************************************/
void epd_set_buffer(uint8_t *buffer, uint16_t width, uint16_t height, uint16_t rotate, uint16_t color) {
    epd_buffer.buffer = buffer;
    epd_buffer.color = color;
    
    epd_buffer.width_memory = width;
    epd_buffer.height_memory = height;
    epd_buffer.width_bytes = (width % 8 == 0) ? (width / 8) : (width / 8 + 1);
    epd_buffer.height_bytes = height;
    epd_buffer.rotation = rotate;

    if (rotate == 0 || rotate == 180) {  
        epd_buffer.width = height;  
        epd_buffer.height = width;  
    } else {  
        epd_buffer.width = width;  
        epd_buffer.height = height;  
    }
}


/*******************************************************************
 * Function Description: Clears the image buffer.
 * 
 * Parameters:
 *   color - The color to fill the buffer (1 = black, 0 = white)
 * 
 * Returns: None
 *******************************************************************/
void epd_clear_buffer(uint8_t color) {
    uint16_t x, y;
    uint32_t addr;

    for (y = 0; y < epd_buffer.height_bytes; y++) {
        for (x = 0; x < epd_buffer.width_bytes; x++) {
            addr = x + y * epd_buffer.width_bytes;  // Calculate buffer address
            epd_buffer.buffer[addr] = color;       // Fill buffer with color
        }
    }
}


/*******************************************************************
 * Function Description: Lights up a single pixel on the e-paper display.
 * 
 * Parameters:
 *   x      - X coordinate of the pixel
 *   y      - Y coordinate of the pixel
 *   color  - Pixel color (1 for black, 0 for white)
 * 
 * Returns: None
 *******************************************************************/
void epd_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    uint16_t X, Y;
    uint32_t addr;
    uint8_t pixel_data;

    // Handle rotation
    switch (epd_buffer.rotation) {
        case 0:
            X = y;
            Y = x;
            break;
        case 90:
            X = x;
            Y = epd_buffer.height - y - 1;
            break;
        case 180:
            X = epd_buffer.width - y - 1;
            Y = epd_buffer.height - x - 1;
            break;
        case 270:
            X = epd_buffer.width - x - 1;
            Y = y;
            break;
        default:
            return;
    }

    // Calculate pixel position in the buffer
    addr = (X / 8) + (Y * epd_buffer.width_bytes);
    pixel_data = epd_buffer.buffer[addr];

    if (color == 1) {  // Black pixel
        epd_buffer.buffer[addr] = pixel_data & ~(0x80 >> (X % 8)); 
    } else {  // White pixel
        epd_buffer.buffer[addr] = pixel_data | (0x80 >> (X % 8));  
    }
}

/*******************************************************************
 * Function Description: Displays a single character on the e-paper display.
 * 
 * Parameters:
 *   x      - X coordinate of the character
 *   y      - Y coordinate of the character
 *   chr    - The character to be displayed
 *   size   - Font size of the character
 *   color  - Pixel color parameter (1 for black, 0 for white)
 * 
 * Returns: None
 *******************************************************************/
void epd_show_char(uint16_t x, uint16_t y, uint16_t chr, uint16_t size, uint16_t color) {
    uint16_t i, m, temp, bytes_per_char, chr_offset;
    uint16_t x0, y0;
    
    x0 = x;
    y0 = y;
    
    // Calculate the number of bytes per character in the font array
    if (size == 8) 
        bytes_per_char = 6;
    else 
        bytes_per_char = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

    // Adjust character index based on ASCII table (subtract space ' ' character)
    chr_offset = chr - ' ';

    // Select appropriate font array based on size
    for (i = 0; i < bytes_per_char; i++) {
        if (size == 8)
            temp = ascii_0806[chr_offset][i]; // Load 0806 font
        else if (size == 12)
            temp = ascii_1206[chr_offset][i]; // Load 1206 font
        else if (size == 16)
            temp = ascii_1608[chr_offset][i]; // Load 1608 font
        else if (size == 24)
            temp = ascii_2412[chr_offset][i]; // Load 2412 font
        else if (size == 48)
            temp = ascii_4824[chr_offset][i]; // Load 4824 font
        else
            return; // Unsupported font size

        // Process each bit in the byte to draw pixels
        for (m = 0; m < 8; m++) {
            if (temp & 0x01)
                epd_draw_pixel(x, y, color);
            else
                epd_draw_pixel(x, y, !color);

            temp >>= 1; // Shift to process the next bit
            y++;
        }

        x++;

        // Handle multi-line characters
        if ((size != 8) && ((x - x0) == size / 2)) {
            x = x0;
            y0 += 8;
        }
        y = y0;
    }
}

/*******************************************************************
 * Function Description: Displays a string on the e-paper display.
 * 
 * Parameters:
 *   x      - X coordinate of the string
 *   y      - Y coordinate of the string
 *   *str   - Pointer to the string to be displayed
 *   size   - Font size of the string
 *   color  - Pixel color (1 for black, 0 for white)
 * 
 * Returns: None
 *******************************************************************/
void epd_show_string(uint16_t x, uint16_t y, const char *str, uint16_t size, uint16_t color) {
    while (*str != '\0') {  // Check if the character is valid (not null)
        epd_show_char(x, y, *str, size, color);
        str++;
        x += size / 2;  // Move to the next character position
    }
}

