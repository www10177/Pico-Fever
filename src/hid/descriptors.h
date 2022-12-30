#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

extern tusb_desc_device_t const keyboard_device_descriptor ;
extern uint8_t const keyboard_report_descriptor[];
extern uint8_t const keyboard_configuration_descriptor[] ;
extern char const* keyboard_string_descriptor[] ;



extern tusb_desc_device_t const switch_device_descriptor ;
extern uint8_t const switch_report_descriptor[];
extern uint8_t const switch_configuration_descriptor[] ;
extern char const* switch_string_descriptor[] ;

extern tusb_desc_device_t const xinput_device_descriptor ;
extern uint8_t const xinput_report_descriptor[];
extern uint8_t const xinput_configuration_descriptor[] ;
extern char const* xinput_string_descriptor[] ;


// #endif /* USB_DESCRIPTORS_H_ */
#endif