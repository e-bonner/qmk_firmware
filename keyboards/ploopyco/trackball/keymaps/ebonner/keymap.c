/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN1, KC_BTN3, DRAG_SCROLL, KC_BTN4, KC_BTN2 ),
    [1] = LAYOUT( DPI_CONFIG, _______, _______, KC_BTN5, _______ ),
    [2] = LAYOUT( _______, _______, _______, _______, _______ ),
    [3] = LAYOUT( _______, _______, _______, _______, _______ ),
    [4] = LAYOUT( _______, _______, _______, _______, _______ ),
    [5] = LAYOUT( _______, _______, _______, _______, _______ ),
    [6] = LAYOUT( _______, _______, _______, _______, _______ ),
    [7] = LAYOUT( _______, _______, _______, _______, _______ )
};

// Sensor timeout state
#define SLEEP_TIME 300000 // ms, if no movement seen will "sleep"
#define SLEEP_EXIT_EVENTS 20 // pixels accumulated to trigger sleep exit, max 127
uint32_t last_move_time;
int8_t x_accum = 0;  // Keep accumulated movement while sleeping
int8_t y_accum = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (mouse_report.x != 0 || mouse_report.y != 0) {
        if (timer_elapsed32(last_move_time) > SLEEP_TIME) {
            // Sleeping
            x_accum += mouse_report.x;
            y_accum += mouse_report.y;
            if (abs(x_accum) > SLEEP_EXIT_EVENTS || abs(y_accum) > SLEEP_EXIT_EVENTS) {
                // Exit sleep mode
                last_move_time = timer_read32();
                mouse_report.x = x_accum;
                mouse_report.y = y_accum;
                x_accum = 0;
                y_accum = 0;
            } else {
                mouse_report.x = 0;
                mouse_report.y = 0;
            }
        } else {
            last_move_time = timer_read32();
        }
    }
    
    return mouse_report;
}

// Bool return specifies whether kb-level handler should run
bool encoder_update_user(uint8_t index, bool clockwise) {
    // Suppress scroll wheel events (the scroll wheel sensor can be
    // noisy if it is on a transition point or not calibrated yet
    return timer_elapsed32(last_move_time) <= SLEEP_TIME;
}
