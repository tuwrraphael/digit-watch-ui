#ifndef DIGIT_UI_GFX_H
#define DIGIT_UI_GFX_H
#include "packed_image.h"
#include <stdint.h>

#define MSG_SERVICES (2)

void render_packed_image(const packed_image_t *img, uint8_t m_x, uint8_t m_y);
void draw_time_indicator(float s, float indicator_length, uint8_t thickness);
void draw_arc_line(float s, const packed_image_t *icon);
void render_timestamped_line(char *text, uint8_t minutes, const packed_image_t *icon, uint8_t y);
void render_text_centered(char *text, uint8_t y);
void render_messages_bar(uint8_t message_counts[MSG_SERVICES], const packed_image_t *icons[MSG_SERVICES], uint8_t y);
#endif