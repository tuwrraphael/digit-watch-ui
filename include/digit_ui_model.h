#ifndef DIGIT_UI_MODEL_H
#define DIGIT_UI_MODEL_H

#include <time.h>
#include <stdint.h>

typedef struct
{
    char arrival_stop[20];
    char departure_stop[20];
    char line[6];
    char direction[20];
    time_t departure_time;
} directions_leg_t;

typedef struct
{
    directions_leg_t *legs;
    uint8_t legs_count;
    time_t departure_time;
    time_t arrival_time;
} directions_t;

typedef union {
    struct
    {
        uint8_t directions_active : 1;
    };
    uint8_t flags;
} digit_ui_state_flags_t;

typedef struct
{
    time_t current_time;
    time_t event_start_time;
    char event_subject[20];
    directions_t directions;
    uint8_t displayed_step;
    digit_ui_state_flags_t display_options;
} digit_ui_state_t;

#define DIGIT_UI_STATE_DEFAULT           \
    {                                    \
        .display_options = {.flags = 0}, \
        .event_subject = "",             \
        .displayed_step = 1,             \
        .directions = {.legs_count = 0 } \
    }

#endif