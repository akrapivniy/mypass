/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STM32F1
#define STM32F1
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

#include <libopencm3/cm3/systick.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "usb-keyboard-msc.h"
#include "keyboard.h"
#include "ssd1306-i2c.h"

uint32_t ms_ticks = 0;
uint32_t key_time_counter = 0;
uint32_t menu_time_counter = 0;

uint32_t keydown_enable = 0;
uint32_t keyup_enable = 0;

uint32_t key_mode = 0;
uint32_t key_pass_count = 0;
uint32_t current_key = 0;

uint8_t current_item = 0;
uint8_t new_current_item = 1;

struct pass_item {
	int id;
	char name[20];
	char user[40];
	char user_switch[40];
	char pass[40];
	char pass_switch[40];
};


struct flash_disk {
	char pin_code[6];
	char reserved[256];
	struct pass_item pass_items[10];
};


struct pass_item pass_items[10] = {
	{
		.id = 0,
		.name = "www.mail.ru",
		.user = "mypasstest",
		.user_switch = "\t",
		.pass = "Password111",
		.pass_switch = "\n",
	},
	{
		.id = 1,
		.name = "www.google.com",
		.pass = "PasswordGGG\n",
	},
	{
		.id = 2,
		.name = "www.shop.ru",
		.pass = "PasswordSSS\n",
	},
	{
		.id = 3,
		.name = "Skype",
		.pass = "Skype6Pass\n",
	},
};

int abs(int x)
{ /*0x1F = 31*/
	int minus_flag = x >> 0x1F;
	return((minus_flag ^ x) - minus_flag);
}

uint32_t char_to_key(char c)
{
	uint32_t keys = 0;

	switch (c) {
	case 'A':keys = KEY_SHIFT << 8;
	case 'a':keys |= KEY_A;
		break;
	case 'B':keys = KEY_SHIFT << 8;
	case 'b':keys |= KEY_B;
		break;
	case 'C':keys = KEY_SHIFT << 8;
	case 'c':keys |= KEY_C;
		break;
	case 'D':keys = KEY_SHIFT << 8;
	case 'd':keys |= KEY_D;
		break;
	case 'E':keys = KEY_SHIFT << 8;
	case 'e':keys |= KEY_E;
		break;
	case 'F':keys = KEY_SHIFT << 8;
	case 'f':keys |= KEY_F;
		break;
	case 'G':keys = KEY_SHIFT << 8;
	case 'g':keys |= KEY_G;
		break;
	case 'I':keys = KEY_SHIFT << 8;
	case 'i':keys |= KEY_I;
		break;
	case 'J':keys = KEY_SHIFT << 8;
	case 'j':keys |= KEY_J;
		break;
	case 'K':keys = KEY_SHIFT << 8;
	case 'k':keys |= KEY_K;
		break;
	case 'L':keys = KEY_SHIFT << 8;
	case 'l':keys |= KEY_L;
		break;
	case 'M':keys = KEY_SHIFT << 8;
	case 'm':keys |= KEY_M;
		break;
	case 'N':keys = KEY_SHIFT << 8;
	case 'n':keys |= KEY_N;
		break;
	case 'O':keys = KEY_SHIFT << 8;
	case 'o':keys |= KEY_O;
		break;
	case 'P':keys = KEY_SHIFT << 8;
	case 'p':keys |= KEY_P;
		break;
	case 'Q':keys = KEY_SHIFT << 8;
	case 'q':keys |= KEY_Q;
		break;
	case 'R':keys = KEY_SHIFT << 8;
	case 'r':keys |= KEY_R;
		break;
	case 'S':keys = KEY_SHIFT << 8;
	case 's':keys |= KEY_S;
		break;
	case 'T':keys = KEY_SHIFT << 8;
	case 't':keys |= KEY_T;
		break;
	case 'U':keys = KEY_SHIFT << 8;
	case 'u':keys |= KEY_U;
		break;
	case 'V':keys = KEY_SHIFT << 8;
	case 'v':keys |= KEY_V;
		break;
	case 'W':keys = KEY_SHIFT << 8;
	case 'w':keys |= KEY_W;
		break;
	case 'X':keys = KEY_SHIFT << 8;
	case 'x':keys |= KEY_X;
		break;
	case 'Y':keys = KEY_SHIFT << 8;
	case 'y':keys |= KEY_Y;
		break;
	case 'Z':keys = KEY_SHIFT << 8;
	case 'z':keys |= KEY_Z;
		break;
	case '0':keys |= KEY_0;
		break;
	case '1':keys |= KEY_1;
		break;
	case '2':keys |= KEY_2;
		break;
	case '3':keys |= KEY_3;
		break;
	case '4':keys |= KEY_4;
		break;
	case '5':keys |= KEY_5;
		break;
	case '6':keys |= KEY_6;
		break;
	case '7':keys |= KEY_7;
		break;
	case '8':keys |= KEY_8;
		break;
	case '9':keys |= KEY_9;
		break;
	case ' ':keys |= KEY_SPACE;
		break;
	case '-':keys |= KEY_MINUS;
		break;
	case '+':keys = KEY_SHIFT << 8;
	case '=':keys |= KEY_EQUALS;
		break;
	case '\n':keys |= KEY_ENTER;
		break;
	case '\t':keys |= KEY_TAB;
		break;
	}
	return keys;
}

static void clock_init(void)
{
	rcc_clock_setup_in_hsi_out_48mhz();

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	rcc_periph_clock_enable(RCC_AFIO);

	// One millisecond is clock rate (48Mhz) divided by a thousand = 48K.
	systick_set_reload(48000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	systick_interrupt_enable();
}

void button_init()
{
	nvic_enable_irq(NVIC_EXTI2_IRQ);
	nvic_enable_irq(NVIC_EXTI3_IRQ);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO2 | GPIO3);

	gpio_clear(GPIOA, GPIO2 | GPIO3);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI2, GPIOA);
	exti_select_source(EXTI3, GPIOA);
	exti_set_trigger(EXTI2, EXTI_TRIGGER_FALLING);
	exti_set_trigger(EXTI3, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI2);
	exti_enable_request(EXTI3);
}

void exti2_isr(void)
{
	exti_reset_request(EXTI2);

	if (!menu_time_counter) {
		if (keydown_enable || keyup_enable) {
			key_mode = 6;
			key_pass_count = 0;
		} else {
			key_mode = 0;
			key_pass_count = 0;
			keydown_enable = 1;
		}
		menu_time_counter = 200;
	}
}

void exti3_isr(void)
{
	exti_reset_request(EXTI3);

	if (!menu_time_counter) {
		current_item = (current_item + 1) & 3;
		new_current_item = 1;
		menu_time_counter = 200;
	}

}

void sys_tick_handler(void)
{
	ms_ticks++;
	if (key_time_counter) key_time_counter--;
	if (menu_time_counter) menu_time_counter--;
}

static void i2c1_init(void)
{
	/* Enable clocks for I2C1 and AFIO. */
	rcc_periph_clock_enable(RCC_I2C1);
	/* Set alternate functions for the SCL and SDA pins of I2C1. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO_I2C1_SCL | GPIO_I2C1_SDA);
	/* Disable the I2C before changing any configuration. */
	i2c_reset(I2C1);
	i2c_peripheral_disable(I2C1);
	/* APB1 is running at 36MHz. */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);
	/* 400KHz - I2C Fast Mode */
	i2c_set_fast_mode(I2C1);
	/*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C1, 0x1e);
	/*
	 * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
	 * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
	 * Incremented by 1 -> 11.
	 */
	i2c_set_trise(I2C1, 0x0b);
	/*
	 * This is our slave address - needed only if we want to receive from
	 * other masters.
	 */
	i2c_set_own_7bit_slave_address(I2C1, 0x30);
	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C1);
}

bool process_data(uint8_t *buff)
{
}

int get_current_key()
{
	if (key_mode == 0) {
		if (pass_items[current_item].user[key_pass_count] != 0) {
			current_key = char_to_key(pass_items[current_item].user[key_pass_count]);
			key_pass_count++;
			return 1;
		} else {
			key_mode = 1;
			key_pass_count = 0;
		}
	}
	if (key_mode == 1) {
		if (pass_items[current_item].user_switch[key_pass_count] != 0) {
			current_key = char_to_key(pass_items[current_item].user_switch[key_pass_count]);
			key_pass_count++;
			return 1;
		} else {
			key_mode = 2;
			key_pass_count = 0;
		}
	}
	if (key_mode == 2) {
		if (pass_items[current_item].pass[key_pass_count] != 0) {
			current_key = char_to_key(pass_items[current_item].pass[key_pass_count]);
			key_pass_count++;
			return 1;
		} else {
			key_mode = 3;
			key_pass_count = 0;
		}
	}
	if (key_mode == 3) {
		if (pass_items[current_item].pass_switch[key_pass_count] != 0) {
			current_key = char_to_key(pass_items[current_item].pass_switch[key_pass_count]);
			key_pass_count++;
			return 1;
		} else {
			key_mode = 0;
			key_pass_count = 0;
			return 0;
		}
	}
	key_mode = 0;
	key_pass_count = 0;
	return 0;
}

void show_item(int item)
{
	ssd1306_new_string(0, 16, pass_items[item].name);

	ssd1306_new_string(0, 32, "user:");
	if (pass_items[item].user[0] == 0)
		ssd1306_string(40, 32, " none");
	else
		ssd1306_string(40, 32, pass_items[item].user);

	ssd1306_new_string(0, 40, "pass:");
	if (pass_items[item].pass[0] == 0)
		ssd1306_string(40, 40, " none");
	else
		ssd1306_string(40, 40, " yes");

	ssd1306_update();
}

int main(void)
{
	volatile int i;
	int key;

	clock_init();
	usb_init(&process_data);

	i2c1_init();


	for (i = 0; i < 100000; i++);

	ssd1306_init(I2C1);


	ssd1306_new_string(0, 8, "Choise profile:");
	ssd1306_update();
	button_init();
	while (1) {
		if (keydown_enable && !key_time_counter) {
			keydown_enable = 0;
			if (get_current_key()) {
				uint32_t modify_key = current_key >> 8;
				if (modify_key) {
					usb_keyboard_key_down(KEY_SHIFT);
					//usb_send_keyboard_report();
					//for (i = 0; i < 100000; i++) __asm__("nop");
				}
				usb_keyboard_key_down(current_key & 0xff);
				usb_send_keyboard_report();
				key_time_counter = 30;
				keyup_enable = 1;
			}

		}
		if (keyup_enable && !key_time_counter) {
			uint32_t modify_key = current_key >> 8;
			keyup_enable = 0;
			usb_keyboard_key_up(current_key & 0xff);
			//			usb_send_keyboard_report();
			if (modify_key) {
				//				for (i = 0; i < 100000; i++) __asm__("nop");
				usb_keyboard_key_up(KEY_SHIFT);
			}
			usb_send_keyboard_report();
			key_time_counter = 90;
			keydown_enable = 1;
		}

		if (new_current_item) {
			show_item(current_item);
			new_current_item = 0;
		}


		__asm__("nop");
	}

	return 0;
}
