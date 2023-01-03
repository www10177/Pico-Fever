#include "gamepad.h"
#include "pico/stdlib.h"
#include <iterator>
#include "hid.h"
#include "tusb.h"
#include "device/usbd_pvt.h"
#include "general_config.h"
#include "rotaryencoder.h"
// #include "SwitchDescriptors.h"
// #include "XInputDescriptors.h"
#include "report.h"
#include "buttons.h"

extern InputMode input_mode;
// RotaryInput rotary = RotaryInput::RotaryInput(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
static SwitchReport switchReport {
    .buttons = 0,
    .hat = SWITCH_HAT_NOTHING,
    .lx = SWITCH_JOYSTICK_MID,
    .ly = SWITCH_JOYSTICK_MID,
    .rx = SWITCH_JOYSTICK_MID,
    .ry = SWITCH_JOYSTICK_MID,
    .vendor = 0,
};
static uint8_t keyboardReport[32] = {0};

// XBOX
uint8_t endpoint_in = 0;
uint8_t endpoint_out = 0;
#define XINPUT_OUT_SIZE 32
uint8_t xinput_out_buffer[XINPUT_OUT_SIZE] = { };

Gamepad::Gamepad()//int gpio_size, const uint8_t *sw_gpio, int debounce_us)
{
    // Assign Variables
    this->gpio_size = SW_GPIO_SIZE;
    this->sw_gpio = new uint8_t[gpio_size];
    std::copy(SW_GPIO, SW_GPIO+ gpio_size, this->sw_gpio);
    this->debounce_us = SW_DEBOUNCE_TIME_US;
    std::copy(SWITCH_BUTTON,SWITCH_BUTTON+gpio_size, this->button_mapping);

    // Allocate variables
    prev_sw_val = new bool[gpio_size]();
    sw_timestamp = new uint64_t[gpio_size]();

    // initialize variables
    for (int i = 0; i < gpio_size; i++)
    {
        prev_sw_val[i] = false;
        sw_timestamp[i] = 0;
    }
    rotary = new RotaryInput();//ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT); 
}

Gamepad::~Gamepad()
{
    delete sw_gpio, prev_sw_val, sw_timestamp, rotary;
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

void Gamepad::send_keyboard_report()
{
      if (rotary->delta[0] + rotary->delta[1] >0 && !has_sent_mouse_report){
        // send mouse report when rotary encoder is moved and not sent mouse report last round 
          tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, rotary->delta[0] * MOUSE_SENS,
                               rotary->delta[1] * MOUSE_SENS, 0, 0);
            has_sent_mouse_report = true;
      }
      else {
        // Sent keyboard report 
        std::fill(std::begin(keyboardReport), std::end(keyboardReport), 0);
        for (int i = 0; i < gpio_size; i++)
        {
            if ((debounced_btn>> i) % 2 == 1)
            {
                uint8_t bit = KEYCODE[i] % 8;
                uint8_t byte = (KEYCODE[i] / 8) + 1;
                if (KEYCODE[i] >= 240 && KEYCODE[i] <= 247)
                {
                    keyboardReport[0] |= (1 << bit);
                }
                else if (byte > 0 && byte <= 31)
                {
                    keyboardReport[byte] |= (1 << bit);
                }
            }
        }

        has_sent_mouse_report = false;
        tud_hid_n_report(0x00, REPORT_ID_KEYBOARD, &keyboardReport,
                        sizeof(keyboardReport));
      }
}

void Gamepad::send_switch_report(){
    uint8_t hat_status = 0;
    switchReport.lx = SWITCH_JOYSTICK_MID;
    switchReport.ly = SWITCH_JOYSTICK_MID;
    switchReport.rx = SWITCH_JOYSTICK_MID;
    switchReport.ry = SWITCH_JOYSTICK_MID;

    switchReport.buttons = 0;

    for (int i = 0; i < gpio_size; i++)
    {
        if ((debounced_btn >> i) % 2 == 1)
        {   
            if (button_mapping[i] == SWITCH_NONE) continue;
            else if (button_mapping[i] <= SWITCH_LEFT ){
                // Hat Button
                // Save Hat Status First, Convert to report later
                // Shift 0~3 bit for up down left right

                hat_status |= (1U << button_mapping[i]);
            }
            else {
                // Button
                // Direct convert to report, shifting number as defined enum 

                switchReport.buttons |= (1U << (button_mapping[i] -SWITCH_BUTTON_BASE));
            }

            // switchReport.hat = SWITCH_HAT_DOWN;
            // switchReport.buttons= SWITCH_MASK_A;
        }
    }


    switch (hat_status) {
        case (1U << SWITCH_UP):                             switchReport.hat = SWITCH_HAT_UP; break;
        case (1U << SWITCH_UP) | (1U << SWITCH_RIGHT):      switchReport.hat = SWITCH_HAT_UPRIGHT; break;
        case (1U << SWITCH_RIGHT):                          switchReport.hat = SWITCH_HAT_RIGHT; break;
        case (1U << SWITCH_DOWN) | (1U << SWITCH_RIGHT):    switchReport.hat = SWITCH_HAT_DOWNRIGHT; break;
        case (1U << SWITCH_DOWN):                           switchReport.hat = SWITCH_HAT_DOWN; break;
        case (1U << SWITCH_DOWN) | (1U << SWITCH_LEFT):     switchReport.hat = SWITCH_HAT_DOWNLEFT; break;
        case (1U << SWITCH_LEFT):                           switchReport.hat = SWITCH_HAT_LEFT; break;
        case (1U << SWITCH_UP) | (1U << SWITCH_LEFT):       switchReport.hat = SWITCH_HAT_UPLEFT; break;
        default:                                            switchReport.hat = SWITCH_HAT_NOTHING; break;
    }
    this->__send_joypad_report(&switchReport, sizeof(switchReport));
}

void Gamepad::__send_joypad_report(void *report, uint16_t report_size) {
	static uint8_t previous_report[CFG_TUD_ENDPOINT0_SIZE] = { };

	if (tud_suspended())
		tud_remote_wakeup();

	if (memcmp(previous_report, report, report_size) != 0)
	{
		bool sent = false;
		switch (input_mode) {
            case INPUT_MODE_XINPUT:

                if ( tud_ready() &&                                          // Is the device ready?
                    (endpoint_in != 0) && (!usbd_edpt_busy(0, endpoint_in)) // Is the IN endpoint available?
                )
                {
                    usbd_edpt_claim(0, endpoint_in);                                // Take control of IN endpoint
                    usbd_edpt_xfer(0, endpoint_in, (uint8_t *)report, report_size); // Send report buffer
                    usbd_edpt_release(0, endpoint_in);                              // Release control of IN endpoint
                    sent = true;
                }
                break;

            default:
                if (tud_hid_ready())
                    sent = tud_hid_report(0 , report, report_size);// 0 for reportid 
                else sent = false;
                break;
		}

		if (sent)
			memcpy(previous_report, report, report_size);
    }
}
void Gamepad::send_report(){
  if (tud_hid_ready()) { 
      rotary->update();
      switch (input_mode)
      {
        case INPUT_MODE_KEYBOARD:
            this->send_keyboard_report();
            break;
        case INPUT_MODE_SWITCH:
            this->send_switch_report();
            break;

      }
  }

}

