#include <stdint.h>
#include <libopencm3/stm32/i2c.h>
#include "fonts.h"

struct ssd1306_font {
	uint8_t width; /* Character width for storage */
	uint8_t height; /* Character height for storage */
	uint8_t firstchar; /* The first character available */
	uint8_t lastchar; /* The last character available */
	const uint8_t *table; /* Font table start address in memory */
};
const struct ssd1306_font font8x8 = {8, 8, 32, 128, table_font8x8};
const struct ssd1306_font font8x8t = {8, 8, 32, 128, table_font8x8t};


/* internal declarations: */
#define SSD_COMMAND_MODE      0x00
#define SSD_DATA_MODE         0x40
#define SSD_INVERSE_DISPLAY   0xA7
#define SSD_DISPLAY_OFF       0xAE
#define SSD_DISPLAY_ON        0xAF
#define SSD_SET_CONTRAST      0x81
#define SSD_EXTERNAL_VCC      0x01
#define SSD_INTERNAL_VCC      0x02
#define SSD_DEACTIVATE_SCROLL 0x2E

#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_Normal_Display      0xA6

#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT  64
#define SSD1306_BUFFER_SIZE    (SSD1306_WIDTH * SSD1306_HEIGHT / 8)
#define SSD1306_I2CADDR  0x3c

static int ssd1306_i2c = 0;
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

static void ssd1306_cmd1(uint8_t c)
{
	uint32_t reg32 __attribute__((unused));

	/* Send START condition. */
	i2c_send_start(ssd1306_i2c);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(ssd1306_i2c) & I2C_SR1_SB)
		& (I2C_SR2(ssd1306_i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	/* Send destination address. */
	i2c_send_7bit_address(ssd1306_i2c, SSD1306_I2CADDR, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_ADDR));

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(ssd1306_i2c);

	/* Sending the data. */
	i2c_send_data(ssd1306_i2c, SSD_COMMAND_MODE); /* stts75 config register */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_BTF)); /* Await ByteTransferedFlag. */
	/* Polarity reverse - LED glows if temp is below Tos/Thyst. */
	i2c_send_data(ssd1306_i2c, c);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));

	/* Send STOP condition. */
	i2c_send_stop(ssd1306_i2c);
}

static void ssd1306_cmd2(uint8_t c0, uint8_t c1)
{
	uint32_t reg32 __attribute__((unused));

	/* Send START condition. */
	i2c_send_start(ssd1306_i2c);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(ssd1306_i2c) & I2C_SR1_SB)
		& (I2C_SR2(ssd1306_i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	/* Send destination address. */
	i2c_send_7bit_address(ssd1306_i2c, SSD1306_I2CADDR, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_ADDR));

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(ssd1306_i2c);

	/* Sending the data. */
	i2c_send_data(ssd1306_i2c, SSD_COMMAND_MODE); /* stts75 config register */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_BTF)); /* Await ByteTransferedFlag. */
	/* Polarity reverse - LED glows if temp is below Tos/Thyst. */
	i2c_send_data(ssd1306_i2c, c0);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	i2c_send_data(ssd1306_i2c, c1);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));

	/* Send STOP condition. */
	i2c_send_stop(ssd1306_i2c);
}

static void ssd1306_cmd3(uint8_t c0, uint8_t c1, uint8_t c2)
{
	uint32_t reg32 __attribute__((unused));

	/* Send START condition. */
	i2c_send_start(ssd1306_i2c);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(ssd1306_i2c) & I2C_SR1_SB)
		& (I2C_SR2(ssd1306_i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	/* Send destination address. */
	i2c_send_7bit_address(ssd1306_i2c, SSD1306_I2CADDR, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_ADDR));

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(ssd1306_i2c);

	/* Sending the data. */
	i2c_send_data(ssd1306_i2c, SSD_COMMAND_MODE); /* stts75 config register */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_BTF)); /* Await ByteTransferedFlag. */
	/* Polarity reverse - LED glows if temp is below Tos/Thyst. */
	i2c_send_data(ssd1306_i2c, c0);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	i2c_send_data(ssd1306_i2c, c1);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	i2c_send_data(ssd1306_i2c, c2);
	while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));

	/* Send STOP condition. */
	i2c_send_stop(ssd1306_i2c);
}

static void ssd1306_data(uint8_t *buf, int size)
{
	uint32_t reg32 __attribute__((unused));
	int i;

	/* Send START condition. */
	i2c_send_start(ssd1306_i2c);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(ssd1306_i2c) & I2C_SR1_SB)
		& (I2C_SR2(ssd1306_i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	/* Send destination address. */
	i2c_send_7bit_address(ssd1306_i2c, SSD1306_I2CADDR, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_ADDR));

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(ssd1306_i2c);

	/* Sending the data. */
	i2c_send_data(ssd1306_i2c, SSD_DATA_MODE);
	while (!(I2C_SR1(ssd1306_i2c) & I2C_SR1_BTF)); /* Await ByteTransferedFlag. */

	for (i = 0; i < size; i++) {
		i2c_send_data(ssd1306_i2c, buf[i]);
		while (!(I2C_SR1(ssd1306_i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	}
	/* Send STOP condition. */
	i2c_send_stop(ssd1306_i2c);
}

/* public functions: */
void ssd1306_init(int i2c_dev)
{
	ssd1306_i2c = i2c_dev;

	ssd1306_cmd1(SSD_DISPLAY_OFF);
	ssd1306_cmd2(SSD1306_SETDISPLAYCLOCKDIV, 0x80);
	ssd1306_cmd2(SSD1306_SETMULTIPLEX, 0x3F);
	ssd1306_cmd2(SSD1306_SETDISPLAYOFFSET, 0x00);
	ssd1306_cmd1(SSD1306_SETSTARTLINE);
	ssd1306_cmd2(SSD1306_CHARGEPUMP, 0x14);
	ssd1306_cmd2(SSD1306_MEMORYMODE, 0x00);
	ssd1306_cmd1(SSD1306_SEGREMAP | 0x1);
	ssd1306_cmd1(SSD1306_COMSCANDEC);
	ssd1306_cmd2(SSD1306_SETCOMPINS, 0x12);
	ssd1306_cmd2(SSD_SET_CONTRAST, 0xFF);
	ssd1306_cmd2(SSD1306_SETPRECHARGE, 0xF1);
	ssd1306_cmd2(SSD1306_SETVCOMDETECT, 0x40);
	ssd1306_cmd1(SSD1306_DISPLAYALLON_RESUME);
	ssd1306_cmd1(SSD1306_Normal_Display);

	ssd1306_cmd3(0x21, 0, 127);
	ssd1306_cmd3(0x22, 0, 7);
	ssd1306_cmd1(SSD_DEACTIVATE_SCROLL);

	ssd1306_clear();
	ssd1306_update();
	ssd1306_cmd1(SSD_DISPLAY_ON);
}

void ssd1306_invert(uint8_t inv)
{
	if (inv)
		ssd1306_cmd1(SSD_INVERSE_DISPLAY);
	else
		ssd1306_cmd1(SSD1306_Normal_Display);
}

void ssd1306_update()
{
	uint16_t i;
	uint8_t x;

	ssd1306_cmd1(SSD1306_SETLOWCOLUMN | 0x0);
	ssd1306_cmd1(SSD1306_SETHIGHCOLUMN | 0x0);
	ssd1306_cmd1(SSD1306_SETSTARTLINE | 0x0);

	uint8_t *p = ssd1306_buffer;
	uint8_t buf[16];
	for (i = 0; i < SSD1306_BUFFER_SIZE; i += 16) {
		for (x = 0; x < 16; x++)
			buf[x] = *p++;
		ssd1306_data(buf, sizeof(buf));
	}
}

void ssd1306_clear()
{
	memset(ssd1306_buffer, 0x00, SSD1306_BUFFER_SIZE);
}

void ssd1306_pixel(int16_t x, int16_t y, uint16_t color)
{
	uint8_t *p = ssd1306_buffer + (x + (y / 8) * SSD1306_WIDTH);
	if (color)
		*p |= 1 << (y % 8);
	else
		*p &= ~(1 << (y % 8));
}

void ssd1306_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t dy, dx;
	uint8_t addx, addy;
	int16_t P, diff, i;
	if (x1 >= x0) {
		dx = x1 - x0;
		addx = 1;
	} else {
		dx = x0 - x1;
		addx = -1;
	}
	if (y1 >= y0) {
		dy = y1 - y0;
		addy = 1;
	} else {
		dy = y0 - y1;
		addy = -1;
	}
	if (dx >= dy) {
		dy *= 2;
		P = dy - dx;
		diff = P - dx;
		for (i = 0; i <= dx; ++i) {
			ssd1306_buffer[x0 + (y0 / 8) * SSD1306_WIDTH] |= ~(1 << y0 % 8);
			if (P < 0) {
				P += dy;
				x0 += addx;
			} else {
				P += diff;
				x0 += addx;
				y0 += addy;
			}
		}
	} else {
		dx *= 2;
		P = dx - dy;
		diff = P - dy;
		for (i = 0; i <= dy; ++i) {
			ssd1306_buffer[x0 + (y0 / 8) * SSD1306_WIDTH] |= ~(1 << y0 % 8);
			if (P < 0) {
				P += dx;
				y0 += addy;
			} else {
				P += diff;
				x0 += addx;
				y0 += addy;
			}
		}
	}
}

void ssd1306_circle(uint8_t x, uint8_t y, uint8_t radius)
{
	int16_t a, b, P;
	a = 0;
	b = radius;
	P = 1 - radius;
	do {
		ssd1306_buffer[(x + a)+ ((y + b) / 8) * SSD1306_WIDTH] |= ~(1 << (y + b) % 8);
		ssd1306_buffer[(x + b)+ ((y + a) / 8) * SSD1306_WIDTH] |= ~(1 << (y + a) % 8);
		ssd1306_buffer[(x - a)+ ((y + b) / 8) * SSD1306_WIDTH] |= ~(1 << (y + b) % 8);
		ssd1306_buffer[(x - b)+ ((y + a) / 8) * SSD1306_WIDTH] |= ~(1 << (y + a) % 8);
		ssd1306_buffer[(x + b)+ ((y - a) / 8) * SSD1306_WIDTH] |= ~(1 << (y - a) % 8);
		ssd1306_buffer[(x + a)+ ((y + b) / 8) * SSD1306_WIDTH] |= ~(1 << (y + b) % 8);
		ssd1306_buffer[(x - a)+ ((y - b) / 8) * SSD1306_WIDTH] |= ~(1 << (y - b) % 8);
		ssd1306_buffer[(x - b)+ ((y - a) / 8) * SSD1306_WIDTH] |= ~(1 << (y - a) % 8);
		if (P < 0)
			P += 3 + 2 * a++;
		else
			P += 5 + 2 * (a++ - b--);
	} while (a <= b);
}

void ssd1306_char(uint8_t x, uint8_t y, uint8_t c, struct ssd1306_font font)
{
	uint8_t col, column[font.width];
	// Check if the requested character is available
	if ((c >= font.firstchar) && (c <= font.lastchar)) {
		// Retrieve appropriate columns from font data
		for (col = 0; col < font.width; col++) {
			column[col] = font.table[((c - 32) * font.width) + col]; // Get first column of appropriate character
		}
	} else {
		// Requested character is not available in this font ... send a space instead
		for (col = 0; col < font.width; col++) {
			column[col] = 0xFF; // Send solid space
		}
	}
	// Render each column
	uint16_t xoffset, yoffset;
	for (xoffset = 0; xoffset < font.width; xoffset++) {
		for (yoffset = 0; yoffset < (font.height + 1); yoffset++) {
			uint8_t bit = 0x00;
			bit = (column[xoffset] << (8 - (yoffset + 1))); // Shift current row bit left
			bit = (bit >> 7); // Shift current row but right (results in 0x01 for black, and 0x00 for white)
			if (bit) {
				ssd1306_pixel(x + xoffset, y + yoffset,1);
			} else 
				ssd1306_pixel(x + xoffset, y + yoffset,0);
		}
	}
}

void ssd1306_clean_string(uint8_t y, struct ssd1306_font font)
{
	// Render each column
	uint16_t xoffset, yoffset;
	for (xoffset = 0; xoffset < SSD1306_WIDTH; xoffset++) {
		for (yoffset = 0; yoffset < (font.height + 1); yoffset++) {
			ssd1306_pixel(xoffset, y + yoffset, 0);
		}
	}
}



void ssd1306_string_font(uint8_t x, uint8_t y, const char *text, struct ssd1306_font font)
{
	uint8_t i;
	
	for (i = 0; i < strlen(text); i++) {
		ssd1306_char(x + (i * (font.width + 1)), y, text[i], font);
	}
}

void ssd1306_string(uint8_t x, uint8_t y, const char *text)
{
	ssd1306_string_font (x,y,text,font8x8);
}

void ssd1306_new_string(uint8_t x, uint8_t y, const char *text)
{
	ssd1306_clean_string (y, font8x8);
	ssd1306_string_font (x,y,text,font8x8);
}