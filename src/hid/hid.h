#ifndef HID_H_
#define HID_H_

#include "tusb.h"

#define _PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))
#define USB_PID                                                      \
  (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
   _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4))

typedef enum
{
	INPUT_MODE_XINPUT,
	INPUT_MODE_SWITCH,
	INPUT_MODE_DINPUT,
    INPUT_MODE_KEYBOARD,
	INPUT_MODE_CONFIG = 255,
} InputMode;

enum {
  REPORT_ID_JOYSTICK = 1,
  REPORT_ID_LIGHTS,
  REPORT_ID_KEYBOARD,
  REPORT_ID_MOUSE,
};

enum { ITF_NUM_HID, ITF_NUM_TOTAL };

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID 0x81
uint8_t const* tud_descriptor_device_cb(void) ;
uint8_t const* tud_hid_descriptor_report_cb(uint8_t itf) ;
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) ;
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) ;
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t* buffer,
                               uint16_t reqlen) ;
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const* buffer,
                           uint16_t bufsize) ;

#endif