#ifndef PROFILES_H
#define PROFILES_H
#include "general_config.h"
#include "../gamepad/buttons.h"
#include "tusb.h" // include for hid key


// Make sure there are at least one profile for each mode
#define SWITCH_PROFILE_COUNT 2
#define KB_PROFILE_COUNT 1

//Remember to setup FN_NEXT_PROFILE and FN_PREV_PROFILE  if you have more than one profile
//Make Sure FN_APPEND_LAYER is set as same key in both layer if you want to use it
Profile SwitchProfiles[SWITCH_PROFILE_COUNT] = {
    {// Profile 1
     .name= "NS Profile1",
     .base_layer_btn= {FN_PREV_PROFILE, FN_NEXT_PROFILE, FN_APPEND_LAYER, SWITCH_X,
                           SWITCH_Y, SWITCH_A, SWITCH_B,
                           SWITCH_UP, SWITCH_DOWN,
                           SWITCH_ZL, SWITCH_L, SWITCH_R, SWITCH_ZR,
                           SWITCH_MINUS, SWITCH_PLUS,
                           FN_BOOTSEL},
     .append_layer_btn= {SWITCH_LEFT, SWITCH_RIGHT,FN_APPEND_LAYER, SWITCH_LEFT,
                             SWITCH_RIGHT, FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_TRANSPARENT}
    },
    { // Profile 2
    .name= "NS OuO2",
    .base_layer_btn= {FN_PREV_PROFILE, FN_NEXT_PROFILE, SWITCH_LEFT, SWITCH_X,
                        SWITCH_Y, SWITCH_A, SWITCH_B,
                        SWITCH_UP, SWITCH_DOWN,
                        SWITCH_ZL, SWITCH_L, SWITCH_R, SWITCH_ZR,
                        SWITCH_MINUS, SWITCH_PLUS,
                        FN_BOOTSEL},
    .append_layer_btn= {FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT,
                            FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT,
                            FN_TRANSPARENT, FN_TRANSPARENT,
                            FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT,
                            FN_TRANSPARENT, FN_TRANSPARENT,
                            FN_TRANSPARENT}
    },
};
Profile KBProfiles[KB_PROFILE_COUNT] = {
    {// Profile 1
     .name= "DJMAX",
     .helperText = "test"
     .base_layer_btn= {HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_ENTER,
                              HID_KEY_J, HID_KEY_K, HID_KEY_L, 
                              HID_KEY_ARROW_UP, HID_KEY_ARROW_DOWN, 
                              HID_KEY_SHIFT_LEFT, HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_SHIFT_RIGHT,
                              HID_KEY_ARROW_LEFT,HID_KEY_ARROW_RIGHT,
                              FN_APPEND_LAYER},
     .append_layer_btn= {FN_TRANSPARENT, HID_KEY_TAB, FN_TRANSPARENT, HID_KEY_ESCAPE,
                             FN_TRANSPARENT, HID_KEY_F9, FN_TRANSPARENT,
                             FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT, FN_TRANSPARENT,
                             FN_BOOTSEL,FN_BOOTSEL,
                             FN_APPEND_LAYER},
      .rotary_mode = {KB_MOUSE_X, KB_MOUSE_Y}
    },
};


#endif