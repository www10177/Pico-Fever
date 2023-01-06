#include "gamepad.h"
#include "pico/stdlib.h"
#include <iterator>
#include "hid.h"
#include "tusb.h"
#include "device/usbd_pvt.h"
// #include "config_include.h"
#include "general_config.h"
// #include "Profiles.h"
// #include "KeyboardProfiles.h"
#include "rotaryencoder.h"
// #include "SwitchDescriptors.h"
// #include "XInputDescriptors.h"
#include "report.h"
#include "buttons.h"
#include "pico/bootrom.h"

extern InputMode input_mode;
// RotaryInput rotary = RotaryInput::RotaryInput(ENC_GPIO_SIZE, ENC_GPIO, ENC_REV, ENC_DBOUNCE_COUNT);
static SwitchReport switchReport{
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
uint8_t xinput_out_buffer[XINPUT_OUT_SIZE] = {};

Gamepad::Gamepad(Profile profiles[], int profile_count)
{

    // Assing profile
    this->profiles = profiles;
    this->profile_count = profile_count;
    // this-> profile_count= sizeof(profiles) / sizeof(profiles[0]); // cannot calculate arr size from pointer
    this->profile_index = 0;
    this->profileNow = &this->profiles[profile_index];

    // initialize variables
    for (int i = 0; i < BTN_GPIO_SIZE; i++)
    {
        prev_sw_val[i] = false;
        sw_timestamp[i] = 0;
    }

    rotary = new RotaryInput();
}

Gamepad::~Gamepad()
{
    delete rotary;
}

void Gamepad::debounce()
{
    debounced_btn = 0;
    for (int i = BTN_GPIO_SIZE - 1; i >= 0; i--)
    {
        if (time_us_64() - sw_timestamp[i] <= BTN_DEBOUNCE_TIME_US ||
            !gpio_get(BTN_GPIO[i]))
        {
            debounced_btn = (debounced_btn << 1) | 1;
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
void Gamepad::update_inputs()
{
    for (int i = 0; i < BTN_GPIO_SIZE; i++)
    {
        // If switch gets pressed, record timestamp
        if (prev_sw_val[i] == false && !gpio_get(BTN_GPIO[i]) == true)
        {
            sw_timestamp[i] = time_us_64();
        }
        prev_sw_val[i] = !gpio_get(BTN_GPIO[i]);
    }
}

void Gamepad::send_keyboard_report()
{
    // Initialize
    int mouse_x =0, mouse_y = 0;
    std::fill(std::begin(keyboardReport), std::end(keyboardReport), 0); // cleanup keyboard report
    uint8_t *button_mapping = isBaseLayer ? profileNow->base_layer_btn : profileNow->append_layer_btn;
    uint8_t arrow_triggered[4] = {HID_KEY_NONE, HID_KEY_NONE,HID_KEY_NONE, HID_KEY_NONE}; // up, down, left, right; used for rotary UDLR, saved kb arrow keycode if triggered


    // Get Rotary Data
    for (int i = 0; i < ENC_GPIO_SIZE; i++) {
        // send mouse report when rotary encoder is moved and not sent mouse report last round
        switch(this->profileNow->rotary_mode[i]){
            case KB_MOUSE_X:
                mouse_x += rotary->delta[i]*MOUSE_SENS;
                break;
            case KB_MOUSE_Y:
                mouse_y += rotary->delta[i]*MOUSE_SENS;
                break;
            case KB_ARROW_UD:
                arrow_triggered[0] |= (rotary->dir_debounced[i] == 0?HID_KEY_ARROW_UP:HID_KEY_NONE);//CCW-spin
                arrow_triggered[1] |= (rotary->dir_debounced[i] == 2?HID_KEY_ARROW_DOWN:HID_KEY_NONE);//CW-spin
                break;
            case KB_ARROW_LR:
                arrow_triggered[2] |= (rotary->dir_debounced[i] == 0?HID_KEY_ARROW_LEFT:HID_KEY_NONE);//CCW-spin
                arrow_triggered[3] |= (rotary->dir_debounced[i] == 2?HID_KEY_ARROW_RIGHT:HID_KEY_NONE);//CW-spin
                break;
        }
    }

    //Rotary: add triggered arrow keycode to keyboard report
    for (int i = 0; i < 4; i++) {
        if (arrow_triggered[i] != HID_KEY_NONE) {
            uint8_t bit = arrow_triggered[i] % 8;
            uint8_t byte = (arrow_triggered[i] / 8) + 1;
            if (byte > 0 && byte <= 31) keyboardReport[byte] |= (1 << bit);
        }
    }

    // Get Button Data
    for (int i = 0; i < BTN_GPIO_SIZE; i++) {
        if ((debounced_btn >> i) % 2 == 1) {
                if (button_mapping[i] == FN_TRANSPARENT) {
                    button_mapping[i] = this->profileNow->base_layer_btn[i]; // modified the button mapping to base layer
                }

                if (button_mapping[i] >= FN_BOOTSEL && button_mapping[i] <= FN_NEXT_PROFILE) {
                    // Functions Btns 
                    this->handle_func_btn(button_mapping[i]);}
                else {
                    uint8_t bit = button_mapping[i] % 8;
                    uint8_t byte = (button_mapping[i] / 8) + 1;
                    if (byte > 0 && byte <= 31) keyboardReport[byte] |= (1 << bit);
                }
        }
    }

    // take turns to sent mouse report and button report 
    // report mouse only when mouse is moved
    if (mouse_x + mouse_y != 0 && !has_sent_mouse_report) {
        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, mouse_x,
                            mouse_y, 0, 0);
        has_sent_mouse_report = true;
    }
    else
    {
        tud_hid_n_report(0x00, REPORT_ID_KEYBOARD, &keyboardReport,
                         sizeof(keyboardReport));
        has_sent_mouse_report = false;
    }
}

void Gamepad::send_switch_report()
{

    //Initialization
    uint8_t hat_status = 0;
    switchReport.lx = SWITCH_JOYSTICK_MID;
    switchReport.ly = SWITCH_JOYSTICK_MID;
    switchReport.rx = SWITCH_JOYSTICK_MID;
    switchReport.ry = SWITCH_JOYSTICK_MID;
    switchReport.buttons = 0;
    uint8_t *button_mapping = isBaseLayer ? profileNow->base_layer_btn : profileNow->append_layer_btn;

    //Rotary Data
    for (int i = 0; i < ENC_GPIO_SIZE; i++) {
        // send mouse report when rotary encoder is moved and not sent mouse report last round
        switch(this->profileNow->rotary_mode[i]){
            case JOY_LEFT_X:
                if (rotary->dir_debounced[i] == 0) //ccw
                    switchReport.lx = SWITCH_JOYSTICK_MIN;
                else if (rotary->dir_debounced[i] == 2) //cw
                    switchReport.lx = SWITCH_JOYSTICK_MAX;
                break;
            case JOY_LEFT_Y:
                if (rotary->dir_debounced[i] == 0) //ccw
                    switchReport.ly = SWITCH_JOYSTICK_MIN;
                else if (rotary->dir_debounced[i] == 2) //cw
                    switchReport.ly = SWITCH_JOYSTICK_MAX;
                break;
            case JOY_RIGHT_X:
                if (rotary->dir_debounced[i] == 0) //ccw
                    switchReport.rx = SWITCH_JOYSTICK_MIN;
                else if (rotary->dir_debounced[i] == 2) //cw
                    switchReport.rx = SWITCH_JOYSTICK_MAX;
                break;
            case JOY_RIGHT_Y:
                if (rotary->dir_debounced[i] == 0) //ccw
                    switchReport.ry = SWITCH_JOYSTICK_MIN;
                else if (rotary->dir_debounced[i] == 2) //cw
                    switchReport.ry = SWITCH_JOYSTICK_MAX;
                break;
        }
    }


    //Button Data
    for (int i = 0; i < BTN_GPIO_SIZE; i++)
    {
        if ((debounced_btn >> i) % 2 == 1)
        {
            if (button_mapping[i] == FN_TRANSPARENT)
            {
                button_mapping[i] = this->profileNow->base_layer_btn[i]; // modified the button mapping to base layer
            }

            if (button_mapping[i] == SWITCH_NONE)
                continue;
            else if (button_mapping[i] <= SWITCH_LEFT)
            {
                // Hat Button
                // Save Hat Status First, Convert to report later
                // Shift 0~3 bit for up down left right
                hat_status |= (1U << button_mapping[i]);
            }
            else if (button_mapping[i] >= SWITCH_Y && button_mapping[i] <= SWITCH_CAPTURE)
            {
                // Buttons
                // Direct convert to report, shifting number with pre-defined enum
                switchReport.buttons |= (1U << (button_mapping[i] - SWITCH_Y));
            }
            else if (button_mapping[i] >= FN_BOOTSEL && button_mapping[i] <= FN_NEXT_PROFILE)
            {
                // Functions Btns
                this->handle_func_btn(button_mapping[i]);
            }

        }
    }

    // Turn Hat Status to Report
    switch (hat_status)
    {
    case (1U << SWITCH_UP):
        switchReport.hat = SWITCH_HAT_UP;
        break;
    case (1U << SWITCH_UP) | (1U << SWITCH_RIGHT):
        switchReport.hat = SWITCH_HAT_UPRIGHT;
        break;
    case (1U << SWITCH_RIGHT):
        switchReport.hat = SWITCH_HAT_RIGHT;
        break;
    case (1U << SWITCH_DOWN) | (1U << SWITCH_RIGHT):
        switchReport.hat = SWITCH_HAT_DOWNRIGHT;
        break;
    case (1U << SWITCH_DOWN):
        switchReport.hat = SWITCH_HAT_DOWN;
        break;
    case (1U << SWITCH_DOWN) | (1U << SWITCH_LEFT):
        switchReport.hat = SWITCH_HAT_DOWNLEFT;
        break;
    case (1U << SWITCH_LEFT):
        switchReport.hat = SWITCH_HAT_LEFT;
        break;
    case (1U << SWITCH_UP) | (1U << SWITCH_LEFT):
        switchReport.hat = SWITCH_HAT_UPLEFT;
        break;
    default:
        switchReport.hat = SWITCH_HAT_NOTHING;
        break;
    }
    this->__send_joypad_report(&switchReport, sizeof(switchReport));
}

void Gamepad::__send_joypad_report(void *report, uint16_t report_size)
{
    static uint8_t previous_report[CFG_TUD_ENDPOINT0_SIZE] = {};

    if (tud_suspended())
        tud_remote_wakeup();

    if (memcmp(previous_report, report, report_size) != 0)
    {
        bool sent = false;
        switch (input_mode)
        {
        case INPUT_MODE_XINPUT:

            if (tud_ready() &&                                          // Is the device ready?
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
                sent = tud_hid_report(0, report, report_size); // 0 for reportid
            else
                sent = false;
            break;
        }

        if (sent)
            memcpy(previous_report, report, report_size);
    }
}
void Gamepad::send_report()
{
    if (tud_hid_ready())
    {
        rotary->update();

        // Checking if append layer btn is pressed
        isBaseLayer = true; // reset the flag
        for (int i = 0; i < BTN_GPIO_SIZE; i++)
        {
            if ((debounced_btn >> i) % 2 == 1 && this->profileNow->base_layer_btn[i] == FN_APPEND_LAYER)
            {
                // Only checking base layer button mapping to avoid the situation that append layer key is not set corrected
                this->isBaseLayer = false;
                break;
            }
        }
        // this->updateDisplay= true; // for debugging 

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
void Gamepad::change_profile(int new_index)
{
    // Index Bounding Checking
    this->profile_index = (new_index + profile_count) % profile_count; // add profile count to make sure it is positive
    this->profileNow = &profiles[profile_index];
    this->isProfileUpdated = true;
}
void Gamepad::handle_func_btn(uint8_t key)
{
    switch (key)
    {
    case FN_NEXT_PROFILE:
        this->change_profile(profile_index + 1); // Bounding Checking in change_profile
        sleep_ms(200);                           // sleep to prevent double press
        return;
    case FN_PREV_PROFILE:
        this->change_profile(profile_index - 1); // Bounding Checking in change_profile
        sleep_ms(200);                           // sleep to prevent double press
        return;
    case FN_BOOTSEL:
        reset_usb_boot(0, 0);
        return;
    case FN_APPEND_LAYER:
        // this->isBaseLayer = false;
        // not handle with it in this function
        break;
    default:
        break;
    }
}
