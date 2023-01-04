#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H
#include "../gamepad/buttons.h" // for RotaryMode enum in profile declaration



// Buttons Configuration
#define BTN_GPIO_SIZE 16               // Number of switches
#define BTN_DEBOUNCE_TIME_US 4000      // Switch debounce delay in us
const uint8_t BTN_GPIO[BTN_GPIO_SIZE] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0
};


//Startup Hotkey 
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
const uint8_t ENC_GPIO[ENC_GPIO_SIZE] = {18,20};      // L_ENC(0, 1); R_ENC(2, 3)
const bool ENC_REV = false;  // Reverse Encoders
#define ENC_DEBOUNCE_COUNT 100 // Encoder debounce
#define MOUSE_SENS 1                  // Mouse sensitivity multiplier
// #define ENC_PULSE (ENC_PPR * 4)       // 4 pulses per PPR

// WS2812
// #define WS2812B_LED_SIZE 16           // Number of WS2812B LEDs
// #define WS2812B_LED_ZONES 1           // Number of WS2812B LED Zones
// #define WS2812B_LEDS_PER_ZONE \
//   WS2812B_LED_SIZE / WS2812B_LED_ZONES  // Number of LEDs per zone
// // #define REACTIVE_TIMEOUT_MAX 1000000  // HID to reactive timeout in us
// const uint8_t WS2812B_GPIO[] = {26,27}; // WS2812B Data Pin 
// const uint8_t WS2812B_GPIO_COUNT= 2; // COUNT of CONSECUTIVE WS2812B DATA PINS

// I2C OLED  Display Related (Using SSD1306 Controller)
#define I2C_DISPLAY_ENABLED true     // Enable I2C Display
#define I2C_DISPLAY_ORIENTATION 0         // 0 or 1; Change this if display is upside down
#define I2C_DISPLAY_ADDRESS 0x3C      // I2C Address of the display
#define I2C_DISPLAY_SDA 16 // SDA Pin
#define I2C_DISPLAY_SCL 17 // SCL Pin
#define I2C_DISPLAY_RATE 800000 // I2C Rate
#define I2C_DISPLAY_ADDRESS 0x3C // I2C Address of the display


typedef struct {
    const char* name="Default Profile";
    uint8_t base_layer_btn[BTN_GPIO_SIZE]; // keycodes for base layer
    uint8_t append_layer_btn[BTN_GPIO_SIZE]; // keycodes for append layer, switch to append layer by pressing user defined button
    const char* helperText="None";
    RotaryMode rotary_mode[ENC_GPIO_SIZE]; // rotary mode for each encoder
}Profile;

#endif