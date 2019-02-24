#include "nrf_gfx.h"
#include "buffer_display.h"
#include <string.h>

uint32_t display_buffer[4 * 128];

static void clear_display_buffer()
{
    memset(display_buffer, 0xFFFFFFFF, sizeof(display_buffer));
}

static ret_code_t disp_def_init(void)
{
    clear_display_buffer();
    return NRF_SUCCESS;
}

static void disp_def_uninit(void)
{
}

static void disp_def_pixel_draw(uint16_t m_x, uint16_t m_y, uint32_t color)
{
    uint16_t x = 128 - m_y;
    uint16_t y = m_x;
    uint16_t index = 4 * y + (x / 32);
    uint32_t mask = (1 << x % 32);
    if (color)
    {
        display_buffer[index] &= ~(mask);
    }
    else
    {
        display_buffer[index] |= mask;
    }
}

static void disp_def_rect_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    uint16_t i, z;
    for (i = 0; i < height; i++)
    {
        for (z = 0; z < width; z++)
        {
            disp_def_pixel_draw(x + z, y + i, color);
        }
    }
}

static void disp_def_dummy_display(void)
{
}

static void disp_def_rotation_set(nrf_lcd_rotation_t rotation)
{
}

static void disp_def_display_invert(bool invert)
{
}

static lcd_cb_t display_cb = {
    .height = DISPLAY_HEIGHT,
    .width = DISPLAY_WIDTH};

const nrf_lcd_t nrf_lcd_buffer_display = {
    .lcd_init = disp_def_init,
    .lcd_uninit = disp_def_uninit,
    .lcd_pixel_draw = disp_def_pixel_draw,
    .lcd_rect_draw = disp_def_rect_draw,
    .lcd_display = disp_def_dummy_display,
    .lcd_rotation_set = disp_def_rotation_set,
    .lcd_display_invert = disp_def_display_invert,
    .p_lcd_cb = &display_cb};