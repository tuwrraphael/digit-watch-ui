#ifndef APP_BUFFER_DISPLAY_H
#define APP_BUFFER_DISPLAY_H

#include "nrf_lcd.h"

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (128)
#define DISPLAY_CENTER_X (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y (DISPLAY_HEIGHT / 2)

extern const nrf_lcd_t nrf_lcd_buffer_display;
extern uint32_t display_buffer[4 * 128];

void clear_display_buffer();
void buffer_display_pixel_draw(uint16_t m_x, uint16_t m_y, uint32_t color);
#endif