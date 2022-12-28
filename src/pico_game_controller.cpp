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
#include "rotary-encoder/rotaryencoder.h"
#include "gamepad/gamepad.h"
// #include "ws2812/rgb_include.h"

//Config Headers
#include "general_config.h"


// Global Variables 
bool has_sent_mouse_report = false; // use for rotary encoder mouse report
RotaryInput rotary(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
Gamepad gamepad(SW_GPIO_SIZE, SW_GPIO, SW_DEBOUNCE_TIME_US);
void (*generate_report)(); // function pointer for usb report generation
InputMode input_mode = INPUT_MODE_KEYBOARD;

uint64_t reactive_timeout_timestamp;


/**
 * Keyboard Mode
 **/
void key_mouse_input()
{
  if (tud_hid_ready()) { 
      rotary.update();
      if (rotary.delta[0] + rotary.delta[1] >0 && !has_sent_mouse_report){
        // send mouse report when rotary encoder is moved and not sent mouse report last round 
          tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, rotary.delta[0] * MOUSE_SENS,
                               rotary.delta[1] * MOUSE_SENS, 0, 0);
            has_sent_mouse_report = true;

      }
      else {
        // Sent keyboard report 
          uint8_t nkro_report[32] = {0};
          gamepad.debounce();
          uint32_t  buttons = gamepad.debounced_btn;
          for (int i = 0; i < SW_GPIO_SIZE; i++)
          {
              if ((buttons >> i) % 2 == 1)
              {
                  uint8_t bit = SW_KEYCODE[i] % 8;
                  uint8_t byte = (SW_KEYCODE[i] / 8) + 1;
                  if (SW_KEYCODE[i] >= 240 && SW_KEYCODE[i] <= 247)
                  {
                      nkro_report[0] |= (1 << bit);
                  }
                  else if (byte > 0 && byte <= 31)
                  {
                      nkro_report[byte] |= (1 << bit);
                  }
              }
          }
            has_sent_mouse_report = false;
          tud_hid_n_report(0x00, REPORT_ID_KEYBOARD, &nkro_report,
                           sizeof(nkro_report));
      }
  }
}

/**
 * Initialize Board Pins
 **/
void init() {
  // Light on onboard LED when connected
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 1);

  reactive_timeout_timestamp = time_us_64();


// Change Loop mode to correspond to input mode
  if (input_mode == INPUT_MODE_KEYBOARD){
    generate_report= key_mouse_input;
  }
}

void startup_hotkeys() {
  if (!gpio_get(SW_GPIO[SW_GPIO_SIZE - 1]) == true)
  {
    // Reboot to Bootloader if last button is pressed
    //  Buttons are pull up, enter when last button is pressed
    reset_usb_boot(0, 0);
  }
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
    startup_hotkeys();
    gamepad.update_inputs();
    gamepad.debounce();
    generate_report();
    tud_task();  // tinyusb device task


    //update_lights();
  }

  return 0;
}
