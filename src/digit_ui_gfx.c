#include "packed_image.h"
#include <stdint.h>
#include <string.h>
#include "nrf_gfx.h"
#include "buffer_display.h"
#include "math.h"

#include "digit_ui_gfx.h"
#include "roboto8pt.h"
#include <stdio.h>

static const FONT_INFO *p_font = &roboto_8ptFontInfo;

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
    uint8_t x = (uint8_t)(DISPLAY_CENTER_X + ((float)cos(arg) * indicator_length));
    uint8_t y = (uint8_t)(DISPLAY_CENTER_Y - ((float)sin(arg) * indicator_length));
    if (x < DISPLAY_CENTER_X || y < DISPLAY_CENTER_Y)
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            x,
            y,
            DISPLAY_CENTER_X,
            DISPLAY_CENTER_Y, thickness);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
    else
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            DISPLAY_CENTER_X,
            DISPLAY_CENTER_Y,
            x,
            y, thickness);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
}

void draw_arc_line(float s, const packed_image_t *icon)
{
    uint8_t radius = 60;
    uint8_t outerRadius = 64;
    uint8_t centerX = DISPLAY_CENTER_X;
    uint8_t centerY = DISPLAY_CENTER_Y;
    float arg = to_arg(s);
    float sinArg = (float)sin(arg);
    float cosArg = (float)cos(arg);
    float yOuter = (centerY - (sinArg * outerRadius));
    float xOuter = (centerX + (cosArg * outerRadius));
    float y = (centerY - (sinArg * radius));
    float x = (centerX + (cosArg * radius));

    if (x < xOuter || y < yOuter)
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            (uint8_t)x,
            (uint8_t)y,
            (uint8_t)xOuter,
            (uint8_t)yOuter, 1);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
    else
    {
        nrf_gfx_line_t line = NRF_GFX_LINE(
            (uint8_t)xOuter,
            (uint8_t)yOuter,
            (uint8_t)x,
            (uint8_t)y, 1);
        (nrf_gfx_line_draw(&nrf_lcd_buffer_display, &line, 1));
    }
    if (NULL != icon)
    {
        float vecX = (x - xOuter) * 1.5f;
        float vecY = (y - yOuter) * 1.5f;
        float imgCenterX = x + vecX;
        float imgCenterY = y + vecY;
        uint8_t imgX = (uint8_t)(imgCenterX - icon->width / 2);
        uint8_t imgY = (uint8_t)(imgCenterY - icon->height / 2);
        render_packed_image(icon, imgX, imgY);
    }
}

static uint8_t measure(char *text)
{
    char *p = text;
    uint8_t length = 0;
    while (*p != '\0')
    {
        if (*p == ' ')
        {
            length += p_font->height / 2;
        }
        else
        {
            if (0 != length)
            {
                length += p_font->spacePixels;
            }
            length += p_font->charInfo[*p - p_font->startChar].widthBits;
        }
        p++;
    }
    return length;
}

#define TIME_ICON_PADDING (2)
#define ICON_FONT_Y_ALIGN (1)
#define BORDER_RADIUS (63)
static double border_padding(uint8_t y, uint8_t height)
{
    if (y > DISPLAY_CENTER_Y)
    {
        y += height;
    }
    if (y > (BORDER_RADIUS + DISPLAY_CENTER_Y)) //prevent negative sqrt
    {
        return DISPLAY_CENTER_X;
    }
    return (DISPLAY_CENTER_X - sqrt(BORDER_RADIUS * BORDER_RADIUS - (y - DISPLAY_CENTER_Y) * (y - DISPLAY_CENTER_Y)));
}

void render_timestamped_line(char *text, uint8_t minutes, const packed_image_t *icon, uint8_t y)
{
    if (minutes > 99)
    {
        minutes = 0;
    }
    char timeString[2];
    sprintf((char *)timeString, (const char *)"%d", minutes);
    uint8_t timeLength = measure(timeString);
    int16_t printX = DISPLAY_CENTER_X - (measure(text) + icon->width + timeLength + TIME_ICON_PADDING)/2;
    double borderPadding = border_padding(y, p_font->height);
    if (printX < borderPadding)
    {
        printX = icon->width + timeLength + TIME_ICON_PADDING + borderPadding;
    } else {
        printX = DISPLAY_CENTER_X - (measure(text) - icon->width - timeLength - TIME_ICON_PADDING)/2;
    }
    nrf_gfx_point_t text_start = NRF_GFX_POINT(printX, y);
    nrf_gfx_print(&nrf_lcd_buffer_display, &text_start, 1, text, p_font, false);
    printX -= icon->width;
    render_packed_image(icon, (uint8_t)printX, y - ICON_FONT_Y_ALIGN);
    text_start.x = printX - timeLength - TIME_ICON_PADDING;
    nrf_gfx_print(&nrf_lcd_buffer_display, &text_start, 1, timeString, p_font, false);
}

void render_text_centered(char *text, uint8_t y)
{
    int16_t printX = DISPLAY_CENTER_X - measure(text) / 2;
    if (printX < 0)
    {
        printX =  border_padding(y, p_font->height);
    }
    nrf_gfx_point_t text_start = NRF_GFX_POINT(printX, y);
    nrf_gfx_print(&nrf_lcd_buffer_display, &text_start, 1, text, p_font, false);
}