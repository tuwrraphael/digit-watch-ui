#include "packed_image.h"
#include <stdint.h>
#include "nrf_gfx.h"
#include "buffer_display.h"
#include "math.h"

#define M_PI (3.14159265358979323846)

void render_packed_image(const packed_image_t *img, uint8_t m_x, uint8_t m_y)
{
    uint32_t col_mask = ~(0xFFFFFFFF << img->height);
    uint8_t startBit = img->startBit;
    uint8_t y = 128 - m_y - img->height + 1;
    uint8_t imgByte = img->startByte;
    for (uint8_t row = 0; row < img->width; row++)
    {
        uint8_t x = m_x + row;
        uint16_t startIndex = 4 * (x) + ((y) / 32);
        uint16_t targetBit = (y) % 32;
        uint32_t copy_mask, to_copy;
        copy_mask = (col_mask << startBit);
        to_copy = (img->data[imgByte] & copy_mask) >> startBit;
        if (startBit + img->height > 32)
        {
            imgByte++;
            uint8_t copiedBefore = 32 - startBit;
            copy_mask = (col_mask >> copiedBefore);
            to_copy |= ((img->data[imgByte] & copy_mask) << copiedBefore);
            startBit = img->height - copiedBefore;
        }
        else
        {
            startBit += img->height;
            imgByte = imgByte + (startBit / 32);
            startBit = startBit % 32;
        }
        display_buffer[startIndex] &= ~(to_copy << targetBit);
        uint8_t copied = (32 - targetBit);
        uint8_t leftOver = copied < img->height ? img->height - copied : 0;
        if (leftOver > 0)
        {
            startIndex++;
            display_buffer[startIndex] &= ~(to_copy >> copied);
        }
    }
}

void draw_time_indicator(float s, float indicator_length, uint8_t thickness)
{
    float arg = ((float)(15 - s) * M_PI) / ((float)30);
    uint8_t x = 64.0f + (cos(arg) * indicator_length);
    uint8_t y = 64.0f - (sin(arg) * indicator_length);
    if (x < 64 || y < 64)
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            x,
            y,
            64,
            64, thickness);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
    else
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            64,
            64,
            x,
            y, thickness);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
}