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
 * v2.0.0 (Feb 2 2023):
 *   -- Move functions to oled driver
 *
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

#define softwareVersion "1.0.0"

static const char PROGMEM capslock24x24[] = {
        // 0,  0,  0,  0,  0,  0,  0,128,192, 96, 48, 24, 24, 48, 96,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 28, 30, 27,249,248,  0,  0,  0,  0,  0,  0,248,249, 27, 30, 28, 24,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,128,192, 96, 48, 24, 24, 48, 96,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 28, 22, 19,241,  0,  0,  0,  0,  0,  0,  0,  0,241, 19, 22, 28, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,  0,  0,  0,  0,  0,  0,  0,

        };

        static const char PROGMEM capslockwWord24x24[] = {
        0,  0,  0,  0,  0,  0,  0,128,192,224,240,248,248,240,224,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 28, 30, 31,248,192,143, 63,135,135, 63,143,192,248, 31, 30, 28, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,  0,  0,  0,  0,  0,  0,  0,

        };

        static const char PROGMEM layer72x16[] = {
            0,  0,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,204,204,204,204,204,220,252,248,240,  0,252,252,252,128,  0,128,192,240,252,252,252,  0,192,240,248,248,184,184,184,184,248,240,224,  0,248,248,248,224,240,120, 56, 56,120,240,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,127,127,112,112,112,112,112,112,112,112,  0, 15, 31,127,123,113,113,113,123, 63,127,127,  0,  0,  3, 51,115,115,115,115,121,127, 63, 31,  0, 15, 31, 63,123,115,115,115,115,123, 59,  9,  0, 63,127,127,  1,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

void placeByteAtPixel(uint8_t data, uint8_t right, uint8_t down, bool invert) {
    const uint8_t powers[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int i;
    uint8_t remainingValue = data;
    bool isOn;
    for (i = 7; i >= 0 ; i--) {
        // if (remainingValue == 0) break;
        if(right >= 128 || down + i >= 64) continue;
        isOn = remainingValue / powers[i] > 0;
        if (invert) isOn = !isOn;
        oled_write_pixel(right, down + i, isOn);
        remainingValue = remainingValue % powers[i]; 
    };
};


void oled_write_raw_sized(const char *data, int col, int row, int width, int height, bool invert) {
    const int len = width * height / 8;
    int i;
    for(i = 0; i < len; i++) {
        uint8_t c = pgm_read_byte(data++);
        // exceeded the desired of number of rows to create
        // if (i / width > height ) {
        //     break;dgfdgdg
        // };
        oled_write_byte_at_pixel(c, col + (i % width), (row + (8 * (i / width))), invert);
    };
};

// void oled_write_raw_P2(const char *data, int col, int row, int width, int height, bool invert) {
//     uint16_t cursor_start_index = col * row;
//     uint16_t size = width * height;
//     if ((size + cursor_start_index) > OLED_MATRIX_SIZE) size = OLED_MATRIX_SIZE - cursor_start_index;
//     for (uint16_t i = cursor_start_index; i < cursor_start_index + size; i++) {
//         uint8_t c = pgm_read_byte(data++);
//         // if (oled_buffer[i] == c) continue;
//         // oled_buffer[i] = c;
//         // oled_dirty |= ((OLED_BLOCK_TYPE)1 << (i / OLED_BLOCK_SIZE));
//     }
// }

void fill_section(uint8_t data, int col, int row, int width, int height, bool invert) {
    const int len = width * height / 8;
    int i;
    for(i = 0; i < len; i++) {
        // exceeded the desired of number of rows to create
        // if (i / width > height ) {
        //     break;
        // };
        placeByteAtPixel(data, col + (i % width), (row + (8 * (i / width))), invert);
    };
};

enum layers {
    _QWERTY = 0,
    _DVORAK,
    _COLEMAK_DH,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    _DAILY,
    _IDE
};

enum custom_keycodes {
    ELLIPSES = SAFE_RANGE,
    ARROW,
    CPY_PST,
    HELP,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    X_CTL,
    CAPS_WDLK,
    CPY_PST_TD
};

td_state_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);


// Aliases for readability
#define QWERTY   DF(_QWERTY)
#define COLEMAK  DF(_COLEMAK_DH)
#define DVORAK   DF(_DVORAK)

#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)
#define DAILY    MO(_DAILY)
#define IDE      MO(_IDE)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
#define CTL_MINS MT(MOD_RCTL, KC_MINUS)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)
#define UNDO G(KC_Z)
#define REDO SGUI(KC_Z)
#define SCREEN_TO_CLIPBOARD SGUI(KC_3)
#define SCREEN_CAPTURE SGUI(KC_5)
// #define TURN_ON_VO
#define PREV_APP SGUI(KC_TAB)
#define NEXT_APP G(KC_TAB)
#define PREV_WINDOW LSA(KC_TILD)
#define NEXT_WINDOW A(KC_TILD)
// #define MISSION_CONTROL
// #define APP_EXPOSE

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
    * Base Layer: QWERTY
    *
    * ,-------------------------------------------.                              ,-------------------------------------------.
    * |  Tab   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  | CP/PST |
    * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
    * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/' "|
    * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
    * | LShift |   Z  |   X  |   C  |   V  |   B  | [ {  |CapsLk|  |F-keys|  ] } |   N  |   M  | ,  < | . >  | /  ? | RShift |
    * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    *                        |Adjust| LCMD | LAlt/| Space| Nav  |  | Sym  | Space| AltGr| RCMD | Menu |
    *                        |      |      | Enter|      |      |  |      |      |      |      |      |
    *                        `----------------------------------'  `----------------------------------'
    */
    [_QWERTY] = LAYOUT(
     KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , CPY_PST,
     CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,                                        KC_H,   KC_J ,  KC_K ,   KC_L ,KC_SCLN,CTL_QUOT,
     KC_LSFT , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , TD(CAPS_WDLK), DAILY,        KC_CAPS  , TD(CPY_PST_TD), KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, KC_RSFT,
                                FKEYS , KC_LGUI, ALT_ENT, KC_BSPC , NAV   ,     SYM   , KC_SPC ,KC_RALT, KC_RGUI, DAILY
    ),   

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
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                  KC_PGUP, KC_HOME, KC_END,   KC_PGUP,  KC_PGDN, _______,
      _______, KC_LSFT, KC_LCTL, KC_LOPT, KC_LGUI, UNDO,                                     KC_PGDN, KC_LEFT, KC_RGHT, KC_UP, KC_DOWN, SCREEN_CAPTURE,
      _______, KC_VOLU, KC_VOLD, KC_MUTE, _______, REDO, _______, _______, _______, _______,XXXXXXX, PREV_APP, NEXT_APP, PREV_WINDOW, NEXT_WINDOW, SCREEN_TO_CLIPBOARD,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

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
    [_SYM] = LAYOUT(
     KC_GRV , KC_UNDS, KC_MINS, KC_AMPR, KC_LABK, KC_RABK,                                       ARROW  , KC_7 ,   KC_8 ,   KC_9 , XXXXXXX, _______,
     KC_PPLS, KC_EQL , KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC,                                     ELLIPSES, KC_4 ,   KC_5 ,   KC_6 , _______, _______,
     _______, KC_BSLS, KC_SLSH, KC_COLN, KC_LPRN, KC_RPRN, KC_PIPE, _______, _______, _______,    KC_0 , KC_1 ,   KC_2 ,   KC_3 , KC_QUES, _______,
                                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

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

    [_FUNCTION] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 , _______,
      _______, KC_LSFT, KC_LCTL, KC_LOPT, KC_LGUI, _______,                                     _______,  KC_F4 ,  KC_F5 ,  KC_F6 , KC_F11 , _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_F1 ,  KC_F2 ,  KC_F3 , KC_F12 , _______,
                                 _______, _______, _______, KC_DEL , _______, _______, _______, _______, _______, _______
    ),

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
    
    [_DAILY] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

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
    [_IDE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
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
    // logo editor: https://joric.github.io/qle/
    // Text to pixels: https://www.gamergeeks.net/apps/pixel/letter-text-generator
    // image to oled array: https://javl.github.io/image2cpp/
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        // static const char PROGMEM qmk_logo[] = {
        //     0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        //     0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        //     0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        static const char PROGMEM shortcuts24x24[] = {
            0xe0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf8, 0xf0, 0xe0, 0xe0, 0xf0, 0xf8, 0xfc, 
            0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf8, 0xf0, 0xc0, 0x07, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x0f, 0x07, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f, 
            0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        static const char  PROGMEM nav24x24[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 
            0xf8, 0xf8, 0xfc, 0xfc, 0xfc, 0x3e, 0x0e, 0x03, 0x08, 0x0c, 0x0c, 0x0e, 0x1e, 0x1f, 0x1f, 0x1f, 
            0x1f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x3f, 0x0f, 0x03, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        static const char  PROGMEM fn24x24[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0x70, 0x38, 0x18, 
            0x38, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x0c, 0x0c, 0x0c, 0xff, 0xff, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0e, 0x1c, 0x18, 0x1c, 0x0e, 0x0f, 0x03, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        static const char PROGMEM symbol24x24[] = {
                    0,  0,  0,  0,  0,  0,  0,  0,  0,254,254,  6,  0,  0,  0,254,254,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,195,195,195,255,255,195,195,195,195,255,255,195,195,195,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96,127,127,  0,  0,  0, 96,127,127,  0,  0,  0,  0,  0,  0,  0,  0,  0

        };
        
        

        static const char PROGMEM command24x24[] = {
            0x60, 0xf0, 0xf8, 0x1c, 0x1c, 0x18, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0x18, 0x1c, 
0x1c, 0xf8, 0xf0, 0x60, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 
0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x06, 0x0f, 0x1f, 0x38, 0x38, 0x18, 0x1f, 0x0f, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0x18, 0x38, 0x38, 0x1f, 0x0f, 0x06 
        };


        static const char PROGMEM control24x24[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0x0f, 0x07, 0x07, 0x0f, 
0x1e, 0x3c, 0x78, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00        };

    

        static const char PROGMEM opt24x32[] = {
            0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 
0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3e, 0xf8, 0xe0, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x0f, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00
        };
    

        

        static const char PROGMEM shift24x24[] = {
             0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x1e, 0x1b, 0xf9, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xf8, 0xf9, 0x1b, 0x1e, 0x1c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x10, 
0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        
        // oled_write_P(PSTR(softwareVersion), false);

        uint8_t mod_state = get_mods();
        const int row = 0;
        // clear_section(255, 0, 0, 127, 58, true);
        if(true) {
            oled_write_raw_sized(command24x24, 0, row, 20, 24, mod_state &  MOD_MASK_GUI); // icon made in white
            oled_write_raw_sized(opt24x32, 26, row, 20, 24, mod_state & MOD_MASK_ALT);
            oled_write_raw_sized(control24x24, 52, row, 20, 24, mod_state & MOD_MASK_CTRL); // 52
            oled_write_raw_sized(shift24x24, 78, row, 20, 24, mod_state & MOD_MASK_SHIFT); // creates weirdness
        }
        
        
        


        // Host Keyboard Layer Statusggg
        // oled_write_raw_sized(shift24x24, 0, 16, 20, 24, false);
        if(true) oled_write_raw_sized(layer72x16, 0, 36, 72, 16, false);
        // oled_write_raw_sized(layer72x16, 0, 0, 72, 16, true);

        
       
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _QWERTY:
                fill_section(0, 74, 32, 24, 24, false); // blank out symbols
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
                // fill_section(255, 74, 32, 24, 24, false);
        }
    
        led_t led_usb_state = host_keyboard_led_state();
        if (true) oled_write_raw_sized(is_caps_word_on() ? capslockwWord24x24 : capslock24x24, 104, row, 24, 24, led_usb_state.caps_lock);
        
        

        // oled_set_cursor(50, 8);
        // oled_write_P(PSTR(softwareVersion), false);

    } else {
        // ORIGINAL
        // clang-format off
        // static const char PROGMEM kyria_logo[] = {
        //     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        //     0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        //     0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        //     0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        //     0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        //     0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        //     0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        //     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        // };
        // // clang-format on
        // oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
        static const char PROGMEM raw_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0, 
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 51, 63,  0,  0,  0,  0,  0, 
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 62,  4,  2,  2,  4,  0, 28, 42, 42, 42, 36,  0,  6, 24, 32, 24,  6,  0,  0,  0, 50, 42, 42, 42, 46,  0, 32,  0, 34, 34, 62, 32, 32,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,120,160,160,160, 64,  0, 96,128,128,128, 96,  0,232,  0,248,  0, 32,248, 32,  0,  0,  0,120,160,160,160, 64,  0, 24,160,160,120,  0,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,248,252, 12, 12, 12, 12,252,248,  0,  0,252,252,  0,  0,  0,  0,  0,248,252, 12, 12, 12, 12,252,248,  0,  0,252,252,112,112,128,128,  0,  0,252,252,  0,  0, 12, 12,252,252, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,255,255,  3,  3,  3,  3,255,255,  0,  0,255,255,192,192,192,  0,  0,255,255,  3,  3,  3,  3,255,255,  0,  0,255,255,  0,  0,  3,  3, 28, 28,255,255,  0,  0,192,192,255,255,192,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        
        };   
        oled_write_raw_P(raw_logo, sizeof(raw_logo));
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

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_X); break;
        case TD_SINGLE_HOLD: register_code(KC_LCTL); break;
        case TD_DOUBLE_TAP: register_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: register_code(KC_LALT); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_X); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LCTL); break;
        case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LALT); break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}

void caps_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: caps_word_toggle(); break;
        case TD_DOUBLE_TAP: register_code(KC_CAPS); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_DOUBLE_TAP: unregister_code(KC_CAPS); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}

void cpy_pst_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: register_code16(G(KC_V)); break;  // command paste - less friction compare to double tap
        case TD_DOUBLE_TAP: register_code16(G(KC_C)); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void cpy_pst_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: unregister_code16(G(KC_V)); break;
        case TD_DOUBLE_TAP: unregister_code16(G(KC_C)); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}


qk_tap_dance_action_t tap_dance_actions[] = {
    [X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
    [CAPS_WDLK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
    [CPY_PST_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cpy_pst_finished, cpy_pst_reset)
};



