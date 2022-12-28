#ifndef BUTTONS_H
#define BUTTONS_H
#include <stdint.h>

class Gamepad
{
public:
    Gamepad(int gpio_size, const uint8_t *sw_gpio, int debounce_us);
    ~Gamepad();

    // Methods
    void update_inputs();
    void debounce();
    uint32_t debounced_btn=0;

private:
    // Configurations Variables
    int gpio_size;
    uint8_t *sw_gpio;
    int debounce_us;

    // Class Members
    uint32_t button_pressed = 0; // Gamepad Pressed Now
    bool *prev_sw_val;
    uint64_t *sw_timestamp;
};
#endif // buttons.h