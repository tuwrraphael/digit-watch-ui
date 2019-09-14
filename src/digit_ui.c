#include "digit_ui_model.h"
#include "digit_ui_gfx.h"
#include "digit_assets.h"
#include "buffer_display.h"
#include "roboto8pt.h"
#include <stdio.h>

#include <time.h>

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

#define SHOW_BEFORE_START_SECS (1800)
#define SHOW_AFTER_ARRIVE_SECS (300)

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
    double next_leg_departure;
    directions_leg_t *current_leg_ptr;
    directions_leg_t *next_leg_ptr;
} leg_info_t;

static leg_info_t get_leg_info(digit_ui_state_t *state)
{
    leg_info_t leg_info = {
        .next_leg_ptr = NULL,
        .current_leg_ptr = NULL};
    for (uint8_t i = 0; i < state->directions.legs_count; i++)
    {
        if ((state->directions.valid_legs & (1 << i)) > 0)
        {
            leg_info.next_leg_departure = difftime(state->directions.legs[i].departure_time, state->current_time);
            if (leg_info.next_leg_departure > 0)
            {
                leg_info.next_leg_ptr = &state->directions.legs[i];
                break;
            }
            else
            {
                leg_info.current_leg_ptr = &state->directions.legs[i];
            }
        }
    }
    return leg_info;
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
        if (startIn >= 0 && startIn <= SHOW_BEFORE_START_SECS)
        {
            render_timestamped_line(state->event_subject, (uint8_t)(startIn / 60), &icon_event, 80);
        }
    }
    else if (state->display_options.directions_active && state->display_options.event_active)
    {
        double late = difftime(state->event_start_time, state->directions.arrival_time);
        if (late > 0)
        {
        }
        else
        {
        }
        double leaveIn = difftime(state->directions.departure_time, state->current_time);
        double arriveIn = difftime(state->directions.arrival_time, state->current_time);
        if (leaveIn <= SHOW_BEFORE_START_SECS && arriveIn >= 0 - SHOW_AFTER_ARRIVE_SECS)
        {
            struct tm departureTime = *localtime(&state->directions.departure_time);
            struct tm arrivalTime = *localtime(&state->directions.arrival_time);
            draw_arc_line(departureTime.tm_min, &icon_leave2);
            draw_arc_line(arrivalTime.tm_min, &icon_target);
        }
        leg_info_t leg_info = get_leg_info(state);
        char line_display[20];

        if (leaveIn >= 0 && leaveIn <= SHOW_BEFORE_START_SECS)
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
            if (NULL != leg_info.next_leg_ptr)
            {
                snprintf(line_display, ARRAY_LEN(line_display), "%s %s", leg_info.next_leg_ptr->line, leg_info.next_leg_ptr->direction);
                render_timestamped_line(line_display, (uint8_t)(leg_info.next_leg_departure / 60), &icon_transit, 70 + roboto_8ptFontInfo.height);
                render_text_centered(leg_info.next_leg_ptr->departure_stop, 70);
            }
            else if (NULL != leg_info.current_leg_ptr && arriveIn >= 0)
            {
                render_timestamped_line("Ankunft", (uint8_t)(arriveIn / 60), &icon_walk, 70 + roboto_8ptFontInfo.height);
                render_text_centered(leg_info.current_leg_ptr->arrival_stop, 70);
            }
        }
    }
}