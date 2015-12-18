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

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "usb-keyboard-uart.h"
#include "keyboard.h"
#include "ssd1306-i2c.h"

uint32_t ms_ticks = 0;
uint32_t keydown_enable = 0;
uint32_t keyup_enable = 0;
uint32_t key_counter = 0;

uint32_t menu_counter = 0;


struct pass_item {
	int id;
	char name[20];
	char user[40];
	char user_switch[40];
	char pass[40];
	char pass_switch[40];
};


struct pass_item pass_items[10] = {
	{
		.id = 0,
		.name = "www.mail.ru",
		.user = "test1",
		.user_switch = "test1",
		.pass = "password123123",
		.pass_switch = "test1",
	},
	{
		.id = 1,
		.name = "www.rambler.ru",
		.pass = "password123123\n",
	},
	{
		.id = 1,
		.name = "www.google.com",
		.pass = "password123123\n",
	},
	{
		.id = 1,
		.name = "Skype",
		.pass = "password123123\n",
	},
};

int abs(int x)
{ /*0x1F = 31*/
	int minus_flag = x >> 0x1F;
	return((minus_flag ^ x) - minus_flag);
}

static void clock_init(void)
{
	rcc_clock_setup_in_hsi_out_48mhz();

	//	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	rcc_periph_clock_enable(RCC_AFIO);

	// One millisecond is clock rate (48Mhz) divided by a thousand = 48K.
	systick_set_reload(48000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	systick_interrupt_enable();
}


void sys_tick_handler(void) 
{
    ms_ticks++;
    if (key_counter) key_counter--;
    if (menu_counter) menu_counter--;
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


void show_item (int item)
{
	ssd1306_new_string (0, 16, pass_items[item].name);
	
	ssd1306_new_string (0, 32, "user:");
	if ( pass_items[item].user[0] == 0 )
		ssd1306_string (40, 32, " none");
	else
		ssd1306_string (40, 32, pass_items[item].user);
	
	ssd1306_new_string (0, 40, "pass:");
	if ( pass_items[item].pass[0] == 0 )
		ssd1306_string (40, 40, " none");
	else
		ssd1306_string (40, 40, " yes");

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
	i = 0;
	key = 4;
	keydown_enable = 1;
	key_counter = 2000;
	while (1) {
		if (keydown_enable && !key_counter) {
			keydown_enable = 0;
			usb_keyboard_key_down (key);
			usb_send_keyboard_report ();
			key_counter = 100;
			keyup_enable = 1;
			
		}
		if (keyup_enable && !key_counter) {
			keyup_enable = 0;
			usb_keyboard_key_up (key);
			usb_send_keyboard_report ();
			key++;
			key_counter = 100;
			keydown_enable = 1;
		}
		
		if (!menu_counter) {
			show_item (i);
			i++;
			i &= 3;
			menu_counter = 1000;
		}
			

		__asm__("nop");
	}

	return 0;
}
