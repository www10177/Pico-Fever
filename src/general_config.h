#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H
#include "tusb.h" // include for hid key
#include "button.h" // include for joypad (NS,XBOX) buttons



// Buttons Confuguration
#define SW_GPIO_SIZE 16               // Number of switches
#define SW_DEBOUNCE_TIME_US 4000      // Switch debounce delay in us
const uint8_t SW_GPIO[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0
};

// Keycodes Binding 
// MODIFY KEYBINDS HERE, MAKE SURE LENGTHS MATCH SW_GPIO_SIZE
const uint8_t KEYCODE[] = {HID_KEY_D, HID_KEY_R, HID_KEY_F, HID_KEY_G,
                              HID_KEY_J, HID_KEY_I, HID_KEY_K, 
                              HID_KEY_ARROW_UP, HID_KEY_ARROW_DOWN, 
                              HID_KEY_Z, HID_KEY_SHIFT_LEFT, HID_KEY_SHIFT_RIGHT, HID_KEY_SLASH,
                              HID_KEY_ARROW_LEFT,HID_KEY_ARROW_RIGHT,
                              HID_KEY_ESCAPE};

// Switch Button Binding
const uint8_t SWITCH_BUTTON[] = {SWITCH_LEFT,SWITCH_UP,SWITCH_RIGHT,SWITCH_B,
                                 SWITCH_Y, SWITCH_X, SWITCH_A,
                                 SWITCH_L3,SWITCH_R3,
                                 SWITCH_ZL,SWITCH_L,SWITCH_R,SWITCH_ZR,
                                 SWITCH_MINUS,SWITCH_PLUS,
                                 SWITCH_HOME};

//Start Hotkey 
#define BOOTSEL_STARTUP_GPIO  0 // GPIO pin for startup hotkey in bootloader more
#define KEYBOARD_STARTUP_GPIO  2 // GPIO pin for startup hotkey in keyboard mode
#define SWITCH_STARTUP_GPIO  4 // GPIO pin for startup hotkey in keyboard mode
#define XINPUT_STARTUP_GPIO  6 // GPIO pin for startup hotkey in keyboard mode

// LED GPIOs                              
// #define LED_GPIO_SIZE 0              // Number of switch LEDs
// const uint8_t LED_GPIO[] = {
// };

// Rotary Encoder 
#define ENC_GPIO_SIZE 2               // Number of encoders
const uint8_t ENC_GPIO[] = {18,20};      // L_ENC(0, 1); R_ENC(2, 3)
const bool ENC_REV = false;  // Reverse Encoders
#define ENC_DEBOUNCE_COUNT 100 // Encoder debounce
#define MOUSE_SENS 1                  // Mouse sensitivity multiplier
// #define ENC_PULSE (ENC_PPR * 4)       // 4 pulses per PPR

// WS2812
#define WS2812B_LED_SIZE 16           // Number of WS2812B LEDs
#define WS2812B_LED_ZONES 1           // Number of WS2812B LED Zones
#define WS2812B_LEDS_PER_ZONE \
  WS2812B_LED_SIZE / WS2812B_LED_ZONES  // Number of LEDs per zone
// #define REACTIVE_TIMEOUT_MAX 1000000  // HID to reactive timeout in us
const uint8_t WS2812B_GPIO[] = {26,27}; // WS2812B Data Pin 
const uint8_t WS2812B_GPIO_COUNT= 2; // COUNT of CONSECUTIVE WS2812B DATA PINS



#endif