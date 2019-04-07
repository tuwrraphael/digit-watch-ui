#include "digit_ui_model.h"
#include "digit_ui_gfx.h"
#include "digit_assets.h"
#include "buffer_display.h"
#include "roboto8pt.h"
#include <stdio.h>

#include <time.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

// static uint8_t overflow_minutes(int16_t minutes)
// {
//     if (minutes < 0)
//     {
//         return 60 - minutes;
//     }
//     return minutes;
// }

typedef struct
{
    double departureIn;
    directions_leg_t *leg_ptr;
} next_leg_t;

static next_leg_t get_next_leg(digit_ui_state_t *state)
{
    next_leg_t next_leg = {
        .leg_ptr = NULL};
    for (uint8_t i = 0; i < state->directions.legs_count; i++)
    {
        if ((state->directions.valid_legs & (1 << i)) > 0)
        {
            next_leg.departureIn = difftime(state->directions.legs[i].departure_time, state->current_time);
            if (next_leg.departureIn > 0)
            {
                next_leg.leg_ptr = &state->directions.legs[i];
                break;
            }
        }
    }
    return next_leg;
}

void digit_ui_render(digit_ui_state_t *state)
{
    clear_display_buffer();
    struct tm current_time = *localtime(&state->current_time);
    uint16_t hourDisplay = ((current_time.tm_min * 5) / 60) + (current_time.tm_hour % 12) * 5;
    draw_time_indicator(current_time.tm_min, 50, 2);
    draw_time_indicator(hourDisplay, 35, 2);
    if (state->display_options.event_active && !state->display_options.directions_active)
    {
        double startIn = difftime(state->event_start_time, state->current_time);
        if (startIn >= 0 && startIn <= 1800)
        {
            render_timestamped_line(state->event_subject, (uint8_t)(startIn / 60), &icon_event, 80);
        }
    }
    else if (state->display_options.directions_active && state->display_options.event_active)
    {
        struct tm departureTime = *localtime(&state->directions.departure_time);
        struct tm arrivalTime = *localtime(&state->directions.arrival_time);

        draw_arc_line(departureTime.tm_min, &icon_leave2);
        draw_arc_line(arrivalTime.tm_min, &icon_target);
        double late = difftime(state->event_start_time, state->directions.arrival_time);
        if (late > 0)
        {
        }
        else
        {
        }
        double leaveIn = difftime(state->directions.departure_time, state->current_time);
        next_leg_t next_leg = get_next_leg(state);
        char line_display[20];

        if (leaveIn >= 0 && leaveIn <= 1800)
        {
            render_timestamped_line(state->event_subject, (uint8_t)(leaveIn / 60), &icon_event, 80);
            // if (NULL != next_leg.leg_ptr)
            // {
            //     snprintf(line_display, ARRAY_LEN(line_display), "%s %s", next_leg.leg_ptr->line, next_leg.leg_ptr->departure_stop);
            //     render_text_centered(line_display, 80 + roboto_8ptFontInfo.height);
            // }
        }
        else
        {
            if (NULL != next_leg.leg_ptr)
            {
                snprintf(line_display, ARRAY_LEN(line_display), "%s %s", next_leg.leg_ptr->line, next_leg.leg_ptr->direction);
                render_timestamped_line(line_display, (uint8_t)(next_leg.departureIn / 60), &icon_transit, 70 + roboto_8ptFontInfo.height);
                render_text_centered(next_leg.leg_ptr->departure_stop, 70);
            }
        }
    }
}