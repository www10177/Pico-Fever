#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H

#define SW_GPIO_SIZE 16               // Number of switches
#define LED_GPIO_SIZE 0              // Number of switch LEDs
#define ENC_GPIO_SIZE 2               // Number of encoders
#define ENC_PPR 600                   // Encoder PPR
#define MOUSE_SENS 1                  // Mouse sensitivity multiplier
#define ENC_DEBOUNCE true // Encoder Debouncing
#define SW_DEBOUNCE_TIME_US 4000      // Switch debounce delay in us
#define ENC_PULSE (ENC_PPR * 4)       // 4 pulses per PPR
#define REACTIVE_TIMEOUT_MAX 1000000  // HID to reactive timeout in us
#define WS2812B_LED_SIZE 16           // Number of WS2812B LEDs
#define WS2812B_LED_ZONES 1           // Number of WS2812B LED Zones
#define WS2812B_LEDS_PER_ZONE \
  WS2812B_LED_SIZE / WS2812B_LED_ZONES  // Number of LEDs per zone


// MODIFY KEYBINDS HERE, MAKE SURE LENGTHS MATCH SW_GPIO_SIZE

const uint8_t SW_KEYCODE[] = {HID_KEY_D, HID_KEY_F, HID_KEY_J, HID_KEY_K,
                              HID_KEY_C, HID_KEY_M, HID_KEY_A, HID_KEY_B,
                              HID_KEY_1, HID_KEY_E, HID_KEY_G, HID_KEY_H,
                              HID_KEY_2,HID_KEY_3,HID_KEY_4,HID_KEY_5};
const uint8_t SW_GPIO[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0
};
const uint8_t LED_GPIO[] = {
};
const uint8_t ENC_GPIO[] = {18,20};      // L_ENC(0, 1); R_ENC(2, 3)
const bool ENC_REV[] = {false,false};  // Reverse Encoders
const uint8_t WS2812B_GPIO[] = {26,27}; // WS2812B Data Pin 
const uint8_t WS2812B_GPIO_COUNT= 2; // COUNT of CONSECUTIVE WS2812B DATA PINS


extern bool joy_mode_check;

#endif