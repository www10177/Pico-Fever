#include "gamepad.h"
#include "pico/stdlib.h"
#include <iterator>

Gamepad::Gamepad(int gpio_size, const uint8_t *sw_gpio, int debounce_us)
{
    // Assign Variables
    this->gpio_size = gpio_size;
    this->sw_gpio = new uint8_t[gpio_size];
    std::copy(sw_gpio, sw_gpio + gpio_size, this->sw_gpio);

    // Allocate variables
    prev_sw_val = new bool[gpio_size]();
    sw_timestamp = new uint64_t[gpio_size]();

    // initialize variables
    for (int i = 0; i < gpio_size; i++)
    {
        prev_sw_val[i] = false;
        sw_timestamp[i] = 0;
    }
}

Gamepad::~Gamepad()
{
    delete sw_gpio, prev_sw_val, sw_timestamp;
}


void Gamepad::debounce()
{
    debounced_btn= 0;
    for (int i = gpio_size - 1; i >= 0; i--)
    {
        if (time_us_64() - sw_timestamp[i] <= debounce_us ||
            !gpio_get(sw_gpio[i]))
        {
            debounced_btn= (debounced_btn << 1) | 1;
        }
        else
        {
            debounced_btn <<= 1;
        }
    }
}

/**
 * Update Input States
 * Note: Switches are pull up, negate value
 **/
void Gamepad::update_inputs() {
  for (int i = 0; i < gpio_size; i++) {
    // If switch gets pressed, record timestamp
    if (prev_sw_val[i] == false && !gpio_get(sw_gpio[i]) == true) {
      sw_timestamp[i] = time_us_64();
    }
    prev_sw_val[i] = !gpio_get(sw_gpio[i]);
  }
}