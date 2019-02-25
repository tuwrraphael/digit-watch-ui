#ifndef DIGIT_UI_GFX_H
#define DIGIT_UI_GFX_H
#include "packed_image.h"
#include <stdint.h>
void render_packed_image(packed_image_t *img, uint8_t m_x, uint8_t m_y);
void draw_time_indicator(float s, float indicator_length, uint8_t thickness);
void draw_time_arc(float from_s, float to_s);
#endif