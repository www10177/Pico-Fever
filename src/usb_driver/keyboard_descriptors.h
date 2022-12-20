#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

#include "device/usbd.h"
#include "descriptorHelper.h"
#include "controller_config.h"
#include "tusb.h"


// because they are missing from tusb_hid.h
#define HID_STRING_INDEX(x) HID_REPORT_ITEM(x, 7, RI_TYPE_LOCAL, 1)
#define HID_STRING_INDEX_N(x, n) HID_REPORT_ITEM(x, 7, RI_TYPE_LOCAL, n)
#define HID_STRING_MINIMUM(x) HID_REPORT_ITEM(x, 8, RI_TYPE_LOCAL, 1)
#define HID_STRING_MINIMUM_N(x, n) HID_REPORT_ITEM(x, 8, RI_TYPE_LOCAL, n)
#define HID_STRING_MAXIMUM(x) HID_REPORT_ITEM(x, 9, RI_TYPE_LOCAL, 1)
#define HID_STRING_MAXIMUM_N(x, n) HID_REPORT_ITEM(x, 9, RI_TYPE_LOCAL, n)

// Joystick Report Descriptor Template - Based off Drewol/rp2040-gamecon
// Button Map | X | Y
#define GAMECON_REPORT_DESC_JOYSTICK(...)                                      \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                      \
      HID_USAGE(HID_USAGE_DESKTOP_JOYSTICK),                                   \
      HID_COLLECTION(HID_COLLECTION_APPLICATION),                              \
      __VA_ARGS__ HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON), HID_USAGE_MIN(1),     \
      HID_USAGE_MAX(SW_GPIO_SIZE),                                             \
      HID_LOGICAL_MIN(0), HID_LOGICAL_MAX(1), HID_REPORT_COUNT(SW_GPIO_SIZE),  \
      HID_REPORT_SIZE(1), HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),   \
      HID_REPORT_COUNT(1), HID_REPORT_SIZE(16 - SW_GPIO_SIZE), /*Padding*/\
      HID_INPUT(HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE),                   \
      HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP), HID_LOGICAL_MIN(0x00),           \
      HID_LOGICAL_MAX_N(0x00ff, 2),                                            \
      HID_USAGE(HID_USAGE_DESKTOP_X), /*Joystick*/                             \
      HID_USAGE(HID_USAGE_DESKTOP_Y), HID_REPORT_COUNT(2), HID_REPORT_SIZE(8), \
      HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), HID_COLLECTION_END

// Light Map
#define GAMECON_REPORT_DESC_LIGHTS(...)                                        \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP), HID_USAGE(0x00),                     \
      HID_COLLECTION(HID_COLLECTION_APPLICATION),                              \
      __VA_ARGS__ HID_REPORT_COUNT(LED_GPIO_SIZE + (WS2812B_LED_ZONES * 3)),   \
      HID_REPORT_SIZE(8), HID_LOGICAL_MIN(0x00), HID_LOGICAL_MAX_N(0x00ff, 2), \
      HID_USAGE_PAGE(HID_USAGE_PAGE_ORDINAL), HID_STRING_MINIMUM(4),           \
      HID_STRING_MAXIMUM(16), HID_USAGE_MIN(1), HID_USAGE_MAX(16),             \
      HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), HID_REPORT_COUNT(1), \
      HID_REPORT_SIZE(8), /*Padding*/                                          \
      HID_INPUT(HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE),                   \
      HID_COLLECTION_END

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

//--------------------------------------------------------------------+
// HID Device Descriptor
//--------------------------------------------------------------------+

tusb_desc_device_t const desc_device_joy = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0xCafe,
    .idProduct = USB_PID,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01};

tusb_desc_device_t const desc_device_key = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0xCafe,
    .idProduct = USB_PID,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x04,

    .bNumConfigurations = 0x01};

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_hid_report_joy[] = {
    GAMECON_REPORT_DESC_JOYSTICK(HID_REPORT_ID(REPORT_ID_JOYSTICK)),
    GAMECON_REPORT_DESC_LIGHTS(HID_REPORT_ID(REPORT_ID_LIGHTS))
};

uint8_t const desc_hid_report_key[] = {
    GAMECON_REPORT_DESC_LIGHTS(HID_REPORT_ID(REPORT_ID_LIGHTS)),
    GAMECON_REPORT_DESC_NKRO(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE))
};

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_configuration_joy[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, protocol, report descriptor len, EP In
    // address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE,
                       sizeof(desc_hid_report_joy), EPNUM_HID,
                       CFG_TUD_HID_EP_BUFSIZE, 1)};


uint8_t const desc_configuration_key[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, protocol, report descriptor len, EP In
    // address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE,
                       sizeof(desc_hid_report_key), EPNUM_HID,
                       CFG_TUD_HID_EP_BUFSIZE, 1)
                       };

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const* string_desc_arr[] = {
    (const char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "SpeedyPotato",              // 1: Manufacturer
    "Pico Game Controller",      // 2: Product
    "123456",                    // 3: Serials, should use chip ID
};


#endif /* USB_DESCRIPTORS_H_ */