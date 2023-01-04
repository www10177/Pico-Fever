#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <stdint.h>
#include "general_config.h"
#include "rotaryencoder.h"

class Gamepad
{
public:
    // static RotaryInput rotary;//(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
    RotaryInput* rotary;//(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
    Profile* profileNow;

    uint16_t debounced_btn=0;
    bool isBaseLayer = true;
    uint16_t LX=0;
    uint16_t LY=0;
    uint16_t RX=0;
    uint16_t RY=0;
    bool isProfileUpdated = true;
    int profile_index= 0;

    Gamepad(Profile profiles[], int profile_count);//int gpio_size, const uint8_t *sw_gpio, int debounce_us);
    ~Gamepad();

    // Methods
    void update_inputs();
    void debounce();
    void send_report();
    void change_profile(int new_index);


// private:

    // Class Members
    // static RotaryInput rotary(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
    // uint32_t button_pressed = 0; // Gamepad Pressed Now
    bool prev_sw_val[BTN_GPIO_SIZE];
    uint64_t sw_timestamp[BTN_GPIO_SIZE];
    bool has_sent_mouse_report = false; // only used in keyboard mode
    Profile* profiles;
    int profile_count;

    //Private Methods
    void __send_joypad_report(void *report, uint16_t report_size) ;
    void send_switch_report();
    void send_keyboard_report();
    void handle_func_btn(uint8_t btn); // Handle with Function button 

};
#endif 