#ifndef __USB_KEYBOARD_H__
#define __USB_KEYBOARD_H__

#include <stdbool.h>
#include <stdint.h>

extern uint8_t nkro_key_report[32];

void usb_init(bool (*)(uint8_t*));

uint32_t usb_send_serial_data(void *buf, int len);

void usb_keyboard_keys_up(void);

void usb_keyboard_key_up(uint8_t usb_keycode);

void usb_keyboard_key_down(uint8_t usb_keycode);

uint32_t usb_send_keyboard_report(void);

uint32_t usb_send_keys_if_changed(void);

#endif // __USB_KEYBOARD_H__
