#ifndef BUTTONS_H
#define BUTTONS_H

// Make sure to use corresponding button define file for your input mode
// enum value matters! used for report generating

#define XINPUT_BUTTON2_BASE 8
typedef enum
{
    // Report Button 1
    XINPUT_NONE = -1,
    XINPUT_UP = 0,
    XINPUT_DOWN = 1,
    XINPUT_LEFT = 2,
    XINPUT_RIGHT = 3,
    XINPUT_START = 4,
    XINPUT_BACK = 5,
    XINPUT_LS = 6,
    XINPUT_RS = 7,

    // Report Button 2
    XINPUT_LB = 8,
    XINPUT_RB = 9,
    XINPUT_HOME = 10,
    XINPUT_A = 12,
    XINPUT_B = 13,
    XINPUT_X = 14,
    XINPUT_Y = 15,
    // Trigger
    XINPUT_LT = 20,
    XINPUT_RT,
} XinputButton;

// #define SWITCH_BUTTON_BASE 4
typedef enum
{
    SWITCH_NONE = -1,
    SWITCH_UP = 0,
    SWITCH_RIGHT = 1,
    SWITCH_DOWN = 2,
    SWITCH_LEFT = 3,

    // Switch Buttons
    SWITCH_Y = 4,
    SWITCH_B = 5,
    SWITCH_A = 6,
    SWITCH_X = 7,
    SWITCH_L = 8,
    SWITCH_R = 9,
    SWITCH_ZL = 10,
    SWITCH_ZR = 11,
    SWITCH_MINUS = 12,
    SWITCH_PLUS = 13,
    SWITCH_L3 = 14,
    SWITCH_R3 = 15,
    SWITCH_HOME = 16,
    SWITCH_CAPTURE = 17,
} SwitchButton;

// Special keys that only used in firmware
// Start from 0xF0 and ends with 0xFF to avoid collision of hid key code
// FN_BOOTSEL(0xF0) and FN_NEXT_PROFILE(0xFF) are used in key checking
// so make sure to put them in the start and end of the enum if you want to add more function keys
typedef enum
{
    FN_BOOTSEL=0xF0,
    FN_TRANSPARENT, // use key of base layer
    FN_APPEND_LAYER, // momentarily switch to append layer while the key is pressed
    // FN_LAYER_SWITCH, // switch to append layer 
    FN_PREV_PROFILE,
    FN_NEXT_PROFILE=0xFF, 
} FunctionKeys;


#endif