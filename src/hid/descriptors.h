#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

// Used for keyboard  descriptors
// because they are missing from tusb_hid.h
#define HID_STRING_INDEX(x) HID_REPORT_ITEM(x, 7, RI_TYPE_LOCAL, 1)
#define HID_STRING_INDEX_N(x, n) HID_REPORT_ITEM(x, 7, RI_TYPE_LOCAL, n)
#define HID_STRING_MINIMUM(x) HID_REPORT_ITEM(x, 8, RI_TYPE_LOCAL, 1)
#define HID_STRING_MINIMUM_N(x, n) HID_REPORT_ITEM(x, 8, RI_TYPE_LOCAL, n)
#define HID_STRING_MAXIMUM(x) HID_REPORT_ITEM(x, 9, RI_TYPE_LOCAL, 1)
#define HID_STRING_MAXIMUM_N(x, n) HID_REPORT_ITEM(x, 9, RI_TYPE_LOCAL, n)

// NKRO Descriptor
#define GAMECON_REPORT_DESC_NKRO(...)                                         \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP), HID_USAGE(HID_USAGE_PAGE_KEYBOARD), \
      HID_COLLECTION(HID_COLLECTION_APPLICATION),                             \
      __VA_ARGS__ HID_REPORT_SIZE(1), HID_REPORT_COUNT(8),                    \
      HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD), HID_USAGE_MIN(224),            \
      HID_USAGE_MAX(231), HID_LOGICAL_MIN(0), HID_LOGICAL_MAX(1),             \
      HID_INPUT(HID_VARIABLE), HID_REPORT_SIZE(1), HID_REPORT_COUNT(31 * 8),  \
      HID_LOGICAL_MIN(0), HID_LOGICAL_MAX(1),                                 \
      HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD), HID_USAGE_MIN(0),              \
      HID_USAGE_MAX(31 * 8 - 1), HID_INPUT(HID_VARIABLE), HID_COLLECTION_END

extern tusb_desc_device_t const keyboard_device_descriptor ;
extern uint8_t const keyboard_report_descriptor[];
extern uint8_t const keyboard_configuration_descriptor[] ;
extern char const* keyboard_string_descriptor[] ;



extern uint8_t const switch_device_descriptor ;
extern uint8_t const switch_report_descriptor[];
extern uint8_t const switch_configuration_descriptor[] ;
extern char const* switch_string_descriptor[] ;

extern uint8_t const xinput_device_descriptor ;
extern uint8_t const xinput_report_descriptor[];
extern uint8_t const xinput_configuration_descriptor[] ;
extern char const* xinput_string_descriptor[] ;


// #endif /* USB_DESCRIPTORS_H_ */
#endif