/*
 * Pico Game Controller
 * Author : Rist
 * Based on code of SpeedyPotato
 * 
 */
//Dependencies 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hid/hid.h"
#include "tusb.h"

//Local Libraries
// #include "rotary-encoder/rotaryencoder.h"
#include "gamepad/gamepad.h"
// #include "ws2812/rgb_include.h"

//Config Headers
#include "general_config.h"


// Global Variables 
bool has_sent_mouse_report = false; // use for rotary encoder mouse report
Gamepad gamepad;//SW_GPIO_SIZE, SW_GPIO, SW_DEBOUNCE_TIME_US);
// void (*generate_report)(); // function pointer for usb report generation
InputMode input_mode = INPUT_MODE_KEYBOARD;

// uint64_t reactive_timeout_timestamp;


/**
 * Initialize Board Pins
 **/

void init() {
  // Light on onboard LED when connected
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 1);

//   reactive_timeout_timestamp = time_us_64();


}

void startup_hotkeys() {
    //  Buttons are pull up, enter when last button is pressed
  if (!gpio_get(BOOTSEL_STARTUP_GPIO) == true) reset_usb_boot(0, 0);
  else if (!gpio_get(SWITCH_STARTUP_GPIO) == true) input_mode = INPUT_MODE_SWITCH;
  else if (!gpio_get(XINPUT_STARTUP_GPIO) == true) input_mode = INPUT_MODE_XINPUT;
  else if (!gpio_get(KEYBOARD_STARTUP_GPIO) == true) input_mode = INPUT_MODE_KEYBOARD;
}

/**
 * Main Loop Function
 **/
int main(void) {
  board_init();
  init();
  startup_hotkeys();
  tusb_init();
  


  while (1) {
    // startup_hotkeys();
    gamepad.update_inputs();
    gamepad.debounce();
    gamepad.send_report();
    tud_task();  // tinyusb device task


    //update_lights();
  }

  return 0;
}
