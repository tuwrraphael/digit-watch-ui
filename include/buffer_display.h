#ifndef APP_BUFFER_DISPLAY_H
#define APP_BUFFER_DISPLAY_H

#include "nrf_lcd.h"

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (128)
extern const nrf_lcd_t nrf_lcd_buffer_display;
extern uint32_t display_buffer[4 * 128];
#endif