/*
 * Pico Game Controller
 * Author : Rist
 * Based on code of SpeedyPotato
 * 
 */
//Dependencies 
    #include <string>
    #include <iostream>

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

#include "hardware/i2c.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "pico-ssd1306/shapeRenderer/ShapeRenderer.h"

//Local Libraries
// #include "rotary-encoder/rotaryencoder.h"
#include "gamepad/gamepad.h"
// #include "ws2812/rgb_include.h"

//Config Headers
#include "pico-ssd1306/ssd1306.h"
#include "general_config.h"
#include "Profiles.h"

// Global Variables 
bool has_sent_mouse_report = false; // use for rotary encoder mouse report
InputMode input_mode = INPUT_MODE_SWITCH;
Gamepad* gamepad = new Gamepad(SwitchProfiles, SWITCH_PROFILE_COUNT); // Make sure the initial profile is matching to initial input mode
// void (*generate_report)(); // function pointer for usb report generation
pico_ssd1306::SSD1306 *display ;
// InputMode input_mode = INPUT_MODE_KEYBOARD;

// uint64_t reactive_timeout_timestamp;


/**
 * Initialize Board Pins
 **/

void init()
{
    // Light on onboard LED when connected
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    // Initialize i2c for ssd1306 display
    if (I2C_DISPLAY_ENABLED ) {
        i2c_init(i2c0, I2C_DISPLAY_RATE); // Init i2c0 and set baud rate to 1Mhz (max supported speed by pico)
        // lower baud rates make the communication slower, and since this lib uses blocking writing,
        // lower baud rates lower maximal achievable frame rate
        gpio_set_function(I2C_DISPLAY_SDA, GPIO_FUNC_I2C);
        gpio_set_function(I2C_DISPLAY_SCL, GPIO_FUNC_I2C); 
        gpio_pull_up(I2C_DISPLAY_SCL); // Pull up the pins for proper communication
        gpio_pull_up(I2C_DISPLAY_SDA); // Pull up the pins for proper communication

        display = new pico_ssd1306::SSD1306(i2c0, I2C_DISPLAY_ADDRESS, pico_ssd1306::Size::W128xH64);
        display->setOrientation(I2C_DISPLAY_ORIENTATION);
    }

    //   reactive_timeout_timestamp = time_us_64();
}

void startup_hotkeys() {
    //  Buttons are pull up, enter when last button is pressed
  if (!gpio_get(BOOTSEL_STARTUP_GPIO) == true) reset_usb_boot(0, 0);
  else if (!gpio_get(SWITCH_STARTUP_GPIO) == true) input_mode = INPUT_MODE_SWITCH;
  else if (!gpio_get(XINPUT_STARTUP_GPIO) == true) input_mode = INPUT_MODE_XINPUT;
  else if (!gpio_get(KEYBOARD_STARTUP_GPIO) == true) input_mode = INPUT_MODE_KEYBOARD;
}
void update_display(Gamepad* gamepad) {
  // Only update display when gamepad profile is updated
  if (gamepad->isProfileUpdated){ 
    display->clear();

    // Start Drawing
    // First 16 rows are yellow (of my oled module)
    // ssd1306 is oled screen with pixel size = 128x64
    std::string title = "Pico Fever | ";
    switch (input_mode) {
        case INPUT_MODE_KEYBOARD: title += "KB"; break;
        case INPUT_MODE_SWITCH: title  += "NS"; break;
        default: title += "DE"; break;
    }
    drawText(display, font_8x8, title.c_str(), 0, 4); // Title 
    int context_y0 = 18;
    drawText(display, font_8x8, gamepad->profileNow->name, 0, context_y0);
    int number= gamepad->isBaseLayer; // add profile count to make sure it is positive
    // int number= sizeof(gamepad->profiles); // add profile count to make sure it is positive
    drawText(display, font_8x8, std::to_string(number).c_str(), 0, context_y0 + 10);
    // number= sizeof(gamepad->profiles[0]); // add profile count to make sure it is positive
    // drawText(display, font_8x8, std::to_string(number).c_str(), 0, context_y0 + 20);

    display->sendBuffer();
    gamepad->isProfileUpdated = false;
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
    // if (I2C_DISPLAY_ENABLED ) update_display(gamepad);
    while (true)
    {
        if (!gpio_get(BOOTSEL_STARTUP_GPIO) == true) reset_usb_boot(0, 0);
        gamepad->update_inputs();
        gamepad->debounce();
        gamepad->send_report();
        if (I2C_DISPLAY_ENABLED ) update_display(gamepad);
        tud_task(); // tinyusb device task

        // update_lights();
    }

  return 0;
}
