#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdint.h>
#include "general_config.h"
#include "rotaryencoder.h"

class Gamepad
{
public:
    // static RotaryInput rotary;//(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
    RotaryInput* rotary;//(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 

    uint32_t debounced_btn=0;
    uint8_t button_mapping[SW_GPIO_SIZE];
    uint16_t LX=0;
    uint16_t LY=0;
    uint16_t RX=0;
    uint16_t RY=0;

    Gamepad();//int gpio_size, const uint8_t *sw_gpio, int debounce_us);
    ~Gamepad();

    // Methods
    void update_inputs();
    void debounce();
    void send_report();


private:
    // Configurations Variables
    int gpio_size;
    uint8_t *sw_gpio;
    int debounce_us;

    // Class Members
    // static RotaryInput rotary(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
    uint32_t button_pressed = 0; // Gamepad Pressed Now
    bool *prev_sw_val;
    uint64_t *sw_timestamp;
    bool has_sent_mouse_report = false; // only used in keyboard mode

    //Private Methods
    void __send_joypad_report(void *report, uint16_t report_size) ;
    void send_switch_report();
    void send_keyboard_report();
};
#endif // buttons.h