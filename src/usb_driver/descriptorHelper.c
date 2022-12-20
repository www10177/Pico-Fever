// #include "descriptorHelper.h"

#include "keyboard_descriptors.h"
// #include "HIDDescriptors.h"
// #include "SwitchDescriptors.h"
// #include "XInputDescriptors.h"
// #include "tusb_config.h"
#include "descriptorHelper.h"
#include "tusb.h"
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------+
// HID Deivce Descriptor
//--------------------------------------------------------------------+
// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
extern InputMode input_mode;
uint8_t const* tud_descriptor_device_cb(void) {

  if (input_mode == INPUT_MODE_KEYBOARD) {
    return (uint8_t const*) &desc_device_key;
  }
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb(uint8_t itf) {
  (void)itf;
  return (joy_mode_check ? desc_hid_report_joy : desc_hid_report_key);
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
  (void)index;  // for multiple configurations
  return (joy_mode_check ? desc_configuration_joy : desc_configuration_key);
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+


static uint16_t _desc_str[64];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void)langid;

  uint8_t chr_count;

  if (index == 0) {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  } else {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
      return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if (chr_count > 63) chr_count = 63;

    // Convert ASCII string into UTF-16
    for (uint8_t i = 0; i < chr_count; i++) {
      _desc_str[1 + i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

  return _desc_str;
}
#ifdef __cplusplus
}
#endif