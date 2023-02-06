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

    /*
    * Base Layer: Dvorak
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |  Tab   | ' "  | , <  | . >  |   P  |   Y  |                              |   F  |   G  |   C  |   R  |   L  |  Bksp  |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |Ctrl/Esc|   A  |   O  |   E  |   U  |   I  |                              |   D  |   H  |   T  |   N  |   S  |Ctrl/- _|
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * | LShift | ; :  |   Q  |   J  |   K  |   X  | [ {  |CapsLk|  |F-keys|  ] } |   B  |   M  |   W  |   V  |   Z  | RShift |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
    *                        |      |      | Enter|      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
        // [_DVORAK] = LAYOUT(
        //  KC_TAB  ,KC_QUOTE,KC_COMM,  KC_DOT,   KC_P ,   KC_Y ,                                        KC_F,   KC_G ,  KC_C ,   KC_R ,  KC_L , KC_BSPC,
        //  CTL_ESC , KC_A ,  KC_O   ,  KC_E  ,   KC_U ,   KC_I ,                                        KC_D,   KC_H ,  KC_T ,   KC_N ,  KC_S , CTL_MINS,
        //  KC_LSFT ,KC_SCLN, KC_Q   ,  KC_J  ,   KC_K ,   KC_X , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_B,   KC_M ,  KC_W ,   KC_V ,  KC_Z , KC_RSFT,
        //                              ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
        // ),

    /*
    * Base Layer: Colemak DH
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |  Tab   |   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ;  : |  Bksp  |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |Ctrl/Esc|   A  |   R  |   S  |   T  |   G  |                              |   M  |   N  |   E  |   I  |   O  |Ctrl/' "|
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * | LShift |   Z  |   X  |   C  |   D  |   V  | [ {  |CapsLk|  |F-keys|  ] } |   K  |   H  | ,  < | . >  | /  ? | RShift |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |Adjust| LGUI | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
    *                        |      |      | Enter|      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
        // [_COLEMAK_DH] = LAYOUT(
        //  KC_TAB  , KC_Q ,  KC_W   ,  KC_F  ,   KC_P ,   KC_B ,                                        KC_J,   KC_L ,  KC_U ,   KC_Y ,KC_SCLN, KC_BSPC,
        //  CTL_ESC , KC_A ,  KC_R   ,  KC_S  ,   KC_T ,   KC_G ,                                        KC_M,   KC_N ,  KC_E ,   KC_I ,  KC_O , CTL_QUOT,
        //  KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_D ,   KC_V , KC_LBRC,KC_CAPS,     FKEYS  , KC_RBRC, KC_K,   KC_H ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
        //                              ADJUST, KC_LGUI, ALT_ENT, KC_SPC , NAV   ,     SYM    , KC_SPC ,KC_RALT, KC_RGUI, KC_APP
        // ),

    /*
    * Nav Layer: Media, navigation
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.b
    * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  | VolUp| Delete |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |        |  GUI |  Alt | Ctrl | Shift|      |                              | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    *
    *
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  |      |        |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |        |shift |  ctl | opt  | gui  | undo |                              | PgDn |  ←   |   →  |   ↓  |  ↑    | screen cap |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |  shift | volup |voldn| mute |      | redo |   ~  | Tab  |  |      |      | Pause|M Prev|M Play|M Next|      | PrtSc  |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
    // [_NAV] = LAYOUT(
    //   _______, _______, _______, _______, _______, _______,                                  KC_PGUP, KC_HOME, KC_END,   KC_PGUP,  KC_PGDN, _______,
    //   _______, KC_LSFT, KC_LCTL, KC_LOPT, KC_LGUI, UNDO,                                     KC_PGDN, KC_LEFT, KC_RGHT, KC_UP, KC_DOWN, SCREEN_CAPTURE,
    //   _______, KC_VOLU, KC_VOLD, KC_MUTE, _______, REDO, _______, _______, _______, _______,XXXXXXX, PREV_APP, NEXT_APP, PREV_WINDOW, NEXT_WINDOW, SCREEN_TO_CLIPBOARD,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),

    /*
    * Sym Layer: Numbers and symbols
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |    `   |  1   |  2   |  3   |  4   |  5   |                              |   6  |  7   |  8   |  9   |  0   |   =    |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |    ~   |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |   +    |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |    |   |   \  |  :   |  ;   |  -   |  [   |  {   |      |  |      |   }  |   ]  |  _   |  ,   |  .   |  /   |   ?    |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    *
    *
    *
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |  ` ~   |  _   |  -   | &    | , >  |  . < |                              |  =>  |  7 & |  8 * |  9 ( |      |   DEL  |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |   +    |  =   |   {  |  }   | } ]  | { ]  |                              |  ... |  4 $ |  5 % |  6 ^ |      |        |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |  SHIFT |   \  |  /   |  :   |  (   |   )  |  |   |      |  |      |  0 ) |  1 ! |  2 @ |  3 # |   ?  |      | SHIFT  |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      | SHIFT|      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
    // [_SYM] = LAYOUT( f].[]|]
    //   KC_GRV ,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                                       KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_EQL ,
    //  KC_TILD , KC_EXLM,  KC_AT , KC_HASH,  KC_DLR, KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
    //  KC_PIPE , KC_BSLS, KC_COLN, KC_SCLN, KC_MINS, KC_LBRC, KC_LCBR, _______, _______, KC_RCBR, KC_RBRC, KC_UNDS, KC_COMM,  KC_DOT, KC_SLSH, KC_QUES,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),
    // [_SYM] = LAYOUT(
    //  KC_GRV , KC_UNDS, KC_MINS, KC_AMPR, KC_LABK, KC_RABK,                                       ARROW  , KC_7 ,   KC_8 ,   KC_9 , XXXXXXX, _______,
    //  KC_PPLS, KC_EQL , KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC,                                     ELLIPSES, KC_4 ,   KC_5 ,   KC_6 , _______, _______,
    //  _______, KC_BSLS, KC_SLSH, KC_COLN, KC_LPRN, KC_RPRN, KC_PIPE, _______, _______, _______,    KC_0 , KC_1 ,   KC_2 ,   KC_3 , KC_QUES, _______,
    //                             _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),

    /*
    * Function Layer: Function keys
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |        |      |      |      |      |      |                              |      |  F7  |  F8  |  F9  | f10  |        |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |        |shift |  ctl | opt  | gui  |      |                              |      | F4   | F5   |  F6  |  f11 |        |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |        |      |      |      |      |      |      |      |  |      |      |      |  F1  |  F2  |  F3  |  f12 |        |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    *
    *
    *
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |        |  F9  | F10  | F11  | F12  |      |                              |      |      |      |      |      |        |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |        |  F5  |  F6  |  F7  |  F8  |      |                              |      | Shift| Ctrl |  Alt |  GUI |        |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
    // [_FUNCTION] = LAYOUT(
    //   _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     _______, _______, _______, _______, _______, _______,
    //   _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
    //   _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),

    // [_FUNCTION] = LAYOUT(
    //   _______, _______, _______, _______, _______, _______,                                     _______,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 , _______,
    //   _______, KC_LSFT, KC_LCTL, KC_LOPT, KC_LGUI, _______,                                     _______,  KC_F4 ,  KC_F5 ,  KC_F6 , KC_F11 , _______,
    //   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_F1 ,  KC_F2 ,  KC_F3 , KC_F12 , _______,
    //                              _______, _______, _______, KC_DEL , _______, _______, _______, _______, _______, _______
    // ),

    /*
    * Adjust Layer: Default layer settings, RGB
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |        |      |      |QWERTY|      |      |                              |      |      |      |      |      |        |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |        |      |      |Dvorak|      |      |                              | TOG  | SAI  | HUI  | VAI  | MOD  |        |
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * |        |      |      |Colmak|      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        |      |      |      |      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
    // [_ADJUST] = LAYOUT(
    //   _______, _______, _______, QWERTY , _______, _______,                                    _______, _______, _______, _______,  _______, _______,
    //   _______, _______, _______, DVORAK , _______, _______,                                    RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI,  RGB_MOD, _______,
    //   _______, _______, _______, COLEMAK, _______, _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______,
    //                              _______, _______, _______,_______, _______, _______, _______, _______, _______, _______
    // ),
    // /*
    //  * DAILY LAYER
    //  *
    //  * ,-------------------------------------------.                              ,-------------------------------------------.
    //  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    //  * |        |      |      | [[[[[[]]]]]]     | Screen   |      |                              |      |      |      |      |      |        |
    //  * |        |      |      |      | Capture  |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        `----------------------------------'  `----------------------------------'
    //  */
    
    // [_DAILY] = LAYOUT(
    //   _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //   _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),

    // /*
    //  * Layer template
    //  *
    //  * ,-------------------------------------------.                              ,-------------------------------------------.
    //  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        `----------------------------------'  `----------------------------------'
    //  */
    // [_IDE] = LAYOUT(
    //   _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //   _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
    //   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    // ),
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
                oled_write_raw_sized(nav24x24, 74, 32, 24, 24, false);
                break;
            case _SYM:
                oled_write_raw_sized(symbol24x24, 74, 32, 24, 24, false);
                break;
            case _FUNCTION:
                oled_write_raw_sized(fn24x24, 74, 32, 24, 24, false);
                break;
            case _DAILY:
                oled_write_raw_sized(shortcuts24x24, 74, 32, 24, 24, false);
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





