#ifndef TAPDANCE 
#define TAPDANCE 

#include "enums.c"
#include <print.h>
#define ONESHOT_TIMEOUT 200  /* Time (in ms) before the one shot key is released */

bool isOneShot = false;
bool locked_layer = false;

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

typedef struct {
    int layer;
} td_layer_tap_t;



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

void layer_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_layer_tap_t *data = (td_layer_tap_t *)user_data;
    int layer = data->layer;
    // int current_layer = get_highest_layer(layer_state);
    print("finished\n");
    // uprintf("layer: %d\n", layer);

    
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: { 
            // uprintf("finish start - oneshot layer %d\n", get_oneshot_layer());
            // uprintf("finish start - layer active %d\n", layer_state_is(layer));
            // if (layer_state_is(layer)) {
            //     layer_off(layer);
            // }

            
            // uprintf("finish end - oneshot layer %d\n", get_oneshot_layer());
            // uprintf("finish end - layer active %d\n", layer_state_is(layer));
            // set_oneshot_layer(layer, ONESHOT_START);
            break;
        }   
        case TD_SINGLE_HOLD: { // set layer until release
            layer_move(layer); 
            break; 
        } 
        case TD_DOUBLE_TAP: { // lock layer toggle. Turns off any existing layers
            uprintf("finish double - oneshot layer %d\n", get_oneshot_layer());
            uprintf("finish double - layer active %d\n", layer_state_is(layer));

            // one shot layer is active. Restore state so button can function as lock
            if (get_oneshot_layer() == layer) {
                print("resetting oneshot + turning off layer\n");
                reset_oneshot_layer();
                layer_off(layer);
            }

            if (layer_state_is(layer)) {
                print("turning off layer\n");
                layer_off(layer);
            } else {
                print("turning on layer\n");
                layer_on(layer);
            }
        } // lock layer, if layer is locked clear
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void layer_reset(qk_tap_dance_state_t *state, void *user_data) {
    td_layer_tap_t *data = (td_layer_tap_t *)user_data;
    int layer = data->layer;
    print("reset\n");
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: {
            uprintf("reset start - oneshot layer %d\n", get_oneshot_layer());
            uprintf("reset start - layer active %d\n", layer_state_is(layer));

            if(get_oneshot_layer() == layer || layer_state_is(layer)) { 
                if(get_oneshot_layer() == layer) {
                    print("reset os\n");
                    reset_oneshot_layer();
                }

                if(layer_state_is(layer)) {
                    print("layer off\n");
                    layer_off(layer);
                }
            } else {
                // turn on one shot
                print("turn on os\n");
                set_oneshot_layer(layer, ONESHOT_START);
                wait_ms(2);
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            }

            uprintf("reset 1 - oneshot layer %d\n", get_oneshot_layer());
            uprintf("reset 1 - layer active %d\n", layer_state_is(layer));

            

            
            isOneShot = true;
            break;
        }
        case TD_SINGLE_HOLD: layer_off(layer); break;
        // case TD_DOUBLE_TAP: unregister_code(KC_D); break;
        // case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: {break;}
    }
    xtap_state.state = TD_NONE;
}

#define layer_tapdance(td_layer) (layer = td_layer; return ACTION_TAP_DANCE_FN_ADVANCED(NULL, layer_finished, layer_reset))


#define LAYER_TAP_DANCE(td_layer) \
    { .fn = {NULL, layer_finished, layer_reset}, .user_data = (void *)&((td_layer_tap_t){td_layer}), }

qk_tap_dance_action_t tap_dance_actions[] = {
    [X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
    [CAPS_WDLK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
    [CPY_PST_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cpy_pst_finished, cpy_pst_reset),
    [DAILY_TD] = LAYER_TAP_DANCE(_DAILY),
    [NAV_TD] = LAYER_TAP_DANCE(_NAV),
    [SYMBOL_TD] = LAYER_TAP_DANCE(_SYM),
    [FUNCTION_TD] = LAYER_TAP_DANCE(_FUNCTION),
};

int getLayerFromTd(uint16_t keycode) {
    switch(keycode) {
        case TD(DAILY_TD):
            return _DAILY;
        case TD(NAV_TD):
            return _NAV;
        case TD(SYMBOL_TD):
            return _SYM;
        case TD(FUNCTION_TD):
            return _FUNCTION;
        default:
            return -1;
    }
}

int getTDIndexFromKeycode(uint16_t keycode) {
    switch(keycode) {
        case TD(DAILY_TD):
            return DAILY_TD;
        case TD(NAV_TD):
            return NAV_TD;
        case TD(SYMBOL_TD):
            return SYMBOL_TD;
        case TD(FUNCTION_TD):
            return FUNCTION_TD;
        default:
            return -1;
    }
}

#endif /* TAPDANCE */ 