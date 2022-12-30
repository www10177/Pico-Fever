/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */
// #ifndef XINPUT_DESCRIPTORS_H
// #define XINPUT_DESCRIPTORS_H

#include <stdint.h>


// static const uint8_t xinput_string_language[]    = { 0x09, 0x04 };
// static const uint8_t xinput_string_manfacturer[] = "Microsoft";
// static const uint8_t xinput_string_product[]     = "XInput STANDARD GAMEPAD";
// static const uint8_t xinput_string_version[]     = "1.0";

// static const uint8_t *xinput_string_descriptors[] =
// {
// 	xinput_string_language,
// 	xinput_string_manfacturer,
// 	xinput_string_product,
// 	xinput_string_version
// };
// char const* xinput_string_descriptor[] = {
//     (const char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
//     "Rist",              // 1: Manufacturer
//     "Pico Fever(XBOX)",      // 2: Product
//     "20221229",                    // 3: Serials, should use chip ID
// };
char const* xinput_string_descriptor[] = {
    (const char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "Microsoft",              // 1: Manufacturer
    "XInput STANDARD GAMEPAD",      // 2: Product
    "1.0",                    // 3: Serials, should use chip ID
};

static const uint8_t xinput_device_descriptor[] =
{
	0x12,       // bLength
	0x01,       // bDescriptorType (Device)
	0x00, 0x02, // bcdUSB 2.00
	0xFF,	      // bDeviceClass
	0xFF,	      // bDeviceSubClass
	0xFF,	      // bDeviceProtocol
	0x40,	      // bMaxPacketSize0 64
	0x5E, 0x04, // idVendor 0x045E
	0x8E, 0x02, // idProduct 0x028E
	0x14, 0x01, // bcdDevice 2.14
	0x01,       // iManufacturer (String Index)
	0x02,       // iProduct (String Index)
	0x03,       // iSerialNumber (String Index)
	0x01,       // bNumConfigurations 1
};

static const uint8_t xinput_configuration_descriptor[] =
{
	0x09,        // bLength
	0x02,        // bDescriptorType (Configuration)
	0x30, 0x00,  // wTotalLength 48
	0x01,        // bNumInterfaces 1
	0x01,        // bConfigurationValue
	0x00,        // iConfiguration (String Index)
	0x80,        // bmAttributes
	0xFA,        // bMaxPower 500mA

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x00,        // bInterfaceNumber 0
	0x00,        // bAlternateSetting
	0x02,        // bNumEndpoints 2
	0xFF,        // bInterfaceClass
	0x5D,        // bInterfaceSubClass
	0x01,        // bInterfaceProtocol
	0x00,        // iInterface (String Index)

	0x10,        // bLength
	0x21,        // bDescriptorType (HID)
	0x10, 0x01,  // bcdHID 1.10
	0x01,        // bCountryCode
	0x24,        // bNumDescriptors
	0x81,        // bDescriptorType[0] (Unknown 0x81)
	0x14, 0x03,  // wDescriptorLength[0] 788
	0x00,        // bDescriptorType[1] (Unknown 0x00)
	0x03, 0x13,  // wDescriptorLength[1] 4867
	0x01,        // bDescriptorType[2] (Unknown 0x02)
	0x00, 0x03,  // wDescriptorLength[2] 768
	0x00,        // bDescriptorType[3] (Unknown 0x00)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x81,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x01,        // bInterval 1 (unit depends on device speed)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x01,        // bEndpointAddress (OUT/H2D)
	0x03,        // bmAttributes (Interrupt)
	0x20, 0x00,  // wMaxPacketSize 32
	0x08,        // bInterval 8 (unit depends on device speed)
};

//Same as HID_report(DINPUT?)
// static const uint8_t xinput_report_descriptor[] =
// {
// 	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
// 	0x09, 0x05,        // Usage (Game Pad)
// 	0xA1, 0x01,        // Collection (Application)
// 	0x15, 0x00,        //   Logical Minimum (0)
// 	0x25, 0x01,        //   Logical Maximum (1)
// 	0x35, 0x00,        //   Physical Minimum (0)
// 	0x45, 0x01,        //   Physical Maximum (1)
// 	0x75, 0x01,        //   Report Size (1)
// 	0x95, 0x10,        //   Report Count (16)
// 	0x05, 0x09,        //   Usage Page (Button)
// 	0x19, 0x01,        //   Usage Minimum (0x01)
// 	0x29, 0x10,        //   Usage Maximum (0x10)
// 	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
// 	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
// 	0x25, 0x07,        //   Logical Maximum (7)
// 	0x46, 0x3B, 0x01,  //   Physical Maximum (315)
// 	0x75, 0x04,        //   Report Size (4)
// 	0x95, 0x01,        //   Report Count (1)
// 	0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
// 	0x09, 0x39,        //   Usage (Hat switch)
// 	0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
// 	0x65, 0x00,        //   Unit (None)
// 	0x95, 0x01,        //   Report Count (1)
// 	0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
// 	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
// 	0x46, 0xFF, 0x00,  //   Physical Maximum (255)
// 	0x09, 0x30,        //   Usage (X)
// 	0x09, 0x31,        //   Usage (Y)
// 	0x09, 0x32,        //   Usage (Z)
// 	0x09, 0x35,        //   Usage (Rz)
// 	0x75, 0x08,        //   Report Size (8)
// 	0x95, 0x04,        //   Report Count (4)
// 	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
// 	                   //   * PS3 "magic" vendor page *
// 	0x06, 0x00, 0xff,  //   Usage Page (Vendor Specific)
// 	0x09, 0x20,        //   Unknown
// 	0x75, 0x08,        //   Report Size (8)
// 	0x95, 0x01,        //   Report Count (1)
// 	0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
// 	0xC0,              // End Collection
// };

static const uint8_t xinput_report_descriptor[] =
{
	0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,        // USAGE (Gamepad)
	0xa1, 0x01,        // COLLECTION (Application)
	0x15, 0x00,        //   LOGICAL_MINIMUM (0)
	0x25, 0x01,        //   LOGICAL_MAXIMUM (1)
	0x35, 0x00,        //   PHYSICAL_MINIMUM (0)
	0x45, 0x01,        //   PHYSICAL_MAXIMUM (1)
	0x75, 0x01,        //   REPORT_SIZE (1)
	0x95, 0x0e,        //   REPORT_COUNT (13)
	0x05, 0x09,        //   USAGE_PAGE (Button)
	0x19, 0x01,        //   USAGE_MINIMUM (Button 1)
	0x29, 0x0e,        //   USAGE_MAXIMUM (Button 13)
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x95, 0x02,        //   REPORT_COUNT (3)
	0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
	0x05, 0x01,        //   USAGE_PAGE (Generic Desktop)
	0x25, 0x07,        //   LOGICAL_MAXIMUM (7)
	0x46, 0x3b, 0x01,  //   PHYSICAL_MAXIMUM (315)
	0x75, 0x04,        //   REPORT_SIZE (4)
	0x95, 0x01,        //   REPORT_COUNT (1)
	0x65, 0x14,        //   UNIT (Eng Rot:Angular Pos)
	0x09, 0x39,        //   USAGE (Hat switch)
	0x81, 0x42,        //   INPUT (Data,Var,Abs,Null)
	0x65, 0x00,        //   UNIT (None)
	0x95, 0x01,        //   REPORT_COUNT (1)
	0x81, 0x01,        //   INPUT (Cnst,Ary,Abs)
	0x26, 0xff, 0x00,  //   LOGICAL_MAXIMUM (255)
	0x46, 0xff, 0x00,  //   PHYSICAL_MAXIMUM (255)
	0x09, 0x30,        //   USAGE (X)
	0x09, 0x31,        //   USAGE (Y)
	0x09, 0x32,        //   USAGE (Z)
	0x09, 0x35,        //   USAGE (Rz)
	0x75, 0x08,        //   REPORT_SIZE (8)
	0x95, 0x04,        //   REPORT_COUNT (6)
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x06, 0x00, 0xff,  //   USAGE_PAGE (Vendor Specific)
	0x09, 0x20,        //   Unknown
	0x09, 0x21,        //   Unknown
	0x09, 0x22,        //   Unknown
	0x09, 0x23,        //   Unknown
	0x09, 0x24,        //   Unknown
	0x09, 0x25,        //   Unknown
	0x09, 0x26,        //   Unknown
	0x09, 0x27,        //   Unknown
	0x09, 0x28,        //   Unknown
	0x09, 0x29,        //   Unknown
	0x09, 0x2a,        //   Unknown
	0x09, 0x2b,        //   Unknown
	0x95, 0x0c,        //   REPORT_COUNT (12)
	0x81, 0x02,        //   INPUT (Data,Var,Abs)
	0x0a, 0x21, 0x26,  //   Unknown
	0x95, 0x08,        //   REPORT_COUNT (8)
	0xb1, 0x02,        //   FEATURE (Data,Var,Abs)
	0xc0               // END_COLLECTION
};
// #endif