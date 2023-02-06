/* Author: Alani Johnson
 * v0 (Aug 2022): 
 *   -- Base functionality with modifications (see details on derivation from Splitkb v2.1)
 *   
 *   Modifications Include:
 *    - Additonal layers
 *    - Copy / Paste button
 *    - Custom Keycodes (Ellipses, arrow, daily (incomplete), ide (incomplete))
 *    - Mac specific commands based on Alani's mac set-up
 *
 * v0.1 (Dec 2022):
 *   -- Update OLED on follower 1/2 to include built by Alani
 *   -- Addition of help button. The help button allows user to see what keyboard code is being pressed. (This functionality was not implemented)
 *
 * v1.0.0 (Jan 29 2023):
 *   -- Add OLED display characters
 *   -- Added Tap Dance Support
 *   -- Added Copy paste tap dance
 *        -- Tap: 
 *        -- Double Tap:
 *   -- Added Caps caps word tap dance
 *
 * v2.0.0 (Feb 5 2023):
 *   -- Move functions to oled driver
 *
 * v2.1.0 (Feb 5 2023):
 *  -- Refactor files
 *
 * Details on derivation below
 * Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include <string.h>
#include <stdint.h>
#include "progmem.h"
#include "oled_constants.c"
#include "layers.c"
#include "enums.c"


#define softwareVersion "1.0.0"

// #define MISSION_CONTROL
// #define APP_EXPOSE

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_wrapper(qwerty_layout),
    [_NAV] = LAYOUT_wrapper(nav_layer),
    [_SYM] = LAYOUT_wrapper(sym_layer),
    [_DAILY] = LAYOUT_wrapper(daily_layer),
    [_FUNCTION] = LAYOUT_wrapper(function_layer),

};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

 // https://github.com/qmk/qmk_firmware/blob/master/keyboards/cannonkeys/satisfaction75/satisfaction_oled.c#L112
 // https://www.youtube.com/watch?v=qP6VCA1q77M&t=968s


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    
    if (is_keyboard_master()) {
        uint8_t mod_state = get_mods();
        const int row = 0;

        if(true) {
            oled_write_raw_sized(command24x24, 0, row, 20, 24, mod_state &  MOD_MASK_GUI); // icon made in white
            oled_write_raw_sized(opt24x32, 26, row, 20, 24, mod_state & MOD_MASK_ALT);
            oled_write_raw_sized(control24x24, 52, row, 20, 24, mod_state & MOD_MASK_CTRL); // 52
            oled_write_raw_sized(shift24x24, 78, row, 20, 24, mod_state & MOD_MASK_SHIFT); // creates weirdness
        }
        
        // Host Keyboard Layer Status
        if(true) oled_write_raw_sized(layer72x16, 0, 36, 72, 16, false);
        
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _QWERTY:
                oled_fill_section(0, 74, 32, 24, 24, false); // blank out symbols
                // fill_section(255, 20, 76, 24, 24, false);
                break;
            case _NAV:
                oled_write_raw_sized(nav24x24, 74, 32, 24, 24, isOneShot);
                break;
            case _SYM:
                oled_write_raw_sized(symbol24x24, 74, 32, 24, 24, isOneShot);
                break;
            case _FUNCTION:
                oled_write_raw_sized(fn24x24, 74, 32, 24, 24, isOneShot);
                break;
            case _DAILY:
                oled_write_raw_sized(shortcuts24x24, 74, 32, 24, 24, isOneShot);
                break;
        }
    
        led_t led_usb_state = host_keyboard_led_state();
        if (true) oled_write_raw_sized(is_caps_word_on() ? capslockwWord24x24 : capslock24x24, 104, row, 24, 24, led_usb_state.caps_lock);
    } else {
        oled_write_raw_P(kyria_alani, sizeof(kyria_alani));
    }
    return false;
}
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ARROW:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("=>");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case ELLIPSES:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("...");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case CPY_PST:  // Hold to Copy, Hold with GUI to Cut, Tap to Paste
            {
            static uint16_t copy_paste_timer;
                if (record->event.pressed) {
                copy_paste_timer = timer_read();
                } else {
                if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {
                    if (get_mods() & MOD_MASK_GUI) {
                    tap_code16(LGUI(KC_X));
                    } else {
                    tap_code16(LGUI(KC_C));
                    }
                } else {
                    tap_code16(LGUI(KC_V));
                }
                }
            }
            break;
    }
    return true;
};





