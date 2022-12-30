// #ifndef KEYBOARD_DESCRIPTORS_H_
// #define KEYBOARD_DESCRIPTORS_H_

#include "device/usbd.h"
// #include "descriptorHelper.h"
// #include "general_config.h"
// #include "controller_config.h"
#include "descriptors.h"
#include "tusb.h"


//--------------------------------------------------------------------+
// HID Device Descriptor
//--------------------------------------------------------------------+

tusb_desc_device_t const keyboard_device_descriptor= {
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
uint8_t const keyboard_report_descriptor[] = {
    // GAMECON_REPORT_DESC_LIGHTS(HID_REPORT_ID(REPORT_ID_LIGHTS)),
    GAMECON_REPORT_DESC_NKRO(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE))
};

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+


uint8_t const keyboard_configuration_descriptor[] = {
    // Config number, interface count, string index, total length, attribute,
    // power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, protocol, report descriptor len, EP In
    // address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE,
                       sizeof(keyboard_report_descriptor), EPNUM_HID,
                       CFG_TUD_HID_EP_BUFSIZE, 1)
                       };

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const* keyboard_string_descriptor[] = {
    (const char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "Rist",              // 1: Manufacturer
    "Pico Fever(KB)",      // 2: Product
    "20221229",                    // 3: Serials, should use chip ID
};


// #endif /* USB_DESCRIPTORS_H_ */