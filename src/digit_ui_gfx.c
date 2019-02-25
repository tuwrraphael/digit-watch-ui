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

static float to_arg(float s)
{
    return ((float)(15 - s) * (float)M_PI) / ((float)30);
}

void draw_time_indicator(float s, float indicator_length, uint8_t thickness)
{
    float arg = to_arg(s);
    uint8_t x = (uint8_t)(64.0f + ((float)cos(arg) * indicator_length));
    uint8_t y = (uint8_t)(64.0f - ((float)sin(arg) * indicator_length));
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

typedef struct
{
    uint8_t minX;
    uint8_t maxX;
    uint8_t minY;
    uint8_t maxY;
} arc_boundaries_t;

static void arc_draw_area(uint8_t x, uint8_t y, arc_boundaries_t *arc_boundaries)
{
    if (y >= arc_boundaries->minY && x >= arc_boundaries->minX)
    {
        buffer_display_pixel_draw(x, y, 1);
    }
}

static void draw_arc_line(float s)
{
    uint8_t radius = 60;
    uint8_t outerRadius = 64;
    uint8_t centerX = 64;
    uint8_t centerY = 64;
    float arg = to_arg(s);
    float sinArg = (float)sin(arg);
    float cosArg = (float)cos(arg);
    uint8_t yOuter = (uint8_t)(centerY - (sinArg * outerRadius));
    uint8_t xOuter = (uint8_t)(centerX + (cosArg * outerRadius));
    uint8_t y = (uint8_t)(centerY - (sinArg * radius));
    uint8_t x = (uint8_t)(centerX + (cosArg * radius));

    if (x < xOuter || y < yOuter)
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            x,
            y,
            xOuter,
            yOuter, 1);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
    else
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            xOuter,
            yOuter,
            x,
            y, 1);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
}

void draw_time_arc(float from_s, float to_s)
{
    if (from_s > 60 || to_s > 60)
    {
        return;
    }
    float init;
    if (from_s > to_s)
    {
        init = from_s - 60.0f;
    }
    else
    {
        init = from_s;
    }
    for (float s = init; s < to_s; s += 0.3f)
    {
        draw_arc_line(s);
    }
}
