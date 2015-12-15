#include <stdint.h>

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

#define SSD1306_WIDTH		128
#define SSD1306_HEIGHT		64
#define SSD1306_BUFFER_SIZE    (SSD1306_WIDTH * SSD1306_HEIGHT / 8)


static int ssd1306_i2c;
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

static void ssd1306_cmd1(uint8_t c)
{
	/*   uint8_t buf[2] ;
	   buf[0] = SSD_COMMAND_MODE ; 
	   buf[1] = c;
	   i2c_xfer(ssd->i2c_dev, sizeof(buf), buf, 0, NULL);
	 */
}

static void ssd1306_cmd2(uint8_t c0, uint8_t c1)
{
	/*
	  uint8_t buf[3];
	   buf[0] = SSD_COMMAND_MODE ;
	   buf[1] = c0;
	   buf[2] = c1;
	   i2c_xfer(ssd->i2c_dev, sizeof(buf), buf, 0, NULL);
	 */
}

static void ssd1306_cmd3(uint8_t c0, uint8_t c1, uint8_t c2)
{
	/*
	   uint8_t buf[4] ;
	   buf[0] = SSD_COMMAND_MODE; 
	   buf[1] = c0;
	   buf[2] = c1;
	   buf[3] = c2;
	   i2c_xfer(ssd->i2c_dev, sizeof(buf), buf, 0, NULL);
	 */
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
	uint8_t buf[17];
	buf[0] = SSD_DATA_MODE;
	for (i = 0; i < SSD1306_BUFFER_SIZE; i += 16) {
		for (x = 1; x <= 16; x++)
			buf[x] = *p++;
		//i2c_xfer(ssd->i2c_dev, sizeof(buf), buf, 0, NULL);
	}
}

void ssd1306_clear()
{
	memset(ssd1306_buffer, 0, SSD1306_BUFFER_SIZE);
}

void ssd1306_set_pixel(int16_t x, int16_t y, uint16_t color)
{
	uint8_t *p = ssd1306_buffer + (x + (y / 8) * SSD1306_WIDTH);
	if (color)
		*p |= 1 << (y % 8);
	else
		*p &= ~(1 << (y % 8));
}


