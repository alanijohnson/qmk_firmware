#ifndef ENUMS 
#define ENUMS 

enum custom_keycodes {
    ELLIPSES = SAFE_RANGE,
    ARROW,
    CPY_PST,
e    USER,
    PASS,
};

// Tap dance enums
enum {
    X_CTL,
    CAPS_WDLK,
    CPY_PST_TD,
    DAILY_TD,
    NAV_TD,
    FUNCTION_TD,
    SYMBOL_TD,
    QUICK_ACCESS_TD,
    BRACES_TD
};

enum layers {
    _QWERTY = 0,
    // _DVORAK,
    // _COLEMAK_DH,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
    _DAILY,
    // _IDE
};

#endif /* ENUMS */ 


