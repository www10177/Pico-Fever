/*
 * Pico Game Controller
 * @author SpeedyPotato
 */
//Dependencies 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
// #include "encoders.pio.h"
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
#include "debounce/debounce_include.h"
// #include "ws2812/rgb_include.h"

//Config Headers
#include "general_config.h"

// PIO pio, pio_1;
// uint32_t enc_val[ENC_GPIO_SIZE];
// uint32_t prev_enc_val[ENC_GPIO_SIZE];
// int cur_enc_val[ENC_GPIO_SIZE];

bool prev_sw_val[SW_GPIO_SIZE];
uint64_t sw_timestamp[SW_GPIO_SIZE];

bool kbm_report;
RotaryInput rotary;

uint64_t reactive_timeout_timestamp;

// void (*ws2812b_mode)(uint32_t);
void (*loop_mode)();
uint16_t (*debounce_mode)();
bool joy_mode_check = true;
InputMode input_mode = INPUT_MODE_KEYBOARD;

// union {
//   struct {
//     uint8_t buttons[LED_GPIO_SIZE];
//     RGB_t rgb[WS2812B_LED_ZONES];
//   } lights;
//   uint8_t raw[LED_GPIO_SIZE + WS2812B_LED_ZONES * 3];
// } lights_report;


/**
 * WS2812B Lighting
 * @param counter Current number of WS2812B cycles
 **/
// void ws2812b_update(uint32_t counter) {
//   if (time_us_64() - reactive_timeout_timestamp >= REACTIVE_TIMEOUT_MAX) {
//     ws2812b_mode(counter);
//   } else {
//     for (int i = 0; i < WS2812B_LED_ZONES; i++) {
//       for (int j = 0; j < WS2812B_LEDS_PER_ZONE; j++) {
//         put_pixel(urgb_u32(lights_report.lights.rgb[i].r,
//                            lights_report.lights.rgb[i].g,
//                            lights_report.lights.rgb[i].b));
//       }
//     }
//   }
// }

/**
 * HID/Reactive Lights
 **/
// void update_lights() {
//   for (int i = 0; i < LED_GPIO_SIZE; i++) {
//     if (time_us_64() - reactive_timeout_timestamp >= REACTIVE_TIMEOUT_MAX) {
//       if (!gpio_get(SW_GPIO[i])) {
//         gpio_put(LED_GPIO[i], 1);
//       } else {
//         gpio_put(LED_GPIO[i], 0);
//       }
//     } else {
//       if (lights_report.lights.buttons[i] == 0) {
//         gpio_put(LED_GPIO[i], 0);
//       } else {
//         gpio_put(LED_GPIO[i], 1);
//       }
//     }
//   }
// }

struct report {
  uint16_t buttons;
  uint8_t joy0;
  uint8_t joy1;
} report;

// /**
//  * Gamepad Mode
//  **/
// void joy_mode() {
//   if (tud_hid_ready()) {
//     // find the delta between previous and current enc_val
//     for (int i = 0; i < ENC_GPIO_SIZE; i++) {
//       cur_enc_val[i] +=
//           ((ENC_REV[i] ? 1 : -1) * (enc_val[i] - prev_enc_val[i]));
//       while (cur_enc_val[i] < 0) cur_enc_val[i] = ENC_PULSE + cur_enc_val[i];
//       cur_enc_val[i] %= ENC_PULSE;

//       prev_enc_val[i] = enc_val[i];
//     }

//     report.joy0 = ((double)cur_enc_val[0] / ENC_PULSE) * (UINT8_MAX + 1);
//     if (ENC_GPIO_SIZE > 1) {
//       report.joy1 = ((double)cur_enc_val[1] / ENC_PULSE) * (UINT8_MAX + 1);
//     }

//     tud_hid_n_report(0x00, REPORT_ID_JOYSTICK, &report, sizeof(report));
//   }
// }

/**
 * Keyboard Mode
 **/
void key_mode() {
  if (tud_hid_ready()) {  // Wait for ready, updating mouse too fast hampers
                          // movement
    if (kbm_report) {
    // if (true) {
      /*------------- Keyboard -------------*/
      uint8_t nkro_report[32] = {0};
      for (int i = 0; i < SW_GPIO_SIZE; i++) {
        if ((report.buttons >> i) % 2 == 1) {
          uint8_t bit = SW_KEYCODE[i] % 8;
          uint8_t byte = (SW_KEYCODE[i] / 8) + 1;
          if (SW_KEYCODE[i] >= 240 && SW_KEYCODE[i] <= 247) {
            nkro_report[0] |= (1 << bit);
          } else if (byte > 0 && byte <= 31) {
            nkro_report[byte] |= (1 << bit);
          }
        }
      }
      tud_hid_n_report(0x00, REPORT_ID_KEYBOARD, &nkro_report,
                       sizeof(nkro_report));
    } else {

    rotary.process();
    // MOUSE: convenient helper to send mouse report if application
    // use template layout report as defined by hid_mouse_report_t
    //tud_hid_mouse_report(uint8_t report_id, uint8_t buttons, int8_t x, int8_t y, int8_t vertical, int8_t horizontal);
    tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, rotary.delta[0] * 1,
                         rotary.delta[1] * 1, 0, 0);
    }
    // Alternate reports
    kbm_report = !kbm_report;
  }
}

/**
 * Update Input States
 * Note: Switches are pull up, negate value
 **/
void update_inputs() {
  for (int i = 0; i < SW_GPIO_SIZE; i++) {
    // If switch gets pressed, record timestamp
    if (prev_sw_val[i] == false && !gpio_get(SW_GPIO[i]) == true) {
      sw_timestamp[i] = time_us_64();
    }
    prev_sw_val[i] = !gpio_get(SW_GPIO[i]);
  }
}


/**
 * Second Core Runnable
 **/
void core1_entry() {
  uint32_t counter = 0;
  while (1) {
    // ws2812b_update(++counter);
    sleep_ms(5);
  }
}

/**
 * Initialize Board Pins
 **/
void init() {
  // LED Pin on when connected
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 1);

  // Set up the state machine for encoders
//   pio = pio0;
//   uint offset = pio_add_program(pio, &encoders_program);


  reactive_timeout_timestamp = time_us_64();

  // Set up WS2812B
//   pio_1 = pio1;
//   uint offset2 = pio_add_program(pio_1, &ws2812_program);
//   for (int i = 0; i < sizeof(WS2812B_GPIO) / sizeof(WS2812B_GPIO[0]); i++) {
//     // BUGS: only one ws2812b can be used
//     gpio_set_dir(SW_GPIO[i], GPIO_IN);
//     gpio_pull_up(SW_GPIO[i]);
//     ws2812_program_init(pio_1, ENC_GPIO_SIZE+i, offset2, WS2812B_GPIO[i], 800000,
//                         false);
//   }


  // Setup LED GPIO
//   for (int i = 0; i < LED_GPIO_SIZE; i++) {
//     gpio_init(LED_GPIO[i]);
//     gpio_set_dir(LED_GPIO[i], GPIO_OUT);
//   }

  // Set listener bools
  kbm_report = false;

  // Joy/KB Mode Switching
    loop_mode = key_mode;
    joy_mode_check = false;
    input_mode = INPUT_MODE_KEYBOARD;

  // RGB Mode Switching
//   if (!gpio_get(SW_GPIO[1])) {
    // ws2812b_mode = &ws2812b_color_cycle;
//     ws2812b_mode = &turbocharger_color_cycle;
//   } else {
//     ws2812b_mode = &turbocharger_color_cycle;
//   }

  // Debouncing Mode
  debounce_mode = &debounce_eager;

  // Disable RGB
//   if (gpio_get(SW_GPIO[8])) {
//     multicore_launch_core1(core1_entry);
//   }
}
void hotkey(){
        if (!gpio_get(SW_GPIO[SW_GPIO_SIZE-1]) == true){
            //Reboot to Bootloader if last button is pressed
            // Buttons are pull up, enter when last button is pressed
            reset_usb_boot(0, 0);
        }
}

/**
 * Main Loop Function
 **/
int main(void) {
  board_init();
  init();
  hotkey();
  tusb_init();
  rotary.setup();
  


  while (1) {
    hotkey();
    tud_task();  // tinyusb device task
    update_inputs();
    report.buttons = debounce_mode();
    loop_mode();


    //update_lights();
  }

  return 0;
}
