#include "digit_ui_model.h"
#include "digit_ui_gfx.h"

#include <time.h>

static uint8_t overflow_minutes(int16_t minutes)
{
    if (minutes < 0)
    {
        return 60 - minutes;
    }
    return minutes;
}

void digit_ui_render(digit_ui_state_t *state)
{
    struct tm current_time = *localtime(&state->current_time);
    uint16_t hourDisplay = ((current_time.tm_min * 5) / 60) + (current_time.tm_hour % 12) * 5;
    draw_time_indicator(current_time.tm_min, 50, 2);
    draw_time_indicator(hourDisplay, 35, 2);
    if (state->display_options.directions_active)
    {
        struct tm departureTime = *localtime(&state->directions.departure_time);
        struct tm arrivalTime = *localtime(&state->directions.arrival_time);
        struct tm eventStartTime = *localtime(&state->event_start_time);
        draw_time_arc(overflow_minutes(departureTime.tm_min - 6), departureTime.tm_min);
        double late = difftime(&state->event_start_time, &state->directions.arrival_time);
        if (late > 0)
        {
            draw_time_arc(eventStartTime.tm_min, arrivalTime.tm_min);
        }
        else
        {
            draw_time_arc(arrivalTime.tm_min, eventStartTime.tm_min);
        }
        double leaveIn = difftime(&state->current_time, &state->directions.departure_time);
        if (leaveIn > 0 && leaveIn < 6) {
            
        }
    }
}