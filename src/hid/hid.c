#include "hid.h"
#include "descriptors.h"
// #include "KeyboardDescriptors.h"
// #include "SwitchDescriptors.h"
// #include "XInputDescriptors.h"
#ifdef __cplusplus
extern "C"
{
#endif
    extern InputMode input_mode;

    //--------------------------------------------------------------------+
    // HID Deivce Descriptor
    //--------------------------------------------------------------------+
    // Invoked when received GET DEVICE DESCRIPTOR
    // Application return pointer to descriptor
    uint8_t const *tud_descriptor_device_cb(void)
    {

        if (input_mode == INPUT_MODE_KEYBOARD)
        {
            return (uint8_t const *)&keyboard_device_descriptor;
        }
        else if (input_mode == INPUT_MODE_SWITCH)
        {
            return switch_device_descriptor;
        }
        else if (input_mode == INPUT_MODE_XINPUT)
        {
            return xinput_device_descriptor;
        }
    }

    //--------------------------------------------------------------------+
    // HID Report Descriptor
    //--------------------------------------------------------------------+

    // Invoked when received GET HID REPORT DESCRIPTOR
    // Application return pointer to descriptor
    // Descriptor contents must exist long enough for transfer to complete
    uint8_t const *tud_hid_descriptor_report_cb(uint8_t itf)
    {
        (void)itf;
        if (input_mode == INPUT_MODE_KEYBOARD)
        {
            return keyboard_report_descriptor;
        }
        else if (input_mode == INPUT_MODE_SWITCH)
        {
            return switch_report_descriptor;
        }
        else if (input_mode == INPUT_MODE_XINPUT)
        {
            return xinput_report_descriptor;
        }
    }

    //--------------------------------------------------------------------+
    // Configuration Descriptor
    //--------------------------------------------------------------------+

    // Invoked when received GET CONFIGURATION DESCRIPTOR
    // Application return pointer to descriptor
    // Descriptor contents must exist long enough for transfer to complete
    uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
    {
        (void)index; // for multiple configurations
        if (input_mode == INPUT_MODE_KEYBOARD)
        {
            return keyboard_configuration_descriptor;
        }
        else if (input_mode == INPUT_MODE_SWITCH)
        {
            return switch_configuration_descriptor;
        }
        else if (input_mode == INPUT_MODE_XINPUT)
        {
            return xinput_configuration_descriptor;
        }
    }

    //--------------------------------------------------------------------+
    // String Descriptors
    //--------------------------------------------------------------------+

    static uint16_t _desc_str[64];

    // Invoked when received GET STRING DESCRIPTOR request
    // Application return pointer to descriptor, whose contents must exist long
    // enough for transfer to complete
    uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
    {
        (void)langid;
        uint8_t chr_count;
        char **string_desc_arr;
        // Mode selection
        if (input_mode == INPUT_MODE_KEYBOARD)
        {
            string_desc_arr = &keyboard_string_descriptor[0];
        }
        else if (input_mode == INPUT_MODE_SWITCH)
        {
            string_desc_arr = &switch_string_descriptor[0];
        }
        else if (input_mode == INPUT_MODE_XINPUT)
        {
            string_desc_arr = &xinput_string_descriptor[0];
        }

        if (index == 0)
        {
            memcpy(&_desc_str[1], string_desc_arr[0], 2);
            chr_count = 1;
        }
        else
        {
            // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
            // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

            if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
                return NULL;

            const char *str = string_desc_arr[index];

            // Cap at max char
            chr_count = strlen(str);
            if (chr_count > 63)
                chr_count = 63;

            // Convert ASCII string into UTF-16
            for (uint8_t i = 0; i < chr_count; i++)
            {
                _desc_str[1 + i] = str[i];
            }
        }

        // first byte is length (including header), second byte is string type
        _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

        return _desc_str;
    }
    // Invoked when received GET_REPORT control request
    // Application must fill buffer report's content and return its length.
    // Return zero will cause the stack to STALL request
    uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                                   hid_report_type_t report_type, uint8_t *buffer,
                                   uint16_t reqlen)
    {
        // TODO not Implemented
        (void)itf;
        (void)report_id;
        (void)report_type;
        (void)buffer;
        (void)reqlen;

        return 0;
    }

    // Invoked when received SET_REPORT control request or
    // received data on OUT endpoint ( Report ID = 0, Type = 0 )
    void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t const *buffer,
                               uint16_t bufsize)
    {
        	(void) itf;

	// echo back anything we received from host
        	tud_hid_report(report_id, buffer, bufsize);
    }

#ifdef __cplusplus
}
#endif