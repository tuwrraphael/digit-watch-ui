#ifndef DIGIT_UI_MODEL_H
#define DIGIT_UI_MODEL_H

#include <time.h>
#include <stdint.h>

typedef struct
{
    char arrival_stop[10];
    char departure_stop[10];
    char line[4];
    char direction[10];
    time_t departure_time;
} directions_leg_t;

typedef struct
{
    directions_leg_t *legs;
    uint8_t legs_count;
    time_t departure_time;
    time_t arrival_time;
} directions_t;

#endif
